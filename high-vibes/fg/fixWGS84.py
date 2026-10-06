from .sutherlandHodgman import *
from .fix_topology_5x6 import collapse_near_duplicates

import json
import os
from math import floor
from typing import Any, Dict, List, Optional, Tuple, Sequence

from shapely.geometry import (
   Point,
   MultiPoint,
   LineString,
   MultiLineString,
   Polygon,
   MultiPolygon,
   GeometryCollection,
   box,
   mapping,
   shape
)
from shapely.ops import unary_union, linemerge, orient
from shapely.validation import explain_validity
import shapely

# ---------- simple file-level debug flag (hardcoded) ----------
DEBUG = False # True
_DEBUG_OUT_DIR = "wgs84_debug_tiles"   # directory where per-tile debug files will be written

# ---------- constants ----------
DUP_EPS = 1e-12
_AREA_EPS = 1e-12

# ---------- tiles and thresholds (4 tiles) ----------
# tiles: xmin, ymin, xmax, ymax
TILES_4 = [
   (-180.0, -90.0, -90.0, 90.0),   # q = 0
   (-90.0, -90.0, 0.0, 90.0),      # q = 1
   (0.0, -90.0, 90.0, 90.0),       # q = 2
   (90.0, -90.0, 180.0, 90.0),     # q = 3
]

# small epsilon used in wrapLonAt comparisons (degrees)
_EPS = 1e-9

# ---------- wrap helpers (degree-based translation of Radians code) ----------
def wrap_lon_at(lon: float, c_lon: float) -> float:
   # - lon: input longitude in degrees (raw input, assumed in -180..180).
   # - c_lon: tile center longitude in degrees.

   # Returns shifted longitude (absolute degrees) such that the returned value is
   # within +/-180 of the tile center.

   # Work in lon relative to center
   rel = lon - c_lon

   # coarse wrap into (-180,180] using floor-based single-step style (but allow multiple-step via floor)
   if rel < -180.0 - _EPS:
      rel += 360.0 * floor((180.0 - rel) / 360.0)
   elif rel > 180.0 + _EPS:
      rel -= 360.0 * floor((rel + 180.0) / 360.0)

   # return absolute lon (relative + center)
   return rel + c_lon

# ---------- staged union helper (kept for future re-enable) ----------
def _staged_union_polygons(polys: List[Polygon], stage_size: int = 64):
   # This function is provided for when geometries are valid and unary_union can be used safely.
   # Currently the pipeline may produce invalid intermediate polygons; when stable, re-enable use.
   if not polys:
      return None, []
   valid = [p for p in polys if p is not None and not p.is_empty and p.area > _AREA_EPS]
   skipped = [i for i, p in enumerate(polys) if p is None or p.is_empty or (hasattr(p, "area") and p.area <= _AREA_EPS)]
   if not valid:
      return None, skipped
   batches = []
   cur: List[Polygon] = []
   for p in valid:
      cur.append(p)
      if len(cur) >= stage_size:
         batches.append(unary_union(cur))
         cur = []
   if cur:
      batches.append(unary_union(cur))
   # merged = unary_union(batches)
   merged = shapely.union_all(batches, grid_size=1e-10)
   return merged, skipped

# ---------- debug store (proper GeoJSON per tile) ----------
_debug_store: Dict[str, List[Dict[str, Any]]] = {}
def _debug_record_tile(xmin: float, xmax: float, record: Dict[str, Any]) -> None:
   if not DEBUG:
      return
   key = f"{int(xmin)}_{int(xmax)}"
   if key not in _debug_store:
      _debug_store[key] = []
   _debug_store[key].append(record)

def _debug_write_files() -> None:
   if not DEBUG:
      return
   os.makedirs(_DEBUG_OUT_DIR, exist_ok=True)
   for key, features in _debug_store.items():
      fname = os.path.join(_DEBUG_OUT_DIR, f"tile_{key}.geojson")
      fc = {"type": "FeatureCollection", "features": features}
      with open(fname, "w", encoding="utf-8") as fh:
         json.dump(fc, fh, ensure_ascii=False, indent=2)

EPS_ZONE_TILE = 1e-3

