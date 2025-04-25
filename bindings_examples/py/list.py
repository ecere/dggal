#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from dggal import *

app = Application(appGlobals=globals())
pydggal_setup(app)

def parseBBox(options, bbox):
   result = True
   bboxOption = options.get('bbox') if options else None
   if bboxOption:
      s = bboxOption
      tokens = tokenizeWith(s, 4, ",", False)
      result = False
      if len(tokens) == 4:
         try:
            a = float(tokens[0])
            b = float(tokens[1])
            c = float(tokens[2])
            d = float(tokens[3])
            if a < 90 and a > -90:
               bbox.ll = (a, b)
               bbox.ur = (c, d)
               result = True
            else:
               result = False
         except ValueError:
            result = False
      if result == False:
         printLn("Invalid bounding box specified")
   return result

def listZones(dggrs, level, options = None):
   exitCode = 0
   centroids = options.get('centroids') if options is not None else None
   compact = options.get('compact') if options is not None else None
   bbox = wholeWorld

   if not parseBBox(options, bbox):
      exitCode = 1

   if compact is not None and centroids is not None:
      exitCode = 1;
      printLn("Cannot return compact list of zones as centroids")

   if level == -1:
      level = 0

   if not exitCode:
      i = 0
      zones = dggrs.listZones(level, bbox)

      if compact is not None:
         dggrs.compactZones(zones)

      printx("[");
      if zones is not None:
         for z in zones:
            printx(", " if i > 0 else " ")
            if centroids is not None:
               centroid = dggrs.getZoneWGS84Centroid(z)
               printx("[ ", centroid.lat, ", ", centroid.lon, " ]")
            else:
               zoneID = dggrs.getZoneTextID(z)
               printx("\"", zoneID, "\"")
            i += 1
      printLn(" ]")
   return 0

dggrs = ISEA3H()
# dggrs = GNOSISGlobalGrid()

# TODO: Parse arguments

level = 0
options = { }
options['bbox'] = '30,40,50,60'

listZones(dggrs, level, options)
