# dggsExport/exportFeatures.py
# GeoJSON exporter for DGGSDataStore
# - worker re-opens DGGSDataStore by path and collection and returns a dict mapping fid -> WKB
# - main thread merges package results, unions geometries with shapely, then populates attributes

from dggal import *

from typing import Dict, Any, List, Optional, Iterable
from concurrent.futures import ProcessPoolExecutor, as_completed
import os
import gc
import sys
import traceback
import shapely
from shapely import wkb as _wkb
from shapely.geometry import shape, mapping, Point, MultiPoint, LineString, MultiLineString, Polygon, MultiPolygon, GeometryCollection, box
from shapely.ops import polygonize
import math
from functools import partial
import numpy as np
import json

try:
   from ogcapi.utils import pretty_json
   from fg.dggsJSONFG import read_dggs_json_fg
   from dggsStore.store import DGGSDataStore, iter_packages
   from fg.reproj import instantiate_projection_for_dggrs_name
   from fg.dggsJSONFG import unproject_and_fix, PolarRootMode
   from fg.fix_topology_5x6 import _get_rhombus_layout, _process_geometry_dropping_vertices, _extract_same_dimension_geoms
except(ImportError):
   from ..ogcapi.utils import pretty_json
   from ..fg.dggsJSONFG import read_dggs_json_fg
   from ..dggsStore.store import DGGSDataStore, iter_packages
   from ..fg.reproj import instantiate_projection_for_dggrs_name
   from ..fg.dggsJSONFG import unproject_and_fix, PolarRootMode
   from ..fg.fix_topology_5x6 import _get_rhombus_layout, _process_geometry_dropping_vertices, _extract_same_dimension_geoms

GRID_SIZE_DEFAULT = 1e-2
WORKERS = 16

def combine_geojson_geometries(geoms: List[Optional[Dict[str, Any]]]) -> Optional[Dict[str, Any]]:
   # Merge a list of GeoJSON geometry dicts into a single GeoJSON geometry dict.
   # - geoms: list of geometry dicts (e.g., {"type":"Polygon","coordinates":...}) or None
   # - returns a single geometry dict or None
   if not geoms:
      return None

   # filter out falsy entries
   items = [g for g in geoms if g]
   if not items:
      return None

   # classify by family
   pts: List[List[float]] = []
   lines: List[List[List[float]]] = []
   polys: List[List[List[List[float]]]] = []
   others: List[Dict[str, Any]] = []

   for g in items:
      t = g.get("type")
      if t == "Point":              pts.append(g["coordinates"])     # coords: [x,y,...]
      elif t == "MultiPoint":       pts.extend(g["coordinates"])     # coords: [[x,y], ...]
      elif t == "LineString":       lines.append(g["coordinates"])   # coords: [[x,y], ...]
      elif t == "MultiLineString":  lines.extend(g["coordinates"])   # coords: [[[x,y],...], ...]
      elif t == "Polygon":          polys.append(g["coordinates"])   # coords: [[ring], [ring], ...]  (ring = [[x,y],...])
      elif t == "MultiPolygon":     polys.extend(g["coordinates"])   # coords: [[[ring],...], [[ring],...], ...]
      else:                         others.append(g)  # unknown or GeometryCollection etc. keep original

   # if mixed families present, return GeometryCollection preserving order:
   families_present = sum(bool(x) for x in (pts, lines, polys, others))
   if families_present > 1:
      # preserve original geometries order from items
      coll = []
      for g in items:
         coll.append(g)
      return {"type": "GeometryCollection", "geometries": coll}

   # only points
   if pts and not lines and not polys and not others:
      if len(pts) == 1:
         return {"type": "Point", "coordinates": pts[0]}
      else:
         return {"type": "MultiPoint", "coordinates": pts}

   # only lines
   if lines and not pts and not polys and not others:
      if len(lines) == 1:
         return {"type": "LineString", "coordinates": lines[0]}
      else:
         return {"type": "MultiLineString", "coordinates": lines}

   # only polygons
   if polys and not pts and not lines and not others:
      if len(polys) == 1:
         # single polygon: keep Polygon shape
         return {"type": "Polygon", "coordinates": polys[0]}
      else:
         # multiple polygons -> MultiPolygon (each polygon is a list of rings)
         return {"type": "MultiPolygon", "coordinates": polys}

   # only others (e.g., GeometryCollection or unknown types)
   if others and not pts and not lines and not polys:
      if len(others) == 1:
         return others[0]
      else:
         return {"type": "GeometryCollection", "geometries": others}

   # fallback: if nothing matched, return None
   return None

