public import IMPORT_STATIC "ecrt"
import IMPORT_STATIC "dggal"

bool parseBBox(Map<String, const String> options, GeoExtent bbox)
{
   bool result = true;
   const String bboxOption = options ? options["bbox"] : null;
   if(bboxOption)
   {
      String s = CopyString(bboxOption);
      String tokens[4];
      int nTokens = TokenizeWith(s, 4, tokens, ",", false);
      double a,b,c,d;
      if(nTokens == 4 &&
         a.OnGetDataFromString(tokens[0]) &&
         b.OnGetDataFromString(tokens[1]) &&
         c.OnGetDataFromString(tokens[2]) &&
         d.OnGetDataFromString(tokens[3]) &&
         a < 90 && a >= -90 && c <= 90 && c > -90)
         bbox = { { a, b }, { c, d } };
      else
      {
         PrintLn($"Invalid bounding box specified");
         result = false;
      }
      delete s;
   }
   return result;
}

int listZones(DGGRS dggrs, int level, Map<String, const String> options)
{
   int exitCode = 0;
   bool centroids = options && options["centroids"] != null;
   bool compact = options && options["compact"] != null;
   GeoExtent bbox = wholeWorld;

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
      int64 i = 0;
      Array<DGGRSZone> zones = dggrs.listZones(level, bbox);
      if(compact)
         dggrs.compactZones(zones);

      Print("[");
      if(zones)
         for(z : zones)
         {
            Print(i > 0 ? ", " : " ");
            if(centroids)
            {
               GeoPoint centroid;
               dggrs.getZoneWGS84Centroid(z, centroid);
               Print("[ ", centroid.lat, ", ", centroid.lon, " ]");
            }
            else
            {
               char zoneID[256];
               dggrs.getZoneTextID(z, zoneID);
               Print("\"", zoneID, "\"");
            }
            i++;
         }
      PrintLn(" ]");
      delete zones;
   }
   return 0;
}
