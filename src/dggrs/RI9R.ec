// This serves as a basis for aperture 9 rhombic grids
// using different projections based on the Rhombic Icosahedral 5x6 space
public import IMPORT_STATIC "ecrt"
private:

import "dggrs"
import "ri5x6"

#include <stdio.h>

define I9R_MAX_VERTICES = 200; // * 1024;

public class RhombicIcosahedral9R : DGGRS
{
   bool equalArea;
   RI5x6Projection pj;

   uint64 countZones(int level)
   {
      return (uint64)(10 * pow(9, level));
   }

   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   double getZoneArea(I9RZone zoneID)
   {
      double area;
      if(equalArea)
      {
         double zoneCount = 10 * pow(9, zoneID.level);
         static double earthArea = 0;
         if(!earthArea) earthArea = wholeWorld.geodeticArea;

         area = earthArea / zoneCount;
      }
      else
         area = 0;

      return area;
   }

   int getMaxDGGRSZoneLevel() { return 16; }
   int getRefinementRatio() { return 9; }
   int getMaxParents() { return 1; }
   int getMaxNeighbors() { return 4; }
   int getMaxChildren() { return 9; }

   uint64 countSubZones(I9RZone zone, int depth)
   {
      return (uint64)pow(9, depth);
   }

   int getZoneLevel(I9RZone zone)
   {
      return zone.level;
   }

   int countZoneEdges(I9RZone zone) { return 4; }

   int getZoneParents(I9RZone zone, I9RZone * parents)
   {
      parents[0] = nullZone;
      if(zone.level > 0)
         parents[0] = zone.parent;
      return parents[0] != nullZone;
   }

   int getZoneChildren(I9RZone zone, I9RZone * children)
   {
      zone.getChildren(children);
      return 9;
   }

   int getZoneNeighbors(I9RZone zone, I9RZone * neighbors, int * nbType)
   {
      zone.getNeighbors(neighbors);
      if(nbType)
         nbType[0] = 0, nbType[1] = 1, nbType[2] = 2, nbType[3] = 3;
      return 4;
   }

   I9RZone getZoneFromWGS84Centroid(int level, const GeoPoint centroid)
   {
      if(level <= 16)
      {
         Pointd v;
         pj.forward(centroid, v);
         return I9RZone::fromCRSExtent(v, v, level);
      }
      return nullZone;
   }

   void getZoneWGS84Centroid(I9RZone zone, GeoPoint centroid)
   {
      pj.inverse(zone.centroid, centroid, false);
   }

   // Text ZIRS
   void getZoneTextID(I9RZone zone, String zoneID)
   {
      zone.getZoneID(zoneID);
   }

   DGGRSZone getZoneFromTextID(const String zoneID)
   {
      return I9RZone::fromZoneID(zoneID);
   }

   // Sub-zone Order
   I9RZone getFirstSubZone(I9RZone parent, int depth)
   {
      CRSExtent e = parent.ri5x6Extent;
      double dx, dy;
      double d = 2 * pow(3, depth);

      dx = (e.br.x - e.tl.x) / d, dy = (e.br.y - e.tl.y) / d;
      return I9RZone::fromCRSExtent(e.tl, { e.tl.x + dx, e.tl.y + dy }, parent.level + depth );
   }

   void compactZones(Array<DGGRSZone> zones)
   {
      int maxLevel = 0, i, count = zones.count;
      AVLTree<I9RZone> zonesTree { };

      for(i = 0; i < count; i++)
      {
         I9RZone zone = (I9RZone)zones[i];
         if(zone != nullZone)
         {
            int level = zone.level;
            if(level > maxLevel)
               maxLevel = level;
            zonesTree.Add(zone);
         }
      }

      compactI9RZones(zonesTree, maxLevel);
      zones.Free();

      count = zonesTree.count;
      zones.size = count;
      i = 0;
      for(z : zonesTree)
         zones[i++] = z;
      delete zonesTree;
   }

