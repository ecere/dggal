from dggal import *
import sys
import json
import argparse

from ogcapi.utils import *

# 3-space indent style throughout this file

def get_dggrs(dggrs_name):
   # resolve DGGRS object from globals exactly as requested
   return globals()[dggrs_name]()

# parse a CURIE like "...-dggrs:ID]" and return a DGGRS instance via get_dggrs()
def get_dggrs_from_curie(curie: str):
   # find the substring after "-dggrs:" and before the next closing bracket ']'
   token = "-dggrs:"
   start = curie.find(token) + len(token)
   end = curie.find("]", start)
   dggrs_id = curie[start:end]
   return get_dggrs(dggrs_id)

def instantiate_projection_for_dggrs_name(dggrs_name):
   # choose projection class by DGGRS short name prefix
   if dggrs_name.startswith("IVEA"):
      return IVEAProjection()
   elif dggrs_name.startswith("RTEA"):
      return RTEAProjection()
   elif dggrs_name.startswith("ISEA"):
      return ISEAProjection()
   elif dggrs_name.startswith("HEALPix"):
      return HEALPixProjection()
   elif dggrs_name.startswith("rHEALPix"):
      return rHEALPixProjection()
   elif dggrs_name.startswith("GNOSISGlobalGrid"):
      # no projection: identity mapping (pass-through lon,lat)
      return None
   else:
      # contract: unrecognized DGGRS name is a hard failure
      raise NameError("unrecognized DGGRS for projection selection: " + dggrs_name)

def geojson_load(path):
   with open(path, "r", encoding="utf-8") as fh:
      return json.load(fh)

def geojson_dump(obj, path):
   with open(path, "w", encoding="utf-8") as fh:
      #json.dump(obj, fh, separators=(",", ":"), ensure_ascii=False)
      fh.write(pretty_json(obj))
      fh.write("\n")

def make_GeoPoint(lon, lat):
   gp = GeoPoint()
   gp.lon = Degrees(lon)
   gp.lat = Degrees(lat)
   return gp

def make_Pointd(x, y):
   pd = Pointd()
   pd.x = float(x)
   pd.y = float(y)
   return pd

def reproject_coord_lonlat_to_crs(lon, lat, proj, ico=False):
   # input: lon, lat in degrees (WGS84)
   # output: [x, y] in DGGRS native CRS (float)
   if proj is None:
      # GNOSISGlobalGrid: identity pass-through (lon, lat)
      return [float(lon), float(lat)]
   in_gp = make_GeoPoint(lon, lat)
   out_pd = Pointd()
   # forward: WGS84 GeoPoint -> projected Pointd; oddGrid False per contract
   proj.forward(in_gp, out_pd)

   # if ico mode requested and projection exposes toIcosahedronNet, apply it in-place
   if ico:
      # projection binding: toIcosahedronNet(v=None, result=None)
      # call with out_pd as both v and result to mutate in-place
      proj.__class__.toIcosahedronNet(out_pd, out_pd)

   return [float(out_pd.x), float(out_pd.y)]

def reproject_coords_array(coords, proj, ico=False):
   out = []
   for lon, lat in coords:
      out.append(reproject_coord_lonlat_to_crs(lon, lat, proj, ico=ico))
   return out

def reproject_geometry(geom, proj, ico=False):
   t = geom["type"]
   if t == "Point":
      lon, lat = geom["coordinates"]
      return {"type": "Point", "coordinates": reproject_coord_lonlat_to_crs(lon, lat, proj, ico=ico)}
   if t == "MultiPoint":
      return {"type": "MultiPoint", "coordinates": reproject_coords_array(geom["coordinates"], proj, ico=ico)}
   if t == "LineString":
      return {"type": "LineString", "coordinates": reproject_coords_array(geom["coordinates"], proj, ico=ico)}
   if t == "MultiLineString":
      return {"type": "MultiLineString", "coordinates": [reproject_coords_array(r, proj, ico=ico) for r in geom["coordinates"]]}
   if t == "Polygon":
      return {"type": "Polygon", "coordinates": [reproject_coords_array(r, proj, ico=ico) for r in geom["coordinates"]]}
   if t == "MultiPolygon":
      return {"type": "MultiPolygon", "coordinates": [[reproject_coords_array(r, proj, ico=ico) for r in poly] for poly in geom["coordinates"]]}
   if t == "GeometryCollection":
      return {"type": "GeometryCollection", "geometries": [reproject_geometry(g, proj, ico=ico) for g in geom["geometries"]]}
   return None

def reproject_feature(feature, proj, ico=False):
   out = {
      "type": "Feature",
      "id": feature.get("id"),
      "properties": feature.get("properties"),
      "geometry": None
   }
   geom = feature.get("geometry")
   if geom is not None:
      out["geometry"] = reproject_geometry(geom, proj, ico=ico)
   return out

def reproject_featurecollection(fc, proj, ico=False):
   out = {
      "type": "FeatureCollection",
      "features": []
   }
   for feat in fc["features"]:
      out["features"].append(reproject_feature(feat, proj, ico=ico))
   return out

