public import IMPORT_STATIC "ecrt"
private:

import "dggrs"
import "WebMercator"

import "Vector3D"

#include <stdio.h>

static define POW_EPSILON = 0.1;

define SID_MAX_VERTICES = 200; // * 1024;

public class SIDZone : private DGGRSZone
{
public:
   bool polar:1:63;
   uint level:5:58, row:29:29, col:29:0;

private:

   property SIDZone parent
   {
      get
      {
         return nullZone;
      }
   }

   property Pointd centroid
   {
      get
      {

         value =
         {
            x = 0,
            y = 0
         };
      }
   }

   property CRSExtent sidExtent
   {
      get
      {
         uint row = this.row, col = this.col;
         double z = (1 << level) / (2*Pi);

         value.tl = {
            col / z - Pi,
            Pi - row / z
         };
         value.br = {
            (col+1) / z - Pi,
            Pi - (row+1) / z
         };
      }
   }

   SIDZone ::fromPoint(const Pointd v, int level)
   {
      SIDZone zone = nullZone;
      /* TODO:
      int64 p = 1LL << level;
      // Conversion from SpatialID to 5x5 space:
      double x = (v.y + v.x + 5 * Pi / 4) * 2/Pi;
      double y = v.x * 4 / Pi + 5 - x;
      int cx = (int)x, cy = (int)y;
      double sx = x - cx, sy = y - cy;
      bool addX = cx > cy, addY = cy > cx;
      int rCol = cx - addX; // or cy - addY
      int rRow = addX ? 0 : addY ? 2 : 1;
      int64 col = Min(Max(0, (int64)(sx * p)), p - 1);
      int64 row = Min(Max(0, (int64)(sy * p)), p - 1);
      int root = (rRow << 2) | rCol;

      if(rCol >= 0 && rCol <= 4 && rRow >= 0 && rRow <= 2 &&
         sx >= -1E-12 && sx < 1 + 1E-12 && sy >= -1E-12 && sy < 1 + 1E-12)
         zone = { level, root, ((int64)row << level) | col };
      */
      return zone;
   }

   Array<Pointd> getSubZoneCentroids(int depth)
   {
      uint dm = 1 << depth;
      Array<Pointd> centroids { size = dm * dm };
      /* TODO:
      int r, c, i = 0;
      int root = rootRhombus, rCol = root & 3, rRow = (root >> 2);
      int64 p = 1LL << level;
      double oop = 1.0 / p;
      int row = (int)(subIndex >> level);
      int col = (int)(subIndex - ((int64)row << level));
      double w = oop, h = oop;
      Pointd tl { rCol + (int)(rRow == 0) + col * oop, rCol + (int)(rRow == 2) + row * oop };

      for(r = 0; r < dm; r++)
      {
         for(c = 0; c < dm; c++, i++)
         {
            double x = tl.x + c * w / dm;
            double y = tl.y + r * h / dm;
            centroids[i] = {
               (x + y) * Pi / 4 - 5 * Pi / 4,
               -(y - x) * Pi / 4
            };
         }
      }*/
      return centroids;
   }

   int getChildren(SIDZone * children)
   {
      int level = this.level;
      uint row = this.row, col = this.col;
      bool polar = this.polar;

      if(level < 25)
      {
         int nLevel = level + 1;

         children[0] = SIDZone { polar, nLevel, 2*row, 2*col };
         children[1] = SIDZone { polar, nLevel, 2*row, 2*col+1 };
         children[2] = SIDZone { polar, nLevel, 2*row+1, 2*col };
         children[3] = SIDZone { polar, nLevel, 2*row+1, 2*col+1 };
         return 4;
      }
      return 0;
   }
}

public class SpatialID : DGGRS
{
   TransverseMercatorProjection tmPJ { };
   WebMercatorProjection wmPJ { };

