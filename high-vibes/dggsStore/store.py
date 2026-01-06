# dggsStore.py
from dggal import *
import os
import json
import sqlite3
import gzip
import threading
import logging
import array
from typing import Any, Dict, List, Optional, Tuple, Iterable, TypedDict, Mapping, Sequence
from types import MethodType
import ubjson
from concurrent.futures import ThreadPoolExecutor, as_completed

DGGS_JSON_SCHEMA_URI = "https://schemas.opengis.net/ogcapi/dggs/1.0/core/schemas/dggs-json/dggs-json.json"

# --- types ---
class ShapeDict(TypedDict, total=False):
   count: int
   subZones: int
   dimensions: Mapping[str, int]

class ValueEntry(TypedDict):
   depth: int
   shape: ShapeDict
   data: Sequence[Optional[float]]

ValuesObject = Mapping[str, "List[ValueEntry]"]
CollectedValues = Mapping[int, ValuesObject]

def decompress_blob(blob: bytes) -> bytes:
   if blob is None:
      return None
   raw = gzip.decompress(blob) if blob[:2] == b"\x1f\x8b" else blob
   return raw

def decode_blob(blob: bytes) -> Optional[Any]:
   if blob is None:
      return None
   raw = gzip.decompress(blob) if blob[:2] == b"\x1f\x8b" else blob
   return ubjson.loadb(raw)

def to_blob(obj: Any, compress: bool = True) -> bytes:
   ub = ubjson.dumpb(obj)
   return gzip.compress(ub) if compress else ub


logger = logging.getLogger("dggsStore")

store_cache: Dict[str, "DGGSDataStore"] = {}
store_lock = threading.Lock()
dggrs_cache: Dict[str, DGGRS] = {}
dggrs_lock = threading.Lock()

# We'll extend the DGGRS class with getZonePrimaryChildren() and getZonePrimaryParent() which are not yet part of DGGAL
def getZonePrimaryChildren7H(self, zone):
   children = self.getZoneChildren(zone)
   if children is not None:
      children.count = 7 # The first 7 children are the primary children for 7H
   return children

def getZonePrimaryChildren3H(self, zone):
   # The 3H children are associated with the parent who is itself a centroid child (snowflake fractal)
   return self.getZoneChildren(zone) if self.isZoneCentroidChild(zone) else [ self.getZoneCentroidChild(zone) ]

def getZonePrimaryParent3H(self, zone):
   primaryParent = None
   parents = self.getZoneParents(zone)
   nParents = 0 if parents is None else len(parents)
   if nParents == 1:
      primaryParent = parents[0]
   elif nParents > 0:
      # NOTE: as of DGGAL 0.0.6, parents[0] is not always the primary parent for 3H
      for p in parents:
         if self.isZoneCentroidChild(p):
            primaryParent = p
            break;
   if parents is not None:
      Instance.delete(parents)
   return primaryParent

def getZonePrimaryParent0(self, zone):
   parents = self.getZoneParents(zone)
   # NOTE: parents[0] should always be the primary parent for 7H
   primaryParent = parents[0] if parents is not None and len(parents) > 0 else None
   if parents is not None:
      Instance.delete(parents)
   return primaryParent

# These match the getSubZones() order

def getSubZoneWeights7H(self, zone: DGGRSZone, depth: int):
   if depth != 1:
      return None  # Relative depth > 1 not yet implemented

   nEdges = self.countZoneEdges(zone)
   if nEdges == 5: # Pentagon
      return [
         1/12.0,                             # 1
         1/12.0, 11/12.0, 11/12.0, 1/12.0,   # 2-5
         11/12.0, 1.0, 11/12.0,              # 6-8
         1/12.0, 11/12.0, 1/12.0             # 9-11
      ]
   else:           # Hexagon
      return [
         1/12.0,                             # 1
         1/12.0, 11/12.0, 11/12.0, 1/12.0,   # 2-5
         11/12.0, 1.0, 11/12.0,              # 6-8
         1/12.0, 11/12.0, 11/12.0, 1/12.0,   # 9-12
         1/12.0                              # 13
      ]

