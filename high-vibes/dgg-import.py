#!/usr/bin/env python3
# dgg-import.py
# Import a GeoTIFF into a DGGS Data Store by sampling the GeoTIFF at DGGS sub-zone centroids
# and writing results into the store via DGGSDataStore.write_zone_batch.

# Usage:
#  python dgg-import.py input.tif --dggrs IVEA4R --data-root data --collection mycol

#Notes:
# - Only a single field is supported (first name from --fields, default "field0").
# - Default import level is computed from the raster native resolution via DGGRS.getLevelFromPixelsAndExtent.
# - The finest root level = data_level - depth (depth defaults to dggrs.get64KDepth()).
# - Batching mirrors dgg-fetch: roots are grouped and written with write_zone_batch.
from __future__ import annotations
from dggal import *
from typing import List, Dict, Any, Optional
import argparse
import json
import os
import sys
import math

# initialize dggal runtime
app = Application(appGlobals=globals()); pydggal_setup(app)

import rasterio
from rasterio.warp import transform
from rasterio.sample import sample_gen
from rasterio.transform import Affine

from dggsStore.store import *
from dggsStore.customDepths import *

# ---------------------------------------------------------------------------
# Low-level sampling / coordinate helpers
# ---------------------------------------------------------------------------

def _coords_for_centroids(centroids, raster_crs: str) -> List[tuple]:
   # centroids: iterable of DGGRSZone centroid objects with .lon/.lat
   pts: List[tuple] = []
   is4326 = raster_crs == "EPSG:4326"
   for gp in centroids:
      if is4326:
         pts.append((gp.lon, gp.lat))
      else:
         xs, ys = transform("EPSG:4326", raster_crs, [gp.lon], [gp.lat])
         pts.append((xs[0], ys[0]))
   return pts

def _sample_values_for_centroids(ds, coords: List[tuple], overview_factor: Optional[int]) -> List[Any]:
   # Sample values for coords. If overview_factor > 1, read that overview via out_shape
   vals: List[Any] = []
   if not coords:
      return vals

   if overview_factor and overview_factor > 1:
      factor = overview_factor
      out_h = max(1, int(ds.height / factor))
      out_w = max(1, int(ds.width / factor))
      arr = ds.read(1, out_shape=(out_h, out_w))
      inv_transform = ~ds.transform
      scale_x = ds.width / out_w
      scale_y = ds.height / out_h

      for (x, y) in coords:
         colf, rowf = inv_transform * (x, y)
         col_o = int(colf / scale_x)
         row_o = int(rowf / scale_y)
         if 0 <= row_o < out_h and 0 <= col_o < out_w:
            v = arr[row_o, col_o]
            vals.append(None if v is None else float(v))
         else:
            vals.append(None)
      return vals

   # fallback: full-resolution sampling
   for arr in ds.sample(coords):
      vals.append(None if arr is None else float(arr[0]))
   return vals

# ---------------------------------------------------------------------------
# Overview selection helpers (meters-per-subzone based)
# ---------------------------------------------------------------------------

def _meters_per_degree_at_lat(lat_deg: float) -> float:
   # approximate meters per degree of longitude at given latitude
   lat_rad = math.radians(lat_deg)
   return 111320.0 * math.cos(lat_rad)

def _choose_overview_factor_for_level(ds, dggrs, zone, root_level: int, relative_depth: int) -> int:
   # Choose the overview factor whose effective meters/pixel best matches the DGGRS
   # meters-per-subzone for (root_level, relative_depth).
   target_m_per_subzone = dggrs.getMetersPerSubZoneFromLevel(root_level, relative_depth)

   # dataset base pixel size in dataset CRS units (pixel width)
   base_px = abs(ds.transform.a)

   # determine whether raster CRS is geographic (degrees) or projected (meters)
   is_geographic = (ds.crs is None) or (ds.crs.to_string() == "EPSG:4326")

   # pick a representative latitude for conversion using the DGGRS centroid API
   lat = dggrs.getZoneWGS84Centroid(zone).lat

   # convert base pixel to meters if raster CRS is geographic
   if is_geographic:
      meters_per_degree = _meters_per_degree_at_lat(lat)
      base_px_m = base_px * meters_per_degree
   else:
      base_px_m = base_px

   # available overview factors (include 1)
   overviews = ds.overviews(1) if ds.count >= 1 else []
   candidates = [1] + list(overviews)

   # choose candidate whose effective meters/pixel (base_px_m * factor) is closest to target
   best = 1
   best_diff = float("inf")
   for f in candidates:
      eff_px_m = base_px_m * f
      diff = abs(eff_px_m - target_m_per_subzone)
      if diff < best_diff:
         best_diff = diff
         best = f

   # progress print for overview selection
   print(f"[OVERVIEW] zone={dggrs.getZoneTextID(zone)} root_level={root_level} depth={relative_depth} "
         f"target_m_per_subzone={target_m_per_subzone:.3f} base_px_m={base_px_m:.6f} chosen_factor={best}",
         flush=True)

   return best

