from fg.sutherlandHodgman import *
from fg.fix_topology_5x6 import collapse_near_duplicates

import json
import os
from math import floor
from typing import Any, Dict, List, Optional, Tuple
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
)
from shapely.ops import unary_union, linemerge
from shapely.validation import explain_validity
import shapely

# ---------- simple file-level debug flag (hardcoded) ----------
DEBUG = True
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
   # This mirrors the logic:
   # if(rel < -180 - eps) rel += 360 * floor((180 - rel)/360)
   # elif(rel > 180 + eps) rel -= 360 * floor((rel + 180)/360)
   if rel < -180.0 - _EPS:
      rel += 360.0 * floor((180.0 - rel) / 360.0)
   elif rel > 180.0 + _EPS:
      rel -= 360.0 * floor((rel + 180.0) / 360.0)

   # return absolute lon (relative + center)
   return rel + c_lon

# ---------- localization using wrap_lon_at ----------
def _localize_ring_with_wrap(ring: List[Tuple[float, float]],
                                      tile_center: float
                                      ) -> Tuple[List[Tuple[float, float]], List[float]]:
   """
   Anchor-based localization that calls wrap_lon_at using the neighbor's shifted longitude
   as the reference (no separate normalization step).

   Behavior:
   - Choose an anchor vertex closest in longitude to tile_center.
   - Compute anchor_shifted = wrap_lon_at(anchor_lon, tile_center).
   - Propagate forward from anchor: for each next vertex call
       wrap_lon_at(lon, prev_shifted)
     where prev_shifted is the previously shifted longitude (neighbor-based reference).
   - Propagate backward from anchor similarly (walking backwards).
   - Preserve ring closure if input ring was closed (first == last).
   - Return (localized_ring, shifts) where shifts[i] = localized_lon[i] - original_lon[i].
   """
   n = len(ring)
   if n == 0:
      return [], []

   # preserve whether input was closed (first == last)
   closed_input = (ring[0] == ring[-1]) and n > 1
   # if closed, ignore the duplicate last point for indexing convenience
   if closed_input:
      ring_core = ring[:-1]
   else:
      ring_core = list(ring)

   m = len(ring_core)
   if m == 0:
      return [], []

   # convert to floats
   orig_lons = [float(lon) for lon, _ in ring_core]
   lats = [float(lat) for _, lat in ring_core]

   # anchor: index with lon closest to tile_center
   anchor_idx = min(range(m), key=lambda i: abs(orig_lons[i] - tile_center))

   # compute anchor shifted lon using tile_center as reference for initial placement
   anchor_lon = orig_lons[anchor_idx]
   shifted: List[float] = [0.0] * m
   shifted[anchor_idx] = float(wrap_lon_at(anchor_lon, tile_center))

   # forward propagation: anchor+1 .. end .. wrap to start .. anchor-1
   prev = shifted[anchor_idx]
   i = (anchor_idx + 1) % m
   while i != anchor_idx:
      lon = orig_lons[i]
      # call wrap_lon_at with neighbor (prev) as reference
      shifted[i] = float(wrap_lon_at(lon, prev))
      prev = shifted[i]
      i = (i + 1) % m

   # build localized ring and shifts; re-append closing point if input was closed
   localized: List[Tuple[float, float]] = []
   shifts: List[float] = []
   for idx in range(m):
      s = shifted[idx] - orig_lons[idx]
      localized.append((shifted[idx], lats[idx]))
      shifts.append(s)

   if closed_input:
      # duplicate first entry to close the ring
      localized.append(localized[0])
      shifts.append(shifts[0])

   return localized, shifts

# ---------- ring test using raw lon (no normalization) ----------
def _ring_has_vertex_in_tile_without_shift(ring: List[Tuple[float, float]], xmin: float, xmax: float) -> bool:
   """
   Return True if any vertex of ring (raw lon) lies within [xmin, xmax].
   Uses raw input lon values as requested.
   """
   for lon, _ in ring:
      lonf = float(lon)
      if xmin <= lonf <= xmax:
         return True
   return False

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

from typing import Sequence

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

