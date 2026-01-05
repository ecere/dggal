#!/usr/bin/env python3
# 5x6 space geometry topology-fixer
# - should correctly handle inner-rings (holes) per polygon
# - using Sutherland-Hodgman clipping and distance5x6 insertion logic
# - strict per-polygon processing: we expand MultiPolygon into polygon parts
# - minimal, deterministic fallback if hole subtraction raises

from typing import List, Tuple, Dict, Any, Optional
from copy import deepcopy
import json
import math
import sys

from shapely.geometry import shape, mapping, Polygon, MultiPolygon
from shapely.ops import unary_union
from shapely.affinity import rotate
import shapely

from fg.faces import Pointd
from fg.distance import distance5x6
from fg.sutherlandHodgman import *

# Configuration (kept as in working version)
_PERIOD = 5.0
_TINY = 1e-14
_MAX_ITERS = 512            # safety bound for iterative intersection insertion
_AREA_EPS = 1e-12
_EPS_TIE = 1e-12

TILE_X_COUNT = 5
TILE_Y_COUNT = 6

DUP_EPS = 1e-10

from typing import List, Tuple

def collapse_near_duplicates(ring: List[Tuple[float, float]], eps: float = 1e-9) -> List[Tuple[float, float]]:
    # Remove consecutive nearly identical vertices using Manhattan distance.
    # Preserves a closing duplicate at the end. No area or validity checks.

    if not ring:
        return ring

    # ensure closing duplicate for processing
    if ring[0] != ring[-1]:
        ring = list(ring) + [ring[0]]
    else:
        ring = list(ring)

    out: List[Tuple[float, float]] = [ring[0]]
    for x, y in ring[1:]:
        prev_x, prev_y = out[-1]
        if abs(x - prev_x) + abs(y - prev_y) > eps:
            out.append((x, y))

    # ensure closure
    if out[0] != out[-1]:
        out.append(out[0])

    return out

# ---------------------------
# distance5x6 insertion
# ---------------------------
def _within_same_tile(a: Tuple[float,float], b: Tuple[float,float]) -> bool:
   return (math.floor(a[0]) == math.floor(b[0])) and (math.floor(a[1]) == math.floor(b[1]))

def _choose_segment_shift(a: Tuple[float,float], b: Tuple[float,float], period: float = _PERIOD, max_shift: int = 2) -> Tuple[int,int]:
   ax, ay = a; bx, by = b
   best = (0, 0)
   best_score = float("inf")
   for sx in range(-max_shift, max_shift + 1):
      tx = bx + sx * period
      dx2 = (tx - ax) * (tx - ax)
      for sy in range(-max_shift, max_shift + 1):
         ty = by + sy * period
         dy2 = (ty - ay) * (ty - ay)
         score = dx2 + dy2
         if score < best_score:
            best_score = score
            best = (sx, sy)
   return best

def _insert_segment_exact(a: Tuple[float,float], b: Tuple[float,float],
                          feature_id: str, seg_index: int,
                          seg_debug_store: Dict[int, List[Dict[str,Any]]],
                          period: float = _PERIOD, max_shift: int = 2) -> List[Tuple[float,float]]:
   out_pts: List[Tuple[float,float]] = []
   cur = Pointd(float(a[0]), float(a[1]))
   tgt = Pointd(float(b[0]), float(b[1]))
   per_debug: List[Dict[str,Any]] = []

   if _within_same_tile((cur.x,cur.y),(tgt.x,tgt.y)):
      seg_debug_store[seg_index] = per_debug
      return [(tgt.x, tgt.y)]

   iters = 0
   while True:
      iters += 1
      if iters > _MAX_ITERS:
         per_debug.append({"error":"max_iters"})
         out_pts.append((tgt.x, tgt.y))
         break

      distance, b_in_a_frame, mod_a, i_src, i_dst, in_north, ends_at_edge = distance5x6(cur, tgt)

      entry = {
         "iter": iters,
         "current": (cur.x, cur.y),
         "target": (tgt.x, tgt.y),
         "i_src": (i_src.x, i_src.y) if i_src else None,
         "i_dst": (i_dst.x, i_dst.y) if i_dst else None,
         "in_north": in_north,
         "ends_at_edge": ends_at_edge,
         "note": "raw-endpoints-call"
      }
      per_debug.append(entry)

      if i_src is None or i_dst is None:
         out_pts.append((tgt.x, tgt.y))
         break

      i_src_global = (i_src.x, i_src.y)
      i_dst_global = (i_dst.x, i_dst.y)

      last = out_pts[-1] if out_pts else (cur.x, cur.y)
      if not (abs(last[0] - i_src_global[0]) < _TINY and abs(last[1] - i_src_global[1]) < _TINY):
         out_pts.append(i_src_global)
      last = out_pts[-1] if out_pts else (cur.x, cur.y)
      if not (abs(last[0] - i_dst_global[0]) < _TINY and abs(last[1] - i_dst_global[1]) < _TINY):
         out_pts.append(i_dst_global)

      cur = Pointd(i_dst.x, i_dst.y)

      if abs(cur.x - tgt.x) < _TINY and abs(cur.y - tgt.y) < _TINY:
         if not (abs(out_pts[-1][0] - tgt.x) < _TINY and abs(out_pts[-1][1] - tgt.y) < _TINY):
            out_pts.append((tgt.x, tgt.y))
         break

   seg_debug_store[seg_index] = per_debug
   return out_pts

