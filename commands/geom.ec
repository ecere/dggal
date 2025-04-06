public import IMPORT_STATIC "ecrt"
import IMPORT_STATIC "dggal"

CRS resolveCRSString(const String crsOption)
{
   CRS crs = 0;
   if(crsOption)
   {
      // NOTE: Currently re-using the same CRS identifiers regardless of actual projection for 5x6 and icosahedron net space
           if(!strcmpi(crsOption, "5x6" )) crs = { ogc, 153456 };
      else if(!strcmpi(crsOption, "ico") ||
             !strcmpi(crsOption, "isea") ||
             !strcmpi(crsOption, "ivea") ||
             !strcmpi(crsOption, "rtea")) crs = { ogc, 1534 };
      else if(!strcmpi(crsOption, "OGC:CRS84")) crs = { ogc, 84 };
      else if(!strcmpi(crsOption, "EPSG:4326")) crs = { epsg, 4326 };
      else if(!strcmpi(crsOption, "rhp")) crs = { ogc, 99999 };
   }
   return crs;
}

void generateZoneFeature(DGGRS dggrs, DGGRSZone zone, CRS crs, int64 id, bool centroids, bool fc, Map<String, FieldValue> properties)
{
   char zoneID[256];
   const String t = fc ? "   " : "";

   dggrs.getZoneTextID(zone, zoneID);

   PrintLn("{");
   PrintLn(t, "   \"type\" : \"Feature\",");
   Print  (t, "   \"id\" : ");
   if(id)
      Print(id);
   else
      printf("\"%s\"", zoneID);
   PrintLn(",");

   generateZoneGeometry(dggrs, zone, crs, id, centroids, fc);

   PrintLn(",");

   PrintLn(t, "   \"properties\" : {");
   Print  (t, "     \"zoneID\" : \"");
   printf("%s", zoneID);
   Print("\"");
   if(properties)
   {
      for(p : properties)
      {
         const String key = &p;
         FieldValue v = p;
         Print(",\n", t, "     \"", key, "\" : ", v);
      }
   }

   PrintLn("");
   PrintLn(t, "   }");
   Print(t, "}");
}

void generateZoneGeometry(DGGRS dggrs, DGGRSZone zone, CRS crs, int64 id, bool centroids, bool fc)
{
   const String t = fc ? "   " : "";

   PrintLn(t, "   \"geometry\" : {");
   PrintLn(t, "      \"type\" : \"", centroids ? "Point" : "Polygon", "\",");
   Print  (t, "      \"coordinates\" : [");

   if(!crs || crs == { ogc, 84 } || crs == { epsg, 4326 })
   {
      if(centroids)
      {
         GeoPoint centroid;
         dggrs.getZoneWGS84Centroid(zone, centroid);
         Print(" ", centroid.lon, ", ", centroid.lat);
      }
      else
      {
         Array<GeoPoint> vertices = dggrs.getZoneRefinedWGS84Vertices(zone, 0);
         if(vertices)
         {
            int count = vertices.count, i;

            PrintLn("");
            Print(t, "         [ ");
            for(i = 0; i < count; i++)
               Print(i ? ", " : "", "[", vertices[i].lon, ", ", vertices[i].lat, "]");
            Print(i ? ", " : "", "[", vertices[0].lon, ", ", vertices[0].lat, "]");
            PrintLn(" ]");
         }
         delete vertices;
         Print(t, "     ");
      }
   }
   else
   {
      if(centroids)
      {
         Pointd centroid;
         dggrs.getZoneCRSCentroid(zone, crs, centroid);
         Print(" ", centroid.x, ", ", centroid.y);
      }
      else
      {
         Array<Pointd> vertices = dggrs.getZoneRefinedCRSVertices(zone, crs, 0);
         if(vertices)
         {
            int count = vertices.count, i;

            PrintLn("");
            Print(t, "         [ ");
            for(i = 0; i < count; i++)
               Print(i ? ", " : "", "[", vertices[i].x, ", ", vertices[i].y, "]");
            Print(i ? ", " : "", "[", vertices[0].x, ", ", vertices[0].y, "]");
            PrintLn(" ]");
         }
         delete vertices;
         Print(t, "     ");
      }
   }
   PrintLn(" ]");
   Print(t, "   }");
}

int generateGeometry(DGGRS dggrs, DGGRSZone zone, Map<String, const String> options)
{
   if(zone != nullZone)
   {
      bool centroids = options && options["centroids"] != null;
      const String crsOption = options ? options["crs"] : null;
      CRS crs = resolveCRSString(crsOption);

      generateZoneFeature(dggrs, zone, crs, 0, centroids, false, null);
      PrintLn("");
      return 0;
   }
   else
      PrintLn($"geom command requires a zone");
   return 1;
}
