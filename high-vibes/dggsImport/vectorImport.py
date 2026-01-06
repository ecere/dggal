# dggsImport/vectorImport.py
# Import a GeoJSON vector into a DGGS Data Store by:
# - preparing input once (reproj + topology fix)
# - writing collection-level attributes into attributes.sqlite via DGGSDataStore.write_collection_attributes
# - writing a WKBC file for workers via fg.wkbc.write_wkb_collection_file
# - spawning worker processes that read WKBC, clip per-root-zone, call write_dggs_json_fg,
#   convert to UBJSON+gzip and return blobs
# - orchestrator batches returned blobs and calls store.write_zone_batch(..., precompressed=True)

from dggal import *

import io
import os
import json
import gzip
from concurrent.futures import ProcessPoolExecutor, as_completed
from typing import Any, Dict, List, Optional
import ubjson

from dggsStore.store import DGGSDataStore
from fg.reproj import geojson_load, instantiate_projection_for_dggrs_name, reproject_featurecollection
from fg.fix_topology_5x6 import fix_feature_collection_5x6_topology
from fg.clippingShapely import clip_featurecollection_to_zone
from fg.dggsJSONFG import write_dggs_json_fg
from fg.wkbc import write_wkb_collection_file, read_wkb_collection_file

# prepare input pipeline (reproj + fix) executed once in parent
def _prepare_input_pipeline(input_path: str, dggrs_name: str, skip_reproj: bool, skip_fix: bool):
   src = geojson_load(input_path)
   if not skip_reproj:
      proj = instantiate_projection_for_dggrs_name(dggrs_name)
      print("Reprojecting to native CRS of", dggrs_name, "...")
      src = reproject_featurecollection(src, proj)
   if not skip_fix:
      print("Fixing reprojected features topology...")
      src = fix_feature_collection_5x6_topology(src)
   return src

# worker: build a single FG blob for a root zone (picklable top-level)
def _vector_package_worker(wkbc_path: str,
   zone_id: int,
   worker_config: dict,
   dggrs_name: str,
   depth: int) -> Optional[bytes]:
   data_root = worker_config["_data_root"]
   collection = worker_config["collection"]
   collection_config = worker_config["collection_config"]

   store = DGGSDataStore(data_root, collection, config=collection_config)
   dggrs = store.dggrs
   root_zone = DGGRSZone(zone_id)

   # read WKBC (geometry-only feature collection)
   src_fc = read_wkb_collection_file(wkbc_path)

   # clip features to zone (out_fc will have features with ids and no properties)
   out_fc, feature_entry_exit_indices = clip_featurecollection_to_zone(src_fc, dggrs, root_zone, refined=False)

   # free the large WKBC source from memory
   del src_fc

   features = out_fc.get("features", []) or []
   if not features:
      return None

   # produce DGGS-JSON-FG object (properties remain empty in out_fc)
   dggs_obj = write_dggs_json_fg(out_fc, feature_entry_exit_indices, dggrs, root_zone, depth)

   # convert to UBJSON then gzip
   ubbuf = io.BytesIO()
   ubjson.dump(dggs_obj, ubbuf)
   gz = gzip.compress(ubbuf.getvalue(), compresslevel=9)

   return gz

# build blobs in parallel for a batch of zones
def _build_vector_blobs_processes(
   store_worker_config: dict,
   wkbc_path: str,
   dggrs,
   zones: List,
   depth: int,
   max_workers: int = 16
) -> Dict[int, bytes]:
   if not zones:
      return {}
   workers = min(max_workers, max(1, len(zones)))
   blobs: Dict[int, bytes] = {}
   with ProcessPoolExecutor(max_workers=workers) as ex:
      futures = {}
      for z in zones:
         fut = ex.submit(_vector_package_worker, wkbc_path, int(z), store_worker_config, dggrs.__class__.__name__, depth)
         futures[fut] = z
      for fut in as_completed(futures):
         zone = futures[fut]
         res = fut.result()
         if not res:
            print(f"[BUILD] zone={dggrs.getZoneTextID(zone)} returned empty, skipping", flush=True)
            continue
         # store the raw gzip'ed blob bytes directly
         blobs[int(zone)] = res
         print(f"[BUILD] zone={dggrs.getZoneTextID(zone)} blob_ready", flush=True)
   return blobs