def _insert_ring_coords(coords: List[Tuple[float,float]], fid: str, part_idx: int) -> Tuple[List[Tuple[float,float]], Dict[int, List[Dict[str,Any]]]]:
   seg_debug_store: Dict[int, List[Dict[str,Any]]] = {}
   out_coords: List[Tuple[float,float]] = [coords[0]]
   for i in range(len(coords)-1):
      a = coords[i]; b = coords[i+1]
      inserted = _insert_segment_exact(a, b, fid, i, seg_debug_store)
      for p in inserted:
         if out_coords and (abs(out_coords[-1][0]-p[0]) < _TINY and abs(out_coords[-1][1]-p[1]) < _TINY):
            continue
         out_coords.append(p)
   if out_coords and (out_coords[0] != out_coords[-1]):
      out_coords.append(out_coords[0])
   return out_coords, seg_debug_store

def _candidate_tiles_from_vertices(coords: List[Tuple[float,float]], margin_neighbors: int = 1) -> List[Tuple[int,int]]:
   tiles = set()
   ring = list(coords)
   if ring and ring[0] != ring[-1]:
      ring.append(ring[0])
   shifts = (0.0, -_PERIOD, +_PERIOD)
   for (x,y) in ring:
      for s in shifts:
         nx = x + s
         ny = y + s
         tx = math.floor(nx)
         ty = math.floor(ny)
         for dx in range(-margin_neighbors, margin_neighbors+1):
            for dy in range(-margin_neighbors, margin_neighbors+1):
               ttx = tx + dx
               tty = ty + dy
               if 0 <= ttx < TILE_X_COUNT and 0 <= tty < TILE_Y_COUNT:
                  tiles.add((ttx, tty))
   return sorted(list(tiles))

# ---------------------------
# Shift selection (prev-vertex continuity rule)
# ---------------------------
from typing import List, Tuple, Optional

