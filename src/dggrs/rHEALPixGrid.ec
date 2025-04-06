public import IMPORT_STATIC "ecrt"
private:

import "dggrs"
import "rHEALPix"

#include <stdio.h>

define RHP_MAX_VERTICES = 200; // * 1024;

class RHPZone : DGGRSZone
{
public:
   uint level:5:59, row:29:30, col:30:0;

   property RHPZone parent
   {
      get
      {
         int level = this.level;
         if(level > 0)
            return { level - 1, row / 3, col / 3 };
         return nullZone;
      }
   }

   property Pointd centroid
   {
      get
      {
         int64 p = (int64)pow(3, level);
         value.x = -Pi    + (col + 0.5) * Pi/2 / p;
         value.y = 3*Pi/4 - (row + 0.5) * Pi/2 / p;
      }
   }

   property CRSExtent rhpExtent
   {
      get
      {
         int64 p = (int64)pow(3, level);
         value.tl.y = 3*Pi/4 - row * Pi/2 / p;
         value.tl.x = -Pi    + col * Pi/2 / p;
         value.br.x = value.tl.x + Pi/2 / p;
         value.br.y = value.tl.y - Pi/2 / p;
      }
   }

   RHPZone ::fromPoint(const Pointd v, int level)
   {
      int row, col;
      int p = (int)pow(3, level);

      row = Max(0, Min(3 * p - 1, (int)((3*Pi/4 - v.y) * p / (Pi/2))));
      col = Max(0, Min(((row / p) == 1 ? 4 * p : p) - 1, (int)((v.x + Pi) * p / (Pi/2))));
      return { level, row, col };
   }

   Array<Pointd> getSubZoneCentroids(int depth)
   {
      int p = (int)pow(3, depth);
      Array<Pointd> centroids { size = p * p };
      int r, c, i = 0;
      CRSExtent e = rhpExtent;
      double w = e.br.x - e.tl.x, h = e.br.y - e.tl.y;

      for(r = 0; r < p; r++)
         for(c = 0; c < p; c++, i++)
            centroids[i] = { e.tl.x + c * w / p, e.tl.y + r * h / p };
      return centroids;
   }

   int getChildren(RHPZone * children)
   {
      int level = this.level, row = this.row, col = this.col;

      if(level < 16)
      {
         int r, c;

         for(r = 0; r < 3; r++)
            for(c = 0; c < 3; c++)
               children[r * 3 + c] = RHPZone { level + 1, row * 3 + r, col * 3 + c };
         return 9;
      }
      return 0;
   }
}

public class rHEALPix : DGGRS
{
   rHEALPixProjection pj { };

   uint64 countZones(int level)
   {
      return (uint64)(6 * pow(9, level));
   }

   double getZoneArea(RHPZone zoneID)
   {
      double area;
      double zoneCount = 6 * pow(9, zoneID.level);
      static double earthArea = 0;
      if(!earthArea) earthArea = wholeWorld.geodeticArea;

      area = earthArea / zoneCount;
      return area;
   }

   int getMaxDGGRSZoneLevel() { return 16; }
   int getRefinementRatio() { return 9; }
   int getMaxParents() { return 1; }
   int getMaxNeighbors() { return 4; }
   int getMaxChildren() { return 9; }

   uint64 countSubZones(RHPZone zone, int depth)
   {
      return (uint64)pow(9, depth);
   }

   int getZoneLevel(RHPZone zone)
   {
      return zone.level;
   }

   int countZoneEdges(RHPZone zone) { return 4; }

   int getZoneParents(RHPZone zone, RHPZone * parents)
   {
      parents[0] = nullZone;
      if(zone.level > 0)
         parents[0] = zone.parent;
      return parents[0] != nullZone;
   }

   int getZoneChildren(RHPZone zone, RHPZone * children)
   {
      return zone.getChildren(children);
   }

