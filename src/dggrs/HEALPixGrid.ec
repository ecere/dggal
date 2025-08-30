public import IMPORT_STATIC "ecrt"
private:

import "dggrs"
import "rHEALPix"

import "Vector3D"

#include <stdio.h>

static define POW_EPSILON = 0.1;

define HP_MAX_VERTICES = 200; // * 1024;

class HPZone : private DGGRSZone
{
public:
   uint level:5:56;
   uint rootRhombus:4:52;
   uint64 subIndex:52:0;

   // These are only to avoid mistakingly accessing the invalid base DGGRSZone row and col properties
   // The formulas should be used directly to avoid the property overhead.
   property int row
   {
      get { return (int)(subIndex >> level); }
   }
   property int col
   {
      get { return (int)(subIndex - (((int64)(subIndex >> level)) << level)); }
   }
   property HPZone parent
   {
      get
      {
         int level = this.level;
         if(level > 0)
         {
            int row = (int)(subIndex >> level);
            int col = (int)(subIndex - ((int64)row << level));
            int pLevel = level - 1;
            return { pLevel, rootRhombus, ((row >> 1) << pLevel) | (col >> 1) };
         }
         return nullZone;
      }
   }

   property Pointd centroid
   {
      get
      {
         int root = rootRhombus, rCol = root & 3, rRow = (root >> 2);
         int64 p = 1LL << level;
         double oop = 1.0 / p;
         int row = (int)(subIndex >> level);
         int col = (int)(subIndex - ((int64)row << level));
         double x = rCol + (int)(rRow == 0) + col * oop;
         double y = rCol + (int)(rRow == 2) + row * oop;

         value =
         {
            x = (x + y + oop) * Pi/4 - 5*Pi/4,
            y = -(y - x) * Pi/4
         };
      }
   }

   property CRSExtent hpExtent
   {
      get
      {
         int root = rootRhombus, rCol = root & 3, rRow = (root >> 2);
         int64 p = 1LL << level;
         double oop = 1.0 / p;
         int row = (int)(subIndex >> level);
         int col = (int)(subIndex - ((int64)row << level));
         double x = rCol + (int)(rRow == 0) + col * oop;
         double y = rCol + (int)(rRow == 2) + row * oop;

         /*
         Conversion from 4x6 to HEALPix:
            x = (x + y) * Pi/4 - 5*Pi/4,
            y = -(y - x) * Pi/4
         */

         value.tl = {
            x = (x + y) * Pi/4 - 5*Pi/4,
            y = -(y - (x + oop)) * Pi/4
         };
         value.br = {
            x = (x + y + 2*oop) * Pi/4 - 5*Pi/4,
            y = -((y + oop) - x) * Pi/4
         };
      }
   }

   HPZone ::fromPoint(const Pointd v, int level)
   {
      HPZone zone = nullZone;
      int64 p = 1LL << level;
      // Conversion from HEALPix to 4x6 space:
      double x = (v.y + v.x + 5 * Pi / 4) * 2/Pi;
      double y = v.x * 4 / Pi + 5 - x;
      int cx = (int)(x + 1E-11);
      int cy = (int)(y + 1E-11);
      double sx = x - cx, sy = y - cy;
      bool addX = cx > cy, addY = cy > cx;
      int rCol = cx - addX; // or cy - addY
      int rRow = addX ? 0 : addY ? 2 : 1;
      int64 col = (int64)(sx * p);
      int64 row = (int64)(sy * p);
      int root = (rRow << 2) | rCol;

      if(rCol >= 0 && rCol <= 4 && rRow >= 0 && rRow <= 2 && col >= 0 && col < p && row >= 0 && row < p)
         zone = { level, root, ((int64)row << level) | col };
      return zone;
   }

