#include <dggal.h>

// parameterized templates in C! (for options map)
typedef Map T(Map, String, constString);
typedef MapIterator T(MapIterator, String, constString);

// For looking up internationalized strings
#define MODULE_NAME "info"

static int zoneInfo(DGGRS dggrs, DGGRSZone zone, T(Map, String, constString) options)
{
   int level = DGGRS_getZoneLevel(dggrs, zone);
   int nEdges = DGGRS_countZoneEdges(dggrs, zone);
   GeoPoint centroid;
   GeoExtent extent;
   GeoPoint vertices[6];
   int nVertices = DGGRS_getZoneWGS84Vertices(dggrs, zone, vertices);
   char zoneID[256];
   double area = DGGRS_getZoneArea(dggrs, zone), areaKM2 = area / 1000000;
   int depth = DGGRS_get64KDepth(dggrs);
   DGGRSZone parents[3], neighbors[6], children[9];
   int nParents = DGGRS_getZoneParents(dggrs, zone, parents);
   int nbTypes[6];
   int nNeighbors = DGGRS_getZoneNeighbors(dggrs, zone, neighbors, nbTypes);
   int nChildren = DGGRS_getZoneChildren(dggrs, zone, children);
   DGGRSZone centroidParent = DGGRS_getZoneCentroidParent(dggrs, zone);
   DGGRSZone centroidChild = DGGRS_getZoneCentroidChild(dggrs, zone);
   bool isCentroidChild = DGGRS_isZoneCentroidChild(dggrs, zone);
   int i;
   const String crs = "EPSG:4326";
   int64 nSubZones;
   constString depthOption = null;
   if(options)
   {
      T(MapIterator, String, constString) it = { options };
      if(Iterator_index((Iterator *)&it, TAp((void *)"depth"), false))
         depthOption = pTA(const char, Iterator_getData((Iterator *)&it));
   }

   if(depthOption)
   {
      int maxDepth = DGGRS_getMaxDepth(dggrs);
      _onGetDataFromString(CO(int), &depth, depthOption);
      if(depth > maxDepth)
      {
         printLn(CO(String), $("Invalid depth (maximum: "), CO(int), maxDepth, ")", null);
         return 1;
      }
   }

   nSubZones = DGGRS_countSubZones(dggrs, zone, depth);

   DGGRS_getZoneWGS84Centroid(dggrs, zone, &centroid);
   DGGRS_getZoneWGS84Extent(dggrs, zone, &extent);
   DGGRS_getZoneTextID(dggrs, zone, zoneID);

   printLn(CO(String), $("Textual Zone ID: "), CO(String), zoneID, null);
   printx(CO(String), $("64-bit integer ID: "), CO(uint64), &zone, CO(String), " (", null);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat" // FORMAT64HEX contains runtime checks for platform
   printf(FORMAT64HEX, zone);
#pragma GCC diagnostic pop
   printLn(CO(String), ")", null);
   printLn(CO(String), "", null);
   printLn(
      CO(String), $("Level "),
      CO(int), &level,
      CO(String), $(" zone ("),
      CO(int), &nEdges,
      CO(String), $(" edges"),
      CO(String), isCentroidChild ? $(", centroid child)") : ")", null);
   printLn(CO(double), &area, CO(String), " m² (", CO(double), &areaKM2, CO(String), " km²)", null);
   printLn(CO(int64), &nSubZones, CO(String), $(" sub-zones at depth "), CO(int), &depth, null);
   printLn(CO(String), $("WGS84 Centroid (lat, lon): "), CO(Degrees), &centroid.lat, CO(String), ", ", CO(Degrees), &centroid.lon, null);
   printLn(
      CO(String), $("WGS84 Extent (lat, lon): { "),
      CO(Degrees), &extent.ll.lat, CO(String), ", ",
      CO(Degrees), &extent.ll.lon, CO(String), " }, { ",
      CO(Degrees), &extent.ur.lat, CO(String), ", ",
      CO(Degrees), &extent.ur.lon, CO(String), " }", null);

   printLn(CO(String), "", null);
   if(nParents)
   {
      printLn(CO(String), $("Parent"), CO(String), nParents > 1 ? "s" : "", CO(String), " (", CO(int), &nParents, CO(String), "):", null);
      for(i = 0; i < nParents; i++)
      {
         char pID[256];
         DGGRS_getZoneTextID(dggrs, parents[i], pID);
         printx(CO(String), "   ", CO(String), pID, null);
         if(centroidParent == parents[i])
            printx(CO(String), $(" (centroid child)"), null);
         printLn(CO(String), "", null);
      }
   }
   else
      printLn(CO(String), $("No parent"), null);

   printLn(CO(String), "", null);
   printLn(CO(String), $("Children ("), CO(int), &nChildren, CO(String), "):", null);
   for(i = 0; i < nChildren; i++)
   {
      char cID[256];
      DGGRS_getZoneTextID(dggrs, children[i], cID);
      printx(CO(String), "   ", CO(String), cID, null);
      if(centroidChild == children[i])
         printx(CO(String), $(" (centroid)"), null);
      printLn(CO(String), "", null);
   }

   printLn(CO(String), "", null);
   printLn(CO(String), $("Neighbors ("), CO(int), &nNeighbors, CO(String), "):", null);
   for(i = 0; i < nNeighbors; i++)
   {
      char nID[256];
      DGGRS_getZoneTextID(dggrs, neighbors[i], nID);
      printLn(CO(String), $("   (direction "), CO(int), &nbTypes[i], CO(String), "): ", CO(String), nID, null);
   }

   printLn(CO(String), "", null);
   printLn(CO(String), "[", CO(String), crs, CO(String), $("] Vertices ("), CO(int), &nVertices, CO(String), "):", null);

   for(i = 0; i < nVertices; i++)
      printLn(CO(String), "   ", CO(Degrees), &vertices[i].lat, CO(String), ", ", CO(Degrees), &vertices[i].lon, null);
   return 0;
}