# ---------------------------------------------------------------------------
# Canonical blob construction helpers
# ---------------------------------------------------------------------------

def _make_depth_obj(depth: int, count_centroids: int, sampled_values: Any) -> Dict[str, Any]:
   return {
      "depth": depth,
      "shape": {"count": count_centroids, "subZones": count_centroids},
      "data": sampled_values
   }

# ---------------------------------------------------------------------------
# Sampling and aggregation builders (produce Dict[int, Dict])
# ---------------------------------------------------------------------------

def _sample_depth_obj_for_zone(store, ds, raster_crs: str, dggrs, zone, data_level: int,
   depth: int, use_overviews: bool) -> Optional[Dict[str, Any]]:
   root_level = dggrs.getZoneLevel(zone)
   if data_level - root_level < 0:
      return None

   centroids = dggrs.getSubZoneWGS84Centroids(zone, depth) or []
   count_centroids = len(centroids)

   coords = _coords_for_centroids(centroids, raster_crs)
   Instance.delete(centroids)
   if not coords:
      print(f"[SAMPLE] zone={dggrs.getZoneTextID(zone)} depth={depth} no coords after transform, skipping", flush=True)
      return None

   # compute overview factor appropriate for this root level using DGGRS meters-per-subzone
   overview_factor: Optional[int] = None
   if use_overviews:
      factor = _choose_overview_factor_for_level(ds, dggrs, zone, root_level, depth)
      if factor and factor > 1:
         overview_factor = factor

   # progress print before sampling
   print(f"[SAMPLE] zone={dggrs.getZoneTextID(zone)} root_level={root_level} depth={depth} "
         f"centroids={count_centroids} overview_factor={overview_factor}", flush=True)

   sampled_values = _sample_values_for_centroids(ds, coords, overview_factor)
   if len(sampled_values) != count_centroids:
      print(f"[SAMPLE] zone={dggrs.getZoneTextID(zone)} depth={depth} count_mismatch sampled={len(sampled_values)} expected={count_centroids}, skipping",
            flush=True)
      return None

   print(f"[SAMPLE] zone={dggrs.getZoneTextID(zone)} depth={depth} sampled_ok", flush=True)
   return _make_depth_obj(depth, count_centroids, sampled_values)

def _build_sample_blobs(store, ds, raster_crs: str, dggrs, dggrs_uri: str,
   zones: List, data_level: int, depth: int, fields: List[str],
   use_overviews: bool) -> Dict[int, Dict[str, Any]]:
   blobs: Dict[int, Dict[str, Any]] = {}
   for zone in zones:
      fields_map: Dict[str, List[Dict[str, Any]]] = {}
      for field_name in fields:
         depth_obj = _sample_depth_obj_for_zone(store, ds, raster_crs, dggrs, zone, data_level, depth, use_overviews)
         if not depth_obj:
            continue
         fields_map.setdefault(field_name, []).append(depth_obj)

      if not fields_map:
         print(f"[BUILD] zone={dggrs.getZoneTextID(zone)} no fields_map produced, skipping", flush=True)
         continue

      zone_text = dggrs.getZoneTextID(zone)
      blobs[int(zone)] = make_dggs_json_blob(dggrs_uri, zone_text, fields_map)
      print(f"[BUILD] zone={zone_text} blob_ready fields={list(fields_map.keys())}", flush=True)
   return blobs