   int getZoneNeighbors(RHPZone zone, RHPZone * neighbors, int * nbType)
   {
      int level = zone.level, row = zone.row, col = zone.col;
      int p = (int)pow(3, level);
      int rr = row / p;

      // Left
      if(col > 0)
         neighbors[0] = { level, row, col - 1 };
      else if(rr == 1) // Equatorial wrap around
         neighbors[0] = { level, row, 4 * p - 1 };
      else if(rr == 0) // North -> equatorial
         neighbors[0] = { level, p, 4 * p - (p-row) };
      else if(rr == 2) // South -> equatorial
         neighbors[0] = { level, 2*p-1, 4 * p - 1 - (row - 2*p) };

      // Right
      if(col < (rr == 1 ? 4*p : p) - 1)
         neighbors[1] = { level, row, col + 1 };
      else if(rr == 1) // Equatorial wrap around
         neighbors[1] = { level, row, 0 };
      else if(rr == 0) // North -> equatorial
         neighbors[1] = { level, p, p + (p-1-row) };
      else if(rr == 2) // South -> equatorial
         neighbors[1] = { level, 2*p-1, p + (row - 2*p) };

      // Top
      if(row > 0 && (col < p || row > p))
         neighbors[2] = { level, row - 1, col };
      else if(rr == 0) // North -> equatorial
         neighbors[2] = { level, p, 2*p + (p-1-col) };
      else if(rr == 1) // Equatorial -> North
      {
         if(col < 2 * p)
            neighbors[2] = { level, (p - (col - p)) - 1, p-1 };  // FIXME: eC bug here if using row =  or col =
         else if(col < 3 * p)
            neighbors[2] = { level, 0, p - 1 - (col - 2*p) };
         else
            neighbors[2] = { level, col - 3*p, 0 };
      }

      // Bottom
      if(row < 3*p-1 && (col < p || row < 2*p-1))
         neighbors[3] = { level, row + 1, col };
      else if(rr == 2) // South -> equatorial
         neighbors[3] = { level, 2*p-1, 2*p + (p-col) - 1 };
      else if(rr == 1) // Equatorial -> South
      {
         if(col < 2 * p)
            neighbors[3] = { level, 2*p + (col - p), p-1 };
         else if(col < 3 * p)
            neighbors[3] = { level, 3*p-1, p - 1 - (col - 2*p) };
         else
            neighbors[3] = { level, 2*p + p - (col - 3*p) - 1, 0 };
      }

      if(nbType)
         nbType[0] = 0, nbType[1] = 1, nbType[2] = 2, nbType[3] = 3;
      return 4;
   }

   RHPZone getZoneFromWGS84Centroid(int level, const GeoPoint centroid)
   {
      if(level <= 16)
      {
         Pointd v;

         pj.forward(centroid, v);

         return RHPZone::fromPoint(v, level);
      }
      return nullZone;
   }

   void getZoneWGS84Centroid(RHPZone zone, GeoPoint centroid)
   {
      pj.inverse(zone.centroid, centroid, false);
   }

   // Text ZIRS
   void getZoneTextID(RHPZone zone, String zoneID)
   {
      int level = zone.level;
      int64 p = (int64)pow(3, level);
      int row = zone.row, col = zone.col;
      int r = (int)(row / p), c = (int)(col / p);
      int i;

      zoneID[0] = r == 0 ? 'N' : r == 2 ? 'S' : (char)('O' + c);
      for(i = 1; i <= level; i++)
      {
         row -= r * p;
         col -= c * p;
         p /= 3;
         r = (int)(row / p);
         c = (int)(col / p);
         zoneID[i] = (char)('0' + r * 3 + c);
      }
      zoneID[i] = 0;
   }

   DGGRSZone getZoneFromTextID(const String zoneID)
   {
      char ch = zoneID[0];
      int r = ch == 'N' ? 0 : ch == 'S' ? 2 : (ch >= 'O' && ch <= 'R') ? 1 : -1;
      if(r == -1)
         return nullZone;
      else
      {
         int c = r == 1 ? ch - 'O' : 0;
         int level = 0;
         while((ch = zoneID[level+1]))
         {
            if(level < 16 && ch >= '0' && ch <= '8')
            {
               int n = ch - '0';
               c *= 3;
               r *= 3;
               r += n / 3;
               c += n % 3;
               level++;
            }
            else
               return nullZone;
         }
         return RHPZone { level, r, c };
      }
   }