def _snap_coordinates_vectorized(coords: np.ndarray, sz_dlon: float) -> np.ndarray:
   # Fast vectorized snap function for coordinate arrays
   x = coords[:, 0]

   # Check standard quadrant boundaries (-180, -90, 0, 90, 180)
   mask_180 = np.abs(np.abs(x) - 180.0) <= sz_dlon
   coords[mask_180, 0] = np.where(x[mask_180] >= 0, 180.0, -180.0)

   coords[np.abs(x - -90.0) <= sz_dlon, 0] = -90.0
   coords[np.abs(x - 0.0) <= sz_dlon, 0] = 0.0
   coords[np.abs(x - 90.0) <= sz_dlon, 0] = 90.0

   return coords

def _fix_geometries_5x6_topology(geom_list, fid):
   if not geom_list:
      return None

   shapes_to_process = list(geom_list) if isinstance(geom_list, (list, tuple)) else [geom_list]
   geom_type = shapes_to_process[0].geom_type
   all_rhombus_pieces = []

   # Ensure our targeted debug subdirectory is ready
   #debug_dir = os.path.join(os.getcwd(), "debug_dumps", f"fid_{fid}")
   #os.makedirs(debug_dir, exist_ok=True)
   #pid = 0

   for index in range(10):
      x_start, y_start, is_even = _get_rhombus_layout(index)
      target_box = box(x_start, y_start, x_start + 1.0, y_start + 1.0)

      local_face_shps = []
      for s_idx, shp in enumerate(shapes_to_process):
         if not shp or shp.is_empty:
            continue

         # --- ATOMIC TOPOLOGY CONVERSION ---
         transformed_shp = _process_geometry_dropping_vertices(shp, x_start, y_start, is_even)

         if not transformed_shp.is_empty:
            if not transformed_shp.is_valid:
               try:
                  validShape = shapely.make_valid(transformed_shp, method="structure", keep_collapsed=False)
               except:
                  validShape = shapely.make_valid(transformed_shp)
            else:
               validShape = transformed_shp
            if validShape.geom_type == "GeometryCollection":
               polys = [g for g in validShape.geoms if "Polygon" in g.geom_type]
               if polys:
                  validShape = polys[0] if len(polys) == 1 else shapely.MultiPolygon(polys)

            local_face_shps.append(validShape)

      # Process intersections directly using your original master function logic
      for transformed_shp in local_face_shps:
         if transformed_shp.intersects(target_box):
            try:
               clipped = transformed_shp.intersection(target_box)
            except Exception:
               clipped = transformed_shp.buffer(0).intersection(target_box)

            filtered_parts = _extract_same_dimension_geoms(clipped, geom_type)
            for part in filtered_parts:
               if not part.is_empty:
                  all_rhombus_pieces.append(part)

   if not all_rhombus_pieces:
      return None

   # --- COMBINED SNAP-TO-GRID UNION PASS ---
   unified_geom = shapely.union_all(all_rhombus_pieces, grid_size=1e-10)
   return unified_geom


