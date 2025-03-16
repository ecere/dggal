// This is the basis for defined projections from the WGS84 ellipsoid to the
// Cartesian 5x6 icosahedral / rhombic space, where a 1x1 square correspond to
// a root rhombus combining two triangular faces of an icosahedron
public import IMPORT_STATIC "ecere"
private:

import "GeoExtent"
import "authalic"

#ifdef ECERE_STATIC
// 3D Math is excluded from libecere's static config
import "Plane"

#define Vector3D DGGVector3D
#define Plane DGGPlane
#endif

define ANCHORS_5x6 = 33;

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

/*static */define phi = (1 + sqrt(5)) / 2;
static define precisionPerDefinition = Degrees { 1e-5 };

class RI5x6Projection
{
   Vector3D icoVertices[12];
   double cp[2][AUTH_ORDER];
   GeoPoint orientation;
   double sinOrientationLat, cosOrientationLat;
   Degrees vertex2Azimuth;

   RI5x6Projection()
   {
      getVertices(icoVertices);
      orientation = { /*(E + F) / 2 /* 90 - 58.2825255885389 = */31.7174744114611, -11.20 };
      sinOrientationLat = sin(orientation.lat); cosOrientationLat = cos(orientation.lat);
      authalicSetup(wgs84Major, wgs84Minor, cp);
   }

   /*static */Degrees ::angleBetweenUnitVectors(const Vector3D u, const Vector3D v)
   {
       // FIXME: Lack of const 'this' causes warning
      if(u.x * v.x + u.y * v.y + u.z * v.z < 0) // if(u.DotProduct(v) < 0)
      {
         double s = Vector3D { -v.x - u.x, -v.y - u.y, -v.z - u.z }.length / 2;
         return Pi - 2 * (Radians)asin(s < -1 ? -1 : s > 1 ? 1 : s);
      }
      else
      {
         double s = Vector3D { v.x - u.x, v.y - u.y, v.z - u.z }.length / 2;
         return 2 * (Radians)asin(s < -1 ? -1 : s > 1 ? 1 : s);
      }
   }

#if 0
   static void ::slerp(Vector3D out, const Vector3D p0, const Vector3D p1, double t)
   {
      double ang0Cos = p0.DotProduct(p1) / p0.DotProduct(p0);
      double ang0Sin = sqrt(1 - ang0Cos*ang0Cos);
      double ang0 = atan2(ang0Sin, ang0Cos);
      //Radians ang0 = angleBetweenUnitVectors(p0, p1);
      //double ang0Sin = sin(ang0);
      double l0 = sin((1-t)*ang0);
      double l1 = sin(t    *ang0);
      out =
      {
         (l0*p0.x + l1*p1.x)/ang0Sin,
         (l0*p0.y + l1*p1.y)/ang0Sin,
         (l0*p0.z + l1*p1.z)/ang0Sin
      };
   }
#endif

   void ::geoToCartesian(const GeoPoint c, Vector3D out)
   {
      double sinLat = sin(c.lat), cosLat = cos(c.lat);
      out = { sin(c.lon) * cosLat, -sinLat, -cos(c.lon) * cosLat };
   }

   void ::cartesianToGeo(const Vector3D c, GeoPoint out)
   {
      double p = sqrt(c.x*c.x + c.z*c.z);
      bool firstIteration = true;

      out = { (Radians)atan2(-c.y, p), (Radians)atan2(c.x, -c.z) };
      while(true)
      {
         Degrees lat2 = (Radians)atan2(-c.y, p);
         if(!firstIteration && fabs(lat2 - out.lat) < 0.000000001)
            break;
         out.lat = lat2;
         firstIteration = false;
      }
   }

   private /*static inline */void ::slerpAngle(
      Vector3D out, const Vector3D p0, const Vector3D p1,
      Radians distance, Radians movement)
   {
      double oOAng0Sin = 1/sin(distance), l0 = sin(distance - movement), l1 = sin(movement);
      out =
      {
         (l0*p0.x + l1*p1.x) * oOAng0Sin,
         (l0*p0.y + l1*p1.y) * oOAng0Sin,
         (l0*p0.z + l1*p1.z) * oOAng0Sin
      };
   }

   private /*static inline */void ::cartesian3DToBary(double b[3],
      const Vector3D p, const Vector3D p1, const Vector3D p2, const Vector3D p3)
   {
      Vector3D d12 { p2.x - p1.x, p2.y - p1.y, p2.z - p1.z };
      Vector3D d13 { p3.x - p1.x, p3.y - p1.y, p3.z - p1.z };
      Vector3D d1p {  p.x - p1.x,  p.y - p1.y,  p.z - p1.z };
      double d00 = d12.DotProduct(d12);
      double d01 = d12.DotProduct(d12);
      double d11 = d13.DotProduct(d13);
      double d20 = d1p.DotProduct(d12);
      double d21 = d1p.DotProduct(d13);
      double oDet = 1 / (d00 * d11 - d01 * d01);
      b[0] = (d11 * d20 - d01 * d21) * oDet;
      b[1] = (d00 * d21 - d01 * d20) * oDet;
      b[2] = 1 - b[0] - b[1];
   }

