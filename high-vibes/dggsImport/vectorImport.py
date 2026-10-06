# dggsImport/vectorImport.py
# Import a GeoJSON vector into a DGGS Data Store by:
# - preparing input once (reproj + topology fix)
# - writing collection-level attributes into attributes.sqlite via DGGSDataStore.write_collection_attributes
# - writing a WKBC file for workers via fg.wkbc.write_wkb_collection_file
# - spawning worker processes that read WKBC, clip per-root-zone, call write_dggs_json_fg,
#   convert to UBJSON+gzip and return blobs
# - orchestrator batches returned blobs and calls store.write_zone_batch(..., precompressed=True)

from dggal import *

import io
import os
import json
import gzip
from concurrent.futures import ProcessPoolExecutor, as_completed
from typing import Any, Dict, List, Optional
import shapely
from shapely.geometry import shape
import ubjson

# Persistent process-local cache for the single global dataset
_WORKER_CACHE = None

try:
   from dggsStore.store import DGGSDataStore
   from fg.reproj import geojson_load, instantiate_projection_for_dggrs_name, reproject_featurecollection
   from fg.fix_topology_5x6 import fix_feature_collection_5x6_topology
   from fg.clippingShapely import clip_featurecollection_to_zone
   from fg.dggsJSONFG import write_dggs_json_fg
   from fg.wkbc import write_wkb_collection_file, read_wkb_collection_file
except(ImportError):
   from ..dggsStore.store import DGGSDataStore
   from ..fg.reproj import geojson_load, instantiate_projection_for_dggrs_name, reproject_featurecollection
   from ..fg.fix_topology_5x6 import fix_feature_collection_5x6_topology
   from ..fg.clippingShapely import clip_featurecollection_to_zone
   from ..fg.dggsJSONFG import write_dggs_json_fg
   from ..fg.wkbc import write_wkb_collection_file, read_wkb_collection_file

def _is_dggrs_5x6(name):
   return name.startswith("IVEA") or name.startswith("RTEA") or name.startswith("ISEA")

# prepare input pipeline (reproj + fix) executed once in parent
def _prepare_input_pipeline(input_path: str, dggrs_name: str, skip_reproj: bool, skip_fix: bool):
   src = geojson_load(input_path)
   if not skip_reproj:
      proj = instantiate_projection_for_dggrs_name(dggrs_name)
      print("Reprojecting to native CRS of", dggrs_name, "...")
      src = reproject_featurecollection(src, proj)
   if not skip_fix:
      if _is_dggrs_5x6(dggrs_name):
         print("Fixing reprojected features topology...")
         src = fix_feature_collection_5x6_topology(src)
   return src

def _initialize_dggal_worker():
   app = Application(appGlobals=globals());
   pydggal_setup(app)

