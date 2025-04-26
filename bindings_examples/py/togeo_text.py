#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from dggal import *

# This alternate implementation of the functions in geom.py and togeo.py converts
# a DGGS-JSON JSON dictionary to a GeoJSON JSON text string
# (instead of using DGGAL::readDGGSJSON() and printing to standard output)

def generateZoneFeature(dggrs, zone, crs, id, centroids: bool, fc: bool, properties):
   t = "   " if fc else ""

   zoneID = dggrs.getZoneTextID(zone)

   features = '{\n'
   features = features + t + '   "type" : "Feature",\n'
   features = features + t + '   "id" : '
   if id:
      features = features + f'"{id}"'
   else:
      features = features + zoneID
   features = features + ',\n'
   features = features + generateZoneGeometry(dggrs, zone, crs, id, centroids, fc)
   features = features + ',\n'
   features = features + t + '   "properties" : {\n'
   features = features + t + f'     "zoneID" : "{zoneID}"'
   if properties:
      for key, v in properties.items():
         features = features + ',\n' + t + f'     "{key}" : {v}'

   features = features + '\n'
   features = features + t + '   }\n'
   features = features + t + '}'
   return features

def generateZoneGeometry(dggrs, zone, crs, id, centroids: bool, fc: bool):
   t = "   " if fc else ""

   geometry = t + '   "geometry" : {\n'
   geometry = geometry + t + '      "type" : "' + ('Point' if centroids else 'Polygon') + '",\n'
   geometry = geometry + t + '      "coordinates" : [\n'

   if not crs or crs == CRS(ogc, 84) or crs == CRS(epsg, 4326):
      if centroids:
         centroid = dggrs.getZoneWGS84Centroid(zone, centroid)
         geometry = geometry + " " + f"{centroid.lon}, {centroid.lat}"
      else:
         vertices = dggrs.getZoneRefinedWGS84Vertices(zone, 0)
         if vertices:
            count = vertices.count

            geometry = geometry + "\n"
            geometry = geometry + t + "         [ "
            for i in range(count):
               geometry = geometry + (", " if i else "") + f"[{vertices[i].lon}, {vertices[i].lat}]"
            geometry = geometry + (", " if i else "") + f"[{vertices[0].lon}, {vertices[0].lat}]"
            geometry = geometry + " ]\n"
         geometry = geometry + t + "     "
   else:
      if centroids:
         centroid = dggrs.getZoneCRSCentroid(zone, crs, centroid)
         geometry = geometry + f" {centroid.x}, {centroid.y}"
      else:
         vertices = dggrs.getZoneRefinedCRSVertices(zone, crs, 0)
         if vertices:
            count = vertices.count

            geometry = geometry + "\n"
            geometry = geometry + t + "         [ \n"

            for i in range(count):
               geometry = geometry + (", " if i else "") + f"[{vertices[i].x}, {vertices[i].y}]"
            geometry = geometry + (", " if i else "") + f"[{vertices[0].x}, {vertices[0].y}]"
            geometry = geometry + " ]\n"
         geometry = geometry + t + "     "
   geometry = geometry + " ]\n"
   geometry = geometry + t + "   }"
   return geometry

def dggsJSON2GeoJSON(dggsJSON, crs: CRS = None, centroids: bool = False):
   result = None
   if dggsJSON is not None:
         dggrsClass = None
         dggrsID = getLastDirectory(dggsJSON['dggrs'])

         # We could use globals()['GNOSISGlobalGrid'] to be more generic, but here we limit to DGGRSs we know
         if   not strnicmp(dggrsID, "GNOSIS", 6): dggrsClass = GNOSISGlobalGrid
         elif not strnicmp(dggrsID, "ISEA3H", 6): dggrsClass = ISEA3H
         elif not strnicmp(dggrsID, "ISEA9R", 6): dggrsClass = ISEA9R
         elif not strnicmp(dggrsID, "IVEA3H", 6): dggrsClass = IVEA3H
         elif not strnicmp(dggrsID, "IVEA9R", 6): dggrsClass = IVEA9R

         if dggrsClass:
            zoneID = dggsJSON['zoneId']
            dggrs = dggrsClass()
            zone = dggrs.getZoneFromTextID(zoneID)

            if zone != nullZone:
                depths = dggsJSON['depths']
                if depths:
                  maxDepth = -1

                  for d in range(len(depths)):
                     depth = depths[d]
                     if depth > maxDepth:
                        maxDepth = depth
                        break;
                  if d < len(depths):
                     depth = maxDepth
                     subZones = dggrs.getSubZones(zone, depth)
                     if subZones:
                        i = 0
                        collection = "{\n"
                        collection = collection + "   \"type\": \"FeatureCollection\",\n"
                        collection = collection + "   \"features\": [ "

                        values = dggsJSON['values']
                        for z in subZones:
                           props = { }
                           collection = collection + (", " if i else "   ")

                           # NOTE: We should eventually try to support __iter__ on containers
                           #       for key, depths in dggsJSON.values.items():
                           for key, vDepths in values.items():
                              if key and vDepths and len(vDepths) > d:
                                 djDepth = vDepths[d]
                                 data = djDepth['data']
                                 props[key] = data[i]

                           collection = collection + generateZoneFeature(dggrs, z, crs, i + 1, centroids, True, props)
                           i += 1
                        collection = collection + " ]\n"
                        collection = collection + "}\n"
                        result = collection
   return result