   private /*static inline */void ::cartesianToBary(double b[3],
      const Pointd p, const Pointd p1, const Pointd p2, const Pointd p3)
   {
      Pointd d31 { p1.x - p3.x, p1.y - p3.y };
      Pointd d23 { p3.x - p2.x, p3.y - p2.y };
      Pointd d3p {  p.x - p3.x,  p.y - p3.y };
      double oDet = -1;
#if 0 //def _DEBUG
      // REVIEW: always -1 for 5x6 space?
      oDet = 1 / (d23.x * d31.y - d23.y * d31.x);
      //if(oDet != -1)
         //PrintLn("oDet: ", oDet);
#endif
      b[0] = (d23.x * d3p.y - d23.y * d3p.x) * oDet;
      b[1] = (d31.x * d3p.y - d31.y * d3p.x) * oDet;
      b[2] = 1 - b[0] - b[1];
   }

   /*static inline*/ void applyOrientation(const GeoPoint c, GeoPoint r)
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

   /*static inline */void revertOrientation(const GeoPoint c, GeoPoint r)
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

   /*static inline */Radians latAuthalicToGeodetic(Radians phi)
   {
      return applyCoefficients(cp[1], phi);
   }

   /*static inline */Radians latGeodeticToAuthalic(Radians phi)
   {
      return applyCoefficients(cp[0], phi);
   }

   static void ::getVertices(Vector3D * vertices /* [12] */)
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

   static int ::getFace(const Pointd p)
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

   bool ::vertexWithinSphericalTri(const Vector3D v3D,
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

   virtual void inverseIcoFace(const Pointd v,
      const Pointd p1, const Pointd p2, const Pointd p3,
      const Vector3D v1, const Vector3D v2, const Vector3D v3,
      GeoPoint out);
   virtual void forwardIcoFace(const Vector3D v,
      const Vector3D v1, const Vector3D v2, const Vector3D v3,
      const Pointd p1, const Pointd p2, const Pointd p3,
      Pointd out);

   virtual bool forward(const GeoPoint p, Pointd v)
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
            forwardIcoFace(
               v3D, icoVertices[icoIndices[face][0]], icoVertices[icoIndices[face][1]], icoVertices[icoIndices[face][2]],
               vertices5x6[face][0], vertices5x6[face][1], vertices5x6[face][2],
               v);
            result = true;
            break;
         }
      }
      return result;
   }

   virtual bool inverse(const Pointd v, GeoPoint p)
   {
      int face = getFace(v);
      if(face != -1)
      {
         uint16 * indices = icoIndices[face];

         inverseIcoFace(v,
            vertices5x6[face][0], vertices5x6[face][1], vertices5x6[face][2],
            icoVertices[indices[0]], icoVertices[indices[1]], icoVertices[indices[2]],
            p);
         return true;
      }
      return false;
   }

   void extent5x6FromWGS84(const GeoExtent wgs84Extent, Pointd topLeft, Pointd bottomRight)
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
         Radians latInc = dLat / ANCHORS_5x6, lonInc = dLon / ANCHORS_5x6;
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

               if(forward(geo, p))
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

                     if(forward(geo, p))
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
}


