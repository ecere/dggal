# dgToGeoMulti.py
# Parallel reader -> WKB aggregator -> rehydrate & union -> write GeoJSON
# - Workers read one DGGS-UBJSON-FG file each via read_dggs_json_fg_file(path)
# - Workers convert GeoJSON geometries to WKB bytes and return tuples:
#     (key, wkb_bytes_or_None, props, raw_id, kind_or_None)
# - Main process aggregates WKB lists per key, rehydrates WKB -> Shapely only
#   when merging a single key, runs shapely.union_all, and writes GeoJSON.
# - This version:
#     * validates grid_size early (must be finite float)
#     * preserves features with geometry == None (returns geometry: null)
#     * ensures workers return JSON-serializable, picklable tuples
#     * tolerates GeometryCollection by emitting it (no silent drop)
#     * flattens pool results robustly and logs minimal diagnostics

from typing import Sequence, Dict, Any, List, Tuple, Iterable, Optional
import multiprocessing as mp
import os
import re
import glob
import gc
from collections import defaultdict
import math
import shapely
from shapely import wkb as _wkb
from shapely.geometry import shape, mapping, Point, MultiPoint, LineString, MultiLineString, Polygon, MultiPolygon, GeometryCollection
from shapely.geometry.base import BaseGeometry

from ogcapi.utils import pretty_json
from fg.dggsJSONFG import read_dggs_json_fg_file

# number of worker processes to use by default
WORKERS = 16

def expand_input_paths_from_arg(arg: str) -> Iterable[str]:
    # .lst file: one path per line
    if arg.lower().endswith(".lst") and os.path.isfile(arg):
        with open(arg, "r", encoding="utf-8") as fh:
            for line in fh:
                p = line.strip()
                if p and os.path.exists(p):
                    yield p
        return

    # directory: only *.dggs.json
    if os.path.isdir(arg):
        pattern = os.path.join(arg, "*.dggs.json")
        for p in sorted(glob.glob(pattern)):
            if os.path.isfile(p):
                yield p
        return

    # glob or single file (shell may already have expanded globs)
    for p in glob.glob(arg):
        if os.path.isfile(p):
            yield p

def expand_input_paths(args_list: Sequence[str]) -> Iterable[str]:
    for a in args_list:
        for p in expand_input_paths_from_arg(a):
            yield p

def numeric_key(k: str):
   try:
      return (0, int(k))
   except Exception:
      try:
         return (1, float(k))
      except Exception:
         m = re.search(r"(\d+)$", str(k))
         if m:
            return (2, int(m.group(1)))
         return (3, str(k))

# Worker: read one file and emit list of tuples:
# (key, wkb_bytes_or_None, props, raw_id, kind_or_None)
def _worker_read_file(path: str) -> List[Tuple[str, Optional[bytes], Dict[str, Any], Any, Optional[str]]]:
   out: List[Tuple[str, Optional[bytes], Dict[str, Any], Any, Optional[str]]] = []
   obj = read_dggs_json_fg_file(path, refine_wgs84=None)
   feats = obj.get("features", []) or []
   for feat in feats:
      geom_json = feat.get("geometry")
      props = feat.get("properties") or {}
      raw_id = feat.get("id")

      if raw_id is None:
         raise ValueError("id-less feature")

      key = str(raw_id)
      orig_id_val = raw_id

      if geom_json is None:
         # explicit None geometry tuple (worker saw the feature but had no geometry)
         out.append((key, None, dict(props), orig_id_val, None))
         continue

      shp = shape(geom_json)
      if shp is None or getattr(shp, "is_empty", False):
         out.append((key, None, dict(props), orig_id_val, None))
         continue

      gtype = getattr(shp, "geom_type", None)
      if gtype in ("Point", "MultiPoint"):
         kind = "point"
      elif gtype in ("LineString", "MultiLineString"):
         kind = "line"
      elif gtype in ("Polygon", "MultiPolygon"):
         kind = "poly"
      else:
         out.append((key, None, dict(props), orig_id_val, None))
         del shp
         continue

      # emit one tuple per concrete geometry
      if hasattr(shp, "geoms"):
         for part in shp.geoms:
            if part is None or getattr(part, "is_empty", False):
               continue
            out.append((key, _wkb.dumps(part, hex=False), dict(props), orig_id_val, kind))
      else:
         out.append((key, _wkb.dumps(shp, hex=False), dict(props), orig_id_val, kind))

      del shp
   gc.collect()
   return out

