public import IMPORT_STATIC "ecrt"
import IMPORT_STATIC "dggal"

import "geom"
import "list" // for bbox parsing

int generateGrid(DGGRS dggrs, int level, Map<String, const String> options)
{
   int exitCode = 0;
   bool centroids = options && options["centroids"] != null;
   bool compact = options && options["compact"] != null;
   GeoExtent bbox = wholeWorld;
   int64 id = 1;
   const String crsOption = options ? options["crs"] : null;
   CRS crs = resolveCRSString(crsOption);

   if(!parseBBox(options, bbox))
      exitCode = 1;
   if(compact && centroids)
   {
      exitCode = 1;
      PrintLn($"Cannot return compact list of zones as centroids");
   }

   if(level == -1)
      level = 0;

   if(!exitCode)
   {
      Array<DGGRSZone> zones = dggrs.listZones(level, bbox);
      if(zones)
      {
         if(compact)
            dggrs.compactZones(zones);

         PrintLn("{");
         PrintLn("   \"type\": \"FeatureCollection\",");
         Print  ("   \"features\": [ ");
         for(z : zones)
         {
            Print(id > 1 ? ", " : "   ");
            generateZoneFeature(dggrs, z, crs, id++, centroids, true, null);
         }
         PrintLn(" ]");
         PrintLn("}");

         delete zones;
      }
   }
   return 0;
}