   // Sub-zone Order
   RHPZone getFirstSubZone(RHPZone parent, int depth)
   {
      int level = parent.level + depth;
      if(level <= 16)
      {
         int p = (int)pow(3, depth);
         return RHPZone { level, parent.row * p, parent.col * p };
      }
      return nullZone;
   }


   void compactZones(Array<DGGRSZone> zones)
   {
      int maxLevel = 0, i, count = zones.count;
      AVLTree<RHPZone> zonesTree { };

      for(i = 0; i < count; i++)
      {
         RHPZone zone = (RHPZone)zones[i];
         if(zone != nullZone)
         {
            int level = zone.level;
            if(level > maxLevel)
               maxLevel = level;
            zonesTree.Add(zone);
         }
      }

      compactRHPZones(zonesTree, maxLevel);
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
      AVLTree<RHPZone> zonesTree { };
      Array<RHPZone> zones { };
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
         RHPZone tlZone, brZone;

         pj.forward(equatorial.ll, ll);
         pj.forward(equatorial.ur, ur);

         tlZone = RHPZone::fromPoint({ ll.x + 1E-15, ur.y - 1E-15}, level);
         brZone = RHPZone::fromPoint({ ur.x - 1E-15, ll.y + 1E-15 }, level);

         for(r = tlZone.row; r <= brZone.row; r++)
            for(c = tlZone.col; c <= brZone.col; c++)
               zones.Add({ level, r, c });
      }

