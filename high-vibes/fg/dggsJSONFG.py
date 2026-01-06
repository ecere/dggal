# dggsJSONFG.py
# Minimal encoder for profile=jsonfg-dggs

from dggal import *
import json
from typing import Dict, Any, List, Sequence, Set
from shapely.geometry import shape
from ogcapi.utils import *
from fg.distance import *
from fg.reproj import get_dggrs, instantiate_projection_for_dggrs_name
from fg.unprojectToWGS84 import *
from fg.fixWGS84 import *

dggal_ffi = dggal.ffi

def _resolve_point_to_subzone_index(shp, dggrs, root_zone, sz_level, sub_indices,
   centroid_pointd, nudge_factor = 1e-8) -> int:
   cx = centroid_pointd.x
   cy = centroid_pointd.y
   defaultCRS = CRS(0)
   px = shp.x; py = shp.y
   #dx = cx - px; dy = cy - py
   #if dx > 3:
   #   dx = dx - 5; dy = dy - 5
   #elif dx < -3:
   #   dx = dx + 5; dy = dy + 5
   #szCentroid = dggal.Pointd(px + dx * nudge_factor, py + dy * nudge_factor)

   d, *unused = distance5x6(Pointd(px, py), centroid_pointd)
   szCentroid = move5x6((px, py), sgn(d.x) * nudge_factor, sgn(d.y) * nudge_factor, 1)
   szCentroid = dggal.Pointd(szCentroid.x, szCentroid.y) # FIXME: utils vs. DGGAL Pointd

   sub_zone = dggrs.getZoneFromCRSCentroid(sz_level, defaultCRS, szCentroid)
   if nullZone == nullZone:
      # idx = dggrs.getSubZoneIndex(root_zone, sub_zone)
      # if idx != -1: idx = idx + 1
      idx = sub_indices.get(int(sub_zone), -2) + 1
   else:
      idx = -1
      print("WARNING: Failed to resolve sub-zone at", szCentroid.x, ",", szCentroid.y)

   if idx == -1:
      print("WARNING:", dggrs.getZoneTextID(sub_zone),
         "is not a sub-zone of", dggrs.getZoneTextID(root_zone), "; skipping vertex",
         px, ",", py)
   return idx

def _ring_to_dggs_indices(ring_coords: Sequence[Sequence[float]], insert_zero_indices: Set[int],
                          dggrs, root_zone, sz_level, sub_indices, centroid_pointd, nudge_factor = 1e-8) -> List[int]:
   out: List[int] = []
   print("Processing ring with ", len(ring_coords), "vertices")

   lastIX = None
   count = 0

   cx = centroid_pointd.x
   cy = centroid_pointd.y
   defaultCRS = CRS(0)
   for i, coord in enumerate(ring_coords):
      if i in insert_zero_indices:
         out.append(0)

      px = coord[0]; py = coord[1]

      #px = 2.9999999995527866
      #py = 2.500000000894427

      #dx = cx - px; dy = cy - py
      #if dx > 3:
      #   dx = dx - 5; dy = dy - 5
      #elif dx < -3:
      #   dx = dx + 5; dy = dy + 5
      #szCentroid = Pointd(px + dx * nudge_factor, py + dy * nudge_factor)

      d, *unused = distance5x6(Pointd(px, py), centroid_pointd)
      dx = d.x * nudge_factor; dy = d.y * nudge_factor
      szCentroid = move5x6((px, py), dx, dy, 1)
      szCentroid = dggal.Pointd(szCentroid.x, szCentroid.y) # FIXME: utils vs. DGGAL Pointd

      sub_zone = dggrs.getZoneFromCRSCentroid(sz_level, defaultCRS, szCentroid)
      if nullZone == nullZone:
         # idx = dggrs.getSubZoneIndex(root_zone, sub_zone)
         # if idx != -1: idx = idx + 1
         idx = sub_indices.get(int(sub_zone), -2) + 1
      else:
         idx = -1
         print("WARNING: Failed to resolve sub-zone at", szCentroid.x, ",", szCentroid.y)

      if idx == -1:
         print("WARNING:", dggrs.getZoneTextID(sub_zone),
            "is not a sub-zone of", dggrs.getZoneTextID(root_zone), "; skipping vertex",
            px, ",", py)
         print("x:", szCentroid.x, "y:", szCentroid.y,
            "cx:", cx, "cy:", cy, "dx:", dx, "dy:", dy,
            "adx:", szCentroid.x - px, "ady:", szCentroid.y - py)

         #idx = dggrs.getSubZoneIndex(root_zone, sub_zone)
         #print("idx =", idx)

         raise BadNudge
      elif lastIX != idx:
         out.append(int(idx))
         lastIX = idx
         count = count + 1
      else:
         pass # TODO: Avoid self-intersection if points get quantized to same sub-zone
      if i and i % 1000 == 0: print(i, "/", len(ring_coords))
   return out, count