# ---------- tile-and-clip for a single polygon using wrap_lon_at ----------
def _tile_and_clip_polygon(exterior_coords: List[Tuple[float, float]],
                           holes_coords: List[List[Tuple[float, float]]],
                           zone_extent, zone_tile_eps, fid, part_idx: int) -> List[Dict[str, Any]]:
   """
   For each of the 4 tiles:
    - Drop exterior ring if it has no vertex inside the tile without shifts (raw lon test).
    - Otherwise apply wrap_lon_at(lon, tile_center) to each vertex.
    - Record shifted polygon as proper GeoJSON in per-tile debug files.
    - Clip rings with rect_clip_polygon and assemble pieces (holes processed similarly).
   """
   poly = Polygon(exterior_coords, holes_coords)
   if poly.is_empty:
      return []

   pieces: List[Dict[str, Any]] = []
   for q, (xmin, ymin, xmax, ymax) in enumerate(TILES_4):
      if not intersects_extent_deg((xmin, ymin, xmax, ymax), zone_extent, zone_tile_eps):
      #   #print("Not processing tile", (xmin, ymin, xmax, ymax), "for zone with extent", zone_extent)
         continue

      tile_center = 0.5 * (xmin + xmax)

      # Drop exterior if it has no vertex inside tile without shifts (raw lon test)
      if not _ring_has_vertex_in_tile_without_shift(exterior_coords, xmin, xmax):
         continue

      # Localize exterior using wrap_lon_at
      localized_ext, ext_shifts = _localize_ring_with_wrap(exterior_coords, tile_center)

      # Build debug geometry (proper GeoJSON Polygon) using shifted coords (ensure closure)
      shifted_ext_closed = list(localized_ext)
      if shifted_ext_closed and shifted_ext_closed[0] != shifted_ext_closed[-1]:
         shifted_ext_closed = shifted_ext_closed + [shifted_ext_closed[0]]

      debug_geom_coords: List[List[List[float]]] = [ [[float(x), float(y)] for (x, y) in shifted_ext_closed] ]
      holes_debug_list: List[Dict[str, Any]] = []

      # Process holes for debug geometry: include shifted holes that pass the raw-vertex test
      for h in holes_coords or []:
         if not h:
            continue
         hh = list(h)
         if hh[0] != hh[-1]:
            hh.append(hh[0])
         if not _ring_has_vertex_in_tile_without_shift(hh, xmin, xmax):
            holes_debug_list.append({"original_hole": hh, "skipped_reason": "no_vertex_in_tile_without_shift"})
            continue
         localized_h, h_shifts = _localize_ring_with_wrap(hh, tile_center)
         shifted_h_closed = list(localized_h)
         if shifted_h_closed and shifted_h_closed[0] != shifted_h_closed[-1]:
            shifted_h_closed = shifted_h_closed + [shifted_h_closed[0]]
         debug_geom_coords.append([[float(x), float(y)] for (x, y) in shifted_h_closed])
         holes_debug_list.append({
            "original_hole": hh,
            "localized_hole_shifted": [[float(x), float(y)] for (x, y) in localized_h],
            "hole_shifts": [float(s) for s in h_shifts]
         })

      # Create debug feature (proper GeoJSON) with shifted polygon geometry
      if DEBUG:
         debug_feature = {
            "type": "Feature",
            "properties": {
               "fid": fid,
               "part_idx": part_idx,
               "tile_xmin": xmin,
               "tile_xmax": xmax,
               "exterior_shifts": [float(s) for s in ext_shifts],
               "holes_info": holes_debug_list
            },
            "geometry": {
               "type": "Polygon",
               "coordinates": debug_geom_coords
            }
         }
         _debug_record_tile(xmin, xmax, debug_feature)

      # Clip exterior ring (use shifted exterior for clipping)
      clipped = rect_clip_polygon(localized_ext, xmin, ymin, xmax, ymax)
      if not clipped:
         continue
      clipped = collapse_near_duplicates(clipped, eps=DUP_EPS)
      if clipped and clipped[0] == clipped[-1]:
         clipped = clipped[:-1]
      if len(clipped) < 3:
         # print("Skipping no clipped")
         continue
      outer_poly = Polygon(clipped)
      if outer_poly.is_empty or outer_poly.area <= 0.0:
         # print("empty or no area")
         continue

      # Process holes: only keep holes that have at least one vertex inside tile without shifts
      hole_polys: List[Polygon] = []
      for h in holes_coords or []:
         if not h:
            continue
         hh = list(h)
         if hh[0] != hh[-1]:
            hh.append(hh[0])
         if not _ring_has_vertex_in_tile_without_shift(hh, xmin, xmax):
            continue
         localized_h, h_shifts = _localize_ring_with_wrap(hh, tile_center)
         clipped_h = rect_clip_polygon(localized_h, xmin, ymin, xmax, ymax)
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

      outer_poly = outer_poly.buffer(0)

      # Subtract holes if present (fail-hard semantics; no try/except)
      if not hole_polys:
         # if outer_poly.is_empty: print("final empty")
         #print("final area:", outer_poly.area)

         final_polys = [outer_poly]
      else:
         hole_union = unary_union(hole_polys)
         if hole_union.is_empty:
            final_polys = [outer_poly]
         else:
            try:
               # outer_poly = outer_poly.buffer(0)
               result = outer_poly.difference(hole_union)
            except:
               print("\nWARNING: Error adding holes to feature", fid)
               print("outer_poly is:", outer_poly)
               print("hole_union is:", hole_union, "\n")

               print("outer valid:", outer_poly.is_valid)
               print("outer validity reason:", explain_validity(outer_poly))
               print("hole_union valid:", hole_union.is_valid)
               print("hole_union validity reason:", explain_validity(hole_union))

               print("outer intersects hole_union:", outer_poly.intersects(hole_union))
               print("outer contains hole_union:", outer_poly.contains(hole_union))
               print("outer covers hole_union:", outer_poly.covers(hole_union))
               print("outer touches hole_union:", outer_poly.touches(hole_union))
               print("outer crosses hole_union:", outer_poly.crosses(hole_union))
               print("outer overlaps hole_union:", outer_poly.overlaps(hole_union))

               result = outer_poly
            if result.geom_type == "Polygon":
               if not result.is_empty and result.area > 0.0:
                  final_polys = [result]
               else:
                  final_polys = []
            else:
               final_polys = [sub for sub in result.geoms if sub.geom_type == "Polygon" and not sub.is_empty and sub.area > 0.0]

      # Filter and accept pieces (handle Polygon, MultiPolygon, GeometryCollection)
      def _accept_polygon_piece(poly):
         # poly is a shapely Polygon

         if poly.is_empty or poly.area <= _AREA_EPS:
            return None
         tol = 1e-12

         #ok = True
         #for cx, cy in list(poly.exterior.coords):
         #   if not (xmin - tol <= cx <= xmax + tol and ymin - tol <= cy <= ymax + tol):
         #      ok = False
         #      break
         #if not ok: return None

         anyInside = False
         for cx, cy in list(poly.exterior.coords):
            if (xmin - tol <= cx <= xmax + tol and ymin - tol <= cy <= ymax + tol):
               anyInside = True
               #print("Discard piece here")
               #return None
         #if not anyInside: return None
         return {"tile_x": xmin, "tile_y": ymin, "geom": poly, "orig_fid": fid, "part_idx": part_idx}

      for g in final_polys:
         # If g is a Polygon, test it directly
         if g.geom_type == "Polygon":
            piece = _accept_polygon_piece(g)
            if piece:
               pieces.append(piece)
            continue

         # If g is a MultiPolygon, iterate sub-polygons
         if g.geom_type == "MultiPolygon":
            for sub in g.geoms:
               piece = _accept_polygon_piece(sub)
               if piece:
                  pieces.append(piece)
            continue

         # If g is a GeometryCollection, extract polygon members
         if g.geom_type == "GeometryCollection":
            for sub in g.geoms:
               if sub.geom_type == "Polygon":
                  piece = _accept_polygon_piece(sub)
                  if piece:
                     pieces.append(piece)
            continue

         # Unexpected geometry type: log and skip
         print(f"Debug: unexpected geometry type {g.geom_type} for fid={fid} part={part_idx}; skipping")

   return pieces

