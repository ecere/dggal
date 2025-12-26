#!/usr/bin/env python3
# dgg-import.py

# Import a GeoTIFF into a DGGS Data Store by sampling the GeoTIFF at DGGS sub-zone centroids
# and writing results into the store via DGGSDataStore.write_zone_batch (same pattern as dgg-fetch).

# Usage:
#  python dgg-import.py input.tif --dggrs IVEA4R --data-root data --collection mycol

#Notes:
# - Only a single field is supported (first name from --fields, default "field0").
# - Default import level is computed from the raster native resolution via DGGRS.getLevelFromPixelsAndExtent.
# - The finest root level = data_level - depth (depth defaults to dggrs.get64KDepth()).
# - Batching mirrors dgg-fetch: roots are grouped and written with write_zone_batch.
from __future__ import annotations
from dggal import *
from typing import List, Dict, Any
import argparse, json, os, sys

# initialize dggal runtime
app = Application(appGlobals=globals()); pydggal_setup(app)

import rasterio
from rasterio.warp import transform
from rasterio.sample import sample_gen

from dggsStore.store import DGGSDataStore

def _coords_for_centroids(centroids, raster_crs: str):
   pts: List[tuple] = []
   is4326 = raster_crs == "EPSG:4326"
   for gp in centroids:
      lon = gp.lon
      lat = gp.lat
      if is4326:
         pts.append((gp.lon, gp.lat))
      else:
         xs, ys = transform("EPSG:4326", raster_crs, [gp.lon], [gp.lat])
         pts.append((xs[0], ys[0]))
   return pts

def build_entry_for_root(dggrs, dggrs_name: str, ds, raster_crs: str, root_zone: DGGRSZone, data_level: int, depth: int, field_name: str) -> Dict[str, Any]:
   root_text = dggrs.getZoneTextID(root_zone)
   root_level = dggrs.getZoneLevel(root_zone)
   rel_depth = depth

   print(f"[IMPORT] processing root {root_text} (root_level={root_level} data_level={data_level} rel_depth={rel_depth})", flush=True)
   if data_level - root_level < 0:
      print(f"[IMPORT] skipping root {root_text} because data_level < root_level", flush=True)
      return {}

   centroids = dggrs.getSubZoneWGS84Centroids(root_zone, rel_depth) or []
   count_centroids = len(centroids)
   print(f"[IMPORT] root {root_text} returned {count_centroids} centroids", flush=True)

   coords = _coords_for_centroids(centroids, raster_crs)

   sampled_values: List[Any] = []
   if coords:
      for arr in sample_gen(ds, coords):
         sampled_values.append(None if arr is None else float(arr[0]))
      print(f"[IMPORT] root {root_text} sampled {len(sampled_values)} values", flush=True)
   else:
      print(f"[IMPORT] root {root_text} has no valid coords to sample", flush=True)

   if len(sampled_values) != count_centroids:
      print(f"[IMPORT] count mismatch for {root_text}: sampled={len(sampled_values)} expected={count_centroids}; skipping root", flush=True)
      return {}

   depth_obj = { "depth": rel_depth, "shape": { "count": count_centroids, "subZones": count_centroids }, "data": sampled_values }
   return {
      "dggrs": dggrs_name,
      "zoneId": root_text,
      "depths": [rel_depth],
      "values": { field_name: [depth_obj] }
   }

def process_batch_import(store: DGGSDataStore, ds, raster_crs: str, dggrs: DGGRS, dggrs_name: str,
   zones: List[int], pkg_index: int, batch_num: int, base_ancestors: List[int], data_level: int, depth: int, field_name: str):
   if not zones:
      print(f"[PACKAGE {pkg_index} BATCH {batch_num}] empty batch, skipping", flush=True)
      return 0

   zone_ids = [int(z) for z in zones]
   root_texts = [dggrs.getZoneTextID(zid) for zid in zone_ids]
   print(f"[PACKAGE {pkg_index} BATCH {batch_num}] preparing entries for {len(zones)} roots", flush=True)
   print(f"[PACKAGE {pkg_index} BATCH {batch_num}] roots in batch: {', '.join(root_texts)}", flush=True)

   entries: Dict[int, Any] = {}
   for zid in zone_ids:
      entry = build_entry_for_root(dggrs, dggrs_name, ds, raster_crs, zid, data_level, depth, field_name)
      if entry:
         entries[zid] = entry

   if not entries:
      print(f"[PACKAGE {pkg_index} BATCH {batch_num}] no entries built, skipping write", flush=True)
      return 0

   sizes = [len(e.get("values", {}).get(field_name, [])) for e in entries.values()]
   total_values = sum(sizes)

   store.write_zone_batch(base_zone=int(zone_ids[0]), entries=entries, base_ancestor_list=[int(x) for x in base_ancestors], precompressed=False)
   written = len(entries)
   print(f"[PACKAGE {pkg_index} BATCH {batch_num}] wrote {written} roots", flush=True)
   return written

