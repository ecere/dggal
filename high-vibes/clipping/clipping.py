#!/usr/bin/env python3
# clipping.py
# Thin wrapper that exposes clip_feature_to_zone and clip_featurecollection_to_zone
# while delegating core work to clippingCore.py.
#
# - uses clipping_log() for debug output.

from dggal import *
from typing import List, Dict, Optional
from bisect import bisect_left

from clipping.clippingCore import *

# Controls: None => all rings; int => only that polygon index for MultiPolygon
POLY_DEBUG_INDEX: Optional[int] = None
# If False -> only outer ring (ring 0) processed; True -> include inner rings
INCLUDE_INNER_RINGS: bool = True

def clipping_log(*args):
   if DEBUG_LEVEL >= 1:
      print(*args)

def attach_hole_ring(out_ring, r_clip_info, hole, h_clip_info, zone):
   # Attach `hole` into `out_ring` in-place using metadata.
   # Contracts:
   #   - r_clip_info["clipper_vertices_out_ix"] is a list of dicts {"out_ix":int, "source_edge_ix":int}
   #   - h_clip_info["clipper_vertices_out_ix"] is the same shape for the hole
   #   - r_clip_info["crossings"] and h_clip_info["crossings"] exist when appropriate
   #   - zone is the clipper polygon vertex list (len(zone) == n_edges)
   # Returns True on success.

   # pick test location from hole's first crossing exit (fallback to entry)
   h0 = h_clip_info["crossings"][0]
   test = h0.get("exit") or h0.get("entry")
   test_edge = test["clipper_edge_ix"]
   test_t = test["clipper_edge_t"]

   n_edges = len(zone)

   # find the outer crossing interval that contains the test (if any)
   target_cross = None
   for oc in r_clip_info.get("crossings", []):
      outer_entry = oc["entry"]
      outer_exit = oc["exit"]
      if crossing_is_between(outer_exit["clipper_edge_ix"], outer_exit["clipper_edge_t"],
                             outer_entry["clipper_edge_ix"], outer_entry["clipper_edge_t"],
                             test_edge, test_t, n_edges):
         target_cross = oc
         break

   # scalar position helper
   def scalar_pos(edge, t, start_edge, start_t, n):
      delta = (edge - start_edge) % n
      return delta + (t - start_t)

   # --- prepare unified candidate search parameters ---
   outer_clippers = r_clip_info.get("clipper_vertices_out_ix", [])

   if target_cross is not None:
      entry = target_cross["entry"]
      exit = target_cross["exit"]
      start_edge, start_t = entry["clipper_edge_ix"], entry["clipper_edge_t"]
      restrict_interval = (exit["clipper_edge_ix"], exit["clipper_edge_t"], start_edge, start_t)
      test_pos = scalar_pos(test_edge, test_t, start_edge, start_t, n_edges)
   else:
      # no crossings: if there are outer clippers use the first as base, else we'll append at end
      if outer_clippers:
         base = outer_clippers[0]
         start_edge, start_t = base["source_edge_ix"], 0.0
         restrict_interval = None
         test_pos = scalar_pos(test_edge, test_t, start_edge, start_t, n_edges)
      else:
         # nothing to search; insert at end
         insert_at = len(out_ring)
         restrict_interval = None
         test_pos = None

   # --- unified candidate search ---
   matched = None
   if test_pos is not None and outer_clippers:
      candidates = []
      if restrict_interval is not None:
         exit_edge, exit_t, entry_edge, entry_t = restrict_interval
         for rec in outer_clippers:
            src_edge = rec["source_edge_ix"]
            if crossing_is_between(exit_edge, exit_t, entry_edge, entry_t, src_edge, 0.0, n_edges):
               pos = scalar_pos(src_edge, 0.0, start_edge, start_t, n_edges)
               if pos < test_pos:
                  candidates.append((pos, rec))
      else:
         # whole-cycle search relative to start_edge/start_t
         for rec in outer_clippers:
            src_edge = rec["source_edge_ix"]
            pos = scalar_pos(src_edge, 0.0, start_edge, start_t, n_edges)
            if pos < test_pos:
               candidates.append((pos, rec))

      if candidates:
         candidates.sort(key=lambda x: x[0])
         matched = candidates[-1][1]

   # determine insert_at using matched or fallback
   if 'insert_at' not in locals():
      if matched is not None:
         insert_at = matched["out_ix"] + 1
      else:
         if target_cross is not None:
            insert_at = target_cross["entry"]["out_vertex_ix"] + 1
         else:
            insert_at = len(out_ring)

   # --- remove outer clipper records not present in the hole (keep only those in hole) ---
   hole_src_edges = {rec["source_edge_ix"] for rec in h_clip_info.get("clipper_vertices_out_ix", [])}
   outer_clippers = r_clip_info.get("clipper_vertices_out_ix", [])
   kept_outer = [rec for rec in outer_clippers if rec["source_edge_ix"] in hole_src_edges]
   removed_out_ixs = sorted({rec["out_ix"] for rec in outer_clippers if rec["source_edge_ix"] not in hole_src_edges})

   if removed_out_ixs:
      original_len = len(out_ring)
      from bisect import bisect_left
      def count_removed_before(idx):
         return bisect_left(removed_out_ixs, idx)

      rem_set = set(removed_out_ixs)
      new_vertices = [v for i, v in enumerate(out_ring) if i not in rem_set]
      out_ring[:] = new_vertices

      # adjust kept_outer out_ix by subtracting number of removed indices before them
      for rec in kept_outer:
         old_out = rec["out_ix"]
         rec["out_ix"] = old_out - count_removed_before(old_out)

      # adjust crossings: if a crossing's out_vertex_ix was removed, move it to the next surviving index
      def next_surviving(old_idx):
         if old_idx >= original_len:
            return len(new_vertices)
         cur = old_idx
         while cur in rem_set:
            cur += 1
            if cur >= original_len:
               return len(new_vertices)
         return cur - count_removed_before(cur)

      for cr in r_clip_info.get("crossings", []):
         cr["entry"]["out_vertex_ix"] = next_surviving(cr["entry"]["out_vertex_ix"])
         cr["exit"]["out_vertex_ix"]  = next_surviving(cr["exit"]["out_vertex_ix"])

      # remap insert_at relative to the new vertex array
      if insert_at >= original_len:
         insert_at = len(out_ring)
      else:
         if insert_at in rem_set:
            insert_at = next_surviving(insert_at)
         else:
            insert_at = insert_at - count_removed_before(insert_at)

      r_clip_info["clipper_vertices_out_ix"] = kept_outer
   else:
      r_clip_info["clipper_vertices_out_ix"] = kept_outer

   # --- prepare hole vertices (drop closing duplicate, drop hole clipper vertices by out_ix) ---
   hole_vertices = list(hole)
   if hole_vertices and hole_vertices[0] == hole_vertices[-1]:
      hole_vertices = hole_vertices[:-1]

   hole_remove_out_ixs = {rec["out_ix"] for rec in h_clip_info.get("clipper_vertices_out_ix", [])}
   filtered = [v for i, v in enumerate(hole_vertices) if i not in hole_remove_out_ixs]
   to_insert = filtered  # keep original order

   # --- insert and update indices by simple increments ---
   out_ring[insert_at:insert_at] = to_insert
   len_inserted = len(to_insert)

   if len_inserted:
      for rec in r_clip_info.get("clipper_vertices_out_ix", []):
         if rec["out_ix"] >= insert_at:
            rec["out_ix"] += len_inserted

      for cr in r_clip_info.get("crossings", []):
         if cr["entry"]["out_vertex_ix"] >= insert_at:
            cr["entry"]["out_vertex_ix"] += len_inserted
         if cr["exit"]["out_vertex_ix"] >= insert_at:
            cr["exit"]["out_vertex_ix"] += len_inserted

   # --- cleanup consecutive duplicates ---
   cleaned = []
   prev = None
   for v in out_ring:
      if prev is None or v != prev:
         cleaned.append(v)
      prev = v
   out_ring[:] = cleaned

   return True