static int dggrsInfo(DGGRS dggrs, T(Map, String, constString) options)
{
   int depth64k = DGGRS_get64KDepth(dggrs);
   int ratio = DGGRS_getRefinementRatio(dggrs);
   int maxLevel = DGGRS_getMaxDGGRSZoneLevel(dggrs);

   printLn(CO(String), $("Refinement Ratio: "), CO(int), &ratio, null);
   printLn(CO(String), $("Maximum level for 64-bit global identifiers (DGGAL DGGRSZone): "), CO(int), &maxLevel, null);
   printLn(CO(String), $("Default ~64K sub-zones relative depth: "), CO(int), &depth64k, null);
   return 0;
}

int displayInfo(DGGRS dggrs, DGGRSZone zone, T(Map, String, constString) options)
{
   if(zone != nullZone)
      return zoneInfo(dggrs, zone, options);
   else
      return dggrsInfo(dggrs, options);
}

Class * class_Map_String_constString;

int main(int argc, char * argv[])
{
   Application app = ecrt_init(null, true, false, argc, argv);
   Module mDGGAL = dggal_init(app);
   int exitCode = 0;
   bool showSyntax = false;
   const char * dggrsName = null;
   int a = 1;
   constString zoneID = null;
   T(Map, String, constString) options;

   class_Map_String_constString = eC_findClass(app, "Map<String, const String>");
   options = newi(Map, String, constString);

        if(!strcmpi(argv[0], "i3h")) dggrsName = "ISEA3H";
   else if(!strcmpi(argv[0], "i9r")) dggrsName = "ISEA9R";
   else if(!strcmpi(argv[0], "ggg")) dggrsName = "GNOSISGlobalGrid";

   if(!dggrsName && argc > 1)
   {
           if(!strcmpi(argv[1], "isea3h")) dggrsName = "ISEA3H";
      else if(!strcmpi(argv[1], "isea9r")) dggrsName = "ISEA9R";
      else if(!strcmpi(argv[1], "gnosis")) dggrsName = "GNOSISGlobalGrid";
      a++;
   }

   if(argc > a)
      zoneID = argv[a++];

   while(a < argc)
   {
      const char * key = argv[a++];
      if(key[0] == '-' && a < argc)
      {
         const char * value = argv[a++];
         T(MapIterator, String, constString) it = { options };
         Iterator_index((Iterator *)&it, TAp((void *)(key+1)), true);
         Iterator_setData((Iterator *)&it, TAp((void *)value));
      }
      else
         exitCode = 1, showSyntax = true;
   }

   if(dggrsName && !exitCode)
   {
      DGGRS dggrs = Instance_new(eC_findClass(mDGGAL, dggrsName));
      DGGRSZone zone = nullZone;

      printLn(CO(String), $("DGGRS: https://maps.gnosis.earth/ogcapi/dggrs/"), CO(String), dggrsName, null);

      if(zoneID)
         zone = DGGRS_getZoneFromTextID(dggrs, zoneID);

      displayInfo(dggrs, zone, options);

      deletei(dggrs);
   }
   else
      showSyntax = true, exitCode = 1;

   deletei(options);

   if(showSyntax)
      printLn(CO(String),
         $("Syntax:\n"
         "   info <dggrs> [zone] [options]\n"
         "where dggrs is one of gnosis, isea3h or isea9r\n"), null);
   deletei(app);
   return exitCode;
}
