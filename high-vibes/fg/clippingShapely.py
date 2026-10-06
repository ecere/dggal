from dggal import *
from typing import Dict, Tuple, List, Any, Optional, Sequence
from shapely.geometry import shape, mapping, Polygon, MultiPolygon, LineString, MultiLineString, Point, MultiPoint, GeometryCollection
from shapely.ops import polygonize, unary_union
from shapely.validation import make_valid
import shapely
import functools
import json
import os
import numpy as np

from . import fix_topology_5x6 as topo
from .sutherlandHodgman import *

# Helper: write zone polygon GeoJSON for debugging
def write_zone_debug_geojson(zone_poly, dggrs, zone, debug_dir: str = "debug_out") -> None:
   # Write the provided zone polygon (Polygon or MultiPolygon) to
   # debug_dir/{dggrs.getZoneTextID(zone)}.geojson for inspection.
   # ensure a polygonal geometry (if None or empty, write an empty FeatureCollection)
   filename = f"zone-{dggrs.getZoneTextID(zone)}.geojson"
   outpath = os.path.join(debug_dir, filename)
   os.makedirs(debug_dir, exist_ok=True)

   if zone_poly is None:
      fc = {"type": "FeatureCollection", "features": []}
   else:
      # If zone_poly is a Polygon or MultiPolygon, map it directly
      geom = mapping(zone_poly)
      feat = {"type": "Feature", "id": dggrs.getZoneTextID(zone), "properties": {"zone": dggrs.getZoneTextID(zone)}, "geometry": geom}
      fc = {"type": "FeatureCollection", "features": [feat]}

   with open(outpath, "w", encoding="utf-8") as fh:
      json.dump(fc, fh, ensure_ascii=False, indent=3)

def _is_dggrs_5x6(name):
   return name.startswith("IVEA") or name.startswith("RTEA") or name.startswith("ISEA")

# Private global helper for staircase clipping in pure 5x6 layout coordinate space
def _clip_and_wrap_to_staircase(shp_geom) -> Any:
   import shapely
   from shapely.geometry import Polygon, box

   staircase_polygon = Polygon([
      (0.0, 0.0), (1.0, 0.0), (1.0, 1.0), (2.0, 1.0), (2.0, 2.0),
      (3.0, 2.0), (3.0, 3.0), (4.0, 3.0), (4.0, 4.0), (5.0, 4.0),
      (5.0, 5.0), (5.0, 6.0), (4.0, 6.0), (4.0, 5.0), (3.0, 5.0),
      (3.0, 4.0), (2.0, 4.0), (2.0, 3.0), (1.0, 3.0), (1.0, 2.0),
      (0.0, 2.0), (0.0, 0.0)
   ])

   results = []

   # Pass 1: Core interior clip
   if shp_geom.intersects(staircase_polygon):
      core_piece = shp_geom.intersection(staircase_polygon)
      if not core_piece.is_empty:
         results.append(core_piece)

   # Pass 2: LEFT-wrapping clip (x < 0)
   left_boundary = box(-1.0, -1.0, 0.0, 1.0)
   if shp_geom.intersects(left_boundary):
      left_piece = shp_geom.intersection(left_boundary)
      if not left_piece.is_empty:
         shifted_left = shapely.transform(left_piece, lambda coords: coords + 5.0)
         if shifted_left.intersects(staircase_polygon):
            final_left = shifted_left.intersection(staircase_polygon)
            if not final_left.is_empty:
               results.append(final_left)

   # Pass 3: RIGHT-wrapping clip (x > 5)
   right_boundary = box(5.0, 5.0, 6.0, 7.0)
   if shp_geom.intersects(right_boundary):
      right_piece = shp_geom.intersection(right_boundary)
      if not right_piece.is_empty:
         shifted_right = shapely.transform(right_piece, lambda coords: coords - 5.0)
         if shifted_right.intersects(staircase_polygon):
            final_right = shifted_right.intersection(staircase_polygon)
            if not final_right.is_empty:
               results.append(final_right)

   if not results:
      return Polygon()

   return shapely.union_all(results, grid_size=1e-10)