def write_dggs_json_fg(out_fc: Dict[str, Any],
                       features_rings_entry_exit_indices: Dict[str, Any],
                       dggrs,
                       root_zone,
                       depth: int,
                       profile_uri: str = "https://www.opengis.net/def/profile/ogc/0/jsonfg-dggs"):
   # strict contract: dggrs.getZoneCRSCentroid(root_zone, CRS(0)) returns Pointd
   centroid_pointd = dggrs.getZoneCRSCentroid(root_zone, CRS(0))

   features = out_fc.get("features", []) or []
   print(f"write_dggs_json_fg: features_in={len(features)}, entry_exits_items={len(features_rings_entry_exit_indices)}")

   dggrs_name = dggrs.__class__.__name__
   dggs_obj: Dict[str, Any] = {
      "conformsTo": [
         "https://www.opengis.net/spec/json-fg-1/0.2/conf/core",
         "https://www.opengis.net/spec/ogcapi-dggs-1/1.0/conf/data-dggs-jsonfg"
      ],
      "links": [
         {"rel": "profile", "href": profile_uri}
      ],
      "dggrs": f"[ogc-dggrs:{dggrs_name}]",
      "zoneId": dggrs.getZoneTextID(root_zone),
      "depth": depth,
      "type": "FeatureCollection",
      "features": []
   }
   ref_ratio = dggrs.getRefinementRatio()
   root_level = dggrs.getZoneLevel(root_zone)
   sz_level = root_level + depth
   nudge_factor = 10.0 / (ref_ratio ** sz_level)
   print("Selecting nudge_factor =", nudge_factor,
      "for root zone of level", root_level, "at depth", depth)

   print("Precalculating all sub-zones...")
   sub_zones = dggrs.getSubZones(root_zone, depth)
   sub_count = sub_zones.count
   sub_ptr = ffi.cast("uint64_t *", sub_zones.array)
   print("Building sub-zone map...")
   sub_indices = { int(sub_ptr[i]): i for i in range(sub_count) }
   Instance.delete(sub_zones)
   print("Quantizing features...")

   for fi, feat in enumerate(features):
      fid = feat.get("id")
      print(f"processing feature {fi} id={fid}")

      props = feat.get("properties", {})
      geom = feat.get("geometry")
      # strict contract: features_rings_entry_exit_indices[fi] exists and has the correct shape for the geometry
      entry_exit_indices = features_rings_entry_exit_indices[fi]
      shp = shape(geom) if geom is not None else None

      dggs_place = None
      if shp is not None:
         if shp.geom_type == "Polygon":
            rings = []
            exterior = list(shp.exterior.coords)
            exterior_entry_exit_indices = entry_exit_indices[0]
            ring, count = _ring_to_dggs_indices(exterior, exterior_entry_exit_indices, dggrs, root_zone, sz_level, sub_indices, centroid_pointd, nudge_factor)
            if ring and count > 3: rings.append(ring)
            for ri, interior in enumerate(shp.interiors, start=1):
               coords = list(interior.coords)
               ring_entry_exit_indices = entry_exit_indices[ri]
               ring, count = _ring_to_dggs_indices(coords, ring_entry_exit_indices, dggrs, root_zone, sz_level, sub_indices, centroid_pointd, nudge_factor)
               if ring and count > 3: rings.append(ring)
            dggs_place = {"type": "Polygon", "coordinates": rings if rings else None}

         elif shp.geom_type == "MultiPolygon":
            mcoords = []
            # strict contract: entry_exit_indices is List[List[List[int]]] (polygons -> rings -> indices)
            for p_index, p in enumerate(shp.geoms):
               poly_rings = []
               polygon_entry_exit_lists = entry_exit_indices[p_index]
               ext = list(p.exterior.coords)
               ext_entry_exit_indices = polygon_entry_exit_lists[0]
               ring, count = _ring_to_dggs_indices(ext, ext_entry_exit_indices, dggrs, root_zone, sz_level, sub_indices, centroid_pointd, nudge_factor)
               if ring and count > 3: poly_rings.append(ring)
               for ri, interior in enumerate(p.interiors, start=1):
                  coords = list(interior.coords)
                  ring_entry_exit_indices = polygon_entry_exit_lists[ri]
                  ring, count = _ring_to_dggs_indices(coords, ring_entry_exit_indices, dggrs, root_zone, sz_level, sub_indices, centroid_pointd, nudge_factor)
                  if ring and count > 3: poly_rings.append(ring)
               if poly_rings:
                  mcoords.append(poly_rings)
            dggs_place = {"type": "MultiPolygon", "coordinates": mcoords if mcoords else None}

         elif shp.geom_type == "LineString":
            coords = list(shp.coords)
            ls, count = _ring_to_dggs_indices(coords, entry_exit_indices, dggrs, root_zone, sz_level, sub_indices, centroid_pointd, nudge_factor)
            dggs_place = {"type": "LineString", "coordinates": ring if ring and count >= 2 else None }

         elif shp.geom_type == "MultiLineString":
            lines_coords = []
            for li, line in enumerate(shp.geoms):
               coords = list(line.coords)
               line_entry_exit = entry_exit_indices[li]
               ls, count = _ring_to_dggs_indices(coords, set(line_entry_exit), dggrs, root_zone, sz_level, sub_indices, centroid_pointd, nudge_factor)
               if ls and count >= 2:
                  lines_coords.append(ls)
            dggs_place = {"type": "MultiLineString", "coordinates": lines_coords if lines_coords else None}

         elif shp.geom_type == "Point":
            idx = _resolve_point_to_subzone_index(shp, dggrs, root_zone, sz_level, sub_indices, centroid_pointd, nudge_factor)
            dggs_place = {"type": "Point", "coordinates": idx if idx != -1 else None }

         elif shp.geom_type == "MultiPoint":
            # MultiPoint -> resolve each point to a subzone index; preserve order
            pts_coords: List[Any] = []
            for p in shp.geoms:
               idx = _resolve_vertex_to_subzone_index(float(p.x), float(p.y), dggrs, root_zone, sz_level, sub_indices, centroid_pointd, nudge_factor)
               if idx != -1:
                  pts_coords.append(idx)
            dggs_place = {"type": "MultiPoint", "coordinates": pts_coords if pts_coords else None}

      out_feature: Dict[str, Any] = {
         "type": "Feature",
         "id": fid,
         "properties": props,
         "geometry": None,
         "place": None,
         "time": None,
         "dggsPlace": dggs_place
      }
      dggs_obj["features"].append(out_feature)
      print(f"processed feature {fi} id={fid}")

   print(f"write_dggs_json_fg: finished features={len(dggs_obj['features'])}")

   return dggs_obj

