public import IMPORT_STATIC "ecrt"

import IMPORT_STATIC "dggal"

define epsilonCentroid = 1E-5; // 1E-11; // REVIEW: Why do automated builds have such a large delta?
define epsilonExtent = 1E-5;//8; // 1E-11;
define epsilonExtentPole = 1E-5; //1E-3;
define epsilonArea = 1E-6;

define earthSurfaceArea = 5.100656217240885092949E14;

struct DGGSTestCase
{
   const String dggrs;
   const String zoneID;
   DGGRSZone key;
   GeoPoint centroid;
   GeoExtent wgs84Extent;
   const String centroidParent;
   const String centroidChild;
   Array<const String> parents;
   Array<const String> children;
   Array<const String> neighbors;
   double area;
   Map<int, const String> firstSubZones; // Depth to ID
   Map<int, Array<const String>> subZones; // Depth to list of zones

   void OnFree()
   {
      delete parents;
      delete children;
      delete neighbors;
   }
};
