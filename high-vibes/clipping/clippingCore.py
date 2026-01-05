#!/usr/bin/env python3
# clippingCore.py
# Core hand-crafted clipping logic
# - Exposes: get_zone_polygon, clip_ring_to_zone

from dggal import *
from typing import List, Dict, Any, Optional
from clipping.clipUtils import *

DEBUG_LEVEL = 2

def get_zone_polygon(dggrs, zone, refined=True, ico=False):
   #Return a list of [x,y] coordinates for the zone polygon in the requested CRS.
   #If ico is True, use the icosahedron-net CRS (OGC:1534) when requesting vertices.
   #Otherwise use CRS(0) as the native CRS.
   if ico:
      crs = CRS(ogc, 1534)
   else:
      crs = CRS(0)

   if refined:
      verts_container = dggrs.getZoneRefinedCRSVertices(zone, crs)
   else:
      verts_container = dggrs.getZoneCRSVertices(zone, crs)

   return [[float(v.x), float(v.y)] for v in verts_container]

def sort_key_for_hits(h: Dict[str, Any]):
   et = h.get("edge_t")
   etv = et if (et is not None) else 0.0
   return (h["t"], h["edge_index"], etv)

def crossing_is_between(a_edge_start: int, a_t_from_start: float,
                        b_edge_start: int, b_t_from_start: float,
                        test_edge_start: int, test_t_from_start: float,
                        n_edges: int) -> bool:

   a = a_edge_start + a_t_from_start
   b = b_edge_start + b_t_from_start
   t = test_edge_start + test_t_from_start

   #print("T = ", t, ", A = ", a, ", B = ", b)

   t = (t - a) % n_edges
   b = (b - a) % n_edges
   r = 0 <= t <= b
   #print("   returning ", r)
   return r

   #t = (t - a) % n_edges
   #b = (b - a) % n_edges
   #return 0 <= t <= b

   #if t > a and t < b: return True
   #if t > b and a > b: return True
   #return False

def vertex_index_is_between(a_e: int, a_t: float, b_e: int, b_t: float,
                            vertex_index: int, n_edges: int, cyclic = True) -> bool:

   if cyclic:
      r = crossing_is_between(a_e, a_t, b_e, b_t, vertex_index, 0, n_edges)
   else:
      a = a_e + a_t
      b = b_e + b_t - (n_edges if vertex_index == 0 else 0)
      r = vertex_index > a and vertex_index < b
   return r

def interval_contains(exit_e: int, exit_t: float,
                      end_e: int, end_t: float,
                      new_e: int, new_t: float,
                      n_edges: int,
                      tol_local: float = 1e-12) -> bool:
   return crossing_is_between(exit_e, exit_t, end_e, end_t, new_e, new_t, n_edges)

def collect_segment_hits(zone, nEdges, A, B):
   seg_hits = []
   for ei in range(nEdges):
      C = zone[ei]
      D = zone[(ei + 1) % nEdges]
      subhits = segment_segment_intersections(C, D, A, B)
      for h in subhits:
         t = h["t"]
         hit = {
            "t": t,
            "edge_t": t,
            "point": h["point"],
            "edge_index": ei
         }
         seg_hits.append(hit)

   seg_hits.sort(key=sort_key_for_hits)
   return seg_hits

def create_new_ring(rings, entry_edge, entry_edge_t, entry_point):
   # Create and append a new ring record with a pending interval and return its index.
   new_ring = {
      "entry_edge": entry_edge,
      "entry_edge_t": entry_edge_t,
      "entry_point": entry_point,
      "vertices": [entry_point] if entry_point is not None else [],
      "intervals": [
         {
            "entry_edge": entry_edge,
            "entry_t": entry_edge_t,
            "exit_edge": None,
            "exit_t": None,
            "pending": True
         }
      ],
      "last_exit_point": None
   }
   rings.append(new_ring)
   return len(rings) - 1