def write_dggs_json_fg_to_file(out_fc: Dict[str, Any],
                       features_rings_entry_exit_indices: Dict[str, Any],
                       output_path: str,
                       dggrs,
                       root_zone,
                       depth: int,
                       profile_uri: str = "https://www.opengis.net/def/profile/ogc/0/jsonfg-dggs"):

   dggs_obj = write_dggs_json_fg(out_fc, features_rings_entry_exit_indices, dggrs, root_zone,
      depth, profile_uri)

   if dggs_obj is not None:
      with open(output_path, "w", encoding="utf-8") as fh:
         fh.write(pretty_json(dggs_obj))
         fh.write("\n")

# map a single integer index to an [x,y] coordinate using centroids list
def _index_to_xy(idx: int, centroids: List[Pointd]) -> List[float]:
   if idx <= 0:
      return []
   p = centroids[idx - 1]

   # return [float(p.lon), float(p.lat)]
   return [float(p.x), float(p.y)]

# Recursively resolve coordinate arrays where the leaf elements are integer indices.
# Preserves nesting shape but replaces numeric leaves with [x,y] pairs.
def resolve_coordinates(coords: Any, centroids: List[Pointd]) -> Any:
   if coords is None:
      return []

   t = type(coords)
   if t is list:
      if coords:
         first = coords[0]
         ft = type(first)
         if ft is int or ft is float:
            out: List[List[float]] = []
            for v in coords:
               iv = int(v)
               xy = _index_to_xy(iv, centroids)
               if xy:
                  out.append(xy)

            return out
      out_list: List[Any] = []
      for item in coords:
         resolved = resolve_coordinates(item, centroids)
         out_list.append(resolved)
      return out_list
   iv = int(coords)
   xy = _index_to_xy(iv, centroids)
   if xy:
      return xy
   return []