def _merge_and_clean_dggs_geometry(
   shps,
   projection,
   valid_5x6_space,
   grid_size,
   do_buffer,
   subzone_level,
   ggg_snap,
   fid=None
):
   if not shps:
      return None

   geom_type = shps[0].geom_type

   # 1. AREAL ROUTE: Polygons / MultiPolygons
   if "Polygon" in geom_type:
      is_projected_5x6 = projection is not None and type(projection).__name__.startswith(("IVEA", "RTEA", "ISEA"))

      if is_projected_5x6:
         # print("5x6 fixing for feature", fid, "...")
         merged = _fix_geometries_5x6_topology(shps, fid)

         #shps = [shapely.make_valid(s) for s in shps if s and not s.is_empty]
         #merged = shps[0] if len(shps) == 1 else shapely.union_all(shps, grid_size=grid_size)

         #merged_geojson = mapping(merged)
         #_dump_debug_geojson(merged_geojson, fid, stage_name="un_dissolved_grid")
      else:
         shps = [shapely.make_valid(s) for s in shps if s and not s.is_empty]

         merged = shps[0] if len(shps) == 1 else shapely.union_all(shps, grid_size=grid_size)

      # Shared buffering and cell-healing step for all areal geometries
      if merged is not None and not merged.is_empty and do_buffer:
         if grid_size == 0:
            merged = merged.buffer(0)
         else:
            healed = merged.buffer(grid_size).buffer(-grid_size)
            if True and is_projected_5x6:
               # Re-clip against valid space to prune bled edges and force clean topology boundaries
               re_clipped = shapely.intersection(healed, valid_5x6_space, grid_size=0)
               clean_polys = _extract_pure_polygons(re_clipped)
               merged = clean_polys[0] if len(clean_polys) == 1 else shapely.MultiPolygon(clean_polys)
            else:
               merged = healed

   # 2. LINEAR ROUTE: LineStrings / MultiLineStrings
   elif "Line" in geom_type:
      flat_lines = []
      for s in shps:
         if s.geom_type == "LineString":
            flat_lines.append(s)
         elif s.geom_type == "MultiLineString":
            flat_lines.extend(s.geoms)

      merged = flat_lines[0] if len(flat_lines) == 1 else shapely.MultiLineString(flat_lines)

      if merged is not None and not merged.is_empty and do_buffer:
         if grid_size > 0:
            merged = shapely.snap(merged, merged, tolerance=grid_size)

         if merged.geom_type == "GeometryCollection":
            lines = [g for g in merged.geoms if g.geom_type in ("LineString", "MultiLineString")]
         elif merged.geom_type == "MultiLineString":
            lines = list(merged.geoms)
         else:
            lines = [merged]

         if lines:
            merged = lines[0] if len(lines) == 1 else shapely.ops.linemerge(lines)
         else:
            merged = None

   # 3. POINT / FALLBACK ROUTE: Points, MultiPoints, etc.
   else:
      merged = shps[0] if len(shps) == 1 else shapely.union_all(shps, grid_size=grid_size)

   # Apply final global GNOSIS coordinate snaps if enabled on valid areal outputs
   if merged and ggg_snap and merged.geom_type in ("Polygon", "MultiPolygon"):
      sz_dlon = 90.0 / (2 ** subzone_level)
      merged = shapely.transform(merged, partial(_snap_coordinates_vectorized, sz_dlon=sz_dlon))
   # return merged Shapely geometry
   return merged

def _initialize_dggal_worker():
   app = Application(appGlobals=globals());
   pydggal_setup(app)

def _is_dggrs_5x6(name):
   return name.startswith("IVEA") or name.startswith("RTEA") or name.startswith("ISEA")

