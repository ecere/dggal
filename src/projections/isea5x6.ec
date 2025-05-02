public import IMPORT_STATIC "ecere"

private:

import "isea"
import "authalic"
import "ri5x6"
import "icoVertexGreatCircle"

/*
   Icosahedron Snyder equal-area (ISEA) projection
   plane space translated to place the leftmost icosahedron vertex at 0, 0,
   rotated 60 degrees clockwise,
   sheared with a 1.0 / sqrt(3) factor horizontally,
   and scaled by 1.0 / 7674457.9483758880087 m, 1.0 / 6646275.5435688359285 m,
   resulting in a 0..5 x 0..6 space with 10 square tiles where
   int(y) == int(x) || int(y) == int(x) + 1 corresponding to the diamonds
   made up of 2 faces of the icosahedron joined at their base
*/

public class ISEAProjection : SliceAndDiceGreatCircleIcosahedralProjection
{
   radialVertex = isea;

   bool forward(const GeoPoint p, Pointd v)
   {
      Vector3D v3D;
      if(isea5x6PJ.geoToCartesian(p, v3D))
      {
         v = { v3D.x, v3D.y };
         return true;
      }
      else
      {
         v = { };
         return false;
      }
   }

#if 0 // This overrides the Slice and Dice with John Snyder's approach
   bool inverse(const Pointd vertex, GeoPoint geo)
   {
      Pointd v = vertex;

      if(v.y < 0 && v.x <= 1)
      {
         Pointd vv
         {
            x = 5 + v.y;
            y = 5 - (v.x - 0) - v.x + v.y;
         };
         v = vv;
      }
      else if(v.x < 0 && v.y <= 1)
      {
         Pointd vv
         {
            x = 5 + v.x;
            y = 5 + v.y;
         };
         v = vv;
      }
      return isea5x6PJ.cartesianToGeo({ v.x, v.y }, geo);
   }
#endif
}


// TODO: We'll probably want to replace the ISEA5x6Projection and ISEAPlanarProjection classes completely
//       by the Slice & Dice approach in SliceAndDiceGreatCircleIcosahedralProjection

ISEA5x6Projection isea5x6PJ { };

class ISEA5x6Projection : ISEAPlanarProjection
{
public:
   bool geoToCartesian(const GeoPoint position, Vector3D result)
   {
      Vector3D v;
      bool r = ISEAPlanarProjection::geoToCartesian(position, v);
      if(r)
         r = RI5x6Projection::fromIcosahedronNet(v, result);
      return r;
   }

   bool cartesianToGeo(const Vector3D position, GeoPoint result)
   {
      Vector3D v;
      bool r = RI5x6Projection::toIcosahedronNet(position, v);
      if(r)
         r = ISEAPlanarProjection::cartesianToGeo(v, result);
      return r;
   }

   bool crsExtentFromWGS84(const GeoExtent wgs84Extent, Pointd topLeft, Pointd bottomRight)
   {
      if((Radians)wgs84Extent.ll.lat < -Pi/2 + 0.0001 &&
         (Radians)wgs84Extent.ll.lon < -Pi   + 0.0001 &&
         (Radians)wgs84Extent.ur.lat >  Pi/2 - 0.0001 &&
         (Radians)wgs84Extent.ur.lon >  Pi   - 0.0001)
      {
         topLeft = { 0, 0 };
         bottomRight = { 5, 6 };
         return true;
      }
      else
         return ISEAPlanarProjection::crsExtentFromWGS84(wgs84Extent, topLeft, bottomRight);
   }

   // TODO: Move this to base RI5x6Projection class
   void wgs84ExtentFromCRS(const Pointd topLeft, const Pointd bottomRight, GeoExtent wgs84Extent)
   {
      if(topLeft.x < 0.0001 &&
         topLeft.y < 0.0001 &&
         bottomRight.x > 5 - 0.0001 &&
         bottomRight.y > 6 - 0.0001)
         wgs84Extent = wholeWorld;
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

               if(cartesianToGeo(p, geo))
               {
                  if(geo.lat < wgs84Extent.ll.lat) wgs84Extent.ll.lat = geo.lat;
                  if(geo.lon < wgs84Extent.ll.lon) wgs84Extent.ll.lon = geo.lon;
                  if(geo.lat > wgs84Extent.ur.lat) wgs84Extent.ur.lat = geo.lat;
                  if(geo.lon > wgs84Extent.ur.lon) wgs84Extent.ur.lon = geo.lon;
               }
               else if(!addedTop && x - y > 1 - yi)
               {
                  // Ensure inclusion of top vertex
                  cartesianToGeo({ 1, 0 }, geo);
                  if(geo.lat < wgs84Extent.ll.lat) wgs84Extent.ll.lat = geo.lat;
                  if(geo.lon < wgs84Extent.ll.lon) wgs84Extent.ll.lon = geo.lon;
                  if(geo.lat > wgs84Extent.ur.lat) wgs84Extent.ur.lat = geo.lat;
                  if(geo.lon > wgs84Extent.ur.lon) wgs84Extent.ur.lon = geo.lon;
                  addedTop = true;
               }
               else if(!addedBottom && y - x > 1 - yi)
               {
                  // Ensure inclusion of bottom vertex
                  cartesianToGeo({ 0, 2 }, geo);
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
}