# build blobs in parallel for a batch of base zones
def _build_vector_blobs_processes(store,                         # Pass store directly
                                  ancestors_lookup: dict,         # Pass ancestors lookup directly
                                  store_worker_config: dict,
                                  wkbc_path: str,
                                  dggrs,
                                  base_zones_list: List,
                                  depth: int,
                                  max_root_level: int,
                                  max_workers: int = 16,
                                  executor=None) -> int:          # Now returns total_written count
   parent_pid = os.getpid()
   if not base_zones_list:
      return 0

   total_written = 0                                              # Counter tracked inline here

   if max_workers == 1 or executor is None:
      for bz in base_zones_list:
         bz_text = dggrs.getZoneTextID(bz)

         bz_id, root_blobs = _base_zone_package_worker(
            wkbc_path, int(bz), store_worker_config, dggrs.__class__.__name__, depth, max_root_level
         )
         if root_blobs:
            base_zone = DGGRSZone(bz_id)
            base_ancestors = ancestors_lookup[bz_id]
            entries = {int(DGGRSZone(r_id)): blob_bytes for r_id, blob_bytes in root_blobs.items()}

            print(f"[BATCH] Writing {len(entries)} child roots to store for base_zone={dggrs.getZoneTextID(base_zone)}", flush=True)
            store.write_zone_batch(base_zone=base_zone, entries=entries, base_ancestor_list=base_ancestors, precompressed=True)
            total_written += len(entries)
      return total_written

   futures = {}

   for bz in base_zones_list:
      bz_text = dggrs.getZoneTextID(bz)

      fut = executor.submit(
         _base_zone_package_worker, wkbc_path, int(bz), store_worker_config, dggrs.__class__.__name__, depth, max_root_level
      )
      futures[fut] = bz

   for fut in as_completed(futures):
      bz = futures[fut]
      bz_text = dggrs.getZoneTextID(bz)
      bz_id, root_blobs = fut.result()
      if not root_blobs:
         continue

      base_zone = DGGRSZone(bz_id)
      base_ancestors = ancestors_lookup[bz_id]

      entries: Dict[int, bytes] = {}
      for r_id, blob_bytes in root_entries_map.items() if 'root_entries_map' in locals() else root_blobs.items():
         entries[int(DGGRSZone(r_id))] = blob_bytes

      print(f"[BATCH] Writing {len(entries)} child roots to store for base_zone={dggrs.getZoneTextID(base_zone)}", flush=True)
      store.write_zone_batch(
         base_zone=base_zone,
         entries=entries,
         base_ancestor_list=base_ancestors,
         precompressed=True
      )
      total_written += len(entries)
      print(f"[BATCH] Write complete for base_zone={dggrs.getZoneTextID(base_zone)}", flush=True)

      # Explicitly clear variables from coordinator memory space to allow instant GC
      del root_blobs, entries
      # =====================================================================

   print(f"[COORDINATOR {parent_pid}] Finished processing batch of base zones.", flush=True)
   return total_written

def _process_batch_vector(store,
                          wkbc_path: str,
                          dggrs,
                          base_zones_batch: List[Tuple[Any, List]],
                          depth: int,
                          max_root_level: int,
                          max_workers: int = 16,
                          executor=None) -> int:
   if not base_zones_batch:
      print("[BATCH] Empty base zone batch, skipping", flush=True)
      return 0

   print(f"[BATCH] Processing batch containing {len(base_zones_batch)} intersecting base zones", flush=True)

   worker_config = {
      "_data_root": store.data_root,
      "collection": store.collection,
      "collection_config": store.config
   }

   just_zones = [bz for bz, ancestors in base_zones_batch]
   ancestors_lookup = {int(bz): ancestors for bz, ancestors in base_zones_batch}

   # Modified signature: passes store and ancestors_lookup down so writes execute on-the-fly
   total_written = _build_vector_blobs_processes(
      store=store,
      ancestors_lookup=ancestors_lookup,
      store_worker_config=worker_config,
      wkbc_path=wkbc_path,
      dggrs=dggrs,
      base_zones_list=just_zones,
      depth=depth,
      max_root_level=max_root_level,
      max_workers=max_workers,
      executor=executor
   )

   if not total_written:
      print("[BATCH] No entries produced for any base zones in this batch, skipping database writes", flush=True)
      return 0

   return total_written

def _shift_coords_healpix_vectorized(coords):
   return coords + [2.0 * math.pi, 0.0]

def _shift_individual_primitive_healpix(part):
   x_apex = -0.75 * math.pi
   epsilon = 1e-12
   p_minx, p_miny, p_maxx, p_maxy = part.bounds
   if p_maxx < (x_apex - abs(p_miny) - epsilon):
      return shapely.transform(part, _shift_coords_healpix_vectorized)
   return part

def fix_geometry_components_healpix(shp):
   if shp is None or shp.is_empty:
      return shp

   g_type = shp.geom_type

   if g_type == "MultiPolygon":
      return shapely.geometry.MultiPolygon([_shift_individual_primitive_healpix(p) for p in shp.geoms])
   elif g_type == "MultiLineString":
      return shapely.geometry.MultiLineString([_shift_individual_primitive_healpix(p) for p in shp.geoms])
   elif g_type == "MultiPoint":
      return shapely.geometry.MultiPoint([_shift_individual_primitive_healpix(p) for p in shp.geoms])
   else:
      return _shift_individual_primitive_healpix(shp)