/*static */Array<Pointd> refine5x6(int count, const Pointd * src, int nDivisions, bool wrap)
{
   int n = (count + 2) * nDivisions;
   Array<Pointd> points { minAllocSize = n };
   int i;
   // double r = 1.0 / nDivisions;
   double e = 1E-11;

   for(i = 0; i < count; i++)
   {
      Pointd p = src[i], next = src[i < count - 1 ? i + 1 : 0];
      int cpx1 = (int)floor(p.x+e), cpy1 = (int)floor(p.y+e);
      int cnx1 = (int)floor(next.x+e), cny1 = (int)floor(next.y+e);
      int cpx2 = (int)floor(p.x-e);
      int cpy2 = (int)floor(p.y-e);
      int cnx2 = (int)floor(next.x-e);
      int cny2 = (int)floor(next.y-e);
      bool nTopRightOfP = (next.x > p.x + e && next.x - p.x < 3) || p.x - next.x > 3;
      bool nTopLeftOfP = (next.x < p.x - e && p.x - next.x < 3) || next.x - p.x > 3;
      bool nBottomRightOfP = (next.y > p.y && next.y - p.y < 3) || p.y - next.y > 3;
      bool nBottomLeftOfP = (next.y < p.y - e && p.y - next.y < 3) || next.y - p.y > 3;
      bool atTopDentCrossingRight    = cpx2 != cpx1 && p.x > p.y && nTopRightOfP;
      bool atTopDentCrossingLeft     = cpy2 != cpy1 && p.x > p.y && nTopLeftOfP;
      bool atBottomDentCrossingLeft  = cpx2 != cpx1 && p.y > p.x + 1 && nBottomLeftOfP;
      bool atBottomDentCrossingRight = cpy2 != cpy1 && p.y > p.x + 1 && nBottomRightOfP;
      bool nextAtTopDentCrossingRight    = cnx2 != cnx1 && next.x > next.y && nTopLeftOfP;
      bool nextAtTopDentCrossingLeft     = cny2 != cny1 && next.x > next.y && nTopRightOfP;
      bool nextAtBottomDentCrossingLeft  = cnx2 != cnx1 && next.y > next.x + 1 && nBottomRightOfP;
      bool nextAtBottomDentCrossingRight = cny2 != cny1 && next.y > next.x + 1 && nBottomLeftOfP;
      int cpx, cpy, cnx, cny;
      int k;
      double dx = (next.x - p.x), dy = (next.y - p.y);
      int nRoot, pRoot;
      bool nSouth, pSouth;
      Pointd pi1, pi2; // Interruption and/or wrapping points
      bool interrupted = false, wrapped = false;

      if((atTopDentCrossingRight && nextAtTopDentCrossingLeft) ||
         (atTopDentCrossingLeft && nextAtTopDentCrossingRight) ||
         (atBottomDentCrossingRight && nextAtBottomDentCrossingLeft) ||
         (atBottomDentCrossingLeft && nextAtBottomDentCrossingRight))
      {
         // Point on each side of interruption -- simply add both points
         points.Add(p);
         points.Add(next);
         continue;
      }

      //if(i != 0) continue;
      //if(i != 1) continue;
      //if(i != 2) continue;
      //if(i != 3) continue;
      //if(i != 4) continue;
      //if(i != 5) continue;

      if(fabs(dx) < 1E-11 && fabs(dy) < 1E-11 && wrap) continue;

      // Cross already for cases where crossing does not happen mid-edge

      if(wrap)
      {
         if(atTopDentCrossingRight)
            p = { cpx1 + 1.0 - (p.y - cpy1), cpy1 + 1 };
         else if(atTopDentCrossingLeft)
            p = { cpx1, cpy1 - (p.x - cpx1) };
         else if(atBottomDentCrossingLeft)
            p = { cpx1 - (p.y - cpy1), cpy1 };
         else if(atBottomDentCrossingRight)
            p = { cpx1 + 1, cpy1 + (cpx1 + 1 - p.x) };

         if(nextAtTopDentCrossingRight)
            next = { cnx1 + 1.0 - (next.y - cny1), cny1 + 1 };
         else if(nextAtTopDentCrossingLeft)
            next = { cnx1, cny1 - (next.x - cnx1) };
         else if(nextAtBottomDentCrossingLeft)
            next = { cnx1 - (next.y - cny1), cny1 };
         else if(nextAtBottomDentCrossingRight)
            next = { cnx1 + 1, cny1 + (cnx1 + 1 - next.x) };

         if(p.x > 5 + e || p.y > 6 + e)
            p.x -= 5, p.y -= 5;
         else if(p.x < -e || p.y < -e)
            p.x += 5, p.y += 5;
         if(next.x > 5 + e || next.y > 6 + e)
            next.x -= 5, next.y -= 5;
         else if(next.x < -e || next.y < -e)
            next.x += 5, next.y += 5;
      }

      cpx1 = (int)floor(p.x+e), cpy1 = (int)floor(p.y+e);
      cnx1 = (int)floor(next.x+e), cny1 = (int)floor(next.y+e);
      cpx = cpx1, cpy = cpy1;
      cnx = cnx1, cny = cny1;
      dx = (next.x - p.x), dy = (next.y - p.y);
      nRoot = cnx + cny, pRoot = cpx + cpy;
      nSouth = nRoot & 1, pSouth = pRoot & 1;

      if(cpx != cnx && cpy != cny)
      {
         if(cnx - cpx > 3)
         {
            if(wrap)
               wrapped = true;
            dx -= 5, dy -= 5;
            if(cpy == 0 && cny == 5 && cnx == 4)
            {
               // Uninterrupted wrap to the left
               pi1 = { 0, p.y + dy * (0 - p.x) / dx };
               pi2 = { pi1.x + 5, pi1.y + 5 };
            }
            else if(cpy == 0 && cny == 4 && cnx == 4)
            {
               // Crossing top dent to the left while wrapping
               // The shortest path is going straight for the edge, hitting it at 30 degrees (taking into account 5x6 shearing)
               interrupted = true;
               pi1 = { p.x + 0.5 * (cpy - p.y), cpy };
               pi2 = { (cnx + 1), cny + 1 - (pi1.x - cpx) };
            }
            else if(cpy == 1 && cny == 5 && cnx == 4)
            {
               // Crossing bottom dent to the left while wrapping
               // The shortest path is going straight for the edge, hitting it at 30 degrees (taking into account 5x6 shearing)
               interrupted = true;
               pi1 = { cpx, p.y + 0.5 * (cpx - p.x) };
               pi2 = { cnx + 1 - (pi1.y - cpy), cny + 1 };
            }
            else
            {
               // This happens on exact same point
#ifdef _DEBUG
               // PrintLn("WARNING: Unexpected case");
#endif
               if(wrapped) continue;
            }
         }
         else if(cnx - cpx < -3)
         {
            if(wrap)
               wrapped = true;
            dx += 5, dy += 5;
            if(cpy == 5 && cny == 0 && cnx == 0)
            {
               // Uninterrupted wrap to the right
               pi1 = { 5, p.y + dy * (5 - p.x) / dx };
               pi2 = { pi1.x - 5, pi1.y - 5 };
               dx -= (pi2.x + 5 - pi1.x);
               dy -= (pi2.y + 5 - pi1.y);
            }
            else if(cpy == 4 && cny == 0 && cnx == 0)
            {
               // Crossing top dent to the right while wrapping
               // The shortest path is going straight for the edge, hitting it at 30 degrees (taking into account 5x6 shearing)
               interrupted = true;
               pi1 = { cpx + 1, p.y + 0.5 * (cpx + 1 - p.x) };
               pi2 = { cnx + 1 - (pi1.y - cpy), cny };
            }
            else if(cpy == 5 && cny == 1 && cnx == 0)
            {
               // Crossing bottom dent to the right while wrapping
               // The shortest path is going straight for the edge, hitting it at 30 degrees
               // (taking into account 5x6 shearing: cos(Degrees { 30 }) / sqrt(3))
               interrupted = true;
               pi1 = { p.x + 0.5 * (cpy + 1 - p.y), cpy + 1 };
               pi2 = { cnx, cny + (cpx + 1 - pi1.x) };
            }
            else
            {
               // This happens on exact same point
#ifdef _DEBUG
               // PrintLn("WARNING: Unexpected case");
#endif
               if(wrapped) continue;
            }
         }
         else if(nSouth == pSouth)
         {
            // Crossing top dent to the right
            if(cnx > cpx && !nSouth && !pSouth)
            {
               // The shortest path is going straight for the edge, hitting it at 30 degrees (taking into account 5x6 shearing)
               interrupted = true;
               pi1 = { cpx + 1, p.y + 0.5 * (cpx + 1 - p.x) };
               pi2 = { cnx + (cpy + 1 - pi1.y), cny };
            }
            // Crossing top dent to the left
            else if(cnx < cpx && !nSouth && !pSouth)
            {
               // The shortest path is going straight for the edge, hitting it at 30 degrees (taking into account 5x6 shearing)
               interrupted = true;
               pi1 = { p.x + 0.5 * (cpy - p.y), cpy };
               pi2 = { cnx + 1, cny + 1 - (pi1.x - cpx) };
            }
            // Crossing bottom dent to the right
            else if(cnx > cpx && nSouth && pSouth)
            {
               // The shortest path is going straight for the edge, hitting it at 30 degrees (taking into account 5x6 shearing)
               interrupted = true;
               pi1 = { p.x + 0.5 * (cpy + 1 - p.y), cpy + 1 };
               pi2 = { cnx, cny + (cpx + 1 - pi1.x) };
            }
            // Crossing bottom dent to the left
            else if(cnx < cpx && nSouth && pSouth)
            {
               // The shortest path is going straight for the edge, hitting it at 30 degrees (taking into account 5x6 shearing)
               interrupted = true;
               pi1 = { cpx, p.y + 0.5 * (cpx - p.x) };
               pi2 = { cnx + 1 - (pi1.y - cpy), cny + 1 };
            }
            else
            {
               // This happens on exact same point
#ifdef _DEBUG
               // PrintLn("WARNING: Unexpected case");
#endif
               if(wrapped) continue;
            }
         }
      }

      if(fabs(dx) < 1E-11 && fabs(dy) < 1E-11 && wrap)
         continue;

      if(interrupted)
      {
         // REVIEW: The check might be obsoleted by new early crossing code
         dx = fabs(pi1.x - p.x) > e ? 2*(pi1.x - p.x) : next.y - pi2.y;
         dy = fabs(pi1.y - p.y) > e ? 2*(pi1.y - p.y) : next.x - pi2.x;
      }

      points.Add(p);

      /*
      if(i == 1) continue;
      if(i == 2) continue;
      if(i == 3) continue;
      if(i == 4) continue;
      if(i == 0) continue;
      */

      if(wrap) // Clarify this boolean parameter -- is actual refining only happening when wrap is true?
      {
         int startPoint = points.count - 1, startK = 0;
         // dx *= r, dy *= r;

         for(k = 1; k <= nDivisions - 1; k++)
         {
            // int count = points.count;
            double lastX = points[startPoint].x, lastY = points[startPoint].y;
            double x = lastX + dx * (k - startK) / nDivisions, y = lastY + dy * (k - startK) / nDivisions;
            int px = (int)floor(x+1E-11), py = (int)floor(y+1E-11);

            // TODO: Use extra point at 0.001 dx & dy before and after north/south pole at half of polar edges

            if(x < 0 || x > 5 || y < 0 || y > 6 || py < px || py - px > 1)
            {
               // Crossing interruption or wrapping: add points on each side
               if(interrupted || wrapped)
               {
                  points.Add(pi1);
                  points.Add(pi2);
                  startPoint = points.count-1;
                  startK = k;
               }
               else
                  points.Add({ x, y }); // This currently happens when walking along x = 6 edge
               if(interrupted)
               {
                  // REVIEW: Do we need point after left over distance?
                  // points.Add({ pi2.x + dx - (pi1.x - lastX), pi2.y + dy - (pi1.y - lastY) });
                  double t = dx;
                  dx = dy, dy = t;
               }
            }
            else
               points.Add({ x, y });
         }
      }
      else if(interrupted)
      {
         points.Add(pi1);
         points.Add(pi2);
      }
   }
   points.minAllocSize = 0;
   return points;
}

