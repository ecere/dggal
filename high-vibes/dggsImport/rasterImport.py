from dggal import *
from typing import List, Dict, Any, Optional
from concurrent.futures import ThreadPoolExecutor, as_completed
import json

from dggsStore.store import *
from dggsStore.customDepths import *

from .rasterSampling import *

import threading
from typing import List, Dict, Any, Optional
from concurrent.futures import ThreadPoolExecutor, ProcessPoolExecutor, as_completed
import rasterio

# top-level process worker (must be picklable)
def _sample_package_worker(ds_path: str,
   zone_id: int,
   worker_config: dict,
   raster_crs: str,
   data_level: int,
   depth: int,
   use_overviews: bool,
   fields: List[str],
   bands: Optional[List[int]] = None
) -> Optional[Dict[str, List[Dict[str, Any]]]]:
   data_root = worker_config["_data_root"]
   collection = worker_config["collection"]
   collection_config = worker_config["collection_config"]
   store = DGGSDataStore(data_root, collection, config=collection_config)
   dggrs = store.dggrs
   # reconstruct zone object from integer id
   zone = DGGRSZone(zone_id)
   with rasterio.open(ds_path) as ds:
      return sample_depth_obj_for_zone(store, ds, raster_crs, dggrs, zone,
         data_level, depth, use_overviews, fields, bands)

# top-level aggregate worker (must be picklable)
def _aggregate_package_worker(worker_config: dict,
   zone_id: int,
   zone_depth: int,
   fields: Optional[List[str]] = None
) -> Optional[Dict[str, List[Dict[str, Any]]]]:
   # Recreate the store inside the child process and call aggregate_zone_at_depth.
   # Accepts only picklable primitives so it can be submitted to ProcessPoolExecutor.
   data_root = worker_config["_data_root"]
   collection = worker_config["collection"]
   collection_config = worker_config["collection_config"]
   store = DGGSDataStore(data_root, collection, config=collection_config)
   # reconstruct zone object from integer id
   root_zone = DGGRSZone(zone_id)
   # call the existing aggregation function (signature unchanged)
   return aggregate_zone_at_depth(store, root_zone, zone_depth, fields)

def _build_blobs_processes(
   store_worker_config: dict,
   ds_path: str,                # dataset path string (ds.name)
   raster_crs: str,
   dggrs,                       # used only in parent for zone text lookup
   dggrs_uri: str,
   zones: List,
   data_level: int,
   depth: int,
   fields: List[str],
   use_overviews: bool,
   aggregate: bool,
   bands: Optional[List[int]] = None,
   max_workers: int = 16
) -> Dict[int, Dict[str, Any]]:
   if not zones:
      return {}

   workers = min(max_workers, max(1, len(zones)))
   blobs: Dict[int, Dict[str, Any]] = {}

   with ProcessPoolExecutor(max_workers=workers) as ex:
      futures = {}
      if aggregate:
         # submit the aggregate wrapper that reconstructs the store in the child
         for z in zones:
            fut = ex.submit(_aggregate_package_worker, store_worker_config, int(z), depth, fields)
            futures[fut] = z
      else:
         for z in zones:
            # pass only picklable primitives to the worker
            fut = ex.submit(_sample_package_worker, ds_path, int(z), store_worker_config,
               raster_crs, data_level, depth, use_overviews, fields, bands)
            futures[fut] = z

      for fut in as_completed(futures):
         zone = futures[fut]
         fields_map = fut.result()
         if not fields_map:
            if aggregate:
               print(f"[AGG] zone={dggrs.getZoneTextID(zone)} aggregate returned empty, skipping", flush=True)
            else:
               print(f"[SAMPLE] zone={dggrs.getZoneTextID(zone)} sample returned empty, skipping", flush=True)
            continue

         zone_text = dggrs.getZoneTextID(zone)
         blobs[int(zone)] = make_dggs_json_blob(dggrs_uri, zone_text, fields_map)
         if aggregate:
            print(f"[AGG] zone={zone_text} agg_blob_accepted", flush=True)
         else:
            print(f"[BUILD] zone={zone_text} blob_ready fields={list(fields_map.keys())}", flush=True)

   return blobs

# ---------------------------------------------------------------------------
# Coordinator: single write boundary (uses the threaded builder)
# - Keeps the original sample_depth_obj_for_zone signature unchanged.
# - Passes the open ds through; the sampling worker will open a per-thread handle.
# ---------------------------------------------------------------------------
def _process_batch(store, ds, raster_crs: str, dggrs, dggrs_uri: str,
   base_zone, batch_zones: List, base_ancestors: List,
   data_level: int, depth: int, fields: List[str], use_overviews: bool, aggregate: bool, bands: List[int] | None = None,
   max_workers: int = 16) -> int:
   if not batch_zones:
      print("[BATCH] empty batch, skipping", flush=True)
      return 0

   print(f"[BATCH] processing batch base_zone={dggrs.getZoneTextID(base_zone)} roots={len(batch_zones)} aggregate={aggregate}", flush=True)

   worker_config = {
      "_data_root": store.data_root,
      "collection": store.collection,
      "collection_config": store.config
   }

   entries = _build_blobs_processes(
      store_worker_config=worker_config,
      ds_path=ds.name,
      raster_crs=raster_crs,
      dggrs=dggrs,
      dggrs_uri=dggrs_uri,
      zones=batch_zones,
      data_level=data_level,
      depth=depth,
      fields=fields,
      use_overviews=use_overviews,
      aggregate=aggregate,
      bands=bands,
      max_workers=max_workers
   )

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

def import_raster(ds, collection_id: str, dggrs_name: str, data_root: str = "data",
   level: int | None = None, depth: int | None = None,
   fields: List[str] | None = None, bands: List[int] | None = None,
   batch_size: int = 32, groupSize: int = 5, aggregate: bool | None = None):

   # Uses provided open rasterio dataset `ds`. No defensive constructs; closes ds on every return.
   # Returns 0 on success, 1 on error.

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
      ds.close()
      return 1
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

   bands_used = bands if bands is not None else list(range(1, ds.count + 1))

   if not fields:
      fields = [f"field{i+1}" for i in range(len(bands_used))]

   print(fields)

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
                  base_ancestors, data_level, depth, fields, use_overviews_for_sampling,
                  aggregate and root_level < deepest_root_level, bands_used
               )
               total_written += written
               batch_zones = []

         if batch_zones:
            batch_num += 1
            print(f"[LEVEL {root_level}] #{pkg_index} BATCH {batch_num}: handling {len(batch_zones)} roots",
               flush=True)
            written = _process_batch(
               store, ds, raster_crs, dggrs, dggrs_uri, base_zone, batch_zones,
               base_ancestors, data_level, depth, fields, use_overviews_for_sampling,
               aggregate and root_level < deepest_root_level, bands_used
            )
            total_written += written

         print(f"[LEVEL {root_level}] #{pkg_index} complete; total_written so far={total_written}", flush=True)

      if aggregate and not finest_level_done:
         finest_level_done = True
         store._compute_fields()

   ds.close()
   print(f"[IMPORT] complete; total written={total_written}", flush=True)
   return 0