def _choose_shifts_prev_vertex(coords: List[Tuple[float, float]],
                               tx: int,
                               ty: int,
                               fid_for_debug: Optional[str] = None) -> List[float]:
    SHIFT = _PERIOD
    EPS_TIE = _EPS_TIE
    cx = tx + 0.5
    cy = ty + 0.5

    ring = list(coords)
    if ring and ring[0] != ring[-1]:
        ring.append(ring[0])
    n = len(ring)
    if n == 0:
        return []

    closed = (ring[0] == ring[-1])
    m = n - 1 if closed else n  # number of unique vertices

    def l1(p, qx, qy):
        return abs(p[0] - qx) + abs(p[1] - qy)

    candidates = [0.0, SHIFT, -SHIFT]
    shifts: List[float] = [0.0] * n

    # per-vertex best single shift (Manhattan)
    best_single_shift: List[float] = [0.0] * n
    best_single_l1: List[float] = [0.0] * n
    for i in range(n):
        xi, yi = ring[i]
        best_s = 0.0
        best_d = l1((xi, yi), cx, cy)
        d_plus = l1((xi + SHIFT, yi + SHIFT), cx, cy)
        d_minus = l1((xi - SHIFT, yi - SHIFT), cx, cy)
        if d_plus + EPS_TIE < best_d:
            best_d = d_plus; best_s = SHIFT
        if d_minus + EPS_TIE < best_d:
            best_d = d_minus; best_s = -SHIFT
        best_single_shift[i] = best_s
        best_single_l1[i] = best_d

    # choose anchor by minimal best-single L1 (exclude duplicate closing vertex)
    anchor = 0
    min_d = None
    for i in range(m):
        if min_d is None or best_single_l1[i] < min_d:
            min_d = best_single_l1[i]
            anchor = i

    # --- conditional uniform-seed shortcut (only when anchor is first and span test) ---
    try:
        poly_orig = Polygon(ring)
    except Exception:
        poly_orig = None
    if best_single_shift[anchor] == 0.0 and poly_orig is not None and poly_orig.is_valid and len(poly_orig.interiors) == 0 and poly_orig.area > _AREA_EPS:
        xs = [p[0] for p in ring[:m]]
        if xs:
            if (max(xs) - min(xs)) > 3:
                seed = best_single_shift[anchor]
                shifted_coords = [(x + seed, y + seed) for (x, y) in ring]
                try:
                    poly_shifted = Polygon(shifted_coords)
                except Exception:
                    poly_shifted = None
                if poly_shifted is not None and poly_shifted.is_valid and len(poly_shifted.interiors) == 0 and poly_shifted.area > _AREA_EPS:
                    return [float(seed) for _ in range(n)]
    # ------------------------------------------------------------------------------

    # seed anchor with its best single shift
    shifts[anchor] = best_single_shift[anchor]

    # forward propagation (anchor+1 .. n-1)
    prev_x = ring[anchor][0] + shifts[anchor]
    prev_y = ring[anchor][1] + shifts[anchor]
    for i in range(anchor + 1, n):
        xi, yi = ring[i]
        ordered_candidates = [shifts[i - 1]] + [c for c in candidates if c != shifts[i - 1]]
        best_s = None
        best_score = None
        for s in ordered_candidates:
            lx = xi + s
            ly = yi + s
            score = abs(lx - prev_x) + abs(ly - prev_y)
            if best_score is None or score + EPS_TIE < best_score:
                best_score = score
                best_s = s
        shifts[i] = best_s if best_s is not None else 0.0
        prev_x = xi + shifts[i]
        prev_y = yi + shifts[i]

    # backward propagation (anchor-1 .. 0)
    for i in range(anchor - 1, -1, -1):
        xi, yi = ring[i]
        ordered_candidates = [shifts[i + 1]] + [c for c in candidates if c != shifts[i + 1]]
        best_s = None
        best_score = None
        next_x = ring[i + 1][0] + shifts[i + 1]
        next_y = ring[i + 1][1] + shifts[i + 1]
        for s in ordered_candidates:
            lx = xi + s
            ly = yi + s
            score = abs(lx - next_x) + abs(ly - next_y)
            if best_score is None or score + EPS_TIE < best_score:
                best_score = score
                best_s = s
        shifts[i] = best_s if best_s is not None else 0.0

    # n >= 3 special-case pass (preserve continuity across closure) using Manhattan metrics
    if n >= 3:
        first_x, first_y = ring[0][0] + shifts[0], ring[0][1] + shifts[0]
        last_idx = n - 1
        last_x, last_y = ring[last_idx][0] + shifts[last_idx], ring[last_idx][1] + shifts[last_idx]
        prev_idx = last_idx - 1
        prev_local_x, prev_local_y = ring[prev_idx][0] + shifts[prev_idx], ring[prev_idx][1] + shifts[prev_idx]
        close_l1 = abs(last_x - first_x) + abs(last_y - first_y)
        avg_edge = (abs(last_x - prev_local_x) + abs(last_y - prev_local_y) +
                    abs(prev_local_x - first_x) + abs(prev_local_y - first_y)) / 2.0
        if avg_edge > 0 and close_l1 > 16.0 * avg_edge:
            ordered_candidates = [shifts[prev_idx]] + [c for c in candidates if c != shifts[prev_idx]]
            best_s = shifts[last_idx]
            best_close = close_l1
            cont_current = abs(last_x - prev_local_x) + abs(last_y - prev_local_y)
            for s in ordered_candidates:
                lx = ring[last_idx][0] + s
                ly = ring[last_idx][1] + s
                new_close = abs(lx - first_x) + abs(ly - first_y)
                cont = abs(lx - prev_local_x) + abs(ly - prev_local_y)
                metric = new_close + 4.0 * cont
                current_metric = best_close + 4.0 * cont_current
                if metric + EPS_TIE < current_metric:
                    best_s = s
                    best_close = new_close
                    cont_current = cont
            if best_s != shifts[last_idx]:
                shifts[last_idx] = best_s

    return [float(s) for s in shifts]