static bool intersects5x6Interruption(const Pointd a0, const Pointd a1, const Pointd b0, const Pointd b1, Pointd i, double * distance)
{
   static const double e = 1E-12; //1E-13;
   double s1y = a1.y - a0.y, s1x = a1.x - a0.x;
   double s2y = b1.y - b0.y, s2x = b1.x - b0.x;
   double dy = a0.y - b0.y, dx = a0.x - b0.x;
   double d = (s1x * s2y - s2x * s1y);

   if(fabs(d) > 1E-13) // Return false for parallel segments
   {
      double factor = 1.0 / d;
      double s = (s1x * dy - s1y * dx) * factor;
      if(s - e >= 0 && s + e <= 1)
      {
         double t = (s2x * dy - s2y * dx) * factor;
         if(t - e >= 0 && t + e <= 1)
         {
            i = { a0.x + t * s1x, a0.y + t * s1y };
            {
               // double dx = i.x - a0.x, dy = i.y - a0.y;
               *distance = t; //dx * dx + dy * dy;
            }
            return true;
         }
      }
   }
   return false;
}

// #define TEST_CROSSING

#ifdef TEST_CROSSING
static struct CrossingTest
{
   Pointd src, dst;
   bool south, left;
};

static void cross5x6InterruptionTest()
{
   CrossingTest testCases[] =
   {
      { { 1, 0.3 }, { 1.7, 1 }, false, false },
      { { 2, 1.3 }, { 2.7, 2 }, false, false },
      { { 3, 2.3 }, { 3.7, 3 }, false, false },
      { { 4, 3.3 }, { 4.7, 4 }, false, false },

      { { 1.7, 1 }, { 1, 0.3 }, false, true },
      { { 2.7, 2 }, { 2, 1.3 }, false, true },
      { { 3.7, 3 }, { 3, 2.3 }, false, true },
      { { 4.7, 4 }, { 4, 3.3 }, false, true },

      { { 0.3, 2 }, { 1, 2.7 }, true, false },
      { { 1.3, 3 }, { 2, 3.7 }, true, false },
      { { 2.3, 4 }, { 3, 4.7 }, true, false },
      { { 3.3, 5 }, { 4, 5.7 }, true, false },

      { { 1, 2.7 }, { 0.3, 2 }, true, true },
      { { 2, 3.7 }, { 1.3, 3 }, true, true },
      { { 3, 4.7 }, { 2.3, 4 }, true, true },
      { { 4, 5.7 }, { 3.3, 5 }, true, true },

      { { 1, 0 }, { 2, 1 }, false, false },
      { { 2, 1 }, { 3, 2 }, false, false },
      { { 3, 2 }, { 4, 3 }, false, false },
      { { 4, 3 }, { 5, 4 }, false, false },

      { { 1, 1 }, { 1, 1 }, false, false },
      { { 2, 2 }, { 2, 2 }, false, false },
      { { 3, 3 }, { 3, 3 }, false, false },
      { { 4, 4 }, { 4, 4 }, false, false },

      { { 2, 1 }, { 1, 0 }, false, true },
      { { 3, 2 }, { 2, 1 }, false, true },
      { { 4, 3 }, { 3, 2 }, false, true },
      { { 5, 4 }, { 4, 3 }, false, true },

      { { 1, 1 }, { 1, 1 }, false, true },
      { { 2, 2 }, { 2, 2 }, false, true },
      { { 3, 3 }, { 3, 3 }, false, true },
      { { 4, 4 }, { 4, 4 }, false, true },

      { { 0, 2 }, { 1, 3 }, true, false },
      { { 1, 3 }, { 2, 4 }, true, false },
      { { 2, 4 }, { 3, 5 }, true, false },
      { { 3, 5 }, { 4, 6 }, true, false },

      { { 1, 2 }, { 1, 2 }, true, false },
      { { 2, 3 }, { 2, 3 }, true, false },
      { { 3, 4 }, { 3, 4 }, true, false },
      { { 4, 5 }, { 4, 5 }, true, false },

      { { 1, 3 }, { 0, 2 }, true, true },
      { { 2, 4 }, { 1, 3 }, true, true },
      { { 3, 5 }, { 2, 4 }, true, true },
      { { 4, 6 }, { 3, 5 }, true, true },

      { { 1, 2 }, { 1, 2 }, true, true },
      { { 2, 3 }, { 2, 3 }, true, true },
      { { 3, 4 }, { 3, 4 }, true, true },
      { { 4, 5 }, { 4, 5 }, true, true }
   };
   int count = sizeof(testCases) / sizeof(testCases[0]), i;

   for(i = 0; i < count; i++)
   {
      const CrossingTest * t = &testCases[i];
      Pointd dst;
      cross5x6Interruption(t->src, dst, t->south, t->left);
      if(fabs(dst.x - t->dst.x) > 1E-8 ||
         fabs(dst.y - t->dst.y) > 1E-8)
      {
         PrintLn("[!FAILED!] failed test for ", t->src, ", south: ", t->south, ", left: ", t->left, ": ", dst, " returned instead of ", t->dst);
         cross5x6Interruption(t->src, dst, t->south, t->left);
      }
   }
}
#endif

