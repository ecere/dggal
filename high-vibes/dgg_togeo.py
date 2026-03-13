#!/usr/bin/env python3
# -*- coding: utf-8

import argparse
import json
import sys
from dggal import *
import geopandas as gpd
from shapely.geometry import Point, Polygon

app = Application(appGlobals=globals())
pydggal_setup(app)

def infer_format_from_filename(path):
   p = str(path).lower() if path else ""
   if p.endswith(".json") or p.endswith(".geojson"):
      return "geojson"
   if p.endswith(".parquet") or p.endswith(".parq") or p.endswith(".pq"):
      return "geoparquet"
   if p.endswith(".fgb") or p.endswith(".flatgeobuf"):
      return "fgb"
   return "geojson"

# Resolve CRS option string to a DGGAL CRS value
def resolve_crs_string(crsOption):
   crs = 0
   if crsOption:
      s = str(crsOption).lower()
      if s == "5x6":
         crs = CRS(ogc, 153456)
      elif s in ("ico", "isea", "ivea", "rtea"):
         crs = CRS(ogc, 1534)
      elif s in ("crs84", "ogc:crs84"):
         crs = CRS(ogc, 84)
      elif s in ("wgs84", "epsg:4326", "4326"):
         crs = CRS(epsg, 4326)
      elif s in ("rhp", "hpx", "hlp"):
         crs = CRS(ogc, 99999)
   return crs

# Build a Shapely geometry directly from DGGRS vertices / centroids
def generate_zone_geometry_shapely(dggrs, zone, crs, centroids: bool):
   if centroids:
      if (not crs) or (crs == CRS(ogc, 84)) or (crs == CRS(epsg, 4326)):
         c = dggrs.getZoneWGS84Centroid(zone)
         return Point(c.lon.value, c.lat.value)
      else:
         c = dggrs.getZoneCRSCentroid(zone, crs)
         return Point(c.x, c.y)
   else:
      if (not crs) or (crs == CRS(ogc, 84)) or (crs == CRS(epsg, 4326)):
         verts = dggrs.getZoneRefinedWGS84Vertices(zone, 0)
         if not verts or verts.count == 0:
            return None
         coords = [(verts[i].lon.value, verts[i].lat.value) for i in range(verts.count)]
      else:
         verts = dggrs.getZoneRefinedCRSVertices(zone, crs, 0)
         if not verts or verts.count == 0:
            return None
         coords = [(verts[i].x, verts[i].y) for i in range(verts.count)]
      if coords and coords[0] != coords[-1]:
         coords.append(coords[0])
      return Polygon(coords)

