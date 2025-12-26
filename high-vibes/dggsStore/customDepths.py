#!/usr/bin/env python3
from dggal import *

from typing import Dict, List, Optional, Mapping, Sequence, TypedDict, Union
import logging

from .store import *
from .aggregateLevel0 import *

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

def _paint_from_stored_root(store, root_zone, depth, sub_index, data):
   pkg = store.compute_package_path_for_root_zone(root_zone)
   if not pkg:
      return
   decoded = store.read_and_decode_zone_blob(pkg, root_zone)
   if not decoded:
      return
   prop_key = store.property_key
   chosen = next((e for e in decoded["values"][prop_key] if int(e["depth"]) == depth), None)
   if chosen is None:
      return
   src_data = chosen["data"]
   dggrs = store.dggrs
   s_subs = dggrs.getSubZones(root_zone, depth)
   count_s_subs = dggrs.countSubZones(root_zone, depth)
   for idx in range(count_s_subs):
      t = sub_index.get(int(s_subs[idx]))
      if t is None:
         continue
      v = src_data[idx]
      if v is None:
         continue
      data[t] = float(v)

def _assemble_from_descendants(store, root_zone, zone_depth, source_root_level):
   dggrs = store.dggrs
   store_depth = store.depth
   n = dggrs.countSubZones(root_zone, zone_depth)
   subs = dggrs.getSubZones(root_zone, zone_depth)
   root_level = dggrs.getZoneLevel(root_zone)
   rel_depth = source_root_level - root_level
   descendants = dggrs.getSubZones(root_zone, rel_depth)
   sub_index = { int(subs[i]): i for i in range(n) }
   data = [None] * n
   count_descendants = len(descendants)
   for i in range(count_descendants):
      _paint_from_stored_root(store, descendants[i], store_depth, sub_index, data)
   return data, n

def _assemble_from_ancestors(store, root_zone, zone_depth, source_root_level):
   dggrs = store.dggrs
   store_depth = store.depth
   n = dggrs.countSubZones(root_zone, zone_depth)
   subs = dggrs.getSubZones(root_zone, zone_depth)
   sub_index = { int(subs[i]): i for i in range(n) }
   data = [None] * n
   root_level = dggrs.getZoneLevel(root_zone)
   ancestors = collect_ancestors_at_level(dggrs, root_zone, source_root_level)
   count_anc = len(ancestors)
   for a in range(count_anc):
      _paint_from_stored_root(store, ancestors[a], store_depth, sub_index, data)
   return data, n

# --- public API (at the end) ---
def assemble_zone_at_depth(store, root_zone, zone_depth):
   root_level = store.dggrs.getZoneLevel(root_zone)
   if root_level + zone_depth > store.maxRefinementLevel:
      return None

   source_root_level = root_level + zone_depth - store.depth
   if source_root_level == root_level:
      # this function is only intended for building unavailable depths")
      return None

   if source_root_level > root_level:
      # Assemble requested depth from descendant root zones deeper than requested zone
      data, n = _assemble_from_descendants(store, root_zone, zone_depth, source_root_level)
   elif source_root_level < 0: # TOOD: Add check whether level zero root zone contain overview lower depths when implemented
      # Assemble requested depth by aggregating data from level 0 zone's default depth
      data, n = assemble_aggregate_from_level0(store, root_zone, zone_depth)
   else:
      # Assemble requested depth from ancestral root zones coarser than requested zone
      data, n = _assemble_from_ancestors(store, root_zone, zone_depth, source_root_level)
   entry = {"depth": zone_depth, "shape": {"count": n, "subZones": n}, "data": data}
   return {store.property_key: [entry]}

def build_dggs_json_from_values(store: DGGSDataStore,
                                zone: DGGRSZone,
                                collected_values: CollectedValues) -> Dict[str, object]:
   zone_depths = sorted(int(d) for d in collected_values.keys())
   # Merge per-depth ValuesObjects into a single values map keyed by property name
   merged_values: Dict[str, List[Dict[str, object]]] = {}
   for d in zone_depths:
      values_obj = collected_values[d]
      for prop_key, entries in values_obj.items():
         if prop_key not in merged_values:
            merged_values[prop_key] = list(entries)
         else:
            merged_values[prop_key].extend(entries)
   dggrsId = store.config["dggrs"]
   return {
      "dggrs": f"[ogc-dggrs:{dggrsId}]",
      "zoneId": store.dggrs.getZoneTextID(zone),
      "depths": zone_depths,
      "values": merged_values,
      "$schema": DGGS_JSON_SCHEMA_URI
   }
