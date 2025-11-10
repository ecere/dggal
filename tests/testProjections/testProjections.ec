import "dggal"

class App : Application
{
   void Main()
   {
      // SliceAndDiceGreatCircleIcosahedralProjection ivea { };
      // RTEAProjection ivea { };
      ISEAProjection ivea { };
      int i, j;
      Radians maxDLat = 0, maxDLon = 0;
      Time startTime = GetTime(), endTime;

      for(i = -18000; i <= 18000; i++)
      {
         for(j = -9000; j <= 9000; j++)
         {
            GeoPoint geo { j / 100.0, i / 100.0 };

            Pointd p { -999, -999 };

            geo.lat += GetRandom(0, 10000) / 1000000.0;
            geo.lon += GetRandom(0, 10000) / 1000000.0;
            if((Radians)geo.lat > (Radians)Degrees { 90 }) geo.lat = 90;
            if((Radians)geo.lon > (Radians)Degrees { 180 }) geo.lon = 180;

            //geo = { -32.304465, -77.042623 }; // ISEA

            //geo = { 0.00000000035136977, -0.0062499800021024 };
            //geo = { 89.9994, -179.63132 };
            //geo = { 89.929742, 11.200041 };
            //geo = { 89.999997, -178.815156 };
            //geo = { -88.588346, -168.800054 };
            //geo = { -88.588345998998, -168.800054 };
            //geo = { -83.552797, -168.800004 };
            //geo = { 89.99977, -168.824602 };
            //geo = { 89.929742, 11.200041 };
            //geo = { 0.188382, -78.8 };
            //geo = { -33.764607,       93.125187 };
            //geo = { -64.600688, -179.99351 };
            //geo = { 89.999997, -178.815156 };
            //geo = { -64.582619, -179.979945 };
            //geo = { -56.13751, -178.962374 };
            //geo = { 36.108603, 131.114345 };
            //geo = { 0.188382, -78.8 };
            //geo = { -64.600688, -179.99351 };
            //geo = { -59.758218, -179.995962 };
            //geo = { 89.999989, -179.527249 };
            //geo = { 89.999997, -178.815156 };
            //geo = { -47.328938, -179.990413 };
            //geo = { 31.791509, -78.652105 };

            if(ivea.forward(geo, p))
            {
               bool newMax = false;
               Radians dLat, dLon;
               GeoPoint g { };

               if(!ivea.inverse(p, g, false))
               {
                  ivea.forward(geo, p);
                  ivea.inverse(p, g, false);
                  PrintLn("Failed to inverse project ", p, " to ", geo);
               }

               dLat = fabs((Radians)g.lat - (Radians)geo.lat);
               dLon = fabs((Radians)g.lon - (Radians)geo.lon);
               if(dLon > Pi) dLon -= 2 * Pi;
               dLon *= cos(geo.lat);

               if(dLon > maxDLon)
                  maxDLon = dLon, newMax = true;
               if(dLat > maxDLat)
                  maxDLat = dLat, newMax = true;

               if(newMax)/* && (dLat > (Radians)Degrees { 0.000000001 } ||
                             dLon > (Radians)Degrees { 0.000000001 }))*/
               {
                  PrintLn("Input: ", geo);
                  PrintLn("5x6: ", p);
                  PrintLn("Output: ", g);

                  PrintLn("dlat: ", (double)(dLat * wgs84Major * 1000), " mm");
                  PrintLn("dlon: ", (double)(dLon * wgs84Major * 1000), " mm");
               }
            }
            else
               PrintLn("Failed forward projection");
         }
      }
      endTime = GetTime();

      PrintLn("648,054,001 projection roundtrips in ", endTime - startTime);

      PrintLn("dlat: ", (double)(maxDLat * wgs84Major * 1000), " mm");
      PrintLn("dlon: ", (double)(maxDLon * wgs84Major * 1000), " mm");

      delete ivea;
   }
}