   Array<Pointd> getSubZoneCentroids(int depth)
   {
      uint dm = 1 << depth;
      Array<Pointd> centroids { size = dm * dm };
      int r, c, i = 0;
      CRSExtent e = hpExtent;
      double w = e.br.x - e.tl.x, h = e.br.y - e.tl.y;

      for(r = 0; r < dm; r++)
         for(c = 0; c < dm; c++, i++)
            centroids[i] = { e.tl.x + c * w / dm, e.tl.y + r * h / dm };
      return centroids;
   }

   int getChildren(HPZone * children)
   {
      int level = this.level;
      uint rootRhombus = this.rootRhombus;
      uint64 subIndex = this.subIndex;
      uint64 p = 1LL << level;
      uint row = (uint)(subIndex / p), col = (uint)(subIndex - row * p);

      if(level < 25)
      {
         int r, c;

         for(r = 0; r < 2; r++)
            for(c = 0; c < 2; c++)
               children[r * 2 + c] = HPZone { level + 1, rootRhombus, (row * 2 + r) * (2*p) + (col * 2 + c) };
         return 4;
      }
      return 0;
   }
}

public class HEALPix : DGGRS
{
   HEALPixProjection pj { };

   void ::cartesianToGeo(const Vector3D c, GeoPoint out)
   {
      double p = sqrt(c.x*c.x + c.z*c.z);

      out = { (Radians)atan2(-c.y, p), (Radians)atan2(c.x, -c.z) };
   }

   uint64 countZones(int level)
   {
      return (uint64)(12 * (pow(4, level)) + POW_EPSILON);
   }

   double getZoneArea(HPZone zoneID)
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

   uint64 countSubZones(HPZone zone, int depth)
   {
      return 1LL << (2 * depth);
   }

   int getZoneLevel(HPZone zone)
   {
      return zone.level;
   }

   int countZoneEdges(HPZone zone) { return 4; }

   int getZoneParents(HPZone zone, HPZone * parents)
   {
      parents[0] = nullZone;
      if(zone.level > 0)
         parents[0] = zone.parent;
      return parents[0] != nullZone;
   }

   int getZoneChildren(HPZone zone, HPZone * children)
   {
      return zone.getChildren(children);
   }

   int getZoneNeighbors(HPZone zone, HPZone * neighbors, int * nbType)
   {
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
            /*root == 4 ? */0xB;
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
            /*root == 0xB ? */4;
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
            /*root == 4 ? **/ 3;
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
            /*root == 3 ? */4;
         neighbors[3] = { level, bRoot, ((int64)0 << level) | col };
      }

