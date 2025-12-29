#!/usr/bin/env python3
from dggal import *

from typing import Dict, List, Optional, Mapping, Sequence, TypedDict, Union, Any
import logging

ffi = dggal.ffi

from .store import *

logger = logging.getLogger("dggs-serve.aggregation")

def collect_ancestors_at_level(dggrs, start_zone, target_level):
   out = []
   stack = [start_zone]
   seen = set()
   while stack:
      n = stack.pop()
      nid = int(n)
      if nid in seen:
         continue
      seen.add(nid)
      lvl = int(dggrs.getZoneLevel(n))
      if lvl == target_level:
         out.append(n)
         continue
      if lvl <= target_level:
         continue
      parents = dggrs.getZoneParents(n)
      for p in parents:
         stack.append(p)
      if parents is not None:
         Instance.delete(parents)
   return out

def assemble_aggregate_from_level0(store, root_zone, zone_depth, fields: List[str] | None = None) -> Optional[Dict[str, List[Dict[str, Any]]]]:
   # Returns a ValuesObject: field -> [ValueEntry]
   # collect level-0 ancestors (roots at level 0 that cover this root_zone)
   # paint their values (at store's depth) onto the target root zone at target depth
   # (assumed to be of a refinement level coarser than the level0 source store depth values)
   ancestors = collect_ancestors_at_level(store.dggrs, root_zone, 0)
   return _aggregate_stored_roots(store, root_zone, zone_depth, ancestors, store.depth, fields)

def aggregate_from_children(store, root_zone, zone_depth, fields: List[str]):
   # Returns a ValuesObject: field -> [ValueEntry]
   # collect immediate children of target root zone
   # paint their values (at store's depth) onto the target root zone
   # (this is currently only used with zone_depth == store.depth)
   children = store.dggrs.getZoneChildren(root_zone)
   values = _aggregate_stored_roots(store, root_zone, zone_depth, children, store.depth, fields)
   if not isinstance(children, list):
      Instance.delete(children)
   return values

def _aggregate_stored_roots(store: DGGSDataStore,
   target_root_zone: DGGRSZone, target_depth: int,
   root_zones, source_depth, fields: List[str], average: bool = True ):

   dggrs = store.dggrs

   subs_count = dggrs.countSubZones(target_root_zone, target_depth)

   # per-field accumulators
   sums_map = { fname: [0.0] * subs_count for fname in fields }
   counts_map = { fname: [0] * subs_count for fname in fields }

   subs = dggrs.getSubZones(target_root_zone, target_depth)
   subs_ptr = ffi.cast("uint64_t *", subs.array)
   sub_index = { int(subs_ptr[i]): i for i in range(subs_count) }

   gzc = dggal.lib.DGGRS_getZoneChildren
   dggrs_impl = dggrs.impl

   if fields is None:
      fields = store.fields

   fields_src_data = [None] * len(fields)

   cBuf = None
   for root_zone in root_zones:
      pkg = store.compute_package_path_for_root_zone(root_zone)
      decoded = None if not pkg else store.read_and_decode_zone_blob(pkg, root_zone)
      if not decoded:
         #print("WARNING: Could not decode blob for zone ", dggrs.getZoneTextID(root_zone))
         continue
      source_subs = dggrs.getSubZones(root_zone, source_depth)
      source_ptr = ffi.cast("uint64_t *", source_subs.array)
      src_index = { int(source_ptr[i]): i for i in range(len(source_subs)) }
      Instance.delete(source_subs)

      rel_depth = dggrs.getZoneLevel(source_ptr[0]) - dggrs.getZoneLevel(subs_ptr[0])

      fieldIX = 0
      for fname in fields:
         fData = None
         entries = decoded["values"].get(fname)
         if entries:
            chosen = next((e for e in entries if int(e["depth"]) == source_depth), None)
            if chosen:
               fData = chosen["data"]
         fields_src_data[fieldIX] = fData
         fieldIX = fieldIX + 1

      if rel_depth == 1 and not cBuf:
         cBuf = ffi.new("uint64_t[13]")
      for si in range(len(subs)):
         t = subs_ptr[si]
         t_idx = sub_index.get(int(t))
         if t_idx is None:
            continue

         if rel_depth == 1:
            n_z = gzc(dggrs_impl, t, cBuf)
            weights = dggrs.getChildrenWeights(t)
            zptr = cBuf
         else:
            src_zones = dggrs.getSubZones(t, rel_depth)
            weights = dggrs.getSubZoneWeights(t, rel_depth)
            n_z = len(src_zones)
            zptr = ffi.cast("uint64_t *", src_zones.array)

         fieldIX = 0
         for fname in fields:
            src_data = fields_src_data[fieldIX]
            fieldIX = fieldIX + 1
            if src_data is None:
               continue

            sums = sums_map[fname]
            counts = counts_map[fname]

            sum = sums[t_idx]
            count = counts[t_idx]

            if weights is None:
               for i in range(n_z):
                  idx = src_index.get(zptr[i])
                  if idx is None:
                     continue
                  val = src_data[idx]
                  if val is None:
                     continue
                  sum += val
                  count += 1.0
            else:
               for i in range(n_z):
                  idx = src_index.get(zptr[i])

                  if idx is None:
                     continue

                  val = src_data[idx]

                  if val is None:
                     continue
                  weight = weights[i]
                  sum += val * weight
                  count += weight

            sums[t_idx] = sum
            counts[t_idx] = count

         if rel_depth != 1:
            Instance.delete(src_zones)

   Instance.delete(subs)

   # compute averages (None when no contributors) per field
   # wrap into ValuesObject: each field maps to a list of ValueEntry dicts
   values_obj: Dict[str, List[Dict[str, Any]]] = {}
   for fname in fields:
      sums = sums_map[fname]
      counts = counts_map[fname]
      if average == True:
         out = [ (sums[i] / counts[i]) if counts[i] > 0 else None for i in range(subs_count) ]
      else:
         out = [ sums[i] if counts[i] > 0 else None for i in range(subs_count) ]
      # REVIEW: Do we want to return None if fully empty or not? Parameter option?
      values_obj[fname] = [ make_dggs_json_depth(target_depth, subs_count, out) ]

   return values_obj
