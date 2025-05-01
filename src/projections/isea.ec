public import IMPORT_STATIC "ecere"

private:

import "ri5x6"

// TODO: We'll probably want to replace the ISEA5x6Projection and ISEAPlanarProjection classes completely
//       by the Slice & Dice approach in SliceAndDiceGreatCircleIcosahedralProjection


/*
   This module includes code adapted from Java to eC from
   Franz-Benjamin Mocnik's ISEA implementation found at
   https://github.com/mocnik-science/geogrid/blob/master/
    src/main/java/org/giscience/utils/geogrid/projections/ISEAProjection.java
   with the following license:

   MIT License

   Copyright (c) 2017-2019 Heidelberg University

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

/* The ISEA projection a projects a sphere on the icosahedron. Thereby the size of areas mapped to the icosahedron
 * are preserved. Angles and distances are however slightly distorted. The angular distortion is below 17.27 degree, and
 * the scale variation is less than 16.3 per cent.
 *
 * The projection has been proposed and has been described in detail by:
 *
 * John P. Snyder: An equal-area map projection for polyhedral globes. Cartographica, 29(1), 10–21, 1992.
 * doi:10.3138/27H7-8K88-4882-1752
 *
 * Another description and improvements can be found in:
 *
 * Erika Harrison, Ali Mahdavi-Amiri, and Faramarz Samavati: Optimization of inverse Snyder polyhedral projection.
 * International Conference on Cyberworlds 2011. doi:10.1109/CW.2011.36
 *
 * Erika Harrison, Ali Mahdavi-Amiri, and Faramarz Samavati: Analysis of inverse Snyder optimizations.
 * In: Marina L. Gavrilova, and C. J. Kenneth Tan (Eds): Transactions on Computational Science XVI. Heidelberg,
 * Springer, 2012. pp. 134–148. doi:10.1007/978-3-642-32663-9_8
 */

// static define phi = (1 + sqrt(5)) / 2;
// radius
static define a2 = wgs84Major * wgs84Major;
static define c2 = wgs84Minor * wgs84Minor;

static define eccentricity = 0.081819190842621494334802451753856674763664983739; // sqrt(1.0 - (c2/a2));
static define log1pe_1me = 0.164005007919614277203379920290070458347565289517;   // log((1+e)/(1-e)))
static define S = Pi * (2 * a2 + c2/eccentricity * log1pe_1me);
/*static */define earthAuthalicRadius = 6371007.180918473897976; //sqrt(S / (4 * Pi)); // R -- authalic sphere radius for WGS84 [6371007.1809184747 m]
static define R2 = earthAuthalicRadius * earthAuthalicRadius; // R^2
/*static */define RprimeOverR = 0.910383281509503568223473953373552451373533737718; // (1 / (2 * sqrt(5)) + 1 / 6.0) * sqrt(Pi * sqrt(3)); // R' / R
/*static */define Rprime = RprimeOverR * earthAuthalicRadius; // R'

// distortion
// static double maximumAngularDistortion = 17.27;
// static double maximumScaleVariation = 1.163;
// static double minimumScaleVariation = .860;

// Vertices of dodecahedron centered in icosahedron faces
static define E = Degrees { 52.622631859350304357142861505062482429904952618128 }; //(Degrees)atan((3 + sqrt(5)) / 4); // Latitude of center of top icosahedron faces
static define F = Degrees { 10.812316963571706291284944774484222898890833082227 }; //(Degrees)atan((3 - sqrt(5)) / 4); // Latitude of center of faces mirroring top icosahedron face
static define numIcosahedronFaces = 20;
static const GeoPoint facesCenterDodecahedronVertices[numIcosahedronFaces] =
{
   {  E, -144 }, {  E, -72 }, {  E,  0 }, {  E,  72 }, {  E, 144 },
   {  F, -144 }, {  F, -72 }, {  F,  0 }, {  F,  72 }, {  F, 144 },
   { -F, -108 }, { -F, -36 }, { -F, 36 }, { -F, 108 }, { -F, 180 },
   { -E, -108 }, { -E, -36 }, { -E, 36 }, { -E, 108 }, { -E, 180 }
};
// static define precision = Degrees { 1e-9 };
static define precision = Degrees { 1e-11 };
static define precisionPerDefinition = Degrees { 1e-5 };