def intersects_extent_deg(a: Sequence[float], b: Sequence[float], deg_epsilon: float = 1e-12) -> bool:
    # Test intersection of axis-aligned geographic extents in degrees.
    # Each extent is (xmin, ymin, xmax, ymax). Handles dateline-crossing extents
    # by splitting them into two normal extents.
    axmin, aymin, axmax, aymax = float(a[0]), float(a[1]), float(a[2]), float(a[3])
    bxmin, bymin, bxmax, bymax = float(b[0]), float(b[1]), float(b[2]), float(b[3])

    # this extent crosses the dateline (xmin > xmax)
    if axmin > axmax:
        a1 = (axmin, aymin, 180.0, aymax)
        a2 = (-180.0, aymin, axmax, aymax)
        return intersects_extent_deg(a1, b, deg_epsilon) or intersects_extent_deg(a2, b, deg_epsilon)

    # other extent crosses the dateline
    if bxmin > bxmax:
        b1 = (bxmin, bymin, 180.0, bymax)
        b2 = (-180.0, bymin, bxmax, bymax)
        return intersects_extent_deg(a, b1, deg_epsilon) or intersects_extent_deg(a, b2, deg_epsilon)

    #print("Cond0:", aymin < bymax - deg_epsilon)
    #print("Cond1:", bymin < aymax - deg_epsilon)
    #print("Cond2:", axmin < bxmax - deg_epsilon)
    #print("Cond3:", bxmin < axmax - deg_epsilon)
    #print("bxmin < axmax: ", bxmin, axmax - deg_epsilon)

    # simple axis-aligned overlap test in degrees with tiny epsilon
    return (
        aymin < bymax - deg_epsilon
        and bymin < aymax - deg_epsilon
        and axmin < bxmax - deg_epsilon
        and bxmin < axmax - deg_epsilon
    )

def _accept_polygon_piece(poly: Polygon, xmin: float, ymin: float, xmax: float, ymax: float, fid: Any, part_idx: int) -> Optional[Dict[str, Any]]:
   # Rely directly on the clipping engine's spatial filtering output bounds
   if poly.is_empty or poly.area <= _AREA_EPS:
      return None

   return {"tile_x": xmin, "tile_y": ymin, "geom": poly, "orig_fid": fid, "part_idx": part_idx}

def _fmt_closed(coords: Any) -> List[List[float]]:
   # Constructs a closed ring array for clean GeoJSON representation.
   lst = list(coords)
   if lst and lst != lst[-1]:
      lst.append(lst[0])
   return [[float(p[0]), float(p[1])] for p in lst]

def _write_milestone_file(path: str, data: Dict[str, Any]) -> None:
   # Safely serializes tracking milestones straight to the local filesystem.
   try:
      with open(path, "w") as f:
         json.dump(data, f, indent=2)
   except Exception as e:
      print(f"Warning: Could not write debug milestone file to {path}: {e}")

def _flatten_to_simple_polygons(raw_polys: List[Polygon]) -> List[Polygon]:
   # Extracts individual valid simple Polygon components from collection objects.
   final_polys = []
   for p in raw_polys:
      if p.is_empty or p.area <= 0.0:
         continue
      if p.geom_type == "Polygon":
         final_polys.append(p)
      elif p.geom_type == "MultiPolygon":
         final_polys.extend([sub for sub in p.geoms if not sub.is_empty and sub.area > 0.0])
   return final_polys

def _execute_phase1_topology_wrap(exterior_coords: List[Tuple[float, float]],
                                  holes_coords: List[List[Tuple[float, float]]],
                                  zone_c_lon: float) -> Tuple[List[Tuple[float, float]], List[List[Tuple[float, float]]]]:
   # Phase 1: Repairs broken raw deprojection topology vertex by vertex.
   phase1_ext = []
   for lon, lat in exterior_coords:
      phase1_ext.append((wrap_lon_at(lon, zone_c_lon), lat))

   phase1_holes = []
   for h in holes_coords or []:
      if not h:
         continue
      stabilized_h = [(wrap_lon_at(lon, zone_c_lon), lat) for lon, lat in h]
      phase1_holes.append(stabilized_h)

   return phase1_ext, phase1_holes

