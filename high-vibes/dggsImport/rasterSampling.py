from dggal import *
from typing import List, Dict, Any, Optional

import rasterio
from rasterio.warp import transform
from rasterio.sample import sample_gen
from rasterio.transform import Affine

from dggsStore.store import make_dggs_json_depth

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

def _sample_values_for_centroids(ds, coords: List[tuple], overview_factor: Optional[int], bands: List[int]) -> List[List[Optional[float]]]:
   # Preallocate per-field lists and write by index. Return per_field_values[band_index][centroid_index].
   # - No exceptions, no defensive introspection, minimal inner-loop work.
   # - Nodata handling is computed once up front; NaN nodata is detected via `nod != nod`.
   if not coords:
      return []

   n = len(coords)
   bcount = len(bands)

   # preallocate per-field lists (bcount lists each of length n)
   per_field_values: List[List[Optional[float]]] = [[None] * n for _ in range(bcount)]

   # nodata values per band (1-based band numbers)
   nodata_vals = ds.nodatavals
   if not nodata_vals:
      nodata_vals = tuple([ds.nodata] * ds.count)

   # align nodata values with requested bands and precompute NaN flags
   nodata_for_bands = [nodata_vals[b - 1] if (b - 1) < len(nodata_vals) else None for b in bands]
   nodata_is_nan = [(nod is not None and nod != nod) for nod in nodata_for_bands]

   if overview_factor and overview_factor > 1:
      factor = overview_factor
      out_h = max(1, int(ds.height / factor))
      out_w = max(1, int(ds.width / factor))
      arr = ds.read(bands, out_shape=(bcount, out_h, out_w))
      inv_transform = ~ds.transform
      scale_x = ds.width / out_w
      scale_y = ds.height / out_h

      for i, (x, y) in enumerate(coords):
         colf, rowf = inv_transform * (x, y)
         col_o = int(colf / scale_x)
         row_o = int(rowf / scale_y)
         if 0 <= row_o < out_h and 0 <= col_o < out_w:
            for bi in range(bcount):
               v = arr[bi, row_o, col_o]
               nod = nodata_for_bands[bi]
               per_field_values[bi][i] = None if (
                  v is None
                  or (nod is not None and (nodata_is_nan[bi] and v != v or (not nodata_is_nan[bi] and v == nod)))
               ) else float(v)
         else:
            # leave None for missing
            pass
   else:
      # full-resolution sampling: request only requested bands from rasterio.sample
      for i, arr in enumerate(ds.sample(coords, indexes=tuple(bands))):
         if arr is None:
            continue
         for j in range(bcount):
            v = arr[j] if j < len(arr) else None
            nod = nodata_for_bands[j]
            per_field_values[j][i] = None if (
               v is None
               or (nod is not None and (nodata_is_nan[j] and v != v or (not nodata_is_nan[j] and v == nod)))
            ) else float(v)

   return per_field_values

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

   # minimal shape validation (no defensive inner-loop checks)
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
