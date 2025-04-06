public import IMPORT_STATIC "ecrt"
import IMPORT_STATIC "dggal"
import "geom"

int subZones(DGGRS dggrs, DGGRSZone zone, int64 index, Map<String, const String> options)
{
   int exitCode = 1;
   int depth = dggrs.get64KDepth();
   const String depthOption = options ? options["depth"] : null;
   bool centroids = options && options["centroids"] != null;
   const String crsOption = options ? options["crs"] : null;
   CRS crs = resolveCRSString(crsOption);

   if(depthOption)
   {
      int maxDepth = index == -1 ? dggrs.getMaxDepth() : dggrs.getIndexMaxDepth();

      depth.OnGetDataFromString(depthOption);
      if(depth > maxDepth)
      {
         PrintLn($"Invalid depth (maximum: ", maxDepth, ")");
         return 1;
      }
   }

   if(zone != nullZone)
   {
      char zoneID[256], subZoneID[256];
      DGGRSZone zoneAtIndex = index == -1 ? nullZone : dggrs.getSubZoneAtIndex(zone, depth, index);

      dggrs.getZoneTextID(zone, zoneID);

      if(zoneAtIndex == nullZone && index != -1)
         PrintLn($"Invalid zone index for zone ", zoneID, $" at depth ", depth, " (", dggrs.countSubZones(zone, depth), $" sub-zones)");
      else if(zoneAtIndex != nullZone)
      {
         if(centroids)
         {
            if(crs)
            {
               Pointd centroid;
               dggrs.getZoneCRSCentroid(zoneAtIndex, crs, centroid);
               Print("[", centroid.x, ", ", centroid.y, "]");
            }
            else
            {
               GeoPoint centroid;
               dggrs.getZoneWGS84Centroid(zoneAtIndex, centroid);
               Print("[", centroid.lat, ", ", centroid.lon, "]");
            }
         }
         else
         {
            dggrs.getZoneTextID(zoneAtIndex, subZoneID);
            PrintLn("\"", subZoneID, "\"");
         }
         exitCode = 0;
      }
      else
      {
         Array<DGGRSZone> subZones = dggrs.getSubZones(zone, depth);
         if(subZones)
         {
            int i = 0;
            Print("[ ");
            for(i = 0; i < subZones.count; i++)
            {
               if(i) Print(", ");
               if(centroids)
               {
                  if(crs)
                  {
                     Pointd centroid;
                     dggrs.getZoneCRSCentroid(subZones[i], crs, centroid);
                     Print("[", centroid.x, ", ", centroid.y, "]");
                  }
                  else
                  {
                     GeoPoint centroid;
                     dggrs.getZoneWGS84Centroid(subZones[i], centroid);
                     Print("[", centroid.lat, ", ", centroid.lon, "]");
                  }
               }
               else
               {
                  dggrs.getZoneTextID(subZones[i], subZoneID);
                  Print("\"", subZoneID, "\"");
               }
            }
            PrintLn(" ]");
         }
         else
            PrintLn("null");
         exitCode = 0;
      }
   }
   else
      PrintLn($"sub command requires a zone");
   return exitCode;
}