def _process_hole_subtraction(outer_poly: Polygon, shifted_holes: List[List[Tuple[float, float]]],
                              xmin: float, ymin: float, xmax: float, ymax: float, fid: Any) -> List[Polygon]:
   # Clips holes, clears shell geometry self-intersections, and executes difference operations.
   hole_polys = []
   for sh_h in shifted_holes:
      clipped_h = rect_clip_polygon(sh_h, xmin, ymin, xmax, ymax)
      if not clipped_h:
         continue
      clipped_h = collapse_near_duplicates(clipped_h, eps=DUP_EPS)
      if clipped_h and clipped_h == clipped_h[-1]:
         clipped_h = clipped_h[:-1]
      if len(clipped_h) < 3:
         continue
      hp = Polygon(clipped_h)
      if not hp.is_empty and hp.area > 0.0:
         hole_polys.append(hp)

   outer_poly = outer_poly.buffer(0)

   if not hole_polys:
      return [outer_poly]

   hole_union = unary_union(hole_polys)
   if hole_union.is_empty:
      return [outer_poly]

   try:
      result = outer_poly.difference(hole_union)
      if result.geom_type == "Polygon":
         return [result]
      elif result.geom_type == "MultiPolygon":
         return list(result.geoms)
      elif result.geom_type == "GeometryCollection":
         return [sub for sub in result.geoms if sub.geom_type == "Polygon"]
   except Exception:
      print(f"\nWARNING: Error subtracting holes for feature {fid}")

   return [outer_poly]

