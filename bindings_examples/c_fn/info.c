#include "dggal_c.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#define Pi 3.1415926535897932384626433832795028841971

#define strcmpi strcasecmp

static int zoneInfo(DGGRS dggrs, DGGRSZone zone, int depthOption)
{
   int level = DGGAL_DGGRS_getZoneLevel(dggrs, zone);
   int nEdges = DGGAL_DGGRS_countZoneEdges(dggrs, zone);
   GeoPoint centroid;
   GeoExtent extent;
   GeoPoint vertices[6];
   int nVertices = DGGAL_DGGRS_getZoneWGS84Vertices(dggrs, zone, vertices);
   char zoneID[256];
   double area = DGGAL_DGGRS_getZoneArea(dggrs, zone), areaKM2 = area / 1000000;
   int depth = DGGAL_DGGRS_get64KDepth(dggrs);
   DGGRSZone parents[3], neighbors[6], children[13];
   int nParents = DGGAL_DGGRS_getZoneParents(dggrs, zone, parents);
   int nbTypes[6];
   int nNeighbors = DGGAL_DGGRS_getZoneNeighbors(dggrs, zone, neighbors, nbTypes);
   int nChildren = DGGAL_DGGRS_getZoneChildren(dggrs, zone, children);
   DGGRSZone centroidParent = DGGAL_DGGRS_getZoneCentroidParent(dggrs, zone);
   DGGRSZone centroidChild = DGGAL_DGGRS_getZoneCentroidChild(dggrs, zone);
   int isCentroidChild = DGGAL_DGGRS_isZoneCentroidChild(dggrs, zone);
   int i;
   constString crs = "EPSG:4326";
   int64_t nSubZones;

   if(depthOption != -1)
   {
      int maxDepth = DGGAL_DGGRS_getMaxDepth(dggrs);
      depth = depthOption;
      if(depth > maxDepth)
      {
         printf("Invalid depth (maximum: %d)\n", maxDepth);
         return 1;
      }
   }

   nSubZones = DGGAL_DGGRS_countSubZones(dggrs, zone, depth);

   DGGAL_DGGRS_getZoneWGS84Centroid(dggrs, zone, &centroid);
   DGGAL_DGGRS_getZoneWGS84Extent(dggrs, zone, &extent);
   DGGAL_DGGRS_getZoneTextID(dggrs, zone, zoneID);

   printf("Textual Zone ID: %s\n", zoneID);
   printf("64-bit integer ID: " "%" PRId64 " (" "0x%" PRIx64 ")\n", zone, zone);
   printf("Level %d zone (%d edges%s)\n", level, nEdges, isCentroidChild ? ", centroid child" : "");
   printf("%f m² (%f km²)\n", area, areaKM2);
   printf("%" PRId64 " sub-zones at depth %d\n", nSubZones, depth);
   printf("WGS84 Centroid (lat, lon): %f, %f\n", centroid.lat * 180 / Pi, centroid.lon * 180 / Pi);
   printf("WGS84 Extent (lat, lon): { %f, %f }, { %f, %f }\n",
      extent.ll.lat * 180 / Pi, extent.ll.lon * 180 / Pi,
      extent.ur.lat * 180 / Pi, extent.ur.lon * 180 / Pi);

   printf("\n");
   if(nParents)
   {
      printf("Parent%s (%d):\n", nParents > 1 ? "s" : "", nParents);
      for(i = 0; i < nParents; i++)
      {
         char pID[256];
         DGGAL_DGGRS_getZoneTextID(dggrs, parents[i], pID);
         printf("   %s", pID);
         if(centroidParent == parents[i])
            printf(" (centroid child)");
         printf("\n");
      }
   }
   else
      printf("No parent\n");

   printf("\n");
   printf("Children (%d):\n", nChildren);
   for(i = 0; i < nChildren; i++)
   {
      char cID[256];
      DGGAL_DGGRS_getZoneTextID(dggrs, children[i], cID);
      printf("   %s", cID);
      if(centroidChild == children[i])
         printf(" (centroid)");
      printf("\n");
   }

   printf("\nNeighbors (%d)\n", nNeighbors);
   for(i = 0; i < nNeighbors; i++)
   {
      char nID[256];
      DGGAL_DGGRS_getZoneTextID(dggrs, neighbors[i], nID);
      printf("   (direction %d): %s\n", nbTypes[i], nID);
   }

   printf("\n[%s] Vertices (%d):\n", crs, nVertices);

   for(i = 0; i < nVertices; i++)
      printf("   %f, %f\n", vertices[i].lat * 180 / Pi, vertices[i].lon * 180 / Pi);
   return 0;
}

