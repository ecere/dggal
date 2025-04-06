#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from dggal import *

app = Application(appGlobals=globals())
pydggal_setup(app)

def zoneInfo(dggrs, zone):
   centroid = GeoPoint()
   centroid = dggrs.getZoneWGS84Centroid(zone)
   area = dggrs.getZoneArea(zone)
   areaKM2 = area / 1000000
   level = dggrs.getZoneLevel(zone)
   zoneID = dggrs.getZoneTextID(zone)
   nEdges = dggrs.countZoneEdges(zone)
   depth = dggrs.get64KDepth()
   parents = dggrs.getZoneParents(zone)
   children = dggrs.getZoneChildren(zone)
   nbTypes = Array("<int>")
   neighbors = dggrs.getZoneNeighbors(zone, nbTypes)
   centroidParent = dggrs.getZoneCentroidParent(zone)
   centroidChild = dggrs.getZoneCentroidChild(zone)
   isCentroidChild = dggrs.isZoneCentroidChild(zone)

   nSubZones = dggrs.countSubZones(zone, depth)
   printLn("Zone ID: ", zoneID)
   printLn("Level ", level, " zone (", nEdges, " edges", ", centroid child)" if isCentroidChild else ")")
   printLn(nSubZones, " sub-zones at depth ", depth)
   printLn("WGS84 Centroid (lat, lon): ", centroid);
   printLn(area, " m² (", areaKM2, " km²)")

   printLn("")
   if parents.count:
      printLn("Parent", "s" if parents.count > 1 else "", " (", parents.count, "):")
      for p in parents:
         pID = dggrs.getZoneTextID(p)
         printx("   ", pID)
         if centroidParent == p:
            printx(" (centroid child)")
         printLn("");
   else:
      printLn("No parent")

   printLn("");
   printLn("Children (", children.count, "):")
   for ch in children:
      cID = dggrs.getZoneTextID(ch)
      printx("   ", cID)
      if centroidChild == ch:
         printx(" (centroid)")
      printLn("")

   printLn("")
   printLn("Neighbors (", neighbors.count, "):")
   i = 0
   for nb in neighbors:
      nID = dggrs.getZoneTextID(nb)
      printLn("   (direction ", nbTypes[i], "): ", nID)
      i += 1

def zoneGeometry(dggrs, zone):
   v = dggrs.getZoneRefinedWGS84Vertices(zone, 0)
   return v

dggrs = ISEA3H()

zoneID = "A4-0-A"
zone = dggrs.getZoneFromTextID(zoneID)

zoneInfo(dggrs, zone)
printLn("")

geom = zoneGeometry(dggrs, dggrs.getZoneFromTextID(zoneID))
printLn("Refined geometry vertices for ", zoneID, " (lon, lat):")
for v in geom:
   printLn("   [", v.lon, ", ", v.lat, "]")

subZones = dggrs.getSubZones(zone, 3)
print(subZones.getCount(), "sub-zones at depth 3:")

for z in subZones:
   printLn("   - ", dggrs.getZoneTextID(z))

depth = dggrs.get64KDepth()
index = 100
subZone = dggrs.getSubZoneAtIndex(zone, depth, index)
printLn("Sub-zone at depth ", depth, " index ", index, " is ", dggrs.getZoneTextID(subZone))
