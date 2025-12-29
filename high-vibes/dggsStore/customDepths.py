#!/usr/bin/env python3
from dggal import *

from typing import Dict, List, Optional, Mapping, Sequence, TypedDict, Union
import logging

from .store import *
from .aggregation import *

logger = logging.getLogger("dggs-serve.customDepths")

# --- utilities ---
def parse_zone_depths(param: str) -> "List[int]":
   if not param:
      return []
   parts = [p.strip() for p in param.split(",") if p.strip()]
   depths: List[int] = []
   for p in parts:
      if "-" in p:
         a, b = p.split("-", 1)
         a_i, b_i = int(a), int(b)
         depths.extend(range(a_i, b_i + 1))
      else:
         depths.append(int(p))
   # ensure unique ascending depths (canonical DGGS-JSON ordering)
   return sorted(set(depths))

# --- helpers ---

def _paint_from_stored_root_multi(store, stored_root, store_depth, sub_index, subs, target_map, fields):
   # Decode the stored_root package once and update target_map for all fields.
   # - target_map: Dict[field, List[Optional[float]]] (preallocated length = n)
   # - fields: list of requested field names
   dggrs = store.dggrs

   pkg = store.compute_package_path_for_root_zone(stored_root)
   if not pkg:
      return

   decoded = store.read_and_decode_zone_blob(pkg, stored_root)
   if not decoded:
      return

   source_subs = dggrs.getSubZones(stored_root, store_depth)
   if not source_subs:
      return
   src_index = { int(source_subs[i]): i for i in range(len(source_subs)) }

   # relative depth between source_subs and target subs
   rel = dggrs.getZoneLevel(source_subs[0]) - dggrs.getZoneLevel(subs[0])

   Instance.delete(source_subs)

   # For each requested field, find the depth entry at store_depth and paint
   for fname in fields:
      entries = decoded["values"].get(fname)
      if not entries:
         continue
      chosen = next(e for e in entries if int(e["depth"]) == store_depth)
      src_data = chosen["data"]
      tgt = target_map[fname]

      # paint: first non-None contributor wins
      for t in subs:
         t_idx = sub_index.get(int(t))
         if t_idx is None:
            continue
         if tgt[t_idx] is not None:
            continue
         src_zones = dggrs.getSubZones(t, rel)
         for s in src_zones:
            idx = src_index.get(int(s))
            if idx is None:
               continue
            val = src_data[idx]
            if val is not None:
               tgt[t_idx] = float(val)
               break
         Instance.delete(src_zones)

def _assemble_from_descendants(store, root_zone, zone_depth, source_root_level, fields: List[str] | None = None):
   dggrs = store.dggrs
   if fields is None:
      fields = store.fields

   store_depth = int(store.depth)
   n = dggrs.countSubZones(root_zone, zone_depth)
   if n == 0:
      return None
   subs = dggrs.getSubZones(root_zone, zone_depth)
   root_level = dggrs.getZoneLevel(root_zone)

   rel_depth = source_root_level - root_level
   descendants = dggrs.getSubZones(root_zone, rel_depth)

   sub_index = { int(subs[i]): i for i in range(n) }
   target_map = { fname: [None] * n for fname in fields }
   for desc in descendants:
      _paint_from_stored_root_multi(store, desc, store_depth, sub_index, subs, target_map, fields)

   Instance.delete(subs)
   Instance.delete(descendants)

   values_obj = {}
   any_data = False
   for fname, arr in target_map.items():
      if any(v is not None for v in arr):
         any_data = True
      entry = {"depth": zone_depth, "shape": {"count": n, "subZones": n}, "data": arr}
      values_obj[fname] = [entry]

   return values_obj if any_data else None

# ValueEntry and ValuesObject assumed:
# ValueEntry = {"depth": int, "shape": {"count": int, "subZones": int, ...}, "data": Sequence[Optional[float]]}
# ValuesObject = Dict[str, List[ValueEntry]]

