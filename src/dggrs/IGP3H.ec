public import IMPORT_STATIC "ecere"
private:

import "ISEA3H"

#ifdef ECERE_STATIC
// 3D Math is excluded from libecere's static config
import "Plane"

#define Vector3D DGGVector3D
#define Plane DGGPlane
#endif

#define POW3(x) ((x) < sizeof(powersOf3) / sizeof(powersOf3[0]) ? (uint64)powersOf3[x] : (uint64)pow(3, x))

static uint16 icoIndices[20][3] =
{
   // Top triangles
   { 0, 1, 2 },
   { 0, 2, 3 },
   { 0, 3, 4 },
   { 0, 4, 5 },
   { 0, 5, 1 },

   // Mirror of Top triangles
   { 6, 2, 1 },
   { 7, 3, 2 },
   { 8, 4, 3 },
   { 9, 5, 4 },
   { 10, 1, 5 },

   // Mirror of Bottom triangles
   { 2, 6, 7 },
   { 3, 7, 8 },
   { 4, 8, 9 },
   { 5, 9, 10 },
   { 1, 10, 6 },

   // Bottom triangles
   { 11, 7, 6 },
   { 11, 8, 7 },
   { 11, 9, 8 },
   { 11,10, 9 },
   { 11, 6, 10 }
};

static Pointd vertices5x6[20][3] =
{
   // Top triangles
   { { 1, 0 }, { 0, 0 }, { 1, 1 } },
   { { 2, 1 }, { 1, 1 }, { 2, 2 } },
   { { 3, 2 }, { 2, 2 }, { 3, 3 } },
   { { 4, 3 }, { 3, 3 }, { 4, 4 } },
   { { 5, 4 }, { 4, 4 }, { 5, 5 } },

   // Mirror of Top triangles
   { { 0, 1 }, { 1, 1 }, { 0, 0 } },
   { { 1, 2 }, { 2, 2 }, { 1, 1 } },
   { { 2, 3 }, { 3, 3 }, { 2, 2 } },
   { { 3, 4 }, { 4, 4 }, { 3, 3 } },
   { { 4, 5 }, { 5, 5 }, { 4, 4 } },

   // Mirror of Bottom triangles
   { { 1, 1 }, { 0, 1 }, { 1, 2 } },
   { { 2, 2 }, { 1, 2 }, { 2, 3 } },
   { { 3, 3 }, { 2, 3 }, { 3, 4 } },
   { { 4, 4 }, { 3, 4 }, { 4, 5 } },
   { { 5, 5 }, { 4, 5 }, { 5, 6 } },

   // Bottom triangles
   { { 0, 2 }, { 1, 2 }, { 0, 1 } },
   { { 1, 3 }, { 2, 3 }, { 1, 2 } },
   { { 2, 4 }, { 3, 4 }, { 2, 3 } },
   { { 3, 5 }, { 4, 5 }, { 3, 4 } },
   { { 4, 6 }, { 5, 6 }, { 4, 5 } }
};

static define phi = (1 + sqrt(5)) / 2;
static define precisionPerDefinition = Degrees { 1e-5 };

public class IGP3H : ISEA3H
{
   // DGGH
   /*
   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   double getZoneArea(ISEA3HZone zone)
   {
      // REVIEW: Is this Goldberg Polyhedra space exactly equal area?
   }
   */
   Vector3D icoVertices[12];
   double cp[2][AUTH_ORDER];

   static inline Radians latAuthalicToGeodetic(Radians phi)
   {
      return applyCoefficients(cp[1], phi);
   }

   static inline Radians latGeodeticToAuthalic(Radians phi)
   {
      return applyCoefficients(cp[0], phi);
   }

   static void getVertices(Vector3D * vertices /* [12] */)
   {
      // double a = edgeSize;
      Radians t = atan(0.5);
      double ty =-sin(t), by =-sin(-t);
      double tc = cos(t), bc = cos(-t);
      double r = 1; //a * sqrt(phi * phi + 1) / 2;
      Radians s = 2*Pi/5;
      int i;

      vertices[ 0] = { 0, -r, 0 }; // North pole
      vertices[11] = { 0,  r, 0 }; // South pole

      for(i = 0; i < 5; i++)
      {
         Radians ta = Degrees { -180 - 36/2 - 72 } + s * i;
         Radians ba = ta + s / 2;

         // North hemisphere vertices
         vertices[1 + i] = { cos(ta) * r * tc, ty * r, sin(ta) * r * tc };
         // South hemisphere vertices
         vertices[6 + i] = { cos(ba) * r * bc, by * r, sin(ba) * r * bc };
      }
   }

   static int getFace(const Pointd p)
   {
      int face = -1;
      static const double epsilon = 1E-11; //1E-9; // 1E-11 fixes /dggs/ISEA3H/zones/Q0-0-D
      double x = p.x, y = p.y;
           if(x < 0 || (y > x && x < 5 - epsilon)) x += epsilon;
      else if(x > 5 || (y < x && x > 0 + epsilon)) x -= epsilon;
           if(y < 0 || (x > y && y < 6 - epsilon)) y += epsilon;
      else if(y > 6 || (x < y && y > 0 + epsilon)) y -= epsilon;

      if(x >= 0 && x <= 5 && y >= 0 && y <= 6)
      {
         int ix = Max(0, Min(4, (int)floor(x)));
         int iy = Max(0, Min(5, (int)floor(y)));
         if((iy == ix || iy == ix + 1))
         {
            int rhombus = ix + iy;
            bool top = x - ix > y - iy;

            switch(rhombus)
            {
               case 0: face = top ? 0 : 5; break;
               case 2: face = top ? 1 : 6; break;
               case 4: face = top ? 2 : 7; break;
               case 6: face = top ? 3 : 8; break;
               case 8: face = top ? 4 : 9; break;

               case 1: face = top ? 10 : 15; break;
               case 3: face = top ? 11 : 16; break;
               case 5: face = top ? 12 : 17; break;
               case 7: face = top ? 13 : 18; break;
               case 9: face = top ? 14 : 19; break;
            }
         }
      }
      return face;
   }

