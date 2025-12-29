#!/usr/bin/env python3
from dggal import *

from typing import Dict, List, Optional, Mapping, Sequence, TypedDict, Union, Any
import logging

from .store import DGGSDataStore

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
   # Aggregate values for a root_zone at zone_depth by scanning stored level-0 ancestors.
   # Returns a ValuesObject: field -> [ValueEntry]
   dggrs = store.dggrs
   if fields is None:
      fields = store.fields

   store_depth = store.depth
   n = dggrs.countSubZones(root_zone, zone_depth)
   subs = dggrs.getSubZones(root_zone, zone_depth)
   sub_index = { int(subs[i]): i for i in range(n) }

   # per-field accumulators
   sums_map: Dict[str, List[float]] = {}
   counts_map: Dict[str, List[float]] = {}
   for fname in fields:
      sums_map[fname] = [0.0] * n
      counts_map[fname] = [0] * n

   # collect level-0 ancestors (roots at level 0 that cover this root_zone)
   ancestors = collect_ancestors_at_level(dggrs, root_zone, 0)
   for ancestor in ancestors:
      _process_stored_root_aggregate(store, ancestor, store_depth, sub_index, subs, sums_map, counts_map, fields)

   Instance.delete(subs)

   # compute averages (None when no contributors) per field
   results: Dict[str, List[Any]] = {}
   for fname in fields:
      sums = sums_map[fname]
      counts = counts_map[fname]
      out = [ (sums[i] / counts[i]) if counts[i] > 0 else None for i in range(n) ]
      results[fname] = out

   # wrap into ValuesObject: each field maps to a list of ValueEntry dicts
   values_obj: Dict[str, List[Dict[str, Any]]] = {}
   for fname, out in results.items():
      values_obj[fname] = [{
         "depth": zone_depth,
         "shape": { "count": n, "subZones": n },
         "data": out
      }]

   return values_obj

def aggregate_from_children(store, root_zone, zone_depth, fields: List[str]):
   dggrs = store.dggrs
   store_depth = int(store.depth)

   n = dggrs.countSubZones(root_zone, zone_depth)
   subs = dggrs.getSubZones(root_zone, zone_depth)
   sub_index = { int(subs[i]): i for i in range(n) }

   # accumulators per field for averaging
   sums_map = { fname: [0.0] * n for fname in fields }
   counts_map = { fname: [0] * n for fname in fields }

   children = dggrs.getZoneChildren(root_zone)
   for child_root in children:
      # this painter must update sums_map and counts_map for all fields in one decode
      _process_stored_root_aggregate(store, child_root, store_depth, sub_index, subs, sums_map, counts_map, fields)

   Instance.delete(subs)

   # compute averages and wrap into ValueEntry
   values_obj = {}
   any_data = False
   for fname in fields:
      sums = sums_map[fname]
      counts = counts_map[fname]
      out = [ (sums[i] / counts[i]) if counts[i] > 0 else None for i in range(n) ]
      if any(v is not None for v in out):
         any_data = True
      entry = {"depth": zone_depth, "shape": {"count": n, "subZones": n}, "data": out}
      values_obj[fname] = [entry]

   return values_obj if any_data else None

def _process_stored_root_aggregate(store, root_zone, depth, sub_index, subs,
   sums_map: Dict[str, List[float]],
   counts_map: Dict[str, List[float]],
   fields: List[str]):
   dggrs = store.dggrs

   pkg = store.compute_package_path_for_root_zone(root_zone)
   if not pkg:
      return

   decoded = store.read_and_decode_zone_blob(pkg, root_zone)
   if not decoded:
      return

   source_subs = dggrs.getSubZones(root_zone, depth)
   src_index = { int(source_subs[i]): i for i in range(len(source_subs)) }

   rel = dggrs.getZoneLevel(source_subs[0]) - dggrs.getZoneLevel(subs[0])

   for fname in fields:
      entries = decoded["values"].get(fname)
      if not entries:
         continue

      chosen = next((e for e in entries if int(e["depth"]) == depth), None)
      if not chosen:
         continue
      src_data = chosen["data"]

      sums = sums_map[fname]
      counts = counts_map[fname]

      for t in subs:
         t_idx = sub_index.get(int(t))
         if t_idx is None:
            continue

         src_zones = dggrs.getSubZones(t, rel)
         weights = dggrs.getSubZoneWeights(t, rel)

         for i, s in enumerate(src_zones):
            idx = src_index.get(int(s))
            if idx is None:
               continue
            val = src_data[idx]
            if val is None:
               continue

            weight = 1.0 if weights is None else float(weights[i])
            sums[t_idx] += float(val) * weight
            counts[t_idx] += weight

         Instance.delete(src_zones)
   Instance.delete(source_subs)