def _tile_and_clip_polygon(exterior_coords: List[Tuple[float, float]],
                           holes_coords: List[List[Tuple[float, float]]],
                           zone_extent, zone_tile_eps, fid, part_idx: int, zone_c_lon: float) -> List[Dict[str, Any]]:

   base_poly = Polygon(exterior_coords, holes_coords)
   if base_poly.is_empty:
      return []

   # Guard folder and filename creation under DEBUG status to save memory allocations
   if DEBUG:
      milestone_dir = os.path.join(os.getcwd(), "debugMilestones")
      for step_idx in range(6):
         os.makedirs(os.path.join(milestone_dir, f"step{step_idx}"), exist_ok=True)

      try:
         ext_slug = f"ext_{int(zone_extent[0])}_{int(zone_extent[1])}_{int(zone_extent[2])}_{int(zone_extent[3])}"
      except (TypeError, IndexError):
         ext_slug = "ext_unknown"

      ctx_filename = f"{ext_slug}_fid_{fid}_part_{part_idx}.geojson"

      # --- STEP 0: RAW FUNCTION INPUT ---
      _write_milestone_file(os.path.join(milestone_dir, "step0", ctx_filename), {
         "type": "Feature",
         "properties": {"step": "0_raw_function_input", "zone_extent": zone_extent, "fid": fid, "part_idx": part_idx, "zone_c_lon": zone_c_lon},
         "geometry": {"type": "Polygon", "coordinates": [_fmt_closed(exterior_coords)]}
      })

   phase1_ext, phase1_holes = _execute_phase1_topology_wrap(exterior_coords, holes_coords, zone_c_lon)
   if not phase1_ext:
      return []

   pieces: List[Dict[str, Any]] = []
   global_accumulated_step4_features = []
   global_accumulated_step5_features = []

   for q, (xmin, ymin, xmax, ymax) in enumerate(TILES_4):
      tile_center = 0.5 * (xmin + xmax)
      tile_label = f"q{q}_{int(xmin)}_{int(xmax)}"
      is_target_tile = (abs(xmin - 90.0) < 1e-3 and abs(xmax - 180.0) < 1e-3)

      wrapped_zone_lon = wrap_lon_at(zone_c_lon, tile_center)
      tile_shift = wrapped_zone_lon - zone_c_lon

      shifted_ext = [(lon + tile_shift, lat) for lon, lat in phase1_ext]
      shifted_holes = [[(lon + tile_shift, lat) for lon, lat in h] for h in phase1_holes]

      s_lons = [float(pt[0]) for pt in shifted_ext]
      s_lats = [float(pt[1]) for pt in shifted_ext]
      shifted_p1_extent = [min(s_lons), min(s_lats), max(s_lons), max(s_lats)]

      # --- STEP 1: PRE-CLIP QUADRANT INPUT ---
      if DEBUG:
         _write_milestone_file(os.path.join(milestone_dir, "step1", f"{tile_label}_{ctx_filename}"), {
            "type": "Feature",
            "properties": {"step": "1_pre_clip_shifted_input", "extent_slug": ext_slug, "tile_bounds": [xmin, ymin, xmax, ymax]},
            "geometry": {"type": "Polygon", "coordinates": [_fmt_closed(shifted_ext)]}
         })

      if not intersects_extent_deg((xmin, ymin, xmax, ymax), shifted_p1_extent, zone_tile_eps):
         continue

      if DEBUG:
         debug_ext_closed = list(shifted_ext)
         if debug_ext_closed and debug_ext_closed != debug_ext_closed[-1]:
            debug_ext_closed.append(debug_ext_closed[0])
         _debug_record_tile(xmin, xmax, {
            "type": "Feature",
            "properties": {"fid": fid, "part_idx": part_idx, "tile_xmin": xmin, "tile_xmax": xmax},
            "geometry": {"type": "Polygon", "coordinates": [[[float(x), float(y)] for (x, y) in debug_ext_closed]]}
         })

      clipped = rect_clip_polygon(shifted_ext, xmin, ymin, xmax, ymax)
      if not clipped:
         continue

      # --- STEP 2: IMMEDIATE CLIPPER OUTPUT ---
      if DEBUG:
         _write_milestone_file(os.path.join(milestone_dir, "step2", f"{tile_label}_{ctx_filename}"), {
            "type": "Feature",
            "properties": {"step": "2_result_immediate_after_clip", "extent_slug": ext_slug, "vertex_count": len(clipped)},
            "geometry": {"type": "Polygon", "coordinates": [_fmt_closed(clipped)]}
         })

      clipped = collapse_near_duplicates(clipped, eps=DUP_EPS)
      if clipped and clipped == clipped[-1]:
         clipped = clipped[:-1]
      if len(clipped) < 3:
         continue
      outer_poly = Polygon(clipped)
      if outer_poly.is_empty or outer_poly.area <= 0.0:
         continue

      # --- STEP 3: CLOSED RING POLYGON ---
      if DEBUG:
         _write_milestone_file(os.path.join(milestone_dir, "step3", f"{tile_label}_{ctx_filename}"), {
            "type": "Feature",
            "properties": {"step": "3_outer_poly_ring_built", "extent_slug": ext_slug, "is_valid": outer_poly.is_valid},
            "geometry": {"type": "Polygon", "coordinates": [_fmt_closed(outer_poly.exterior.coords)]}
         })

      raw_output_polys = _process_hole_subtraction(outer_poly, shifted_holes, xmin, ymin, xmax, ymax, fid)
      final_polys = _flatten_to_simple_polygons(raw_output_polys)

      # Only accumulate the milestone array payloads if actively debugging
      if DEBUG:
         for idx, p in enumerate(final_polys):
            global_accumulated_step4_features.append({
               "type": "Feature",
               "properties": {"tile_q": q, "part_idx": idx, "geom_type": p.geom_type, "bounds": [xmin, xmax]},
               "geometry": {"type": "Polygon", "coordinates": [_fmt_closed(p.exterior.coords)]}
            })

      for g_idx, global_poly in enumerate(final_polys):
         if DEBUG and is_target_tile:
            global_accumulated_step5_features.append({
               "type": "Feature",
               "properties": {"tile_q": q, "global_idx": g_idx, "is_valid": global_poly.is_valid, "bounds": [xmin, xmax]},
               "geometry": {"type": "Polygon", "coordinates": [_fmt_closed(global_poly.exterior.coords)]}
            })

         if global_poly.geom_type == "Polygon":
            piece = _accept_polygon_piece(global_poly, xmin, ymin, xmax, ymax, fid, part_idx)
            if piece: pieces.append(piece)
         elif global_poly.geom_type == "MultiPolygon":
            for sub in global_poly.geoms:
               if sub.geom_type == "Polygon":
                  piece = _accept_polygon_piece(sub, xmin, ymin, xmax, ymax, fid, part_idx)
                  if piece: pieces.append(piece)

   # --- WRITE OUT COMBINED PASSTHROUGH COLLECTIONS INTO THEIR OWN STEPS ---
   if DEBUG:
      if global_accumulated_step4_features:
         _write_milestone_file(os.path.join(milestone_dir, "step4", f"combined_{ctx_filename}"), {
            "type": "FeatureCollection", "features": global_accumulated_step4_features
         })

      if global_accumulated_step5_features:
         _write_milestone_file(os.path.join(milestone_dir, "step5", f"combined_{ctx_filename}"), {
            "type": "FeatureCollection", "features": global_accumulated_step5_features
         })

   return pieces