# Convert a geometry object (GeoJSON geometry) whose coordinates are index-based
# into a geometry with numeric coordinates. Returns a new geometry dict.
def convert_geometry_indexed(geom: Dict[str, Any], centroids: List[Pointd], fid: str) -> Dict[str, Any]:
   gtype = geom["type"]

   if gtype == "Point":
      coords = geom.get("coordinates", [])
      resolved = resolve_coordinates(coords, centroids)
      if not resolved:
         print(f"Warning: Point geometry empty for feature id={fid}")
         return None
      return {"type": "Point", "coordinates": resolved}

   if gtype == "MultiPoint":
      coords = geom.get("coordinates", [])
      resolved = resolve_coordinates(coords, centroids)
      if not resolved:
         print(f"Warning: MultiPoint has 0 points for feature id={fid}")
         return None
      return {"type": "MultiPoint", "coordinates": resolved}

   if gtype == "LineString":
      coords = geom.get("coordinates", [])
      resolved = resolve_coordinates(coords, centroids)
      if not resolved or len(resolved) < 2:
         print(f"Warning: LineString too short (len={len(resolved) if resolved else 0}) for feature id={fid}")
         return None
      return {"type": "LineString", "coordinates": resolved}

   if gtype == "MultiLineString":
      coords = geom.get("coordinates", [])
      resolved = resolve_coordinates(coords, centroids)
      if not resolved:
         print(f"Warning: MultiLineString has 0 LineStrings for feature id={fid}")
         return None
      filtered = [line for line in resolved if line and len(line) >= 2]
      if not filtered:
         print(f"Warning: MultiLineString all LineStrings too short for feature id={fid}")
         return None
      return {"type": "MultiLineString", "coordinates": filtered}

   if gtype == "Polygon":
      coords = geom.get("coordinates", [])
      resolved = resolve_coordinates(coords, centroids)
      if not resolved or not resolved[0] or len(resolved[0]) <= 3:
         print(f"Warning: Polygon exterior ring too short (len={len(resolved[0]) if resolved and resolved[0] else 0}) for feature id={fid}")
         return None
      exterior = resolved[0]
      holes = [h for h in resolved[1:] if h and len(h) > 3]
      return {"type": "Polygon", "coordinates": [exterior] + holes}

   if gtype == "MultiPolygon":
      coords = geom.get("coordinates", [])
      resolved = resolve_coordinates(coords, centroids)
      if not resolved:
         print(f"Warning: MultiPolygon has 0 polygons for feature id={fid}")
         return None
      out_polys: List[List[Any]] = []
      for poly in resolved:
         if not poly or not poly[0] or len(poly[0]) <= 3:
            print(f"Warning: Skipping polygon with short exterior ring (len={len(poly[0]) if poly and poly[0] else 0}) in MultiPolygon for feature id={fid}")
            continue
         exterior = poly[0]
         holes = [h for h in poly[1:] if h and len(h) > 3]
         out_polys.append([exterior] + holes)
      if not out_polys:
         print(f"Warning: MultiPolygon has no valid polygons after filtering for feature id={fid}")
         return None
      return {"type": "MultiPolygon", "coordinates": out_polys}

   if gtype == "GeometryCollection":
      geoms = geom.get("geometries", [])
      out_geoms: List[Dict[str, Any]] = []
      for g in geoms:
         conv = convert_geometry_indexed(g, centroids, fid)
         if conv is None:
            inner_id = g.get("id") or "<no-id>"
            print(f"Warning: GeometryCollection contained empty/invalid geometry id={inner_id} in feature id={fid}")
            continue
         out_geoms.append(conv)
      if not out_geoms:
         print(f"Warning: GeometryCollection empty after conversion for feature id={fid}")
         return None
      return {"type": "GeometryCollection", "geometries": out_geoms}

   coords = geom.get("coordinates")
   if coords is None:
      print(f"Warning: Geometry of type {gtype} missing coordinates for feature id={fid}")
      return None
   return geom