def getSubZoneWeights3H(self, zone: DGGRSZone, depth: int):
   if depth != 1:
      return None  # Relative depth > 1 not yet implemented

   nEdges = self.countZoneEdges(zone)
   if nEdges == 5: # Pentagon
      return [
         1/3.0, 1/3.0,       # 1-2
         1/3.0, 1.0, 1/3.0,  # 3-5
         1/3.0               # 6
      ]
   else:           # Hexagon
      return [
         1/3.0, 1/3.0,       # 1-2
         1/3.0, 1.0, 1/3.0,  # 3-5
         1/3.0, 1/3.0        # 6-7
      ]

def getSubZoneWeightsNested(self, zone: DGGRSZone, depth: int):
   return None  # Fully nested DGGRSs sub-zones have equal weighting

# These match the getZoneChildren() order

_3H_CH_WEIGHTS = array.array('d', [1.0, 1.0/3.0, 1.0/3.0, 1.0/3.0, 1.0/3.0, 1.0/3.0, 1.0/3.0])
_7H_CH_WEIGHTS = array.array('d', [1.0,
   11/12.0, 11/12.0, 11/12.0, 11/12.0, 11/12.0, 11/12.0,
    1/12.0,   1/12.0, 1/12.0,  1/12.0,  1/12.0,  1/12.0 ])

def getChildrenWeights7H(self, zone: DGGRSZone):
   return _7H_CH_WEIGHTS

#def getChildrenWeights7H(self, zone: DGGRSZone):
#   nEdges = self.countZoneEdges(zone)
#   if nEdges == 5: # Pentagon
#      return [
#         1.0, # Centroid
#         11/12.0, 11/12.0, 11/12.0, 11/12.0, 11/12.0,  # Primary Children
#          1/12.0,  1/12.0,  1/12.0,  1/12.0,  1/12.0   # Secondary Children
#      ]
#   else:           # Hexagon
#      return [
#         1.0, # Centroid
#         11/12.0, 11/12.0, 11/12.0, 11/12.0, 11/12.0, 11/12.0, # Primary Children
#          1/12.0,  1/12.0,  1/12.0,  1/12.0,  1/12.0,  1/12.0  # Secondary Children
#      ]

def getChildrenWeights3H(self, zone: DGGRSZone):
   return _3H_CH_WEIGHTS

#def getChildrenWeights3H(self, zone: DGGRSZone):
#   nEdges = self.countZoneEdges(zone)
#   if nEdges == 5: # Pentagon
#      return [
#         1.0, # Centroid
#         1/3.0, 1/3.0, 1/3.0, 1/3.0, 1/3.0        # Vertex Children
#      ]
#   else:           # Hexagon
#      return [
#         1.0, # Centroid
#         1/3.0, 1/3.0, 1/3.0, 1/3.0, 1/3.0, 1/3.0 # Vertex Children
#      ]

def getChildrenWeightsNested(self, zone: DGGRSZone):
   return None  # Fully nested DGGRSs sub-zones have equal weighting

def get_or_create_dggrs(dggrsID: str) -> DGGRS:
   key = f"dggrs:{dggrsID}"
   with dggrs_lock:
      dggrs = dggrs_cache.get(key)
      if dggrs is not None:
         return dggrs
      cls = globals().get(dggrsID)
      if cls is None:
         print(f"DGGRS class not found: {dggrsID!r}")
         return None
      dggrs = cls()

      maxNB = dggrs.getMaxNeighbors()
      if maxNB == 6: # This is only true for 3H and 7H as of DGGAL 0.0.6
         # Hexagonal DGGRS
         ratio = dggrs.getRefinementRatio()
         if ratio == 3:
            dggrs.getZonePrimaryChildren = MethodType(getZonePrimaryChildren3H, dggrs)
            dggrs.getZonePrimaryParent = MethodType(getZonePrimaryParent3H, dggrs)
            dggrs.getSubZoneWeights = MethodType(getSubZoneWeights3H, dggrs)
            dggrs.getChildrenWeights = MethodType(getChildrenWeights3H, dggrs)
         else:
            dggrs.getZonePrimaryChildren = MethodType(getZonePrimaryChildren7H, dggrs)
            dggrs.getZonePrimaryParent = MethodType(getZonePrimaryParent0, dggrs)
            dggrs.getSubZoneWeights = MethodType(getSubZoneWeights7H, dggrs)
            dggrs.getChidlrenWeights = MethodType(getChildrenWeights7H, dggrs)
      else:
         # Fully nested DGGRS
         dggrs.getZonePrimaryChildren = dggrs.getZoneChildren
         dggrs.getZonePrimaryParent = MethodType(getZonePrimaryParent0, dggrs)
         dggrs.getSubZoneWeights = MethodType(getSubZoneWeightsNested, dggrs)
         dggrs.getChildrenWeights = MethodType(getChildrenWeightsNested, dggrs)

      dggrs_cache[key] = dggrs
      return dggrs

