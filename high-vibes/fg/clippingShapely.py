from dggal import *
from typing import Dict, Tuple, List, Any, Optional, Sequence
from shapely.geometry import shape, mapping, Polygon, MultiPolygon, LineString, MultiLineString, Point, MultiPoint, GeometryCollection
from shapely.ops import polygonize, unary_union
from shapely.validation import make_valid
import json
import os

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

def get_zone_polygon(dggrs, zone, refined: bool = False, ico: bool = False, unclipped: bool = False) -> Optional[Polygon]:
   # Build the raw zone polygon (refined=False => 5 or 6 vertices), run the
   # same insertion/localize/clip pipeline used for features across all
   # candidate tiles the zone touches, then union the clipped pieces and
   # return a single polygonal geometry (Polygon or MultiPolygon).

   # 1) build raw zone polygon
   crs = CRS(ogc, 1534) if ico else CRS(0)

   if True: #refined:
      verts_container = dggrs.getZoneRefinedCRSVertices(zone, crs, 0)
   else:
      verts_container = dggrs.getZoneCRSVertices(zone, crs)

   coords = [[float(v.x), float(v.y)] for v in verts_container]
   if not coords:
      return Polygon()
   if coords[0] != coords[-1]:
      coords = coords + [coords[0]]

   raw_ring = coords

   if unclipped or not _is_dggrs_5x6(type(dggrs).__name__):
      return Polygon(raw_ring)

   #print(raw_ring)

   # NOTE: Code below is all specific to 5x6 space

   # 2) run distance5x6 insertion on the raw ring (same routine used for features)

   # This may not be necessary when using the refined tiles, and causes some left-over slivers
   # connecting warping tiles...
   #inserted_coords, _seg_debug = topo._insert_ring_coords(raw_ring, f"zone_{dggrs.getZoneTextID(zone)}", 0)

   #return Polygon(raw_ring) #
   inserted_coords = raw_ring

   # 3) determine candidate tiles the inserted ring touches
   candidates = topo._candidate_tiles_from_vertices(inserted_coords, margin_neighbors=1)
   if not candidates:
      return Polygon()

   # 4) for each candidate tile, clip/localize using the same tile pipeline
   polys = []
   for (tx, ty) in candidates:
      # _tile_and_filter_staircase will localize the ring for each tile and return clipped pieces
      #pieces = topo._tile_and_filter_staircase(inserted_coords, f"zone_{dggrs.getZoneTextID(zone)}", 0)
      pieces = topo._tile_and_filter_staircase(inserted_coords, [], f"zone_{dggrs.getZoneTextID(zone)}", 0)

      # pieces may include many tiles; filter to the current tile (tile_and_filter returns only relevant tiles,
      # but we call it per-ring for consistency with feature pipeline)
      for p in pieces:
         if p.get("tile_x") == tx and p.get("tile_y") == ty:
            #print("Adding piece of clipper from ", tx, ty)
            polys.append(p["geom"])
         #polys.append(p["geom"])

   if not polys:
      return Polygon()

   # 5) staged union/repair using the same routine as features
   merged, skipped = topo._staged_union_polygons(polys, f"zone_{dggrs.getZoneTextID(zone)}")
   if merged is None:
      return Polygon()

   #print(merged)

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
      else:
         if zone_poly is None:
            zone_poly = get_zone_polygon(dggrs, zone, refined=refined, ico=ico)
            if not zone_poly.is_valid:
               zone_poly = make_valid(zone_poly)
            zone_bounds = zone_poly.bounds
            # write_zone_debug_geojson(zone_poly, dggrs, zone, debug_dir="zone_tiles")

      src_shp = None

      if geom and "bbox" not in feat:
         src_shp = shape(geom)
         feat["bbox"] = src_shp.bounds

      f_minx, f_miny, f_maxx, f_maxy = feat["bbox"]
      if f_minx > zone_bounds[2] or f_maxx < zone_bounds[0] or f_miny > zone_bounds[3] or f_maxy < zone_bounds[1]:
         continue

      if not src_shp:
         src_shp = shape(geom)

      if not src_shp.is_valid:
         src_shp = make_valid(src_shp)

      clipped = src_shp.intersection(zone_poly_lines if geom_type in ("LineString", "MultiLineString") else zone_poly)
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
      out_fc["features"].append({"type": "Feature", "id": fid, "properties": props, "geometry": out_geom})

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