# Return (features_list, error_message). features_list is list of (shapely_geom, properties)
def dggsJSON2shapely_features(dggsJSON, crs: CRS = None, centroids: bool = False):
   if dggsJSON is None:
      return (None, "No input JSON provided")

   if 'dggrs' not in dggsJSON:
      return (None, "Missing 'dggrs' in input JSON")
   dggrsID = getLastDirectory(dggsJSON['dggrs'])
   dggrsClass = None

   if not strnicmp(dggrsID, "GNOSIS", 6):    dggrsClass = GNOSISGlobalGrid
   elif not strnicmp(dggrsID, "ISEA4R", 6):  dggrsClass = ISEA4R
   elif not strnicmp(dggrsID, "ISEA9R", 6):  dggrsClass = ISEA9R
   elif not strnicmp(dggrsID, "ISEA3H", 6):  dggrsClass = ISEA3H
   elif not strnicmp(dggrsID, "ISEA7H", 6):  dggrsClass = ISEA7H
   elif not strnicmp(dggrsID, "IVEA4R", 6):  dggrsClass = IVEA4R
   elif not strnicmp(dggrsID, "IVEA9R", 6):  dggrsClass = IVEA9R
   elif not strnicmp(dggrsID, "IVEA3H", 6):  dggrsClass = IVEA3H
   elif not strnicmp(dggrsID, "IVEA7H", 6):  dggrsClass = IVEA7H
   elif not strnicmp(dggrsID, "RTEA4R", 6):  dggrsClass = RTEA4R
   elif not strnicmp(dggrsID, "RTEA9R", 6):  dggrsClass = RTEA9R
   elif not strnicmp(dggrsID, "RTEA3H", 6):  dggrsClass = RTEA3H
   elif not strnicmp(dggrsID, "RTEA7H", 6):  dggrsClass = RTEA7H
   elif not strnicmp(dggrsID, "HEALPix", 7): dggrsClass = HEALPix
   elif not strnicmp(dggrsID, "rHEALPix", 8):dggrsClass = rHEALPix

   if not dggrsClass:
      return (None, "Failure to recognize DGGRS: " + str(dggrsID))

   dggrs = dggrsClass()

   if 'zoneId' not in dggsJSON:
      return (None, "Missing 'zoneId' in input JSON")
   zoneID = dggsJSON['zoneId']
   zone = dggrs.getZoneFromTextID(zoneID)
   if zone == nullZone:
      return (None, "Invalid zone ID: " + str(zoneID))

   depths = dggsJSON.get('depths', None)
   if not depths:
      return (None, "Missing 'depths' in input JSON")

   maxDepth = -1
   for idx in range(len(depths)):
      depth_val = depths[idx]
      if depth_val > maxDepth:
         maxDepth = depth_val
   if maxDepth < 0:
      return (None, "No valid depth found")
   depth = maxDepth

   subZones = dggrs.getSubZones(zone, depth)
   if not subZones:
      return (None, "No subzones returned for depth " + str(depth))

   values = dggsJSON.get('values', {})
   features = []
   i = 0
   for z in subZones:
      geom = generate_zone_geometry_shapely(dggrs, z, crs, centroids)
      props = {}
      for key, vDepths in values.items():
         if key and vDepths and len(vDepths) > idx:
            djDepth = vDepths[idx]
            data = djDepth.get('data', [])
            if len(data) > i:
               raw = data[i]
               props[key] = raw
      props['zoneID'] = str(dggrs.getZoneTextID(z))
      features.append((geom, props))
      i += 1

   return (features, None)

def write_features_shapely(features, out_path, fmt):
   geoms = []
   props = []
   for geom, properties in features:
      geoms.append(geom)
      props.append(properties)
   gdf = gpd.GeoDataFrame(props, geometry=geoms, crs="EPSG:4326")
   if fmt == "geojson":
      gdf.to_file(out_path, driver="GeoJSON")
      return True
   if fmt == "geoparquet":
      gdf.to_parquet(out_path, compression='snappy')
      return True
   if fmt == "fgb":
      gdf.to_file(out_path, driver="FlatGeobuf")
      return True
   return False

def main(argv=None):
   p = argparse.ArgumentParser(prog="dgg togeo (shapely direct)")
   p.add_argument("input", help="DGGS-JSON input file (plain JSON)")
   p.add_argument("output", help="Output file path")
   p.add_argument("-format", "-f", choices=["geojson","geoparquet","fgb","auto"], default="auto")
   p.add_argument("-crs", dest="crs", help="CRS option: 5x6, ico, EPSG:4326, OGC:CRS84, hpx, rhp")
   p.add_argument("-centroids", action="store_true", help="Emit centroid points instead of polygons")
   args = p.parse_args(argv)

   fmt = args.format if args.format != "auto" else infer_format_from_filename(args.output)

   crs = resolve_crs_string(args.crs)

   with open(args.input, 'r', encoding='utf-8') as fh:
      dggsJSON = json.load(fh)

   features, err = dggsJSON2shapely_features(dggsJSON, crs=crs, centroids=args.centroids)
   if err:
      print("Error:", err, file=sys.stderr)
      sys.exit(1)

   ok = write_features_shapely(features, args.output, fmt)
   if not ok:
      print("Error: unsupported output format: " + str(fmt), file=sys.stderr)
      sys.exit(1)

   print("Wrote", len(features), "features to", args.output, "(", fmt, ")")
   sys.exit(0)

if __name__ == "__main__":
   main()