      if(nbType)
         nbType[0] = 0, nbType[1] = 1, nbType[2] = 2, nbType[3] = 3;
      return 4;
   }

   HPZone getZoneFromWGS84Centroid(int level, const GeoPoint centroid)
   {
      if(level <= 26)
      {
         Pointd v;

         pj.forward(centroid, v);

         return HPZone::fromPoint(v, level);
      }
      return nullZone;
   }

   void getZoneWGS84Centroid(HPZone zone, GeoPoint centroid)
   {
      pj.inverse(zone.centroid, centroid, false);
   }

   // Text ZIRS
   void getZoneTextID(HPZone zone, String zoneID)
   {
      int level = zone.level;
      int root = zone.rootRhombus;
      uint64 subIndex = zone.subIndex;
      sprintf(zoneID, __runtimePlatform == win32 ? "%c%X-%I64X" : "%c%X-%llX",
         (char)(level + 'A'), root, subIndex);
   }

   DGGRSZone getZoneFromTextID(const String zoneID)
   {
      HPZone result = nullZone;
      char levelChar;
      uint root;
      uint64 ix;

      if(sscanf(zoneID, __runtimePlatform == win32 ? "%c%d-%I64X" : "%c%d-%llX", &levelChar, &root, &ix) == 3 &&
      levelChar >= 'A' && levelChar <= 'Z')
      {
         // TODO: Additional validation
         result = { levelChar - 'A', root, ix };
      }
      return result;
   }

   // Sub-zone Order
   HPZone getFirstSubZone(HPZone parent, int depth)
   {
      int pLevel = parent.level, level = pLevel + depth;
      if(level <= 26)
      {
         uint root = parent.rootRhombus;
         uint64 pSubIndex = parent.subIndex;
         uint dm = 1 << depth;
         int pRow = (int)(pSubIndex >> pLevel);
         int pCol = (int)(pSubIndex - ((int64)pRow << pLevel));
         return HPZone { level, root, (((uint64)pRow * dm) << level) | (pCol * dm) };
      }
      return nullZone;
   }

   Array<DGGRSZone> getSubZones(HPZone parent, int relativeDepth)
   {
      int pLevel = parent.level, level = pLevel + relativeDepth;
      if(level <= 26)
      {
         uint root = parent.rootRhombus;
         uint64 pSubIndex = parent.subIndex;
         uint dm = 1 << relativeDepth;
         int pRow = (int)(pSubIndex >> pLevel);
         int pCol = (int)(pSubIndex - ((int64)pRow << pLevel));
         Array<DGGRSZone> subZones { size = dm * dm };
         int r, c, i = 0;

         for(r = 0; r < dm; r++)
            for(c = 0; c < dm; c++, i++)
               subZones[i] = HPZone { level, root, (((uint64)pRow * dm + r) << level) | (pCol * dm + c) };
         return subZones;
      }
      return null;
   }

   Array<Pointd> getSubZoneCRSCentroids(HPZone parent, CRS crs, int depth)
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
                  pj.inverse(centroids[i], geo, false);
                  centroids[i] = crs == { ogc, 84 } ? { geo.lon, geo.lat } : { geo.lat, geo.lon };
               }
               break;
            default: delete centroids;
         }
      }
      return centroids;
   }

   Array<GeoPoint> getSubZoneWGS84Centroids(HPZone parent, int depth)
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

   void compactZones(Array<DGGRSZone> zones)
   {
      int maxLevel = 0, i, count = zones.count;
      AVLTree<HPZone> zonesTree { };

      for(i = 0; i < count; i++)
      {
         HPZone zone = (HPZone)zones[i];
         if(zone != nullZone)
         {
            int level = zone.level;
            if(level > maxLevel)
               maxLevel = level;
            zonesTree.Add(zone);
         }
      }

      compactHPZones(zonesTree, maxLevel);
      zones.Free();

      count = zonesTree.count;
      zones.size = count;
      i = 0;
      for(z : zonesTree)
         zones[i++] = z;
      delete zonesTree;
   }

   /*
   void addPolarZones(AVLTree<HPZone> zonesTree, HPZone pZone, int level, const GeoExtent bbox)
   {
      GeoExtent e;
      getZoneWGS84Extent(pZone, e);

      if(e.intersects(bbox))
      {
         int zLevel = pZone.level;
         if(level == zLevel)
            zonesTree.Add(pZone);
         else
         {
            int sr = pZone.row * 3, sc = pZone.col * 3, r, c;

            for(r = sr; r < sr + 3; r++)
               for(c = sc; c < sc + 3; c++)
                  addPolarZones(zonesTree, { zLevel + 1, r, c }, level, bbox);
         }
      }
   }
   */

   Array<DGGRSZone> listZones(int level, const GeoExtent bbox)
   {
      AVLTree<HPZone> zonesTree { };
      Array<HPZone> zones { };
      int root;
      int l;

      for(root = 0; root < 12; root++)
         zonesTree.Add({ 0, root, 0 });

      if(level == 0 && bbox != null)
      {
         AVLTree<HPZone> tmp { };

         for(z : zonesTree)
         {
            HPZone zone = (HPZone)z;
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
         AVLTree<HPZone> tmp { };

         for(z : zonesTree)
         {
            HPZone zz = z;
            HPZone children[4];
            int i;
            int n = zz.getChildren(children);

            for(i = 0; i < n; i++)
            {
               HPZone c = children[i];
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

/*
      // TODO:
      Radians bound = pj.latAuthalicToGeodetic(asin(2/3.0));
      GeoExtent equatorial, north, south;
      int r, c;

      equatorial.clip(bbox, { { -bound, -180 }, { bound, 180 } });
      north.clip(bbox, { { bound, -180 }, { 90, 180 } });
      south.clip(bbox, { { -90, -180 }, { -bound, 180 } });

      if(equatorial.nonNull)
      {
         // REVIEW: Dateline handling
         Pointd ll, ur;
         HPZone tlZone, brZone;
         Radians dLon = equatorial.ur.lon - equatorial.ll.lon;
         if(dLon < 0) dLon += 2 * Pi;

         pj.forward(equatorial.ll, ll);
         pj.forward(equatorial.ur, ur);

         if(fabs(ur.x - ll.x) < dLon / 2)
            ll.x = -Pi, ur.x = Pi;

         tlZone = HPZone::fromPoint({ ll.x + 1E-15, ur.y - 1E-15 }, level);
         brZone = HPZone::fromPoint({ ur.x - 1E-15, ll.y + 1E-15 }, level);

         for(r = tlZone.row; r <= brZone.row; r++)
         {
            if(brZone.col >= tlZone.col)
            {
               for(c = tlZone.col; c <= brZone.col; c++)
                  zonesTree.Add({ level, r, c });
            }
            else
            {
               for(c = 0; c <= tlZone.col; c++)
                  zonesTree.Add({ level, r, c });

               for(c = brZone.col; c < 4 * POW3(level); c++)
                  zonesTree.Add({ level, r, c });
            }
         }
      }

      if(north.nonNull)
         addPolarZones(zonesTree, { 0, 0, 0 }, level, north);
      if(south.nonNull)
         addPolarZones(zonesTree, { 0, 2, 0 }, level, south);
*/

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
   Array<GeoPoint> getZoneRefinedWGS84Vertices(HPZone zone, int edgeRefinement)
   {
      GeoPoint v[HP_MAX_VERTICES];
      int count = getHPRefinedWGS84Vertices(this, zone, v);
      Array<GeoPoint> vertices { size = count };
      memcpy(vertices.array, v, sizeof(GeoPoint) * count);
      return vertices;
   }

   int getZoneWGS84Vertices(HPZone zone, GeoPoint * vertices)
   {
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

      /*
      Conversion from 4x6 to HEALPix:
         x =  (x + y) * Pi/4 - 5*Pi/4,
         y = -(y - x) * Pi/4
      */

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
      return count;
   }

   void getZoneWGS84Extent(HPZone zone, GeoExtent value)
   {
      // REVIEW:
      CRSExtent e = zone.hpExtent;
      GeoPoint v[4];
      int i;
      bool includesNorthPole = e.tl.y > Pi/2 && e.br.y < Pi/2 && e.tl.x < -3*Pi/4 && e.br.x > -3*Pi/4;
      bool includesSouthPole = e.tl.y > -Pi/2 && e.br.y < -Pi/2 && e.tl.x < -3*Pi/4 && e.br.x > -3*Pi/4;

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
      if(value.ur.lon - value.ll.lon > Pi)
      {
         value.ll.lon = Pi;
         value.ur.lon = -Pi;
         for(i = 0; i < 4; i++)
         {
            if(v[i].lon > 0 && v[i].lon < value.ll.lon) value.ll.lon = v[i].lon;
            if(v[i].lon < 0 && v[i].lon > value.ur.lon) value.ur.lon = v[i].lon;
         }
      }

      if(value.ll.lon < -180)
         value.ll.lon += 360;

      if(includesNorthPole)
      {
         value.ll.lon = -180;
         value.ur.lon =  180;
         value.ur.lat =  90;
      }
      else if(includesSouthPole)
      {
         value.ll.lon = -180;
         value.ur.lon =  180;
         value.ll.lat =  -90;
      }
   }

   HPZone getZoneFromCRSCentroid(int level, CRS crs, const Pointd centroid)
   {
      if(level <= 26)
      {
         switch(crs)
         {
            case 0: case CRS { ogc, 99999 }: return HPZone::fromPoint(centroid, level);
            case CRS { epsg, 4326 }:
            case CRS { ogc, 84 }:
               return (HPZone)getZoneFromWGS84Centroid(level,
                  crs == { ogc, 84 } ?
                     { centroid.y, centroid.x } :
                     { centroid.x, centroid.y });
         }
      }
      return nullZone;
   }

   void getZoneCRSCentroid(HPZone zone, CRS crs, Pointd centroid)
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

   int getZoneCRSVertices(HPZone zone, CRS crs, Pointd * vertices)
   {
      uint count = 0, i;
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

      /*
      Conversion from 4x6 to HEALPix:
         x =  (x + y) * Pi/4 - 5*Pi/4,
         y = -(y - x) * Pi/4
      */

      v[0].x = (x + y) * Pi/4 - 5*Pi/4;
      v[0].y = -(y - x) * Pi/4;

      v[1].x = (x + y + oop) * Pi/4 - 5*Pi/4;
      v[1].y = -(y + oop - x) * Pi/4;

      v[2].x = (x + y + 2*oop) * Pi/4 - 5*Pi/4;
      v[2].y = -(y - x) * Pi/4;

      v[3].x = (x + oop + y) * Pi/4 - 5*Pi/4;
      v[3].y = -(y - x - oop) * Pi/4;

      switch(crs)
      {
         case 0: case CRS { ogc, 99999 }:
            count = 4;
            memcpy(vertices, v, sizeof(Pointd) * 4);
            break;
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

   Array<Pointd> getZoneRefinedCRSVertices(HPZone zone, CRS crs, int edgeRefinement)
   {
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
            GeoPoint v[HP_MAX_VERTICES];
            int count = getHPRefinedWGS84Vertices(this, zone, v), i;
            Array<Pointd> vertices { size = count };
            for(i = 0; i < count; i++)
               vertices[i] = crs == { ogc, 84 } ? { v[i].lat, v[i].lon } : { v[i].lon, v[i].lat };
            return vertices;
         }
      }
      return null;
   }

   void getZoneCRSExtent(HPZone zone, CRS crs, CRSExtent extent)
   {
      switch(crs)
      {
         case 0: case CRS { ogc, 99999 }: extent = zone.hpExtent; break;
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

static void compactHPZones(AVLTree<HPZone> zones, int level)
{
   AVLTree<HPZone> output { };
   AVLTree<HPZone> next { };
   int l;

   for(l = level - 1; l >= 0; l--)
   {
      int i;
      for(z : zones)
      {
         HPZone zone = z, parent = zone.parent;
         if(!next.Find(parent))
         {
            bool parentAllIn = true;
            HPZone children[4];
            int n = parent.getChildren(children);

            for(i = 0; i < n; i++)
            {
               HPZone ch = children[i];
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

static uint getHPRefinedWGS84Vertices(HEALPix dggrs, HPZone zone, GeoPoint * outVertices)
{
   #define NUM_HP_ANCHORS 30
   uint count = 0;
   Pointd dp[4];
   Radians maxDLon = -99999, urLon = -MAXDOUBLE;
   Radians minDLon =  99999, llLon =  MAXDOUBLE;
   GeoPoint centroid;
   int i;
   HEALPixProjection pj = dggrs.pj;
   //bool includesNorthPole = e.tl.y > Pi/2 && e.br.y < Pi/2 && e.tl.x < -3*Pi/4 && e.br.x > -3*Pi/4;
   //bool includesSouthPole = e.tl.y > -Pi/2 && e.br.y < -Pi/2 && e.tl.x < -3*Pi/4 && e.br.x > -3*Pi/4;

   dggrs.getZoneCRSVertices(zone, 0, dp);

   dggrs.getZoneWGS84Centroid(zone, centroid);

   for(i = 0; i < 4; i++)
   {
      const Pointd * p = &dp[i], * np = &dp[i == 3 ? 0 : i+1];
      int numAnchors = NUM_HP_ANCHORS;
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