def _base_zone_package_worker(wkbc_path: str,
                              base_zone_id: int,
                              worker_config: dict,
                              dggrs_name: str,
                              depth: int,
                              max_root_level: int) -> Tuple[int, Dict[int, bytes]]:
   worker_pid = os.getpid()
   print(f"[WORKER {worker_pid}] Task started for base_zone_id={base_zone_id} max_root_level={max_root_level}.", flush=True)

   store = DGGSDataStore(worker_config["_data_root"], worker_config["collection"], config=worker_config["collection_config"])
   dggrs = store.dggrs
   base_zone = DGGRSZone(base_zone_id)

   global _WORKER_CACHE
   if _WORKER_CACHE is None:
      print(f"[WORKER {worker_pid}] Cache miss in process {worker_pid}. Reading and materializing WKBC once...", flush=True)
      # 1. READ RAW WORK CHUNK ONCE
      src_fc = read_wkb_collection_file(wkbc_path)

      features = src_fc.get("features", []) or []
      for feat in features:
         geom_dict = feat.get("geometry")
         if geom_dict and "_shapely_geom" not in feat:
            shp = shape(geom_dict)
            # shapely.prepare(shp)
            feat["_shapely_geom"] = shp

      _WORKER_CACHE = src_fc
   else:
      src_fc = _WORKER_CACHE

   if dggrs_name.startswith("HEALPix"):
      a4_0 = 0x40000000000000
      if int(base_zone_id) == a4_0 or dggrs.isZoneDescendantOf(base_zone, DGGRSZone(a4_0), 0):
         print(f"[WORKER {worker_pid}] Base Zone {dggrs.getZoneTextID(base_zone)} is an A4-0 descendant. Flagging for context shift.", flush=True)

         local_features = []
         for feat in src_fc.get("features", []):
            if "_shapely_geom" in feat:
               shifted_feat = dict(feat)
               shifted_feat["_shapely_geom"] = fix_geometry_components_healpix(feat["_shapely_geom"])
               shifted_feat["bbox"] = shifted_feat["_shapely_geom"].bounds
               local_features.append(shifted_feat)
            else:
               local_features.append(feat)

         src_fc = {"type": "FeatureCollection", "features": local_features}

   local_blobs: Dict[int, bytes] = {}

   # Walk the sub-grid roots in pure local memory within this single process context
   roots_iter = store.iter_roots_for_base(base_zone, max_root_level, up_to=False)
   for zone in roots_iter:
      root_zone = DGGRSZone(zone)

      #print(f"[WORKER {worker_pid}] Executing native clip_featurecollection_to_zone math...", flush=True)
      out_fc, indices = clip_featurecollection_to_zone(src_fc, dggrs, root_zone, refined=False)
      feat_list = out_fc.get("features", []) or []
      if not feat_list:
         continue

      #print(f"[WORKER {worker_pid}] Compiling features into DGGS-JSON-FG schema structures...", flush=True)
      dggs_obj = write_dggs_json_fg(out_fc, indices, dggrs, root_zone, depth)

      #print(f"[WORKER {worker_pid}] Serializing payload to binary UBJSON...", flush=True)
      ubbuf = io.BytesIO()
      ubjson.dump(dggs_obj, ubbuf)

      #print(f"[WORKER {worker_pid}] Gzipping binary stream payload...", flush=True)
      gz = gzip.compress(ubbuf.getvalue(), compresslevel=9)

      #print(f"[WORKER {worker_pid}] Task successfully complete for zone_id={int(zone)}.", flush=True)
      local_blobs[int(zone)] = gz

   print(f"[WORKER {worker_pid}] Task complete for base_zone_id={base_zone_id}. Packed Blobs={len(local_blobs)}", flush=True)
   return base_zone_id, local_blobs

