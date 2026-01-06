#!/usr/bin/env python3
# dgg-fg
# CLI for DGGS-JSON-FG operations (tiledg and togeo; reproj, fix, clip, tile, clipdg)

import sys
import os
import json
import argparse
from dggal import *
import glob
from typing import Iterable, Dict, Any, List, Sequence
import shapely
import gc
from shapely.geometry import shape, mapping, Polygon, MultiPolygon, LineString, MultiLineString, Point, MultiPoint, GeometryCollection

from fg.reproj import *
from fg.fix_topology_5x6 import fix_feature_collection_5x6_topology, fix_geojson_file_5x6_topology
from fg.dggsJSONFG import write_dggs_json_fg_to_file, read_dggs_json_fg_file
from fg.dgToGeoMulti import togeo_multi_mode
from fg.clippingShapely import clip_featurecollection_to_zone

GRID_SIZE=1e-2

# initialize DGGAL (assumes Application and pydggal_setup are available)
app = Application(appGlobals=globals())
pydggal_setup(app)

def _prepare_input_pipeline(input_path: str, dggrs_name: str, ico: bool, skip_reproj: bool, skip_fix: bool):
   # Load input GeoJSON, optionally reproject and optionally run 5x6 topology fix.
   # Returns a GeoJSON FeatureCollection ready for clipping.

   # load input (assume GeoJSON FeatureCollection)
   src = geojson_load(input_path)

   # reproj step (unless skipped)
   if not skip_reproj:
      proj = instantiate_projection_for_dggrs_name(dggrs_name)
      print("Reprojecting to native CRS of", dggrs_name, "...")
      src = reproject_featurecollection(src, proj, ico=ico)

   # topology fix step (unless skipped)
   if not skip_fix:
      print("Fixing reprojected features topology...")
      src = fix_feature_collection_5x6_topology(src)

   return src