def get_zone_polygon(dggrs, zone, refined: bool = False, ico: bool = False, unclipped: bool = False) -> Optional[Polygon]:
   # Build the raw zone polygon (refined=False => 5 or 6 vertices), run the
   # same insertion/localize/clip pipeline used for features across all
   # candidate tiles the zone touches, then union the clipped pieces and
   # return a single polygonal geometry (Polygon or MultiPolygon).

   # 1) build raw zone polygon
   crs = CRS(ogc, 1534) if ico else CRS(0)

   verts_container = dggrs.getZoneRefinedCRSVertices(zone, crs, 0)

   coords = [[float(v.x), float(v.y)] for v in verts_container]

   Instance.delete(verts_container)

   if not coords:
      return Polygon()
   if coords[0] != coords[-1]:
      coords = coords + [coords[0]]

   if type(dggrs).__name__.startswith("HEALPix"):
      a4_0 = 0x40000000000000
      if int(zone) == a4_0 or dggrs.isZoneDescendantOf(zone, DGGRSZone(a4_0), 0):
         coords = [[pt[0] + 2.0 * math.pi, pt[1]] for pt in coords]

   raw_ring = coords

   if (not _is_dggrs_5x6(type(dggrs).__name__) or
       (unclipped and # We still need to clip for zones extending past 5x6 wrapping points
       (min(xs := [pt[0] for pt in coords]) > -1e-10 and max(xs) < 5.0 + 1e-10))):
      return Polygon(raw_ring)

   #print(raw_ring)

   # NOTE: Code below is all specific to 5x6 space

   return Polygon(raw_ring) # We may no longer need this _clip_and_wrap_to_staircase() at all after properly rotating/dropping the far geometry

   inserted_coords = raw_ring

   shp_geom = Polygon(inserted_coords)
   if shp_geom.is_empty:
      return Polygon()

   # 4) Run the clean static canvas clipping operation directly
   merged = _clip_and_wrap_to_staircase(shp_geom)

   if merged.is_empty:
      return Polygon()

   # 6) return the merged polygonal geometry (Polygon or MultiPolygon)
   return merged

def _collect_boundary_points(shp) -> List[tuple]:
   pts: List[tuple] = []
   if shp is None or shp.is_empty:
      return pts
   if isinstance(shp, Polygon):
      pts.extend(list(shp.exterior.coords))
      for interior in shp.interiors:
         pts.extend(list(interior.coords))
      return pts
   if isinstance(shp, MultiPolygon):
      for poly in shp.geoms:
         pts.extend(list(poly.exterior.coords))
         for interior in poly.interiors:
            pts.extend(list(interior.coords))
      return pts
   if isinstance(shp, (LineString, MultiLineString)):
      if isinstance(shp, LineString):
         pts.extend(list(shp.coords))
      else:
         for line in shp.geoms:
            pts.extend(list(line.coords))
      return pts
   return pts

# Assumed available in the module:
# - get_zone_polygon(dggrs, zone, refined=False, ico=False)
# - write_zone_debug_geojson(zone_poly, dggrs, zone, debug_dir="zone_tiles")
# - _collect_boundary_points(shp) -> Iterable[(x,y)] of original source boundary points

def _entry_exit_indices_for_ring(seq: Sequence[Sequence[float]], orig_set: set) -> List[int]:
   """
   Produce ordered entry/exit indices for a ring (circular sequence).
   Rules:
     - initial state is inside
     - inside -> outside at vertex i : append i
     - outside -> inside at vertex i : append next index (i+1 if < n else 0)
     - if 0 is present then ensure n-1 is also present
   Returns: List[int]
   """
   n = len(seq)
   if n == 0:
      return []

   inside_flags = [(float(x), float(y)) in orig_set for (x, y) in seq]

   state = True  # initial state is inside
   entry_exit_indices: List[int] = []
   for i in range(n):
      inside = inside_flags[i]
      if inside != state:
         if state:
            # inside -> outside : append i
            entry_exit_indices.append(i)
         else:
            # outside -> inside : append next index, wrap to 0 for rings
            next_i = i + 1
            entry_exit_indices.append(next_i if next_i < n else 0)
         state = inside

   # wrap consistency: if 0 present then ensure n-1 is present
   if n > 0 and 0 in entry_exit_indices and (n - 1) not in entry_exit_indices:
      entry_exit_indices.append(n - 1)

   return entry_exit_indices

def _entry_exit_indices_for_line(seq: Sequence[Sequence[float]], orig_set: set) -> List[int]:
   """
   Produce ordered entry/exit indices for a line (linear sequence).
   Rules:
     - initial state is inside
     - inside -> outside at vertex i : append i
     - outside -> inside at vertex i : append next index (i+1 if < n else n-1)
     - no circular wrap
   Returns: List[int]
   """
   n = len(seq)
   if n == 0:
      return []

   inside_flags = [(float(x), float(y)) in orig_set for (x, y) in seq]

   state = True
   entry_exit_indices: List[int] = []
   for i in range(n):
      inside = inside_flags[i]
      if inside != state:
         if state:
            entry_exit_indices.append(i)
         else:
            next_i = i + 1
            entry_exit_indices.append(next_i if next_i < n else (n - 1))
         state = inside

   return entry_exit_indices

