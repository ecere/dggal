public import IMPORT_STATIC "ecrt"
private:

import "GeoExtent"

static define epsilon = 1.0e-7;

static inline double tsfnz_0e(Radians phi)
{
   return tan((Pi/2 - phi) / 2);
}

static inline Radians phi2z_0e(double ts)
{
   return Pi/2 - 2 * atan(ts);
}

public class WebMercatorProjection
{
   GeoPoint center;

   Degrees minMaxLat;
   minMaxLat = phi2z_0e(exp(-Pi)); // 85.0511287798066

   public virtual bool forward(const GeoPoint p, Pointd v)
   {
      // WARNING: output is in radians -- not meters
      bool result = true;
      Radians lat = p.lat;

      if(lat < -Pi/2 + Radians { epsilon })
         lat = -Pi/2, result = lat < -Pi/2-Radians { epsilon };
      else if(lat > Pi/2 - Radians { epsilon })
         lat = Pi/2, result = lat > +Pi/2+Radians { epsilon };
      v =
      {
         x = wrapLonAt(-1, p.lon, center.lon),
             // Work around to avoid odd 0.0000000007081 EPSG:3857 projection of lat=0 reporter by P. Rushforth
         y = lat ? -log(tsfnz_0e(lat)) : 0
      };
      return result;
   }

   public virtual bool inverse(const Pointd v, GeoPoint result, bool oddGrid)
   {
      // WARNING: input is in radians -- not meters
      result =
      {
         lat = phi2z_0e(exp(-v.y)),
         lon = wrapLon(center.lon + v.x)
      };
      return true;
   }
}

public class TransverseMercatorProjection
{
   GeoPoint center;

   Degrees minMaxLat;
   minMaxLat = phi2z_0e(exp(-Pi)); // 85.0511287798066

   public virtual bool forward(const GeoPoint p, Pointd v)
   {
      bool result = true;
      Radians lat = p.lat, lon = p.lon;

      v.x = atanh(cos(lat) * sin(lon));
      v.y = atan2(tan(lat), cos(lon));
      return result;
   }

   public virtual bool inverse(const Pointd v, GeoPoint result, bool oddGrid)
   {
      double x = v.x, y = v.y;

      double sinhX = sinh(x), coshX = cosh(x), sinY  = sin(y), cosY  = cos(y);
      result = { asin(sinY / coshX), lon = atan2(sinhX, cosY) };
      return true;
   }
}