# ---------------------------
# Localize ring using chosen shifts
# ---------------------------
def _localized_ring_on_the_fly(coords, tx, ty, fid=""):
   ring = list(coords)
   if ring and ring[0] != ring[-1]:
      ring.append(ring[0])
   shifts = _choose_shifts_prev_vertex(ring, tx, ty, fid_for_debug=fid)
   localized = []
   for i, (x, y) in enumerate(ring):
      s = shifts[i] if i < len(shifts) else 0.0
      localized.append((x + s, y + s))
   if localized and localized[0] != localized[-1]:
      localized.append(localized[0])
   return localized, None

# ---------------------------
# Tile clipping and assembly (core behavior)
# This version is polygon-part aware: it accepts an exterior ring and a list of hole rings.
# ---------------------------
def _tile_and_filter_staircase_polygon(exterior_coords: List[Tuple[float,float]],
                                       holes_coords: List[List[Tuple[float,float]]],
                                       fid: str, part_idx: int) -> List[Dict[str,Any]]:
   # Build a polygon from the provided rings for area checks; we do not rely on its validity for clipping
   try:
      poly = Polygon(exterior_coords, holes_coords)
   except Exception:
      poly = Polygon(exterior_coords)

   if poly.is_empty:
      return []

   candidates = _candidate_tiles_from_vertices(exterior_coords, margin_neighbors=1)
   pieces: List[Dict[str,Any]] = []
   for tx, ty in candidates:
      #if tx != 1 or ty != 1: continue
      #if fid != 221: continue

      if not (ty == tx or ty == tx + 1):
         continue

      # Localize exterior and holes using the same shift logic
      localized_ext, _ = _localized_ring_on_the_fly(exterior_coords, tx, ty, fid=str(fid))
      localized_holes: List[List[Tuple[float,float]]] = []
      for h in holes_coords or []:
         if not h:
            continue
         # ensure ring closure handling
         hh = list(h)
         if hh and hh[0] != hh[-1]:
            hh.append(hh[0])
         # localize hole relative to exterior shifts by choosing nearest exterior vertex shift
         # reuse _choose_shifts_prev_vertex logic indirectly via _localized_ring_on_the_fly on exterior
         # compute localized hole by mapping each hole vertex to nearest exterior index shift
         # (this mirrors earlier behavior where holes are attached to exterior via nearest vertex)
         # For simplicity and determinism we compute shifts from exterior and apply to hole vertices
         ext_ring = list(exterior_coords)
         if ext_ring and ext_ring[0] != ext_ring[-1]:
            ext_ring.append(ext_ring[0])
         shifts = _choose_shifts_prev_vertex(ext_ring, tx, ty, fid_for_debug=str(fid))
         # drop last duplicate in ext_ring to align shifts length
         if shifts and len(shifts) == len(ext_ring):
            shifts_use = shifts[:-1]
         else:
            shifts_use = shifts
         localized_h = []
         for (hx, hy) in hh:
            # find nearest exterior vertex index
            best_idx = 0
            best_d = None
            for idx, (exx, exy) in enumerate(ext_ring[:-1]):
               dx = hx - exx; dy = hy - exy
               d = dx*dx + dy*dy
               if best_d is None or d < best_d:
                  best_d = d; best_idx = idx
            s = shifts_use[best_idx] if best_idx < len(shifts_use) else 0.0
            localized_h.append((hx + s, hy + s))
         if localized_h and (localized_h[0] != localized_h[-1]):
            localized_h.append(localized_h[0])
         if len(localized_h) >= 3:
            localized_holes.append(localized_h)

      # Clip exterior ring
      clipped = rect_clip_polygon(localized_ext, tx, ty, tx+1, ty+1)
      if not clipped:
         continue

      clipped = collapse_near_duplicates(clipped, eps=DUP_EPS)

      # Clip holes individually and build hole polygons
      hole_polys: List[Polygon] = []
      for hh in localized_holes:
         clipped_h = rect_clip_polygon(hh, tx, ty, tx+1, ty+1)
         if not clipped_h:
            continue

         clipped_h = collapse_near_duplicates(clipped_h, eps=DUP_EPS)

         if clipped_h and clipped_h[0] == clipped_h[-1]:
            clipped_h = clipped_h[:-1]
         if len(clipped_h) < 3:
            continue
         hp = Polygon(clipped_h)
         if hp.is_empty or hp.area <= 0.0:
            continue
         hole_polys.append(hp)

      # Build outer polygon from clipped exterior
      if clipped and clipped[0] == clipped[-1]:
         clipped = clipped[:-1]
      if len(clipped) < 3:
         continue
      outer_poly = Polygon(clipped)
      if outer_poly.is_empty or outer_poly.area <= 0.0:
         continue

      # If there are holes, subtract them; if subtraction fails, fall back to outer_poly (preserve prior behavior)
      final_polys: List[Polygon] = []
      if not hole_polys:
         final_polys = [outer_poly]
      else:
         try:
            hole_union = unary_union(hole_polys)
            # short-circuit trivial cases
            if hole_union.is_empty:
               final_polys = [outer_poly]
            else:
               # attempt difference; if it raises, fall back to outer_poly
               try:
                  result = outer_poly.difference(hole_union)
                  if isinstance(result, Polygon):
                     if not result.is_empty and result.area > 0.0:
                        final_polys = [result]
                  else:
                     for sub in getattr(result, "geoms", []) or []:
                        if isinstance(sub, Polygon) and not sub.is_empty and sub.area > 0.0:
                           final_polys.append(sub)
               except Exception:
                  # fallback to outer only (this mirrors tolerant earlier behavior)
                  final_polys = [outer_poly]
         except Exception:
            final_polys = [outer_poly]

      # Filter and accept pieces
      for geom in final_polys:
         if geom is None or geom.is_empty or geom.area <= 0.0:
            continue
         # quick bounding check: ensure coords lie within tile bounds (with tiny tolerance)
         ok = True
         bounding_tol = 1e-12
         for (cx, cy) in list(geom.exterior.coords):
            if not (tx - bounding_tol <= cx <= tx + 1 + bounding_tol and
                    ty - bounding_tol <= cy <= ty + 1 + bounding_tol):
               ok = False; break
         if not ok:
            continue
         if geom.area <= _AREA_EPS:
            continue
         pieces.append({"tile_x": tx, "tile_y": ty, "geom": geom, "orig_fid": fid, "part_idx": part_idx})
   return pieces