def insert_whole_hole_ring(polygons, holeRing):
   for poly in polygons:
      out_ring = poly[0]
      # if rings_overlap(out_ring, holeRing):
      for p in holeRing:
         if point_in_ring(p, out_ring):
            # sprint("Inserting hole ring into polygon")
            poly.append(holeRing)
            break

def insert_clipped_hole_ring(polygons, outer_clip_info, holeRings, hole_clip_info, zone):
   for pix, poly in enumerate(polygons):
      out_ring = poly[0]
      for hix, hole in enumerate(holeRings):
         # print("Testing if we attach this one here")
         #if rings_overlap(out_ring, hole):
         for p in hole:
            if point_in_ring(p, out_ring):
               # print("YES WE DO?")
               attach_hole_ring(out_ring, outer_clip_info[pix], hole, hole_clip_info[hix], zone)
               break

def clip_polygon(rings, metadata, zone_vertices):

   if not rings: return None
   outer_ring = rings[0]

   clipped = clip_ring_to_zone(outer_ring, zone_vertices)
   if clipped is None: return None

   clip_info = clipped["clip_info"]

   clipped_polygons = []
   metadata["clip_infos"].append(clip_info)

   for ring in clipped.get("rings", []):
      clipped_polygons.append([ring])

   if INCLUDE_INNER_RINGS and len(clipped_polygons):
      for ri in range(1, len(rings)):
         ring = rings[ri]

         if DEBUG_LEVEL >= 2:
            clipping_log("   processing inner ring", ri)

         in_clipped = clip_ring_to_zone(ring, zone_vertices)
         in_result = in_clipped["result"]
         if DEBUG_LEVEL >= 2:
            print("Inner ring", ri, "result is", in_result)

         if in_result == "full":        # Entire polygon is removed by hole
            return [ ]
         elif in_result == "empty":     # Hole is completely outside the zone
            continue
         elif in_result == "unchanged": # Hole was not clipped in any way -- add to right polygon
            insert_whole_hole_ring(clipped_polygons, in_clipped["rings"][0])
         elif in_result == "modified":  # Hole was clipped, and therefore no longer a hole in clipped polygon
            hole_clip_info = in_clipped["clip_info"]
            insert_clipped_hole_ring(clipped_polygons, clip_info,
               in_clipped["rings"], hole_clip_info, zone_vertices)
   return clipped_polygons

