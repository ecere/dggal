public import IMPORT_STATIC "ecere"
private:

import "ISEA9R"
import "ISEA3HSubZones"

#include <stdio.h>

// Goldberg polyhedra: https://en.wikipedia.org/wiki/Goldberg_polyhedron
/*                                                              dk = td (for next step)
ISEA3H                                                          tk for two steps (= dkdk = tdtd); dktk = tkdk                (m + n)^2 − mn       10T + 2
level GP notation Name                             Class       Conway                                                            T                 Count
   0: GP(1,0)     dodecahedron                     1           D     dI                  dI               D                D     1                    12
   1: GP(1,1)     truncated icosahedron            2         dkD     tI                dkdI             dkD              tdD     3                    32
   2: GP(3,0)     truncated pentakis dodecahedron  1         tkD   dktI              dkdkdI           dkdkD            tdtdD     9                    92
   3: GP(3,3)                                      2       tkdkD   tktI            dkdkdkdI         dkdkdkD          tdtdtdD     27                  272
   4: GP(9,0)                                      1       tktkD dktktI          dkdkdkdkdI       dkdkdkdkD        tdtdtdtdD     81                  812
   5: GP(9,9)                                      2     dktktkD tktktI        dkdkdkdkdkdI     dkdkdkdkdkD      tdtdtdtdtdD    243                 2432
*/

public class ISEA3H : DGGRS
{
   // DGGH
   uint64 countZones(int level)
   {
      return (uint64)(10 * pow(3, level) + 2);
   }

   int getMaxDGGRSZoneLevel() { return 33; }
   int getRefinementRatio() { return 3; }

   uint64 countSubZones(ISEA3HZone zone, int depth)
   {
      return zone.getSubZonesCount(depth);
   }

   int getZoneLevel(ISEA3HZone zone)
   {
      return zone.level;
   }

   int countZoneEdges(ISEA3HZone zone) { return zone.nPoints; }

   bool isZoneCentroidChild(ISEA3HZone zone)
   {
      return zone.isCentroidChild;
   }

   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   double getZoneArea(ISEA3HZone zone)
   {
      uint64 zoneCount = countZones(zone.level);
      static double earthArea = 0;
      if(!earthArea) earthArea = wholeWorld.geodeticArea;
      // zoneCount - 12 is the number of hexagons; the 12 pentagons take up the area of 10 hexagons (5/6 * 12)
      return earthArea / (zoneCount - 2) * (zone.nPoints == 5 ? 5/6.0 : 1);
   }

   ISEA3HZone getZoneFromCRSCentroid(int level, CRS crs, const Pointd centroid)
   {
      switch(crs)
      {
         case 0: case CRS { ogc, 153456 }: return ISEA3HZone::fromCentroid(level, centroid);
         case CRS { ogc, 1534 }:
         {
            Vector3D c5x6;
            ISEA5x6Projection::fromISEAPlanar({ centroid.x, centroid.y }, c5x6);
            return ISEA3HZone::fromCentroid(level, { c5x6.x, c5x6.y });
         }
         case CRS { epsg, 4326 }:
         case CRS { ogc, 84 }:
            return (ISEA3HZone)getZoneFromWGS84Centroid(level,
               crs == { ogc, 84 } ?
                  { centroid.y, centroid.x } :
                  { centroid.x, centroid.y });
      }
      return nullZone;
   }

   ISEA3HZone getZoneFromWGS84Centroid(int level, const GeoPoint centroid)
   {
      Vector3D v;
      isea5x6PJ.geoToCartesian(centroid, v);
      return ISEA3HZone::fromCentroid(level, { v.x, v.y });
   }

   void getZoneCRSCentroid(ISEA3HZone zone, CRS crs, Pointd centroid)
   {
      switch(crs)
      {
         case 0: case CRS { ogc, 153456 }: centroid = zone.centroid; break;
         case CRS { ogc, 1534 }:
         {
            Pointd c5x6 = zone.centroid;
            Vector3D c;
            ISEA5x6Projection::toISEAPlanar({c5x6.x, c5x6.y }, c);
            centroid = { c.x, c.y };
            break;
         }
         case CRS { epsg, 4326 }:
         case CRS { ogc, 84 }:
         {
            GeoPoint geo = zone.wgs84Centroid;
            centroid = crs == { ogc, 84 } ?
               { geo.lon, geo.lat } :
               { geo.lat, geo.lon };
            break;
         }
      }
   }

   void getZoneWGS84Centroid(ISEA3HZone zone, GeoPoint centroid)
   {
      centroid = zone.wgs84Centroid;
   }