/*static */define invSqrt3 = 0.57735026918962576450914878050195745564760175127;
/*static */define invTriWidth = 0.000000130302362294123870772483295681045794621239; // 1 / triWidth
/*static*/ define sqrt3 = 1.73205080756887729352744634150587236694280525381038; //sqrt(3);

/*static */define Rprime2X = 2 * Rprime;
/*static */define AzMax = Degrees { 120 };
/*static */define sdc2vos = F + 2 * Degrees { 58.282525588538994675786 }/*(Degrees)atan(phi)*/ - Degrees { 90 }; // g -- sphericalDistanceFromCenterToVerticesOnSphere
/*static */define tang = 0.763932022500210303590826331268723764559381640388; //tan(sdc2vos);
/*static */define cotTheta = 1.0 / tan(Degrees { 30 });
/*static */define RprimeTang = Rprime * tang; // twice the center-to-base distance
/*static */define centerToBase = RprimeTang / 2;
/*static */define triWidth = RprimeTang * sqrt3;
/*static */define triWidthOver2 = 3837228.974186817588708390593582550750582603355645708023;
static define Rprime2Tan2g = RprimeTang * RprimeTang;
static define cosG = cos(Degrees { 36 });
static define sinGcosSDC2VoS = sin(Degrees { 36 }) * cos(sdc2vos); // sin G cos g
static define westVertexLon = Degrees { -144 };
static const double yOffsets[4] = { -2*centerToBase, -4*centerToBase, -5*centerToBase, -7*centerToBase };

struct ISEAFacePoint
{
   int face;
   double x, y;
};

// #define POLAR_ORIENTATION
// #define FULLER_ORIENTATION
// #define FIGURE15_ORIENTATION

class ISEAPlanarProjection
{
   double cp[2][AUTH_ORDER]; // 0: geodetic -> authalic; 1: authalic -> geodetic

   ISEAPlanarProjection()
   {
      authalicSetup(wgs84Major, wgs84Minor, cp);
   }
public:
   // In authalic latitude
   property GeoPoint orientation
   {
      set { orientation = value; sinOrientationLat = sin(value.lat); cosOrientationLat = cos(value.lat); }
      get { value = orientation; }
   }

   // In authalic latitude
   property GeoPoint vertex1
   {
      set { vertex1 = value; sinVertex1Lat = sin(value.lat); cosVertex1Lat = cos(value.lat); }
      get { value = vertex1; }
   }

#if defined(FULLER_ORIENTATION)
   // R. Buckminster Fuller's Dymaxion map orientation
  property::vertex1 = { 2.3009, -5.2454 }; vertex2Azimuth = 7.46658 + 144;
#elif defined(FIGURE15_ORIENTATION)
   // Pole centered on icosahedron triangle for Figure 15
   // http://localhost:8080/ogcapi/collections/NaturalEarth:physical:ne_10m_graticules:ne_10m_graticules_5/map?crs=OGC:1534&width=2048&bbox-crs=OGC:153456&bbox=2,2,3,2&void-color=beige
   property::orientation = { sdc2vos, 0 }; vertex2Azimuth = 180;
   //property::vertex1 = { 90 - sdc2vos, 0 }; vertex2Azimuth = 180;
   //property::orientation = { -sdc2vos, 0 };
   //property::vertex1 = { -90 + sdc2vos, 0 };
#elif defined(POLAR_ORIENTATION)
   // Polar orientation (+proj=isea +orient=pole)
   //property::orientation = { 0, 0 };
   property::vertex1 = { 90, 0 };
   /*
    * One corner of the icosahedron is, by default, facing to the north pole, and one to the south pole. The provided
    * orientation is relative to the default orientation.
    *
    * The orientation shifts every location by the angle orientation.lon in direction of positive
    * longitude, and thereafter by the angle orientation.lat in direction of positive latitude.
    */
#else
   // Default orientation symmetric to equator (+proj=isea), with a 0.05 degrees West tweak for authalic conversion
   property::orientation = { (E + F) / 2 /* 90 - 58.2825255885389 = 31.7174744114611 */, -11.20 };
   //property::vertex1 = { atan(phi), 11.20 };
   /* Sets the orientation of the icosahedron such that the north and the south poles are mapped to the edge midpoints
    * of the icosahedron. The equator is thus mapped symmetrically.
    */
#endif