      if(north.nonNull)
      {
         int p = (int)pow(3, level);

         for(r = 0; r < p; r++)
            for(c = 0; c < p; c++)
            {
               RHPZone zone { level, r, c };
               GeoExtent e;

               getZoneWGS84Extent(zone, e);

               if(e.intersects(north))
                  zones.Add(zone);
            }
      }
      if(south.nonNull)
      {
         int p = (int)pow(3, level);

         for(r = 0; r < p; r++)
            for(c = 0; c < p; c++)
            {
               RHPZone zone { level, 2*p+r, c };
               GeoExtent e;

               getZoneWGS84Extent(zone, e);

               if(e.intersects(south))
                  zones.Add(zone);
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

   Array<GeoPoint> getSubZoneWGS84Centroids(RHPZone parent, int depth)
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
   Array<GeoPoint> getZoneRefinedWGS84Vertices(RHPZone zone, int edgeRefinement)
   {
      GeoPoint v[RHP_MAX_VERTICES];
      int count = getRHPRefinedWGS84Vertices(this, zone, v);
      Array<GeoPoint> vertices { size = count };
      memcpy(vertices.array, v, sizeof(GeoPoint) * count);
      return vertices;
   }

   int getZoneWGS84Vertices(RHPZone zone, GeoPoint * vertices)
   {
      CRSExtent extent = zone.rhpExtent;
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

   void getZoneWGS84Extent(RHPZone zone, GeoExtent value)
   {
      CRSExtent e = zone.rhpExtent;
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
         if(v[i].lon < value.ll.lon) value.ll.lon = v[i].lon;
         if(v[i].lon > value.ur.lon) value.ur.lon = v[i].lon;
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

   RHPZone getZoneFromCRSCentroid(int level, CRS crs, const Pointd centroid)
   {
      if(level <= 16)
      {
         switch(crs)
         {
            case 0: case CRS { ogc, 99999 }: return RHPZone::fromPoint(centroid, level);
            case CRS { epsg, 4326 }:
            case CRS { ogc, 84 }:
               return (RHPZone)getZoneFromWGS84Centroid(level,
                  crs == { ogc, 84 } ?
                     { centroid.y, centroid.x } :
                     { centroid.x, centroid.y });
         }
      }
      return nullZone;
   }

   Array<DGGRSZone> getSubZones(DGGRSZone parent, int relativeDepth)
   {
      int level = parent.level + relativeDepth;
      int row = parent.row, col = parent.col;
      int p = (int)pow(3, relativeDepth);
      Array<DGGRSZone> subZones { size = p * p };
      int r, c, i = 0;

      for(r = 0; r < p; r++)
         for(c = 0; c < p; c++, i++)
            subZones[i] = RHPZone { level, row * p + r, col * p + c };
      return subZones;
   }

   void getZoneCRSCentroid(RHPZone zone, CRS crs, Pointd centroid)
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

   int getZoneCRSVertices(RHPZone zone, CRS crs, Pointd * vertices)
   {
      uint count = 0, i;
      CRSExtent extent = zone.rhpExtent;
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
   Array<Pointd> getZoneRefinedCRSVertices(RHPZone zone, CRS crs, int edgeRefinement)
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
            GeoPoint v[RHP_MAX_VERTICES];
            int count = getRHPRefinedWGS84Vertices(this, zone, v), i;
            Array<Pointd> vertices { size = count };
            for(i = 0; i < count; i++)
               vertices[i] = crs == { ogc, 84 } ? { v[i].lat, v[i].lon } : { v[i].lon, v[i].lat };
            return vertices;
         }
      }
      return null;
   }

   void getZoneCRSExtent(RHPZone zone, CRS crs, CRSExtent extent)
   {
      switch(crs)
      {
         case 0: case CRS { ogc, 99999 }: extent = zone.rhpExtent; break;
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

   Array<Pointd> getSubZoneCRSCentroids(RHPZone parent, CRS crs, int depth)
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
}

static void compactRHPZones(AVLTree<RHPZone> zones, int level)
{
   AVLTree<RHPZone> output { };
   AVLTree<RHPZone> next { };
   int l;

   for(l = level - 1; l >= 0; l--)
   {
      int i;
      for(z : zones)
      {
         RHPZone zone = z, parent = zone.parent;
         if(!next.Find(parent))
         {
            bool parentAllIn = true;
            RHPZone children[9];

            parent.getChildren(children);

            for(i = 0; i < 9; i++)
            {
               RHPZone ch = children[i];
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
static uint getRHPRefinedWGS84Vertices(rHEALPix dggrs, RHPZone zone, GeoPoint * outVertices)
{
   #define NUM_RHP_ANCHORS 30
   uint count = 0;
   CRSExtent e = zone.rhpExtent;
   Pointd dp[4] = { {e.tl.x, e.tl.y}, {e.tl.x, e.br.y}, {e.br.x, e.br.y}, {e.br.x, e.tl.y} };
   Radians maxDLon = -99999, urLon = -MAXDOUBLE;
   Radians minDLon =  99999, llLon =  MAXDOUBLE;
   GeoPoint centroid;
   int i;
   rHEALPixProjection pj = dggrs.pj;
   bool includesNorthPole = e.tl.y > Pi/2 && e.br.y < Pi/2 && e.tl.x < -3*Pi/4 && e.br.x > -3*Pi/4;
   bool includesSouthPole = e.tl.y > -Pi/2 && e.br.y < -Pi/2 && e.tl.x < -3*Pi/4 && e.br.x > -3*Pi/4;

   dggrs.getZoneWGS84Centroid(zone, centroid);

   for(i = 0; i < 4; i++)
   {
      const Pointd * p = &dp[i], * np = &dp[i == 3 ? 0 : i+1];
      int numAnchors = NUM_RHP_ANCHORS;
      int j;
      double dx = np->x - p->x, dy = np->y - p->y;

      for(j = 0; j < numAnchors; j++)
      {
         Pointd in { p->x + dx * j / numAnchors, p->y + dy * j / numAnchors };
         GeoPoint out;
         Pointd nin { p->x + dx * (j+1) / numAnchors, p->y + dy * (j+1) / numAnchors };
         int ps = pj.getPolarSection(in), nps = pj.getPolarSection(nin);
         bool crossingDateline = includesNorthPole ? (ps == 3 && nps == 0) : (ps == 0 && nps == 3);

         if(crossingDateline)
            nps = pj.getPolarSection(nin);

         if(pj.inverse(in, out, false))
         {
            Radians dLon = out.lon - centroid.lon;

            if(dLon > Pi) dLon -= 2*Pi, out.lon -= 2*Pi;
            if(dLon <-Pi) dLon += 2*Pi, out.lon += 2*Pi;

            if(dLon > maxDLon)
               maxDLon = dLon, urLon = out.lon;
            if(dLon < minDLon)
               minDLon = dLon, llLon = out.lon;

            outVertices[count++] = out;

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
         }
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