# worker: collects GeoJSON per feature id, coalesces with combine_geojson_geometries,
# converts to Shapely, merges with merge_shapely_geometries(do_buffer=False),
# serializes merged geometry to WKB, and returns Dict[int, bytes]
def _worker_process_package(
   datastore_path: str,
   collection: str,
   pkg_path: str,
   base_zone_id: int,
   root_level: int,
   target_level: int,
   debug: bool,
   grid_size: float = GRID_SIZE_DEFAULT
) -> Dict[int, bytes]:
   store = DGGSDataStore(datastore_path, collection)
   print(f'Processing root zones of level {root_level} under base zone {store.dggrs.getZoneTextID(base_zone_id)} in process {os.getpid()}', flush=True)

   # accumulate GeoJSON geometries per feature id (string keys while reading)
   features: Dict[int, List[Dict[str, Any]]] = {}

   for root_zone in store.iter_roots_for_base(base_zone_id, root_level, up_to=False):
      dggsubjson = store.read_and_decode_zone_blob(pkg_path, root_zone)
      geojson = read_dggs_json_fg(dggsubjson, unproject=False, refine_wgs84=None) if dggsubjson else None
      if geojson:
         feats = geojson.get('features', []) or []
         for feat in feats:
            geom_json = feat.get('geometry')
            fid = feat.get('id')

            # We're assuming non-zero integer feature IDs
            if not fid or not isinstance(fid, int):
               raise BaseException

            # append raw GeoJSON geometry (may be None)
            if geom_json is not None:
               if fid not in features:
                  features[fid] = []
               features[fid].append(geom_json)

   # merge per-feature and serialize to WKB; worker does NOT run final buffer cleanup
   dggrs_name = store.config['dggrs']
   projection = instantiate_projection_for_dggrs_name(store.config['dggrs'])
   ggg_snap = False if projection else True
   ge = GeoExtent()
   store.dggrs.getZoneWGS84Extent(base_zone_id, ge)
   is5x6 = _is_dggrs_5x6(dggrs_name)
   extent = [float(ge.ll.lon), float(ge.ll.lat), float(ge.ur.lon), float(ge.ur.lat)]

   subzone_level = target_level
   # does this bounding box touches the literal geodetic pole limits?
   if is5x6:
      is_polar_root = PolarRootMode.RI5x6 # Automatic pole detection so this should always be set
   elif (extent[1] <= -90.0 + 1e-7 or extent[3] >= 90.0 - 1e-7):
      is_polar_root = PolarRootMode.GGG if not projection else PolarRootMode.RHEALPIX if dggrs_name.startswith("rHEALPix") else PolarRootMode.HEALPIX
   else:
      is_polar_root = PolarRootMode.NONE

   result: Dict[int, bytes] = {}
   for fid, geoms in features.items():
      merged_geojson = combine_geojson_geometries(geoms)
      if merged_geojson is None: continue
      # free memory for this entry
      geoms.clear()

      if merged_geojson and (not projection or not is5x6):
         merged_geojson = unproject_and_fix(
            projection, extent, merged_geojson, fid, refine_wgs84=None, fix_geom=True,
            root_level=root_level, subzone_level=subzone_level, is_polar_root=is_polar_root
         ) #1e-2)

      if merged_geojson is None: continue

      shp = shape(merged_geojson)
      if shp is None: continue

      # merge shapely geometries (worker-level union across parts), no buffer cleanup here
      merged_shp = shp #merge_shapely_geometries([shp], do_buffer=False, grid_size=grid_size, ggg_snap=ggg_snap)
      if merged_shp is None: continue

      # serialize to WKB (binary) and store under integer feature id
      result[fid] = _wkb.dumps(merged_shp, hex=False)

   if projection: Instance.delete(projection)

   gc.collect()
   return result

# Helper to process an individual coordinate ring and add intermediate polar points
def _process_polar_ring(coords: list, offsets: list) -> list:
   if not coords:
      return coords

   new_coords = []
   L = len(coords)

   for i in range(L):
      p = coords[i]
      new_coords.append(p)

      if i < L - 1:
         n = coords[i + 1]
         # Verify both endpoints are locked onto the exact same polar baseline
         if p in (-90.0, 90.0) and n == p:
            lon_start, lat = p, p
            lon_end = n

            delta = lon_end - lon_start
            midpoint_dist = abs(delta) / 2.0
            direction = 1.0 if delta >= 0 else -1.0

            # Step forward from the starting node
            for offset in offsets:
               if offset < midpoint_dist:
                  new_coords.append((lon_start + (offset * direction), lat))

            # Step backward from the ending node
            for offset in reversed(offsets):
               if offset < midpoint_dist:
                  new_coords.append((lon_end - (offset * direction), lat))

   return new_coords