# Wrapper that accepts exterior+holes or a Polygon/MultiPolygon shapely object
def _tile_and_filter_staircase(exterior_or_poly, holes_coords: Optional[List[List[Tuple[float,float]]]], fid: str, part_idx: int) -> List[Dict[str,Any]]:
   """
   Accepts either:
    - exterior_or_poly: list of exterior coords and holes_coords provided separately, or
    - exterior_or_poly: a shapely Polygon or MultiPolygon (in which case holes are extracted)
   For compatibility with the original pipeline we support both call styles.
   """
   # If caller passed a shapely Polygon, extract rings
   if isinstance(exterior_or_poly, Polygon):
      ext = list(exterior_or_poly.exterior.coords)
      holes = [list(h.coords) for h in exterior_or_poly.interiors]
      return _tile_and_filter_staircase_polygon(ext, holes, fid, part_idx)
   if isinstance(exterior_or_poly, MultiPolygon):
      pieces: List[Dict[str,Any]] = []
      for idx, sub in enumerate(getattr(exterior_or_poly, "geoms", []) or []):
         if isinstance(sub, Polygon):
            pieces.extend(_tile_and_filter_staircase_polygon(list(sub.exterior.coords),
                                                            [list(h.coords) for h in sub.interiors],
                                                            fid, part_idx))
      return pieces
   # otherwise assume exterior_or_poly is exterior coords list and holes_coords provided
   if isinstance(exterior_or_poly, list):
      return _tile_and_filter_staircase_polygon(exterior_or_poly, holes_coords or [], fid, part_idx)
   raise ValueError("_tile_and_filter_staircase expects exterior coords list or shapely Polygon/MultiPolygon")

