#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# This alternate implementation of the functions in geom.py and togeo.py converts
# a DGGS-JSON JSON dictionary to a GeoJSON JSON dictionary
# (instead of using DGGAL::readDGGSJSON() and printing to standard output)

from dggal import *

def generateZoneGeometry(dggrs, zone, crs, id, centroids: bool, fc: bool):
   coordinates = []
   if not crs or crs == CRS(ogc, 84) or crs == CRS(epsg, 4326):
      if centroids:
         centroid = dggrs.getZoneWGS84Centroid(zone, centroid)
         coordinates.append(centroid.lon.value)
         coordinates.append(centroid.lat.value)
      else:
         vertices = dggrs.getZoneRefinedWGS84Vertices(zone, 0)
         if vertices:
            contour = [ ]
            for v in vertices:
               contour.append([ v.lon.value, v.lat.value])
            contour.append([vertices[0].lon.value, vertices[0].lat.value])
            coordinates.append(contour)
   else:
      if centroids:
         centroid = dggrs.getZoneCRSCentroid(zone, crs, centroid)
         coordinates.append(centroid.x)
         coordinates.append(centroid.y)
      else:
         vertices = dggrs.getZoneRefinedCRSVertices(zone, crs, 0)
         if vertices:
            count = vertices.count
            contour = [ ]
            for v in vertices:
               contour.append([v.x, v.y])
            contour.append([vertices[0].x, vertices[0].y])
            coordinates.append(contour)
   geometry = {
      'type': 'Point' if centroids else 'Polygon',
      'coordinates': coordinates
   }
   return geometry

def generateZoneFeature(dggrs, zone, crs, id, centroids: bool, fc: bool, props):
   zoneID = dggrs.getZoneTextID(zone)

   properties = {
      'zoneID': f'{zoneID}'
   }
   if props:
      for key, v in props.items():
         properties[key] = v

   features = {
      'type': 'Feature',
      'id': id if id is not None else zoneID,
      'geometry': generateZoneGeometry(dggrs, zone, crs, id, centroids, fc),
      'properties': properties
   }
   return features

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
                        values = dggsJSON['values']
                        features = [ ]
                        for z in subZones:
                           props = { }

                           # NOTE: We should eventually try to support __iter__ on containers
                           #       for key, depths in dggsJSON.values.items():
                           for key, vDepths in values.items():
                              if key and vDepths and len(vDepths) > d:
                                 djDepth = vDepths[d]
                                 data = djDepth['data']
                                 props[key] = data[i]

                           features.append(generateZoneFeature(dggrs, z, crs, i + 1, centroids, True, props))
                           i += 1
                        result = {
                           'type': 'FeatureCollection',
                           'features': features
                        }
   return result