# Helper to process an individual coordinate ring and add intermediate polar points
def _process_polar_ring(coords: list, offsets: list) -> list:
   if not coords:
      return coords

   new_coords = []
   L = len(coords)

   for i in range(L):
      p = coords[i]
      new_coords.append(p)

      if i < L - 1:
         n = coords[i + 1]
         # Verify both endpoints are locked onto the exact same polar latitude baseline
         if p[1] in (-90.0, 90.0) and n[1] == p[1]:
            lon_start = p[0]
            lat = p[1]
            lon_end = n[0]

            delta = lon_end - lon_start
            midpoint_dist = abs(delta) / 2.0
            direction = 1.0 if delta >= 0 else -1.0

            # Step forward from the starting node
            for offset in offsets:
               if offset < midpoint_dist:
                  new_coords.append((lon_start + (offset * direction), lat))

            # Step backward from the ending node
            for offset in reversed(offsets):
               if offset < midpoint_dist:
                  new_coords.append((lon_end - (offset * direction), lat))

   return new_coords


# Traverses a Shapely geometry and injects extra structural vertices near polar nodes
def _inject_polar_densification_points(geom: shapely.geometry.base.BaseGeometry) -> shapely.geometry.base.BaseGeometry:
   if geom is None or geom.is_empty:
      return geom

   # Single array containing all required directional border offsets
   offsets = [1.0, 2.0, 4.0, 8.0, 16.0, 32.0]

   if isinstance(geom, shapely.geometry.Polygon):
      exterior = _process_polar_ring(list(geom.exterior.coords), offsets)
      interiors = [_process_polar_ring(list(hole.coords), offsets) for hole in geom.interiors]
      return shapely.geometry.Polygon(exterior, interiors)

   elif isinstance(geom, shapely.geometry.MultiPolygon):
      new_polys = []
      for poly in geom.geoms:
         exterior = _process_polar_ring(list(poly.exterior.coords), offsets)
         interiors = [_process_polar_ring(list(hole.coords), offsets) for hole in poly.interiors]
         new_polys.append(shapely.geometry.Polygon(exterior, interiors))
      return shapely.geometry.MultiPolygon(new_polys)

   return geom

def extract_same_type_only(geom, target_type: str):
   if geom.is_empty:
      return geom
   if geom.geom_type == target_type:
      return geom
   # Handle cases where the geometry is a multi-type but within the same family
   if geom.geom_type.startswith("Multi") and target_type in geom.geom_type:
      return geom
   # Flatten and filter out multi-dimensional leakage or GeometryCollections
   if hasattr(geom, "geoms"):
      parts = []
      for g in geom.geoms:
         if g.geom_type == target_type:
            parts.append(g)
         elif g.geom_type.startswith("Multi") and target_type in g.geom_type:
            parts.extend(g.geoms)
      if not parts:
         return shapely.geometry.GeometryCollection()
      if len(parts) == 1:
         return parts[0]
      if target_type == "Polygon":
         return shapely.geometry.MultiPolygon(parts)
      if target_type == "LineString":
         return shapely.geometry.MultiLineString(parts)
      if target_type == "Point":
         return shapely.geometry.MultiPoint(parts)
   return geom