   Array<DGGRSZone> listZones(int level, const GeoExtent bbox)
   {
      uint64 p = (uint64)pow(3, level);
      uint64 numCols = 5*p, numRows = 6*p;
      AVLTree<I9RZone> zonesTree { };
      Array<I9RZone> zones { };
      Pointd tl, br;
      int row, col, y1, y2, x1, x2;
      bool extentCheck = true;

      if(bbox != null && bbox.OnCompare(wholeWorld))
      {
         // Avoid the possibility of including extra zones for single point boxes
         if(fabs((Radians)bbox.ur.lat - (Radians)bbox.ll.lat) < 1E-11 &&
            fabs((Radians)bbox.ur.lon - (Radians)bbox.ll.lon) < 1E-11)
         {
            DGGRSZone zone = getZoneFromWGS84Centroid(level, bbox.ll);
            if(zone != nullZone)
               zones = { [ (I9RZone)zone ] };
            return (Array<DGGRSZone>)zones;
         }

         pj.extent5x6FromWGS84(bbox, tl, br);
      }
      else
         extentCheck = false, pj.extent5x6FromWGS84(wholeWorld, tl, br);
      x1 = Min(Max(0, (int64)(tl.x * p)), numCols-1);
      y1 = Min(Max(0, (int64)(tl.y * p)), numRows-1);
      x2 = Min(Max(0, (int64)(br.x * p)), numCols-1);
      y2 = Min(Max(0, (int64)(br.y * p)), numRows-1);

      if(!p)
         y2 = y1-1; // Avoid divisions by 0, returning null for negative levels

      for(row = y1; row <= y2; row++)
      {
         for(col = x1; col <= x2; col++)
         {
            // Eliminate rows outside the ISEA staircase
            uint rowOP = (uint)(row / p), colOP = (uint)(col / p);
            int root = rowOP + colOP;
            int y = (int)(row - rowOP * p), x = (int)(col - colOP * p);
            uint64 ix = y * p + x;
            int rRow, rCol;
            if(iLRCFromLRtI((char)('A' + level), root, ix, &rRow, &rCol) != -1 && row == rRow && col == rCol)
            {
               I9RZone zone { level, row, col };
               if(extentCheck)
               {
                  GeoExtent e;

                  getZoneWGS84Extent(zone, e);
                  if(!e.intersects(bbox))
                     continue;
               }
               zonesTree.Add(zone);
            }
         }
      }

      zones.minAllocSize = zonesTree.count;
      for(t : zonesTree)
         zones.Add(t);
      zones.minAllocSize = 0;
      if(!zones.count)
         delete zones;

      delete zonesTree;
      return (Array<DGGRSZone>)zones;
   }

   Array<GeoPoint> getSubZoneWGS84Centroids(I9RZone parent, int depth)
   {
      Array<GeoPoint> geo = null;
      Array<Pointd> centroids = parent.getSubZoneCentroids(depth);
      if(centroids)
      {
         uint count = centroids.count;
         int i;

         geo = { size = count };
         for(i = 0; i < count; i++)
            pj.inverse(centroids[i], geo[i], false);
         delete centroids;
      }
      return geo;
   }

   // edge refinement is not supported
   Array<GeoPoint> getZoneRefinedWGS84Vertices(I9RZone zone, int edgeRefinement)
   {
      GeoPoint v[I9R_MAX_VERTICES];
      int count = getI9RRefinedWGS84Vertices(this, zone, v);
      Array<GeoPoint> vertices { size = count };
      memcpy(vertices.array, v, sizeof(GeoPoint) * count);
      return vertices;
   }

   int getZoneWGS84Vertices(I9RZone zone, GeoPoint * vertices)
   {
      CRSExtent extent = zone.ri5x6Extent;
      Pointd v5x6[4] =
      {
         extent.tl,
         { extent.tl.x, extent.br.y },
         extent.br,
         { extent.br.x, extent.tl.y }
      };
      uint count = 4, i;
      for(i = 0; i < count; i++)
         pj.inverse(v5x6[i], vertices[i], false);
      return count;
   }

