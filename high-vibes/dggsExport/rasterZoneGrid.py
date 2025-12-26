#!/usr/bin/env python3
# rasterZoneGrid.py
# Shared-memory global zone buffer creation and multiprocessing prefill.
# Provides:
# - create_shared_zone_grid(width, height, null_zone_int) -> (zone_grid, shm)
# - prefill_zone_grid(shm_name, dggrs, level, deg_per_pixel, workers)
# Notes:
# - On Unix with fork we use uintptr_t optimization to avoid re-creating DGGRS.
# - On other platforms / start methods we pass a DGGRS id and recreate DGGRS in workers.

from __future__ import annotations
from dggal import *
import os
import math
from concurrent.futures import ProcessPoolExecutor, as_completed
from multiprocessing import shared_memory, get_start_method
from typing import Tuple, Optional

import numpy as np

dggal_ffi = dggal.ffi

# module-level worker state set by initializer in each process
_WORKER_DGGRS_IMPL = None
_WORKER_SHAPE: Tuple[int,int] = None

# initializer receives either a non-zero dggrs_impl_addr (uintptr_t) OR a dggrs_id string/int
# initargs layout: (dggrs_impl_addr:int, dggrs_id:Optional[str], shape:Tuple[int,int])
def _proc_initializer(dggrs_impl_addr: int, dggrs_id: Optional[str], shape: Tuple[int,int]):
   import dggal as _dggal
   ffi_local = _dggal.ffi

   if dggrs_impl_addr:
      # uintptr_t path: cast integer back to void*
      impl = ffi_local.cast("void *", dggrs_impl_addr)
   else:
      # id path: recreate DGGRS in worker and take its impl
      # expects dggal to expose get_or_create_dggrs(dggrs_id)
      dggrs_obj = _dggal.get_or_create_dggrs(dggrs_id)
      impl = dggrs_obj.impl

   globals()["_WORKER_DGGRS_IMPL"] = impl
   globals()["_WORKER_SHAPE"] = shape

def _proc_fill_tile(shm_name: str, level: int, deg_per_pixel: float,
                    px_min: int, px_max: int, py_min: int, py_max: int):
   import numpy as _np
   import math as _math
   import dggal as _dggal
   from dggal import GeoPoint as _GeoPoint
   from multiprocessing import shared_memory as _shm

   _WORKER_DGGRS_IMPL = globals()["_WORKER_DGGRS_IMPL"]
   _WORKER_SHAPE = globals()["_WORKER_SHAPE"]

   shm = _shm.SharedMemory(name=shm_name)
   arr = _np.ndarray(_WORKER_SHAPE, dtype=_np.uint64, buffer=shm.buf)
   flat = arr.ravel()

   deg_to_rad = _math.pi / 180.0
   rad_per_pixel = deg_per_pixel * deg_to_rad
   half_pixel_rad = 0.5 * rad_per_pixel
   top_lat_rad = 90.0 * deg_to_rad
   left_lon_rad = -180.0 * deg_to_rad

   gp = _GeoPoint()
   gp_impl = gp.impl
   get_zone_local = _dggal.lib.DGGRS_getZoneFromWGS84Centroid
   dggrs_impl_local = _WORKER_DGGRS_IMPL

   height, width = _WORKER_SHAPE

   for py in range(py_min, py_max + 1):
      lat_rad = top_lat_rad - (py * rad_per_pixel + half_pixel_rad)
      gp_impl.lat = lat_rad
      base = py * width
      for px in range(px_min, px_max + 1):
         gp_impl.lon = left_lon_rad + (px * rad_per_pixel + half_pixel_rad)
         flat[base + px] = _np.uint64(int(get_zone_local(dggrs_impl_local, level, gp_impl)))

   shm.close()
   return None

def create_shared_zone_grid(width: int, height: int, null_zone_int: int):
   # Create a shared-memory uint64 array shaped (height, width) and initialize to null_zone_int.
   # Returns (zone_grid, shm) where shm is the SharedMemory object (caller must close/unlink).
   size = width * height * np.dtype(np.uint64).itemsize
   shm = shared_memory.SharedMemory(create=True, size=size)
   zone_grid = np.ndarray((height, width), dtype=np.uint64, buffer=shm.buf)
   zone_grid.fill(int(null_zone_int))
   return zone_grid, shm

def prefill_zone_grid(shm_name: str, dggrs, level: int, deg_per_pixel: float,
                      workers: int = 8) -> None:
   # Prefill the shared zone buffer identified by shm_name. Uses a 4x2 tiling scheme.
   # This function does not unlink the shared memory; caller is responsible for cleanup.

   # reconstruct shape from dggrs + level to pass to initializer
   meters_per_subzone = dggrs.getMetersPerSubZoneFromLevel(level, 0)
   meters_per_degree = 111319.49079327358
   deg_per_pixel_local = meters_per_subzone / meters_per_degree
   # compute width/height consistent with main raster dimensions
   width = int(math.ceil(360.0 / deg_per_pixel_local))
   height = int(math.ceil(180.0 / deg_per_pixel_local))

   # build 4x2 tile rects
   tile_rects = []
   for tx in range(4):
      for ty in range(2):
         lon_min = -180.0 + tx * 90.0
         lon_max = lon_min + 90.0
         lat_max = 90.0 - ty * 90.0
         lat_min = lat_max - 90.0
         half = 0.5 * deg_per_pixel
         px_min_f = (lon_min + 180.0 - half) / deg_per_pixel
         px_max_f = (lon_max + 180.0 - half) / deg_per_pixel
         px_min = int(math.ceil(px_min_f)); px_max = int(math.floor(px_max_f))
         px_min = max(0, px_min); px_max = min(width - 1, px_max)
         py_min_f = (90.0 - (lat_max + half)) / deg_per_pixel
         py_max_f = (90.0 - (lat_min - half)) / deg_per_pixel
         py_min = int(math.ceil(py_min_f)); py_max = int(math.floor(py_max_f))
         py_min = max(0, py_min); py_max = min(height - 1, py_max)
         if px_max >= px_min and py_max >= py_min:
            tile_rects.append((px_min, px_max, py_min, py_max))

   # decide whether to use uintptr_t optimization
   start_method = get_start_method()
   use_addr = (os.name == "posix" and start_method == "fork")

   if use_addr:
      # cast DGGRS impl pointer to uintptr_t and pass to workers
      addr = int(dggal_ffi.cast("uintptr_t", dggrs.impl))
      initargs = (addr, None, (height, width))
   else:
      # pass DGGRS id and let workers recreate DGGRS (portable)
      dggrs_id = getattr(dggrs, "id", None)
      initargs = (0, dggrs_id, (height, width))

   with ProcessPoolExecutor(max_workers=workers, initializer=_proc_initializer, initargs=initargs) as ex:
      futures = [ex.submit(_proc_fill_tile, shm_name, level, deg_per_pixel, px_min, px_max, py_min, py_max)
                 for (px_min, px_max, py_min, py_max) in tile_rects]
      for fut in as_completed(futures):
         fut.result()

   return None
