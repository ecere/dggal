public import IMPORT_STATIC "ecrt"
import IMPORT_STATIC "dggal"
import IMPORT_STATIC "GeoExtents"

import "geom"  // For resolveCRSString()

int displayInfo(DGGRS dggrs, DGGRSZone zone, Map<String, const String> options)
{
   if(zone != nullZone)
      return zoneInfo(dggrs, zone, options);
   else
      return dggrsInfo(dggrs, options);
}

static int zoneInfo(DGGRS dggrs, DGGRSZone zone, Map<String, const String> options)
{
   int level = dggrs.getZoneLevel(zone);
   int nEdges = dggrs.countZoneEdges(zone);
   GeoPoint geoCentroid;
   GeoExtent geoExtent;
   GeoPoint geoVertices[6];
   Pointd vertices[6];
   Pointd centroid;
   CRSExtent extent;
   char zoneID[256];
   double area = dggrs.getZoneArea(zone);
   int depth = dggrs.get64KDepth();
   DGGRSZone parents[3], neighbors[6], children[13];
   int nParents = dggrs.getZoneParents(zone, parents);
   int nbTypes[6];
   int nNeighbors = dggrs.getZoneNeighbors(zone, neighbors, nbTypes);
   int nChildren = dggrs.getZoneChildren(zone, children);
   DGGRSZone centroidParent = dggrs.getZoneCentroidParent(zone);
   DGGRSZone centroidChild = dggrs.getZoneCentroidChild(zone);
   bool isCentroidChild = dggrs.isZoneCentroidChild(zone);
   int i;
   int64 nSubZones;
   const String depthOption = options ? options["depth"] : null;
   const String crsOption = options ? options["crs"] : null;
   CRS crs = resolveCRSString(crsOption);
   const String crsString = crsOption ? crsOption : "EPSG:4326";
   int nVertices;

   if(crsOption)
      nVertices = dggrs.getZoneCRSVertices(zone, crs, vertices);
   else
      nVertices = dggrs.getZoneWGS84Vertices(zone, geoVertices);

   if(depthOption)
   {
      int maxDepth = dggrs.getMaxDepth();
      depth.OnGetDataFromString(depthOption);
      if(depth > maxDepth)
      {
         PrintLn($"Invalid depth (maximum: ", maxDepth, ")");
         return 1;
      }
   }

   nSubZones = dggrs.countSubZones(zone, depth);

   if(crsOption)
   {
      dggrs.getZoneCRSCentroid(zone, crs, centroid);
      dggrs.getZoneCRSExtent(zone, crs, extent);
   }
   else
   {
      dggrs.getZoneWGS84Centroid(zone, geoCentroid);
      dggrs.getZoneWGS84Extent(zone, geoExtent);
   }

   dggrs.getZoneTextID(zone, zoneID);

   PrintLn($"Textual Zone ID: ", zoneID);
   Print($"64-bit integer ID: ", (uint64)zone, " (");
   printf(FORMAT64HEX, zone);
   PrintLn(")");
   PrintLn("");
   PrintLn($"Level ", level, $" zone (", nEdges, $" edges", isCentroidChild ? $", centroid child)" : ")");
   PrintLn(area, " m² (", area / 1000000, " km²)");
   PrintLn(nSubZones, $" sub-zones at depth ", depth);
   if(crsOption)
   {
      PrintLn($"Centroid: ", centroid.x, ", ", centroid.y);
      PrintLn($"Extent: { top-left: { ", extent.tl.x, ", ", extent.tl.y, " }, bottom-right: { ", extent.br.x, ", ", extent.br.y, " } }");
   }
   else
   {
      PrintLn($"WGS84 Centroid (lat, lon): ", geoCentroid.lat, ", ", geoCentroid.lon);
      PrintLn($"WGS84 Extent (lat, lon): { ", geoExtent.ll.lat, ", ", geoExtent.ll.lon, " }, { ", geoExtent.ur.lat, ", ", geoExtent.ur.lon, " }");
   }

   PrintLn("");
   if(nParents)
   {
      PrintLn($"Parent", nParents > 1 ? "s" : "", " (", nParents, "):");
      for(i = 0; i < nParents; i++)
      {
         char pID[256];
         dggrs.getZoneTextID(parents[i], pID);
         Print("   ", pID);
         if(centroidParent == parents[i])
            Print($" (centroid child)");
         PrintLn("");
      }
   }
   else
      PrintLn($"No parent");

   PrintLn("");
   PrintLn($"Children (", nChildren, "):");
   for(i = 0; i < nChildren; i++)
   {
      char cID[256];
      dggrs.getZoneTextID(children[i], cID);
      Print("   ", cID);
      if(centroidChild == children[i])
         Print($" (centroid)");
      PrintLn("");
   }

   PrintLn("");
   PrintLn($"Neighbors (", nNeighbors, "):");
   for(i = 0; i < nNeighbors; i++)
   {
      char nID[256];
      dggrs.getZoneTextID(neighbors[i], nID);
      PrintLn($"   (direction ", nbTypes[i], "): ", nID);
   }

   PrintLn("");
   PrintLn("[", crsString, $"] Vertices (", nVertices, "):");

   for(i = 0; i < nVertices; i++)
   {
      if(crsOption)
         PrintLn("   ", vertices[i].x, ", ", vertices[i].y);
      else
         PrintLn("   ", geoVertices[i].lat, ", ", geoVertices[i].lon);
   }
   return 0;
}

static int dggrsInfo(DGGRS dggrs, Map<String, const String> options)
{
   int depth64k = dggrs.get64KDepth();
   int ratio = dggrs.getRefinementRatio();
   int maxLevel = dggrs.getMaxDGGRSZoneLevel();

   PrintLn($"Refinement Ratio: ", ratio);
   PrintLn($"Maximum level for 64-bit global identifiers (DGGAL DGGRSZone): ", maxLevel);
   PrintLn($"Default ~64K sub-zones relative depth: ", depth64k);
   return 0;
}