   void getZoneCRSExtent(ISEA3HZone zone, CRS crs, CRSExtent extent)
   {
      switch(crs)
      {
         case 0: case CRS { ogc, 153456 }: extent = zone.isea5x6Extent; break;
         case CRS { ogc, 1534 }: extent = zone.isea5x6Extent; break;
         case CRS { epsg, 4326 }:
         case CRS { ogc, 84 }:
         {
            GeoExtent geo = zone.wgs84Extent;
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
      extent = zone.wgs84Extent;
   }

   int getZoneNeighbors(ISEA3HZone zone, ISEA3HZone * neighbors, ISEA3HNeighbor * nbType)
   {
      return zone.getNeighbors(neighbors, nbType);
   }

   ISEA3HZone getZoneCentroidParent(ISEA3HZone zone)
   {
      return zone.centroidParent;
   }

   ISEA3HZone getZoneCentroidChild(ISEA3HZone zone)
   {
      return zone.centroidChild;
   }

   int getZoneParents(ISEA3HZone zone, ISEA3HZone * parents)
   {
      return zone.getParents(parents);
   }

   int getZoneChildren(ISEA3HZone zone, ISEA3HZone * children)
   {
      return zone.getChildren(children);
   }

   int getZoneCRSVertices(ISEA3HZone zone, CRS crs, Pointd * vertices)
   {
      uint count = 0, i;

      switch(crs)
      {
         case 0: case CRS { ogc, 153456 }:
            count = zone.getVertices(vertices);
            break;
         case CRS { ogc, 1534 }:
            count = zone.getVertices(vertices);
            for(i = 0; i < count; i++)
            {
               Vector3D v;
               ISEA5x6Projection::toISEAPlanar({ vertices[i].x, vertices[i].y }, v);
               vertices[i] = { v.x, v.y };
            }
            break;
         case CRS { ogc, 84 }:
         case CRS { epsg, 4326 }:
            count = zone.getVertices(vertices);
            for(i = 0; i < count; i++)
            {
               GeoPoint geo;
               isea5x6ToGeo(vertices[i], geo);
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
         isea5x6ToGeo(v5x6[i], vertices[i]);
      return count;
   }

   Array<Pointd> getZoneRefinedCRSVertices(ISEA3HZone zone, CRS crs, int edgeRefinement)
   {
      return zone.getRefinedVertices(crs, edgeRefinement, false);
   }

   Array<GeoPoint> getZoneRefinedWGS84Vertices(ISEA3HZone zone, int edgeRefinement)
   {
      return (Array<GeoPoint>)zone.getRefinedVertices({ epsg, 4326 }, edgeRefinement, true);
   }

   Array<DGGRSZone> listZones(int level, const GeoExtent bbox)
   {
      return listISEA3HZones(level, bbox);
   }

   // Text ZIRS
   void getZoneTextID(ISEA3HZone zone, String zoneID)
   {
      zone.getZoneID(zoneID);
   }

   ISEA3HZone getZoneFromTextID(const String zoneID)
   {
      return ISEA3HZone::fromZoneID(zoneID);
   }

   // Sub-zone Order
   ISEA3HZone getFirstSubZone(ISEA3HZone zone, int depth)
   {
      return zone.getFirstSubZone(depth);
   }

   Array<Pointd> getSubZoneCRSCentroids(ISEA3HZone parent, CRS crs, int depth)
   {
      Array<Pointd> centroids = parent.getSubZoneCentroids(depth);
      if(centroids)
      {
         uint count = centroids.count;
         switch(crs)
         {
            case 0: case CRS { ogc, 153456 }: break;
            case CRS { ogc, 1534 }:
            {
               int i;
               for(i = 0; i < count; i++)
               {
                  Vector3D c;
                  ISEA5x6Projection::toISEAPlanar({ centroids[i].x, centroids[i].y }, c);
                  centroids[i] = { c.x, c.y };
               }
               break;
            }
            case CRS { epsg, 4326 }:
            case CRS { ogc, 84 }:
            {
               int i;
               for(i = 0; i < count; i++)
               {
                  GeoPoint geo;
                  isea5x6ToGeo(centroids[i], geo);
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
            isea5x6ToGeo(centroids[i], geo[i]);
         delete centroids;
      }
      return geo;
   }

   void compactZones(Array<DGGRSZone> zones)
   {
      int maxLevel = 0, i, count = zones.count;
      AVLTree<ISEA3HZone> zonesTree { };

      for(i = 0; i < count; i++)
      {
         ISEA3HZone zone = (ISEA3HZone)zones[i];
         if(zone != nullZone)
         {
            int level = zone.level;
            if(level > maxLevel)
               maxLevel = level;
            zonesTree.Add(zone);
         }
      }

      compactISEA3HZones(zonesTree, maxLevel);
      zones.Free();

      count = zonesTree.count;
      zones.size = count;
      i = 0;
      for(z : zonesTree)
         zones[i++] = z;

      delete zonesTree;
   }
}

enum ISEA3HNeighbor
{
   // The names reflect the planar ISEA projection arrangement
   top,        // Odd level only, except when replacing topLeft/topRight in interruptions for even level
   bottom,     // Odd level only, except when replacing bottomLeft/bottomRight in interruptions for even level
   left,       // Even level only, except when bottomLeft/topLeft is used instead of bottom/top for even level
   right,      // Even level only
   topLeft,
   topRight,
   bottomLeft,
   bottomRight
};

// Public for use in tests...
public class ISEA3HZone : private DGGRSZone
{
public:
   uint levelISEA9R:5:58;  // 0 .. 16
   uint rootRhombus:4:54;  // 0 .. 9
   uint64 rhombusIX:51:3; // (left to right, top to bottom)   0 .. 205,891,132,094,648 (3^16 * 3^16 - 1)
   uint subHex:3:0;       // 0..2 for even level: (1 and 2 for special pole cases)
                        // 3..5 for odd level:  (3,4,5 for normal cases, 6 and 7 for special pole cases)

private:
   property int level
   {
      get { return levelISEA9R * 2 + (subHex > 2); }
   }

   property int nPoints
   {
      get
      {
         int sh = subHex;
         if(sh == 1 || sh == 2 || sh == 6 || sh == 7 || (rhombusIX == 0 && (sh == 0 || sh == 3)))
            return 5; // Polar zones and 0 rhombus index 0 (A and D) are pentagons
         return 6;
      }
   }

   ISEA3HZone ::fromZoneID(const String zoneID)
   {
      ISEA3HZone result = nullZone;
      char levelChar;
      uint root;
      uint64 ix;
      char subHex;
      int row, col, l9r = -1;

      if(sscanf(zoneID, __runtimePlatform == win32 ? "%c%d-%I64X-%c" : "%c%d-%llX-%c",
         &levelChar, &root, &ix, &subHex) == 4)
         l9r = iseaLRCFromLRtI(levelChar, root, ix, &row, &col);
      if(l9r != -1 && validate(l9r, row, col, subHex))
      {
         char id[256];

         result = fromISEA9R(l9r, row, col, subHex);

         result.getZoneID(id);
         if(strcmp(id, zoneID))
            result = nullZone;
      }
      return result;
   }

   // This function generates the proposed ISEA3H DGGS Zone ID string
   // in the form {ISEA9RLevelChar}{RootRhombus}-{HexIndexWithinRootRhombus}-{SubHex}
   void getZoneID(String zoneID)
   {
      if(this == nullZone)
         sprintf(zoneID, "(null)");
      else
      {
         uint l9r = this.levelISEA9R;
         uint root = this.rootRhombus;
         uint64 ix = this.rhombusIX;
         char subHex = (char)(this.subHex + 'A');
         sprintf(zoneID,
            __runtimePlatform == win32 ? "%c%d-%I64X-%c" : "%c%d-%llX-%c",
            'A' + l9r, root, ix, subHex);
      }
   }

   ISEA3HZone ::fromISEA9R(int level, uint row, uint col, char subHex)
   {
      uint64 p = (uint64)pow(3, level);
      uint rowOP = (uint)(row / p), colOP = (uint)(col / p);
      int root = rowOP + colOP;
      int y = (int)(row - rowOP * p), x = (int)(col - colOP * p);
      uint64 ix = y * p + x;

      // Avoid returning bad key
      if(subHex < 'A' || subHex > 'H' || root > 9 || rowOP < colOP || rowOP - colOP > 1 || y >= p || x >= p)
         return nullZone;
      else if(subHex != 'A' && subHex != 'D' && subHex != 'E' && subHex != 'F' &&
              !((subHex == 'B' || subHex == 'G') && row == 0 && col == p-1) &&
              !((subHex == 'C' || subHex == 'H') && col == 4*p && row == 6*p-1))
         return nullZone;
      return { level, root, ix, subHex - 'A'};
   }

   bool ::validate(uint levelISEA9R, uint row, uint col, char subHex)
   {
      uint64 p = (uint64)pow(3, levelISEA9R);
      uint rowOP = (uint)(row / p), colOP = (uint)(col / p);
      int y = (int)(row - rowOP * p), x = (int)(col - colOP * p);
      uint root = rowOP + colOP;
      if(subHex < 'A' || subHex > 'H' || root > 9 || rowOP < colOP || rowOP - colOP > 1 || y >= p || x >= p)
         return false;
      else if(subHex != 'A' && subHex != 'D' && subHex != 'E' && subHex != 'F' &&
              !((subHex == 'B' || subHex == 'G') && row == 0 && col == p-1) &&
              !((subHex == 'C' || subHex == 'H') && col == 4*p && row == 6*p-1))
         return false;
      return true;
   }

   property ISEA3HZone parent0
   {
      get
      {
         int sh = this.subHex;
         if(!levelISEA9R && sh < 3)
            return nullZone;
         else
         {
            ISEA3HZone key { };
            if(sh >= 3)
            {
               key = this;
               key.subHex = sh == 6 ? 1 : sh == 7 ? 2 : 0;
            }
            else
            {
               int row, col, level = iseaLRCFromLRtI((char)('A' + levelISEA9R), rootRhombus, rhombusIX, &row, &col);
               uint64 p = (uint64)pow(3.0, level);
               uint64 r = rhombusIX / p, c = rhombusIX % p;
               uint rm3 = (uint)(r % 3), cm3 = (uint)(c % 3);
               key = fromISEA9R(level - 1, row / 3, col / 3, (char)('A' + (sh == 1 ? 6 : sh == 2 ? 7 : (cm3 > 1 ? 4 : rm3 > 1 ? 5 : 3))));
            }
            return key;
         }
      }
   }

   ISEA3HZone getNeighbor(ISEA3HNeighbor which)
   {
      Pointd centroid = this.centroid;
      int cx = (int)floor(centroid.x + 1E-11);
      int cy = (int)floor(centroid.y + 1E-11);
      bool south = centroid.y - centroid.x - 1E-11 > 1; // Not counting pentagons as south or north
      bool north = centroid.x - centroid.y - 1E-11 > 0;
      bool northPole = north && fabs(centroid.x - centroid.y - 1.0) < 1E-11;
      bool southPole = south && fabs(centroid.y - centroid.x - 2.0) < 1E-11;
      uint l9r = levelISEA9R;
      uint64 p = (uint64)pow(3, l9r);
      double d = 1.0 / p, x = 0, y = 0;
      int sh = subHex;
      Pointd v;
      bool crossEarly = true;

      if(sh < 3)
      {
         // Even level

         // NOTE: See getNeighbors() for special interruption cases
         switch(which)
         {
            case top:
               if(south && centroid.x - cx < 1E-11)
               {
                  crossEarly = false;
                  if(southPole)
                     x = -3, y = -3-d;
                  else // Extra top neighbor at south interruptions
                     y = -d;
               }
               break;
            case bottom:
               if(north && centroid.y - cy < 1E-11)
               {
                  crossEarly = false;
                  if(northPole)
                     x = 2-d, y = 2;
                  else // Extra bottom neighbor at north interruptions
                     x = -d;
               }
               break;
            case left:        x = -d, y = -d; break;
            case right:       x =  d, y =  d; break;
            case topLeft:
               if(northPole)
                  crossEarly = false, x = 3-d, y = 3;
               else if(southPole)
                  crossEarly = false, y = -d;
               else
                  y = -d;
               break;
            case bottomLeft:
               if(southPole)
                  crossEarly = false, x = -2, y = -2-d;
               else
                  x = -d;
               break;
            case topRight:
               if(northPole)
                  crossEarly = false, x = 4-d, y = 4;
               else if(southPole)
                  crossEarly = false, x = -4, y = -d - 4;
               else
                  x = d;
               break;
            case bottomRight:
               if(southPole)
                  crossEarly = false, x = -1, y = -1-d;
               else
                  y =  d;
               break;
         }
      }
      else
      {
         // Odd level
         double do3 = d/3;

         // NOTE: See getNeighbors() for special interruption cases
         switch(which)
         {
            case top:
               if(southPole)
                  x =   do3 - 5, y = -do3 - 5, crossEarly = false;
               else if(!northPole)
                  x =   do3, y = -do3;
               break;
            case bottom:
               if(northPole)
                  x = 1-do3, y = 1+do3, crossEarly = false;
               else if(!southPole)
                  x =  -do3, y =  do3;
               break;
            case topLeft:
               if(northPole)
                  x = 2-do3, y = 2+do3, crossEarly = false;
               else if(southPole)
                  x =   do3, y = -do3;
               else
                  x = -do3, y =-2*do3;
               break;
            case bottomLeft:
               if(northPole)
                  x = 4-do3, y = 4+do3, crossEarly = false;
               else if(southPole)
                  x = do3 - 2, y = -do3 - 2;
               else
                  x = -2*do3, y = -do3;
               break;
            case topRight:
               if(northPole)
                  x = 3-do3, y = 3+do3, crossEarly = false;
               else if(southPole)
                  x = do3 - 4, y = -do3 - 4;
               else
                  x =  2*do3, y = do3;
               break;
            case bottomRight:
               if(northPole)
                  x = 5-do3, y = 5+do3, crossEarly = false;
               else if(southPole)
                  x = do3 - 1, y = -do3 - 1;
               else
                  x = do3, y = 2*do3;
               break;
            case right: // Currently stand-in for second bottom / top neighbor
               // Extra bottom neighbor at north interruptions
               if(north && !northPole && centroid.y - cy < 1E-11)
                  crossEarly = false, y = do3, x = -do3;
               // Extra bottom neighbor at south interruptions
               else if(south && !southPole && centroid.x - cx < 1E-11)
                  crossEarly = false, x = do3, y = -do3;
               break;
         }
      }
      if(x || y)
      {
         ISEA3HZone result;
         // REVIEW: This is the only place we use moveISEAVertex2()
         moveISEAVertex2(v, centroid, x, y, crossEarly);
         result = fromCentroid(2*l9r + (sh >= 3), v);
         if(result == this)
            return nullZone; // This should not happen
         return result;
      }
      else
         return nullZone;
      /*
      TODO: more direct path?
      int row, col, level = iseaLRCFromLRtI((char)('A' + levelISEA9R), rootRhombus, rhombusIX, &row, &col);
      int sh = 0;
      col += x, row += y;
      return fromISEA9R(level, row, col, (char)('A' + sh));
      */
   }

   int getNeighbors(ISEA3HZone neighbors[6], ISEA3HNeighbor i3hNB[6])
   {
      int numNeighbors = 0;
      ISEA3HNeighbor n;
      ISEA3HNeighbor localNB[6];

      if(i3hNB == null) i3hNB = localNB;

      for(n = 0; n < ISEA3HNeighbor::enumSize; n++)
      {
         ISEA3HZone nb = getNeighbor(n);
         if(nb != nullZone)
         {
            ISEA3HNeighbor which = n;
            if(numNeighbors)
            {
               // Handle special cases here so that getNeighbor()
               // can still return same neighbor for multiple directions
               if(n == topRight && i3hNB[numNeighbors-1] == topLeft && neighbors[numNeighbors-1] == nb)
               {
                  i3hNB[numNeighbors-1] = top;
                  continue;
               }
               else if(n == bottomRight && i3hNB[numNeighbors-1] == bottomLeft && neighbors[numNeighbors-1] == nb)
               {
                  i3hNB[numNeighbors-1] = bottom;
                  continue;
               }
               else if(n == topRight && i3hNB[numNeighbors-1] != topLeft)
                  which = top;
               else if(n == bottomRight && i3hNB[numNeighbors-1] != bottomLeft)
                  which = bottom;
            }
            i3hNB[numNeighbors] = which;
            neighbors[numNeighbors++] = nb;
         }
      }
      return numNeighbors;
   }

   property ISEA3HZone centroidParent
   {
      get
      {
         ISEA3HZone cParent = parent0;
         if(cParent != nullZone && cParent.isCentroidChild)
            return cParent; // At least one of vertex children's parent is a centroid child,
                            // but few centroid child parent are themselves a centroid child
         else
         {
            // TODO: directly compute centroid parent?
            ISEA3HZone parents[3];
            int n = getParents(parents), i;

            for(i = 1; i < n; i++)
               if(parents[i].isCentroidChild)
                  return parents[i];
            return nullZone;
         }
      }
   }

   int getContainingGrandParents(ISEA3HZone cgParents[3])
   {
      ISEA3HZone cParent = centroidParent;
      int n;

      if(isCentroidChild)
      {
         if(cParent != nullZone)
            cgParents[0] = cParent.parent0, n = 1;
         else
            n = parent0.getParents(cgParents);
      }
      else
         n = cParent.getParents(cgParents);

#ifdef _DEBUG
      if(n != 3 && n != 1)
         Print("WARNING: Wrong assumption");
#endif
      return n;
   }

   int getParents(ISEA3HZone parents[3])
   {
      ISEA3HZone parent0 = this.parent0;

      parents[0] = parent0;
      if(isCentroidChild)
         return parent0 == nullZone ? 0 : 1;
      else
      {
         int sh = subHex;

         if(sh >= 3)
         {
            // Odd level
            parents[1] = parent0.getNeighbor(right);
            parents[2] = parent0.getNeighbor(sh == 4 /* E */ ? topRight : bottomRight /* F */);
         }
         else
         {
            // Even level
            Pointd centroid = this.centroid;
            Pointd p0Centroid = parent0.centroid;
            double dx = centroid.x - p0Centroid.x;
            double dy = centroid.y - p0Centroid.y;
            int p0cx = (int)floor(p0Centroid.x + 1E-11);
            int p0cy = (int)floor(p0Centroid.y + 1E-11);
            bool onBottomCrossingLeft = p0Centroid.y - p0Centroid.x + 1E-11 > 1 && p0Centroid.x - p0cx < 1E-11;
            bool onTopCrossingRight = p0Centroid.x - p0Centroid.y + 1E-11 > 0 && p0Centroid.y - p0cy < 1E-11;

            if(fabs(dx) < 1E-11)
            {
               if(dy > 0)
               {
                  // Bottom-Right vertex child of p0
                  parents[1] = parent0.getNeighbor(bottomRight);
                  parents[2] = parent0.getNeighbor(onBottomCrossingLeft ? bottomLeft : bottom);
               }
               else
               {
                  // Top-Left vertex child of p0
                  parents[1] = parent0.getNeighbor(topLeft);
                  parents[2] = parent0.getNeighbor(top);
               }
            }
            else if(fabs(dy) < 1E-11)
            {
               if(dx > 0)
               {
                  // Top-Right vertex child of p0
                  parents[1] = parent0.getNeighbor(topRight);
                  parents[2] = parent0.getNeighbor(onTopCrossingRight ? topLeft : top);
               }
               else
               {
                  // Bottom-Left vertex child of p0
                  parents[1] = parent0.getNeighbor(bottomLeft);
                  parents[2] = parent0.getNeighbor(bottom);
               }
            }
            else
            {
               if(dx > 0)
               {
                  // Right vertex child of p0
                  parents[1] = parent0.getNeighbor(topRight);
                  parents[2] = parent0.getNeighbor(bottomRight);
               }
               else
               {
                  // Left vertex child of p0
                  parents[1] = parent0.getNeighbor(topLeft);
                  parents[2] = parent0.getNeighbor(bottomLeft);
               }
            }
         }
         return 3;
      }
   }

   ISEA3HZone ::fromCentroid(uint level, const Pointd centroid) // in ISEA5x6 CRS
   {
      Pointd c = centroid;
      uint l9r = level / 2;
      uint64 p = (uint64)pow(3, l9r);
      double d =  1.0 / p;
      bool isNorthPole = false, isSouthPole = false;
      if(fabs(c.x - c.y - 1) < 1E-10)
         isNorthPole = true;
      else if(fabs(c.y - c.x - 2) < 1E-10)
         isSouthPole = true;
      else if(c.y < -1E-11 && c.x > -1E-11)
         c.x -= c.y, c.y = 0;
      /*
      else if(c.y > 5.0 + 1E-11 && c.x < 4.0 + 1E-11) // B7-7-A -> B9-3-A, B9-6-A, but what about B5-3-A ?
         c.x += (c.y - 5.0), c.y = 5.0; // REVIEW: This may no longer be necessary?
      */
      else if((int)floor(c.x + 1E-11) > (int)floor(c.y + 1E-11))
      {
         // Over top dent to the right
         int cy = Min(5, (int)floor(c.y + 1E-11));
         c.x += (cy+1 - c.y), c.y = cy+1;
      }
      else if((int)floor(c.y + 1E-11) - (int)floor(c.x + 1E-11) > 1)
      {
         // Over bottom dent to the right -- REVIEW: This may no longer be necessary?
         int cx = Min(4, (int)floor(c.x + 1E-11));
         c.y += (cx+1 - c.x), c.x = cx+1;
      }
      else if(c.x < -1E-11 || c.y < -1E-11)
         moveISEAVertex(c, { 5, 5 }, c.x, c.y);
      if(c.x > 5 - 1E-11 && c.y > 5 - 1E-11 &&  // This handles bottom right wrap e.g., A9-0E and A9-0-F
         c.x + c.y > 5.0 + 5.0 - d - 1E-11)
         c.x -= 5, c.y -= 5;
      {
         int cx = Min(4, (int)(c.x + 1E-11)), cy = Min(5, (int)(c.y + 1E-11));  // Coordinate of root rhombus
         uint root = cx + cy;
         uint64 x = (uint64)((c.x - cx) * p + 1E-6 /*11*/); // Row and Column within root rhombus
         uint64 y = (uint64)((c.y - cy) * p + 1E-6 /*11*/);
         uint64 rix = y * p + x;  // Index within root rhombus
         double xd = (c.x - cx) * p - x;
         double yd = (c.y - cy) * p - y;
         uint sh;

         if(isNorthPole)
            sh = (level & 1) ? 6 : 1, root = 0, rix = p-1;
         else if(isSouthPole)
            sh = (level & 1) ? 7 : 2, root = 9, rix = p * (p-1);
         else
         {
            if(cy < cx || xd < -1 || yd < -1 || x >= p || y >= p || rix >= p*p)
               return nullZone; // y cannot be smaller than x

            // PrintLn("   rix: ", rix, ", x: ", x, ", y: ", y, ", xd: ", xd, ", yd: ", yd);
            if(level & 1) // Odd level
            {
               if(root == 0 && y == 0 && x == p - 1 && xd > 0.667 && yd < 0.332)
                  sh = 6; // "North" pole G
               else if(root == 9 && y == p-1 && x == 0 && yd > 0.667 && xd < 0.332)
                  sh = 7; // "South" pole H
               else if(xd > 0.334)
                  sh = 4; // E
               else if(yd > 0.334)
                  sh = 5; // F
               else
                  sh = 3; // D
            }
            else          // Even level
            {
               if(root == 0 && y == 0 && x == p-1 && xd > 0.667 && yd < 0.332)
                  sh = 1; // "North" pole B
               else if(root == 9 && y == p-1 && x == 0 && xd < 0.332 && yd > 0.667)
                  sh = 2; // "South" pole C
               else
                  sh = 0; // A
            }
         }
         return { l9r, root, rix, sh };
      }
   }

   int getVertices(Pointd * vertices)
   {
      int sh = subHex;
      uint level = levelISEA9R, root = rootRhombus;
      uint64 rix = rhombusIX;
      uint64 p = (uint64)pow(3, level);
      uint64 rowOP = (root + 1) >> 1, colOP = root >> 1;
      uint64 ixOP = (uint64)(rix / p);
      uint64 row = (uint64)(rowOP * p + ixOP);
      uint64 col = (uint64)((colOP - ixOP) * p + rix); // distributivity on: rix - (ixOP * p) for (rix % p)
      double d =  1.0 / p;
      Pointd tl { col * d, row * d };
      int i = 0;
      bool south = root & 1;
      Pointd v;

      switch(sh)
      {
         case 0:  // Even level -- regular
            moveISEAVertex(vertices[i++], tl,  2*d/3,    d/3);
            moveISEAVertex(vertices[i++], tl,    d/3,  2*d/3);
            if(!south || rix) // 0 rhombusIndex are pentagons
               moveISEAVertex(vertices[i++], tl, -  d/3,    d/3);
            moveISEAVertex(vertices[i++], tl, -2*d/3, -  d/3);
            if(south || rix) // 0 rhombusIndex are pentagons
               moveISEAVertex(vertices[i++], tl, -  d/3, -2*d/3);
            moveISEAVertex(vertices[i++], tl,    d/3, -  d/3);
            break;
         case 1:  // Even level -- "North" pole
            if(sh == 1 && row == 0 && col == p-1)
            {
               moveISEAVertex(v, tl, d/3, -d/3);
               vertices[i++] = { v.x + 1, v.y + 1 };
               vertices[i++] = { v.x + 2, v.y + 2 };
               vertices[i++] = { v.x + 3, v.y + 3 };
               vertices[i++] = { v.x + 4, v.y + 4 };
               vertices[i++] = { v.x + 5, v.y + 5 };
            }
            break;
         case 2: // Even level -- "South" pole
            if(col == 4*p && row == 6*p-1)
            {
               moveISEAVertex(v, tl, -d/3, d/3);
               vertices[i++] = { v.x - 0, v.y - 0 };
               vertices[i++] = { v.x - 1, v.y - 1 };
               vertices[i++] = { v.x - 2, v.y - 2 };
               vertices[i++] = { v.x - 3, v.y - 3 };
               vertices[i++] = { v.x - 4, v.y - 4 };
            }
            break;
         case 3:  // Odd level -- type D
            if(south || rix) // 0 rhombusIndex are pentagons
               moveISEAVertex(vertices[i++], tl, d/3,0);
            moveISEAVertex(vertices[i++], tl, d/3, d/3);
            moveISEAVertex(vertices[i++], tl,0, d/3);
            if(!south || rix) // 0 rhombusIndex are pentagons
               moveISEAVertex(vertices[i++], tl,-d/3,    0);
            moveISEAVertex(vertices[i++], tl,-d/3,-d/3);
            moveISEAVertex(vertices[i++], tl,    0,-d/3);
            break;
         case 4:  // Odd level -- type E
            moveISEAVertex(vertices[i++], tl,  d/3,0);
            moveISEAVertex(vertices[i++], tl,2*d/3,0);
            moveISEAVertex(vertices[i++], tl,    d,  d/3);
            moveISEAVertex(vertices[i++], tl,    d,2*d/3);
            moveISEAVertex(vertices[i++], tl,2*d/3,2*d/3);
            moveISEAVertex(vertices[i++], tl,  d/3,  d/3);
            break;
         case 5:  // Odd level -- type F
            moveISEAVertex(vertices[i++], tl,0,   d/3);
            moveISEAVertex(vertices[i++], tl, d/3,   d/3);
            moveISEAVertex(vertices[i++], tl,2*d/3,2*d/3);
            moveISEAVertex(vertices[i++], tl,2*d/3,    d);
            moveISEAVertex(vertices[i++], tl,  d/3,    d);
            moveISEAVertex(vertices[i++], tl,0,2*d/3);
            break;
         case 6:  // Odd level -- "North" pole
            if(row == 0 && col == p-1)
            {
               moveISEAVertex(v, tl, 2*d/3, 0);
               // vertices[i++] = { v.x + 1 - d/3, v.y + 1 - d/3 };
               vertices[i++] = { v.x + 1, v.y + 1 };
               vertices[i++] = { v.x + 2, v.y + 2 };
               vertices[i++] = { v.x + 3, v.y + 3 };
               vertices[i++] = { v.x + 4, v.y + 4 };
               vertices[i++] = { v.x + 5, v.y + 5 };
            }
            break;
         case 7:  // Odd level -- "South" pole
            if(col == 4*p && row == 6*p-1)
            {
               moveISEAVertex(v, tl, d/3, d);
               vertices[i++] = { v.x - 0, v.y - 0 };
               vertices[i++] = { v.x - 1, v.y - 1 };
               vertices[i++] = { v.x - 2, v.y - 2 };
               vertices[i++] = { v.x - 3, v.y - 3 };
               vertices[i++] = { v.x - 4, v.y - 4 };
               // vertices[i++] = { v.x - 4 - d/3, v.y - 4 - d/3 };
            }
            break;
      }
      return i;
   }

   Array<Pointd> getRefinedVertices(CRS crs, int edgeRefinement, bool useGeoPoint)   // 0 for 1-20 based on level
   {
      Array<Pointd> rVertices = null;
      Pointd vertices[9];
      int numPoints = 0;
      int i;
      int row, col, level = iseaLRCFromLRtI((char)('A' + levelISEA9R), rootRhombus, rhombusIX, &row, &col);
      int subHex = this.subHex;
      bool result = true;
      uint64 p = (uint64)pow(3, level);
      double d =  1.0 / p;
      bool crs84 = crs == CRS { ogc, 84 } || crs == CRS { epsg, 4326 };
      Pointd v;
      Pointd tl = ISEA9RZone { level, row, col }.isea5x6Extent.tl;

      //static const double sqrt3_2 = 0.8660254037844;  // a = √3/2 × s  (0.5 / tan(30°))
      // double a = d, s = a / sqrt3_2;

      switch(subHex)
      {
         case 0:  // Even level -- regular A
            moveISEAVertex(vertices[numPoints++], tl,  2*d/3,    d/3);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in moveISEAVertex()?
            moveISEAVertex(vertices[numPoints++], tl,    d/3,  2*d/3);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in moveISEAVertex()?
            moveISEAVertex(vertices[numPoints++], tl, -  d/3,    d/3);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in moveISEAVertex()?
            moveISEAVertex(vertices[numPoints++], tl, -2*d/3, -  d/3);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in moveISEAVertex()?
            moveISEAVertex(vertices[numPoints++], tl, -  d/3, -2*d/3);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in moveISEAVertex()?
            moveISEAVertex(vertices[numPoints++], tl,    d/3, -  d/3);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in moveISEAVertex()?
            break;
         case 1:  // Even level -- "North" pole B
            if(row == 0 && col == p-1)
            {
               moveISEAVertex(v, tl, d/3, -d/3);

               // These are the pentagon's 5 vertices
               vertices[numPoints++] = { v.x + 5, v.y + 5 };

               if(!crs84)
               {
                  // Trapezoidal cap
                  vertices[numPoints++] = { 5, v.y + 5 + 0.5 * d/3 };
                  vertices[numPoints++] = { 5, 4 }; // This is the "north" pole
                  vertices[numPoints++] = { 1, 0 }; // This is also the "north" pole
                  vertices[numPoints++] = { v.x + 1 - 0.5*d/3, 0 };

                  /*
                  // Rectangular cap
                  vertices[numPoints++] = { 5, 4 }; // "North" pole
                  vertices[numPoints++] = { 0, -1 }; // Also "North" pole
                  vertices[numPoints++] = { v.x, v.y }; // Extra vertex to fill polygon
                  */
               }
               vertices[numPoints++] = { v.x + 1, v.y + 1 };
               vertices[numPoints++] = { v.x + 2, v.y + 2 };
               vertices[numPoints++] = { v.x + 3, v.y + 3 };
               vertices[numPoints++] = { v.x + 4, v.y + 4 };
            }
            else
               result = false;
            break;
         case 2:  // Even level -- "South" pole C
            if(col == 4*p && row == 6*p-1)
            {
               moveISEAVertex(v, tl, -d/3, d/3);
               vertices[numPoints++] = { v.x - 0, v.y - 0 };
               vertices[numPoints++] = { v.x - 1, v.y - 1 };
               vertices[numPoints++] = { v.x - 2, v.y - 2 };
               vertices[numPoints++] = { v.x - 3, v.y - 3 };
               if(!crs84)
               {
                  // Trapezoidal cap
                  vertices[numPoints++] = { 0, v.y - 3 - 0.5 * d/3 };
                  vertices[numPoints++] = { 0, 2 }; // "South" pole
                  vertices[numPoints++] = { 4, 6 }; // Also "South" pole
                  vertices[numPoints++] = { v.x + 1 + 0.5 *d/3, 6 };

                  // Rectangular cap Extra vertices to fill polygon in ISEA CRSs
                  /*
                  vertices[numPoints++] = { v.x - 4, v.y - 4 };
                  vertices[numPoints++] = { -1, 1 }; // "South" pole
                  vertices[numPoints++] = { 4, 6 }; // Also "South" pole
                  */
               }
               vertices[numPoints++] = { v.x + 1,  v.y + 1 };
            }
            else
               result = false;
            break;
         case 3:  // Odd level -- type D
            if(crs84)
            {
               moveISEAVertex(vertices[numPoints++], tl, d/3,0);
               if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
                  vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in moveISEAVertex()?
            }
            else
            {
               moveISEAVertex(vertices[numPoints++], tl, d/3,-2E-11);
               moveISEAVertex(vertices[numPoints++], tl, d/3,2E-11);
            }
            moveISEAVertex(vertices[numPoints++], tl, d/3, d/3);
            if(crs84)
            {
               moveISEAVertex(vertices[numPoints++], tl, 0, d/3);
               if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
                  vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in moveISEAVertex()?
            }
            else
            {
               moveISEAVertex(vertices[numPoints++], tl,2E-11, d/3);
               moveISEAVertex(vertices[numPoints++], tl,-2E-11, d/3);
            }
            moveISEAVertex(vertices[numPoints++], tl,-d/3,    0);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in moveISEAVertex()?
            moveISEAVertex(vertices[numPoints++], tl,-d/3,-d/3);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in moveISEAVertex()?
            moveISEAVertex(vertices[numPoints++], tl,    0,-d/3);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in moveISEAVertex()?
            break;
         case 4:  // Odd level -- type E
            moveISEAVertex(vertices[numPoints++], tl,  d/3,2E-11);
            moveISEAVertex(vertices[numPoints++], tl,2*d/3,2E-11);
            moveISEAVertex(vertices[numPoints++], tl,    d,  d/3);
            moveISEAVertex(vertices[numPoints++], tl,    d,2*d/3);
            moveISEAVertex(vertices[numPoints++], tl,2*d/3,2*d/3);
            moveISEAVertex(vertices[numPoints++], tl,  d/3,  d/3);
            break;
         case 5:  // Odd level -- type F
            moveISEAVertex(vertices[numPoints++], tl,2E-11,   d/3);
            moveISEAVertex(vertices[numPoints++], tl, d/3,   d/3);
            moveISEAVertex(vertices[numPoints++], tl,2*d/3,2*d/3);
            moveISEAVertex(vertices[numPoints++], tl,2*d/3,    d);
            moveISEAVertex(vertices[numPoints++], tl,  d/3,    d);
            moveISEAVertex(vertices[numPoints++], tl,2E-11,2*d/3);
            break;
         case 6:  // Odd level -- "North" pole G
            if(row == 0 && col == p-1)
            {
               moveISEAVertex(v, tl, 2*d/3, 0);
               // These are the pentagon's 5 vertices
               // vertices[numPoints++] = { v.x + 1 - d/3, v.y + 1 - d/3 }; -- For version before fix that crossed the interruption
               vertices[numPoints++] = { v.x + 0, v.y + 0 };
               vertices[numPoints++] = { v.x + 1, v.y + 1 };
               vertices[numPoints++] = { v.x + 2, v.y + 2 };
               vertices[numPoints++] = { v.x + 3, v.y + 3 };
               vertices[numPoints++] = { v.x + 4, v.y + 4 };
               if(!crs84)
               {
                  // Extra vertices to fill polygon in ISEA CRSs
                  vertices[numPoints++] = { 5, 4 + d/3 }; // This extends to right border of last triangle
                  vertices[numPoints++] = { 5, 4 }; // This is the "north" pole
                  vertices[numPoints++] = { 1, 0 }; // This is also the "north" pole
               }
            }
            else
               result = false;
            break;
         case 7:
            if(col == 4*p && row == 6*p-1)
            {
               // Odd level -- "South" pole H
               moveISEAVertex(v, tl, d/3, d);
               // These are the pentagon's 5 vertices
               vertices[numPoints++] = { v.x - 0, v.y - 0 };
               vertices[numPoints++] = { v.x - 1, v.y - 1 };
               vertices[numPoints++] = { v.x - 2, v.y - 2 };
               vertices[numPoints++] = { v.x - 3, v.y - 3 };
               vertices[numPoints++] = { v.x - 4, v.y - 4 };
               if(!crs84)
               {
                  // Extra vertices to fill polygon in ISEA CRSs
                  vertices[numPoints++] = { 0, 2 - d/3 }; // This extends to the left wrapping point
                  vertices[numPoints++] = { 0, 2 }; // This is the "south" pole
                  vertices[numPoints++] = { 4, 6 }; // This is also the "south" pole
               }
            }
            else
               result = false;
            break;
         default:
            result = false;
      }

      if(result)
      {
         Array<Pointd> ap = null;
         bool geodesic = false; //true;
         bool refine = subHex < 3;  // Only use refinement for ISEA for even levels

         if(crs84)
         {
            GeoExtent e = this.approxWGS84Extent; // REVIEW: Currently only used to decide whether to wrap
            GeoPoint centroid = this.wgs84Centroid;
            Radians dLon = (Radians)e.ur.lon - (Radians)e.ll.lon;
            bool wrap = (dLon < 0 || e.ll.lon > centroid.lon || dLon > Pi);
            int lonQuad = (int)(((Radians)centroid.lon + Pi) * (4 / (2*Pi)));

            if(geodesic)
            {
               ap = { size = numPoints };
               for(i = 0; i < numPoints; i++)
               {
                  GeoPoint point;
                  isea5x6ToGeo(vertices[i], point);
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
                  if(isea5x6ToGeo(r[i], point))
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
               if(crs == CRS { ogc, 1534 })
               {
                  Vector3D v { };
                  isea5x6PJ.toISEAPlanar({ r[i].x, r[i].y }, v);
                  ap[i] = { v.x, v.y };
               }
               else if(!crs || crs == CRS { ogc, 153456 })
                  ap[i] = { r[i].x, r[i].y };
            }
            delete r;
         }
         else
         {
            ap = { size = numPoints };
            for(i = 0; i < numPoints; i++)
            {
               if(crs == CRS { ogc, 1534 })
               {
                  Vector3D v { };
                  isea5x6PJ.toISEAPlanar({ vertices[i].x, vertices[i].y }, v);
                  ap[i] = { v.x, v.y };
               }
               else if(!crs || crs == CRS { ogc, 153456 })
                  ap[i] = { vertices[i].x, vertices[i].y };
            }
         }
         rVertices = ap;
      }
      return rVertices;
   }

   property ISEA3HZone centroidChild
   {
      get
      {
         if(this == nullZone)
            return nullZone;
         else
         {
            ISEA3HZone centroidChild;
            uint l9r = levelISEA9R, sh = subHex;
            uint root = rootRhombus;
            uint64 rix = rhombusIX;

            if(sh == 0) // Centroid child for Even level
               centroidChild = { l9r, root, rix, 3 };
            else
            {
               uint64 p = (uint64)pow(3.0, l9r);
               switch(sh)
               {
                  case 1: centroidChild = { l9r, 0, p-1, 6 }; break; // Even level "North" Pole
                  case 2: centroidChild = { l9r, 9, p*(p-1), 7 }; break; // Even level "South" Pole
                  case 6: centroidChild = { l9r+1, 0, 3*p-1, 1 }; break; // Odd level "North" Pole
                  case 7: centroidChild = { l9r+1, 9, 3*p*(3*p-1), 2 }; break; // Odd level "South" Pole
                  default:
                  {
                     // Centroid child for Odd level
                     int rowOP = (root + 1) >> 1, colOP = root >> 1, ixOP = (int)(rix / p);
                     int row = (int)(rowOP * p + ixOP), col = (int)((colOP - ixOP) * p + rix); // distributivity on: ix - (ixOP * p) for (ix % p)
                     int r = row * 3 + ((sh == 5) ? 2 : (sh == 4) ? 1 : 0);
                     int c = col * 3 + ((sh == 4) ? 2 : (sh == 5) ? 1 : 0);
                     centroidChild = ISEA3HZone::fromISEA9R(l9r + 1, r, c, 'A');
                     break;
                  }
               }
            }
            return centroidChild;
         }
      }
   }

   int getChildren(ISEA3HZone children[7])
   {
      uint l9r = levelISEA9R, sh = subHex;
      int i = 0;

      children[i++] = centroidChild;

      if(sh == 1 || sh == 2 || sh == 6 || sh == 7)
      {
         // Special cases for the poles
         uint64 p = (uint64)pow(3.0, l9r);
         switch(sh)
         {
            case 1: // Even level "North" Pole
               for(; i < 6; i++)
                  children[i] = { l9r, (i-1)*2, p-1, 4 };
               break;
            case 2: // Even level "South" Pole
               for(; i < 6; i++)
                  children[i] = { l9r, (i-1)*2 + 1, p*(p-1), 5 };
               break;
            case 6: // Odd level "North" Pole
               for(; i < 6; i++)
                  children[i] = { l9r+1, (i-1)*2, 3*p-1, 0 };
               break;
            case 7: // Odd level "South" Pole
               for(; i < 6; i++)
                  children[i] = { l9r+1, (i-1)*2+1, 3*p*(3*p-1), 0 };
               break;
         }
      }
      else
      {
         Pointd vertices[6];
         int nVertices = getVertices(vertices);
         int nextLevel = 2*l9r+1 + (sh > 0);

         for(; i < nVertices + 1; i++)
            children[i] = fromCentroid(nextLevel, vertices[i-1]);
      }
      return i;
   }

   property GeoExtent approxWGS84Extent
   {
      get
      {
         int sh = subHex;
         int i;
         GeoPoint centroid = this.wgs84Centroid;
         Radians minDLon = 99999, maxDLon = -99999;
         Pointd vertices[7];  // REVIEW: Should this be 6? can't ever be 7?
         int nVertices = getVertices(vertices);

         value.clear();
         for(i = 0; i < nVertices; i++)
         {
            Pointd * cv = &vertices[i];
            GeoPoint p;
            if(isea5x6ToGeo(cv, p))
            {
               Radians dLon = p.lon - centroid.lon;

               if(dLon > Pi) dLon -= 2* Pi;
               if(dLon < -Pi) dLon += 2* Pi;

               if(p.lat > value.ur.lat) value.ur.lat = p.lat;
               if(p.lat < value.ll.lat) value.ll.lat = p.lat;

               if(dLon > maxDLon)
                  maxDLon = dLon, value.ur.lon = p.lon;
               if(dLon < minDLon)
                  minDLon = dLon, value.ll.lon = p.lon;
            }
         }

         if(sh == 1 || sh == 6)
         {
            // "North" pole
            value.ll.lon = -Pi;
            value.ur.lon = Pi;
            value.ur.lat = Pi/2;
         }
         else if(sh == 2 || sh == 7)
         {
            // "South" pole
            value.ll.lon = -Pi;
            value.ur.lon = Pi;
            value.ll.lat = -Pi/2;
         }
      }
   }

   property GeoExtent wgs84Extent
   {
      get
      {
         int i;
         GeoPoint centroid = this.wgs84Centroid;
         Radians minDLon = 99999, maxDLon = -99999;
         Array<GeoPoint> vertices = (Array<GeoPoint>)getRefinedVertices({ epsg, 4326 }, 0, true);
         int nVertices = vertices ? vertices.count : 0;

         value.clear();
         for(i = 0; i < nVertices; i++)
         {
            GeoPoint p = vertices[i];
            Radians dLon = p.lon - centroid.lon;

            if(dLon > Pi) dLon -= 2* Pi;
            if(dLon < -Pi) dLon += 2* Pi;

            if(p.lat > value.ur.lat) value.ur.lat = p.lat;
            if(p.lat < value.ll.lat) value.ll.lat = p.lat;

            if(dLon > maxDLon)
               maxDLon = dLon, value.ur.lon = p.lon;
            if(dLon < minDLon)
               minDLon = dLon, value.ll.lon = p.lon;
         }
         if((Radians)value.ll.lon < -Pi)
            value.ll.lon += 2*Pi;
         if((Radians)value.ur.lon > Pi)
            value.ur.lon -= 2*Pi;

         delete vertices;
      }
   }

   property CRSExtent iseaExtent
   {
      get
      {
         int i;
         Array<Pointd> vertices = getRefinedVertices({ ogc, 1534 }, 0, false);
         int nVertices = vertices ? vertices.count : 0;

         value.tl.x = MAXDOUBLE, value.tl.y = -MAXDOUBLE;
         value.br.x = -MAXDOUBLE, value.br.y = MAXDOUBLE;
         for(i = 0; i < nVertices; i++)
         {
            const Pointd * v = &vertices[i];
            double x = v->x, y = v->y;

            if(y < value.br.y) value.br.y = y;
            if(y > value.tl.y) value.tl.y = y;
            if(x > value.br.x) value.br.x = x;
            if(x < value.tl.x) value.tl.x = x;
         }
         delete vertices;
      }
   }

   property CRSExtent isea5x6Extent
   {
      get
      {
         int i;
         Array<Pointd> vertices = getRefinedVertices({ ogc, 153456 }, 0, false);
         int nVertices = vertices ? vertices.count : 0;

         value.tl.x = MAXDOUBLE, value.tl.y = MAXDOUBLE;
         value.br.x = -MAXDOUBLE, value.br.y = -MAXDOUBLE;
         for(i = 0; i < nVertices; i++)
         {
            const Pointd * v = &vertices[i];
            double x = v->x, y = v->y;

            if(y > value.br.y) value.br.y = y;
            if(y < value.tl.y) value.tl.y = y;
            if(x > value.br.x) value.br.x = x;
            if(x < value.tl.x) value.tl.x = x;
         }
         delete vertices;
      }
   }

   property Pointd centroid
   {
      get
      {
         int sh = subHex;
         if(sh == 1 || sh == 6)
            value = { 1, 0 }; // "North" pole (Even level B and Odd level G)
         else if(sh == 2 || sh == 7)
            value = { 4, 6 }; // "South" pole (Even level C and Odd level H)
         else
         {
            uint level = levelISEA9R, root = rootRhombus;
            uint64 rix = rhombusIX;
            uint64 p = (uint64)pow(3, level);
            uint64 rowOP = (root + 1) >> 1, colOP = root >> 1;
            uint64 ixOP = (uint64)(rix / p);
            uint64 row = (uint64)(rowOP * p + ixOP);
            uint64 col = (uint64)((colOP - ixOP) * p + rix); // distributivity on: rix - (ixOP * p) for (rix % p)
            double d =  1.0 / p;
            Pointd tl { col * d, row * d };

            if(sh == 0 || sh == 3)
               value = { tl.x, tl.y }; // Even level A or Odd level D hex
            else if(sh == 4) // Odd level E hex
               value = { tl.x + 2*d/3, tl.y + d/3 };
            else if(sh == 5) // Odd level F hex
               value = { tl.x + d/3, tl.y + 2*d/3 };
         }
      }
   }

   property GeoPoint wgs84Centroid
   {
      get
      {
         isea5x6ToGeo(centroid, value);
      }
   }

   property bool isCentroidChild
   {
      get
      {
         int sh = subHex;
         if(this == nullZone)
            return false;
         else if(sh > 2)  // Odd level
            return sh == 3 || sh == 6 || sh == 7; // D, G and H are centroid children
         else if(sh == 1 || sh == 2) // B and C are centroid children
            return true;
         else
         {
            // Even level -- some A are centroid children
            int level = this.levelISEA9R;
            uint64 rix = this.rhombusIX;
            uint64 p = (uint64)pow(3.0, level);
            uint64 r = rix / p, c = rix % p;
            if(!(r % 3) && !(c % 3))
               return true; // Both row & column multiple of 3 are centroid children
            else if(!((r + c) % 3))
               return true; // (row + column) multiple of 3 are centroid children
         }
         return false;
      }
   }

   int64 getSubZonesCount(int rDepth)
   {
      int64 nHexSubZones = rDepth > 0 ? ((int64)(pow(3, rDepth) + pow(3, (rDepth + 1)/2)) + 1) : 1;
      return (int64)ceil(nHexSubZones * nPoints / 6.0);
   }

   ISEA3HZone getFirstSubZone(int rDepth)
   {
      Pointd firstCentroid;

      getFirstSubZoneCentroid(rDepth, firstCentroid);
      return fromCentroid(level + rDepth, firstCentroid);
   }

   void getFirstSubZoneCentroid(int rDepth, Pointd firstCentroid)
   {
      getISEA3HFirstSubZoneCentroid(this, rDepth, firstCentroid);
   }

   Array<Pointd> getSubZoneCentroids(int rDepth)
   {
      return getISEA3HSubZoneCentroids(this, rDepth);
   }

   private /*static */bool orderISEA3HZones(int zoneLevel, AVLTree<ISEA3HZone> tsZones, Array<ISEA3HZone> zones)
   {
      Array<Pointd> centroids = getSubZoneCentroids(zoneLevel - level);
      if(centroids)
      {
         int nSubZones = centroids.count;
         int i;

         for(i = 0; i < nSubZones; i++)
         {
            ISEA3HZone key = ISEA3HZone::fromCentroid(zoneLevel, centroids[i]);
            if(tsZones.Find(key))
               zones.Add(key);
            else
            {
      #ifdef _DEBUG
               PrintLn("WARNING: mismatched sub-zone while re-ordering");
      #endif
            }
         }
         delete centroids;
         return true;
      }
      else
         return false; // Work around until all sub-zone listing fully handled
   }
}

static void compactISEA3HZones(AVLTree<ISEA3HZone> zones, int level)
{
   AVLTree<ISEA3HZone> output { };
   AVLTree<ISEA3HZone> next { };
   int l;

   for(l = level - 2; l >= 0; l -= 2)
   {
      int i;
      for(z : zones)
      {
         ISEA3HZone zone = z, cgParents[3];
         int nCGParents = zone.getContainingGrandParents(cgParents);
         int p;
         for(p = 0; p < nCGParents; p++)
         {
            ISEA3HZone gParent = cgParents[p];
            if(gParent != nullZone && !next.Find(gParent))
            {
               ISEA3HZone cZone = gParent.centroidChild.centroidChild;
               ISEA3HZone neighbors[6];
               int nNeighbors = cZone.getNeighbors(neighbors, null);
               bool parentAllIn = true;

               for(i = 0; i < nNeighbors; i++)
               {
                  ISEA3HZone nb = neighbors[i];
                  if(nb != nullZone && !zones.Find(nb))
                  {
                     parentAllIn = false;
                     break;
                  }
               }

               if(parentAllIn)
               {
                  // Grandparent vertex children's centroid children are partially within it
                  // and must be present to perform replacement
                  ISEA3HZone children[7];
                  int nChildren = gParent.getChildren(children);

                  for(i = 1; i < nChildren; i++)
                  {
                     ISEA3HZone ch = children[i];
                     if(ch != nullZone)
                     {
                        ISEA3HZone cChild = ch.centroidChild;

                        if(!zones.Find(cChild))
                        {
                           Pointd cv = cChild.centroid;
                           int cl = 2*cChild.levelISEA9R + (cChild.subHex >= 3);
                           ISEA3HZone sub = ISEA3HZone::fromCentroid(cl + 2, cv);
                           if(!output.Find(sub))
                              parentAllIn = false;
                        }
                     }
                  }
                  if(parentAllIn)
                     next.Add(gParent);
               }
            }
         }
      }

      for(z : zones)
      {
         ISEA3HZone zone = z, cgParents[3];
         int nCGParents = zone.getContainingGrandParents(cgParents), i;
         bool allIn = true;

         for(i = 0; i < nCGParents; i++)
         {
            if(!next.Find(cgParents[i]))
            {
               allIn = false;
               break;
            }
         }
         if(!allIn)
            output.Add(zone);
      }

      if(/*0 && */l - 2 >= 0 && next.count)
      {
         // Not done -- next level becomes zones to compact
         zones.copySrc = next;
         next.Free();
      }
      else
      {
         // Done -- next is combined with output into final zones
         zones.copySrc = output;
         for(z : next)
            zones.Add(z);
         //break;
      }
   }

   delete output;
   delete next;

   if(zones.count >= 32 && zones.firstIterator.data.level == 1)
   {
      int nL1 = 0;
      /*
      // REVIEW: Sometimes all level 1 zones are included, but extra sub zones as well
      bool allL1 = true;
      for(z : zones; z.level != 1)
      {
         allL1 = false;
         break;
      }
      */
      for(z : zones)
      {
         ISEA3HZone zone = z;
         int level = zone.level;
         if(level == 1)
            nL1++;
         else if(level > 1)
            break;
      }

      // if(allL1)
      if(nL1 == 32)
      {
         // Simplifying full globe to level 0 zones
         int r;
         zones.Free();
         for(r = 0; r < 10; r++)
            zones.Add({ 0, r, 0, 0 });
         zones.Add({ 0, 0, 0, 1 });
         zones.Add({ 0, 9, 0, 2 });
      }
   }
}

#include <stdio.h>

static Array<DGGRSZone> listISEA3HZones(int zoneLevel, const GeoExtent bbox)
{
   Array<DGGRSZone> zones = null;
   AVLTree<ISEA3HZone> tsZones { };
   ISEA5x6Projection pj = isea5x6PJ;
   int isea9RLevel = zoneLevel / 2;
   uint64 power = (uint64)pow(3, isea9RLevel);
   double z = 1.0 / power;
   int hexSubLevel = zoneLevel & 1;
   Pointd tl, br;
   double x, y;
   bool extentCheck = true;

   if(bbox != null && bbox.OnCompare(wholeWorld))
   {
      // fputs("WARNING: accurate bounding box not yet supported\n", stderr);
      pj.crsExtentFromWGS84(bbox, tl, br);
   }
   else
      extentCheck = false, pj.crsExtentFromWGS84(wholeWorld, tl, br);

   for(y = tl.y; y < br.y + z; y += z)
   {
      int rootY = (int)(y + 1E-11);
      int row = (int)(y / z + 1E-11);
      for(x = tl.x; x < br.x + z; x += z)
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
            GeoExtent e = zone.approxWGS84Extent;
            if(!e.intersects(bbox))
               continue;

            e = zone.wgs84Extent;
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