# ---------------------------
# Validation, repair, staged union, assembly
# ---------------------------
def _validate_geom(g):
   return (not g.is_empty) and (g.area > 0.0) and g.is_valid

def _repair_geom(g):
   r = g.buffer(0)
   if not r.is_empty and r.area > 0.0 and r.is_valid:
      return r
   return g

#def _staged_union_polygons(polys: List[Polygon], fid: str):
#   if not polys:
#      return None, []
#   skipped = []
#   merged = unary_union(polys)
#   return merged, skipped

def _staged_union_polygons(polys: List[Polygon], fid: str):
   result = None
   if polys:
      result = shapely.union_all(polys, grid_size=1e-10)
      #result = result.buffer( 1e-9)
      #result = result.buffer(-1e-9)

      #result = unary_union(polys, grid_size=1e-9)
      #result = result.buffer(1e-9)  # This is the smallest epsilon that avoids the seams
      #result = MultiPolygon(polys)
   return result, []

def _assemble_feature_from_pieces(all_kept_pieces: List[Dict[str,Any]], fid: str, props: Dict[str,Any]) -> List[Dict[str,Any]]:
   polys = []
   for p in all_kept_pieces:
      g = p["geom"]
      if isinstance(g, Polygon):
         polys.append(g)
      elif isinstance(g, MultiPolygon):
         for sub in g.geoms:
            polys.append(sub)
      else:
         for sub in getattr(g, "geoms", []) or []:
            if isinstance(sub, Polygon):
               polys.append(sub)
   valid_polys = []
   invalid_indices = []
   for i, p in enumerate(polys):
      if _validate_geom(p):
         valid_polys.append(p)
      else:
         rp = _repair_geom(p)
         if _validate_geom(rp):
            valid_polys.append(rp)
         else:
            invalid_indices.append(i)
   merged, skipped = _staged_union_polygons(valid_polys, fid)
   out_features = []
   if merged is None:
      for i, p in enumerate(valid_polys):
         out_features.append({"type":"Feature","id": f"{fid}_part_{i}","properties": props,"geometry": mapping(p)})
   else:
      if isinstance(merged, Polygon):
         out_features.append({"type":"Feature","id": fid,"properties": props,"geometry": mapping(merged)})
      elif isinstance(merged, MultiPolygon):
         out_features.append({"type":"Feature","id": fid,"properties": props,"geometry": mapping(merged)})
      else:
         polys2 = []
         for g in getattr(merged, "geoms", []) or []:
            if isinstance(g, Polygon):
               polys2.append(g)
            elif isinstance(g, MultiPolygon):
               for sub in g.geoms:
                  polys2.append(sub)
         if not polys2:
            out_features.append({"type":"Feature","id": fid,"properties": props,"geometry": mapping(Polygon())})
         elif len(polys2) == 1:
            out_features.append({"type":"Feature","id": fid,"properties": props,"geometry": mapping(polys2[0])})
         else:
            out_features.append({"type":"Feature","id": fid,"properties": props,"geometry": mapping(MultiPolygon(polys2))})
   return out_features

