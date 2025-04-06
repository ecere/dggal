public import IMPORT_STATIC "ecrt"
private:

import "GeoExtent"
import "authalic"

public class HEALPixProjection
{
   double cp[2][AUTH_ORDER];

   HEALPixProjection()
   {
      authalicSetup(wgs84Major, wgs84Minor, cp);
   }

   Radians latAuthalicToGeodetic(Radians phi)
   {
      return applyCoefficients(cp[1], phi);
   }

   Radians latGeodeticToAuthalic(Radians phi)
   {
      return applyCoefficients(cp[0], phi);
   }

   public virtual bool forward(const GeoPoint p, Pointd v)
   {
      double sinLat = sin(latGeodeticToAuthalic(p.lat));
      if(fabs(sinLat) <= 2/3.0)
      {
         // Equatorial regions
         v.x = (Radians)p.lon;
         v.y = 3 * Pi/8 * sinLat;
      }
      else
      {
         // Polar caps
         int n = (Radians)p.lon > Pi/2 ? 3 : (Radians)p.lon > 0 ? 2 : (Radians)p.lon > -Pi / 2 ? 1 : 0;
         double s = sqrt(1 - fabs(sinLat));
         Radians dLon = (Radians)p.lon - (-Pi + Pi/2 * n);
         double width = sqrt(3) * Pi/2 * s;
         v.x = -Pi + Pi/2 * n + (Pi/2 - width) / 2 + dLon * width / (Pi/2);
         v.y = Sgn((Radians)p.lat) * (Pi - width) / 2;
      }
      return true;
   }

   public virtual bool inverse(const Pointd v, GeoPoint result, bool oddGrid)
   {
      bool r = true;
      if(fabs(v.y) <= Pi/4)
      {
         // Equatorial region
         result.lon = (Radians)v.x;
         result.lat = latAuthalicToGeodetic(asin(8 * v.y / (3 * Pi)));
      }
      else
      {
         // Polar caps
         int n = (Radians)v.x > Pi/2 ? 3 : (Radians)v.x > 0 ? 2 : (Radians)v.x > -Pi / 2 ? 1 : 0;
         double width = Pi - 2*fabs(v.y);
         if(width > 1E-15)
         {
            double s = 2 * width / (sqrt(3) * Pi);
            Radians dLon = (v.x - (- Pi + Pi/2 * n + (Pi/2 - width) / 2)) * Pi/2 / width;
            if(dLon < 0 || dLon > Pi/2 + 1E-15)
               r = false;
            result.lat = Sgn(v.y) * latAuthalicToGeodetic(asin(1 - s*s));
            result.lon = dLon + (-Pi + Pi/2 * n);
         }
         else
            result = { Sgn(v.y) * 90, 0 };
      }
      return r;
   }
}

public class rHEALPixProjection : HEALPixProjection
{
   public virtual bool forward(const GeoPoint p, Pointd v)
   {
      int sgn;
      double y;

      HEALPixProjection::forward(p, v);

      sgn = v.y < 0 ? -1 : 1;
      y = v.y * sgn;
      if(y <= Pi/4)
         // Equatorial region
         return true;
      else
      {
         // Polar caps
         int n = (Radians)p.lon > Pi/2 ? 3 : (Radians)p.lon > 0 ? 2 : (Radians)p.lon > -Pi / 2 ? 1 : 0;
         switch(n)
         {
            case 0: return true;
            case 1: v = { -Pi/4 - y, sgn * (v.x + 3*Pi/4) }; break;
            case 2: v = { - Pi/2 - v.x, sgn * (Pi - y) }; break;
            case 3: v = { y - 5*Pi/4, sgn * (5*Pi/4 - v.x) }; break;
         }
      }
      return true;
   }

   int getPolarSection(const Pointd v)
   {
      int n = -1;
      int sgn = v.y < 0 ? -1 : 1;
      double y = v.y * sgn;
      if(y >= Pi/4 && v.x <= -Pi/2)
      {
         double dx = Pi + v.x, dy = 3*Pi/4 - y, mdx = Pi/2 - dx;
         // The epsilon here helps with detecting crossing dateline
         n = (dx > dy) ? (mdx > dy ? 2 : 1) : (mdx > dy + 1E-15 ? 3 : 0);
      }
      return n;
   }

   public virtual bool inverse(const Pointd v, GeoPoint result, bool oddGrid)
   {
      int sgn = v.y < 0 ? -1 : 1;
      double y = v.y * sgn;
      if(y <= Pi/4 + 1E-15)
         // Equatorial region
         return HEALPixProjection::inverse(v, result, oddGrid);
      else if(v.x <= -Pi/2)
      {
         // Polar caps
         double dx = Pi + v.x, dy = 3*Pi/4 - y, mdx = Pi/2 - dx;
         int n = (dx > dy) ? (mdx > dy ? 2 : 1) : (mdx > dy ? 3 : 0);
         Pointd vv;

         switch(n)
         {
            case 0: vv = v; break;
            case 1: vv = { y - 3*Pi/4, sgn * (-Pi/4 - v.x) }; break;
            case 2: vv = { -v.x - Pi/2, sgn * (Pi - y) }; break;
            case 3: vv = { 5*Pi/4 - y, sgn * (5*Pi/4 + v.x) }; break;
         }
         return HEALPixProjection::inverse(vv, result, oddGrid);
      }
      return false;
   }
}
