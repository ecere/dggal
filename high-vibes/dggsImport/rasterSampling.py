from dggal import *
from typing import List, Dict, Any, Optional, Tuple
import itertools
import rasterio
from rasterio.warp import transform
from rasterio.sample import sample_gen
from rasterio.transform import Affine, rowcol
from rasterio.windows import Window
from cffi import FFI
from pyproj import Transformer

import numpy as np

from dggsStore.store import make_dggs_json_depth

# ---------------------------------------------------------------------------
# Low-level sampling / coordinate helpers
# ---------------------------------------------------------------------------

def _coords_for_centroids(centroids, raster_crs: str) -> List[Tuple[float, float]]:
   ffi = FFI()
   n = centroids.count
   ptr = ffi.cast("double *", centroids.array)
   buf = ffi.buffer(ptr, n * 2 * ffi.sizeof("double"))
   arr = np.frombuffer(buf, dtype=np.float64).reshape((n, 2))   # columns: [lat_rad, lon_rad]

   lat_rad = arr[:, 0]
   lon_rad = arr[:, 1]

   # If raster CRS is geographic degrees, we must supply degrees to rasterio.
   if raster_crs == "EPSG:4326":
      # convert radians -> degrees (fast vectorized)
      lon_deg = np.degrees(lon_rad)
      lat_deg = np.degrees(lat_rad)
      coords = np.column_stack((lon_deg, lat_deg))
   else:
      # Otherwise transform directly from radians -> target CRS units without explicit deg conversion
      transformer = Transformer.from_crs("EPSG:4326", raster_crs, always_xy=True)
      # transformer.transform accepts arrays and the radians flag
      xs, ys = transformer.transform(lon_rad, lat_rad, radians=True)
      coords = np.column_stack((xs, ys))
   return [ (float(x), float(y)) for x, y in coords ]

def _sample_values_for_centroids(ds, coords: List[tuple], overview_factor: Optional[int], bands: List[int]) -> List[List[Optional[float]]]:
   # Vectorized sampling: one read covering all coords, then NumPy advanced indexing.
   # Returns per_field_values[band_index][centroid_index] with None for nodata/missing.
   if not coords:
      return []

   n = len(coords)
   bcount = len(bands)

   # preallocate numpy result with NaN sentinel
   result_np = np.full((bcount, n), np.nan, dtype=np.float64)

   # nodata values per band (1-based band numbers)
   nodata_vals = ds.nodatavals
   if not nodata_vals:
      nodata_vals = tuple([ds.nodata] * ds.count)
   nodata_for_bands = np.array([nodata_vals[b - 1] if (b - 1) < len(nodata_vals) else None for b in bands], dtype=object)
   nodata_is_nan = np.array([(nod is not None and nod != nod) for nod in nodata_for_bands], dtype=bool)

   # split coords into arrays
   xs = np.array([c[0] for c in coords], dtype=float)
   ys = np.array([c[1] for c in coords], dtype=float)

   # compute pixel row/col indices at dataset resolution (vectorized)
   rows_all, cols_all = rowcol(ds.transform, xs, ys, op=int)

   # determine bounding window in pixel coordinates (clamped to dataset)
   min_row = int(max(0, rows_all.min()))
   max_row = int(min(ds.height - 1, rows_all.max()))
   min_col = int(max(0, cols_all.min()))
   max_col = int(min(ds.width - 1, cols_all.max()))

   # if all points outside dataset bounds, return lists of None
   if min_row > max_row or min_col > max_col:
      return [[None] * n for _ in range(bcount)]

   win_row_off = min_row
   win_col_off = min_col
   win_height = max_row - min_row + 1
   win_width = max_col - min_col + 1

   # overview/downsampled read if requested
   if overview_factor and overview_factor > 1:
      factor = overview_factor
      out_h = max(1, int(win_height / factor))
      out_w = max(1, int(win_width / factor))
      arr = ds.read(bands, window=Window(win_col_off, win_row_off, win_width, win_height), out_shape=(bcount, out_h, out_w))
      # relative positions inside window
      rel_rows = rows_all - win_row_off
      rel_cols = cols_all - win_col_off
      # map to overview indices (vectorized)
      row_o = np.floor_divide(rel_rows * out_h, max(1, win_height)).astype(int)
      col_o = np.floor_divide(rel_cols * out_w, max(1, win_width)).astype(int)
      valid_mask = (row_o >= 0) & (row_o < out_h) & (col_o >= 0) & (col_o < out_w)
      if valid_mask.any():
         valid_indices = np.nonzero(valid_mask)[0]
         vals = arr[:, row_o[valid_mask], col_o[valid_mask]]   # shape (bcount, n_valid)
         result_np[:, valid_indices] = vals
   else:
      # full-resolution read of the single window
      arr = ds.read(bands, window=Window(win_col_off, win_row_off, win_width, win_height))
      rel_rows = rows_all - win_row_off
      rel_cols = cols_all - win_col_off
      valid_mask = (rel_rows >= 0) & (rel_rows < win_height) & (rel_cols >= 0) & (rel_cols < win_width)
      if valid_mask.any():
         valid_indices = np.nonzero(valid_mask)[0]
         vals = arr[:, rel_rows[valid_mask], rel_cols[valid_mask]]   # shape (bcount, n_valid)
         result_np[:, valid_indices] = vals

   # vectorized nodata handling per band
   for bi in range(bcount):
      nod = nodata_for_bands[bi]
      if nod is None:
         # ensure non-finite values are NaN
         mask = ~np.isfinite(result_np[bi])
         result_np[bi, mask] = np.nan
      else:
         if nodata_is_nan[bi]:
            mask = np.isnan(result_np[bi])
            result_np[bi, mask] = np.nan
         else:
            finite_mask = np.isfinite(result_np[bi])
            eq_mask = finite_mask & (result_np[bi] == float(nod))
            result_np[bi, eq_mask] = np.nan

   # convert to per-field Python lists with None for NaN
   per_field_values: List[List[Optional[float]]] = []
   for bi in range(bcount):
      row = result_np[bi]
      per_field_values.append([None if not np.isfinite(x) else float(x) for x in row])

   return per_field_values