# coordinator: process a batch of root zones and write to store (precompressed)
def _process_batch_vector(store, wkbc_path: str, dggrs, base_zone, batch_zones: List, base_ancestors: List,
   depth: int, max_workers: int = 16) -> int:
   if not batch_zones:
      print("[BATCH] empty batch, skipping", flush=True)
      return 0

   print(f"[BATCH] processing batch base_zone={dggrs.getZoneTextID(base_zone)} roots={len(batch_zones)}", flush=True)

   worker_config = {
      "_data_root": store.data_root,
      "collection": store.collection,
      "collection_config": store.config
   }

   entries_map = _build_vector_blobs_processes(
      store_worker_config=worker_config,
      wkbc_path=wkbc_path,
      dggrs=dggrs,
      zones=batch_zones,
      depth=depth,
      max_workers=max_workers
   )

   if not entries_map:
      print("[BATCH] no entries produced for this batch, skipping write", flush=True)
      return 0

   # Build entries according to the store contract:
   # entries is a mapping DGGRSZone -> gzip'ed DGGS-UBJSON-FG blob (bytes)
   entries: Dict[int, bytes] = {}   # int are DGGRSZone
   for zid, blob in entries_map.items():
      zone = DGGRSZone(int(zid))
      entries[int(zone)] = blob

   print(f"[BATCH] writing {len(entries)} entries to store for base_zone={dggrs.getZoneTextID(base_zone)}", flush=True)
   store.write_zone_batch(
      base_zone=base_zone,
      entries=entries,
      base_ancestor_list=base_ancestors,
      precompressed=True
   )
   print(f"[BATCH] write complete for base_zone={dggrs.getZoneTextID(base_zone)} wrote={len(entries)}", flush=True)
   return len(entries)

# top-level import_vector (keeps CLI signature unchanged elsewhere)
def import_vector(input_geojson_path: str,
                  collection_id: str,
                  dggrs_name: str,
                  data_root: str = "data",
                  level: int | None = None,
                  depth: int | None = None,
                  batch_size: int = 32,
                  groupSize: int = 5,
                  max_workers: int = 16,
                  skip_reproj: bool = False,
                  skip_fix: bool = False) -> int:

   dggrs_init = globals().get(dggrs_name)
   if dggrs_init is None:
      print("Unsupported DGGRS:", dggrs_name, flush=True)
      return 1
   dggrs = dggrs_init()

   if depth is None:
      depth = dggrs.get64KDepth()
      print(f"[IMPORT] using default depth (get64KDepth) = {depth}", flush=True)

   if level is None:
      print("import_vector requires --level to be specified (absolute quantize level)", flush=True)
      return 1

   data_level = level
   deepest_root_level = max(0, data_level - depth)

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

   # prepare input once (reproj + fix)
   src = _prepare_input_pipeline(input_geojson_path, dggrs_name, skip_reproj=skip_reproj, skip_fix=skip_fix)

   # write collection-level attributes (features list) into store.attributes.sqlite
   features = src.get("features", []) or []
   if features:
      store.write_collection_attributes(features)
      print(f"[IMPORT] wrote collection attributes for {len(features)} features", flush=True)

   # write WKBC file for workers (WKBC contains geometries and feature ids; properties are not included)
   tmp_wkbc_path = os.path.join(store.collection_dir, "tmp_input.wkbc")
   write_wkb_collection_file(src, tmp_wkbc_path)
   print(f"[IMPORT] wrote WKBC to {tmp_wkbc_path}", flush=True)

   pkg_index = 0
   total_written = 0

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
            print(f"[SKIP] package base {base_text} (base_level={base_level}) deeper than target {max_root_level}", flush=True)
            continue

         roots_iter = store.iter_roots_for_base(base_zone, max_root_level, up_to=up_to)

         batch_num = 0
         batch_zones: List = []
         for zone in roots_iter:
            root_level = dggrs.getZoneLevel(zone)
            if data_level - root_level < 0:
               print(f"[IMPORT] skipping root {dggrs.getZoneTextID(zone)} because data_level < root_level", flush=True)
               continue
            batch_zones.append(zone)
            if len(batch_zones) >= batch_size:
               batch_num += 1
               print(f"[LEVEL {root_level}] #{pkg_index} BATCH {batch_num}: handling {len(batch_zones)} roots", flush=True)
               written = _process_batch_vector(
                  store, tmp_wkbc_path, dggrs, base_zone, batch_zones, base_ancestors,
                  depth, max_workers=max_workers
               )
               total_written += written
               batch_zones = []

         if batch_zones:
            batch_num += 1
            print(f"[LEVEL {root_level}] #{pkg_index} BATCH {batch_num}: handling {len(batch_zones)} roots", flush=True)
            written = _process_batch_vector(
               store, tmp_wkbc_path, dggrs, base_zone, batch_zones, base_ancestors,
               depth, max_workers=max_workers
            )
            total_written += written
            batch_zones = []

         print(f"[LEVEL {root_level}] #{pkg_index} complete; total_written so far={total_written}", flush=True)

   # cleanup temporary WKBC
   if os.path.exists(tmp_wkbc_path):
      os.remove(tmp_wkbc_path)

   print(f"[IMPORT] complete; total written={total_written}", flush=True)
   return 0