   GeoPoint orientation;
   double sinOrientationLat, cosOrientationLat;
   Degrees vertex2Azimuth;

   IGP3H()
   {
      getVertices(icoVertices);
      orientation = { /*(E + F) / 2 /* 90 - 58.2825255885389 = */31.7174744114611, -11.20 };
      sinOrientationLat = sin(orientation.lat); cosOrientationLat = cos(orientation.lat);
      authalicSetup(wgs84Major, wgs84Minor, cp);
   }

   bool infGoldberg5x6FromGeo(const GeoPoint p, Pointd v)
   {
      bool result = false;
      GeoPoint point { latGeodeticToAuthalic(p.lat), p.lon };
      double sinLat, cosLat;
      Vector3D v3D;
      int face;

      applyOrientation(point, point);

      sinLat = sin(point.lat), cosLat = cos(point.lat);
      v3D = { sin(point.lon) * cosLat, -sinLat, -cos(point.lon) * cosLat };

      v = { };
      result = false;
      // TODO: Directly determine face
      for(face = 0; face < 20; face++)
      {
         if(vertexWithinSphericalTri(v3D,
            icoVertices[icoIndices[face][0]], icoVertices[icoIndices[face][1]], icoVertices[icoIndices[face][2]]))
         {
            split3DBy4(
               v3D, icoVertices[icoIndices[face][0]], icoVertices[icoIndices[face][1]], icoVertices[icoIndices[face][2]],
               v, vertices5x6[face][0], vertices5x6[face][1], vertices5x6[face][2], 24);
            result = true;
            break;
         }
      }
      return result;
   }

   void infGoldberg5x6ExtentFromWGS84(const GeoExtent wgs84Extent, Pointd topLeft, Pointd bottomRight)
   {
      if((Radians)wgs84Extent.ll.lat < -Pi/2 + 0.0001 &&
         (Radians)wgs84Extent.ll.lon < -Pi   + 0.0001 &&
         (Radians)wgs84Extent.ur.lat >  Pi/2 - 0.0001 &&
         (Radians)wgs84Extent.ur.lon >  Pi   - 0.0001)
      {
         topLeft = { 0, 0 };
         bottomRight = { 5, 6 };
      }
      else
      {
         Radians lat, lon;
         Radians maxLon = wgs84Extent.ur.lon + (wgs84Extent.ur.lon < wgs84Extent.ll.lon ? 2*Pi : 0);
         Radians dLat = wgs84Extent.ur.lat - wgs84Extent.ll.lat;
         Radians dLon = maxLon - wgs84Extent.ll.lon;
         Radians latInc = dLat / ISEA_ANCHORS, lonInc = dLon / ISEA_ANCHORS;
         GeoPoint leftX, topY, rightX, bottomY;

         if(dLon < 0) dLon += 2*Pi;

         topLeft = { MAXDOUBLE, MAXDOUBLE };
         bottomRight = { -MAXDOUBLE, -MAXDOUBLE };

         for(lat = wgs84Extent.ll.lat; lat <= wgs84Extent.ur.lat; lat += latInc)
         {
            for(lon = wgs84Extent.ll.lon; lon <= maxLon; lon += lonInc)
            {
               GeoPoint geo { lat, lon };
               Pointd p;

               if(infGoldberg5x6FromGeo(geo, p))
               {
                  if(p.x < topLeft.x) topLeft.x = p.x, leftX = geo;
                  if(p.x > bottomRight.x) bottomRight.x = p.x, rightX = geo;
                  if(p.y < topLeft.y) topLeft.y = p.y, topY = geo;
                  if(p.y > bottomRight.y) bottomRight.y = p.y, bottomY = geo;
               }
               if(!lonInc) break;
            }
            if(!latInc) break;
         }

         if(lonInc && latInc)
         {
            // Additional passes closer to min/maxes (TODO: derivatives and polynomials, Jacobian matrices?)
            Radians latInc2 = 0.1 * latInc, lonInc2 = 0.1 * lonInc;

            for(tp : [ leftX, rightX, topY, bottomY ])
            {
               for(lat = Max((Radians)wgs84Extent.ll.lat, (Radians)tp.lat - latInc); lat <= Min((Radians)wgs84Extent.ur.lat, (Radians)tp.lat + latInc); lat += latInc2)
               {
                  for(lon = Max((Radians)wgs84Extent.ll.lon, (Radians)tp.lon - lonInc); lon <= Min((Radians)maxLon, (Radians)tp.lon + lonInc); lon += lonInc2)
                  {
                     GeoPoint geo { lat, lon };
                     Pointd p;

                     if(infGoldberg5x6FromGeo(geo, p))
                     {
                        if(p.x < topLeft.x) topLeft.x = p.x;
                        if(p.x > bottomRight.x) bottomRight.x = p.x;
                        if(p.y < topLeft.y) topLeft.y = p.y;
                        if(p.y > bottomRight.y) bottomRight.y = p.y;
                     }
                  }
               }
            }
         }
      }
   }