# ---------- wrapper that accepts exterior+holes or polygon-like lists ----------
def _tile_and_clip(exterior_or_poly, holes_coords: Optional[List[List[Tuple[float, float]]]],
   zone_extent, eps_zone_tile, fid, part_idx: int, zone_c_lon: float,
   original_geom: Optional[Dict[str,Any]] = None) -> List[Dict[str, Any]]:
   # exterior_or_poly can be a shapely Polygon/MultiPolygon or a list of coords (exterior)
   if hasattr(exterior_or_poly, "exterior"):
      if exterior_or_poly.geom_type == "Polygon":
         ext = list(exterior_or_poly.exterior.coords)
         holes = [list(h.coords) for h in exterior_or_poly.interiors]
         return _tile_and_clip_polygon(ext, holes, zone_extent, eps_zone_tile, fid, part_idx, zone_c_lon)
      pieces: List[Dict[str, Any]] = []
      for sub in exterior_or_poly.geoms:
         ext = list(sub.exterior.coords)
         holes = [list(h.coords) for h in sub.interiors]
         pieces.extend(_tile_and_clip_polygon(ext, holes, zone_extent, eps_zone_tile, fid, part_idx, zone_c_lon))
      return pieces
   return _tile_and_clip_polygon(exterior_or_poly, holes_coords or [], zone_extent, eps_zone_tile, fid, part_idx, zone_c_lon)

# ---------- assemble features from pieces (disabled union to avoid GEOS errors) ----------
def _assemble_feature_from_pieces(all_kept_pieces: List[Dict[str, Any]], fid: str, props: Dict[str, Any]) -> List[Dict[str, Any]]:
   polys: List[Polygon] = []
   for p in all_kept_pieces:
      g = p["geom"]
      if g.geom_type == "Polygon":
         polys.append(g)
      elif g.geom_type == "MultiPolygon":
         for sub in g.geoms:
            polys.append(sub)
      else:
         for sub in getattr(g, "geoms", []) or []:
            if sub.geom_type == "Polygon":
               polys.append(sub)

   valid_polys: List[Polygon] = []
   for p in polys:
      if not p.is_empty and p.area > _AREA_EPS and len(list(p.exterior.coords)) >= 4:
         valid_polys.append(p)
      else:
         rp = p.buffer(0)
         if rp.geom_type == "Polygon" and not rp.is_empty and rp.area > _AREA_EPS:
            valid_polys.append(rp)
         elif rp.geom_type == "MultiPolygon":
            for sub in rp.geoms:
               if not sub.is_empty and sub.area > _AREA_EPS:
                  valid_polys.append(sub)

   merged, skipped = _staged_union_polygons(valid_polys, fid)

   out_features: List[Dict[str, Any]] = []
   if merged is None:
      for i, p in enumerate(valid_polys):
         out_features.append({"type": "Feature", "id": f"{fid}_part_{i}", "properties": props, "geometry": mapping(p)})
      return out_features

   if merged.geom_type in ("Polygon", "MultiPolygon"):
      out_features.append({"type": "Feature", "id": fid, "properties": props, "geometry": mapping(merged)})
      return out_features

   polys2: List[Polygon] = []
   for g in getattr(merged, "geoms", []) or []:
      if g.geom_type == "Polygon":
         polys2.append(g)
      elif g.geom_type == "MultiPolygon":
         for sub in g.geoms:
            polys2.append(sub)
   if not polys2:
      out_features.append({"type": "Feature", "id": fid, "properties": props, "geometry": mapping(Polygon())})
   elif len(polys2) == 1:
      out_features.append({"type": "Feature", "id": fid, "properties": props, "geometry": mapping(polys2[0])})
   else:
      out_features.append({"type": "Feature", "id": fid, "properties": props, "geometry": mapping(MultiPolygon(polys2))})
   return out_features

# ---------- coords lon range helper ----------
def _coords_lon_range(coords: Any) -> Tuple[float, float]:
   if coords is None:
      return (9999.0, -9999.0)
   if isinstance(coords, list):
      if not coords:
         return (9999.0, -9999.0)
      first = coords[0]
      if isinstance(first, (int, float)):
         lon = float(first)
         return (lon, lon)
      minlon = 9999.0
      maxlon = -9999.0
      for item in coords:
         a, b = _coords_lon_range(item)
         if a < minlon:
            minlon = a
         if b > maxlon:
            maxlon = b
      return (minlon, maxlon)
   if isinstance(coords, (int, float)):
      lon = float(coords)
      return (lon, lon)
   return (9999.0, -9999.0)

