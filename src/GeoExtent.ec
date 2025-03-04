public import IMPORT_STATIC "ecere"

private:

#include <float.h>

public define wholeWorld = GeoExtent { { -90, -180 }, { 90, 180 } };

public define wgs84InvFlattening = 298.257223563;
public define wgs84Major = Meters { 6378137.0 };
public define wgs84Minor = wgs84Major - (wgs84Major / wgs84InvFlattening); // 6356752.3142451792955399

static define epsilon = 1.0e-7;
define radEpsilon = Radians { 10 * DBL_EPSILON };

public enum CRSRegistry { epsg, ogc };

public class CRS : uint64
{
public:
   CRSRegistry registry:30;
   int crsID:32;
   bool h:1;
};

Radians wrapLon(Radians x)
{
   if(x < -Pi - radEpsilon)
      x += (2*Pi) * (floor((Pi - x) / (2*Pi)));
   else if(x > Pi + radEpsilon)
      x -= (2*Pi) * (floor((x + Pi) / (2*Pi)));
	return x;
}

Radians wrapLonAt(int q, Radians lon, Radians cLon)
{
   lon -= cLon;
   if(lon < -Pi - Radians { epsilon })
      lon += (2*Pi) * floor((Pi - lon) / (2*Pi));
   else if(lon > Pi + Radians { epsilon })
      lon -= (2*Pi) * floor((lon + Pi) / (2*Pi));
   if(q != -1 && (lon > Pi/2 || lon < -Pi/2))
   {
      Radians testLon = (-Pi + q*Pi/2+Pi/4);
      Radians wrap = wrapLon(testLon - cLon);
      int lh = (int)((wrap + Pi) * 4 / (2*Pi));
      if(lh == 0 && lon > 0)
         lon -= 2*Pi;
      else if(lh == 3 && lon < 0)
         lon += 2*Pi;
   }
	return lon;
}

void wrapCRS84Points(int count, GeoPoint * points, Radians lon)
{
   if(points)
   {
      int lonQuad = (int)(((Radians)lon + Pi) * (4 / (2*Pi)));
      int i;
      for(i = 0; i < count; i++)
         points[i].lon = wrapLonAt(lonQuad, points[i].lon, 0);
   }
}

public struct GeoPoint
{
   Degrees lat, lon;

   int OnCompare(GeoPoint b) //TODO: This doesn't give particularly detailed information about the values
   {
      if(lat < b.lat) return -1;
      if(lat > b.lat) return  1;
      if(lon < b.lon) return -1;
      if(lon > b.lon) return  1;
      return 0;
   }

   // TOFIX: Degrees unserialization keeps searching for the class...
   void OnUnserialize(IOChannel f)
   {
      double lat, lon;
      f.Get(lat);
      f.Get(lon);
      this.lat = Radians { lat };
      this.lon = Radians { lon };
   }

   void OnSerialize(IOChannel f)
   {
      f.Put((double)(Radians)lat);
      f.Put((double)(Radians)lon);
   }
};

public struct GeoExtent
{
   GeoPoint ll, ur;

   void clear()
   {
      ll = { MAXDOUBLE, MAXDOUBLE };
      ur = { -MAXDOUBLE, -MAXDOUBLE };
   }

   property double geodeticArea
   {
      get
      {
         static const double ooa = 1.0 / wgs84Minor, aob = wgs84Minor / wgs84Major;
         static const double a2 = wgs84Minor * wgs84Minor;
         static const double bmabpa = (wgs84Major - wgs84Minor) * (wgs84Major + wgs84Minor);
         double srbmabpa = sqrt(bmabpa);
         double pl1 = fabs(fabs((Radians)ll.lat) - Pi/2) < 1E-12 ? Sgn(ll.lat) * Pi/2 : atan(aob * tan(ll.lat));
         double pl2 = fabs(fabs((Radians)ur.lat) - Pi/2) < 1E-12 ? Sgn(ur.lat) * Pi/2 : atan(aob * tan(ur.lat));
         double sinl1 = sin(pl1), sinl2 = sin(pl2);
         Radians dLon = ur.lon - ll.lon;
         if(dLon < 0) dLon += 2*Pi;

         return dLon *
            (wgs84Major *
               (a2 * wgs84Minor * (asinh(srbmabpa * sinl2 * ooa) - asinh(srbmabpa * sinl1 * ooa)) +
               fabs(wgs84Minor) * srbmabpa * (sinl2 * sqrt(bmabpa * sinl2 * sinl2 + a2) - sinl1 * sqrt(bmabpa * sinl1 * sinl1 + a2)))
            ) * 0.5 * ooa / srbmabpa;
      }
   }

   bool intersects(const GeoExtent b)
   {
      if(ll.lon < MAXDOUBLE && ll.lon > ur.lon)
      {
         GeoExtent a { { ll.lat, ll.lon }, { ur.lat, 180 } };
         GeoExtent c { { ll.lat, -180 }, { ur.lat, ur.lon } };
         return a.intersects(b) || c.intersects(b);
      }
      else if(b.ll.lon < MAXDOUBLE && b.ll.lon > b.ur.lon)
      {
         GeoExtent a { { b.ll.lat, b.ll.lon }, { b.ur.lat, 180 } };
         GeoExtent c { { b.ll.lat, -180 }, { b.ur.lat, b.ur.lon } };
         return intersects(a) || intersects(c);
      }
      else
      return (Radians)ll.lat < (Radians)b.ur.lat - radEpsilon &&
             (Radians)b.ll.lat < (Radians)ur.lat - radEpsilon &&
             (Radians)ll.lon < (Radians)b.ur.lon - radEpsilon &&
             (Radians)b.ll.lon < (Radians)ur.lon - radEpsilon;
   }
};