   bool vertexWithinSphericalTri(const Vector3D v3D,
      const Vector3D v1, const Vector3D v2, const Vector3D v3)
   {
      Plane planes[3];
      int i;

      planes[0].FromPoints(v1, v2, { 0, 0, 0 });
      planes[1].FromPoints(v2, v3, { 0, 0, 0 });
      planes[2].FromPoints(v3, v1, { 0, 0, 0 });

      for(i = 0; i < 3; i++)
      {
         if(planes[i].a * v3D.x + planes[i].b * v3D.y + planes[i].c * v3D.z + planes[i].d > 0)
            return false;
      }
      return true;
   }

   void split3DBy4(const Vector3D v3D, const Vector3D v1, const Vector3D v2, const Vector3D v3,
      Pointd v, const Pointd p1, const Pointd p2, const Pointd p3, int depth)
   {
      Pointd np[3];
      Vector3D nv[3];
      Vector3D v12 { (v1.x + v2.x) / 2, (v1.y + v2.y) / 2, (v1.z + v2.z) / 2 };
      Vector3D v23 { (v2.x + v3.x) / 2, (v2.y + v3.y) / 2, (v2.z + v3.z) / 2 };
      Vector3D v31 { (v3.x + v1.x) / 2, (v3.y + v1.y) / 2, (v3.z + v1.z) / 2 };
      Pointd p12 { (p1.x + p2.x) / 2, (p1.y + p2.y) / 2 };
      Pointd p23 { (p2.x + p3.x) / 2, (p2.y + p3.y) / 2 };
      Pointd p31 { (p3.x + p1.x) / 2, (p3.y + p1.y) / 2 };

      if(vertexWithinSphericalTri(v3D, v12, v23, v1))
      {
         nv[0] = v12, nv[1] = v23, nv[2] = v1;
         np[0] = p12, np[1] = p23, np[2] = p1;
      }
      else if(vertexWithinSphericalTri(v3D, v31, v1, v23))
      {
         nv[0] = v31, nv[1] = v1, nv[2] = v23;
         np[0] = p31, np[1] = p1, np[2] = p23;
      }
      else if(vertexWithinSphericalTri(v3D, v12, v2, v23))
      {
         nv[0] = v12, nv[1] = v2, nv[2] = v23;
         np[0] = p12, np[1] = p2, np[2] = p23;
      }
      else if(vertexWithinSphericalTri(v3D, v31, v23, v3))
      {
         nv[0] = v31, nv[1] = v23, nv[2] = v3;
         np[0] = p31, np[1] = p23, np[2] = p3;
      }
      else
      {
         nv[0] = { }, nv[1] = { }, nv[2] = { };
         np[0] = { }, np[1] = { }, np[2] = { };
         PrintLn("WARNING: Unmatched tri");
      }

      if(depth > 0)
         split3DBy4(v3D, nv[0], nv[1], nv[2], v, np[0], np[1], np[2], depth - 1);
      else
         v = { (np[0].x + np[1].x + np[2].x) / 3, (np[0].y + np[1].y + np[2].y) / 3 };
   }

   static inline void applyOrientation(const GeoPoint c, GeoPoint r)
   {
      if(orientation.lat || orientation.lon)
      {
         Degrees lon = c.lon + orientation.lon;
         double sinLat = sin(c.lat), cosLat = cos(c.lat);
         double sinLon = sin(lon),  cosLon = cos(lon);
         double cosLoncosLat = cosLon * cosLat;
         r = {
            lat = asin(sinLat * cosOrientationLat + cosLoncosLat * sinOrientationLat),
            lon = atan2(sinLon * cosLat, cosLoncosLat * cosOrientationLat - sinLat * sinOrientationLat)
         };
      }
      else
         r = c;
      r.lon += vertex2Azimuth;
   }