   void ::cartesianToGeo(const Vector3D c, GeoPoint out)
   {
      double p = sqrt(c.x*c.x + c.z*c.z);

      out = { (Radians)atan2(-c.y, p), (Radians)atan2(c.x, -c.z) };
   }

   uint64 countZones(int level)
   {
      return (uint64)(12 * (pow(4, level)) + POW_EPSILON);
   }

   double getZoneArea(SIDZone zoneID)
   {
      double area;
      double zoneCount = 12 * pow(4, zoneID.level);
      static double earthArea = 0;
      if(!earthArea) earthArea = wholeWorld.geodeticArea;

      area = earthArea / zoneCount;
      return area;
   }

   int getMaxDGGRSZoneLevel() { return 26; }
   int getRefinementRatio() { return 4; }
   int getMaxParents() { return 1; }
   int getMaxNeighbors() { return 4; }
   int getMaxChildren() { return 4; }

   uint64 countSubZones(SIDZone zone, int depth)
   {
      return 1LL << (2 * depth);
   }

   int getZoneLevel(SIDZone zone)
   {
      return zone.level;
   }

   int countZoneEdges(SIDZone zone) { return 4; }

   int getZoneParents(SIDZone zone, SIDZone * parents)
   {
      parents[0] = nullZone;
      if(zone.level > 0)
         parents[0] = zone.parent;
      return parents[0] != nullZone;
   }

   int getZoneChildren(SIDZone zone, SIDZone * children)
   {
      return zone.getChildren(children);
   }

   int getZoneNeighbors(SIDZone zone, SIDZone * neighbors, int * nbType)
   {
      /*TODO:

      int level = zone.level, root = zone.rootRhombus;
      uint64 subIndex = zone.subIndex;
      int row = (int)(subIndex >> level);
      int col = (int)(subIndex - ((int64)row << level));
      int64 p = 1LL << level;

      // Left
      if(col > 0)
         neighbors[0] = { level, root, ((int64)row << level) | (col - 1) };
      else if(root >= 8)
      {
         // Crossing interruption to the left
         int lRoot = root == 8 ? 0xB : root - 1;
         neighbors[0] = { level, lRoot, ((int64)(p-1) << level) | (p-1-row) };
      }
      else
      {
         int lRoot =
            root <= 3 ? root + 4 :
            root >= 5 && root <= 7 ? root + 3 :
            0xB;
         neighbors[0] = { level, lRoot, ((int64)row << level) | (p - 1) };
      }

      // Right
      if(col < p-1)
         neighbors[1] = { level, root, ((int64)row << level) | (col + 1) };
      else if(root <= 3)
      {
         // Crossing interruption to the right
         int rRoot = root == 3 ? 0 : root + 1;
         neighbors[1] = { level, rRoot, ((int64)0 << level) | (p-1-row) };
      }
      else
      {
         int rRoot =
            root >= 4 && root <= 7 ? root - 4 :
            root >= 8 && root <= 0xA ? root - 3 :
            4;
         neighbors[1] = { level, rRoot, ((int64)row << level) | 0 };
      }

      // Top
      if(row > 0)
         neighbors[2] = { level, root, ((int64)(row - 1) << level) | col };
      else if(root <= 3)
      {
         // Crossing interruption to the left
         int tRoot = root == 0 ? 3 : root - 1;
         neighbors[2] = { level, tRoot, ((int64)(p-1-col) << level) | (p - 1) };
      }
      else
      {
         int tRoot =
            root >= 8 && root <= 0xB ? root - 4 :
            root >= 5 && root <= 7 ? root - 5 :
            3;
         neighbors[2] = { level, tRoot, ((int64)(p - 1) << level) | col };
      }

      // Bottom
      if(row < p-1)
         neighbors[3] = { level, root, ((int64)(row + 1) << level) | col };
      else if(root >= 8)
      {
         // Crossing interruption to the right
         int bRoot = root == 0xB ? 8 : root + 1;
         neighbors[3] = { level, bRoot, ((int64)(p-1-col) << level) | 0 };
      }
      else
      {
         int bRoot =
            root >= 4 && root <= 7 ? root + 4 :
            root >= 0 && root <= 2 ? root + 5 :
            4;
         neighbors[3] = { level, bRoot, ((int64)0 << level) | col };
      }

      if(nbType)
         nbType[0] = 0, nbType[1] = 1, nbType[2] = 2, nbType[3] = 3;
      */
      return 0; //4;
   }