def _entry_exit_for_polygon(poly: Polygon, orig_set: set) -> List[List[int]]:
   # Return List[List[int]] for a Polygon: exterior then interiors.
   rings_entry_exit: List[List[int]] = []
   exterior_seq = [(float(x), float(y)) for (x, y) in poly.exterior.coords]
   rings_entry_exit.append(_entry_exit_indices_for_ring(exterior_seq, orig_set))
   for interior in poly.interiors:
      seq = [(float(x), float(y)) for (x, y) in interior.coords]
      rings_entry_exit.append(_entry_exit_indices_for_ring(seq, orig_set))
   return rings_entry_exit

def _entry_exit_for_multipolygon(mpoly: MultiPolygon, orig_set: set) -> List[List[List[int]]]:
   # Return List[List[List[int]]] for a MultiPolygon: list of polygons, each a list of rings.
   mpolys_entry_exit: List[List[List[int]]] = []
   for p in mpoly.geoms:
      mpolys_entry_exit.append(_entry_exit_for_polygon(p, orig_set))
   return mpolys_entry_exit

def _shift_cross_seam_coords(coords, z_cx, z_cy):
   # Computes the seam thresholds directly using scalar min/max values
   # from the raw coordinates, avoiding unnecessary array math.
   new_coords = coords.copy()

   x = coords[:, 0]
   y = coords[:, 1]

   offset = 2.0

   # Check thresholds by subtracting the zone centroid directly from scalar bounds
   if (np.max(x) - z_cx) > offset and (np.max(y) - z_cy) > offset:
      shift = -5.0
   elif (np.min(x) - z_cx) < -offset and (np.min(y) - z_cy) < -offset:
      shift = 5.0
   else:
      shift = 0.0

   # Apply the unified translation to both dimensions in absolute lockstep
   new_coords[:, 0] += shift
   new_coords[:, 1] += shift

   return new_coords