   virtual bool geoToCartesian(const GeoPoint position, Vector3D result)
   {
      ISEAFacePoint p { face = 0 };
      Radians lat = latGeodeticToAuthalic(cp, position.lat);

      if(sphereToIcosahedron({ lat, position.lon }, p))
      {
         if(p.face >= 0 && p.face <= 19)
         {
            int fy = p.face / 5, fx = p.face - 5*fy;
            double x = p.x + (2*fx + fy/2 + 1) * triWidth/2;
            double y = p.y + yOffsets[fy] + 3*centerToBase;
            result = { x = x, y = y, z = p.face + 1 }; // Storing face in z for now
         }
      }
      return p.face != -1;
   }

   virtual bool cartesianToGeo(const Vector3D _position, GeoPoint result)
   {
      Vector3D position = _position;
      bool r = false;
      static const double epsilon = 1E-11; //0.000000001;
      int face = (int)position.z;

      // This fixed e.g., http://localhost:8080/ogcapi/collections/gebco/dggs/ISEA3H/zones/C0-4-D/data.png?crs=OGC:1534
      // http://localhost:8080/ogcapi/collections/gebco/dggs/ISEA3H/zones/B1-6-D/data.png?crs=OGC:1534
      // but causes an extra unwanted portion on a global map, so for now wrapping around above instead
      //if(position.x < 0 || (position.x < triWidth / 2 && position.y < 0 && position.y * cr < position.x * sr))
      //   ; //position.x += 5 * triWidth; // Wrap around

      if(!face)
      {
         double x = (position.x + position.y * invSqrt3) * invTriWidth;
         double y = (position.x - position.y * invSqrt3) * invTriWidth;

              if(x < 0 || (y > x && x < 5 - epsilon)) x += epsilon;
         else if(x > 5 || (y < x && x > 0 + epsilon)) x -= epsilon;
              if(y < 0 || (x > y && y < 6 - epsilon)) y += epsilon;
         else if(y > 6 || (x < y && y > 0 + epsilon)) y -= epsilon;
         if(x >= 0 && x <= 5 && y >= 0 && y <= 6)
         {
            int ix = Max(0, Min(4, (int)x));
            int iy = Max(0, Min(5, (int)y));
            if(iy == ix || iy == ix + 1)
            {
               int rhombus = ix + iy;
               bool top = x - ix > y - iy;
               face = -1;

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
               face++;
            }
         }
      }

      if(face)
      {
         // Degrees faceLon = facesCenterDodecahedronVertices[face].lon;
         int fy = (face-1) / 5, fx = (face-1) - 5*fy;
         double x = position.x - (2*fx + fy/2 + 1) * triWidth/2;
         double y = position.y - (yOffsets[fy] + 3 * centerToBase);
         GeoPoint dst;

         r = icosahedronToSphere({ face - 1, x, y }, dst);
         dst.lat = latAuthalicToGeodetic(cp, dst.lat);

         /*if(fabs((Radians)dst.lon - -Pi) < epsilon && fabs((double)dst.lon - (double)faceLon) > 72)
            dst.lon += 2*Pi;
         else if(fabs((Radians)dst.lon - Pi) < epsilon && fabs((double)dst.lon - (double)faceLon) > 72)
           dst.lon -= 2*Pi;
           */
              if((Radians)dst.lon < -Pi - epsilon) dst.lon += 2*Pi;
         else if((Radians)dst.lon >  Pi + epsilon) dst.lon -= 2*Pi;

         result = { dst.lat, dst.lon };
      }
      return r;
   }