def _build_agg_blobs(store, dggrs, dggrs_uri: str, zones: List, depth: int, fields: List[str]) -> Dict[int, Dict[str, Any]]:
   blobs: Dict[int, Dict[str, Any]] = {}
   for zone in zones:
      # aggregate_zone_at_depth returns a fields_map (Mapping[str, List[ValueEntry]])
      fields_map = aggregate_zone_at_depth(store, zone, depth)
      if not fields_map:
         print(f"[AGG] zone={dggrs.getZoneTextID(zone)} aggregate returned empty, skipping", flush=True)
         continue

      zone_text = dggrs.getZoneTextID(zone)
      blobs[int(zone)] = make_dggs_json_blob(dggrs_uri, zone_text, fields_map)
      print(f"[AGG] zone={zone_text} agg_blob_accepted", flush=True)
   return blobs

# ---------------------------------------------------------------------------
# Coordinator: single write boundary
# ---------------------------------------------------------------------------

def _process_batch(store, ds, raster_crs: str, dggrs, dggrs_uri: str,
   base_zone, batch_zones: List, base_ancestors: List,
   data_level: int, depth: int, field_name: str, use_overviews: bool, aggregate: bool) -> int:
   # Keep compatibility with existing single-field call sites
   fields = [field_name]

   if not batch_zones:
      print("[BATCH] empty batch, skipping", flush=True)
      return 0

   print(f"[BATCH] processing batch base_zone={dggrs.getZoneTextID(base_zone)} roots={len(batch_zones)} aggregate={aggregate}", flush=True)

   if aggregate:
      entries = _build_agg_blobs(store, dggrs, dggrs_uri, batch_zones, depth, fields)
   else:
      entries = _build_sample_blobs(store, ds, raster_crs, dggrs, dggrs_uri,
         batch_zones, data_level, depth, fields, use_overviews)

   if not entries:
      print("[BATCH] no entries produced for this batch, skipping write", flush=True)
      return 0

   print(f"[BATCH] writing {len(entries)} entries to store for base_zone={dggrs.getZoneTextID(base_zone)}", flush=True)
   store.write_zone_batch(
      base_zone=base_zone,
      entries=entries,
      base_ancestor_list=base_ancestors,
      precompressed=False
   )
   print(f"[BATCH] write complete for base_zone={dggrs.getZoneTextID(base_zone)} wrote={len(entries)}", flush=True)
   return len(entries)

# ---------------------------------------------------------------------------
# Main import function (control flow preserved)
# ---------------------------------------------------------------------------