def ensure_package_table(path: str) -> None:
   os.makedirs(os.path.dirname(path), exist_ok=True)
   conn = sqlite3.connect(path)
   conn.execute("PRAGMA synchronous=OFF")
   cur = conn.cursor()
   cur.execute("""
     CREATE TABLE IF NOT EXISTS zone_data(
       root_zone_id TEXT PRIMARY KEY,
       data BLOB NOT NULL
     )""")
   conn.commit()
   conn.close()

def write_sqlite_two_col(path: str, entries: List[Tuple[str, bytes]]) -> int:
   ensure_package_table(path)
   conn = sqlite3.connect(path)
   conn.execute("PRAGMA synchronous=OFF")
   cur = conn.cursor()
   for zid, blob in entries:
      cur.execute("INSERT OR REPLACE INTO zone_data(root_zone_id, data) VALUES(?,?)", (zid, blob))
   conn.commit()
   conn.close()
   return len(entries)

def read_package_root_ids_from_sqlite(pkg_path: str, limit: Optional[int] = None) -> List[str]:
   if not pkg_path or not os.path.exists(pkg_path):
      return []
   conn = sqlite3.connect(pkg_path)
   conn.row_factory = sqlite3.Row
   if limit is None:
      cur = conn.execute("SELECT root_zone_id FROM zone_data")
   else:
      cur = conn.execute("SELECT root_zone_id FROM zone_data LIMIT ?", (limit,))
   rows = cur.fetchall()
   conn.close()
   return [r[0] for r in rows if r[0] is not None]