def finalize_result(projection, extent, converted, fid):
   if converted:
      converted = unproject_geojson_to_wgs84(converted, projection, extent) #, False)

   if converted:
      dlon = extent[2] - extent[0]
      if dlon > 180: dlon = dlon - 360
      eps_zone_tile =  abs(dlon) / 20.0 / 100000.0
      converted = fix_WGS84_geometry(converted, extent, eps_zone_tile, fid)

   return converted

# read a DGGS-JSON-FG file and convert index-based geometries to numeric coordinates
# Returns the top-level object in the same shape it was read (geometry, Feature, or FeatureCollection)
def read_dggs_json_fg(path: str) -> Dict[str, Any]:
   with open(path, "r", encoding="utf-8") as fh:
      data = json.load(fh)

   curie = data["dggrs"]
   token = "-dggrs:"
   start = curie.find(token) + len(token)
   end = curie.find("]", start)
   dggrs_id =  curie[start:end]

   dggrs = get_dggrs(dggrs_id)

   projection = instantiate_projection_for_dggrs_name(dggrs_id)

   zone_text = data["zoneId"]
   depth = int(data["depth"])
   ge = GeoExtent()

   root_zone = dggrs.getZoneFromTextID(zone_text)
   dggrs.getZoneWGS84Extent(root_zone, ge)
   extent = [float(ge.ll.lon), float(ge.ll.lat), float(ge.ur.lon), float(ge.ur.lat)]

   # centroids: List[GeoPoint] = dggrs.getSubZoneWGS84Centroids(root_zone, depth)
   centroids: List[Pointd] = dggrs.getSubZoneCRSCentroids(root_zone, CRS(0), depth)

   print("Converting DGGS-JSON-FG for level", dggrs.getZoneLevel(root_zone),
      "root zone", zone_text, "of DGGRS", dggrs_id, "at sub-zone depth", depth)

   #print("Done calculating centroids.")

   top_type = data["type"]

   if top_type == "FeatureCollection":
      feats = data["features"]
      out_feats: List[Dict[str, Any]] = []
      for feat in feats:
         geom = feat["dggsPlace"]
         props = feat.get("properties", {})
         id = feat.get("id", None)
         converted = convert_geometry_indexed(geom, centroids, id)
         feature = {
            "type": "Feature",
            "id": id,
            "properties": {
               "dggrs": curie,
               "zoneId": zone_text,
               "depth": depth,
               **props
            },
            "geometry": finalize_result(projection, extent, converted, id)
         }
         out_feats.append(feature)
      result = {"type": "FeatureCollection", "features": out_feats}
   elif top_type == "Feature":
      geom = data["dggsPlace"]
      props = data.get("properties", {})
      converted = convert_geometry_indexed(geom, centroids, id)
      feature = {
         "type": "Feature",
         "properties": {
            "dggrs": curie,
            "zoneId": zone_text,
            "depth": depth,
            **props
         },
         "geometry": finalize_result(projection, extent, converted)
      }
      result = feature
   else:
      geom = data
      result = finalize_result(projection, extent, convert_geometry_indexed(geom, centroids, id), id)

   Instance.delete(centroids)
   Instance.delete(projection)
   Instance.delete(dggrs)

   return result
