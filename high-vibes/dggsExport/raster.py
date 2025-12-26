#!/usr/bin/env python3
# raster.py
# Streaming, multiprocessing rasterizer for DGGS packages
# - Parent creates shared memory for zone grid and output buffer
# - Parent pre-fills the zone grid
# - Parent submits one task per package; each worker re-creates a lightweight
#   DGGSDataStore from a small config and iterates roots lazily
# - Workers map shared memory by name and write into the shared output buffer

from __future__ import annotations
from dggal import *
import dggal
from dggsStore.store import DGGSDataStore
import time
import math
import logging
from concurrent.futures import ProcessPoolExecutor, as_completed
from typing import Tuple, Dict, List, Iterator

import numpy as np
import rasterio
from rasterio.transform import from_origin
import os

from .rasterZoneGrid import create_shared_zone_grid, prefill_zone_grid

# FFI handle
dggal_ffi = dggal.ffi

# logging
logging.basicConfig(level=logging.INFO, format="%(levelname)s:%(message)s")
logger = logging.getLogger(__name__)

# geometry helpers
def compute_raster_dimensions(dggrs, level: int) -> Tuple[float, float, int, int, object]:
   meters_per_subzone = dggrs.getMetersPerSubZoneFromLevel(level, 0)
   meters_per_degree = 111319.49079327358
   deg_per_pixel = meters_per_subzone / meters_per_degree
   width = int(math.ceil(360.0 / deg_per_pixel))
   height = int(math.ceil(180.0 / deg_per_pixel))
   transform = from_origin(-180.0, 90.0, deg_per_pixel, deg_per_pixel)
   return meters_per_subzone, deg_per_pixel, width, height, transform

def _lon_ranges_for_extent(min_lon: float, max_lon: float) -> List[Tuple[float, float]]:
   if max_lon > 180.0:
      max_lon -= 360.0
   if (max_lon - min_lon) >= 360.0 - 1e-9:
      return [(-180.0, 180.0)]
   if max_lon < min_lon:
      return [(min_lon, 180.0), (-180.0, max_lon)]
   return [(min_lon, max_lon)]

# prepare_root: runs in worker process; expects store and dggrs available there
def prepare_root(store: DGGSDataStore, dggrs, pkg_path: str, zone: DGGRSZone,
                 depth: int, deg_per_pixel: float, width: int, height: int):
   t0 = time.time()
   decoded = store.read_and_decode_zone_blob(pkg_path, zone)
   decode_s = time.time() - t0

   prop_key = store.property_key
   prop_list = decoded["values"][prop_key]
   chosen = next(e for e in prop_list if int(e["depth"]) == depth)

   values = np.array(list(chosen["data"]), dtype=np.float64, copy=True)

   subs_obj = dggrs.getSubZones(zone, depth)
   n = subs_obj.count
   raw = dggal_ffi.buffer(subs_obj.array, n * 8)
   subs_array = np.frombuffer(raw, dtype=np.uint64)

   # idx_map semantics: zero-based indices into values; 0 is valid
   idx_map = {int(z): i for i, z in enumerate(subs_array)}

   ext = GeoExtent()
   dggrs.getZoneWGS84Extent(zone, ext)
   ll_lon = float(ext.ll.lon); ll_lat = float(ext.ll.lat)
   ur_lon = float(ext.ur.lon); ur_lat = float(ext.ur.lat)

   lon_ranges = _lon_ranges_for_extent(ll_lon, ur_lon)

   min_y = max(0, int(math.floor((90.0 - ur_lat) / deg_per_pixel - 0.5)))
   max_y = min(height - 1, int(math.floor((90.0 - ll_lat) / deg_per_pixel - 0.5)))

   return values, subs_array, idx_map, lon_ranges, min_y, max_y, decode_s

