# dggsStore.py
from dggal import *
import os
import json
import sqlite3
import gzip
import threading
import logging
from typing import Any, Dict, List, Optional, Tuple, Iterable, TypedDict, Mapping, Sequence
import ubjson

DGGS_JSON_SCHEMA_URI = "https://schemas.opengis.net/ogcapi/dggs/part1/1.0/openapi/schemas/dggrs-json/schema"

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

def get_or_create_dggrs(dggrsID: str) -> DGGRS:
   key = f"dggrs:{dggrsID}"
   with dggrs_lock:
      inst = dggrs_cache.get(key)
      if inst is not None:
         return inst
      cls = globals().get(dggrsID)
      if cls is None:
         print(f"DGGRS class not found: {dggrsID!r}")
         return None
      inst = cls()
      dggrs_cache[key] = inst
      return inst

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
      self._compute_property_key()

   def _compute_property_key(self):
      self.property_key: Optional[str] = None
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
                  values_map = decoded["values"]
                  self.property_key = next(iter(values_map.keys()))
      # print("Computed property key: ", self.property_key)

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
           parents = dggrs.getZoneParents(zone)
           if not parents:
               break
           zone = parents[0]
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

   def iter_bases_under_lvl0(self, lvl0: DGGRSZone, base_level: int, up_to: bool = False, use_visited: bool = False,
      in_extent_cb=None) -> Iterable[Tuple[DGGRSZone, List[DGGRSZone]]]:
      if base_level <= 0:
         return
      dggrs = self.dggrs
      stack: List[Tuple[DGGRSZone, List[DGGRSZone]]] = []
      visited = set() if use_visited else None

      # start from lvl0's children (iter_bases yields lvl0 itself when appropriate)
      children0 = dggrs.getZoneChildren(lvl0) or []
      for child in children0:
         stack.append((child, [lvl0]))

      while stack:
         zone, base_ancestors = stack.pop()

         if use_visited:
            zid = int(zone)
            if zid in visited:
               continue
            visited.add(zid)

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
            continue

         # otherwise descend normally
         children = dggrs.getZoneChildren(zone) or []
         for child in children:
            stack.append((child, base_ancestors))


   def iter_bases(self, base_level: int, up_to: bool = False, use_visited: bool = False,
      in_extent_cb=None) -> Iterable[Tuple[DGGRSZone, List[DGGRSZone]]]:
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
               use_visited=use_visited,
               in_extent_cb=in_extent_cb,
            ):
               yield (base_zone, base_ancestors)

   def iter_roots_for_base(self, base_zone: DGGRSZone, level: int, up_to: bool=False, use_visited: bool=False, in_extent_cb=None) -> Iterable[DGGRSZone]:
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
      children = dggrs.getZoneChildren(base_zone) or []
      for child in children:
         stack.append((child, base_level + 1))

      visited = set() if use_visited else None

      while stack:
         zone, ref = stack.pop()
         zid = int(zone)

         if use_visited:
            if zid in visited:
               continue
            visited.add(zid)

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
            children = dggrs.getZoneChildren(zone) or []
            for child in children:
               stack.append((child, ref + 1))

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
                        precompressed: bool = False) -> None:
      dggrs = self.dggrs
      # determine package path (use provided pkg_path or compute from base_zone)
      if pkg_path is None:
         pkg_path = self.compute_package_path_for_root_zone(base_zone, base_ancestor_list=base_ancestor_list)

      # prepare rows for sqlite: (root_zone_text, blob)
      rows: List[Tuple[str, bytes]] = []
      for zone_obj, data_obj in entries.items():
         zone_text = dggrs.getZoneTextID(zone_obj)
         blob = data_obj if precompressed else to_blob(data_obj)
         rows.append((zone_text, blob))

      pkg_dir = os.path.dirname(pkg_path)
      if pkg_dir:
         os.makedirs(pkg_dir, exist_ok=True)

      write_sqlite_two_col(pkg_path, rows)

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