def main(argv):
   p = argparse.ArgumentParser(prog="dgg-fg")
   sub = p.add_subparsers(dest="cmd", required=True)

   # reproj subcommand
   r = sub.add_parser("reproj", help="Reproject GeoJSON into DGGRS native CRS")
   r.add_argument("--dggrs", required=True)
   r.add_argument("--ico", action="store_true", default=False, help="apply icosahedron-net transform after projection")
   r.add_argument("--input", dest="input_opt", default=None)
   r.add_argument("--output", dest="output_opt", default=None)
   r.add_argument("input_pos", nargs="?", default=None)
   r.add_argument("output_pos", nargs="?", default=None)

   # fix subcommand
   r = sub.add_parser("fix", help="Fix topology of DGGRS native CRS GeoJSON")
   r.add_argument("--dggrs", required=True)
   r.add_argument("--ico", action="store_true", default=False, help="use icosahedron-net CRS")
   r.add_argument("--input", dest="input_opt", default=None)
   r.add_argument("--output", dest="output_opt", default=None)
   r.add_argument("input_pos", nargs="?", default=None)
   r.add_argument("output_pos", nargs="?", default=None)

   # clip subcommand
   c = sub.add_parser("clip", help="Clip GeoJSON to a single DGGRS zone")
   c.add_argument("--dggrs", required=True)
   c.add_argument("--zone", required=True)
   c.add_argument("--refined", action="store_true", default=False)
   c.add_argument("--ico", action="store_true", default=False, help="use icosahedron-net CRS for zone geometry and reprojection")
   c.add_argument("--skip-reproj", action="store_true", default=False, help="assume input is already reprojected (skip reproj step)")
   c.add_argument("--skip-fix", action="store_true", default=False, help="assume input is already reproj'ed and topology-fixed (skip reproj and fix)")
   c.add_argument("--input", dest="input_opt", default=None)
   c.add_argument("--output", dest="output_opt", default=None)
   c.add_argument("input_pos", nargs="?", default=None)
   c.add_argument("output_pos", nargs="?", default=None)

   # clipdg subcommand (DGGS-JSON-FG output for single zone)
   cd = sub.add_parser("clipdg", help="Clip GeoJSON to a single DGGRS zone and write DGGS-JSON-FG")
   cd.add_argument("--dggrs", required=True)
   cd.add_argument("--zone", required=True)
   cd.add_argument("--refined", action="store_true", default=False)
   cd.add_argument("--ico", action="store_true", default=False, help="use icosahedron-net CRS for zone geometry and reprojection")
   cd.add_argument("--skip-reproj", action="store_true", default=False, help="assume input is already reprojected (skip reproj step)")
   cd.add_argument("--skip-fix", action="store_true", default=False, help="assume input is already reproj'ed and topology-fixed (skip reproj and fix)")
   cd.add_argument("--input", dest="input_opt", default=None)
   cd.add_argument("--output", dest="output_opt", default=None)
   cd.add_argument("input_pos", nargs="?", default=None)
   cd.add_argument("output_pos", nargs="?", default=None)
   cd.add_argument("--depth", type=int, default=None, help="depth for sub-zone index resolution (default: 2 * ~64K sub-zones depth)")

   # tile (multiclip) subcommand
   t = sub.add_parser("tile", help="Clip GeoJSON to all zones at a refinement level")
   t.add_argument("--dggrs", required=True)
   t.add_argument("--level", type=int, default=0, help="refinement level (default 0)")
   t.add_argument("--bbox", nargs=4, type=float, metavar=("LAT0","LON0","LAT1","LON1"),
                  help="WGS84 bounding box: lat0 lon0 lat1 lon1 (degrees). Default: wholeWorld")
   t.add_argument("--outdir", default="zone_tiles", help="output directory (default: zone_tiles)")
   t.add_argument("--refined", action="store_true", default=False,
                  help="pass refined=True to clip_featurecollection_to_zone")
   t.add_argument("--ico", action="store_true", default=False, help="use icosahedron-net CRS for zone geometry and reprojection")
   t.add_argument("--skip-reproj", action="store_true", default=False, help="assume input is already reprojected (skip reproj step)")
   t.add_argument("--skip-fix", action="store_true", default=False, help="assume input is already reproj'ed and topology-fixed (skip reproj and fix)")
   t.add_argument("--input", dest="input_opt", default=None)
   t.add_argument("input_pos", nargs="?", default=None)

   # tiledg subcommand (DGGS-JSON-FG output for tiled zones)
   td = sub.add_parser("tiledg", help="Clip GeoJSON to all zones at a refinement level and write DGGS-JSON-FG files")
   td.add_argument("--dggrs", required=True)
   td.add_argument("--level", type=int, default=0, help="refinement level (default 0)")
   td.add_argument("--bbox", nargs=4, type=float, metavar=("LAT0","LON0","LAT1","LON1"),
                   help="WGS84 bounding box: lat0 lon0 lat1 lon1 (degrees). Default: wholeWorld")
   td.add_argument("--outdir", default="zone_tiles_dg", help="output directory (default: zone_tiles_dg)")
   td.add_argument("--refined", action="store_true", default=False,
                   help="pass refined=True to clip_featurecollection_to_zone")
   td.add_argument("--ico", action="store_true", default=False, help="use icosahedron-net CRS for zone geometry and reprojection")
   td.add_argument("--skip-reproj", action="store_true", default=False, help="assume input is already reprojected (skip reproj step)")
   td.add_argument("--skip-fix", action="store_true", default=False, help="assume input is already reproj'ed and topology-fixed (skip reproj and fix)")
   td.add_argument("--input", dest="input_opt", default=None)
   td.add_argument("input_pos", nargs="?", default=None)
   td.add_argument("--depth", type=int, default=None, help="depth for sub-zone index resolution (default: 2 * ~64K sub-zones depth)")

   # togeo subcommand
   #tg = sub.add_parser("togeo", help="Convert DGGS-JSON-FG to GeoJSON using DGGAL centroids")
   #tg.add_argument("input", help="Input DGGS-JSON-FG file")
   #tg.add_argument("output", help="Output GeoJSON file")
   tg = sub.add_parser("togeo", help="Convert DGGS-JSON-FG to GeoJSON")
   tg.add_argument("paths", nargs="+", help="Input files/dirs/glob/.lst and output file (last argument is output)")
   tg.add_argument("--grid-size", dest="grid_size", default=GRID_SIZE)

   args = p.parse_args(argv)
   skip_reproj = getattr(args, 'skip_reproj', False) or getattr(args, 'skip_fix', False)

   if args.cmd == "reproj":
      input_path = args.input_opt if args.input_opt is not None else args.input_pos
      output_path = args.output_opt if args.output_opt is not None else args.output_pos
      if input_path is None or output_path is None:
         raise SystemExit("usage: dgg-fg reproj --dggrs <DGGRS> <input> <output>  (or use --input/--output)")
      dggrs = get_dggrs(args.dggrs)
      proj = instantiate_projection_for_dggrs_name(args.dggrs)
      src = geojson_load(input_path)
      # reproject_featurecollection returns a GeoJSON FeatureCollection
      out_fc = reproject_featurecollection(src, proj, ico=args.ico)
      geojson_dump(out_fc, output_path)
      return

   if args.cmd == "fix":
      input_path = args.input_opt if args.input_opt is not None else args.input_pos
      output_path = args.output_opt if args.output_opt is not None else args.output_pos
      if input_path is None or output_path is None:
         raise SystemExit("usage: dgg-fg fix --dggrs <DGGRS> <input> <output>  (or use --input/--output)")
      dggrs = get_dggrs(args.dggrs)
      proj = instantiate_projection_for_dggrs_name(args.dggrs)
      fix_geojson_file_5x6_topology(input_path, output_path)
      return

   if args.cmd == "clip":
      input_path = args.input_opt if args.input_opt is not None else args.input_pos
      output_path = args.output_opt if args.output_opt is not None else args.output_pos
      if input_path is None or output_path is None:
         raise SystemExit("usage: dgg-fg clip --dggrs <DGGRS> --zone <ZONEID> <input> <output>  (or use --input/--output)")
      dggrs = get_dggrs(args.dggrs)
      zone = dggrs.getZoneFromTextID(args.zone)
      if zone == nullZone:
         print("Invalid zone identifier", args.zone)
         return

      # prepare input: reproj + fix unless skipped
      src = _prepare_input_pipeline(input_path, args.dggrs, args.ico, skip_reproj, args.skip_fix)

      out_fc, feature_entry_exit_indices = clip_featurecollection_to_zone(src, dggrs, zone, refined=args.refined, ico=args.ico)
      geojson_dump(out_fc, output_path)
      return

   if args.cmd == "clipdg":
      input_path = args.input_opt if args.input_opt is not None else args.input_pos
      output_path = args.output_opt if args.output_opt is not None else args.output_pos
      if input_path is None or output_path is None:
         raise SystemExit("usage: dgg-fg clipdg --dggrs <DGGRS> --zone <ZONEID> <input> <output>  (or use --input/--output)")
      dggrs = get_dggrs(args.dggrs)
      zone = dggrs.getZoneFromTextID(args.zone)
      if zone == nullZone:
         print("Invalid zone identifier", args.zone)
         return

      src = _prepare_input_pipeline(input_path, args.dggrs, args.ico, skip_reproj, args.skip_fix)

      depth_val = args.depth if args.depth is not None else 2 * dggrs.get64KDepth()
      out_fc, feature_entry_exit_indices = clip_featurecollection_to_zone(src, dggrs, zone, refined=args.refined, ico=args.ico)
      write_dggs_json_fg_to_file(out_fc, feature_entry_exit_indices, output_path, dggrs, zone, depth_val)
      return

   if args.cmd == "tile":
      input_path = args.input_opt if args.input_opt is not None else args.input_pos
      if input_path is None:
         raise SystemExit("usage: dgg-fg tile --dggrs <DGGRS> [--level N] [--bbox lat0 lon0 lat1 lon1] [--outdir DIR] --input <input.geojson>")
      dggrs = get_dggrs(args.dggrs)

      # bounding box: either wholeWorld or GeoExtent((lat0,lon0),(lat1,lon1))
      if args.bbox is None:
         boundingBox = wholeWorld
      else:
         lat0, lon0, lat1, lon1 = args.bbox
         boundingBox = GeoExtent((lat0, lon0), (lat1, lon1))

      refinementLevel = args.level
      zones = dggrs.listZones(refinementLevel, boundingBox)

      outdir = args.outdir
      os.makedirs(outdir, exist_ok=True)

      # prepare input once (reproj + fix unless skipped)
      src = _prepare_input_pipeline(input_path, args.dggrs, args.ico, skip_reproj, args.skip_fix)

      for zone in zones:
         textid = dggrs.getZoneTextID(zone)
         print("Clipping to zone", textid)
         out_fc, feature_entry_exit_indices = clip_featurecollection_to_zone(src, dggrs, zone, ico=args.ico)   #refined=args.refined,
         out_path = os.path.join(outdir, f"{textid}.geojson")
         geojson_dump(out_fc, out_path)

      return

   if args.cmd == "tiledg":
      input_path = args.input_opt if args.input_opt is not None else args.input_pos
      if input_path is None:
         raise SystemExit("usage: dgg-fg tiledg --dggrs <DGGRS> [--level N] [--bbox lat0 lon0 lat1 lon1] [--outdir DIR] --input <input.geojson>")
      dggrs = get_dggrs(args.dggrs)

      # bounding box: either wholeWorld or GeoExtent((lat0,lon0),(lat1,lon1))
      if args.bbox is None:
         boundingBox = wholeWorld
      else:
         lat0, lon0, lat1, lon1 = args.bbox
         boundingBox = GeoExtent((lat0, lon0), (lat1, lon1))

      refinementLevel = args.level
      zones = dggrs.listZones(refinementLevel, boundingBox)

      outdir = args.outdir
      os.makedirs(outdir, exist_ok=True)

      # prepare input once (reproj + fix unless skipped)
      src = _prepare_input_pipeline(input_path, args.dggrs, args.ico, skip_reproj, args.skip_fix)

      depth_val = args.depth if args.depth is not None else 2 * dggrs.get64KDepth()

      for zone in zones:
         textid = dggrs.getZoneTextID(zone)
         print("Clipping to zone", textid)

         out_fc, feature_entry_exit_indices = clip_featurecollection_to_zone(src, dggrs, zone, refined=args.refined, ico=args.ico)
         out_path = os.path.join(outdir, f"{textid}.dggs.json")

         write_dggs_json_fg_to_file(out_fc, feature_entry_exit_indices, out_path, dggrs, zone, depth_val)

      return

   if args.cmd == "togeo":
      paths = args.paths
      if len(paths) < 2:
         p.error("provide at least one input and one output path")

      *input_args, output_path = paths

      # legacy single-file fast path: exactly one input and it is a regular file (not .lst, not dir, not glob)
      single_input = False
      if len(input_args) == 1:
         a = input_args[0]
         if os.path.isfile(a) and not a.lower().endswith(".lst") and not os.path.isdir(a):
            single_input = True

      if single_input:
         obj = read_dggs_json_fg_file(input_args[0])
         with open(output_path, "w", encoding="utf-8") as fh:
            fh.write(pretty_json(obj))
            fh.write("\n")
      else:
         # Level 0 spacing / depth 16 gives ~0.00979 degrees so 0.01 makes sense here
         # Derive from use dggrs.getMetersPerSubZoneFromLevel(root_zone_level, depth) ?
         # multi-file mode: expand inputs (each arg may be a file, dir, glob, or .lst)
         togeo_multi_mode(input_args, output_path, grid_size=float(args.grid_size))
      return 0

if __name__ == "__main__":
   main(sys.argv[1:])