class DGGSDataStore:
   def __init__(self, data_root: str, collection: str, config: Optional[dict] = None):
      self.data_root = data_root
      self.collection = collection
      self.collection_dir = os.path.join(data_root, collection)

      if config is not None:
         os.makedirs(os.path.dirname(self.collection_dir), exist_ok=True)
         self.config = config
      else:
         if not os.path.isdir(self.collection_dir):
            print(f"Collection directory not found: {self.collection_dir!r}")
            return
         cfg_path = os.path.join(self.collection_dir, "collection.json")
         logger.info("Loading collection config from %s", cfg_path)
         if not os.path.isfile(cfg_path):
            print(f"collection.json not found at {cfg_path!r}")
            return
         with open(cfg_path, "r", encoding="utf-8") as f:
            self.config = json.load(f)

      dggrsID = self.config.get("dggrs")
      if dggrsID is None:
         print("Missing 'dggrs' in collection config")
         return
      self.dggrs = get_or_create_dggrs(dggrsID)

      if "depth" not in self.config:
         print("Missing 'depth' in collection config")
         return
      self.depth = int(self.config["depth"])

      maxRefinementLevel = self.config.get("maxRefinementLevel")
      if maxRefinementLevel is None:
         print("Missing 'maxRefinementLevel' in collection config")
      self.maxRefinementLevel = int(maxRefinementLevel)

      self._compute_groups()
      self._compute_fields()

   def _compute_fields(self):
      # Prefer collection-level attributes.sqlite (vector collection) first,
      # otherwise inspect a package .sqlite (raster case).
      self.fields: List[str] = None

      attr_path = self._attributes_db_path()
      if os.path.exists(attr_path):
         conn = sqlite3.connect(f"file:{os.path.abspath(attr_path)}?mode=ro", uri=True, check_same_thread=False)
         cur = conn.cursor()
         cur.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='attributes'")
         if cur.fetchone():
            cur.execute("PRAGMA table_info(attributes)")
            cols = [r[1] for r in cur.fetchall()]
            conn.close()
            cols = [c for c in cols if c != "feature_id"]
            if cols:
               self.fields = cols
         else:
            conn.close()
      else:
         # attributes.sqlite not present -> inspect package .sqlite files
         sample_pkg = None
         for root, _, names in os.walk(self.collection_dir):
            for n in names:
               if n.endswith(".sqlite"):
                  sample_pkg = os.path.join(root, n)
                  break
            if sample_pkg:
               break

         if sample_pkg:
            root_ids = read_package_root_ids_from_sqlite(sample_pkg, limit=1)
            if root_ids:
               sample_root = root_ids[0]
               conn2 = sqlite3.connect(sample_pkg)
               conn2.row_factory = sqlite3.Row
               cur2 = conn2.execute("SELECT data FROM zone_data WHERE root_zone_id = ?", (sample_root,))
               r2 = cur2.fetchone()
               conn2.close()
               if r2:
                  blob = r2["data"]
                  if blob:
                     raw = gzip.decompress(blob) if blob[:2] == b"\x1f\x8b" else blob
                     decoded = ubjson.loadb(raw)
                     values_map = decoded.get("values") if isinstance(decoded, dict) else None
                     if isinstance(values_map, dict):
                        self.fields = list(values_map.keys())
      # print("Computed fields: ", self.fields)

   def _compute_groups(self) -> None:
       self.groupSize = self.config.get("groupSize", 1)
       self.deepest_root = max(0, self.maxRefinementLevel - self.depth)
       count = self.deepest_root + 1
       num_groups = (count + self.groupSize - 1) // self.groupSize
       self.group0Size = count - (num_groups - 1) * self.groupSize

   def _is_base_level(self, level: int) -> bool:
      # Return True if `level` is a group base (level 0 or group0Size + k*groupSize).
      if level == 0:
         return True
      if level < self.group0Size:
         return False
      return (level - self.group0Size) % self.groupSize == 0

   def _base_level_for_root(self, root_level: int) -> int:
      if root_level == 0:
         base_level = 0
      else:
         if root_level < self.group0Size:
            base_level = 0
         else:
            k = (root_level - self.group0Size) // self.groupSize
            base_level = self.group0Size + k * self.groupSize
      # print("Returned base = ", base_level, " for ", root_level)
      return base_level

   def _compute_ancestral_group_base_list(self, zone: DGGRSZone) -> list:
       # Collect group-base ancestors in digging-down order (top -> ... -> deepest).
       # Walks from `zone` up to root, but PREPENDS each discovered base so the final
       # list is ordered as if discovered by digging down.
       # Each entry is a tuple (level, zone).

       dggrs = self.dggrs
       bases: list = []

       current_level = dggrs.getZoneLevel(zone)
       while True:
           if self._is_base_level(current_level):
               # prepend so final order is top -> ... -> deepest
               bases.insert(0, zone)
           zone = dggrs.getZonePrimaryParent(zone)
           if zone is None:
               break
           current_level = dggrs.getZoneLevel(zone)

       return bases

   def compute_package_path_for_root_zone(self, zone: DGGRSZone, base_ancestor_list: Optional[list] = None) -> str:
      # Compute package path for root `zone`. If `base_ancestor_list` is not provided,
      # build it with _compute_ancestral_group_base_list. The list is expected in
      # digging-down order (top -> ... -> deepest). All base ancestors are added
      # as parent directories in that same order; the filename is derived from the
      # closest-to-zone base (the last item in the list).

      dggrs = self.dggrs
      if base_ancestor_list is None:
         base_ancestor_list = self._compute_ancestral_group_base_list(zone)

      # base_ancestor_list is top -> ... -> deepest
      base_texts = [dggrs.getZoneTextID(int(n)) for n in base_ancestor_list if n is not None]

      # closest-to-zone base is the last entry
      base_zone = base_ancestor_list[-1]
      base_level = dggrs.getZoneLevel(base_zone)
      base_text = base_texts[-1]

      group_end = base_level + (self.group0Size if base_level == 0 else self.groupSize) - 1
      filename = f"{base_text}_L{group_end}.sqlite"

      # directories are ALL base_texts except the last, in the same top->... order
      dirs = base_texts[:-1]

      parts = [self.collection_dir] + dirs + [filename]

      return os.path.join(*parts)

   def read_zone_blob(self, pkg_path: str, zone: DGGRSZone) -> Optional[bytes]:
      if not os.path.isfile(pkg_path):
         return None
      zone_text = self.dggrs.getZoneTextID(zone)
      conn = sqlite3.connect(pkg_path)
      conn.row_factory = sqlite3.Row
      cur = conn.execute("SELECT data FROM zone_data WHERE root_zone_id = ?", (zone_text,))
      row = cur.fetchone()
      conn.close()
      if not row:
         logger.warning("read_zone_blob: package=%s missing root_zone_id=%s", pkg_path, zone_text)
         return None
      return row["data"]

   def read_and_decode_zone_blob(self, pkg_path: str, zone: DGGRSZone) -> dict | None:
      return decode_blob(self.read_zone_blob(pkg_path, zone))

   def read_package_root_ids(self, pkg_path: str, limit: Optional[int] = None) -> set:
      ids = read_package_root_ids_from_sqlite(pkg_path, limit=limit)
      return set(ids)

   def _iter_lvl0_seeds(self) -> Iterable[Any]:
      dggrs = self.dggrs
      seeds = dggrs.listZones(0, wholeWorld)
      if not seeds:
         return
      for lvl0 in seeds:
         yield lvl0
      Instance.delete(seeds)

   def iter_bases_under_lvl0(self, lvl0: DGGRSZone, base_level: int, up_to: bool = False,
      in_extent_cb=None) -> Iterable[Tuple[DGGRSZone, List[DGGRSZone]]]:
      if base_level <= 0:
         return
      dggrs = self.dggrs
      stack: List[Tuple[DGGRSZone, List[DGGRSZone]]] = []

      # start from lvl0's children (iter_bases yields lvl0 itself when appropriate)
      children0 = dggrs.getZonePrimaryChildren(lvl0) or []
      for child in children0:
         stack.append((child, [lvl0]))
      if not isinstance(children0, list):
         Instance.delete(children0)

      while stack:
         zone, base_ancestors = stack.pop()

         if in_extent_cb is not None and not in_extent_cb(zone):
            continue

         zone_level = dggrs.getZoneLevel(zone)

         # exact base level: yield and do not descend
         if zone_level == base_level:
            yield (zone, base_ancestors + [zone])
            continue

         # coarser base when up_to=True: yield but still descend to find finer bases
         if up_to and zone_level < base_level and self._is_base_level(zone_level):
            yield (zone, base_ancestors + [zone])
            children = dggrs.getZoneChildren(zone) or []
            for child in children:
               stack.append((child, base_ancestors + [zone]))
            if not isinstance(children, list):
               Instance.delete(children)
            continue

         # otherwise descend normally
         children = dggrs.getZonePrimaryChildren(zone) or []
         for child in children:
            stack.append((child, base_ancestors))
         if not isinstance(children, list):
            Instance.delete(children)

   def iter_bases(self, base_level: int, up_to: bool = False, in_extent_cb=None) -> Iterable[Tuple[DGGRSZone, List[DGGRSZone]]]:
      if not self._is_base_level(base_level):
         return
      dggrs = self.dggrs

      for lvl0 in self._iter_lvl0_seeds():
         lvl0_level = dggrs.getZoneLevel(lvl0)

         # yield lvl0 itself when it qualifies
         if self._is_base_level(lvl0_level) and (lvl0_level == base_level or (up_to and lvl0_level <= base_level)):
            yield (lvl0, [lvl0])

         if base_level > 0:
            # delegate to helper, forwarding flags (do not prefetch)
            for base_zone, base_ancestors in self.iter_bases_under_lvl0(
               lvl0,
               base_level,
               up_to=up_to,
               in_extent_cb=in_extent_cb,
            ):
               yield (base_zone, base_ancestors)

   def iter_roots_for_base(self, base_zone: DGGRSZone, level: int, up_to: bool=False, in_extent_cb=None) -> Iterable[DGGRSZone]:
      dggrs = self.dggrs
      base_level = dggrs.getZoneLevel(base_zone)

      # nothing to do if requested level is above base_level when not up_to
      if not up_to and level == base_level:
         yield base_zone
         return

      # include base itself for up_to when in range
      if up_to and base_level <= level:
         yield base_zone

      # if requested level is shallower than base_level, nothing more to traverse
      if level < base_level:
         return

      stack: List[Tuple[DGGRSZone, int]] = []
      children = dggrs.getZonePrimaryChildren(base_zone) or []
      for child in children:
         stack.append((child, base_level + 1))
      if not isinstance(children, list):
         Instance.delete(children)

      while stack:
         zone, ref = stack.pop()
         zid = int(zone)

         # yield according to up_to vs exact semantics
         if up_to:
            if base_level <= ref <= level:
               yield zone
         else:
            if ref == level:
               yield zone
               continue

         # extent filter
         if in_extent_cb is not None and not in_extent_cb(zone):
            continue

         # only descend while ref < level (children would be ref+1)
         if ref < level:
            children = dggrs.getZonePrimaryChildren(zone) or []
            for child in children:
               stack.append((child, ref + 1))
            if not isinstance(children, list):
               Instance.delete(children)

   def list_zones_with_data_at_level(self, root_level: int, as_textIDs: bool = False) -> List[Any]:
      result: List[Any] = []
      dggrs = self.dggrs
      base_level = self._base_level_for_root(root_level)

      for base_zone, base_ancestors in self.iter_bases(base_level, up_to=False):
         pkg = self.compute_package_path_for_root_zone(base_zone, base_ancestors)
         if not pkg:
            continue
         root_ids = self.read_package_root_ids(pkg)
         if not root_ids:
            continue
         for root_zone in self.iter_roots_for_base(base_zone, root_level, up_to=False):
            zid_text = dggrs.getZoneTextID(root_zone)
            if zid_text and zid_text in root_ids:
               result.append(zid_text if as_textIDs else root_zone)
      return result

   def write_zone_batch(self,
                        base_zone: 'DGGRSZone',
                        entries: Dict['DGGRSZone', Any],
                        base_ancestor_list: Optional[List['DGGRSZone']] = None,
                        pkg_path: Optional[str] = None,
                        precompressed: bool = False,
                        max_workers: int = 8) -> None:
      dggrs = self.dggrs

      if pkg_path is None:
         pkg_path = self.compute_package_path_for_root_zone(base_zone, base_ancestor_list=base_ancestor_list)

      items: List[Tuple[str, Any]] = []
      for zone_obj, data_obj in entries.items():
         zone_text = dggrs.getZoneTextID(zone_obj)
         items.append((zone_text, data_obj))

      output_rows: List[Tuple[str, bytes]] = []

      if items:
         if precompressed:
            for zone_text, data_obj in items:
               output_rows.append((zone_text, data_obj))
         else:
            workers = min(max_workers, max(1, len(items)))
            with ThreadPoolExecutor(max_workers=workers) as ex:
               fut_map = {}
               for zone_text, data_obj in items:
                  fut = ex.submit(to_blob, data_obj)
                  fut_map[fut] = zone_text

               for fut in as_completed(fut_map):
                  zone_text = fut_map[fut]
                  output_rows.append((zone_text, fut.result()))

      pkg_dir = os.path.dirname(pkg_path)
      write_sqlite_two_col(pkg_path, output_rows)

   # Vector Attributes DB
   def _attributes_db_path(self) -> str:
      return os.path.join(self.collection_dir, "attributes.sqlite")

   def _ensure_attributes_db(self) -> None:
      path = self._attributes_db_path()
      os.makedirs(self.collection_dir, exist_ok=True)
      conn = sqlite3.connect(path)
      conn.execute("PRAGMA journal_mode = WAL")
      conn.commit()
      conn.close()

   def write_collection_attributes(self, features: List[Dict[str, Any]]) -> None:
      # features: list of {"id": int_or_str, "properties": {...}}
      # Normalize feature ids to integers; allocate numeric ids if missing or non-integer.
      self._ensure_attributes_db()
      path = self._attributes_db_path()
      conn = sqlite3.connect(path)
      cur = conn.cursor()

      # sequence table for generated ids
      cur.execute("CREATE TABLE IF NOT EXISTS attr_seq(name TEXT PRIMARY KEY, value INTEGER NOT NULL)")
      cur.execute("INSERT OR IGNORE INTO attr_seq(name, value) VALUES('feature_id', 1)")

      # collect attribute keys and sample types
      keys = {}
      for feat in features:
         props = feat.get("properties", {}) or {}
         for k, v in props.items():
            if k in keys:
               continue
            if isinstance(v, str):
               keys[k] = "text"
            elif isinstance(v, bool):
               keys[k] = "bool"
            elif isinstance(v, (int, float)):
               keys[k] = "number"
            else:
               # arrays and objects -> JSON
               keys[k] = "json"

      # create lookup tables for text attributes
      for k, t in keys.items():
         if t == "text":
            lk = f"attr_{k}_lk"
            cur.execute(
               f"CREATE TABLE IF NOT EXISTS {lk}("
               " id INTEGER PRIMARY KEY AUTOINCREMENT,"
               " value TEXT UNIQUE"
               ")"
            )

      # create main attributes table with dynamic columns
      cols = ["feature_id INTEGER PRIMARY KEY"]
      for k, t in keys.items():
         if t == "text":
            cols.append(f"\"{k}\" INTEGER")        # FK to attr_<k>_lk
         elif t == "number":
            cols.append(f"\"{k}\" REAL")
         elif t == "bool":
            cols.append(f"\"{k}\" INTEGER")
         else:
            cols.append(f"\"{k}\" TEXT")          # JSON text

      cur.execute(f"CREATE TABLE IF NOT EXISTS attributes({', '.join(cols)})")

      # prepare inserts: allocate feature ids if needed, insert lookup values, then insert attributes row
      rows = []
      for feat in features:
         fid = feat.get("id")
         if not isinstance(fid, int) or fid == 0:
            cur.execute("BEGIN IMMEDIATE")
            cur.execute("SELECT value FROM attr_seq WHERE name='feature_id'")
            v = cur.fetchone()[0]
            fid = int(v)
            cur.execute("UPDATE attr_seq SET value = ? WHERE name = 'feature_id'", (v + 1,))
            conn.commit()
         props = feat.get("properties", {}) or {}
         row = {"feature_id": int(fid)}
         for k, t in keys.items():
            v = props.get(k)
            if t == "text":
               if v is None:
                  row[k] = None
               else:
                  lk = f"attr_{k}_lk"
                  cur.execute(f"INSERT OR IGNORE INTO {lk}(value) VALUES(?)", (v,))
                  cur.execute(f"SELECT id FROM {lk} WHERE value = ?", (v,))
                  row[k] = cur.fetchone()[0]
            elif t == "number":
               row[k] = float(v) if v is not None else None
            elif t == "bool":
               row[k] = 1 if v else 0 if v is not None else None
            else:
               row[k] = json.dumps(v, separators=(",", ":")) if v is not None else None
         rows.append(row)

      # bulk upsert rows
      if rows:
         col_names = ["feature_id"] + [k for k in keys.keys()]
         placeholders = ",".join("?" for _ in col_names)
         insert_sql = f"INSERT OR REPLACE INTO attributes({', '.join('\"'+c+'\"' for c in col_names)}) VALUES({placeholders})"
         params = []
         for r in rows:
            params.append(tuple(r.get(c) for c in col_names))
         cur.executemany(insert_sql, params)

      conn.commit()
      conn.close()

   def get_attributes_for_feature_ids(self, ids: List[int]) -> Dict[int, Dict[str, Any]]:
      # returns mapping feature_id -> {attr: value} with text FKs resolved to strings
      if not ids:
         return {}
      self._ensure_attributes_db()
      path = self._attributes_db_path()
      conn = sqlite3.connect(path)
      cur = conn.cursor()

      q = ",".join("?" for _ in ids)
      cur.execute(f"PRAGMA table_info(attributes)")
      cols = [r[1] for r in cur.fetchall()]  # column names
      cur.execute(f"SELECT {', '.join(cols)} FROM attributes WHERE feature_id IN ({q})", tuple(ids))
      rows = cur.fetchall()
      out = {}
      # find text FK columns by checking for corresponding lookup table existence
      text_cols = []
      for c in cols:
         lk = f"attr_{c}_lk"
         cur.execute("SELECT name FROM sqlite_master WHERE type='table' AND name = ?", (lk,))
         if cur.fetchone():
            text_cols.append(c)

      for r in rows:
         rec = {}
         for ci, c in enumerate(cols):
            if c == "feature_id":
               fid = int(r[ci])
               continue
            val = r[ci]
            if val is None:
               rec[c] = None
            elif c in text_cols:
               lk = f"attr_{c}_lk"
               cur.execute(f"SELECT value FROM {lk} WHERE id = ?", (int(val),))
               rec[c] = cur.fetchone()[0]
            else:
               # attempt to parse JSON for json columns, otherwise return numeric/bool as-is
               try:
                  rec[c] = json.loads(val) if isinstance(val, str) else val
               except Exception:
                  rec[c] = val
         out[int(fid)] = rec
      conn.close()
      return out

   def delete_feature_attributes(self, feature_id: int) -> None:
      # Remove a feature's attributes (collection-level). Also cleanup orphaned lookup values.
      path = self._attributes_db_path()
      self._ensure_attributes_db()
      conn = sqlite3.connect(path)
      cur = conn.cursor()
      # remove the attributes row for the feature
      cur.execute("BEGIN IMMEDIATE")
      cur.execute("DELETE FROM attributes WHERE feature_id = ?", (int(feature_id),))
      # find all attr_*_lk tables and remove any lookup rows no longer referenced
      cur.execute("SELECT name FROM sqlite_master WHERE type='table' AND name LIKE 'attr\\_%\\_lk' ESCAPE '\\'")
      lk_tables = [r[0] for r in cur.fetchall()]
      for lk in lk_tables:
         # delete lookup rows not referenced by any attributes row
         # derive attribute column name from lookup table name: attr_<col>_lk -> "<col>"
         col = lk[len("attr_"):-len("_lk")]
         # only attempt cleanup if the attributes table has that column
         cur.execute("PRAGMA table_info(attributes)")
         cols = [r[1] for r in cur.fetchall()]
         if col in cols:
            cur.execute(
               f"DELETE FROM {lk} WHERE id NOT IN (SELECT DISTINCT \"{col}\" FROM attributes WHERE \"{col}\" IS NOT NULL)"
            )
      conn.commit()
      conn.close()