   SIDZone getZoneFromWGS84Centroid(int level, const GeoPoint centroid)
   {
      SIDZone zone = nullZone;
      if(level <= 26)
      {
         Pointd v;

         wmPJ.forward(centroid, v);
         zone = SIDZone::fromPoint(v, level);

         if(zone == nullZone)
         {
            tmPJ.forward(centroid, v);
            zone = SIDZone::fromPoint(v, level);
         }
      }
      return zone;
   }

   void getZoneWGS84Centroid(SIDZone zone, GeoPoint centroid)
   {
      if(zone.polar)
         tmPJ.inverse(zone.centroid, centroid, false);
      else
         wmPJ.inverse(zone.centroid, centroid, false);
   }

   // Text ZIRS
   void getZoneTextID(SIDZone zone, String zoneID)
   {
      #define USE_SLASHES false // true
      sprintf(zoneID, USE_SLASHES ? "%d:%d:%d" : "%d/%d/%d", zone.level, zone.col, zone.row);
   }

   DGGRSZone getZoneFromTextID(const String zoneID)
   {
      SIDZone result = nullZone;
      /* TODO:
      char levelChar;
      uint root;
      uint64 ix;

      if(sscanf(zoneID, __runtimePlatform == win32 ? "%c%X-%I64X" : "%c%X-%llX", &levelChar, &root, &ix) == 3 &&
         levelChar >= 'A' && levelChar <= 'Z' && root <= 0xB)
      {
         int level = levelChar - 'A';
         if(ix < (1LL << (level<<1)))
            result = { level, root, ix };
      }
      */
      return result;
   }

   // Sub-zone Order
   SIDZone getFirstSubZone(SIDZone parent, int depth)
   {
      int pLevel = parent.level, level = pLevel + depth;
      if(level <= 26)
      {
         /* TODO:
         uint root = parent.rootRhombus;
         uint64 pSubIndex = parent.subIndex;
         uint dm = 1 << depth;
         int pRow = (int)(pSubIndex >> pLevel);
         int pCol = (int)(pSubIndex - ((int64)pRow << pLevel));
         return SIDZone { level, root, (((uint64)pRow * dm) << level) | (pCol * dm) };
         */
      }
      return nullZone;
   }

   Array<DGGRSZone> getSubZones(SIDZone parent, int relativeDepth)
   {
      int pLevel = parent.level, level = pLevel + relativeDepth;

      if(level <= 26 && relativeDepth <= 15)
      {
         /* TODO:
         uint root = parent.rootRhombus;
         uint64 pSubIndex = parent.subIndex;
         uint dm = 1 << relativeDepth;
         int pRow = (int)(pSubIndex >> pLevel);
         int pCol = (int)(pSubIndex - ((int64)pRow << pLevel));
         Array<DGGRSZone> subZones { size = dm * dm };
         int r, c, i = 0;

         for(r = 0; r < dm; r++)
            for(c = 0; c < dm; c++, i++)
            {

               subZones[i] = SIDZone { level, root, (((uint64)pRow * dm + r) << level) | (pCol * dm + c) };
            }
         return subZones;
         */
      }
      return null;
   }