# ---------------------------------------------------------------------------
# Overview selection helpers (meters-per-subzone based)
# ---------------------------------------------------------------------------

def _meters_per_degree_at_lat(lat_deg: float) -> float:
   # approximate meters per degree of longitude at given latitude
   lat_rad = math.radians(lat_deg)
   return 111320.0 * math.cos(lat_rad)

def _choose_overview_factor_for_level(ds, dggrs, zone, root_level: int, relative_depth: int) -> int:
   target = dggrs.getMetersPerSubZoneFromLevel(root_level, relative_depth)
   px_x = abs(ds.transform.a); px_y = abs(ds.transform.e)
   is_geo = (ds.crs is None) or (ds.crs.to_string() == "EPSG:4326")
   max_allowed = target * (1.0 + 0.01) # 1% tolerance

   if is_geo:
      base_x = None
      base_y = px_y * 111132.92  # m/degrees
   else:
      base_x = px_x
      base_y = px_y

   best = 1
   best_eff = 0.0

   factors = itertools.chain([1], ds.overviews(1) if ds.count >= 1 else [])
   for f in factors:
      eff_x = (base_x * f) if (base_x is not None) else 0.0
      eff_y = base_y * f

      candidate_eff = eff_x if eff_x >= eff_y else eff_y
      if candidate_eff <= max_allowed and candidate_eff > best_eff:
         best, best_eff = f, candidate_eff

   # progress print for overview selection
   print(f"[OVERVIEW] zone={dggrs.getZoneTextID(zone)} root_level={root_level} depth={relative_depth} "
         f"target_m_per_subzone={target:.3f} overview_px_m={best_eff:.6f} chosen_factor={best}",
         flush=True)

   return 1 if best_eff == 0.0 else best

# ---------------------------------------------------------------------------
# Sampling and aggregation builders (produce Dict[int, Dict])
# ---------------------------------------------------------------------------

def sample_depth_obj_for_zone(store, ds, raster_crs: str, dggrs, zone, data_level: int,
   depth: int, use_overviews: bool, fields: List[str], bands: List[int] | None = None) -> Optional[Dict[str, List[Dict[str, Any]]]]:
   # Sample centroids for `zone` at `depth` and return fields_map:
   #  { field_name: [ depth_entry ] }
   # Assumes caller's `fields` corresponds to requested `bands`.
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

   overview_factor: Optional[int] = None
   if use_overviews:
      factor = _choose_overview_factor_for_level(ds, dggrs, zone, root_level, depth)
      if factor and factor > 1:
         overview_factor = factor

   print(f"[SAMPLE] zone={dggrs.getZoneTextID(zone)} root_level={root_level} depth={depth} "
         f"centroids={count_centroids} overview_factor={overview_factor}", flush=True)

   bands_to_sample = bands if bands is not None else list(range(1, ds.count + 1))
   bcount = len(bands_to_sample)

   # get per-field arrays: per_field_values[band_index][centroid_index]
   per_field_values = _sample_values_for_centroids(ds, coords, overview_factor, bands_to_sample)

   if not per_field_values or len(per_field_values) != bcount or any(len(pv) != count_centroids for pv in per_field_values):
      print(f"[SAMPLE] zone={dggrs.getZoneTextID(zone)} depth={depth} shape_mismatch sampled_bands={len(per_field_values) if per_field_values else 0} expected_bands={bcount} or centroid_count_mismatch, skipping", flush=True)
      return None

   print(f"[SAMPLE] zone={dggrs.getZoneTextID(zone)} depth={depth} sampled_ok", flush=True)

   # Build fields_map directly; fields is expected to align with sampled bands
   fields_map: Dict[str, List[Dict[str, Any]]] = {}
   for i, fname in enumerate(fields):
      per_field_vals = per_field_values[i]   # safe: validated above
      per_field_depth_entry = make_dggs_json_depth(depth, count_centroids, per_field_vals)
      fields_map[fname] = [per_field_depth_entry]

   return fields_map