def import_geotiff(tiff_path: str, dggrs_name: str, data_root: str = "data",
   collection_id: str | None = None, level: int | None = None, depth: int | None = None,
   fields: List[str] | None = None, batch_size: int = 32, groupSize: int = 5,
   aggregate: bool | None = None):
   if collection_id is None:
      collection_id = os.path.splitext(os.path.basename(tiff_path))[0]

   ds = rasterio.open(tiff_path)
   raster_crs = ds.crs.to_string() if ds.crs is not None else "EPSG:4326"

   has_overviews = False
   if ds.count >= 1:
      has_overviews = bool(ds.overviews(1))

   if aggregate is None:
      aggregate = not has_overviews
   print(f"[IMPORT] has_overviews={has_overviews} aggregate={aggregate}", flush=True)

   use_overviews_for_sampling = (not aggregate) and has_overviews

   dggrs_init = globals().get(dggrs_name)
   if dggrs_init is None:
      print("Unsupported DGGRS:", dggrs_name, flush=True)
      return
   dggrs = dggrs_init()

   if level is None:
      b = ds.bounds
      if raster_crs != "EPSG:4326":
         xs, ys = transform(raster_crs, "EPSG:4326", [b.left, b.right], [b.bottom, b.top])
         min_lon, max_lon = min(xs[0], xs[1]), max(xs[0], xs[1])
         min_lat, max_lat = min(ys[0], ys[1]), max(ys[0], ys[1])
      else:
         min_lon, max_lon = min(b.left, b.right), max(b.left, b.right)
         min_lat, max_lat = min(b.bottom, b.top), max(b.bottom, b.top)
      level = dggrs.getLevelFromPixelsAndExtent(GeoExtent((min_lat, min_lon), (max_lat, max_lon)),
         Point(ds.width, ds.height), 0)
      print(f"[IMPORT] computed data level from raster: {level}", flush=True)

   if depth is None:
      depth = dggrs.get64KDepth()
      print(f"[IMPORT] using default depth (get64KDepth) = {depth}", flush=True)

   data_level = level
   deepest_root_level = max(0, data_level - depth)

   if not fields:
      fields = ["field0"]
   field_name = fields[0]

   coll_info = {
      "dggrs": dggrs_name, "maxRefinementLevel": data_level, "depth": depth,
      "groupSize": groupSize, "title": collection_id, "description": collection_id, "version": "1.0"
   }
   dggrs_uri = f"[ogc-dggrs:{dggrs_name}]"

   base = os.path.join(data_root, collection_id)
   os.makedirs(base, exist_ok=True)
   with open(os.path.join(base, "collection.json"), "w", encoding="utf-8") as fh:
      json.dump(coll_info, fh, indent=2)
   print(f"[IMPORT] Wrote collection config to {os.path.join(base, 'collection.json')}", flush=True)

   store = DGGSDataStore(data_root, collection_id, config=coll_info)
   dggrs = store.dggrs
   max_base_level = store._base_level_for_root(deepest_root_level)
   print(
      f"[IMPORT] Computed levels: data_level={data_level} depth={depth} finest_root_level={deepest_root_level} "
      f"max_base_level={max_base_level} batch_size={batch_size}", flush=True
   )
   print(f"[DIAG] using groupSize={groupSize} (recommended default is 5)", flush=True)

   pkg_index = 0
   total_written = 0
   finest_level_done = False

   for root_level in range(deepest_root_level, -1, -1):
      base_level = store._base_level_for_root(root_level)
      up_to = False
      for base_zone, base_ancestors in store.iter_bases(base_level, up_to=up_to):
         pkg_index += 1
         base_text = dggrs.getZoneTextID(base_zone)
         print(f"[LEVEL {root_level}] #{pkg_index}: base_zone={base_text}", flush=True)

         base_level = dggrs.getZoneLevel(base_zone)
         package_group_levels = store.group0Size if base_level == 0 else store.groupSize
         package_max = base_level + package_group_levels - 1
         max_root_level = root_level
         if base_level > max_root_level:
            print(f"[SKIP] package base {base_text} (base_level={base_level}) deeper than target {max_root_level}",
               flush=True)
            continue

         print(
            f"[LEVEL {root_level}] #{pkg_index}: iterating roots up to level {max_root_level} "
            f"(base_level={base_level}, package_max={package_max}, target={max_root_level})",
            flush=True
         )
         roots_iter = store.iter_roots_for_base(base_zone, max_root_level, up_to=up_to)

         batch_num = 0
         batch_zones: List = []
         for zone in roots_iter:
            root_level = dggrs.getZoneLevel(zone)
            if data_level - root_level < 0:
               print(f"[IMPORT] skipping root {dggrs.getZoneTextID(zone)} because data_level < root_level",
                  flush=True)
               continue
            batch_zones.append(zone)
            if len(batch_zones) >= batch_size:
               batch_num += 1
               print(f"[LEVEL {root_level}] #{pkg_index} BATCH {batch_num}: handling {len(batch_zones)} roots",
                  flush=True)
               written = _process_batch(
                  store, ds, raster_crs, dggrs, dggrs_uri, base_zone, batch_zones,
                  base_ancestors, data_level, depth, field_name, use_overviews_for_sampling,
                  aggregate and root_level < deepest_root_level
               )
               total_written += written
               batch_zones = []

         if batch_zones:
            batch_num += 1
            print(f"[LEVEL {root_level}] #{pkg_index} BATCH {batch_num}: handling {len(batch_zones)} roots",
               flush=True)
            written = _process_batch(
               store, ds, raster_crs, dggrs, dggrs_uri, base_zone, batch_zones,
               base_ancestors, data_level, depth, field_name, use_overviews_for_sampling,
               aggregate and root_level < deepest_root_level
            )
            total_written += written

         print(f"[LEVEL {root_level}] #{pkg_index} complete; total_written so far={total_written}", flush=True)

      if aggregate and not finest_level_done:
         finest_level_done = True
         store._compute_property_key()

   ds.close()
   print(f"[IMPORT] complete; total written={total_written}", flush=True)

# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

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
   )

if __name__ == "__main__":
   main()