# ---------------------------
# High-level feature processing (preserve original flow)
# This version expands MultiPolygon into polygon parts and passes exterior+holes to the tileper-part function.
# ---------------------------
def fix_feature_5x6_topology(feature: Dict[str,Any]) -> List[Dict[str,Any]]:
   geom_json = feature.get("geometry")
   if geom_json is None:
      return []
   shp = shape(geom_json)
   fid = feature.get("id") or feature.get("properties",{}).get("id") or "0"
   props = feature.get("properties", {})
   out_features: List[Dict[str,Any]] = []

   parts = []
   # parts will be list of tuples: (exterior_coords, [hole_coords...])
   if shp.geom_type == "Polygon":
      ext = list(shp.exterior.coords)
      holes = [list(h.coords) for h in shp.interiors]
      parts = [(ext, holes)]
   elif shp.geom_type == "MultiPolygon":
      parts = []
      for p in shp.geoms:
         if isinstance(p, Polygon):
            ext = list(p.exterior.coords)
            holes = [list(h.coords) for h in p.interiors]
            parts.append((ext, holes))
   else:
      return []

   all_kept_pieces: List[Dict[str,Any]] = []
   for pidx, (ext_ring, hole_rings) in enumerate(parts):
      inserted_coords, seg_debug = _insert_ring_coords(ext_ring, fid, pidx)
      pieces = _tile_and_filter_staircase(inserted_coords, hole_rings, fid, pidx)
      all_kept_pieces.extend(pieces)

   if not all_kept_pieces:
      return []

   assembled = _assemble_feature_from_pieces(all_kept_pieces, fid, props)
   return assembled

# ---------------------------
# FeatureCollection processing
# ---------------------------
def fix_feature_collection_5x6_topology(gj: Dict[str,Any]) -> Dict[str,Any]:
   if gj.get("type") != "FeatureCollection":
      return {"type":"FeatureCollection","features":[]}
   out_gj = {"type":"FeatureCollection", "features": []}
   for feat in gj.get("features", []):
      out = fix_feature_5x6_topology(feat)
      if out:
         out_gj["features"].extend(out)
   return out_gj

# ---------------------------
# Feature processing pipeline (emit tiles option)
# This mirrors the original pipeline but expands MultiPolygon parts and includes holes.
# ---------------------------
def _process_feature(feature: Dict[str,Any], emit_tiles: bool) -> List[Dict[str,Any]]:
   geom_json = feature.get("geometry")
   if geom_json is None:
      return [feature]
   shp = shape(geom_json)
   fid = feature.get("id") or feature.get("properties",{}).get("id") or "0"
   props = feature.get("properties", {})

   all_kept_pieces: List[Dict[str,Any]] = []

   if shp.geom_type == "Polygon":
      parts = [(list(shp.exterior.coords), [list(h.coords) for h in shp.interiors])]
   elif shp.geom_type == "MultiPolygon":
      parts = []
      for p in shp.geoms:
         if isinstance(p, Polygon):
            parts.append((list(p.exterior.coords), [list(h.coords) for h in p.interiors]))
   else:
      return [feature]

   for pidx, (coords, holes) in enumerate(parts):
      inserted_coords, seg_debug_store = _insert_ring_coords(coords, fid, pidx)
      kept = _tile_and_filter_staircase(inserted_coords, holes, fid, pidx)
      all_kept_pieces.extend(kept)

   if emit_tiles:
      out_feats: List[Dict[str,Any]] = []
      for p in all_kept_pieces:
         geom = p["geom"]
         out_feats.append({"type":"Feature","id": f"{fid}_p{p['part_idx']}_t{p['tile_x']}_{p['tile_y']}",
                           "properties": {"tile_x": p["tile_x"], "tile_y": p["tile_y"], "orig_fid": fid, "part_idx": p["part_idx"]},
                           "geometry": mapping(geom)})
      return out_feats

   return _assemble_feature_from_pieces(all_kept_pieces, fid, props)

def fix_geojson_file_5x6_topology(input_path: str, output_path: str):
   # Load GeoJSON FeatureCollection from input_path, run the high-level
   # topology fixer (fix_feature_collection_5x6_topology) and write the result
   # to output_path.

   with open(input_path, "r", encoding="utf-8") as fh:
      data = json.load(fh)

   out_fc = fix_feature_collection_5x6_topology(data)

   with open(output_path, "w", encoding="utf-8") as fh:
      json.dump(out_fc, fh, ensure_ascii=False, indent=3)
