from typing import Dict, Any, List, Tuple
from shapely.geometry import shape, mapping, box
from shapely.ops import transform, unary_union, polygonize
from shapely.validation import make_valid
import shapely

from .faces import Pointd
from .distance import distance5x6
from .sutherlandHodgman import *

DUP_EPS = 1e-10

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

import os
import json
from typing import Dict, Any, List, Tuple
from shapely.geometry import shape, mapping, box, MultiLineString, MultiPolygon, Polygon, LineString
from shapely.ops import unary_union
from shapely.validation import make_valid
from shapely.geometry.base import BaseGeometry

def _get_rhombus_layout(index: int) -> Tuple[float, float, bool]:
   x_start = float(index // 2)
   y_start = float(index // 2) if (index % 2 == 0) else float((index // 2) + 1)
   return x_start, y_start, (index % 2 == 0)

def _extract_same_dimension_geoms(geom: BaseGeometry, geom_type: str) -> List[BaseGeometry]:
   if geom.is_empty:
      return []
   is_poly = "Polygon" in geom_type
   is_line = "LineString" in geom_type or "LinearRing" in geom_type
   is_point = "Point" in geom_type

   valid_parts = []
   if geom.geom_type in ("GeometryCollection", "MultiPolygon", "MultiLineString", "MultiPoint"):
      for part in geom.geoms:
         valid_parts.extend(_extract_same_dimension_geoms(part, geom_type))
      return valid_parts

   if is_poly and "Polygon" in geom.geom_type:
      valid_parts.append(geom)
   elif is_line and ("LineString" in geom.geom_type or "LinearRing" in geom.geom_type):
      valid_parts.append(geom)
   elif is_point and "Point" in geom.geom_type:
      valid_parts.append(geom)
   return valid_parts


def _forward_transform_coordinate_list(coords: List[Tuple], x_start: float, y_start: float, is_even: bool, is_polygon: bool) -> List[Tuple]:
   # Processes a raw list of coordinates. Transforms them sequentially and strictly DROPS
   # any vertex that falls completely outside the valid 4-neighbor neighborhood box.
   left_x, right_x = x_start, x_start + 1.0
   top_y, bottom_y = y_start, y_start + 1.0

   transformed_coords = []

   for coord in coords:
      x, y = coord[0], coord[1]

      eps1 = 1e-9
      eps2 = 1e-9
      eps3 = 1e-9

      # Step 1: Global Seam Wrapping (Use inclusive operators to capture points exactly on the border lines)
      if (x - x_start) >= 3.0 - eps1 or (y - y_start) >= 3.0 - eps1:
         x, y = x - 5.0, y - 5.0
      elif (x - x_start) <= -2.0 + eps1 or (y - y_start) <= -2.0 + eps1:
         x, y = x + 5.0, y + 5.0

      # Step 2: Local Face Fold Rotations (Exactly 2 strict conditions per case)
      if is_even:
         # x > right => CCW 90 deg around Bottom-Right corner
         if x > right_x + eps2:
            px, py = right_x, bottom_y
            dx, dy = x - px, y - py
            x, y = px + dy, py - dx
         # y < top => CW 90 deg around Top-Left corner
         elif y < top_y - eps2:
            px, py = left_x, top_y
            dx, dy = x - px, y - py
            x, y = px - dy, py + dx
      else:
         # x < left => CCW 90 deg around Top-Left corner
         if x < left_x - eps2:
            px, py = left_x, top_y
            dx, dy = x - px, y - py
            x, y = px + dy, py - dx
         # y > bottom => CW 90 deg around Bottom-Right corner
         elif y > bottom_y + eps2:
            px, py = right_x, bottom_y
            dx, dy = x - px, y - py
            x, y = px - dy, py + dx

      # --- STEP 3: STRICT VERTEX DROPPING ---
      # If the coordinate drops out of the 4-neighbor grid boundaries, delete it by omitting it.
      if (x_start - 1.0 <= x + eps3 <= x_start + 2.0 + eps3) and (y_start - 1.0 <= y + eps3 <= y_start + 2.0 + eps3):
         transformed_coords.append((x, y))

   # For polygon boundary rings, ensure closure constraints are perfectly satisfied if we dropped points
   if is_polygon and len(transformed_coords) >= 3:
      if transformed_coords[0] != transformed_coords[-1]:
         transformed_coords.append(transformed_coords[0])

   return transformed_coords


def _process_geometry_dropping_vertices(geom: BaseGeometry, x_start: float, y_start: float, is_even: bool) -> BaseGeometry:
   # Recursively reconstructs geometry structures from raw dropped coordinate strings
   # to guarantee type compliance and prevent LinearRing closure exceptions.
   if geom.is_empty:
      return geom

   if geom.geom_type == "Polygon":
      exterior = _forward_transform_coordinate_list(geom.exterior.coords, x_start, y_start, is_even, is_polygon=True)
      if len(exterior) < 4:  # Minimum valid Polygon boundary ring requirement
         return Polygon()

      interiors = []
      for hole in geom.interiors:
         h_coords = _forward_transform_coordinate_list(hole.coords, x_start, y_start, is_even, is_polygon=True)
         if len(h_coords) >= 4:
            interiors.append(h_coords)
      return Polygon(exterior, interiors)

   elif geom.geom_type == "MultiPolygon":
      polys = [_process_geometry_dropping_vertices(p, x_start, y_start, is_even) for p in geom.geoms]
      valid_polys = [p for p in polys if not p.is_empty]
      return MultiPolygon(valid_polys) if valid_polys else MultiPolygon()

   elif geom.geom_type == "LineString":
      coords = _forward_transform_coordinate_list(geom.coords, x_start, y_start, is_even, is_polygon=False)
      return LineString(coords) if len(coords) >= 2 else LineString()

   elif geom.geom_type == "MultiLineString":
      lines = [_process_geometry_dropping_vertices(l, x_start, y_start, is_even) for l in geom.geoms]
      valid_lines = [l for l in lines if not l.is_empty]
      return MultiLineString(valid_lines) if valid_lines else MultiLineString()

   return geom


def fix_feature_collection_5x6_topology(gj: Dict[str, Any]) -> Dict[str, Any]:
   if gj.get("type") != "FeatureCollection":
      return {"type": "FeatureCollection", "features": []}

   features = gj.get("features", [])
   if not features:
      return {"type": "FeatureCollection", "features": []}

   # os.makedirs("debug5x6", exist_ok=True)

   # Storage mapping structure: index -> fid -> list_of_geometries
   rhombi_geometry_store = {idx: {} for idx in range(10)}
   debug_file_registry = {idx: [] for idx in range(10)}

   # Process every single layout completely
   for index in range(10):
      x_start, y_start, is_even = _get_rhombus_layout(index)
      target_box = box(x_start, y_start, x_start + 1.0, y_start + 1.0)

      current_rhombus_dict = rhombi_geometry_store[index]
      debug_list = debug_file_registry[index]

      for feat in features:
         geom_json = feat.get("geometry")
         if geom_json is None:
            continue

         shp = shape(geom_json)
         if shp.is_empty:
            continue

         geom_type = geom_json.get("type")
         fid = feat.get("id") or feat.get("properties", {}).get("id") or "0"

         if fid not in current_rhombus_dict:
            current_rhombus_dict[fid] = []

         if geom_type in ["Point", "MultiPoint"]:
            if index == 0:
               current_rhombus_dict[fid].append(shp)
            continue

         # --- EXPLICIT TOPOLOGY PROCESSING ---
         # Transforms the shapes and safely handles vertex dropping directly without exceptions
         transformed_shp = _process_geometry_dropping_vertices(shp, x_start, y_start, is_even)

         if transformed_shp.is_empty:
            continue

         if not transformed_shp.is_valid:
            transformed_shp = make_valid(transformed_shp)

         # Extract sub components directly
         clean_debug_parts = _extract_same_dimension_geoms(transformed_shp, geom_type)
         for part in clean_debug_parts:
            if not part.is_empty:
               debug_feat = feat.copy()
               debug_feat["geometry"] = mapping(part)
               debug_list.append(debug_feat)

         if transformed_shp.intersects(target_box):
            try:
               clipped = transformed_shp.intersection(target_box)
            except Exception:
               clipped = transformed_shp.buffer(0).intersection(target_box)

            filtered_parts = _extract_same_dimension_geoms(clipped, geom_type)
            for part in filtered_parts:
               if not part.is_valid:
                  part = make_valid(part)
                  current_rhombus_dict[fid].extend(_extract_same_dimension_geoms(part, geom_type))
               else:
                  current_rhombus_dict[fid].append(part)

   # Optimized Bulk file logging block
   #for index in range(10):
   #   with open(f"debug5x6/{index}.geojson", "w") as f:
   #      json.dump({"type": "FeatureCollection", "features": debug_file_registry[index]}, f)

   # Unify geometry results
   out_gj = {"type": "FeatureCollection", "features": []}
   for feat in features:
      geom_json = feat.get("geometry")
      if not geom_json:
         continue

      geom_type = geom_json.get("type")
      fid = feat.get("id") or feat.get("properties", {}).get("id") or "0"

      # Aggregate matching geometry pieces from every root rhombus dictionary collection
      all_rhombus_pieces = []
      for index in range(10):
         if fid in rhombi_geometry_store[index]:
            all_rhombus_pieces.extend(rhombi_geometry_store[index][fid])

      if all_rhombus_pieces:
         if geom_type in ["Point", "MultiPoint"]:
            unified_geom = all_rhombus_pieces[0] if len(all_rhombus_pieces) == 1 else unary_union(all_rhombus_pieces)
         else:
            # --- COMBINED SNAP-TO-GRID UNION PASS ---
            # Bypasses precision cracks natively at the C-level using your original grid snap size
            unified_geom = shapely.union_all(all_rhombus_pieces, grid_size=1e-10)

            if unified_geom.geom_type == "GeometryCollection":
               unified_geom = shapely.union_all(_extract_same_dimension_geoms(unified_geom, geom_type), grid_size=1e-10)

            if "Multi" in geom_type and not unified_geom.geom_type.startswith("Multi"):
               if "Polygon" in geom_type:
                  unified_geom = MultiPolygon([unified_geom])
               elif "LineString" in geom_type:
                  unified_geom = MultiLineString([unified_geom])

         # Make a shallow copy of the feature block and update only its final geometry
         out_feat = feat.copy()
         out_feat["geometry"] = mapping(unified_geom)
         out_gj["features"].append(out_feat)

   return out_gj

def fix_geojson_file_5x6_topology(input_path: str, output_path: str):
   # Load GeoJSON FeatureCollection from input_path, run the high-level
   # topology fixer (fix_feature_collection_5x6_topology) and write the result
   # to output_path.

   with open(input_path, "r", encoding="utf-8") as fh:
      data = json.load(fh)

   out_fc = fix_feature_collection_5x6_topology(data)

   with open(output_path, "w", encoding="utf-8") as fh:
      json.dump(out_fc, fh, ensure_ascii=False, indent=3)