   Array<Pointd> getSubZoneCRSCentroids(SIDZone parent, CRS crs, int depth)
   {
      Array<Pointd> centroids = parent.getSubZoneCentroids(depth);
      if(centroids)
      {
         uint count = centroids.count, i;
         switch(crs)
         {
            case 0: case CRS { ogc, 99999 }: break;
            case CRS { epsg, 4326 }:
            case CRS { ogc, 84 }:
               for(i = 0; i < count; i++)
               {
                  GeoPoint geo;

                  // TODO:
               #ifdef POLAR_MODE
                  TransverseMercatorProjection pj = tmPJ;
               #else
                  WebMercatorProjection pj = wmPJ;
               #endif

                  pj.inverse(centroids[i], geo, false);
                  centroids[i] = crs == { ogc, 84 } ? { geo.lon, geo.lat } : { geo.lat, geo.lon };
               }
               break;
            default: delete centroids;
         }
      }
      return centroids;
   }

   Array<GeoPoint> getSubZoneWGS84Centroids(SIDZone parent, int depth)
   {
      Array<GeoPoint> geo = null;
      Array<Pointd> centroids = parent.getSubZoneCentroids(depth);
      if(centroids)
      {
         uint count = centroids.count;
         int i;

         // TODO:
      #ifdef POLAR_MODE
         TransverseMercatorProjection pj = tmPJ;
      #else
         WebMercatorProjection pj = wmPJ;
      #endif

         geo = { size = count };
         for(i = 0; i < count; i++)
            pj.inverse(centroids[i], geo[i], false);
         delete centroids;
      }
      return geo;
   }

