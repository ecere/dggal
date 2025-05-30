#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from dggal import *

app = Application(appGlobals=globals())
pydggal_setup(app)

def generateZoneFeature(dggrs, zone, crs, id, centroids: bool, fc: bool, properties):
   t = "   " if fc else ""

   zoneID = dggrs.getZoneTextID(zone)

   printLn('{')
   printLn(t, '   "type" : "Feature",')
   printx (t, '   "id" : ')
   if id:
      printx(id)
   else:
      printx(f'"{zoneID}"')
   printLn(',')

   generateZoneGeometry(dggrs, zone, crs, id, centroids, fc)

   printLn(',')

   printLn(t, '   "properties" : {')
   printx (t, '     "zoneID" : "')
   printx(zoneID)
   printx('"')
   if properties:
      for key, v in properties.items():
         printx(',\n', t, '     "', key, '" : ', v)

   printLn('')
   printLn(t, '   }')
   printx(t, '}')

def generateZoneGeometry(dggrs, zone, crs, id, centroids: bool, fc: bool):
   t = "   " if fc else ""

   printLn(t, '   "geometry" : {');
   printLn(t, '      "type" : "', 'Point' if centroids else 'Polygon', '",')
   printx (t, '      "coordinates" : [')

   if not crs or crs == CRS(ogc, 84) or crs == CRS(epsg, 4326):
      if centroids:
         centroid = dggrs.getZoneWGS84Centroid(zone)
         printx(" ", centroid.lon, ", ", centroid.lat)
      else:
         vertices = dggrs.getZoneRefinedWGS84Vertices(zone, 0)
         if vertices:
            count = vertices.count

            printLn("")
            printx(t, "         [ ")
            for i in range(count):
               printx(", " if i else "", "[", vertices[i].lon, ", ", vertices[i].lat, "]")
            printx(", " if count else "", "[", vertices[0].lon, ", ", vertices[0].lat, "]")
            printLn(" ]")
         printx(t, "     ")
   else:
      if centroids:
         centroid = dggrs.getZoneCRSCentroid(zone, crs)
         printx(" ", centroid.x, ", ", centroid.y);
      else:
         vertices = dggrs.getZoneRefinedCRSVertices(zone, crs, 0)
         if vertices:
            count = vertices.count

            printLn("")
            printLn(t, "         [ ")

            for i in range(count):
               printx(", " if i else "", "[", vertices[i].x, ", ", vertices[i].y, "]")
            printx(", " if count else "", "[", vertices[0].x, ", ", vertices[0].y, "]")
            printLn(" ]")
         printx(t, "     ")
   printLn(" ]")
   printx(t, "   }")

def generateGeometry(dggrs, zone, options):
   if zone != nullZone:
      centroids = options.get('centroids') if options else None
      crsOption = options.get('crs') if options else None
      crs = 0

      if crsOption:
         if crsOption == "5x6":    crs = CRS(ogc, 153456)
         elif crsOption == "isea": crs = CRS(ogc, 1534)

      generateZoneFeature(dggrs, zone, crs, 0, centroids, False, None)
      printLn("")
      return 0
   else:
      printLn("geom command requires a zone")
   return 1

if __name__ == "__main__":
   dggrs = ISEA3H()

   # TODO: Parse arguments

   options = { }
   #options['crs'] = 'isea'

   zoneID = 'A4-0-A'
   zone = dggrs.getZoneFromTextID(zoneID)

   generateGeometry(dggrs, zone, options)