def import_geotiff(tiff_path: str, dggrs_name: str, data_root: str = "data", collection_id: str | None = None,
   level: int | None = None, depth: int | None = None, fields: List[str] | None = None, batch_size: int = 32,
   groupSize: int = 5, use_visited: bool = False):
   if collection_id is None:
      collection_id = os.path.splitext(os.path.basename(tiff_path))[0]

   ds = rasterio.open(tiff_path)
   raster_crs = ds.crs.to_string() if ds.crs is not None else "EPSG:4326"

   print(f"[IMPORT] Instantiating DGGSDataStore (minimal) groupSize={groupSize} dggrs={dggrs_name}", flush=True)

   # instantiate DGGRS implementation from globals (user-provided class name)
   dggrs_init = globals().get(dggrs_name)
   if dggrs_init is None:
      print("Unsupported DGGRS:", dggrs_name, flush=True)
      return
   dggrs = dggrs_init()

   # compute data level if not provided
   if level is None:
      bounds = ds.bounds
      if raster_crs != "EPSG:4326":
         xs, ys = transform(raster_crs, "EPSG:4326", [bounds.left, bounds.right], [bounds.bottom, bounds.top])
         min_lon, max_lon = min(xs[0], xs[1]), max(xs[0], xs[1])
         min_lat, max_lat = min(ys[0], ys[1]), max(ys[0], ys[1])
      else:
         min_lon, max_lon = min(bounds.left, bounds.right), max(bounds.left, bounds.right)
         min_lat, max_lat = min(bounds.bottom, bounds.top), max(bounds.bottom, bounds.top)

      extent = GeoExtent((min_lat, min_lon), (max_lat, max_lon))
      pixels = Point(ds.width, ds.height)
      level = dggrs.getLevelFromPixelsAndExtent(extent, pixels, 0)
      print(f"[IMPORT] computed data level from raster: {level}", flush=True)

   if depth is None:
      depth = dggrs.get64KDepth()
      print(f"[IMPORT] using default depth (get64KDepth) = {depth}", flush=True)

   data_level = level
   # deepest_root_level is the global target for root levels
   deepest_root_level = max(0, data_level - depth)

   if not fields:
      fields = ["field0"]
   field_name = fields[0]

   coll_info = {
      "dggrs": dggrs_name,
      "maxRefinementLevel": data_level,
      "depth": depth,
      "groupSize": groupSize,
      "title": collection_id,
      "description": collection_id,
      "version": "1.0"
   }

   base = os.path.join(data_root, collection_id)
   os.makedirs(base, exist_ok=True)
   coll_json_path = os.path.join(base, "collection.json")
   with open(coll_json_path, "w", encoding="utf-8") as fh:
      json.dump(coll_info, fh, indent=2)
   print(f"[IMPORT] Wrote collection config to {coll_json_path}", flush=True)

   # create store with full config so store constructs the DGGRS used for writes
   print(f"[IMPORT] Creating DGGSDataStore with full config", flush=True)
   store = DGGSDataStore(data_root, collection_id, config=coll_info)
   dggrs = store.dggrs

   deepest_root_level = max(0, data_level - depth)
   max_base_level = store._base_level_for_root(deepest_root_level)
   print(f"[IMPORT] Computed levels: data_level={data_level} depth={depth} finest_root_level={deepest_root_level} max_base_level={max_base_level} batch_size={batch_size}", flush=True)
   print(f"[DIAG] using groupSize={groupSize} (recommended default is 5)", flush=True)

   pkg_index = 0
   total_written = 0

   for base_zone, base_ancestors in store.iter_bases(max_base_level, up_to=True, use_visited=use_visited):
      #print("inside for base_zone, base_ancestors in store.iter_bases(max_base_level, up_to=True, use_visited=use_visited):")
      pkg_index += 1
      base_text = dggrs.getZoneTextID(int(base_zone))

      print(f"[PACKAGE] #{pkg_index}: base_zone={base_text}", flush=True)

      base_level = dggrs.getZoneLevel(int(base_zone))
      package_group_levels = store.group0Size if base_level == 0 else store.groupSize
      package_max = base_level + package_group_levels - 1

      # compute max_root_level from global target (data_level - depth)
      max_root_level = max(0, data_level - depth)

      if base_level > max_root_level:
         print(f"[SKIP] package base {base_text} (base_level={base_level}) deeper than target {max_root_level}", flush=True)
         continue

      print(f"[PACKAGE] #{pkg_index}: iterating roots up to level {max_root_level} (base_level={base_level}, package_max={package_max}, target={max_root_level})", flush=True)

      roots_iter = store.iter_roots_for_base(base_zone, max_root_level, up_to=True, use_visited=use_visited)

      batch_num = 0
      batch_zones: List[int] = []
      roots_seen_in_package = 0

      for zone in roots_iter:
         zid = int(zone)
         root_text = dggrs.getZoneTextID(zid)

         root_level = dggrs.getZoneLevel(zid)
         if data_level - root_level < 0:
            print(f"[IMPORT] skipping root {root_text} because data_level < root_level", flush=True)
            continue

         roots_seen_in_package += 1
         print(f"[PACKAGE] #{pkg_index}: seen root #{roots_seen_in_package} -> {root_text} (root_level={root_level})", flush=True)
         batch_zones.append(zid)

         if len(batch_zones) >= batch_size:
            batch_num += 1
            print(f"[PACKAGE] #{pkg_index} BATCH {batch_num}: about to write {len(batch_zones)} roots: {', '.join(dggrs.getZoneTextID(z) for z in batch_zones)}", flush=True)
            written = process_batch_import(store, ds, raster_crs, dggrs, dggrs_name, batch_zones, pkg_index, batch_num, base_ancestors, data_level, depth, field_name)
            total_written += written
            batch_zones = []

      if batch_zones:
         batch_num += 1
         print(f"[PACKAGE] #{pkg_index} BATCH {batch_num}: final write of {len(batch_zones)} roots: {', '.join(dggrs.getZoneTextID(z) for z in batch_zones)}", flush=True)
         written = process_batch_import(store, ds, raster_crs, dggrs, dggrs_name, batch_zones, pkg_index, batch_num, base_ancestors, data_level, depth, field_name)
         total_written += written

      print(f"[PACKAGE] #{pkg_index} complete; total_written so far={total_written}", flush=True)

   ds.close()
   print(f"[IMPORT] complete; total written={total_written}", flush=True)

