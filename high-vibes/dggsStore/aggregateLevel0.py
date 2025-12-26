#!/usr/bin/env python3
from dggal import *

from typing import Dict, List, Optional, Mapping, Sequence, TypedDict, Union
import logging

from .store import DGGSDataStore

logger = logging.getLogger("dggs-serve.aggregateLevel0")

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
   return out

def _process_stored_root_aggregate(store, root_zone, depth, sub_index, subs, sums, counts):
   dggrs = store.dggrs
   prop_key = store.property_key
   pkg = store.compute_package_path_for_root_zone(root_zone)
   if not pkg:
      return

   source_subs = dggrs.getSubZones(root_zone, depth)
   source_counts = dggrs.countSubZones(root_zone, depth)
   src_index = { int(source_subs[i]): i for i in range(source_counts) }
   rel_depth = dggrs.getZoneLevel(source_subs[0]) - dggrs.getZoneLevel(subs[0])

   decoded = store.read_and_decode_zone_blob(pkg, root_zone)
   if not decoded:
      return

   chosen = next((e for e in decoded["values"][prop_key] if int(e["depth"]) == depth), None)
   if not chosen:
      return
   data = chosen["data"]

   for t in subs:
      src_zones = dggrs.getSubZones(t, rel_depth)
      t_idx = sub_index.get(int(t))
      for s in src_zones:
         idx = src_index[int(s)]
         if idx is not None:
            val = data[idx]
            if val is not None:
               fv = float(val)
               sums[t_idx] += fv
               counts[t_idx] += 1

def assemble_aggregate_from_level0(store, root_zone, zone_depth):
   dggrs = store.dggrs
   store_depth = store.depth
   n = dggrs.countSubZones(root_zone, zone_depth)
   subs = dggrs.getSubZones(root_zone, zone_depth)
   sub_index = { int(subs[i]): i for i in range(n) }
   sums = [0.0] * n
   counts = [0] * n
   ancestors = collect_ancestors_at_level(dggrs, root_zone, 0)
   for ancestor in ancestors:
      _process_stored_root_aggregate(store, ancestor, store_depth, sub_index, subs, sums, counts)

   return [ (sums[i] / counts[i]) if counts[i] > 0 else None for i in range(n) ], n