   static inline void revertOrientation(const GeoPoint c, GeoPoint r)
   {
      Degrees lon = c.lon - vertex2Azimuth;

      if(c.lat < Degrees { -90 } + precisionPerDefinition || c.lat > Degrees { 90 } - precisionPerDefinition) lon = 0;
      if(orientation.lat || orientation.lon)
      {
         double sinLat = sin(c.lat), cosLat = cos(c.lat);
         double sinLon = sin(lon), cosLon  = cos(lon);
         double cosLonCosLat = cosLon * cosLat;
         r = {
            asin(sinLat * cosOrientationLat - cosLonCosLat * sinOrientationLat),
            atan2(sinLon * cosLat, cosLonCosLat * cosOrientationLat + sinLat * sinOrientationLat) - orientation.lon
         };
      }
      else
         r = { c.lat, lon };
   }

#if 0
   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   __attribute__ ((unused))
   static void splitBy3(
      const Pointd p1, const Pointd p2, const Pointd p3,
      const Vector3D v1, const Vector3D v2, const Vector3D v3,
      const Pointd v, Pointd * np, Vector3D * nv)
   {
      Pointd d12 { p2.x - p1.x, p2.y - p1.y };
      Pointd d23 { p3.x - p2.x, p3.y - p2.y };
      Pointd d31 { p1.x - p3.x, p1.y - p3.y };
      Pointd p12[2] = {
         { p1.x + d12.x  /3, p1.y + d12.y  /3 },
         { p1.x + d12.x*2/3, p1.y + d12.y*2/3 }
      };
      Pointd p23[2] = {
         { p2.x + d23.x  /3, p2.y + d23.y  /3 },
         { p2.x + d23.x*2/3, p2.y + d23.y*2/3 }
      };
      Pointd p31[2] = {
         { p3.x + d31.x  /3, p3.y + d31.y  /3 },
         { p3.x + d31.x*2/3, p3.y + d31.y*2/3 }
      };
      Pointd pc { (p12[1].x + p31[0].x) / 2, (p12[1].y + p31[0].y) / 2 };
      int t;
      const Pointd * tris[9][3] =
      {
         { p1, &p12[0], &p31[1] },
         { &pc, &p31[1], &p12[0] },
         { &p12[1], &p23[0], &p12[0] },
         { &pc, &p12[0], &p23[0] },
         { &p12[1], p2, &p23[0] },
         { &pc, &p23[0], &p23[1] },
         { &pc, &p23[1], &p31[1] },
         { &p31[0], &p31[1], &p23[1] },
         { &p31[0], &p23[1], p3 }
      };

      Vector3D dv12 { v2.x - v1.x, v2.y - v1.y, v2.z - v1.z };
      Vector3D dv23 { v3.x - v2.x, v3.y - v2.y, v3.z - v2.z };
      Vector3D dv31 { v1.x - v3.x, v1.y - v3.y, v1.z - v3.z };
      Vector3D v12[2] = {
         { v1.x + dv12.x  /3, v1.y + dv12.y  /3, v1.z + dv12.z  /3 },
         { v1.x + dv12.x*2/3, v1.y + dv12.y*2/3, v1.z + dv12.z*2/3 }
      };
      Vector3D v23[2] = {
         { v2.x + dv23.x  /3, v2.y + dv23.y  /3, v2.z + dv23.z  /3 },
         { v2.x + dv23.x*2/3, v2.y + dv23.y*2/3, v2.z + dv23.z*2/3 }
      };
      Vector3D v31[2] = {
         { v3.x + dv31.x  /3, v3.y + dv31.y  /3, v3.z + dv31.z  /3 },
         { v3.x + dv31.x*2/3, v3.y + dv31.y*2/3, v3.z + dv31.z*2/3 }
      };
      Vector3D vc { (v12[1].x + v31[0].x) / 2, (v12[1].y + v31[0].y) / 2, (v12[1].z + v31[0].z) / 2 };

      if(fabs(d12.x) > fabs(d31.x))
      {
         if(d12.x > 0)
         {
            // Bottom-left orientation
            if(v.x >= p12[1].x)
               t = 4;
            else if(v.x >= p12[0].x)
            {
               if(v.y >= pc.y)
               {
                  if(v.x - p12[0].x >= (p12[0].y - pc.y) - (v.y - pc.y))
                     t = 2;
                  else
                     t = 3;
               }
               else
                  t = 5;
            }
            else
            {
               if(v.y >= p31[1].y)
               {
                  if(v.x - p1.x >= v.y - pc.y)
                     t = 1;
                  else
                     t = 0;
               }
               else if(v.y >= p31[0].y)
               {
                  if(v.x - p1.x >= (pc.y - p31[0].y) - (v.y - p31[0].y))
                     t = 6;
                  else
                     t = 7;
               }
               else
                  t = 8;
            }
         }
         else
         {
            // Top-right orientation
            if(v.x <= p12[1].x)
               t = 4;
            else if(v.x <= p12[0].x)
            {
               if(v.y <= pc.y)
               {
                  if(v.x - p12[1].x <= (pc.y - p12[1].y) - (v.y - p12[1].y))
                     t = 2;
                  else
                     t = 3;
               }
               else
                  t = 5;
            }
            else
            {
               if(v.y <= p31[1].y)
               {
                  if(v.x - p12[0].x <= v.y - p12[0].y)
                     t = 1;
                  else
                     t = 0;
               }
               else if(v.y <= p31[0].y)
               {
                  if(v.x - pc.x <= (p31[0].y - pc.y) - (v.y - pc.y))
                     t = 6;
                  else
                     t = 7;
               }
               else
                  t = 8;
            }
         }
      }
      else
      {
         if(d12.y > 0)
         {
            // Top-left orientation
            if(v.y >= p12[1].y)
               t = 4;
            else if(v.y >= p12[0].y)
            {
               if(v.x <= pc.x)
               {
                  if(v.y - p12[0].y >= v.x - p12[0].x)
                     t = 2;
                  else
                     t = 3;
               }
               else
                  t = 5;
            }
            else
            {
               if(v.x <= p31[1].x)
               {
                  if(v.y - p1.y >= (pc.x - p1.x) - (v.x - p1.x))
                     t = 1;
                  else
                     t = 0;
               }
               else if(v.x <= p31[0].x)
               {
                  if(v.y - p1.y >= v.x - p31[1].x)
                     t = 6;
                  else
                     t = 7;
               }
               else
                  t = 8;
            }
         }
         else
         {
            // Bottom-right orientation
            if(v.y <= p12[1].y)
               t = 4;
            else if(v.y <= p12[0].y)
            {
               if(v.x >= pc.x)
               {
                  if(v.y - p12[1].y <= v.x - pc.x)
                     t = 2;
                  else
                     t = 3;
               }
               else
                  t = 5;
            }
            else
            {
               if(v.x >= p31[1].x)
               {
                  if(v.y - pc.y <= (p1.x - pc.x) - (v.x - pc.x))
                     t = 1;
                  else
                     t = 0;
               }
               else if(v.x >= p31[0].x)
               {
                  if(v.y - pc.y <= v.x - p31[0].x)
                     t = 6;
                  else
                     t = 7;
               }
               else
                  t = 8;
            }
         }
      }

      np[0] = *tris[t][0], np[1] = *tris[t][1], np[2] = *tris[t][2];
      switch(t)
      {
         case 0: nv[0] = v1,     nv[1] = v12[0], nv[2] = v31[1]; break;
         case 1: nv[0] = vc;     nv[1] = v31[1], nv[2] = v12[0]; break;
         case 2: nv[0] = v12[1], nv[1] = v23[0]; nv[2] = v12[0]; break;
         case 3: nv[0] = vc,     nv[1] = v12[0], nv[2] = v23[0]; break;
         case 4: nv[0] = v12[1], nv[1] = v2,     nv[2] = v23[0]; break;
         case 5: nv[0] = vc,     nv[1] = v23[0], nv[2] = v23[1]; break;
         case 6: nv[0] = vc,     nv[1] = v23[1], nv[2] = v31[1]; break;
         case 7: nv[0] = v31[0], nv[1] = v31[1], nv[2] = v23[1]; break;
         case 8: nv[0] = v31[0], nv[1] = v23[1], nv[2] = v3;     break;
      }
   }
#endif

   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   static void splitBy4(
      const Pointd p1, const Pointd p2, const Pointd p3,
      const Vector3D v1, const Vector3D v2, const Vector3D v3,
      const Pointd v, Pointd * np, Vector3D * nv)
   {
      Pointd d12 { p2.x - p1.x, p2.y - p1.y };
      // Pointd d23 { p3.x - p2.x, p3.y - p2.y };
      Pointd d31 { p3.x - p1.x, p3.y - p3.y };
      Pointd p12 { (p1.x + p2.x) / 2, (p1.y + p2.y) / 2 };
      Pointd p23 { (p2.x + p3.x) / 2, (p2.y + p3.y) / 2 };
      Pointd p31 { (p3.x + p1.x) / 2, (p3.y + p1.y) / 2 };
      const Pointd * tris[4][3] =
      {
         { &p12, &p23, p1 },
         { &p31, p1, &p23 },
         { &p12, p2, &p23 },
         { &p31, &p23, p3 }
      };
      Vector3D v12 { (v1.x + v2.x) / 2, (v1.y + v2.y) / 2, (v1.z + v2.z) / 2 };
      Vector3D v23 { (v2.x + v3.x) / 2, (v2.y + v3.y) / 2, (v2.z + v3.z) / 2 };
      Vector3D v31 { (v3.x + v1.x) / 2, (v3.y + v1.y) / 2, (v3.z + v1.z) / 2 };
      int t;
      if(fabs(d12.x) > fabs(d31.x))
      {
         if(d12.x > 0)
         {
            // Bottom-left orientation
            if(v.x >= p12.x)
               t = 2;
            else if(v.y >= p31.y)
            {
               if(v.x - p1.x >= (p1.y - p31.y) - (v.y - p31.y))
                  t = 0;
               else
                  t = 1;
            }
            else
               t = 3;
         }
         else
         {
            // Top-right orientation
            if(v.x <= p12.x)
               t = 2;
            else if(v.y <= p31.y)
            {
               if(v.x - p12.x <= (p23.y - p1.y) - (v.y - p1.y))
                  t = 0;
               else
                  t = 1;
            }
            else
               t = 3;
         }
      }
      else
      {
         if(d12.y > 0)
         {
            // Top-left orientation
            if(v.y >= p12.y)
               t = 2;
            else if(v.x <= p31.x)
            {
               if(v.y - p1.y >= v.x - p1.x)
                  t = 0;
               else
                  t = 1;
            }
            else
               t = 3;
         }
         else
         {
            // Bottom-right orientation
            if(v.y <= p12.y)
               t = 2;
            else if(v.x >= p31.x)
            {
               if(v.y - p23.y <= v.x - p23.x)
                  t = 0;
               else
                  t = 1;
            }
            else
               t = 3;
         }
      }

      np[0] = *tris[t][0], np[1] = *tris[t][1], np[2] = *tris[t][2];
      switch(t)
      {
         case 0: nv[0] = v12, nv[1] = v23, nv[2] = v1; break;
         case 1: nv[0] = v31; nv[1] = v1,  nv[2] = v23; break;
         case 2: nv[0] = v12; nv[1] = v2,  nv[2] = v23; break;
         case 3: nv[0] = v31; nv[1] = v23, nv[2] = v3; break;
      }
   }