def get_store(data_root: str, collection: str, config: Optional[dict] = None) -> DGGSDataStore:
   key = f"{data_root}::{collection}"
   with store_lock:
      st = store_cache.get(key)
      if st is None:
         st = DGGSDataStore(data_root, collection, config)
         if getattr(st, "group0Size", None) is None:
            st = None
         store_cache[key] = st
      return st

def close_all_stores() -> None:
   with store_lock:
      store_cache.clear()
   with dggrs_lock:
      dggrs_cache.clear()

# Build a canonical DGGS-JSON envelope from a field-organized map.
# - store: DGGSDataStore (provides dggrs and config)
# - zone: DGGRSZone (zone object or int accepted by store.dggrs helpers)
# - fields_map: Dict[fieldName, List[ValueEntry]] where each ValueEntry already
#   contains "depth", "shape", and "data" and follows the contract.
def make_dggs_json_blob(dggrs_uri: str, zone_text: str, fields_map: Dict[str, List[Dict[str, Any]]]) -> Dict[str, Any]:
   depth_nums: List[int] = []
   for fld_depths in fields_map.values():
      for e in fld_depths:
         dn = int(e["depth"])
         if dn not in depth_nums:
            depth_nums.append(dn)

   envelope: Dict[str, Any] = {
      "dggrs": dggrs_uri,
      "zoneId": zone_text,
      "depths": depth_nums,
      "values": { fname: entries for fname, entries in fields_map.items() },
      "$schema": DGGS_JSON_SCHEMA_URI
   }
   # print("Value: ", envelope["values"]["field2"][0]["data"][0])
   return envelope