# ---------- ensure output family and drop degenerate families ----------
def _ensure_output_type(merged, orig_type: str):
   if merged is None:
      return None
   # If merged is a list of shapely geometries (we disabled unary_union), wrap into GeometryCollection
   if isinstance(merged, list):
      merged = GeometryCollection(merged)

   if getattr(merged, "is_empty", False):
      return None

   if orig_type == "Point":
      if merged.geom_type == "Point":
         return mapping(merged)
      if merged.geom_type == "MultiPoint":
         if len(merged.geoms) == 1:
            return mapping(merged.geoms[0])
         return mapping(merged)
      if merged.geom_type == "GeometryCollection":
         pts = [g for g in merged.geoms if g.geom_type == "Point"]
         if not pts:
            return None
         if len(pts) == 1:
            return mapping(pts[0])
         return mapping(MultiPoint([p.coords[0] for p in pts]))
      return None

   if orig_type == "MultiPoint":
      if merged.geom_type == "MultiPoint":
         return mapping(merged)
      if merged.geom_type == "Point":
         return mapping(MultiPoint([merged.coords[0]]))
      if merged.geom_type == "GeometryCollection":
         coords = []
         for g in merged.geoms:
            if g.geom_type == "Point":
               coords.append(g.coords[0])
            elif g.geom_type == "MultiPoint":
               for p in g.geoms:
                  coords.append(p.coords[0])
         if not coords:
            return None
         return mapping(MultiPoint(coords))
      return None

   if orig_type == "LineString":
      if merged.geom_type == "LineString":
         return mapping(merged)
      if merged.geom_type == "MultiLineString":
         if len(merged.geoms) == 1:
            return mapping(merged.geoms[0])
         return mapping(merged)
      if merged.geom_type == "GeometryCollection":
         lines = []
         for g in merged.geoms:
            if g.geom_type == "LineString":
               lines.append(g)
            elif g.geom_type == "MultiLineString":
               for sub in g.geoms:
                  lines.append(sub)
         if not lines:
            return None
         if len(lines) == 1:
            return mapping(lines[0])
         return mapping(MultiLineString([list(l.coords) for l in lines]))
      return None

   if orig_type == "MultiLineString":
      if merged.geom_type == "MultiLineString":
         return mapping(merged)
      if merged.geom_type == "LineString":
         return mapping(MultiLineString([list(merged.coords)]))
      if merged.geom_type == "GeometryCollection":
         parts = []
         for g in merged.geoms:
            if g.geom_type == "LineString":
               parts.append(list(g.coords))
            elif g.geom_type == "MultiLineString":
               for sub in g.geoms:
                  parts.append(list(sub.coords))
         if not parts:
            return None
         return mapping(MultiLineString(parts))
      return None

   if orig_type == "Polygon":
      if merged.geom_type == "Polygon":
         return mapping(merged)
      if merged.geom_type == "MultiPolygon":
         if len(merged.geoms) == 1:
            return mapping(merged.geoms[0])
         return mapping(merged)
      if merged.geom_type == "GeometryCollection":
         polys = []
         for g in merged.geoms:
            if g.geom_type == "Polygon":
               polys.append(g)
            elif g.geom_type == "MultiPolygon":
               for sub in g.geoms:
                  polys.append(sub)
         if not polys:
            return None
         if len(polys) == 1:
            return mapping(polys[0])
         return mapping(MultiPolygon(polys))
      return None

   if orig_type == "MultiPolygon":
      if merged.geom_type == "MultiPolygon":
         return mapping(merged)
      if merged.geom_type == "Polygon":
         return mapping(MultiPolygon([merged]))
      if merged.geom_type == "GeometryCollection":
         parts = []
         for g in merged.geoms:
            if g.geom_type == "Polygon":
               parts.append(g)
            elif g.geom_type == "MultiPolygon":
               for sub in g.geoms:
                  parts.append(sub)
         if not parts:
            return None
         return mapping(MultiPolygon(parts))
      return None

   return mapping(merged)