private /*static */void cross5x6Interruption(const Pointd iSrc, Pointd iDst, bool south, bool left)
{
#ifdef TEST_CROSSING
   static bool crossTested;
   if(!crossTested)
   {
      crossTested = true;
      cross5x6InterruptionTest();
   }
#endif

   switch(south)
   {
      case false: // Crossing North hemisphere
         switch(left)
         {
            case true: // Crossing left
            {
               int ix = (int)(iSrc.y + 1E-11);
               iDst = { iSrc.y, ix - (iSrc.x - ix) };
               break;
            }
            case false: // Crossing right
            {
               int iy = (int)(iSrc.x - 1 + 1E-11);
               iDst = { iy + 2 - (iSrc.y - iy), iSrc.x };
               break;
            }
         }
         break;
      case true: // Crossing South hemisphere
         switch(left)
         {
            case true: // Crossing left
            {
               int iy = (int)(iSrc.x + 1 + 1E-11);
               iDst = { iy - 1 - (iSrc.y - iy), iSrc.x + 1 };
               break;
            }
            case false: // Crossing right
            {
               int ix = (int)(iSrc.y - 2 + 1E-11);
               iDst = { iSrc.y - 1, ix + 3 - (iSrc.x - ix) };
               break;
            }
         }
         break;
   }
   if(iDst.x > 5 - 1E-11 && iDst.y > 5 - 1E-11)
      iDst.x -= 5, iDst.y -= 5;
   else if(iDst.x < 1E-11 && iDst.y < 1 - 1E-11)
      iDst.x += 5, iDst.y += 5;
}