   // TODO: Avoid recursion
   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   static void /*::*/resolvePoint(const Pointd v,
      const Pointd p1, const Pointd p2, const Pointd p3,
      const Vector3D v1, const Vector3D v2, const Vector3D v3,
      GeoPoint out, int depth)
   {
      Pointd np[3];
      Vector3D nv[3];

      // splitBy3(p1, p2, p3, v1, v2, v3, v, np, nv);
      splitBy4(p1, p2, p3, v1, v2, v3, v, np, nv);

      if(depth > 0)
         resolvePoint(v, np[0], np[1], np[2], nv[0], nv[1], nv[2], out, depth - 1);
      else
      {
         // Use center of triangle
         Vector3D c {
            (nv[0].x + nv[1].x + nv[2].x) / 3,
            (nv[0].y + nv[1].y + nv[2].y) / 3,
            (nv[0].z + nv[1].z + nv[2].z) / 3 };
         bool firstIteration = true;
         double p;

         //c.Normalize(c);

         p = sqrt(c.x*c.x + c.z*c.z);
         out = { (Radians)atan2(-c.y, p), (Radians)atan2(c.x, -c.z) };
         while(true)
         {
            Degrees lat2 = (Radians)atan2(-c.y, p);
            if(!firstIteration && fabs(lat2 - out.lat) < 0.000000001)
               break;
            out.lat = lat2;
            firstIteration = false;
         }
         revertOrientation(out, out);
         out.lat = latAuthalicToGeodetic(out.lat);
      }
   }