# ---------- wrapper that accepts exterior+holes or polygon-like lists ----------
def _tile_and_clip(exterior_or_poly, holes_coords: Optional[List[List[Tuple[float, float]]]],
   zone_extent, eps_zone_tile, fid, part_idx: int,
   original_geom: Optional[Dict[str,Any]] = None) -> List[Dict[str, Any]]:
   # exterior_or_poly can be a shapely Polygon/MultiPolygon or a list of coords (exterior)
   if hasattr(exterior_or_poly, "exterior"):
      if exterior_or_poly.geom_type == "Polygon":
         ext = list(exterior_or_poly.exterior.coords)
         holes = [list(h.coords) for h in exterior_or_poly.interiors]
         return _tile_and_clip_polygon(ext, holes, zone_extent, eps_zone_tile, fid, part_idx)
      pieces: List[Dict[str, Any]] = []
      for sub in exterior_or_poly.geoms:
         ext = list(sub.exterior.coords)
         holes = [list(h.coords) for h in sub.interiors]
         pieces.extend(_tile_and_clip_polygon(ext, holes, zone_extent, eps_zone_tile, fid, part_idx))
      return pieces
   return _tile_and_clip_polygon(exterior_or_poly, holes_coords or [], zone_extent, eps_zone_tile, fid, part_idx)

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

   # Disabled staged union to avoid GEOS errors while geometries are still invalid.
   # When the clipped polygons are consistently valid, re-enable staged union:
   merged, skipped = _staged_union_polygons(valid_polys, fid)
   # For now produce a MultiPolygon directly from valid_polys to avoid GEOS unary_union on invalid inputs.
   #merged, skipped = (MultiPolygon(valid_polys), None)

   out_features: List[Dict[str, Any]] = []
   if merged is None:
      for i, p in enumerate(valid_polys):
         out_features.append({"type": "Feature", "id": f"{fid}_part_{i}", "properties": props, "geometry": mapping(p)})
      return out_features

   if merged.geom_type in ("Polygon", "MultiPolygon"):
      out_features.append({"type": "Feature", "id": fid, "properties": props, "geometry": mapping(merged)})
      return out_features

   # fallback: extract polygonal parts
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

   def _process_single_geometry(geom: Optional[Dict[str, Any]], zone_extent: List[float], fid_for_debug = None) -> Optional[Dict[str, Any]]:
      if geom is None:
         return None

      orig_type = geom["type"]
      tile_geoms = []

      if orig_type == "Polygon":
         ext = geom["coordinates"][0]
         holes = geom["coordinates"][1:]
         pieces = _tile_and_clip(ext, holes, zone_extent, eps_zone_tile, fid_for_debug, part_idx=0)
         #print("_tile_and_clip returned", len(pieces), "pieces")
         for p in pieces:
            tile_geoms.append(p["geom"])

      elif orig_type == "MultiPolygon":
         for i, poly in enumerate(geom["coordinates"]):
            ext = poly[0]
            holes = poly[1:]
            pieces = _tile_and_clip(ext, holes, zone_extent, eps_zone_tile, fid_for_debug, part_idx=i)
            for p in pieces:
               tile_geoms.append(p["geom"])
      else:
         for xmin, ymin, xmax, ymax in TILES_4:
            tile_center = 0.5 * (xmin + xmax)
            shp = _geom_to_shapely_shifted(geom, tile_center)
            tile_box = box(xmin, ymin, xmax, ymax)
            inter = shp.intersection(tile_box)
            if not inter.is_empty:
               tile_geoms.append(inter)

      if not tile_geoms:
         return None


      # Disabled unary_union to avoid GEOS errors while geometries are still invalid.
      # When clipped polygons are consistently valid, re-enable:
      merged = unary_union(tile_geoms)
      #merged = shapely.union_all(tile_geoms, grid_size = 1e-9)
      # merged = tile_geoms

      if orig_type in ("Polygon", "MultiPolygon"):
         all_kept_pieces = [{"geom": g} for g in tile_geoms]
         assembled = _assemble_feature_from_pieces(all_kept_pieces, fid_for_debug, props={})
         if not assembled:
            if DEBUG:
               _debug_write_files()
            return None
         if len(assembled) == 1:
            if DEBUG:
               _debug_write_files()
            return assembled[0]["geometry"]
         polys = []
         for f in assembled:
            g = f["geometry"]
            if g is None:
               continue
            if g["type"] == "Polygon":
               polys.append(Polygon(g["coordinates"][0], g["coordinates"][1:]))
            elif g["type"] == "MultiPolygon":
               for sub in g["coordinates"]:
                  polys.append(Polygon(sub[0], sub[1:]))
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
         fixed = _process_single_geometry(geom, zone_extent, fid_for_debug=fid)
         new_feat = dict(feat)
         new_feat["geometry"] = fixed
         out["features"].append(new_feat)
      return out

   if obj["type"] == "Feature":
      geom = obj["geometry"]
      if geom is None:
         return obj
      fid = obj.get("id", "0")
      fixed = _process_single_geometry(geom, zone_extent, fid_for_debug=fid)
      out = dict(obj)
      out["geometry"] = fixed
      return out

   return _process_single_geometry(obj, zone_extent, fid_for_debug=fid)