#if !defined(__EMSCRIPTEN__)
__attribute__ ((optimize("-fno-unsafe-math-optimizations")))
#endif
bool crosses5x6Interruption(const Pointd cIn, double dx, double dy, Pointd iSrc, Pointd iDst, bool * north)
{
   bool result = false;
   static Pointd interruptions[2 /* hemisphere */][5 /* root rhombus */][2 /* left, right */][2 /* points*/] =
   {
      {  // North interruptions
         { { { 0, 0 }, { 1, 0 } }, { { 1, 0 }, { 1, 1 } } },
         { { { 1, 1 }, { 2, 1 } }, { { 2, 1 }, { 2, 2 } } },
         { { { 2, 2 }, { 3, 2 } }, { { 3, 2 }, { 3, 3 } } },
         { { { 3, 3 }, { 4, 3 } }, { { 4, 3 }, { 4, 4 } } },
         { { { 4, 4 }, { 5, 4 } }, { { 5, 4 }, { 5, 5 } } }
      },
      {  // South interruptions
         { { { 0, 1 }, { 0, 2 } }, { { 0, 2 }, { 1, 2 } } },
         { { { 1, 2 }, { 1, 3 } }, { { 1, 3 }, { 2, 3 } } },
         { { { 2, 3 }, { 2, 4 } }, { { 2, 4 }, { 3, 4 } } },
         { { { 3, 4 }, { 3, 5 } }, { { 3, 5 }, { 4, 5 } } },
         { { { 4, 5 }, { 4, 6 } }, { { 4, 6 }, { 5, 6 } } }
      }
   };
   int h, r, s;
   int crossH, crossS;
   double minD = MAXDOUBLE;
   Pointd c = cIn;
        if(fabs(c.x - 0) < 1E-12) c.x = 0;
   else if(fabs(c.x - 1) < 1E-12) c.x = 1;
   else if(fabs(c.x - 2) < 1E-12) c.x = 2;
   else if(fabs(c.x - 3) < 1E-12) c.x = 3;
   else if(fabs(c.x - 4) < 1E-12) c.x = 4;
   else if(fabs(c.x - 5) < 1E-12) c.x = 5;
        if(fabs(c.y - 0) < 1E-12) c.y = 0;
   else if(fabs(c.y - 1) < 1E-12) c.y = 1;
   else if(fabs(c.y - 2) < 1E-12) c.y = 2;
   else if(fabs(c.y - 3) < 1E-12) c.y = 3;
   else if(fabs(c.y - 4) < 1E-12) c.y = 4;
   else if(fabs(c.y - 5) < 1E-12) c.y = 5;
   else if(fabs(c.y - 6) < 1E-12) c.y = 6;

   for(h = 0; h < 2; h++)
   {
      for(r = 0; r < 5; r++)
      {
         for(s = 0; s < 2; s++)
         {
            Pointd iCur;
            double d;

            if(intersects5x6Interruption(
               c, { c.x + dx, c.y + dy },
               interruptions[h][r][s][0], interruptions[h][r][s][1],
               iCur, &d) && d < minD)
            {
               iSrc = iCur;
               minD = d;
               crossH = h, crossS = s;
               result = true;
            }
         }
      }
   }
   if(result)
   {
      cross5x6Interruption(iSrc, iDst, crossH == 1, crossS == 0);
      *north = crossH == 0;
   }
   return result;
}

// Jumping over 5x6 interruptions in the 5x6 space is still tricky, and subject to optimization.
// These 3 functions are three attempts to do the correct thing in various scenarios.
// Currently, one works better than the others for particular scenarios.
// The end goal is to have a single function which works better than all of them,
// and which could even potentially support jumping over multiple interruptions
// such as for iterating polar scanlines.

