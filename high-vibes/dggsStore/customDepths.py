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

def _paint_from_stored_root_multi(store, target_root_zone : DGGRSZone, target_depth: int,
   stored_roots, stored_depth, fields) -> Dict[str, List[Optional[float]]]:

   # For each requested field, find the depth entry at store_depth and paint
   dggrs = store.dggrs

   fields_src_data = [None] * len(fields)
   target_maps = [None] * len(fields)

   subs = dggrs.getSubZones(target_root_zone, target_depth)
   subs_ptr = ffi.cast("uint64_t *", subs.array)
   subs_len = subs.count
   sub_index = { int(subs_ptr[i]): i for i in range(subs_len) }

   fieldIX = 0
   target_map = { }
   for fname in fields:
      tgt = [None] * subs_len
      target_map[fname] = tgt
      tgt = target_map[fname]
      target_maps[fieldIX] = tgt
      fieldIX = fieldIX + 1

   dggrs = store.dggrs
   dggrs_impl = dggrs.impl
   getSZ = dggal.lib.DGGRS_getSubZones
   array_offset = dggal.lib.class_Array.offset

   any_data = False

   for stored_root in stored_roots:
      # Decode the stored_root package once and update target_map for all fields.
      # - target_map: Dict[field, List[Optional[float]]] (preallocated length = n)
      # - fields: list of requested field names
      pkg = store.compute_package_path_for_root_zone(stored_root)
      decoded = store.read_and_decode_zone_blob(pkg, stored_root) if pkg else None
      if not decoded:
         continue

      source_subs = dggrs.getSubZones(stored_root, stored_depth)
      source_ptr = ffi.cast("uint64_t *", source_subs.array)
      src_index = { int(source_ptr[i]): i for i in range(len(source_subs)) }
      # relative depth between source_subs and target subs
      rel_depth = dggrs.getZoneLevel(source_ptr[0]) - dggrs.getZoneLevel(subs_ptr[0])
      Instance.delete(source_subs)

      fieldIX = 0
      for fname in fields:
         fData = None
         entries = decoded["values"].get(fname)
         if entries:
            chosen = next((e for e in entries if int(e["depth"]) == stored_depth), None)
            if chosen:
               fData = chosen["data"]
         fields_src_data[fieldIX] = fData
         fieldIX = fieldIX + 1

      # paint: first non-None contributor wins
      for tIX in range(subs_len):
         t = subs_ptr[tIX]
         t_idx = sub_index.get(int(t))
         if t_idx is None:
            continue

         #src_zones = dggrs.getSubZones(t, rel_depth)
         #src_zones_ptr = ffi.cast("uint64_t *", src_zones.array)
         #src_zones_len = len(src_zones)

         # Skip Python bindings for better performance
         src_zones = getSZ(dggrs_impl, t, rel_depth)
         a = ffi.cast("struct class_members_Array *", ffi.cast("char *", src_zones) + array_offset)
         src_zones_ptr = ffi.cast("uint64_t *", a.array)
         src_zones_len = a.count

         fieldIX = 0
         for fname in fields:
            tgt = target_maps[fieldIX]
            src_data = fields_src_data[fieldIX]
            fieldIX = fieldIX + 1
            if src_data is None:
               continue
            if tgt[t_idx] is not None:
               continue

            for sIX in range(src_zones_len):
               idx = src_index.get(src_zones_ptr[sIX])
               if idx is None:
                  continue
               val = src_data[idx]
               if val is not None:
                  tgt[t_idx] = val
                  any_data = True
                  break
         # Instance.delete(src_zones)
         lib.Instance_delete(src_zones)
   Instance.delete(subs)

   if any_data:
      values_obj = { }
      for fname, arr in target_map.items():
         values_obj[fname] = [ make_dggs_json_depth(target_depth, subs_len, arr) ]
   else:
      values_obj = None

   return values_obj

# ValueEntry and ValuesObject assumed:
# ValueEntry = {"depth": int, "shape": {"count": int, "subZones": int, ...}, "data": Sequence[Optional[float]]}
# ValuesObject = Dict[str, List[ValueEntry]]

def _assemble_from_descendants(store, root_zone, zone_depth, source_root_level, fields: List[str] | None = None):
   dggrs = store.dggrs
   if fields is None:
      fields = store.fields

   root_level = dggrs.getZoneLevel(root_zone)
   rel_depth = source_root_level - root_level
   descendants = dggrs.getSubZones(root_zone, rel_depth)

   values_obj = _paint_from_stored_root_multi(store, root_zone, zone_depth, descendants, store.depth, fields)

   Instance.delete(descendants)

   return values_obj

def _assemble_from_ancestors(store, root_zone, zone_depth, source_root_level, fields: List[str] | None = None) -> Optional[Dict[str, List[Dict[str, Any]]]]:
   dggrs = store.dggrs
   if fields is None:
      fields = store.fields
   ancestors = collect_ancestors_at_level(dggrs, root_zone, source_root_level)
   if not ancestors:
      return None
   return _paint_from_stored_root_multi(store, root_zone, zone_depth, ancestors, store.depth, fields)

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