   // Converts geographic coordinates to coordinates on the icosahedron (forward projection)
   bool sphereToIcosahedron(const GeoPoint oc, ISEAFacePoint r)
   {
      bool result = false;
      GeoPoint c;
      int faces[2 + numIcosahedronFaces] = { -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
      int i;
      double sinLat, cosLat;

      applyOrientation(oc, c);

      sinLat = sin(c.lat), cosLat = cos(c.lat);

      guessFace(c, faces);

      for(i = 0; i < 2 + numIcosahedronFaces; i++)
      {
         int face = faces[i];
         if(i < 2 || (faces[i] != faces[0] && faces[i] != faces[1]))
         {
            GeoPoint c0 = facesCenterDodecahedronVertices[face];
            double sinLat0 = sin(c0.lat), cosLat0 = cos(c0.lat);
            Degrees dLon = c.lon - c0.lon;
            double sinDLon = sin(dLon), cosDLon = cos(dLon);
            double cosLatCosDLon = cosLat * cosDLon;
            Degrees Az_earth = atan2(cosLat * sinDLon, cosLat0 * sinLat - sinLat0 * cosLatCosDLon); // Az
            Degrees AzAdjustment = faceOrientation(face);

            Az_earth += AzAdjustment;
            while(Az_earth <     0) AzAdjustment += AzMax, Az_earth += AzMax;
            while(Az_earth > AzMax) AzAdjustment -= AzMax, Az_earth -= AzMax;
            {
               double sinAz_earth = sin(Az_earth), cosAz_earth = cos(Az_earth);
               Degrees z = acos(sinLat0 * sinLat + cosLat0 * cosLatCosDLon);
               Degrees q = atan2(tang, cosAz_earth + sinAz_earth * cotTheta);
               if(z <= q + precision)
               {
                  Degrees H = acos(sinAz_earth * sinGcosSDC2VoS - cosAz_earth * cosG);
                  double area = (Radians)(Az_earth + westVertexLon + H) * R2; // A_G and A_{ABD}
                  Degrees Az = atan2(2 * area, Rprime2Tan2g - area * 2 * cotTheta); // Az'
                  double sinAz = sin(Az), cosAz = cos(Az);
                  Degrees q = atan2(tang, (cosAz_earth + sinAz_earth * cotTheta));
                  double d = RprimeTang / (cosAz + sinAz * cotTheta); // d'
                  double rho = d * sin(z / 2) / sin(q / 2);

                  Az -= AzAdjustment;
                  r = { face = face, x = rho * sin(Az), y = rho * cos(Az) };
                  result = true;
                  break;
               }
            }
         }
      }
      if(!result)
         r = { face = -1 };
      return result;
   }

   // Converts coordinates on the icosahedron to geographic coordinates (inverse projection)
   __attribute__ ((optimize("-fno-unsafe-math-optimizations"))) bool icosahedronToSphere(const ISEAFacePoint c, GeoPoint r)
   {
      if(c.face >= 0 && c.face < numIcosahedronFaces)
      {
         Degrees Az = atan2(c.x, c.y); // Az'
         double rho = sqrt(c.x * c.x + c.y * c.y);
         Degrees AzAdjustment = faceOrientation(c.face);

         Az += AzAdjustment;
         while(Az <     0) AzAdjustment += AzMax, Az += AzMax;
         while(Az > AzMax) AzAdjustment -= AzMax, Az -= AzMax;
         {
            double sinAz = sin(Az), cosAz = cos(Az);
            double cotAz = cosAz / sinAz;
            double area = Rprime2Tan2g / (2 * (cotAz + cotTheta)); // A_G or A_{ABD}
            Degrees deltaAz = 10 * precision;
            Degrees degAreaOverR2Plus180Minus36 = (Degrees) Radians { area / R2 } - westVertexLon;
            Degrees Az_earth = Az;

            // REVIEW: Fixing angular units caused some tests to fail -- possibly use safe asin/ascos approach?
            while(fabs((Radians)deltaAz) > (Radians)precision)
            {
               double sinAzEarth = sin(Az_earth), cosAzEarth = cos(Az_earth);
               Degrees H = acos(sinAzEarth * sinGcosSDC2VoS - cosAzEarth * cosG);
               Degrees FAz_earth = degAreaOverR2Plus180Minus36 - H - Az_earth; // F(Az) or g(Az)
               double F2Az_earth = (cosAzEarth * sinGcosSDC2VoS + sinAzEarth * cosG) / sin(H) - 1; // F'(Az) or g'(Az)
               deltaAz = -FAz_earth / F2Az_earth; // Delta Az^0 or Delta Az
               Az_earth += deltaAz;
            }
            {
               double sinAz_earth = sin(Az_earth), cosAz_earth = cos(Az_earth);
               double q = atan2(tang, (cosAz_earth + sinAz_earth * cotTheta));
               double d = RprimeTang / (cosAz + sinAz * cotTheta); // d'
               Degrees z = 2 * asin(rho / d * sin(q / 2));

               Az_earth -= AzAdjustment;
               {
                  Degrees lat0 = facesCenterDodecahedronVertices[c.face].lat;
                  double sinLat0 = sin(lat0), cosLat0 = cos(lat0);
                  double sinZ = sin(z), cosZ = cos(z);
                  double cosLat0SinZ = cosLat0 * sinZ;
                  Degrees lat = asin(sinLat0 * cosZ + cosLat0SinZ * cos(Az_earth));
                  Degrees lon = facesCenterDodecahedronVertices[c.face].lon + atan2(sin(Az_earth) * cosLat0SinZ, cosZ - sinLat0 * sin(lat));

                  revertOrientation({ lat, lon }, r);
               }
            }
         }
         return true;
      }
      r = { double::inf(), double::inf() };
      return false;
   }

   bool crsExtentFromWGS84(const GeoExtent wgs84Extent, Pointd topLeft, Pointd bottomRight)
   {
      if((Radians)wgs84Extent.ll.lat < -Pi/2 + 0.0001 &&
         (Radians)wgs84Extent.ll.lon < -Pi   + 0.0001 &&
         (Radians)wgs84Extent.ur.lat >  Pi/2 - 0.0001 &&
         (Radians)wgs84Extent.ur.lon >  Pi   - 0.0001)
      {
         topLeft = { 0, 3*centerToBase };
         bottomRight = { 5.5 * triWidth, -6*centerToBase };
      }
      else
      {
         Radians lat, lon;
         Radians maxLon = wgs84Extent.ur.lon + (wgs84Extent.ur.lon < wgs84Extent.ll.lon ? 2*Pi : 0);
         Radians dLat = wgs84Extent.ur.lat - wgs84Extent.ll.lat;
         Radians dLon = maxLon - wgs84Extent.ll.lon;
         Radians latInc = dLat / ANCHORS_5x6, lonInc = dLon / ANCHORS_5x6;
         bool isPlanar = _class == class(ISEAPlanarProjection);
         GeoPoint leftX, topY, rightX, bottomY;

         if(dLon < 0) dLon += 2*Pi;

         topLeft = { MAXDOUBLE, isPlanar ? -MAXDOUBLE : MAXDOUBLE };
         bottomRight = { -MAXDOUBLE, isPlanar ? MAXDOUBLE : -MAXDOUBLE };

         for(lat = wgs84Extent.ll.lat; lat <= wgs84Extent.ur.lat; lat += latInc)
         {
            for(lon = wgs84Extent.ll.lon; lon <= maxLon; lon += lonInc)
            {
               GeoPoint geo { lat, lon };
               Vector3D p;

               if(geoToCartesian(geo, p))
               {
                  if(p.x < topLeft.x) topLeft.x = p.x, leftX = geo;
                  if(p.x > bottomRight.x) bottomRight.x = p.x, rightX = geo;

                  if(isPlanar)
                  {
                     if(p.y > topLeft.y) topLeft.y = p.y, topY = geo;
                     if(p.y < bottomRight.y) bottomRight.y = p.y, bottomY = geo;
                  }
                  else
                  {
                     if(p.y < topLeft.y) topLeft.y = p.y, topY = geo;
                     if(p.y > bottomRight.y) bottomRight.y = p.y, bottomY = geo;
                  }
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
                     Vector3D p;

                     if(geoToCartesian(geo, p))
                     {
                        if(p.x < topLeft.x) topLeft.x = p.x;
                        if(p.x > bottomRight.x) bottomRight.x = p.x;
                        if(isPlanar)
                        {
                           if(p.y > topLeft.y) topLeft.y = p.y;
                           if(p.y < bottomRight.y) bottomRight.y = p.y;
                        }
                        else
                        {
                           if(p.y < topLeft.y) topLeft.y = p.y;
                           if(p.y > bottomRight.y) bottomRight.y = p.y;
                        }
                     }
                  }
               }
            }
         }
      }
      return true;
   }