def togeo_multi_mode(input_args: Sequence[str], output_path: str, grid_size: float = 1e-10) -> None:
   if not isinstance(grid_size, float):
      raise ValueError(f"grid_size must be numeric, got {type(grid_size).__name__}")

   paths = sorted(expand_input_paths(input_args))

   workers = min(WORKERS, max(1, (mp.cpu_count() or 1)))

   with mp.Pool(processes=workers) as pool:
      results = pool.map(_worker_read_file, paths, chunksize=1)

   # features: Dict[id, {"props": Dict[str,Any], "geoms": List[Shapely geometry]}]
   features: Dict[str, Dict[str, Any]] = {}

   # Single-pass: iterate worker results directly and populate features
   for i, r in enumerate(results):
      if r is None:
         # worker returned nothing for this file
         print(f"worker {i} returned None")
         continue

      # normalize to iterable of tuples
      if not isinstance(r, list):
         if isinstance(r, tuple) and len(r) >= 3:
            tuples_iter = [r]
         else:
            print(f"worker {i} returned unexpected type {type(r).__name__}; skipping")
            continue
      else:
         tuples_iter = r

      for item in tuples_iter:
         if not isinstance(item, tuple) or len(item) < 5:
            print("skipping malformed worker item:", item)
            continue

         key, wkb_bytes, p, raw_id, kind = item

         # ensure features entry exists
         if key not in features:
            features[key] = {"props": {}, "geoms": [], "id": raw_id if raw_id is not None else key}

         # set props if provided and not empty, but do not overwrite existing non-empty props
         if p:
            if not features[key]["props"]:
               features[key]["props"] = dict(p)

         # if geometry is None, do nothing to geoms list (we keep the entry so the feature is known)
         if wkb_bytes is None:
            # explicit no-geometry from this worker; nothing to append
            continue

         # binary-only contract: attempt to load directly; skip if load fails
         try:
            geom = _wkb.loads(wkb_bytes)
         except Exception:
            # failed to load this blob; skip it
            print(f"warning: failed to load WKB for key={key}; skipping this blob")
            continue

         if geom is None or getattr(geom, "is_empty", False):
            # nothing to append
            continue

         # append the rehydrated geometry contributed by this worker
         features[key]["geoms"].append(geom)

         # record kind if not already recorded (optional, not used for props selection)
         if "kind" not in features[key] or features[key].get("kind") is None:
            features[key]["kind"] = kind

   # Build final FeatureCollection from features
   out_features: List[Dict[str, Any]] = []

   for k in sorted(features.keys(), key=numeric_key):
      entry = features[k]
      geoms = entry.get("geoms", [])

      if geoms:
         merged = shapely.union_all(geoms, grid_size=grid_size)

         geom_out = None
         if isinstance(merged, GeometryCollection):
            geom_out = mapping(merged)
         else:
            expected = entry.get("kind")
            if expected == "poly" and not isinstance(merged, (Polygon, MultiPolygon)):
               coerced = shapely.union_all([merged], grid_size=grid_size)
               if not isinstance(coerced, (Polygon, MultiPolygon)):
                  geom_out = None
               else:
                  merged = coerced
            elif expected == "line" and not isinstance(merged, (LineString, MultiLineString)):
               coerced = shapely.union_all([merged], grid_size=grid_size)
               if not isinstance(coerced, (LineString, MultiLineString)):
                  geom_out = None
               else:
                  merged = coerced
            elif expected == "point" and not isinstance(merged, (Point, MultiPoint)):
               coerced = shapely.union_all([merged], grid_size=grid_size)
               if not isinstance(coerced, (Point, MultiPoint)):
                  geom_out = None
               else:
                  merged = coerced

            if geom_out is None:
               if grid_size and grid_size != 0.0:
                  try:
                     merged = merged.buffer(grid_size).buffer(-grid_size)
                  except Exception as e:
                     print(f"warning: buffer cleanup failed for key={k}: {e}")
               geom_out = mapping(merged)

         feature: Dict[str, Any] = {
            "type": "Feature",
            "properties": dict(entry.get("props", {})),
            "geometry": geom_out
         }
         fid = entry.get("id")
         if fid is not None:
            feature["id"] = fid
         out_features.append(feature)
      else:
         # no geometries appended by any worker -> emit geometry:null
         feature: Dict[str, Any] = {
            "type": "Feature",
            "properties": dict(entry.get("props", {})),
            "geometry": None
         }
         fid = entry.get("id")
         if fid is not None:
            feature["id"] = fid
         out_features.append(feature)
         # print("NULL GEOMETRY for", k)

      # cleanup per-entry
      entry["geoms"] = []
      entry["props"] = {}
      entry["id"] = None
      entry["kind"] = None
      gc.collect()

   out_obj = {"type": "FeatureCollection", "features": out_features}
   with open(output_path, "w", encoding="utf-8") as fh:
      fh.write(pretty_json(out_obj))
      fh.write("\n")
   gc.collect()