def _assemble_from_ancestors(store, root_zone, zone_depth, source_root_level, fields: List[str] | None = None) -> Optional[Dict[str, List[Dict[str, Any]]]]:
   dggrs = store.dggrs
   if fields is None:
      fields = store.fields

   ancestors = collect_ancestors_at_level(dggrs, root_zone, source_root_level)
   if not ancestors:
      return None

   n = dggrs.countSubZones(root_zone, zone_depth)

   subs = dggrs.getSubZones(root_zone, zone_depth)
   sub_index = { int(subs[i]): i for i in range(n) }

   # per-field arrays (initialized to None)
   target_map: Dict[str, List[Optional[float]]] = { fname: [None] * n for fname in fields }

   store_depth = int(store.depth)

   for ancestor in ancestors:
      pkg = store.compute_package_path_for_root_zone(ancestor)
      if not pkg:
         continue
      decoded = store.read_and_decode_zone_blob(pkg, ancestor)
      if not decoded:
         continue

      source_subs = dggrs.getSubZones(ancestor, store_depth)
      src_index = { int(source_subs[i]): i for i in range(len(source_subs)) }
      rel_depth = dggrs.getZoneLevel(source_subs[0]) - dggrs.getZoneLevel(subs[0])

      # decode-once: extract all requested fields from this decoded blob
      for fname in fields:
         entries = decoded["values"].get(fname)
         if not entries:
            continue
         chosen = next((e for e in entries if int(e["depth"]) == store_depth), None)
         if not chosen:
            continue
         src_data = chosen["data"]
         tgt = target_map[fname]

         # paint: first non-None contributor wins
         for t in subs:
            t_idx = sub_index.get(int(t))
            if t_idx is None:
               continue
            if tgt[t_idx] is not None:
               continue
            src_zones = dggrs.getSubZones(t, rel_depth)
            for s in src_zones:
               idx = src_index.get(int(s))
               if idx is None:
                  continue
               val = src_data[idx]
               if val is not None:
                  tgt[t_idx] = float(val)
                  break
            Instance.delete(src_zones)
      Instance.delete(source_subs)
   Instance.delete(subs)

   # build ValuesObject (one ValueEntry per field)
   values_obj: Dict[str, List[Dict[str, Any]]] = {}
   any_data = False
   for fname, arr in target_map.items():
      if any(v is not None for v in arr):
         any_data = True
      entry = {"depth": zone_depth, "shape": {"count": n, "subZones": n}, "data": arr}
      values_obj[fname] = [entry]

   return values_obj if any_data else None


def assemble_zone_at_depth(store, root_zone, zone_depth, fields: List[str] | None = None) -> Optional[Dict[str, List[Dict[str, Any]]]]:
   dggrs = store.dggrs
   if fields is None:
      fields = store.fields

   root_level = dggrs.getZoneLevel(root_zone)
   if root_level + zone_depth > store.maxRefinementLevel:
      return None

   source_root_level = root_level + zone_depth - store.depth
   if source_root_level == root_level:
      return None
   elif source_root_level < 0:
      return assemble_aggregate_from_level0(store, root_zone, zone_depth, fields)
   elif source_root_level > root_level:
      return _assemble_from_descendants(store, root_zone, zone_depth, source_root_level, fields)
   else:
      return _assemble_from_ancestors(store, root_zone, zone_depth, source_root_level, fields)

def aggregate_zone_at_depth(store, root_zone, zone_depth, fields: List[str] | None = None) -> Optional[Dict[str, List[Dict[str, Any]]]]:
   if fields is None:
      fields = store.fields

   t = store.dggrs.getZoneTextID(root_zone)
   # print("Aggregating for zone", t, "at depth", zone_depth)
   r = aggregate_from_children(store, root_zone, zone_depth, fields)
   # print("... done aggregating for zone", t, "at depth", zone_depth)
   return r