def clip_featurecollection_to_zone(fc: Dict, dggrs, zone,
   refined: bool = False, ico: bool = False) -> Tuple[Dict, List[Any]]:
   """
   Clip GeoJSON FeatureCollection `fc` to `zone`.

   Returns (out_fc_geojson, features_entry_exit_indices) where
   features_entry_exit_indices is aligned 1:1 with features in out_fc and its shape
   strictly matches the *clipped* geometry type produced for each feature:
     - clipped Polygon -> List[List[int]]  (exterior then interiors)
     - clipped MultiPolygon -> List[List[List[int]]] (polygons -> rings -> indices)
     - clipped LineString -> List[int]
     - clipped MultiLineString -> List[List[int]] (one list per line)
   Points and MultiPoint features do not contribute entry/exit indices (an empty list is appended
   to preserve 1:1 alignment but they never contain markers).
   """
   zone_poly = None # get_zone_polygon(dggrs, zone, refined=refined, ico=ico)
   zone_poly_lines = None # get_zone_polygon(dggrs, zone, refined=refined, ico=ico, unclipped=True)
   #if not zone_poly.is_valid:
   #   zone_poly = make_valid(zone_poly)

   #write_zone_debug_geojson(zone_poly, dggrs, zone, debug_dir="zone_tiles")

   out_fc: Dict[str, Any] = {"type": "FeatureCollection", "features": []}
   features_entry_exit_indices: List[Any] = []

   dggrs_name = type(dggrs).__name__
   is5x6 = _is_dggrs_5x6(dggrs_name)

   zc = dggrs.getZoneCRSCentroid(zone, CRS(0)) if is5x6 else None
   if is5x6:
      z_cx, z_cy = float(zc.x), float(zc.y)

   for feat in fc.get("features", []):
      geom = feat.get("geometry")
      props = feat.get("properties")
      fid = feat.get("id")

      if geom is None:
         continue

      geom_type = geom.get("type")

      if geom_type in ("LineString", "MultiLineString"):
         if zone_poly_lines is None:
            zone_poly_lines = get_zone_polygon(dggrs, zone, refined=refined, ico=ico, unclipped=True)
            zone_bounds = zone_poly_lines.bounds
            target_mask = zone_poly_lines
            if is5x6: z_cx, z_cy = target_mask.centroid.x, target_mask.centroid.y
      else:
         if zone_poly is None:
            zone_poly = get_zone_polygon(dggrs, zone, refined=refined, ico=ico)
            if not zone_poly.is_valid:
               zone_poly = make_valid(zone_poly)
            target_mask = zone_poly
            zone_bounds = zone_poly.bounds
            # write_zone_debug_geojson(zone_poly, dggrs, zone, debug_dir="zone_tiles")
            if is5x6: z_cx, z_cy = target_mask.centroid.x, target_mask.centroid.y

      src_shp = feat.get("_shapely_geom")

      if geom and "bbox" not in feat:
         if src_shp is None:
            src_shp = shape(geom)
            feat["_shapely_geom"] = src_shp
         feat["bbox"] = src_shp.bounds

      f_minx, f_miny, f_maxx, f_maxy = feat["bbox"]

      # skip only if truly out-of-bounds AND not a cross-seam candidate
      if f_minx > zone_bounds[2] or f_maxx < zone_bounds[0] or f_miny > zone_bounds[3] or f_maxy < zone_bounds[1]:
         if not is5x6 or not (max(abs(f_minx - zone_bounds[2]), abs(f_maxx - zone_bounds[0])) > 2.0 or max(abs(f_miny - zone_bounds[3]), abs(f_maxy - zone_bounds[1])) > 2.0):
            continue

      if not src_shp:
         src_shp = shape(geom)
         feat["_shapely_geom"] = src_shp

      if is5x6 and (zone_bounds[2] > 5 + 1e-12 or zone_bounds[3] > 6 + 1e-12 or zone_bounds[0] < -1e-12 or zone_bounds[1] < -1e-12):
         # We need to shift the geometry to valid space to clip this zone...
         atomic_parts = shapely.get_parts(src_shp)
         transformed_parts = [shapely.transform(part, functools.partial(_shift_cross_seam_coords, z_cx=z_cx, z_cy=z_cy)) for part in atomic_parts if not part.is_empty]
         aligned_shp = shapely.union_all(transformed_parts) if transformed_parts else src_shp
      else:
         aligned_shp = src_shp

      if not aligned_shp.is_valid:
         aligned_shp = shapely.make_valid(aligned_shp)

      clipped = aligned_shp.intersection(target_mask)
      if clipped is None or clipped.is_empty:
         continue

      # Keep only parts matching the original geometry class, discarding cross-dimension artifacts.
      if geom_type in ("Polygon", "MultiPolygon"):
         if clipped.geom_type in ("Polygon", "MultiPolygon"):
            poly_clipped = clipped
         elif clipped.geom_type == "GeometryCollection":
            polys = [g for g in clipped.geoms if g.geom_type in ("Polygon", "MultiPolygon")]
            poly_clipped = polys[0] if len(polys) == 1 else MultiPolygon(polys) if polys else None
         else:
            poly_clipped = None

      elif geom_type in ("LineString", "MultiLineString"):
         if clipped.geom_type in ("LineString", "MultiLineString"):
            poly_clipped = clipped
         elif clipped.geom_type == "GeometryCollection":
            lines = [g for g in clipped.geoms if g.geom_type in ("LineString", "MultiLineString")]
            poly_clipped = lines[0] if len(lines) == 1 else MultiLineString(lines) if lines else None
         else:
            poly_clipped = None

      else: # Points / MultiPoints
         poly_clipped = clipped

      if poly_clipped is None or poly_clipped.is_empty:
         continue

      out_geom = mapping(poly_clipped)
      out_fc["features"].append({
          "type": "Feature",
          "id": fid,
          "properties": props,
          "geometry": out_geom,
          "_shapely_geom": poly_clipped
      })

      # original boundary points are considered "inside"
      orig_pts = _collect_boundary_points(src_shp)
      orig_set = set((float(x), float(y)) for (x, y) in orig_pts)

      # Determine clipped geometry type and produce entry/exit indices shaped to that clipped type.
      clipped_type = poly_clipped.geom_type  # 'Polygon', 'MultiPolygon', 'LineString', 'MultiLineString', 'Point', 'MultiPoint', etc.

      if clipped_type == "Polygon":
         # Return a Polygon-shaped entry_exit: List[List[int]]
         rings_entry_exit = _entry_exit_for_polygon(poly_clipped, orig_set)
         features_entry_exit_indices.append(rings_entry_exit)

      elif clipped_type == "MultiPolygon":
         # Return a MultiPolygon-shaped entry_exit: List[List[List[int]]]
         mpolys_entry_exit = _entry_exit_for_multipolygon(poly_clipped, orig_set)
         features_entry_exit_indices.append(mpolys_entry_exit)

      elif clipped_type == "LineString":
         # Return a LineString-shaped entry_exit: List[int]
         seq = [(float(x), float(y)) for (x, y) in poly_clipped.coords]
         line_entry_exit = _entry_exit_indices_for_line(seq, orig_set)
         features_entry_exit_indices.append(line_entry_exit)

      elif clipped_type == "MultiLineString":
         # Return a MultiLineString-shaped entry_exit: List[List[int]]
         lines_entry_exit: List[List[int]] = []
         for line in poly_clipped.geoms:
            seq = [(float(x), float(y)) for (x, y) in line.coords]
            lines_entry_exit.append(_entry_exit_indices_for_line(seq, orig_set))
         features_entry_exit_indices.append(lines_entry_exit)

      elif clipped_type in ("Point", "MultiPoint"):
         # Points do not add markers. Append an empty list to preserve alignment.
         features_entry_exit_indices.append([])

      else:
         # Unknown/other clipped geometry types: preserve alignment with an empty structure
         features_entry_exit_indices.append([])

   return out_fc, features_entry_exit_indices