#if !defined(__EMSCRIPTEN__)
__attribute__ ((optimize("-fno-unsafe-math-optimizations")))
#endif
void move5x6Vertex(Pointd v, const Pointd c, double dx, double dy)
{
   int cx = (int)(c.x + 1E-11), cy = (int)(c.y + 1E-11);
   int vx, vy;

   v = { c.x + dx, c.y + dy };
   vx = (int)floor(c.x + dx - Sgn(dx) * 1E-11);
   vy = (int)floor(c.y + dy - Sgn(dy) * 1E-11);

   if(((vx != cx && fabs(v.y - vy) > 1E-11) || (vy != cy && fabs(v.x - vx) > 1E-11)) &&
      (vy - vx > 1 || vy < vx))
   {
      if(vx < cx)
      {
         // Stepping over bottom dent to the left
         v.x = cx - (c.y - cy) + dx - dy;
         v.y = cy + dx;
      }
      else if(vx > cx)
      {
         // Stepping over top dent to the right
         v.x = cx - (c.y - cy) + dx - dy;
         v.y = cy + dx;
      }
      else if(vy < cy)
      {
         // Stepping over top dent to the left
         v.x = cx + dy;
         v.y = cy - (c.x - cx) - dx + dy;
      }
      else if(vy > cy)
      {
         // Stepping over bottom dent to the right
         v.x = cx + dy;
         v.y = cy - (c.x - cx) - dx + dy;
      }
   }
#if 0 // _DEBUG   // TODO: Clarify where the behavior of move5x6Vertex2() differ
   {
      Pointd v2;
      move5x6Vertex2(v2, c, dx, dy, false);
      if(fabs(v2.x - v.x) > 1E-11 ||
         fabs(v2.y - v.y) > 1E-11)
         move5x6Vertex2(v2, c, dx, dy, false);
   }
#endif
}

// NOTE: This does not have safe optimizations disabled, which might potentially
//       expalin issues using it after cross5x6Interruption()
void move5x6Vertex3(Pointd v, const Pointd c, double dx, double dy)
{
   bool north;
   Pointd i1, i2;
   if(crosses5x6Interruption(c, dx, dy, i1, i2, &north))
   {
      // Assuming crossing right for now
      if(north)
      {
         v.x = i2.x - 2 * (dy - (i1.y - c.y));
         v.y = i2.y + dx - (i1.x - c.x);
      }
      else
      {
         v.x = i2.x + dy - (i1.y - c.y);
         v.y = i2.y + 2 * (dx - (i1.x - c.x));
      }
   }
   else
      v = { c.x + dx, c.y + dy };

   if(v.x > 5 && v.y > 5)
      v.x -= 5, v.y -= 5;
   else if(v.x < 0 && v.y < 1)
      v.x += 5, v.y += 5;
}