def make_dggs_json_depth(depth: int, count_centroids: int, sampled_values: Any) -> Dict[str, Any]:
   return {
      "depth": depth,
      "shape": {"count": count_centroids, "subZones": count_centroids},
      "data": sampled_values
   }

# Merge multiple per-depth ValuesObjects into a single field-organized envelope.
# - collected_values: Dict[depth:int, ValuesObject] where ValuesObject is
#   Dict[fieldName, List[ValueEntry]]
# This function delegates to build_zone_blob_from_fields for the final envelope.
def build_dggs_json_from_values(store, zone, collected_values: Dict[int, Dict[str, List[Dict[str, Any]]]]) -> Dict[str, Any]:
   # Merge per-depth ValuesObjects into a single fields_map
   merged_fields: Dict[str, List[Dict[str, Any]]] = {}
   # iterate depths in canonical ascending order
   for d in sorted(int(dk) for dk in collected_values.keys()):
      values_obj = collected_values[d]
      for field, entries in values_obj.items():
         # append entries in the order provided by each depth
         merged_fields.setdefault(field, []).extend(entries)

   dggrs_id = store.config["dggrs"]
   dggrs_uri = f"[ogc-dggrs:{dggrs_id}]"
   zone_text = store.dggrs.getZoneTextID(zone)
   return make_dggs_json_blob(dggrs_uri, zone_text, merged_fields)
