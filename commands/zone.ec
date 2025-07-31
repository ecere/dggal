public import IMPORT_STATIC "ecrt"
import IMPORT_STATIC "dggal"
import IMPORT_STATIC "GeoExtents"

import "info"
import "geom"  // For resolveCRSString()

int queryZone(DGGRS dggrs, const String coordinates, int level, Map<String, const String> options)
{
   int exitCode = 1;
   String coords[2];
   String s = CopyString(coordinates);
   int n = s ? TokenizeWith(s, 2, coords, ",", false) : 0;
   Degrees lat, lon;
   if(n == 2 && lat.OnGetDataFromString(coords[0]) && lon.OnGetDataFromString(coords[1]) &&
                lat <= Degrees {90} && lat >= Degrees {-90})
   {
      DGGRSZone zone;
      const String crsOption = options ? options["crs"] : null;
      CRS crs = resolveCRSString(crsOption);

      if(level == -1)
         level = 0;

      if(crs)
         zone = dggrs.getZoneFromCRSCentroid(level, crs, { lat, lon });
      else
         zone = dggrs.getZoneFromCRSCentroid(level, { epsg, 4326 }, { lat, lon });
      if(zone != nullZone)
      {
         displayInfo(dggrs, zone, options);
         exitCode = 0;
      }
      else
         PrintLn($"Could not identify zone from coordinates");
   }
   else if(coordinates)
      PrintLn($"Invalid coordinates for zone query");
   else
      PrintLn($"Missing coordinates for zone query");
   delete s;
   return exitCode;
}