# Process-safe utility that saves intermediate 5x6 geometries with descriptive names
def _dump_debug_geojson(geojson_dict: Any, fid: Any, stage_name: str) -> None:
   try:
      dump_dir = os.path.join(os.getcwd(), "debug_dumps")
      os.makedirs(dump_dir, exist_ok=True)

      fid_str = str(fid) if fid is not None else "unknown"
      pid = os.getpid()

      # Clear, meaningful filename format reflecting the processing stage
      filename = f"5x6_layout_fid_{fid_str}_stage_{stage_name}_pid_{pid}.geojson"
      file_path = os.path.join(dump_dir, filename)

      # geojson_dict is already a mapped dictionary, dump it directly
      with open(file_path, "w") as df:
         json.dump(geojson_dict, df, indent=2)
      print(f"💾 DEBUG SUCCESS: Dumped {stage_name} geometry to {file_path}")
   except Exception as dump_err:
      print(f"⚠️ DEBUG FAILURE: Could not dump debug stage ({stage_name}): {dump_err}")

# Filters out linear and point artifacts, returning only pure Polygon pieces
def _extract_pure_polygons(geometry_collection: Any) -> List[Polygon]:
   pure_polygons = []
   if geometry_collection.geom_type == 'Polygon':
      pure_polygons.append(geometry_collection)
   elif geometry_collection.geom_type in ('MultiPolygon', 'GeometryCollection'):
      for part in geometry_collection.geoms:
         if part.geom_type == 'Polygon':
            pure_polygons.append(part)
   return pure_polygons

def calculate_grid_size(dggrs, subzone_level, projection):
   linear_m = math.sqrt(dggrs.getRefZoneArea(subzone_level))
   earth_linear_m = math.sqrt(5.100656217240885092949E14)
   units_linear = 0.0

   if projection is None:
      units_linear = math.sqrt(4.0 * (180.0 ** 2.0) / math.pi)
   else:
      proj_type = type(projection)
      if proj_type in (IVEAProjection, RTEAProjection, ISEAProjection):
         units_linear = math.sqrt(10.0)
      elif proj_type in (HEALPixProjection, rHEALPixProjection):
         units_linear = math.sqrt(6.0 * (math.pi / 2.0) ** 2.0)

   return linear_m * (units_linear / earth_linear_m)