   void wgs84ExtentFromCRS(const Pointd topLeft, const Pointd bottomRight, GeoExtent wgs84Extent)
   {
      if(0 && topLeft.x < 0.0001 &&
         topLeft.y < 0.0001 &&
         bottomRight.x > 5 - 0.0001 &&
         bottomRight.y > 6 - 0.0001)
      {
         wgs84Extent = wholeWorld;
      }
      else
      {
         double x, y;
         Radians minY = Min((Radians)topLeft.y, (Radians)bottomRight.y);
         Radians maxY = Max((Radians)topLeft.y, (Radians)bottomRight.y);
         double dx = bottomRight.x - topLeft.x;
         double dy = maxY - minY;
         double xi, yi;
         int numAnchors;
         bool addedTop = false, addedBottom = false;
         if(dx < 1E-7 && dy < 1E-7) // REVIEW:
            numAnchors = 1;
         else
            numAnchors = ANCHORS_5x6;

         xi = dx / numAnchors, yi = dy / numAnchors;

         wgs84Extent.ll = { MAXDOUBLE, MAXDOUBLE };
         wgs84Extent.ur = { -MAXDOUBLE, -MAXDOUBLE };

         // Avoid infinite loop due to huge coordinates too close together
         if((yi && maxY && fabs(yi / maxY) < 1E-11) || (xi && bottomRight.x && fabs(xi / bottomRight.x) < 1E-11)) return;

         for(y = minY; y <= maxY + yi * 0.1; y += yi)
         {
            for(x = topLeft.x; x <= bottomRight.x + xi * 0.1; x += xi)
            {
               Vector3D p { x, y };
               GeoPoint geo;

               static const double sr = -sqrt3/2, cr = 0.5;    // sin(-60), cos(-60)
               if(p.x < 0 || (p.x < triWidth / 2 && p.y < 0 && p.y * cr < p.x * sr))
                  p.x += 5 * triWidth; // Wrap around

               if(cartesianToGeo(p, geo))
               {
                  if(geo.lat < wgs84Extent.ll.lat) wgs84Extent.ll.lat = geo.lat;
                  if(geo.lon < wgs84Extent.ll.lon) wgs84Extent.ll.lon = geo.lon;
                  if(geo.lat > wgs84Extent.ur.lat) wgs84Extent.ur.lat = geo.lat;
                  if(geo.lon > wgs84Extent.ur.lon) wgs84Extent.ur.lon = geo.lon;
               }
               else if(!addedTop && p.y > 3*centerToBase - yi)
               {
                  // Ensure inclusion of top vertex
                  cartesianToGeo({ triWidth/2, 3*centerToBase }, geo);
                  if(geo.lat < wgs84Extent.ll.lat) wgs84Extent.ll.lat = geo.lat;
                  if(geo.lon < wgs84Extent.ll.lon) wgs84Extent.ll.lon = geo.lon;
                  if(geo.lat > wgs84Extent.ur.lat) wgs84Extent.ur.lat = geo.lat;
                  if(geo.lon > wgs84Extent.ur.lon) wgs84Extent.ur.lon = geo.lon;
                  addedTop = true;
               }
               else if(!addedBottom && p.y < -6*centerToBase + yi)
               {
                  // Ensure inclusion of bottom vertex
                  cartesianToGeo({ triWidth, -6*centerToBase }, geo);
                  if(geo.lat < wgs84Extent.ll.lat) wgs84Extent.ll.lat = geo.lat;
                  if(geo.lon < wgs84Extent.ll.lon) wgs84Extent.ll.lon = geo.lon;
                  if(geo.lat > wgs84Extent.ur.lat) wgs84Extent.ur.lat = geo.lat;
                  if(geo.lon > wgs84Extent.ur.lon) wgs84Extent.ur.lon = geo.lon;
                  addedBottom = true;
               }
               if(!xi)
                  break;
            }
            if(!yi)
               break;
         }
      }
   }
private:
   GeoPoint orientation;
   double cosOrientationLat, sinOrientationLat;

