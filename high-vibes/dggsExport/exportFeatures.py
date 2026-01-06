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
from shapely.geometry import shape, mapping, Point, MultiPoint, LineString, MultiLineString, Polygon, MultiPolygon, GeometryCollection

from ogcapi.utils import pretty_json
from fg.dggsJSONFG import read_dggs_json_fg
from dggsStore.store import DGGSDataStore, iter_packages
from fg.reproj import instantiate_projection_for_dggrs_name
from fg.dggsJSONFG import unproject_and_fix

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

# merge_shapely_geometries: merge iterable of Shapely geometries and return a single Shapely geometry
# - strict contract: geoms is an iterable of Shapely geometry objects (caller responsibility)
# - do_buffer defaults to False
# - if do_buffer is True and grid_size == 0, perform a single buffer(0) call; otherwise perform buffer(grid).buffer(-grid)
def merge_shapely_geometries(
   geoms: Iterable[Any],
   *,
   do_buffer: bool = False,
   grid_size: float = 1e-10
) -> Optional[Any]:
   # convert iterable to list (caller must supply valid Shapely geometries)
   geom_list = list(geoms)
   if not geom_list:
      return None

   # single geometry -> use directly
   if len(geom_list) == 1:
      merged = geom_list[0]
   else:
      merged = shapely.union_all(geom_list, grid_size=grid_size)

   if merged is None or merged.is_empty:
      return None

   # optional cleanup: single buffer when grid_size == 0, otherwise buffer(grid).buffer(-grid)
   if do_buffer:
      if grid_size == 0:
         merged = merged.buffer(0)
      else:
         merged = merged.buffer(grid_size).buffer(-grid_size)

   # return merged Shapely geometry
   return merged

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
   projection = instantiate_projection_for_dggrs_name(store.config['dggrs'])
   ge = GeoExtent()
   store.dggrs.getZoneWGS84Extent(base_zone_id, ge)
   extent = [float(ge.ll.lon), float(ge.ll.lat), float(ge.ur.lon), float(ge.ur.lat)]

   result: Dict[int, bytes] = {}
   for fid, geoms in features.items():
      merged_geojson = combine_geojson_geometries(geoms)
      if merged_geojson is None: continue
      # free memory for this entry
      geoms.clear()

      if merged_geojson:
         merged_geojson = unproject_and_fix(projection, extent, merged_geojson, fid, refine_wgs84=None) #1e-2)

      if merged_geojson is None: continue

      shp = shape(merged_geojson)
      if shp is None: continue

      # merge shapely geometries (worker-level union across parts), no buffer cleanup here
      merged_shp = merge_shapely_geometries([shp], do_buffer=False, grid_size=grid_size)
      if merged_shp is None: continue

      # serialize to WKB (binary) and store under integer feature id
      result[fid] = _wkb.dumps(merged_shp, hex=False)

   Instance.delete(projection)

   gc.collect()
   return result

# orchestrator: receives list of worker results (each Dict[int, bytes]),
# aggregates WKBs per feature id, rehydrates to Shapely, calls merge_shapely_geometries(do_buffer=True),
# converts final Shapely geometry to GeoJSON mapping
def orchestrator_finalize(
   package_results: List[Dict[int, bytes]],
   projection,
   *,
   grid_size: float = GRID_SIZE_DEFAULT
) -> Dict[int, dict]:
   # aggregate WKB lists per feature id
   agg: Dict[int, List[bytes]] = {}
   for pkg in package_results:
      for fid, wkb_bytes in pkg.items():
         if fid not in agg:
            agg[fid] = []
         agg[fid].append(wkb_bytes)

   # merge per-feature across workers, perform final buffer cleanup, convert to GeoJSON
   final_geoms: Dict[int, dict] = {}
   #extent = [-180,-90,180,90]
   for fid, wkb_list in agg.items():
      # rehydrate all WKBs to Shapely geometries
      shps = [_wkb.loads(b) for b in wkb_list]
      # merge across workers and perform final cleanup (do_buffer=True)
      merged = merge_shapely_geometries(shps, do_buffer=True, grid_size=grid_size)
      geojson = mapping(merged) if merged else None
      # REVIEW: It would be ideal to unproject at the end, but it currently runs into topology issues
      #if geojson:
      #   geojson = unproject_and_fix(projection, extent, geojson, fid, refine_wgs84=1e-2)
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

   projection = None #instantiate_projection_for_dggrs_name(store.config['dggrs'])

   with ProcessPoolExecutor(max_workers=worker_count) as ex:
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
   final_geoms: Dict[int, dict] = orchestrator_finalize(package_results, projection, grid_size=grid_size)

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