   bool infGoldberg5x6ToGeo(const Pointd v, GeoPoint p)
   {
      int face = getFace(v);
      if(face != -1)
      {
         uint16 * indices = icoIndices[face];

         resolvePoint(v,
            vertices5x6[face][0], vertices5x6[face][1], vertices5x6[face][2],
            icoVertices[indices[0]], icoVertices[indices[1]], icoVertices[indices[2]],
            p, 24);
         return true;
      }
      return false;
   }

   ISEA3HZone getZoneFromCRSCentroid(int level, CRS crs, const Pointd centroid)
   {
      if(level <= 33)
      {
         switch(crs)
         {
            case 0: return ISEA3HZone::fromCentroid(level, centroid);
            case CRS { epsg, 4326 }:
            case CRS { ogc, 84 }:
               return (ISEA3HZone)getZoneFromWGS84Centroid(level,
                  crs == { ogc, 84 } ?
                     { centroid.y, centroid.x } :
                     { centroid.x, centroid.y });
         }
      }
      return nullZone;
   }

   ISEA3HZone getZoneFromWGS84Centroid(int level, const GeoPoint centroid)
   {
      if(level <= 33)
      {
         Pointd v;
         infGoldberg5x6FromGeo(centroid, v);
         return ISEA3HZone::fromCentroid(level, v);
      }
      return nullZone;
   }

   void getZoneCRSCentroid(ISEA3HZone zone, CRS crs, Pointd centroid)
   {
      switch(crs)
      {
         case 0: centroid = zone.centroid; break;
         case CRS { epsg, 4326 }:
         case CRS { ogc, 84 }:
         {
            GeoPoint geo;
            infGoldberg5x6ToGeo(zone.centroid, geo);
            centroid = crs == { ogc, 84 } ?
               { geo.lon, geo.lat } :
               { geo.lat, geo.lon };
            break;
         }
      }
   }

   void getZoneWGS84Centroid(ISEA3HZone zone, GeoPoint centroid)
   {
      infGoldberg5x6ToGeo(zone.centroid, centroid);
   }

   void getZoneCRSExtent(ISEA3HZone zone, CRS crs, CRSExtent extent)
   {
      switch(crs)
      {
         case 0: extent = zone.isea5x6Extent; break;
         case CRS { epsg, 4326 }:
         case CRS { ogc, 84 }:
         {
            GeoExtent geo;
            getZoneWGS84Extent(zone, geo);
            extent.crs = crs;
            if(crs == { ogc, 84 })
            {
               extent.tl = { geo.ll.lon, geo.ur.lat };
               extent.br = { geo.ur.lon, geo.ll.lat };
            }
            else
            {
               extent.tl = { geo.ur.lat, geo.ll.lon };
               extent.br = { geo.ll.lat, geo.ur.lon };
            }
            break;
         }
      }
   }
   void getZoneWGS84Extent(ISEA3HZone zone, GeoExtent extent)
   {
      int i;
      GeoPoint centroid;
      Radians minDLon = 99999, maxDLon = -99999;
      Array<GeoPoint> vertices = (Array<GeoPoint>)getRefinedVertices(zone, { epsg, 4326 }, 0, true);
      int nVertices = vertices ? vertices.count : 0;

      getZoneWGS84Centroid(zone, centroid);

      extent.clear();
      for(i = 0; i < nVertices; i++)
      {
         GeoPoint p = vertices[i];
         Radians dLon = p.lon - centroid.lon;

         if(dLon > Pi) dLon -= 2* Pi;
         if(dLon < -Pi) dLon += 2* Pi;

         if(p.lat > extent.ur.lat) extent.ur.lat = p.lat;
         if(p.lat < extent.ll.lat) extent.ll.lat = p.lat;

         if(dLon > maxDLon)
            maxDLon = dLon, extent.ur.lon = p.lon;
         if(dLon < minDLon)
            minDLon = dLon, extent.ll.lon = p.lon;
      }
      if((Radians)extent.ll.lon < -Pi)
         extent.ll.lon += 2*Pi;
      if((Radians)extent.ur.lon > Pi)
         extent.ur.lon -= 2*Pi;

      delete vertices;
   }

   int getZoneCRSVertices(ISEA3HZone zone, CRS crs, Pointd * vertices)
   {
      uint count = 0, i;

      switch(crs)
      {
         case 0:
            count = zone.getVertices(vertices);
            break;
         case CRS { ogc, 84 }:
         case CRS { epsg, 4326 }:
            count = zone.getVertices(vertices);
            for(i = 0; i < count; i++)
            {
               GeoPoint geo;
               infGoldberg5x6ToGeo(vertices[i], geo);
               vertices[i] = crs == { ogc, 84 } ? { geo.lon, geo.lat } : { geo.lat, geo.lon };
            }
            break;
      }
      return count;
   }