   void getZoneWGS84Extent(I9RZone zone, GeoExtent value)
   {
      int i;
      GeoPoint centroid;
      Radians minDLon = 99999, maxDLon = -99999;
      GeoPoint vertices[I9R_MAX_VERTICES];
      int nVertices = getI9RRefinedWGS84Vertices(this, zone, vertices);

      getZoneWGS84Centroid(zone, centroid);

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
   }

   I9RZone getZoneFromCRSCentroid(int level, CRS crs, const Pointd centroid)
   {
      if(level <= 16)
      {
         switch(crs)
         {
            case 0: case CRS { ogc, 153456 }: return I9RZone::fromCRSExtent(centroid, centroid, level);
            case CRS { ogc, 1534 }:
            {
               Pointd c5x6;
               RI5x6Projection::fromIcosahedronNet(centroid, c5x6);
               return I9RZone::fromCRSExtent(c5x6, c5x6, level);
            }
            case CRS { epsg, 4326 }:
            case CRS { ogc, 84 }:
               return (I9RZone)getZoneFromWGS84Centroid(level,
                  crs == { ogc, 84 } ?
                     { centroid.y, centroid.x } :
                     { centroid.x, centroid.y });
         }
      }
      return nullZone;
   }

   void getZoneCRSCentroid(I9RZone zone, CRS crs, Pointd centroid)
   {
      switch(crs)
      {
         case CRS { ogc, 1534 }:
         {
            Pointd c5x6 = zone.centroid;
            RI5x6Projection::toIcosahedronNet(c5x6, centroid);
            break;
         }
         case 0: case CRS { ogc, 153456 }: centroid = zone.centroid; break;
         case CRS { epsg, 4326 }:
         case CRS { ogc, 84 }:
         {
            GeoPoint geo;

            getZoneWGS84Centroid(zone, geo);
            centroid = crs == { ogc, 84 } ?
               { geo.lon, geo.lat } :
               { geo.lat, geo.lon };
            break;
         }
      }
   }

   int getZoneCRSVertices(I9RZone zone, CRS crs, Pointd * vertices)
   {
      uint count = 0, i;
      CRSExtent extent = zone.ri5x6Extent;
      Pointd v[4] =
      {
         extent.tl,
         { extent.tl.x, extent.br.y },
         extent.br,
         { extent.br.x, extent.tl.y }
      };

      switch(crs)
      {
         case CRS { ogc, 153456 }: case 0:
            count = 4;
            memcpy(vertices, v, sizeof(Pointd) * 4);
            break;
         case CRS { ogc, 1534 }:
         {
            uint count = 4, i;
            CRSExtent extent = zone.ri5x6Extent;
            Pointd v[4] =
            {
               extent.tl,
               { extent.tl.x, extent.br.y },
               extent.br,
               { extent.br.x, extent.tl.y }
            };

            for(i = 0; i < count; i++)
               RI5x6Projection::toIcosahedronNet(v[i], vertices[i]);
            return count;
         }
         case CRS { ogc, 84 }:
         case CRS { epsg, 4326 }:
            count = 4;
            for(i = 0; i < count; i++)
            {
               GeoPoint geo;
               pj.inverse(v[i], geo, false);
               vertices[i] = crs == { ogc, 84 } ? { geo.lon, geo.lat } : { geo.lat, geo.lon };
            }
            break;
      }
      return count;
   }

   // No refinement needed in ISEA CRSs
   Array<Pointd> getZoneRefinedCRSVertices(I9RZone zone, CRS crs, int edgeRefinement)
   {
      switch(crs)
      {
         case CRS { ogc, 1534 }:
         {
            Array<Pointd> vertices { size = 4 };
            getZoneCRSVertices(zone, crs, vertices.array);
            return vertices;
         }
         case 0: case CRS { ogc, 153456 }:
         {
            Array<Pointd> vertices { size = 4 };
            getZoneCRSVertices(zone, crs, vertices.array);
            return vertices;
         }
         case CRS { ogc, 84 }: case CRS { epsg, 4326 }:
         {
            GeoPoint v[I9R_MAX_VERTICES];
            int count = getI9RRefinedWGS84Vertices(this, zone, v), i;
            Array<Pointd> vertices { size = count };
            for(i = 0; i < count; i++)
               vertices[i] = crs == { ogc, 84 } ? { v[i].lat, v[i].lon } : { v[i].lon, v[i].lat };
            return vertices;
         }
      }
      return null;
   }