def main():
   p = argparse.ArgumentParser(prog="dgg-import")
   p.add_argument("tiff", help="input GeoTIFF")
   p.add_argument("--dggrs", default="IVEA4R", help="DGGRS name (class name or string)")
   p.add_argument("--collection", default=None, help="collection id (defaults to filename without ext)")
   p.add_argument("--data-root", default="data", help="data root directory (default data)")
   p.add_argument("--level", type=int, default=None, help="DGGS data level (defaults to raster native resolution)")
   p.add_argument("--depth", type=int, default=None, help="depth (default dggrs.get64KDepth())")
   p.add_argument("--fields", default="field0", help="comma-separated field names (only first used)")
   p.add_argument("--batch-size", type=int, default=32, help="Number of root zones per write batch")
   p.add_argument("--groupSize", type=int, default=5, help="Levels per package (default 5)")
   p.add_argument("--visited", dest="use_visited", action="store_true", help="enable visited protection")
   args = p.parse_args()

   fields = [f.strip() for f in args.fields.split(",") if f.strip()]

   import_geotiff(
      tiff_path=args.tiff,
      dggrs_name=args.dggrs,
      data_root=args.data_root,
      collection_id=args.collection,
      level=args.level,
      depth=args.depth,
      fields=fields,
      batch_size=args.batch_size,
      groupSize=args.groupSize,
      use_visited=args.use_visited,
   )

if __name__ == "__main__":
   main()