def prepare_valid_5x6_space():
   # 1. Programmatically reconstruct the true 10-rhombus valid space footprint
   rhombus_polygons = []
   def _get_rhombus_layout(index: int):
      x_start = float(index // 2)
      y_start = float(index // 2) if (index % 2 == 0) else float((index // 2) + 1)
      return x_start, y_start, (index % 2 == 0)
   for index in range(10):
       x_start, y_start, is_even = _get_rhombus_layout(index)
       vertices = [
            (x_start, y_start),
            (x_start + 1.0, y_start),
            (x_start + 1.0, y_start + 1.0),
            (x_start, y_start + 1.0),
            (x_start, y_start)
       ]
       rhombus_polygons.append(Polygon(vertices))
   valid_5x6_space = shapely.unary_union(rhombus_polygons)
   return valid_5x6_space

# orchestrator: receives list of worker results (each Dict[int, bytes]),
# aggregates WKBs per feature id, rehydrates to Shapely, calls merge_shapely_geometries(do_buffer=True),
# converts final Shapely geometry to GeoJSON mapping
def orchestrator_finalize(
   package_results: List[Dict[int, bytes]],
   projection,
   *,
   grid_size: float = GRID_SIZE_DEFAULT,
   root_level=None,
   subzone_level=None,
   is_polar_root=None,
   dggrs=None
) -> Dict[int, dict]:
   # aggregate WKB lists per feature id
   agg: Dict[int, List[bytes]] = {}
   for pkg in package_results:
      for fid, wkb_bytes in pkg.items():
         if fid not in agg:
            agg[fid] = []
         agg[fid].append(wkb_bytes)

   ggg_snap = False if projection else True

   # merge per-feature across workers, perform final buffer cleanup, convert to GeoJSON
   final_geoms: Dict[int, dict] = {}

   extent = [-180,-90,180,90]

   is5x6 = _is_dggrs_5x6(type(dggrs).__name__)

   valid_5x6_space = prepare_valid_5x6_space() if is5x6 else None

   if projection and not is5x6:
      grid_size = 10 * calculate_grid_size(dggrs, subzone_level, projection)
   else:
      grid_size = (1.1 if is5x6 else 1.1 if projection else 10) * calculate_grid_size(dggrs, subzone_level, projection)
   print(f"Selected a grid_size of {grid_size} CRS units to heal sub-zone gaps")

   finalSnap = None
   for fid, wkb_list in agg.items():
      # rehydrate all WKBs to Shapely geometries
      shps = [_wkb.loads(b) for b in wkb_list]
      # merge across workers and perform final cleanup (do_buffer=True)

      gs = grid_size
      if projection and not is5x6 and shps and "Polygon" in shps[0].geom_type:
         gs *= 59

      final_merged_geometry = _merge_and_clean_dggs_geometry(
         shps=shps,
         projection=projection,
         valid_5x6_space=valid_5x6_space,
         grid_size=gs,
         do_buffer=True,
         subzone_level=subzone_level,
         ggg_snap=ggg_snap,
         fid=fid
      )

      if final_merged_geometry is None or final_merged_geometry.is_empty:
         continue

      if projection and is5x6:
         # Map the unified geometry directly to GeoJSON
         merged_geojson = mapping(final_merged_geometry)
         #_dump_debug_geojson(merged_geojson, fid, stage_name="dissolved_final")

         merged_geojson = unproject_and_fix(
               projection, extent, merged_geojson, fid, refine_wgs84=None, fix_geom=True,
               root_level=root_level, subzone_level=subzone_level, is_polar_root=is_polar_root
            ) #1e-2)
         merged = shape(merged_geojson) if merged_geojson else None
      else:
         merged = final_merged_geometry

      if projection and merged and not merged.is_empty and merged.geom_type in ("Polygon", "MultiPolygon"):
         if finalSnap is None:
            is4R = is5x6 and type(dggrs).__name__.endswith(("4R"))
            is9R = is5x6 and type(dggrs).__name__.endswith(("9R"))

            if is9R:
               finalSnap = grid_size * 1400 # at 1100 Antarctica has gaps for 9R
            elif is4R:
               finalSnap = grid_size * 850 # at 800 Antarctica has gaps for 4R
            elif not is5x6 and projection:
               finalSnap = grid_size * 1
            else:
               finalSnap = grid_size * 62
            print("Final buffer snapping of", finalSnap)
         merged = merged.buffer(finalSnap).buffer(-finalSnap)
         merged = merged.intersection(box(-180.0, -90.0, 180.0, 90.0))

      # Determine primary geometry family before modifications change it
      primary_type = "Polygon"
      if shps:
         if "Line" in shps[0].geom_type:
            primary_type = "LineString"
         elif "Point" in shps[0].geom_type:
            primary_type = "Point"

      if merged and merged.geom_type in ("Polygon", "MultiPolygon") and not merged.is_empty:
         merged = shapely.ops.orient(merged, sign=1.0)
         # Inject the extra polar points right after all buffering/fixing actions finish
         if merged:
            merged = _inject_polar_densification_points(merged)

      elif merged and not merged.is_empty:
         if merged.geom_type in ("MultiLineString", "GeometryCollection"):
            lines = [g for g in merged.geoms if g.geom_type == "LineString"]
            if lines:
               merged = lines[0] if len(lines) == 1 else shapely.ops.linemerge(lines)

      if merged and not merged.is_empty:
         if not merged.is_valid:
            merged = shapely.validation.make_valid(merged)
            merged = shapely.ops.orient(merged, sign=1.0)
            try:
               merged = shapely.validation.make_valid(merged, method="structure")
            except TypeError:
               merged = shapely.validation.make_valid(merged)

         # Unified dynamic extraction for all geometry type families
         merged = extract_same_type_only(merged, primary_type)

         if merged and not merged.is_empty:
            merged = shapely.ops.orient(merged, sign=1.0)

      geojson = mapping(merged) if merged else None
      final_geoms[fid] = geojson

   return final_geoms

def export_to_geojson(
   store: DGGSDataStore,
   sampling_level: int,
   output_path: str,
   *,
   level: Optional[int] = None,
   workers: Optional[int] = None,
   debug: bool = False,
   max_packages: Optional[int] = None,
   grid_size: float = GRID_SIZE_DEFAULT
) -> None:
   """
   Orchestrate export:
   - sampling_level: requested sampling level (CLI --level)
   - clamps to store.maxRefinementLevel
   - computes root_level = max(0, sampling_level_clamped - store.depth)
   - computes base_level = store._base_level_for_root(root_level)
   - iterates packages at base_level, dispatches one package per worker
   - merges package results, unions geometries, populates attributes, writes GeoJSON
   """
   requested_level = sampling_level if level is None else level
   sampling_level_clamped = min(requested_level, store.maxRefinementLevel)
   root_level = max(0, sampling_level_clamped - store.depth)
   base_level = store._base_level_for_root(root_level)

   cpu_count = os.cpu_count() or 1
   worker_count = workers if workers is not None else min(WORKERS, max(1, cpu_count))

   datastore_path = store.data_root
   collection = store.collection

   pkg_iter = iter_packages(store, base_level)

   futures = []
   package_results: List[Dict[int, bytes]] = []
   submitted = 0

   projection = instantiate_projection_for_dggrs_name(store.config['dggrs'])

   with ProcessPoolExecutor(max_workers=worker_count, initializer=_initialize_dggal_worker) as ex:
      for pkg_path, base_zone_id, base_ancestors_ids in pkg_iter:
         if max_packages and submitted >= max_packages:
            break
         submitted += 1
         fut = ex.submit(
            _worker_process_package,
            datastore_path,
            collection,
            pkg_path,
            base_zone_id,
            root_level,
            sampling_level_clamped,
            debug,
         )
         futures.append(fut)

      for fut in as_completed(futures):
         exc = fut.exception()
         if exc:
            traceback.print_exception(type(exc), exc, exc.__traceback__, file=sys.stderr)
            raise BaseException
            continue
         res = fut.result()
         if not res:
            continue
         package_results.append(res)

   # aggregate and finalize geometries from workers
   print("All zone data processed, merging final features...")

   dggrs_name = store.config['dggrs']
   is5x6 = _is_dggrs_5x6(dggrs_name)

   if is5x6:
      is_polar_root = PolarRootMode.RI5x6 # Automatic pole detection so this should always be set
   elif True: #(extent[1] <= -90.0 + 1e-7 or extent[3] >= 90.0 - 1e-7):
      is_polar_root = PolarRootMode.GGG if not projection else PolarRootMode.RHEALPIX if dggrs_name.startswith("rHEALPix") else PolarRootMode.HEALPIX
   else:
      is_polar_root = PolarRootMode.NONE

   final_geoms: Dict[int, dict] = orchestrator_finalize(package_results, projection, grid_size=grid_size,
      root_level=root_level,subzone_level=sampling_level_clamped, is_polar_root=is_polar_root, dggrs=store.dggrs)

   if projection:
      Instance.delete(projection)

   # build feature list from finalized geometries (workers do not return props)
   out_features: List[Dict[str, Any]] = []
   for fid in sorted(final_geoms):
      out_features.append({
         'type': 'Feature',
         'id': fid,
         'properties': {},
         'geometry': final_geoms[fid]
      })

   print("Populating feature attributes...")
   # populate attributes from store
   ids = [f['id'] for f in out_features]
   if ids:
      attrs_map = store.get_attributes_for_feature_ids(ids)
      for feat in out_features:
         feat['properties'] = attrs_map.get(feat['id'], {}) or {}

   print("Writing final geoJSON (", output_path, ")...")
   out_obj = {'type': 'FeatureCollection', 'features': out_features}
   with open(output_path, 'w', encoding='utf-8') as fh:
      fh.write(pretty_json(out_obj))
      fh.write('\n')

   gc.collect()
