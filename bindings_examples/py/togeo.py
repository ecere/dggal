#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from dggal import *
from geom import *

def convertToGeoJSON(inputFile, options):
   exitCode = 1
   f = fileOpen(inputFile, FileOpenMode.read)
   if f:
      dggsJSON = readDGGSJSON(f)
      if dggsJSON:
         dggrsClass = None

         dggrsID = getLastDirectory(dggsJSON.dggrs)

         # We could use globals()['GNOSISGlobalGrid'] to be more generic, but here we limit to DGGRSs we know
         if   not strnicmp(dggrsID, "GNOSIS", 6): dggrsClass = GNOSISGlobalGrid
         elif not strnicmp(dggrsID, "ISEA3H", 6): dggrsClass = ISEA3H
         elif not strnicmp(dggrsID, "ISEA9R", 6): dggrsClass = ISEA9R
         elif not strnicmp(dggrsID, "IVEA3H", 6): dggrsClass = IVEA3H
         elif not strnicmp(dggrsID, "IVEA9R", 6): dggrsClass = IVEA9R
         elif not strnicmp(dggrsID, "RTEA3H", 6): dggrsClass = RTEA3H
         elif not strnicmp(dggrsID, "RTEA9R", 6): dggrsClass = RTEA9R
         elif not strnicmp(dggrsID, "rHEALPix", 8): dggrsClass = rHEALPix

         if dggrsClass:
            zoneID = dggsJSON.zoneId
            dggrs = dggrsClass()
            zone = dggrs.getZoneFromTextID(zoneID)

            if zone != nullZone:
               if dggsJSON.depths:
                  maxDepth = -1

                  for d in range(dggsJSON.depths.count):
                     depth = dggsJSON.depths[d]
                     if depth > maxDepth:
                        maxDepth = depth
                        break;
                  if d < dggsJSON.depths.count:
                     depth = maxDepth
                     subZones = dggrs.getSubZones(zone, depth)
                     centroids = options.get('centroids') if options else None
                     crsOption = options.get('crs') if options else None
                     crs = 0

                     if crsOption:
                        if   crsOption == "5x6":  crs = CRS(ogc, 153456)
                        elif crsOption == "isea": crs = CRS(ogc, 1534)

                     if subZones:
                        i = 0

                        printLn("{")
                        printLn("   \"type\": \"FeatureCollection\",")
                        printx ("   \"features\": [ ")

                        for z in subZones:
                           props = { }
                           printx(", " if i else "   ")

                           # NOTE: We should eventually try to support __iter__ on containers
                           #       for key, depths in dggsJSON.values.items():
                           it = MapIterator(map = dggsJSON.values)
                           while it.next():
                              key = it.key
                              depths = it.value
                              if key and depths and depths.count > d:
                                 djDepth = depths[d]
                                 data = djDepth.data
                                 props[key] = data[i]

                           generateZoneFeature(dggrs, z, crs, i + 1, centroids, True, props)
                           i += 1
                        printLn(" ]")
                        printLn("}")
            else:
               printLn("Invalid zone ID: ", zoneID)
         else:
            printLn("Failure to recognize DGGRS")
      else:
         printLn("Failure to parse DGGS-JSON file ", inputFile)
   else:
      printLn("Failure to open file ", inputFile)
   return exitCode

# TODO: Parse arguments

input = "A4-0-A.json"
options = { }
#options['crs'] = 'isea'

convertToGeoJSON(input, options)