   void compactZones(Array<DGGRSZone> zones)
   {
      int maxLevel = 0, i, count = zones.count;
      AVLTree<SIDZone> zonesTree { };

      for(i = 0; i < count; i++)
      {
         SIDZone zone = (SIDZone)zones[i];
         if(zone != nullZone)
         {
            int level = zone.level;
            if(level > maxLevel)
               maxLevel = level;
            zonesTree.Add(zone);
         }
      }

      compactSIDZones(zonesTree, maxLevel);
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
      AVLTree<SIDZone> zonesTree { };
      Array<SIDZone> zones { };
      int l;

      zonesTree.Add({ 0, 0, 0 });

      if(level == 0 && bbox != null)
      {
         AVLTree<SIDZone> tmp { };

         for(z : zonesTree)
         {
            SIDZone zone = (SIDZone)z;
            GeoExtent e;
            getZoneWGS84Extent(zone, e);

            if(e.intersects(bbox))
               tmp.Add(zone);
         }
         delete zonesTree;
         zonesTree = tmp;
      }

      for(l = 1; l <= level; l++)
      {
         AVLTree<SIDZone> tmp { };

         for(z : zonesTree)
         {
            SIDZone zz = z;
            SIDZone children[4];
            int i;
            int n = zz.getChildren(children);

            for(i = 0; i < n; i++)
            {
               SIDZone c = children[i];
               if(bbox != null)
               {
                  GeoExtent e;
                  if(!tmp.Find(c))
                  {
                     getZoneWGS84Extent(c, e);
                     if(!e.intersects(bbox))
                        continue;
                  }
                  else
                     continue;
               }
               tmp.Add(children[i]);
            }
         }
         delete zonesTree;
         zonesTree = tmp;
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

   // edge refinement is not supported
   Array<GeoPoint> getZoneRefinedWGS84Vertices(SIDZone zone, int edgeRefinement)
   {
      GeoPoint v[SID_MAX_VERTICES];
      int count = getSIDRefinedWGS84Vertices(this, zone, v);
      Array<GeoPoint> vertices { size = count };
      memcpy(vertices.array, v, sizeof(GeoPoint) * count);
      return vertices;
   }

   int getZoneWGS84Vertices(SIDZone zone, GeoPoint * vertices)
   {
      uint count = 0;
      /*
      Pointd v[4];
      int level = zone.level;
      int root = zone.rootRhombus, rCol = root & 3, rRow = (root >> 2);
      uint64 subIndex = zone.subIndex;
      int64 p = 1LL << level;
      double oop = 1.0 / p;
      int row = (int)(subIndex >> level);
      int col = (int)(subIndex - ((int64)row << level));
      double x = rCol + (int)(rRow == 0) + col * oop;
      double y = rCol + (int)(rRow == 2) + row * oop;
      uint count = 4, i;

      v[0].x = (x + y) * Pi/4 - 5*Pi/4;
      v[0].y = -(y - x) * Pi/4;

      v[1].x = (x + y + oop) * Pi/4 - 5*Pi/4;
      v[1].y = -(y + oop - x) * Pi/4;

      v[2].x = (x + y + 2*oop) * Pi/4 - 5*Pi/4;
      v[2].y = -(y - x) * Pi/4;

      v[3].x = (x + oop + y) * Pi/4 - 5*Pi/4;
      v[3].y = -(y - x - oop) * Pi/4;

      for(i = 0; i < count; i++)
         pj.inverse(v[i], vertices[i], false);
      */
      return count;
   }

   void getZoneWGS84Extent(SIDZone zone, GeoExtent value)
   {
      CRSExtent e = zone.sidExtent;
      GeoPoint v[4];
      int i;

      double x, y;
      double dx = e.br.x - e.tl.x;
      double dy = e.tl.y - e.br.y;

      // TODO:
   #ifdef POLAR_MODE
      TransverseMercatorProjection pj = tmPJ;
   #else
      WebMercatorProjection pj = wmPJ;
   #endif

      for(y = e.br.y; y <= e.tl.y; y += dy / 40)
      {
         for(x = e.tl.x; x <= e.br.x; x += dx / 40)
         {
            GeoPoint vv;
            pj.inverse({ x, y }, vv, false);

            if(vv.lat < value.ll.lat) value.ll.lat = vv.lat;
            if(vv.lat > value.ur.lat) value.ur.lat = vv.lat;

            if(fabs(fabs((Radians)vv.lat) - Pi/2) > 1E-11)
            {
               if(vv.lon < value.ll.lon) value.ll.lon = vv.lon;
               if(vv.lon > value.ur.lon) value.ur.lon = vv.lon;
            }
         }
      }


      pj.inverse(e.tl, v[0], false);
      pj.inverse({ e.tl.x, e.br.y }, v[1], false);
      pj.inverse(e.br, v[2], false);
      pj.inverse({ e.br.x, e.tl.y }, v[3], false);

      value.clear();
      for(i = 0; i < 4; i++)
      {
         if(v[i].lat < value.ll.lat) value.ll.lat = v[i].lat;
         if(v[i].lat > value.ur.lat) value.ur.lat = v[i].lat;

         if(fabs(fabs((Radians)v[i].lat) - Pi/2) > 1E-11)
         {
            if(v[i].lon < value.ll.lon) value.ll.lon = v[i].lon;
            if(v[i].lon > value.ur.lon) value.ur.lon = v[i].lon;
         }
      }
      /*if(value.ur.lon - value.ll.lon > Pi)
      {
         value.ll.lon = Pi;
         value.ur.lon = -Pi;
         for(i = 0; i < 4; i++)
         {
            if(v[i].lon > 0 && v[i].lon < value.ll.lon) value.ll.lon = v[i].lon;
            if(v[i].lon < 0 && v[i].lon > value.ur.lon) value.ur.lon = v[i].lon;
         }
      }*/

      if(value.ll.lon < -180)
         value.ll.lon += 360;
      if(value.ur.lon < -180)
         value.ur.lon += 360;
   }

   SIDZone getZoneFromCRSCentroid(int level, CRS crs, const Pointd centroid)
   {
      if(level <= 26)
      {
         switch(crs)
         {
            case 0: case CRS { ogc, 99999 }: return SIDZone::fromPoint(centroid, level);
            case CRS { epsg, 4326 }:
            case CRS { ogc, 84 }:
               return (SIDZone)getZoneFromWGS84Centroid(level,
                  crs == { ogc, 84 } ?
                     { centroid.y, centroid.x } :
                     { centroid.x, centroid.y });
         }
      }
      return nullZone;
   }

   void getZoneCRSCentroid(SIDZone zone, CRS crs, Pointd centroid)
   {
      switch(crs)
      {
         case 0: case CRS { ogc, 99999 }: centroid = zone.centroid; break;
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

   int getZoneCRSVertices(SIDZone zone, CRS crs, Pointd * vertices)
   {
      uint count = 0;
      CRSExtent extent = zone.sidExtent;
      uint i;
      Pointd v[4] =
      {
         extent.tl,
         { extent.tl.x, extent.br.y },
         extent.br,
         { extent.br.x, extent.tl.y }
      };

      switch(crs)
      {
         case 0: case CRS { ogc, 99999 }:
            count = 4;
            memcpy(vertices, v, sizeof(Pointd) * 4);
            break;
         case CRS { ogc, 84 }:
         case CRS { epsg, 4326 }:
         {
            // TODO:
         #ifdef POLAR_MODE
            TransverseMercatorProjection pj = tmPJ;
         #else
            WebMercatorProjection pj = wmPJ;
         #endif

            count = 4;

            for(i = 0; i < count; i++)
            {
               GeoPoint geo;
               pj.inverse(v[i], geo, false);
               vertices[i] = crs == { ogc, 84 } ? { geo.lon, geo.lat } : { geo.lat, geo.lon };
            }
            break;
         }
      }
      return count;
   }

   Array<Pointd> getZoneRefinedCRSVertices(SIDZone zone, CRS crs, int edgeRefinement)
   {
      crs = { ogc, 99999 };
      switch(crs)
      {
         case 0: case CRS { ogc, 99999 }:
         {
            Array<Pointd> vertices { size = 4 };
            getZoneCRSVertices(zone, crs, vertices.array);
            return vertices;
         }
         case CRS { ogc, 84 }: case CRS { epsg, 4326 }:
         {
            GeoPoint v[SID_MAX_VERTICES];
            int count = getSIDRefinedWGS84Vertices(this, zone, v), i;
            Array<Pointd> vertices { size = count };
            for(i = 0; i < count; i++)
               vertices[i] = crs == { ogc, 84 } ? { v[i].lat, v[i].lon } : { v[i].lon, v[i].lat };
            return vertices;
         }
      }
      return null;
   }

   void getZoneCRSExtent(SIDZone zone, CRS crs, CRSExtent extent)
   {
      switch(crs)
      {
         case 0: case CRS { ogc, 99999 }: extent = zone.sidExtent; break;
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
}

static void compactSIDZones(AVLTree<SIDZone> zones, int level)
{
   AVLTree<SIDZone> output { };
   AVLTree<SIDZone> next { };
   int l;

   for(l = level - 1; l >= 0; l--)
   {
      int i;
      for(z : zones)
      {
         SIDZone zone = z, parent = zone.parent;
         if(!next.Find(parent))
         {
            bool parentAllIn = true;
            SIDZone children[4];
            int n = parent.getChildren(children);

            for(i = 0; i < n; i++)
            {
               SIDZone ch = children[i];
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

static uint getSIDRefinedWGS84Vertices(SpatialID dggrs, SIDZone zone, GeoPoint * outVertices)
{
   #define NUM_SID_ANCHORS 30
   uint count = 0;
   Pointd dp[4];
   Radians maxDLon = -99999, urLon = -MAXDOUBLE;
   Radians minDLon =  99999, llLon =  MAXDOUBLE;
   GeoPoint centroid;
   int i;

#ifdef POLAR_MODE
   TransverseMercatorProjection pj = dggrs.tmPJ;
#else
   WebMercatorProjection pj = dggrs.wmPJ;
#endif
   //bool includesNorthPole = e.tl.y > Pi/2 && e.br.y < Pi/2 && e.tl.x < -3*Pi/4 && e.br.x > -3*Pi/4;
   //bool includesSouthPole = e.tl.y > -Pi/2 && e.br.y < -Pi/2 && e.tl.x < -3*Pi/4 && e.br.x > -3*Pi/4;

   dggrs.SpatialID::getZoneCRSVertices(zone, 0, dp);

   dggrs.SpatialID::getZoneWGS84Centroid(zone, centroid);

   for(i = 0; i < 4; i++)
   {
      const Pointd * p = &dp[i], * np = &dp[i == 3 ? 0 : i+1];
      int numAnchors = NUM_SID_ANCHORS;
      int j;
      double dx = np->x - p->x, dy = np->y - p->y;

      for(j = 0; j < numAnchors; j++)
      {
         Pointd in { p->x + dx * j / numAnchors, p->y + dy * j / numAnchors };
         GeoPoint out;
         // Pointd nin { p->x + dx * (j+1) / numAnchors, p->y + dy * (j+1) / numAnchors };

         if(pj.inverse(in, out, false))
         {
            Radians dLon = out.lon - centroid.lon;

            if(dLon > Pi) dLon -= 2*Pi, out.lon -= 2*Pi;
            if(dLon <-Pi) dLon += 2*Pi, out.lon += 2*Pi;

            if(dLon > maxDLon)
               maxDLon = dLon, urLon = out.lon;
            if(dLon < minDLon)
               minDLon = dLon, llLon = out.lon;

            if(fabs((Radians)out.lat) > Pi/2 - 0.1 /*1E-9*/ && count && fabs((Radians)out.lon - (Radians)outVertices[count-1].lon) > Pi/6)
            {
               GeoPoint outLon;
               in.y = in.y - Sgn(in.y) * 1E-11;
               pj.inverse(in, outLon, false);
               out.lon = outLon.lon;

               if(Pi/2 - fabs((Radians)outVertices[count-1].lat) > 0.001)
               {
                  outVertices[count].lat = Sgn(out.lat) * Pi/2;
                  outVertices[count].lon = outVertices[count-1].lon;
                  count++;
               }
               else if(fabs((Radians)outVertices[count-1].lon - (Radians)out.lon) > Pi/6)
               {
                  outVertices[count].lat = Sgn(out.lat) * Pi/2;
                  outVertices[count].lon = out.lon;
                  count++;
               }
            }

            outVertices[count++] = out;

            /*
            if(crossingDateline && includesSouthPole)
            {
               if(fabs((Radians)out.lon - -Pi) > 1E-9)
                  outVertices[count++] = { out.lat, -180 };
               outVertices[count++] = { -90, -180 };
               outVertices[count++] = { -90, 180 };
               if(fabs((Radians)out.lon - Pi) > 1E-9)
                  outVertices[count++] = { out.lat, 180 };
            }
            if(crossingDateline && includesNorthPole)
            {
               if(fabs((Radians)out.lon - Pi) > 1E-9)
                  outVertices[count++] = { out.lat, 180 };
               outVertices[count++] = { 90, 180 };
               outVertices[count++] = { 90, -180 };
               if(fabs((Radians)out.lon - -Pi) > 1E-9)
                  outVertices[count++] = { out.lat, -180 };
            }
            */
         }
#ifdef _DEBUG
         else
         {
            PrintLn("WARNING: Failure to inverse project");
            // pj.inverse(in, out, false);
         }
#endif
      }
   }

   if(fabs(llLon - -Pi) < 1E-9)
      urLon = Pi;
   if(fabs(urLon - Pi) < 1E-9)
      llLon = -Pi;

   for(i = 0; i < count; i++)
      if((Radians)outVertices[i].lon > (Radians)urLon + 1E-11)
         outVertices[i].lon -= 2*Pi;
      else if(outVertices[i].lon < (Radians)llLon - 1E-11)
         outVertices[i].lon += 2*Pi;
   return count;
}