   GeoPoint vertex1;
   double cosVertex1Lat, sinVertex1Lat;
   Degrees vertex2Azimuth;

   // Applies the selected icosahedron orientation
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
      else if(vertex1.lat != 90 || vertex1.lon)
      {
         Degrees dLon = c.lon - (vertex1.lon + Pi);
         double sinLat = sin(c.lat), cosLat = cos(c.lat);
         double sinDLon = sin(dLon), cosDLon = cos(dLon);
         r = {
            lat = asin(sinVertex1Lat * sinLat - cosVertex1Lat * cosLat * cosDLon),
            lon = atan2(cosLat * sinDLon, sinVertex1Lat * cosLat * cosDLon + cosVertex1Lat * sinLat) + Pi;
         };
      }
      else
         r = c;
      r.lon += vertex2Azimuth;
   }

   // Inverse of applyOrientation()
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
      else if(vertex1.lat != 90 || vertex1.lon)
      {
         Degrees dLon = lon - Pi;
         double sinLat = sin(c.lat), cosLat = cos(c.lat);
         double sinDLon = sin(dLon), cosDLon = cos(dLon);
         r = {
            lat = asin(sinVertex1Lat * sinLat + cosVertex1Lat * cosLat * cosDLon),
            lon = atan2(cosLat * sinDLon, sinVertex1Lat * cosLat * cosDLon - cosVertex1Lat * sinLat) + Pi + vertex1.lon;
         };
      }
      else
         r = { c.lat, lon };
   }

   static inline void ::guessFace(const GeoPoint c, int result[2])
   {
      Degrees lat = c.lat, lon = c.lon;
      if(lat > E - F)
      {
              if (lon < Degrees { -108 }) result[0] =  0, result[1] =  5;
         else if (lon < Degrees {  -36 }) result[0] =  1, result[1] =  6;
         else if (lon < Degrees {   36 }) result[0] =  2, result[1] =  7;
         else if (lon < Degrees {  108 }) result[0] =  3, result[1] =  8;
         else                             result[0] =  4, result[1] =  9;
      }
      else if(lat < F - E)
      {
              if (lon < Degrees { -144 }) result[0] = 19, result[1] = 14;
         else if (lon < Degrees {  -72 }) result[0] = 15, result[1] = 10;
         else if (lon < Degrees {    0 }) result[0] = 16, result[1] = 11;
         else if (lon < Degrees {   72 }) result[0] = 17, result[1] = 12;
         else if (lon < Degrees {  144 }) result[0] = 18, result[1] = 13;
         else                             result[0] = 19, result[1] = 14;
      }
      else
      {
              if (lon < Degrees { -144 }) result[0] =  5, result[1] = 14;
         else if (lon < Degrees { -108 }) result[0] =  5, result[1] = 10;
         else if (lon < Degrees {  -72 }) result[0] =  6, result[1] = 10;
         else if (lon < Degrees {  -36 }) result[0] =  6, result[1] = 11;
         else if (lon < Degrees {    0 }) result[0] =  7, result[1] = 11;
         else if (lon < Degrees {   36 }) result[0] =  7, result[1] = 12;
         else if (lon < Degrees {   72 }) result[0] =  8, result[1] = 12;
         else if (lon < Degrees {  108 }) result[0] =  8, result[1] = 13;
         else if (lon < Degrees {  144 }) result[0] =  9, result[1] = 13;
         else                             result[0] =  9, result[1] = 14;
      }
   }

   static inline Degrees ::faceOrientation(int face)
   {
      return (face <= 4 || (10 <= face && face <= 14)) ? 0 : 180;
   }
}