# paint worker: maps shared memory and writes values
def _paint_worker(shm_zone_name: str, shm_out_name: str,
                  width: int, height: int,
                  deg_per_pixel: float,
                  lon_ranges: List[Tuple[float,float]],
                  min_y: int, max_y: int,
                  subs_array: np.ndarray, values: np.ndarray,
                  nodata: float, null_zone_int: int,
                  idx_map: Dict[int,int]):
   from multiprocessing import shared_memory as _shm

   shm_zone = _shm.SharedMemory(name=shm_zone_name)
   zone_grid = np.ndarray((height, width), dtype=np.uint64, buffer=shm_zone.buf)

   shm_out = _shm.SharedMemory(name=shm_out_name)
   out_arr = np.ndarray((height, width), dtype=np.float32, buffer=shm_out.buf)

   nz = int(null_zone_int)
   nod = nodata

   for lon_min, lon_max in lon_ranges:
      min_x = max(0, int(math.floor((lon_min + 180.0) / deg_per_pixel - 0.5)))
      max_x = min(width - 1, int(math.floor((lon_max + 180.0) / deg_per_pixel - 0.5)))
      if max_x < min_x:
         continue

      cols = max_x - min_x + 1

      block = zone_grid[min_y:max_y+1, min_x:max_x+1].astype(np.uint64, copy=False)
      block_flat = block.ravel()

      valid_mask = (block_flat != nz)
      if not np.any(valid_mask):
         continue

      valid_positions = np.flatnonzero(valid_mask)
      zone_vals = block_flat[valid_positions]

      idxs = np.fromiter((idx_map.get(int(z), -1) for z in zone_vals),
                         dtype=np.int32, count=zone_vals.size)

      present_mask = (idxs >= 0)
      if not np.any(present_mask):
         continue

      present_positions = valid_positions[present_mask]
      value_indices = idxs[present_mask]

      gathered_vals = values[value_indices]
      valid_value_mask = (gathered_vals != nod)
      if not np.any(valid_value_mask):
         continue

      final_positions = present_positions[valid_value_mask]
      final_values = gathered_vals[valid_value_mask].astype(np.float32, copy=False)

      rows_idx = final_positions // cols
      cols_idx = final_positions % cols
      out_arr[min_y + rows_idx, min_x + cols_idx] = final_values

   shm_zone.close()
   shm_out.close()
   return None

# iter_packages yields primitive ids only (no CFFI objects)
def iter_packages(store: DGGSDataStore, root_level: int) -> Iterator[Tuple[str, int, List[int]]]:
   base_level = store._base_level_for_root(root_level)
   for base_zone, base_ancestors in store.iter_bases(base_level, up_to=False):
      pkg_path = store.compute_package_path_for_root_zone(root_level, base_ancestor_list=base_ancestors)
      if not pkg_path:
         continue
      base_zone_id = int(base_zone)
      base_ancestors_ids = [int(b) for b in base_ancestors]
      yield pkg_path, base_zone_id, base_ancestors_ids

# per-package worker: reconstruct store and DGGRSZone from ids inside worker
def _paint_package_worker(pkg_path: str,
                          base_zone_id: int,
                          base_ancestors_ids: List[int],
                          worker_config: dict,
                          root_level: int,
                          depth: int,
                          deg_per_pixel: float,
                          width: int,
                          height: int,
                          shm_zone_name: str,
                          shm_out_name: str,
                          nodata: float,
                          null_zone_int: int):
   data_root = worker_config["_data_root"]
   collection = worker_config["collection"]
   collection_config = worker_config["collection_config"]

   store = DGGSDataStore(data_root, collection, config=collection_config)
   dggrs = store.dggrs

   # reconstruct base_zone as DGGRSZone inside worker
   # prefer dggrs API if available, otherwise use DGGRSZone constructor
   if hasattr(dggrs, "zoneFromId"):
      base_zone = dggrs.zoneFromId(base_zone_id)
   else:
      base_zone = DGGRSZone(base_zone_id)

   for root_zone in store.iter_roots_for_base(base_zone, root_level, up_to=False):
      values, subs_array, idx_map, lon_ranges, min_y, max_y, _decode_s = prepare_root(
         store, dggrs, pkg_path, root_zone, depth, deg_per_pixel, width, height
      )

      if values.size == 0 or subs_array.size == 0:
         continue

      _paint_worker(shm_zone_name, shm_out_name,
                    width, height,
                    deg_per_pixel,
                    lon_ranges,
                    min_y, max_y,
                    subs_array, values,
                    nodata, null_zone_int,
                    idx_map)