def clip_feature_to_zone(feature: Dict, dggrs, zone, refined=True, feature_id=None, ico=False):
   if DEBUG_LEVEL >= 2:
      fID = feature.get('id') if feature_id is None else feature_id
      clipping_log(f"processing FEATURE id={fID}")

   zone_vertices = get_zone_polygon(dggrs, zone, refined=refined, ico=ico)
   orientation_sign = -1
   geom = feature["geometry"]
   t = geom["type"]
   metadata = {"clip_infos": []}

   if t == "Polygon" or t == "MultiPolygon":
      if t == "Polygon":
         if POLY_DEBUG_INDEX is None or POLY_DEBUG_INDEX == 0:
            clipped_polygons = clip_polygon(geom["coordinates"], metadata, zone_vertices)
      else:
         polys = geom["coordinates"]
         poly_indices = range(len(polys)) if POLY_DEBUG_INDEX is None else ([POLY_DEBUG_INDEX] if 0 <= POLY_DEBUG_INDEX < len(polys) else [])
         clipped_polygons = []
         for pi in poly_indices:
            #if DEBUG_LEVEL >= 2:
            #   print("START processing MultiPolygon poly", pi)
            poly = polys[pi]
            clipped_polygons.extend(clip_polygon(poly, metadata, zone_vertices))

      if clipped_polygons:
         if len(clipped_polygons) == 1:
            out_geom = {"type": "Polygon", "coordinates": clipped_polygons[0]}
         else:
            out_geom = {"type": "MultiPolygon", "coordinates": clipped_polygons}

         out_feature = {"type": "Feature", "id": feature.get("id"), "properties": feature.get("properties"), "geometry": out_geom}
         return out_feature, metadata
      return None, metadata

   if t == "LineString":
      coords = geom["coordinates"]
      clipped = clip_ring_to_zone(coords, zone_vertices, orientation_sign)
      metadata["crossings"].append(clipped["crossings"] if clipped is not None else [])
      lines = [run for run in clipped["polygons"] if run and len(run) >= 2]
      if not lines:
         return None, metadata
      out_geom = {"type": "LineString", "coordinates": lines[0]} if len(lines) == 1 else {"type": "MultiLineString", "coordinates": lines}
      out_feature = {"type": "Feature", "id": feature.get("id"), "properties": feature.get("properties"), "geometry": out_geom}
      return out_feature, metadata

   if t == "MultiLineString":
      lines = []
      for ls in geom["coordinates"]:
         clipped = clip_ring_to_zone(ls, zone_vertices, orientation_sign)
         metadata["crossings"].append(clipped["crossings"] if clipped is not None else [])
         for run in clipped["polygons"]:
            if run and len(run) >= 2:
               lines.append(run)
      if not lines:
         return None, metadata
      out_geom = {"type": "MultiLineString", "coordinates": lines}
      out_feature = {"type": "Feature", "id": feature.get("id"), "properties": feature.get("properties"), "geometry": out_geom}
      return out_feature, metadata

   if t == "Point":
      pt = feature["geometry"]["coordinates"]
      if point_in_ring(pt, zone_vertices):
         return {"type": "Feature", "id": feature.get("id"), "properties": feature.get("properties"), "geometry": {"type": "Point", "coordinates": [float(pt[0]), float(pt[1])] }}, metadata
      return None, metadata

   if t == "MultiPoint":
      pts = []
      for p in feature["geometry"]["coordinates"]:
         if point_in_ring(p, zone_vertices):
            pts.append([float(p[0]), float(p[1])])
      if not pts:
         return None, metadata
      out_geom = {"type": "MultiPoint", "coordinates": pts}
      out_feature = {"type": "Feature", "id": feature.get("id"), "properties": feature.get("properties"), "geometry": out_geom}
      return out_feature, metadata

   return None, metadata

def clip_featurecollection_to_zone(fc: Dict, dggrs, zone, refined=False, ico=False):
   out_features = []
   metadata = {}
   for feat in fc.get("features", []):
      feature_id = feat.get("id", None)
      #if feature_id != 68: continue  # Canada
      #if feature_id != 182: continue # Italy
      #if feature_id != 195: continue # Luxembourg
      #if feature_id != 68 and feature_id != 182: continue
      clipped, meta = clip_feature_to_zone(feat, dggrs, zone, refined=refined, feature_id=feature_id, ico=ico)
      metadata[feat.get("id")] = meta
      if clipped is not None:
         out_features.append(clipped)

   out_fc = {"type": "FeatureCollection", "features": out_features}
   return out_fc, metadata