static int dggrsInfo(DGGRS dggrs)
{
   int depth64k = DGGAL_DGGRS_get64KDepth(dggrs);
   int ratio = DGGAL_DGGRS_getRefinementRatio(dggrs);
   int maxLevel = DGGAL_DGGRS_getMaxDGGRSZoneLevel(dggrs);

   printf("Refinement Ratio: %d\n", ratio);
   printf("Maximum level for 64-bit global identifiers (DGGAL DGGRSZone): %d\n", maxLevel);
   printf("Default ~64K sub-zones relative depth: %d\n", depth64k);
   return 0;
}

int displayInfo(DGGRS dggrs, DGGRSZone zone, int depthOption)
{
   if(zone != nullZone)
      return zoneInfo(dggrs, zone, depthOption);
   else
      return dggrsInfo(dggrs);
}

int main(int argc, char * argv[])
{
   DGGALModule dggal = DGGAL_init();
   int exitCode = 0;
   int showSyntax = false;
   const char * dggrsName = NULL;
   int a = 1;
   constString zoneID = NULL;
   int depthOption = -1;

        if(!strcmpi(argv[0], "i3h") || !strcmpi(argv[0], "isea3h")) dggrsName = "ISEA3H";
   else if(!strcmpi(argv[0], "i9r") || !strcmpi(argv[0], "isea9r")) dggrsName = "ISEA9R";
   else if(!strcmpi(argv[0], "i4r") || !strcmpi(argv[0], "isea4r")) dggrsName = "ISEA4R";
   else if(!strcmpi(argv[0], "i7h") || !strcmpi(argv[0], "isea7h")) dggrsName = "ISEA7H";
   else if(!strcmpi(argv[0], "iz7") || !strcmpi(argv[0], "isea7h_z7")) dggrsName = "ISEA7H_Z7";

   else if(!strcmpi(argv[0], "r3h") || !strcmpi(argv[0], "rtea3h")) dggrsName = "RTEA3H";
   else if(!strcmpi(argv[0], "r9r") || !strcmpi(argv[0], "rtea9r")) dggrsName = "RTEA9R";
   else if(!strcmpi(argv[0], "r4r") || !strcmpi(argv[0], "rtea4r")) dggrsName = "RTEA4R";
   else if(!strcmpi(argv[0], "r7h") || !strcmpi(argv[0], "rtea7h")) dggrsName = "RTEA7H";
   else if(!strcmpi(argv[0], "rz7") || !strcmpi(argv[0], "rtea7h_z7")) dggrsName = "RTEA7H_Z7";

   else if(!strcmpi(argv[0], "v3h") || !strcmpi(argv[0], "ivea3h")) dggrsName = "IVEA3H";
   else if(!strcmpi(argv[0], "v9r") || !strcmpi(argv[0], "ivea9r")) dggrsName = "IVEA9R";
   else if(!strcmpi(argv[0], "v4r") || !strcmpi(argv[0], "ivea4r")) dggrsName = "IVEA4R";
   else if(!strcmpi(argv[0], "v7h") || !strcmpi(argv[0], "ivea7h")) dggrsName = "IVEA7H";
   else if(!strcmpi(argv[0], "vz7") || !strcmpi(argv[0], "ivea7h_z7")) dggrsName = "IVEA7H_Z7";

   else if(!strcmpi(argv[0], "ggg") || !strcmpi(argv[0], "gnosis")) dggrsName = "GNOSISGlobalGrid";

   else if(!strcmpi(argv[0], "rhp") || !strcmpi(argv[0], "rHEALPix")) dggrsName = "rHEALPix";
   else if(!strcmpi(argv[0], "hpx") || !strcmpi(argv[0], "HEALPix")) dggrsName = "HEALPix";

   if(!dggrsName && argc > 1)
   {
           if(!strcmpi(argv[1], "isea3h")) dggrsName = "ISEA3H";
      else if(!strcmpi(argv[1], "isea9r")) dggrsName = "ISEA9R";
      else if(!strcmpi(argv[1], "isea4r")) dggrsName = "ISEA4R";
      else if(!strcmpi(argv[1], "isea7h_z7")) dggrsName = "ISEA7H_Z7";

      else if(!strcmpi(argv[1], "rtea3h")) dggrsName = "RTEA3H";
      else if(!strcmpi(argv[1], "rtea9r")) dggrsName = "RTEA9R";
      else if(!strcmpi(argv[1], "rtea4r")) dggrsName = "RTEA4R";
      else if(!strcmpi(argv[1], "rtea7h")) dggrsName = "RTEA7H";
      else if(!strcmpi(argv[1], "rtea7h_z7")) dggrsName = "RTEA7H_Z7";

      else if(!strcmpi(argv[1], "ivea3h")) dggrsName = "IVEA3H";
      else if(!strcmpi(argv[1], "ivea9r")) dggrsName = "IVEA9R";
      else if(!strcmpi(argv[1], "ivea4r")) dggrsName = "IVEA4R";
      else if(!strcmpi(argv[1], "ivea7h")) dggrsName = "IVEA7H";
      else if(!strcmpi(argv[1], "ivea7h_z7")) dggrsName = "IVEA7H_Z7";

      else if(!strcmpi(argv[1], "gnosis")) dggrsName = "GNOSISGlobalGrid";

      else if(!strcmpi(argv[1], "rHEALPix")) dggrsName = "rHEALPix";
      else if(!strcmpi(argv[1], "HEALPix")) dggrsName = "HEALPix";
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
         if(!strcmpi(key + 1, "depth"))
            depthOption = strtol(value, NULL, 10);
      }
      else
         exitCode = 1, showSyntax = true;
   }

   if(dggrsName && !exitCode)
   {
      DGGRS dggrs = DGGAL_DGGRS_new(dggal, dggrsName);
      DGGRSZone zone = nullZone;

      printf("DGGRS: https://maps.gnosis.earth/ogcapi/dggrs/%s\n", dggrsName);

      if(zoneID)
         zone = DGGAL_DGGRS_getZoneFromTextID(dggrs, zoneID);

      displayInfo(dggrs, zone, depthOption);

      // Compacted Zones Test
      #if 0
      DGGRS isea4r = DGGAL_DGGRS_new(dggal, "ISEA4R");
      const char * zoneIDs[] = { "B5-0", "B5-1", "B5-2", "B5-3" };
      unsigned int count = sizeof(zoneIDs) / sizeof(zoneIDs[0]);
      Array_DGGRSZone a = DGGAL_Array_DGGRSZone_new(count);
      DGGRSZone * zones = DGGAL_Array_DGGRSZone_getPointer(a);
      int i;

      for(i = 0; i < count; i++)
         zones[i] = DGGAL_DGGRS_getZoneFromTextID(isea4r, zoneIDs[i]);

      DGGAL_DGGRS_compactZones(isea4r, a);
      zones = DGGAL_Array_DGGRSZone_getPointer(a);
      count = DGGAL_Array_DGGRSZone_getCount(a);

      printf("Compacted zones\n");
      for(i = 0; i < count; i++)
      {
         char id[256];
         DGGAL_DGGRS_getZoneTextID(isea4r, zones[i], id);
         printf("   %s\n", id);
      }
      DGGAL_Array_DGGRSZone_delete(a);
      DGGAL_DGGRS_delete(isea4r);
      #endif

      DGGAL_DGGRS_delete(dggrs);
   }
   else
      showSyntax = true, exitCode = 1;

   if(showSyntax)
      printf(
         "Syntax:\n"
         "   info <dggrs> [zone] [options]\n"
         "where dggrs is one of gnosis, isea(4r/9r/3h/7h/7h_z7), ivea(4r/9r/3h/7h/7h_z7), rtea(4r/9r/3h/7h/7h_z7), healpix, rhealpix\n");

   DGGAL_terminate(dggal);
   return exitCode;
}