   void getZoneCRSExtent(I9RZone zone, CRS crs, CRSExtent extent)
   {
      switch(crs)
      {
         case 0: case CRS { ogc, 153456 }: extent = zone.ri5x6Extent; break;
         case CRS { ogc, 1534 }:
            getIcoNetExtentFromVertices(/*this, */zone, extent);
            break;
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

   Array<Pointd> getSubZoneCRSCentroids(I9RZone parent, CRS crs, int depth)
   {
      Array<Pointd> centroids = parent.getSubZoneCentroids(depth);
      if(centroids)
      {
         uint count = centroids.count, i;
         switch(crs)
         {
            case CRS { ogc, 1534 }:
               for(i = 0; i < count; i++)
                  RI5x6Projection::toIcosahedronNet(centroids[i], centroids[i]);
               break;
            case CRS { ogc, 153456 }: case 0: break;
            case CRS { epsg, 4326 }:
            case CRS { ogc, 84 }:
               for(i = 0; i < count; i++)
               {
                  GeoPoint geo;
                  pj.inverse(centroids[i], geo, false);
                  centroids[i] = crs == { ogc, 84 } ? { geo.lon, geo.lat } : { geo.lat, geo.lon };
               }
               break;
            default: delete centroids;
         }
      }
      return centroids;
   }
}

// public for use in tests
public class I9RZone : DGGRSZone
{
public:
   uint level:5:59, row:29:30, col:30:0;

private:
   property I9RZone parent
   {
      get
      {
         int l = level;
         return (l > 0) ? I9RZone { l - 1, row / 3, col / 3 } : nullZone;
      }
   }

   Array<Pointd> getSubZoneCentroids(int rDepth)
   {
      uint64 s = (int64)pow(3, rDepth), nSubZones = s * s;
      if(nSubZones < 1LL<<31)
      {
         Array<Pointd> centroids { size = (uint)nSubZones };
         int i = 0, y, x;
         CRSExtent e = ri5x6Extent;
         double dx = (e.br.x - e.tl.x) / s, dy = (e.br.y - e.tl.y) / s;

         for(y = 0; y < s; y++)
         {
            double yy = e.tl.y + (y + 0.5) * dy;
            for(x = 0; x < s; x++, i++)
            {
               centroids[i].x = e.tl.x + (x + 0.5) * dx;
               centroids[i].y = yy;
            }
         }
         return centroids;
      }
      return null;
   }

   property CRSExtent ri5x6Extent
   {
      get
      {
         double z = 1.0 / pow(3, level);
         value.tl = { col * z, row * z };
         value.br = { value.tl.x + z, value.tl.y + z };
         value.crs = { ogc, 153456 };
      }
   }

   property Pointd centroid
   {
      get
      {
         double z = 1.0 / pow(3, level);
         value = { (col + 0.5) * z, (row + 0.5) * z };
      }
   }

   // This function generates the proposed ISEA9R DGGS Zone ID string
   // in the form {LevelChar}{RootRhombus}-{HexIndexWithinRootRhombus}
   // from an ISEA9R TileMatrixSet Level, Row, Column
   void getZoneID(String zoneID)
   {
      int level = this.level;
      uint row = this.row, col = this.col;
      uint64 p = (uint64)pow(3, level);
      uint rowOP = (uint)(row / p), colOP = (uint)(col / p);
      int root = rowOP + colOP;
      int y = (int)(row - rowOP * p), x = (int)(col - colOP * p);
      uint64 ix = y * p + x;
      sprintf(zoneID,
         __runtimePlatform == win32 ? "%c%d-%I64X" : "%c%d-%llX",
         'A' + level, root, ix);
   }

   I9RZone ::fromCRSExtent(const Pointd topLeft, const Pointd bottomRight, int level)
   {
      uint64 p = (uint64)pow(3, level);
      int64 numRows = 6 * p, numCols = 5 * p;
      Pointd mid
      {
         (topLeft.x + bottomRight.x) / 2,
         (topLeft.y + bottomRight.y) / 2
      };
      int row = Min(Max(0, (int)(mid.y * p)), numRows-1);
      int col = Min(Max(0, (int)(mid.x * p)), numCols-1);
      // WARNING: Cannot return negative level in DGGRSZone
      return I9RZone { Max(0, level), row, col };
   }

   I9RZone ::fromZoneID(const String zoneID)
   {
      I9RZone result = nullZone;
      char levelChar;
      uint root;
      uint64 ix;
      int row, col, l9r = -1;

      if(sscanf(zoneID, __runtimePlatform == win32 ? "%c%d-%I64X" : "%c%d-%llX", &levelChar, &root, &ix) == 3)
      {
         l9r = iLRCFromLRtI(levelChar, root, ix, &row, &col);
         if(l9r != -1)
         {
            char id[256];
            result = { l9r, row, col };
            result.getZoneID(id);
            // Further validation (ISEA3H zones still validated as ISEA9R)
            if(strcmp(id, zoneID))
               result = nullZone;
         }
      }
      return result;
   }

   void getChildren(I9RZone children[9])
   {
      uint l = level+1;
      uint row1 = row * 3;
      uint col1 = col * 3;
      int x, y;

      for(y = 0; y < 3; y++)
         for(x = 0; x < 3; x++)
         {
            I9RZone key = I9RZone { l, row1 + y, col1 + x};
#if 0
            if(key.lat != row1 + y)
               Print("Ybug");
            if(key.lon != col1 + x)
               Print("Xbug");
#endif
            children[y * 3 + x] = key;
         }
   }

   void getNeighbors(I9RZone neighbors[4])
   {
      uint l = level;
      int row = this.row, col = this.col;
      int p = (int)pow(3, l);
      uint numRows = 6 * p, numCols = 5 * p;
      int colOP = col / p, rowOP = row / p;
      int topDelta = (row - 1) / p - colOP;
      int leftDelta = rowOP - (col - 1) / p;
      int bottomDelta = (row + 1) / p - colOP;
      int rightDelta = rowOP - (col + 1) / p;

      // Top
      if(row == 0 || (topDelta && topDelta != 1))
      {
         // Crossing over top interruption to the left
         int r = (rowOP ? rowOP - 1 : 4) * p + p - 1 - (col - colOP * p);
         int c = (rowOP ? colOP - 1 : 4) * p + p - 1;
         neighbors[0] = I9RZone { l, r, c };
      }
      else
         neighbors[0] = I9RZone { l, row - 1, col };

      // Left
      if(col == 0 || (leftDelta && leftDelta != 1))
      {
         int r, c;
         if((leftDelta && leftDelta != 1) || (col == 0 && rowOP > colOP))
         {
            // Crossing over bottom interruption to the left
            r = (colOP ? rowOP - 1 : 5) * p + p - 1;
            c = (colOP ? colOP - 1 : 4) * p + p - 1 - (row - rowOP * p);
         }
         else
         {
            // Wrapping to the left
            r = row + 5 * p;
            c = col - 1 + 5 * p;
         }
         neighbors[1] = I9RZone { l, r, c };
      }
      else
         neighbors[1] = I9RZone { l, row, col - 1 };

      // Right
      if(col == numCols-1 || (rightDelta && rightDelta != 1))
      {
         int r, c;
         if((rightDelta && rightDelta != 1) || (col == numCols-1 && rowOP == colOP))
         {
            // Crossing over top interruption to the right
            r = (colOP < 4 ? rowOP + 1 : 0) * p + 0;
            c = (colOP < 4 ? colOP + 1 : 0) * p + p - 1 - (row - rowOP * p);
         }
         else
         {
            // Wrapping to the right
            r = row - 5 * p;
            c = col + 1 - 5 * p;
         }
         neighbors[2] = I9RZone { l, r, c };
      }
      else
         neighbors[2] = I9RZone { l, row, col + 1 };

      // Bottom
      if(row == numRows-1 || (bottomDelta && bottomDelta != 1))
      {
         // Crossing over bottom interruption to the right
         int r = (rowOP < 5 ? rowOP + 1 : 1) * p + p - 1 - (col - colOP * p);
         int c = (rowOP < 5 ? colOP + 1 : 0) * p + 0;
         neighbors[3] = I9RZone { l, r, c };
      }
      else
         neighbors[3] = I9RZone { l, row + 1, col };
   }
}


// This function returns an I9R TileMatrixSet Level, Row, Column
// from the LevelChar, RootDiamond and IndexWithinRootDiamond components
// of the proposed I9R DGGRS Zone ID string
int iLRCFromLRtI(char levelChar, int root, uint64 ix, int * row, int * col)
{
   int level = levelChar - 'A';
   if(level >= 0 && level <= 16 && root >= 0 && root <= 9)
   {
      uint64 p = (uint64)pow(3, level);
      if(ix >= 0 && ix < p * p)
      {
         int rowOP = (root + 1) >> 1, colOP = root >> 1;
         int ixOP = (int)(ix / p);
         *row = (int)(rowOP * p + ixOP);
         *col = (int)((colOP - ixOP) * p + ix); // distributivity on: ix - (ixOP * p) for (ix % p)
         return level;
      }
   }
   *row = -1, *col = -1;
   return -1;
}

static void compactI9RZones(AVLTree<I9RZone> zones, int level)
{
   AVLTree<I9RZone> output { };
   AVLTree<I9RZone> next { };
   int l;

   for(l = level - 1; l >= 0; l--)
   {
      int i;
      for(z : zones)
      {
         I9RZone zone = z, parent = zone.parent;
         if(!next.Find(parent))
         {
            bool parentAllIn = true;
            I9RZone children[9];

            parent.getChildren(children);

            for(i = 0; i < 9; i++)
            {
               I9RZone ch = children[i];
               if(ch != nullZone && !zones.Find(ch))
               {
                  parentAllIn = false;
                  break;
               }
            }

            if(parentAllIn)
               next.Add(parent);
            else
               output.Add(zone);
         }
      }

      if(l - 1 >= 0 && next.count)
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
}

   // NOTE: custom edgeRefinement not currently supported
static uint getI9RRefinedWGS84Vertices(RhombicIcosahedral9R dggrs, I9RZone zone, GeoPoint * outVertices)
{
   #define NUM_ISEA9R_ANCHORS 30
   uint count = 0;
   CRSExtent e = zone.ri5x6Extent;
   Pointd dp[4] = { {e.tl.x, e.tl.y}, {e.tl.x, e.br.y}, {e.br.x, e.br.y}, {e.br.x, e.tl.y} };
   Radians maxDLon = -99999, urLon = -MAXDOUBLE;
   Radians minDLon =  99999, llLon =  MAXDOUBLE;
   GeoPoint centroid;
   int i;
   RI5x6Projection pj = dggrs.pj;

   dggrs.getZoneWGS84Centroid(zone, centroid);

   for(i = 0; i < 4; i++)
   {
      const Pointd * p = &dp[i], * np = &dp[i == 3 ? 0 : i+1];
      bool northPole = (fabs(p->y - 0) < 1E-11 && fabs(np->y - 0) < 1E-11) ||
         (fabs(p->x - 5) < 1E-11 && fabs(np->x - 5) < 1E-11 && (p->y + 1E-11 < 5 || np->y + 1E-11 < 5));
      bool southPole = !northPole &&
                        ((fabs(p->y - 3) < 1E-11 && fabs(np->y - 3) < 1E-11 && (p->x + 1E-11 < 2 || np->x + 1E-11 < 2)) ||
                         (fabs(p->x - 2) < 1E-11 && fabs(np->x - 2) < 1E-11 && (p->y - 1E-11 > 3 || np->y - 1E-11 > 3)));
      int numAnchors = NUM_ISEA9R_ANCHORS;
      int j;
      double dx = np->x - p->x, dy = np->y - p->y;

      for(j = 0; j < numAnchors; j++)
      {
         Pointd in { p->x + dx * j / numAnchors, p->y + dy * j / numAnchors };
         GeoPoint out;
         bool crossingPole = (northPole || southPole) && j == numAnchors / 2;
         #define CROSSING_POLE_DELTA   0.001
         if(crossingPole)
         {
            // Extra point for crossing the pole
            Pointd in1 { in.x - dx * CROSSING_POLE_DELTA, in.y - dy * CROSSING_POLE_DELTA };
            if(pj.inverse(in1, out, false))
            {
               Radians dLon = out.lon - centroid.lon;
               if(dLon > Pi) dLon -= 2*Pi, out.lon -= 2*Pi;
               if(dLon <-Pi) dLon += 2*Pi, out.lon += 2*Pi;
               if(dLon > maxDLon)
                  maxDLon = dLon, urLon = out.lon;
               if(dLon < minDLon)
                  minDLon = dLon, llLon = out.lon;

               outVertices[count++] = out;
            }
         }
         // Don't include point on pole itself
         if(!crossingPole && pj.inverse(in, out, false))
         {
            Radians dLon = out.lon - centroid.lon;
            if(dLon > Pi) dLon -= 2*Pi, out.lon -= 2*Pi;
            if(dLon <-Pi) dLon += 2*Pi, out.lon += 2*Pi;
            if(dLon > maxDLon)
               maxDLon = dLon, urLon = out.lon;
            if(dLon < minDLon)
               minDLon = dLon, llLon = out.lon;

            outVertices[count++] = out;
         }

         if(crossingPole)
         {
            // Extra point for crossing the pole
            Pointd in2 { in.x + dx * CROSSING_POLE_DELTA, in.y + dy * CROSSING_POLE_DELTA };
            if(pj.inverse(in2, out, false))
            {
               Radians dLon = out.lon - centroid.lon;
               if(dLon > Pi) dLon -= 2*Pi, out.lon -= 2*Pi;
               if(dLon <-Pi) dLon += 2*Pi, out.lon += 2*Pi;
               if(dLon > maxDLon)
                  maxDLon = dLon, urLon = out.lon;
               if(dLon < minDLon)
                  minDLon = dLon, llLon = out.lon;

               outVertices[count++] = out;
            }
         }
      }
   }

   for(i = 0; i < count; i++)
      if((Radians)outVertices[i].lon > (Radians)urLon + 1E-11)
         outVertices[i].lon -= 2*Pi;
      else if(outVertices[i].lon < (Radians)llLon - 1E-11)
         outVertices[i].lon += 2*Pi;

   return count;
}

static void getIcoNetExtentFromVertices(I9RZone zone, CRSExtent value)
{
   CRSExtent k = zone.ri5x6Extent;
   Pointd p[4];

   RI5x6Projection::toIcosahedronNet({k.tl.x, k.tl.y }, p[0]);
   RI5x6Projection::toIcosahedronNet({k.tl.x, k.br.y }, p[1]);
   RI5x6Projection::toIcosahedronNet({k.br.x, k.br.y }, p[2]);
   RI5x6Projection::toIcosahedronNet({k.br.x, k.tl.y }, p[3]);
   value.crs = { ogc, 1534 };
   value.tl.x = Min(Min(p[0].x, p[1].x), Min(p[2].x, p[3].x));
   value.tl.y = Max(Max(p[0].y, p[1].y), Max(p[2].y, p[3].y));
   value.br.x = Max(Max(p[0].x, p[1].x), Max(p[2].x, p[3].x));
   value.br.y = Min(Min(p[0].y, p[1].y), Min(p[2].y, p[3].y));
}

/*
static void getIcoNetExtentFromVertices(I9RZone zone, CRS crs, CRSExtent value)
{
   int i;
   Array<Pointd> vertices = dggrs.getZoneRefinedCRSVertices(zone, crs, 0); //, false);
   int nVertices = vertices ? vertices.count : 0;

   value.crs = crs;
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
*/