def finalize_rings(rings, zone, nEdges):
   if len(rings):
      print("__Finalization pass__")

   for p_idx in range(len(rings)):
      ring_rec = rings[p_idx]
      intervals = ring_rec.get("intervals", [])
      if not intervals:
         continue

      last = intervals[-1]

      entry_edge = ring_rec.get("entry_edge")
      entry_point = ring_rec.get("entry_point")
      entry_edge_t = ring_rec.get("entry_edge_t")

      synthetic_hit = {"edge_index": entry_edge, "point": entry_point, "edge_t": entry_edge_t}

      ring_len_before = len(ring_rec.get("vertices", []))
      intervals_before = len(intervals)

      process_entry(rings, synthetic_hit, zone, nEdges, target_pidx=p_idx)

      ring_len_after = len(ring_rec.get("vertices", []))
      intervals_after = len(ring_rec.get("intervals", []))

      if ring_len_after == ring_len_before and intervals_after == intervals_before:
         vertices = ring_rec.setdefault("vertices", [])
         if not vertices or vertices[-1] != entry_point:
            vertices.append(entry_point)
         last["entry_edge"] = entry_edge
         last["entry_t"] = entry_edge_t
         last["pending"] = False

   # Return List[Ring] where each Ring is a List[List[float]]
   out_rings = []
   clip_info = []
   for ring_ix, ring_rec in enumerate(rings):
      ring = ring_rec.get("vertices", [])
      if not ring:
         continue
      # ensure ring is closed
      if ring[0] != ring[-1]:
         ring = list(ring)
         ring.append(ring[0])
      out_rings.append(ring)

      # build clip info for this ring from the accumulated metadata
      crossings = ring_rec.get("crossings", [])
      clipper_vertices_out_ix = ring_rec.get("clipper_vertices_out_ix", [])
      clip_info.append({
         "ring_ix": ring_ix,
         "crossings": crossings,
         "clipper_vertices_out_ix": clipper_vertices_out_ix
      })

   return (out_rings, clip_info)

def process_entry(rings, hit, zone, nEdges, target_pidx=None):
   entry_edge = hit["edge_index"]
   entry_point = hit["point"]
   entry_edge_t = hit["edge_t"]

   print("-> Entering zone from edge", entry_edge, "at", entry_point)

   found_pidx = None
   ring_rec = None
   exit_e = None; exit_t = None
   appended = []

   if target_pidx is not None:
      ring_rec = rings[target_pidx]
      intervals = ring_rec["intervals"]
      if intervals:
         last = intervals[-1]
         exit_e = last.get("exit_edge")
         exit_t = last.get("exit_t")
      found_pidx = target_pidx
   else:
      for p_idx in list(range(len(rings) - 1, -1, -1)):
         ring_rec = rings[p_idx]
         intervals = ring_rec["intervals"]
         for int_idx in range(len(intervals) - 1, -1, -1):
            interval = intervals[int_idx]
            exit_e = interval["exit_edge"]; exit_t = interval["exit_t"]

            if exit_e is None or exit_t is None:
               continue

            entry_e0 = interval["entry_edge"]; entry_t0 = interval["entry_t"]

            if DEBUG_LEVEL >= 2:
               print("Checking if ", entry_edge + entry_edge_t, " is between ",
                  exit_e + exit_t, " and ", entry_e0 + entry_t0)

            if interval_contains(exit_e, exit_t, entry_e0, entry_t0, entry_edge, entry_edge_t, nEdges, 0.0):
               if DEBUG_LEVEL >= 2:
                  print("Continuing an existing ring!")

               left_interval = {
                  "exit_edge": interval["exit_edge"], "exit_t": interval["exit_t"],
                  "entry_edge": entry_edge,           "entry_t": entry_edge_t,
                  "pending": False
               }
               right_interval = {
                  "exit_edge": None,                    "exit_t": None,
                  "entry_edge": interval["entry_edge"], "entry_t": interval["entry_t"],
                  "pending": True
               }
               intervals[int_idx:int_idx+1] = [left_interval, right_interval]
               found_pidx = p_idx
               break
         if found_pidx is not None:
            break
      if found_pidx is None:
         found_pidx = create_new_ring(rings, entry_edge, entry_edge_t, entry_point)
         ring_rec = rings[found_pidx]

   if found_pidx is not None:
      ring_rec.setdefault("crossings", [])
      ring_rec.setdefault("vertices", [])
      ring_rec.setdefault("clipper_vertices_out_ix", [])   # now list of dicts: {"out_ix":int, "source_edge_ix":int}
      ring_rec.setdefault("pending_entry", None)

      if exit_e is not None and exit_t is not None:
         for vi in range(nEdges):
            if DEBUG_LEVEL >= 2:
               print("Checking ", vi, "between", exit_e + exit_t, " to ", entry_edge + entry_edge_t)
            if vertex_index_is_between(exit_e, exit_t, entry_edge, entry_edge_t, vi, nEdges,
               cyclic = target_pidx is not None):
               if DEBUG_LEVEL >= 2:
                  print("...adding zone vertex (between edge", vi, "and", (vi + 1) % nEdges, ")")
               vpt = zone[vi]
               ix = len(ring_rec["vertices"])
               ring_rec["vertices"].append(vpt)
               # record both the output index and the source clipper edge index (t is implicitly 0)
               ring_rec["clipper_vertices_out_ix"].append({"out_ix": ix, "source_edge_ix": vi})
               appended.append(vpt)

      ix = len(ring_rec["vertices"])
      ring_rec["vertices"].append(entry_point)
      appended.append(entry_point)

      entry_token = {
         "out_vertex_ix": ix,
         "clipper_edge_ix": entry_edge,
         "clipper_edge_t": entry_edge_t
      }
      ring_rec["pending_entry"] = entry_token

   return (found_pidx, appended)