   int getZoneWGS84Vertices(ISEA3HZone zone, GeoPoint * vertices)
   {
      Pointd v5x6[6];
      uint count = zone.getVertices(v5x6), i;
      for(i = 0; i < count; i++)
         infGoldberg5x6ToGeo(v5x6[i], vertices[i]);
      return count;
   }

   Array<Pointd> getZoneRefinedCRSVertices(ISEA3HZone zone, CRS crs, int edgeRefinement)
   {
      return getRefinedVertices(zone, crs, edgeRefinement, false);
   }

   Array<GeoPoint> getZoneRefinedWGS84Vertices(ISEA3HZone zone, int edgeRefinement)
   {
      return (Array<GeoPoint>)getRefinedVertices(zone, { epsg, 4326 }, edgeRefinement, true);
   }

   void getApproxWGS84Extent(ISEA3HZone zone, GeoExtent extent)
   {
      int sh = zone.subHex;
      int i;
      GeoPoint centroid;
      Radians minDLon = 99999, maxDLon = -99999;
      Pointd vertices[7];  // REVIEW: Should this be 6? can't ever be 7?
      int nVertices = zone.getVertices(vertices);

      getZoneWGS84Centroid(zone, centroid);

      extent.clear();
      for(i = 0; i < nVertices; i++)
      {
         Pointd * cv = &vertices[i];
         GeoPoint p;
         if(infGoldberg5x6ToGeo(cv, p))
         {
            Radians dLon = p.lon - centroid.lon;

            if(dLon > Pi) dLon -= 2* Pi;
            if(dLon < -Pi) dLon += 2* Pi;

            if(p.lat > extent.ur.lat) extent.ur.lat = p.lat;
            if(p.lat < extent.ll.lat) extent.ll.lat = p.lat;

            if(dLon > maxDLon)
               maxDLon = dLon, extent.ur.lon = p.lon;
            if(dLon < minDLon)
               minDLon = dLon, extent.ll.lon = p.lon;
         }
      }

      if(sh == 1 || sh == 6)
      {
         // "North" pole
         extent.ll.lon = -Pi;
         extent.ur.lon = Pi;
         extent.ur.lat = Pi/2;
      }
      else if(sh == 2 || sh == 7)
      {
         // "South" pole
         extent.ll.lon = -Pi;
         extent.ur.lon = Pi;
         extent.ll.lat = -Pi/2;
      }
   }

   private static Array<Pointd> getRefinedVertices(ISEA3HZone zone, CRS crs, int edgeRefinement, bool useGeoPoint) // 0 edgeRefinement for 1-20 based on level
   {
      Array<Pointd> rVertices = null;
      bool crs84 = crs == CRS { ogc, 84 } || crs == CRS { epsg, 4326 };
      Pointd vertices[9];
      int numPoints = zone.getBaseRefinedVertices(crs84, vertices);
      if(numPoints)
      {
         Array<Pointd> ap = null;
         bool geodesic = false; //true;
         int level = zone.level;
         bool refine = zone.subHex < 3;  // Only use refinement for ISEA for even levels -- REVIEW: Should this be odd level here?
         int i;

         if(crs84)
         {
            GeoExtent e; // REVIEW: Currently only used to decide whether to wrap
            GeoPoint centroid;
            Radians dLon;
            bool wrap;
            int lonQuad;

            getApproxWGS84Extent(zone, e);
            dLon = (Radians)e.ur.lon - (Radians)e.ll.lon;

            getZoneWGS84Centroid(zone, centroid);
            wrap = (dLon < 0 || e.ll.lon > centroid.lon || dLon > Pi);
            lonQuad = (int)(((Radians)centroid.lon + Pi) * (4 / (2*Pi)));

            if(geodesic)
            {
               ap = { size = numPoints };
               for(i = 0; i < numPoints; i++)
               {
                  GeoPoint point;
                  infGoldberg5x6ToGeo(vertices[i], point);
                  if(wrap)
                     point.lon = wrapLonAt(lonQuad, point.lon, 0);
                  ap[i] = useGeoPoint ? { (Radians) point.lat, (Radians) point.lon } :
                     crs == { ogc, 84 } ? { point.lon, point.lat } : { point.lat, point.lon };
               }
            }
            else
            {
               int nDivisions = edgeRefinement ? edgeRefinement :
                  level < 3 ? 20 : level < 5 ? 15 : level < 8 ? 10 : level < 10 ? 8 : level < 11 ? 5 : level < 12 ? 2 : 1;
               Array<Pointd> r = refineISEA(numPoints, vertices, /*1024 * */ nDivisions, true); // * 1024 results in level 2 zones areas accurate to 0.01 km^2
               ap = { minAllocSize = /*size = */r.count };
               for(i = 0; i < r.count; i++)
               {
                  GeoPoint point;
                  // Imprecisions causes some failures... http://localhost:8080/ogcapi/collections/gebco/dggs/ISEA3H/zones/L0-2B3FA-G
                  if(infGoldberg5x6ToGeo(r[i], point))
                  {
                     if(wrap)
                        point.lon = wrapLonAt(lonQuad, point.lon, 0);
                     ap.Add(useGeoPoint ? { (Radians) point.lat, (Radians) point.lon } :
                        crs == { ogc, 84 } ? { point.lon, point.lat } : { point.lat, point.lon });
                  }
               }
               ap.minAllocSize = 0;
               delete r;
            }
         }
         else if(refine)
         {
            Array<Pointd> r = refineISEA(numPoints, vertices, 1, false);
            ap = { size = r.count };
            for(i = 0; i < r.count; i++)
            {
               if(!crs)
                  ap[i] = { r[i].x, r[i].y };
            }
            delete r;
         }
         else
         {
            ap = { size = numPoints };
            for(i = 0; i < numPoints; i++)
            {
               if(!crs)
                  ap[i] = { vertices[i].x, vertices[i].y };
            }
         }
         rVertices = ap;
      }
      return rVertices;
   }