def _process_single_geometry(geom: Optional[Dict[str, Any]], zone_extent: List[float], eps_zone_tile, fid_for_debug = None) -> Optional[Dict[str, Any]]:
   if geom is None:
      return None

   orig_type = geom["type"]
   tile_geoms = []

   # Safe geographic mean calculation for extents that cross the dateline (xmin > xmax)
   z_xmin, z_ymin, z_xmax, z_ymax = zone_extent
   if z_xmin > z_xmax:
      zone_c_lon = 0.5 * (z_xmin + z_xmax + 360.0)
      if zone_c_lon > 180.0:
         zone_c_lon -= 360.0
   else:
      zone_c_lon = 0.5 * (z_xmin + z_xmax)

   if orig_type == "Polygon":
      ext = geom["coordinates"][0]
      holes = geom["coordinates"][1:]
      pieces = _tile_and_clip(ext, holes, zone_extent, eps_zone_tile, fid_for_debug, 0, zone_c_lon)
      for p in pieces:
         tile_geoms.append(p["geom"])

   elif orig_type == "MultiPolygon":
      for i, poly in enumerate(geom["coordinates"]):
         ext = poly[0]
         holes = poly[1:]
         pieces = _tile_and_clip(ext, holes, zone_extent, eps_zone_tile, fid_for_debug, i, zone_c_lon)
         for p in pieces:
            tile_geoms.append(p["geom"])
   else:
      shp = shape(geom)
      for xmin, ymin, xmax, ymax in TILES_4:
         tile_box = box(xmin, ymin, xmax, ymax)
         if shp.intersects(tile_box):
            inter = shp.intersection(tile_box)
            if not inter.is_empty:
               tile_geoms.append(inter)

   if not tile_geoms:
      return None

   merged = unary_union(tile_geoms)

   if orig_type in ("Polygon", "MultiPolygon"):
      all_kept_pieces = [{"geom": g} for g in tile_geoms]
      assembled = _assemble_feature_from_pieces(all_kept_pieces, fid_for_debug, props={})
      if not assembled:
         if DEBUG:
            _debug_write_files()
         return None
      polys = []
      for f in assembled:
         g = f["geometry"]
         if g is None:
            continue
         if g["type"] == "Polygon":
            polys.append(orient(Polygon(g["coordinates"][0], g["coordinates"][1:]), sign=1.0))
         elif g["type"] == "MultiPolygon":
            for sub in g["coordinates"]:
               polys.append(orient(Polygon(sub[0], sub[1:]), sign=1.0))
      if not polys:
         if DEBUG:
            _debug_write_files()
         return None
      if len(polys) == 1:
         if DEBUG:
            _debug_write_files()
         return mapping(polys[0])
      if DEBUG:
         _debug_write_files()
      return mapping(MultiPolygon(polys))

   # For non-polygon families, coerce merged (list) into GeometryCollection and ensure output type
   if DEBUG:
      _debug_write_files()
   return _ensure_output_type(merged, orig_type)

# ---------- main public function ----------
def fix_WGS84_geometry(obj: Any, zone_extent: List[float], eps_zone_tile = EPS_ZONE_TILE, fid = None) -> Any:
   # quick dateline check
   #if obj["type"] == "FeatureCollection":
   #   minlon = 9999.0
   #   maxlon = -9999.0
   #   for feat in obj["features"]:
   #      geom = feat["geometry"]
   #      a, b = _coords_lon_range(geom["coordinates"] if geom is not None else None)
   #      if a < minlon:
   #         minlon = a
   #      if b > maxlon:
   #         maxlon = b
   #   if maxlon - minlon < 180.0:
   #      return obj
   #else:
   #   geom = obj["geometry"] if obj["type"] == "Feature" else obj
   #   a, b = _coords_lon_range(geom["coordinates"] if geom is not None else None)
   #   if b - a < 180.0:
   #      return obj

   #print("Zone extent:", zone_extent)

   # preserve top-level shape
   if obj["type"] == "FeatureCollection":
      out = {"type": "FeatureCollection", "features": []}
      for feat in obj["features"]:
         geom = feat["geometry"]
         if geom is None:
            new_feat = dict(feat)
            new_feat["geometry"] = None
            out["features"].append(new_feat)
            continue
         fid = feat.get("id", str(len(out["features"])))
         fixed = _process_single_geometry(geom, zone_extent, eps_zone_tile, fid_for_debug=fid)
         new_feat = dict(feat)
         new_feat["geometry"] = fixed
         out["features"].append(new_feat)
      return out

   if obj["type"] == "Feature":
      geom = obj["geometry"]
      if geom is None:
         return obj
      fid = obj.get("id", "0")
      fixed = _process_single_geometry(geom, zone_extent, eps_zone_tile, fid_for_debug=fid)
      out = dict(obj)
      out["geometry"] = fixed
      return out

   return _process_single_geometry(obj, zone_extent, eps_zone_tile, fid_for_debug=fid)