def process_exit(rings, hit, zone, nEdges):

   exit_edge = hit["edge_index"]
   exit_point = hit["point"]
   exit_edge_t = hit["edge_t"]

   print("<- Exiting zone from edge ", exit_edge, " at ", exit_point)

   idx = len(rings) - 1 if rings else create_new_ring(rings, None, None, None)
   ring_rec = rings[idx]
   interval = ring_rec["intervals"][-1]
   interval["exit_edge"] = exit_edge
   interval["exit_t"] = exit_edge_t
   interval["pending"] = False

   # ensure metadata containers exist
   ring_rec.setdefault("crossings", [])
   ring_rec.setdefault("vertices", [])
   ring_rec.setdefault("clipper_vertices_out_ix", [])
   ring_rec.setdefault("pending_entry", None)

   # append exit point and record its index
   ix = len(ring_rec["vertices"])
   ring_rec["vertices"].append(exit_point)
   ring_rec["last_exit_point"] = exit_point

   # pair with the single pending_entry token
   entry_token = ring_rec.get("pending_entry", None)
   ring_rec["pending_entry"] = None

   crossing_pair = {
      "entry": {
         "out_vertex_ix": entry_token["out_vertex_ix"] if entry_token else None,
         "clipper_edge_ix": entry_token["clipper_edge_ix"] if entry_token else None,
         "clipper_edge_t": entry_token["clipper_edge_t"] if entry_token else None
      },
      "exit": {
         "out_vertex_ix": ix,
         "clipper_edge_ix": exit_edge,
         "clipper_edge_t": exit_edge_t
      }
   }
   ring_rec["crossings"].append(crossing_pair)

   return (idx, [exit_point])

def clip_ring_to_zone(ring_coords, zone):
   nEdges = len(zone) if zone is not None else 0
   if nEdges == 0 or not ring_coords or len(ring_coords) < 2:
      return {"rings": [], "clip_info": [], "result": "empty"}

   n = len(ring_coords)

   # find first outside point (we want to start outside)
   first_out = None
   for j in range(0, n):
      if not point_in_ring(ring_coords[j], zone):
         first_out = j
         break

   # Special case 1: no outside point found -> entire ring is inside the zone (no intersection)
   if first_out is None:
      ring = list(ring_coords)
      if ring[0] != ring[-1]:
         ring.append(ring[0])

      clip_info = [{
         "ring_ix": 0,
         "crossings": [],
         "clipper_vertices_out_ix": []
      }]

      return {"rings": [ring], "clip_info": clip_info, "result": "unchanged"}

   # Special case 2: all zone vertices are inside the ring -> full clipped zone
   all_zone_inside = True
   for zv in zone:
      if not point_in_ring(zv, ring_coords):
         all_zone_inside = False
         break
   if all_zone_inside:
      full_zone = list(zone)
      if full_zone[0] != full_zone[-1]:
         full_zone.append(full_zone[0])

      # clipper_vertices_out_ix should contain indices 0..len(zone)-1
      clipper_vertices_out_ix = [{"out_ix": i, "source_edge_ix": i} for i in range(len(zone))]
      clip_info = [{
         "ring_ix": 0,
         "crossings": [],
         "clipper_vertices_out_ix": clipper_vertices_out_ix
      }]
      return {"rings": [full_zone], "clip_info": clip_info, "result": "full"}

   start = first_out
   state_inside = point_in_ring(ring_coords[start], zone)
   rings = []

   i = start
   while True:
      A = ring_coords[i]
      B = ring_coords[(i + 1) % n]
      seg_hits = collect_segment_hits(zone, nEdges, A, B)

      # process hits in traversal order (seg_hits assumed ordered by t)
      for hit in seg_hits:
         #print(hit)

         prev_state = state_inside
         state_inside = not state_inside
         if (not prev_state) and state_inside:
            process_entry(rings, hit, zone, nEdges)
         elif prev_state and (not state_inside):
            process_exit(rings, hit, zone, nEdges)

      new_state = point_in_ring(B, zone)
      if new_state != state_inside:
         print("MISMATCHED STATE!!!!")

      if state_inside:
         rings[-1]["vertices"].append(B)
      i = (i + 1) % n
      if i == start:
         break

   out_rings, clip_info = finalize_rings(rings, zone, nEdges)
   return {"rings": out_rings, "clip_info": clip_info, "result": "modified"}