# top-level rasterize: streaming, no materialization of all roots
def rasterize_to_geotiff(store: DGGSDataStore, level: int, outfile: str, workers: int = 8,
                         nodata: float = np.finfo(np.float32).max, compress: str = "lzw",
                         debug: bool = False, max_packages: int = 0) -> dict:
   start = time.time()
   logger.info("rasterize start level=%d workers=%d", level, workers)

   dggrs = store.dggrs
   depth = store.depth

   meters_per_subzone, deg_per_pixel, width, height, transform = compute_raster_dimensions(dggrs, level)

   logger.info("Allocating shared global zone_grid %dx%d (uint64) and out buffer (float32)", width, height)
   zone_grid, shm_zone = create_shared_zone_grid(width, height, int(nullZone))

   from multiprocessing import shared_memory as _shm
   size_out = width * height * np.dtype(np.float32).itemsize
   shm_out = _shm.SharedMemory(create=True, size=size_out)
   out_shared = np.ndarray((height, width), dtype=np.float32, buffer=shm_out.buf)
   out_shared.fill(nodata)

   # prefill zone grid in parent
   prefill_zone_grid(shm_zone.name, dggrs, level, deg_per_pixel, workers=workers)

   root_level = max(0, level - depth)

   # worker_config contains parsed collection.json to avoid disk reparse
   worker_config = {
      "_data_root": store.data_root,
      "collection": store.collection,
      "collection_config": store.config
   }

   futures = []
   submitted = 0
   null_zone_int = int(nullZone)

   with ProcessPoolExecutor(max_workers=workers) as ex:
      for pkg_path, base_zone_id, base_ancestors_ids in iter_packages(store, root_level):
         if max_packages and submitted >= max_packages:
            break
         submitted += 1
         fut = ex.submit(
            _paint_package_worker,
            pkg_path, base_zone_id, base_ancestors_ids,
            worker_config, root_level, depth,
            deg_per_pixel, width, height,
            shm_zone.name, shm_out.name,
            float(nodata), null_zone_int
         )
         futures.append(fut)

      for fut in as_completed(futures):
         fut.result()

   elapsed = time.time() - start
   logger.info("rasterize complete elapsed=%.2f s", elapsed)

   profile = {
      "driver": "GTiff",
      "height": height,
      "width": width,
      "count": 1,
      "dtype": "float32",
      "crs": "EPSG:4326",
      "transform": transform,
      "nodata": nodata,
      "compress": compress
   }

   os.makedirs(os.path.dirname(outfile) or ".", exist_ok=True)
   with rasterio.open(outfile, "w", **profile) as dst:
      dst.write(out_shared, 1)

   # cleanup shared memory handles
   shm_out.close()
   shm_out.unlink()
   shm_zone.close()
   shm_zone.unlink()

   return {"elapsed_seconds": elapsed}

# legacy build_package_map (kept for compatibility)
def build_package_map(store: DGGSDataStore, root_level: int):
   pkg_map = {}
   root_count = 0
   for base_zone, base_ancestors in store.iter_bases(store._base_level_for_root(root_level), up_to=False):
      pkg_path = store.compute_package_path_for_root_zone(root_level, base_ancestor_list=base_ancestors)
      for zone in store.iter_roots_for_base(base_zone, root_level, up_to=False):
         pkg_map.setdefault(pkg_path, []).append(zone)
         root_count += 1
   return pkg_map, root_count
