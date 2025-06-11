public import IMPORT_STATIC "ecrt"
import IMPORT_STATIC "dggal"
import "geom"

int convertToGeoJSON(const String inputFile, Map<String, const String> options)
{
   int exitCode = 1;
   File f = FileOpen(inputFile, read);
   if(f)
   {
      DGGSJSON dggsJSON = readDGGSJSON(f);
      if(dggsJSON)
      {
         char dggrsID[256];
         subclass(DGGRS) dggrsClass = null;

         GetLastDirectory(dggsJSON.dggrs, dggrsID);

         if(!strncasecmp(dggrsID, "GNOSIS", 6)) dggrsClass = class(GNOSISGlobalGrid);
         else if(!strncasecmp(dggrsID, "ISEA3H", 6))   dggrsClass = class(ISEA3H);
         else if(!strncasecmp(dggrsID, "ISEA9R", 6))   dggrsClass = class(ISEA9R);
         else if(!strncasecmp(dggrsID, "IVEA3H", 6))   dggrsClass = class(IVEA3H);
         else if(!strncasecmp(dggrsID, "IVEA9R", 6))   dggrsClass = class(IVEA9R);
         else if(!strncasecmp(dggrsID, "RTEA3H", 6))   dggrsClass = class(RTEA3H);
         else if(!strncasecmp(dggrsID, "RTEA9R", 6))   dggrsClass = class(RTEA9R);
         else if(!strncasecmp(dggrsID, "rHEALPix", 8))   dggrsClass = class(rHEALPix);

         if(dggrsClass)
         {
            const String zoneID = dggsJSON.zoneId;
            DGGRS dggrs = eInstance_New(dggrsClass);
            DGGRSZone zone = dggrs.getZoneFromTextID(zoneID);

            if(zone != nullZone)
            {
               if(dggsJSON.depths)
               {
                  int d;
                  int maxDepth = -1;

                  for(d = 0; d < dggsJSON.depths.count; d++)
                  {
                     int depth = dggsJSON.depths[d];
                     if(depth > maxDepth)
                     {
                        maxDepth = depth;
                        break;
                     }
                  }
                  if(d < dggsJSON.depths.count)
                  {
                     int depth = maxDepth;
                     Array<DGGRSZone> subZones = dggrs.getSubZones(zone, depth);
                     bool centroids = options && options["centroids"] != null;
                     const String crsOption = options ? options["crs"] : null;
                     CRS crs = resolveCRSString(crsOption);

                     if(subZones)
                     {
                        int64 i = 0;
                        Map<String, FieldValue> props { };

                        PrintLn("{");
                        PrintLn("   \"type\": \"FeatureCollection\",");
                        Print  ("   \"features\": [ ");

                        for(z : subZones)
                        {
                           props.Free();
                           Print(i > 0 ? ", " : "   ");

                           for(prop : dggsJSON.values)
                           {
                              const String key = &prop;
                              Array<DGGSJSONDepth> depths = prop;

                              if(key && depths && depths.count > d)
                              {
                                 DGGSJSONDepth djDepth = depths[d];
                                 Array<FieldValue> data = (Array<FieldValue>)djDepth.data;
                                 props[key] = data[(uint)i];
                              }
                           }

                           generateZoneFeature(dggrs, z, crs, i + 1, centroids, true, props);

                           i++;
                        }

                        props.Free();
                        delete props;
                        PrintLn(" ]");
                        PrintLn("}");
                     }
                  }
               }
            }
            else
               PrintLn($"Invalid zone ID: ", zoneID);

            delete dggrs;
         }
         else
            PrintLn($"Failure to recognize DGGRS");
         delete dggsJSON;
      }
      else
         PrintLn($"Failure to parse DGGS-JSON file ", inputFile);
      delete f;
   }
   else
      PrintLn($"Failure to open file ", inputFile);
   return exitCode;
}