// REVIEW: This function may potentially replace the previous attempt above
#if !defined(__EMSCRIPTEN__)
__attribute__ ((optimize("-fno-unsafe-math-optimizations")))
#endif
void move5x6Vertex2(Pointd v, const Pointd srcC, double dx, double dy, bool crossEarly)
{
   double e = 1E-11;
   Pointd c = srcC;
   int cx = (int)floor(c.x+e), cy = (int)floor(c.y+e);
   int cx2 = (int)floor(c.x-e), cy2 = (int)floor(c.y-e);
   Pointd n { c.x + dx, c.y + dy };
   bool atTopDentCrossingRight, atTopDentCrossingLeft, atBottomDentCrossingLeft, atBottomDentCrossingRight;
   bool nTopRightOfP, nTopLeftOfP, nBottomRightOfP, nBottomLeftOfP;
   int vx, vy;

   if(n.x < 0) n.x += 5; else if(n.x > 5) n.x -= 5;
   if(n.y < 0) n.y += 5; else if(n.y > 5) n.y -= 5;

   nTopRightOfP = (n.x > c.x + e && n.x - c.x < 3) || c.x - n.x > 3;
   nTopLeftOfP = (n.x < c.x - e && c.x - n.x < 3) || n.x - c.x > 3;
   nBottomRightOfP = (n.y > c.y && n.y - c.y < 3) || c.y - n.y > 3;
   nBottomLeftOfP = (n.y < c.y - e && c.y - n.y < 3) || n.y - c.y > 3;
   atTopDentCrossingRight    = cx2 != cx && c.x > c.y && nTopRightOfP;
   atTopDentCrossingLeft     = cy2 != cy && c.x > c.y && nTopLeftOfP;
   atBottomDentCrossingLeft  = cx2 != cx && c.y > c.x + 1 && nBottomLeftOfP;
   atBottomDentCrossingRight = cy2 != cy && c.y > c.x + 1 && nBottomRightOfP;

   // Cross already for cases where crossing does not happen mid-edge
   if(crossEarly)
   {
      if(atTopDentCrossingRight)
         c = { cx + 1.0 - (c.y - cy), cy + 1 };
      else if(atTopDentCrossingLeft)
         c = { cx, cy - (c.x - cx) };
      else if(atBottomDentCrossingLeft)
         c = { cx - (c.y - cy), cy };
      else if(atBottomDentCrossingRight)
         c = { cx + 1, cy + (cx + 1 - c.x) };

      if(c.x > 5 || c.y > 6 + e)
         c.x -= 5, c.y -= 5;
      else if(c.x < 0 || c.y < -e)
         c.x += 5, c.y += 5;
      cx = (int)floor(c.x + e), cy = (int)floor(c.y + e);
   }

   v = { c.x + dx, c.y + dy };
   vx = (int)floor(c.x + dx + 1E-11), vy = (int)floor(c.y + dy + 1E-11);

   if(((vx != cx && fabs(v.y - vy) > 1E-11) || (vy != cy && fabs(v.x - vx) > 1E-11)) &&
      (vy - vx > 1 || vy < vx) /* REVIEW: && (c.x - cx > 1E-11 || c.y - cy > 1E-11)*/ )
   {
      Pointd pi1, pi2; // Interruptions
      // Assuming the crossing point is at half the distance

      if(fabs(v.x - v.y - 1) < 1E-10) v = { 1, 0 }; // "North" pole
      else if(fabs(v.y - v.x - 2) < 1E-10) v = { 4, 6 }; // "South" pole
      else if(vx < cx && v.x - vx < 1-e)
      {
         // Stepping over bottom dent to the left
         pi1 = { cx, c.y + 0.5 * (cx - c.x) };
         pi2 = { cx - (pi1.y - cy), cy };
         v.x = pi2.x + pi1.y - c.y;
         v.y = pi2.y + pi1.x - c.x;
      }
      else if(vx > cx && v.x - vx > e)
      {
         // Stepping over top dent to the right
         pi1 = { cx + 1, c.y + 0.5 * (cx + 1 - c.x) };
         pi2 = { cx + 1 + (cy + 1 - pi1.y), cy + 1 };
         v.x = pi2.x + pi1.y - c.y;
         v.y = pi2.y + pi1.x - c.x;
      }
      else if(vy < cy && v.y - vy < 1-e)
      {
         // Stepping over top dent to the left
         pi1 = { c.x + 0.5 * (cy - c.y), cy };
         pi2 = { cx, cy - (pi1.x - cx) };
         v.x = pi2.x + pi1.y - c.y;
         v.y = pi2.y + pi1.x - c.x;
      }
      else if(vy > cy && v.y - vy > e)
      {
         // Stepping over bottom dent to the right
         pi1 = { v.x + 0.5 * (cy + 1 - c.y), cy + 1 };
         pi2 = { cx + 1, cy + 1 + (cx + 1 - pi1.x) };
         v.x = pi2.x + pi1.y - c.y;
         v.y = pi2.y + pi1.x - c.x;
      }
   }
   if(v.x > 5)
      v.x -= 5, v.y -= 5;
   else if(v.x < 0)
      v.x += 5, v.y += 5;
}

void canonicalize5x6(const Pointd _src, Pointd out)
{
   bool south = false, cross = false;
   bool np = false, sp = false;
   Pointd src = _src;
   int cx, cy;

   if(src.x > 5 - 1E-11 && src.y > 5 - 1E-11)
      src.x -= 5, src.y -= 5;
   if(src.x < -1E-11 || src.y < -1E-11)
      src.x += 5, src.y += 5;

   cx = (int)floor(src.x + 1E-11);
   cy = (int)floor(src.y + 1E-11);

   switch(cy)
   {
      case 0: cross = fabs(src.x - 1) < 1E-11; np = cross && fabs(src.y - 0) < 1E-11; break;
      case 1: cross = fabs(src.x - 2) < 1E-11; np = cross && fabs(src.y - 1) < 1E-11; break;
      case 2: cross = fabs(src.x - 3) < 1E-11; np = cross && fabs(src.y - 2) < 1E-11; break;
      case 3: cross = fabs(src.x - 4) < 1E-11; np = cross && fabs(src.y - 3) < 1E-11; break;
      case 4: cross = fabs(src.x - 5) < 1E-11; np = cross && fabs(src.y - 4) < 1E-11; break;
   }

   switch(cx)
   {
      case 0: if(fabs(src.y - 2) < 1E-11) { cross = true; south = true; sp = cross && fabs(src.x - 0) < 1E-11; } break;
      case 1: if(fabs(src.y - 3) < 1E-11) { cross = true; south = true; sp = cross && fabs(src.x - 1) < 1E-11; } break;
      case 2: if(fabs(src.y - 4) < 1E-11) { cross = true; south = true; sp = cross && fabs(src.x - 2) < 1E-11; } break;
      case 3: if(fabs(src.y - 5) < 1E-11) { cross = true; south = true; sp = cross && fabs(src.x - 3) < 1E-11; } break;
      case 4: if(fabs(src.y - 6) < 1E-11) { cross = true; south = true; sp = cross && fabs(src.x - 4) < 1E-11; } break;
   }

   if(sp)
      out = { 4, 6 };
   else if(np)
      out = { 1, 0 };
   else if(cross)
      cross5x6Interruption(src, out, south, false);
   else if(fabs(src.x - 5) < 1E-11)
      out = { 0, src.y - 5 };
   else
      out = src;
}
