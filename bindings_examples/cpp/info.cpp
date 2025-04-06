#include <dggal.hpp>

// For looking up internationalized strings
#define MODULE_NAME "info"

static int zoneInfo(DGGRS & dggrs, DGGRSZone zone, TMap<constString, constString> & options)
{
   int level = dggrs.getZoneLevel(zone);
   int nEdges = dggrs.countZoneEdges(zone);
   GeoPoint centroid;
   GeoExtent extent;
   GeoPoint vertices[6];
   int nVertices = dggrs.getZoneWGS84Vertices(zone, vertices);
   char zoneID[256];
   double area = dggrs.getZoneArea(zone), areaKM2 = area / 1000000;
   int depth = dggrs.get64KDepth();
   DGGRSZone parents[3], neighbors[6], children[9];
   int nParents = dggrs.getZoneParents(zone, parents);
   int nbTypes[6];
   int nNeighbors = dggrs.getZoneNeighbors(zone, neighbors, nbTypes);
   int nChildren = dggrs.getZoneChildren(zone, children);
   DGGRSZone centroidParent = dggrs.getZoneCentroidParent(zone);
   DGGRSZone centroidChild = dggrs.getZoneCentroidChild(zone);
   bool isCentroidChild = dggrs.isZoneCentroidChild(zone);
   int i;
   constString crs = "EPSG:4326";
   int64 nSubZones;
   constString depthOption = null;

   TMapIterator<constString, constString> it;
   it.map = options;
   if(it.index("depth", false))
      depthOption = it.value;

   if(depthOption)
   {
      int maxDepth = dggrs.getMaxDepth();
      _onGetDataFromString(CO(int), &depth, depthOption);
      if(depth > maxDepth)
      {
         printLn($("Invalid depth (maximum: "), maxDepth, ")");
         return 1;
      }
   }

   nSubZones = dggrs.countSubZones(zone, depth);

   dggrs.getZoneWGS84Centroid(zone, centroid);
   dggrs.getZoneWGS84Extent(zone, extent);
   dggrs.getZoneTextID(zone, zoneID);

   printLn($("Textual Zone ID: "), zoneID);
   printx($("64-bit integer ID: "), (uint64)zone, " ("); // FIXME: It would be nice not needing this cast

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat" // FORMAT64HEX contains runtime checks for platform
   printf(FORMAT64HEX, zone);
#pragma GCC diagnostic pop
   printLn(")");
   printLn("");
   printLn($("Level "), level, $(" zone ("), nEdges, $(" edges"),
      isCentroidChild ? $(", centroid child)") : ")");
   printLn(area, " m² (", areaKM2, " km²)");
   printLn(nSubZones, $(" sub-zones at depth "), depth);
   printLn($("WGS84 Centroid (lat, lon): "), centroid.lat, ", ", centroid.lon);
   printLn($("WGS84 Extent (lat, lon): { "),
      // FIXME: It would be nice not needing this GeoPoint cast
      ((GeoPoint)extent.ll).lat, ", ", ((GeoPoint)extent.ll).lon, " }, { ",
      ((GeoPoint)extent.ur).lat, ", ", ((GeoPoint)extent.ur).lon, " }");

   printLn("");
   if(nParents)
   {
      printLn($("Parent"), nParents > 1 ? "s" : "", " (", nParents, "):");
      for(i = 0; i < nParents; i++)
      {
         char pID[256];
         dggrs.getZoneTextID(parents[i], pID);
         printx("   ", pID);
         if(centroidParent == parents[i])
            printx($(" (centroid child)"));
         printLn("");
      }
   }
   else
      printLn($("No parent"));

   printLn("");
   printLn($("Children ("), nChildren, "):");
   for(i = 0; i < nChildren; i++)
   {
      char cID[256];
      dggrs.getZoneTextID(children[i], cID);
      printx("   ", cID);
      if(centroidChild == children[i])
         printx($(" (centroid)"));
      printLn("");
   }

   printLn("");
   printLn($("Neighbors ("), nNeighbors, "):");
   for(i = 0; i < nNeighbors; i++)
   {
      char nID[256];
      dggrs.getZoneTextID(neighbors[i], nID);
      printLn($("   (direction "), nbTypes[i], "): ", nID);
   }

   printLn("");
   printLn("[", crs, $("] Vertices ("), nVertices, "):");

   for(i = 0; i < nVertices; i++)
      printLn("   ", vertices[i].lat, ", ", vertices[i].lon);

   return 0;
}

static int dggrsInfo(DGGRS & dggrs, TMap<constString, constString> & options)
{
   int depth64k = dggrs.get64KDepth();
   int ratio = dggrs.getRefinementRatio();
   int maxLevel = dggrs.getMaxDGGRSZoneLevel();

   printLn($("Refinement Ratio: "), ratio);
   printLn($("Maximum level for 64-bit global identifiers (DGGAL DGGRSZone): "), maxLevel);
   printLn($("Default ~64K sub-zones relative depth: "), depth64k);
   return 0;
}

int displayInfo(DGGRS & dggrs, DGGRSZone zone, TMap<constString, constString> & options)
{
   if(zone != nullZone)
      return zoneInfo(dggrs, zone, options);
   else
      return dggrsInfo(dggrs, options);
}

class DGGALInfoApp : Application
{
public:
   eC_Module mDGGAL;

   DGGALInfoApp() : Application(ecrt_init(null, true, false, null, null))
   {
      mDGGAL = dggal_init(__thisModule);
      dggal_cpp_init(Module(__thisModule));
   }
};

int main(int argc, char * argv[])
{
   int exitCode = 0;
   DGGALInfoApp app;

   bool showSyntax = false;
   const char * dggrsName = null;
   int a = 1;
   constString zoneID = null;

   TMap<constString, constString> options;

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
         TMapIterator<constString, constString> it;
         it.map = options;
         it.index(key + 1, true);
         it.value = (char *)value;
      }
      else
         exitCode = 1, showSyntax = true;
   }

   if(dggrsName && !exitCode)
   {
      DGGRS * dggrs = new DGGRS((eC_Instance)newi(eC_findClass(app.mDGGAL, dggrsName)));
      DGGRSZone zone = nullZone;

      printLn($("DGGRS: https://maps.gnosis.earth/ogcapi/dggrs/"), dggrsName);

      if(zoneID)
         zone = dggrs->getZoneFromTextID(zoneID);

      displayInfo(*dggrs, zone, options);
      delete dggrs;
   }
   else
      showSyntax = true, exitCode = 1;

   if(showSyntax)
      printLn($("Syntax:\n"
         "   info <dggrs> [zone] [options]\n"
         "where dggrs is one of gnosis, isea3h or isea9r\n"));
   return exitCode;
}