def import_vector(input_geojson_path: str,
                  collection_id: str,
                  dggrs_name: str,
                  data_root: str = "data",
                  level: int = None,
                  depth: int = None,
                  batch_size: int = 32,
                  groupSize: int = 5,
                  max_workers: int = 16,
                  skip_reproj: bool = False,
                  skip_fix: bool = False) -> int:

   dggrs_init = globals().get(dggrs_name)
   if dggrs_init is None:
      print("Unsupported DGGRS:", dggrs_name, flush=True)
      return 1
   dggrs = dggrs_init()

   if depth is None:
      depth = dggrs.get64KDepth()
      print(f"[IMPORT] using default depth (get64KDepth) = {depth}", flush=True)

   if level is None:
      print("import_vector requires --level to be specified (absolute quantize level)", flush=True)
      return 1

   data_level = level
   deepest_root_level = max(0, data_level - depth)

   coll_info = {
      "dggrs": dggrs_name, "maxRefinementLevel": data_level, "depth": depth,
      "groupSize": groupSize, "title": collection_id, "description": collection_id, "version": "1.0"
   }
   # dggrs_uri = f"[ogc-dggrs:{dggrs_name}]"

   base = os.path.join(data_root, collection_id)
   os.makedirs(base, exist_ok=True)
   with open(os.path.join(base, "collection.json"), "w", encoding="utf-8") as fh:
      json.dump(coll_info, fh, indent=2)
   print(f"[IMPORT] Wrote collection config to {os.path.join(base, 'collection.json')}", flush=True)

   store = DGGSDataStore(data_root, collection_id, config=coll_info)
   dggrs = store.dggrs
   max_base_level = store._base_level_for_root(deepest_root_level)
   print(
      f"[IMPORT] Computed levels: data_level={data_level} depth={depth} finest_root_level={deepest_root_level} "
      f"max_base_level={max_base_level} batch_size={batch_size}", flush=True
   )
   print(f"[DIAG] using groupSize={groupSize} (recommended default is 5)", flush=True)

   # prepare input once (reproj + fix)
   src = _prepare_input_pipeline(input_geojson_path, dggrs_name, skip_reproj=skip_reproj, skip_fix=skip_fix)

   # write collection-level attributes (features list) into store.attributes.sqlite
   features = src.get("features", []) or []

   fc_min_x, fc_min_y = float('inf'), float('inf')
   fc_max_x, fc_max_y = float('-inf'), float('-inf')

   if features:
      store.write_collection_attributes(features)
      print(f"[IMPORT] wrote collection attributes for {len(features)} features", flush=True)

      for feat in features:
         geom = feat.get("geometry")
         if geom and "bbox" not in feat:
            g_type = geom.get("type")
            coords = geom.get("coordinates", [])
            if g_type == "Point":
               feat["bbox"] = [coords[0], coords[1], coords[0], coords[1]]
            elif g_type in ("LineString", "MultiPoint"):
               xs = [p[0] for p in coords]
               ys = [p[1] for p in coords]
               feat["bbox"] = [min(xs), min(ys), max(xs), max(ys)]
            elif g_type in ("Polygon", "MultiLineString"):
               pts = [p for ring in coords for p in ring]
               xs = [p[0] for p in pts]
               ys = [p[1] for p in pts]
               feat["bbox"] = [min(xs), min(ys), max(xs), max(ys)]
            else:
               feat["bbox"] = shape(geom).bounds

         if "bbox" in feat:
            fb = feat["bbox"]
            if fb[0] < fc_min_x: fc_min_x = fb[0]
            if fb[1] < fc_min_y: fc_min_y = fb[1]
            if fb[2] > fc_max_x: fc_max_x = fb[2]
            if fb[3] > fc_max_y: fc_max_y = fb[3]

   # Widen the global longitudinal limit across the wrap divide for HEALPix grids
   if dggrs_name.startswith("HEALPix") and fc_min_x < -3/4 * math.pi:
      fc_max_x = max(fc_max_x, fc_min_x + 2.0 * math.pi)

   src["bbox"] = [fc_min_x, fc_min_y, fc_max_x, fc_max_y]
   print(f"[IMPORT] Calculated unified FeatureCollection bbox: {src['bbox']}", flush=True)

   # write WKBC file for workers (WKBC contains geometries and feature ids; properties are not included)
   tmp_wkbc_path = os.path.join('/dev/shm' if os.path.exists('/dev/shm') else store.collection_dir, f"tmp_input_{os.getpid()}.wkbc")
   write_wkb_collection_file(src, tmp_wkbc_path)
   print(f"[IMPORT] wrote WKBC to {tmp_wkbc_path}", flush=True)

   pkg_index = 0
   total_written = 0

   with ProcessPoolExecutor(max_workers=max_workers, initializer=_initialize_dggal_worker, max_tasks_per_child=None) as executor:
      for root_level in range(deepest_root_level, -1, -1):
         base_level = store._base_level_for_root(root_level)
         up_to = False

         base_zones_batch: List[Tuple[Any, List]] = []
         skipped_count = 0
         processed_count = 0

         for base_zone, base_ancestors in store.iter_bases(base_level, up_to=up_to):
            current_base_level = dggrs.getZoneLevel(base_zone)
            if current_base_level > root_level:
               continue

            bz_extent = CRSExtent()
            dggrs.getZoneCRSExtent(base_zone, CRS(0), bz_extent)

            # bz_text = dggrs.getZoneTextID(base_zone)

            if dggrs_name.startswith("HEALPix"):
               a4_0 = 0x40000000000000
               if int(base_zone) == a4_0 or dggrs.isZoneDescendantOf(base_zone, DGGRSZone(a4_0), 0):
                  bz_extent.tl = Pointd(bz_extent.tl.x + 2.0 * math.pi, bz_extent.tl.y)
                  bz_extent.br = Pointd(bz_extent.br.x + 2.0 * math.pi, bz_extent.br.y)

            bz_min_x = bz_extent.tl.x if bz_extent.tl.x < bz_extent.br.x else bz_extent.br.x
            bz_max_x = bz_extent.br.x if bz_extent.tl.x < bz_extent.br.x else bz_extent.tl.x
            bz_min_y = bz_extent.tl.y if bz_extent.tl.y < bz_extent.br.y else bz_extent.br.y
            bz_max_y = bz_extent.br.y if bz_extent.tl.y < bz_extent.br.y else bz_extent.tl.y

            # print(f"   [EVAL {bz_text}] Zone Extent: Min_Lat={bz_min_lat:.6f}, Max_Lat={bz_max_lat:.6f} | Min_Lon={bz_min_lon:.6f}, Max_Lon={bz_max_lon:.6f}", flush=True)

            if bz_min_x > fc_max_x or bz_max_x < fc_min_x or bz_min_y > fc_max_y or bz_max_y < fc_min_y:
               skipped_count += 1
               continue

            processed_count += 1
            base_zones_batch.append((base_zone, base_ancestors))

            if len(base_zones_batch) >= batch_size:
               pkg_index += 1
               written = _process_batch_vector(
                  store, tmp_wkbc_path, dggrs, base_zones_batch,
                  depth, root_level, max_workers=max_workers, executor=executor
               )
               total_written += written
               base_zones_batch = []

         if base_zones_batch:
            pkg_index += 1
            written = _process_batch_vector(
               store, tmp_wkbc_path, dggrs, base_zones_batch,
               depth, root_level, max_workers=max_workers, executor=executor
            )
            total_written += written
            base_zones_batch = []

         print(f"[LEVEL {root_level}] Kept={processed_count}, Pruned/Skipped={skipped_count}, Total Written={total_written}", flush=True)

   # cleanup temporary WKBC
   if os.path.exists(tmp_wkbc_path):
      os.remove(tmp_wkbc_path)

   print(f"[IMPORT] complete; total written spatial data packets={total_written}", flush=True)
   return 0