   Array<DGGRSZone> listZones(int level, const GeoExtent bbox)
   {
      return listIGP3HZones(level, bbox);
   }

   // Sub-zone Order
   Array<Pointd> getSubZoneCRSCentroids(ISEA3HZone parent, CRS crs, int depth)
   {
      Array<Pointd> centroids = parent.getSubZoneCentroids(depth);
      if(centroids)
      {
         uint count = centroids.count;
         switch(crs)
         {
            case 0: break;
            case CRS { epsg, 4326 }:
            case CRS { ogc, 84 }:
            {
               int i;
               for(i = 0; i < count; i++)
               {
                  GeoPoint geo;
                  infGoldberg5x6ToGeo(centroids[i], geo);
                  centroids[i] = crs == { ogc, 84 } ? { geo.lon, geo.lat } : { geo.lat, geo.lon };
               }
            }
            default: delete centroids;
         }
      }
      return centroids;
   }

   Array<GeoPoint> getSubZoneWGS84Centroids(ISEA3HZone parent, int depth)
   {
      Array<GeoPoint> geo = null;
      Array<Pointd> centroids = parent.getSubZoneCentroids(depth);
      if(centroids)
      {
         uint count = centroids.count;
         int i;

         geo = { size = count };
         for(i = 0; i < count; i++)
            infGoldberg5x6ToGeo(centroids[i], geo[i]);
         delete centroids;
      }
      return geo;
   }

   static Array<DGGRSZone> listIGP3HZones(int zoneLevel, const GeoExtent bbox)
   {
      Array<DGGRSZone> zones = null;
      AVLTree<ISEA3HZone> tsZones { };
      int isea9RLevel = zoneLevel / 2;
      uint64 power = POW3(isea9RLevel);
      double z = 1.0 / power;
      int hexSubLevel = zoneLevel & 1;
      Pointd tl, br;
      double x, y;
      bool extentCheck = true;

      if(bbox != null && bbox.OnCompare(wholeWorld))
      {
         // fputs("WARNING: accurate bounding box not yet supported\n", stderr);
         infGoldberg5x6ExtentFromWGS84(bbox, tl, br);
      }
      else
         extentCheck = false, infGoldberg5x6ExtentFromWGS84(wholeWorld, tl, br);

      for(y = tl.y; y < br.y + 2*z; y += z)
      {
         int rootY = (int)(y + 1E-11);
         int row = (int)(y / z + 1E-11);
         for(x = tl.x; x < br.x + 2*z; x += z)
         {
            int rootX = (int)(x + 1E-11);
            int col = (int)(x / z + 1E-11);
            int d = rootY - rootX;
            if(rootX < 5 && (d == 0 || d == 1))
            {
               int nHexes = 0, h;
               ISEA3HZone hexes[4];

               hexes[nHexes++] = ISEA3HZone::fromISEA9R(isea9RLevel, row, col, hexSubLevel ? 'D' : 'A');
               if(hexes[nHexes-1] == nullZone)
                  continue; // This should no longer happen...

               if(hexSubLevel)
               {
                  // TODO: Test whether each sub-hex is within
                  hexes[nHexes++] = ISEA3HZone::fromISEA9R(isea9RLevel, row, col, 'E');
                  hexes[nHexes++] = ISEA3HZone::fromISEA9R(isea9RLevel, row, col, 'F');
               }
               if(row == 0 && col == power-1) // "North" pole
                  hexes[nHexes++] = ISEA3HZone::fromISEA9R(isea9RLevel, row, col, hexSubLevel ? 'G' : 'B');
               if(col == 4*power && row == 6*power-1) // "South" pole
                  hexes[nHexes++] = ISEA3HZone::fromISEA9R(isea9RLevel, row, col, hexSubLevel ? 'H' : 'C');

               for(h = 0; h < nHexes; h++)
                  tsZones.Add(hexes[h]);
            }
         }
      }

      if(tsZones.count)
      {
         zones = { minAllocSize = tsZones.count };
         for(t : tsZones)
         {
            ISEA3HZone zone = t;
            if(extentCheck)
            {
               // REVIEW: Computing the detailed wgs84Extent is slow as it uses refined vertices and involves a large amount of inverse projections.
               //         Are we missing large numbers of hexagons first eliminating those outside the approximate extent?
               GeoExtent e;

               getApproxWGS84Extent(zone, e);
               if(!e.intersects(bbox))
                  continue;

               getZoneWGS84Extent(zone, e);
               if(!e.intersects(bbox))
                  continue;
            }
            zones[zones.count++] = zone;
         }
         zones.Sort(true);
      }
      delete tsZones;
      return zones;
   }
}
