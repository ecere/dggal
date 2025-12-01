public import IMPORT_STATIC "ecrt"
private:

import "dggrs"
import "ri5x6"
import "icoVertexGreatCircle"

#include <stdio.h>

void computeTriCentroid(Pointd c, const Pointd v0, const Pointd v1, const Pointd v2)
{
   c.x = v0.x + (v1.x - v0.x + v2.x - v0.x) / 3;
   c.y = v0.y + (v1.y - v0.y + v2.y - v0.y) / 3;
}

void computePgonCentroid(Pointd c, const Pointd * v /*[5]*/)
{
   int i;

   c = { v[1].x - v[0].x, v[1].y - v[0].y };
   for(i = 2; i < 5; i++)
   {
      c.x += v[i].x - v[0].x;
      c.y += v[i].y - v[0].y;
   }
   c.x = c.x / 5 + v[0].x;
   c.y = c.y / 5 + v[0].y;
}

public class A5 : DGGRS
{
   SliceAndDiceGreatCircleIcosahedralProjection pj { };

   A5()
   {
      // Official A5 orientation
      pj.orientation = { 0, -87 };
      pj.updateOrientation();
   }

   // DGGH
   uint64 countZones(int level)
   {
      if(level == 0)
         return 12;
      else if(level == 1)
         return 60;
      else
         return 60LL << (2 * (level - 2));
   }

   int getMaxDGGRSZoneLevel() { return 20; }
   int getRefinementRatio() { return 4; }
   int getMaxParents() { return 3; } // REVIEW:
   int getMaxNeighbors() { return 5; }
   int getMaxChildren() { return 7; }

   uint64 countSubZones(A5Zone zone, int depth)
   {
      return 0; // TODO: zone.getSubZonesCount(depth);
   }

   int getZoneLevel(A5Zone zone)
   {
      return zone.level;
   }

   int countZoneEdges(A5Zone zone) { return zone.nPoints; }

   bool isZoneCentroidChild(A5Zone zone)
   {
      return false;
   }

   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   double getZoneArea(A5Zone zone)
   {
      double area;
      uint64 zoneCount = countZones(zone.level);
      static double earthArea = 0;
      if(!earthArea) earthArea = wholeWorld.geodeticArea;
      area = earthArea / zoneCount;
      return area;
   }

#if 0
   A5Zone getZoneFromCRSCentroid(int level, CRS crs, const Pointd centroid)
   {
      if(level <= 19)
      {
         switch(crs)
         {
            case 0: case CRS { ogc, 153456 }:
               return A5Zone::fromCentroid(level, centroid);
            case CRS { ogc, 1534 }:
            {
               Pointd c5x6;
               RI5x6Projection::fromIcosahedronNet({ centroid.x, centroid.y }, c5x6);
               return A5Zone::fromCentroid(level, { c5x6.x, c5x6.y });
            }
            case CRS { epsg, 4326 }:
            case CRS { ogc, 84 }:
               return (A5Zone)RhombicIcosahedral7H::getZoneFromWGS84Centroid(level,
                  crs == { ogc, 84 } ?
                     { centroid.y, centroid.x } :
                     { centroid.x, centroid.y });
         }
      }
      return nullZone;
   }

   int getZoneNeighbors(A5Zone zone, A5Zone * neighbors, I7HNeighbor * nbType)
   {
      return zone.getNeighbors(neighbors, nbType);
   }

   A5Zone getZoneCentroidParent(A5Zone zone)
   {
      A5Zone parents[2];
      int n = getZoneParents(zone, parents), i;

      for(i = 0; i < n; i++)
         if(parents[i].isCentroidChild)
            return parents[i];
      return nullZone;
   }

   A5Zone getZoneCentroidChild(A5Zone zone)
   {
      return zone.centroidChild;
   }

   int getZoneParents(A5Zone zone, A5Zone * parents)
   {
      return zone.getParents(parents);
   }

   int getZoneChildren(A5Zone zone, A5Zone * children)
   {
      return zone.getChildren(children);
   }
#endif

   // Text ZIRS
   void getZoneTextID(A5Zone zone, String zoneID)
   {
      zone.getZoneID(zoneID);
   }

   A5Zone getZoneFromTextID(const String zoneID)
   {
      return A5Zone::fromZoneID(zoneID);
   }

#if 0
   // Sub-zone Order
   A5Zone getFirstSubZone(A5Zone zone, int depth)
   {
      return zone.getFirstSubZone(depth);
   }

   void compactZones(Array<DGGRSZone> zones)
   {
      int maxLevel = 0, i, count = zones.count;
      AVLTree<A5Zone> zonesTree { };

      for(i = 0; i < count; i++)
      {
         A5Zone zone = (A5Zone)zones[i];
         if(zone != nullZone)
         {
            int level = zone.level;
            if(level > maxLevel)
               maxLevel = level;
            zonesTree.Add(zone);
         }
      }

      compactI7HZones(zonesTree, maxLevel);
      zones.Free();

      count = zonesTree.count;
      zones.size = count;
      i = 0;
      for(z : zonesTree)
         zones[i++] = z;

      delete zonesTree;
   }

   int getIndexMaxDepth()
   {
      return 19;
   }

   static bool ::findByIndex(Pointd centroid, int64 index, const Pointd c)
   {
      centroid = c;
      return false;
   }

   static bool ::findSubZone(const Pointd szCentroid, int64 index, const Pointd c)
   {
      Pointd centroid;

      canonicalize5x6(c, centroid);
      if(fabs(centroid.x - szCentroid.x) < 1E-11 &&
         fabs(centroid.y - szCentroid.y) < 1E-11)
         return false;
      return true;
      // return *zone != A5Zone::fromCentroid(zone->level, centroid);
   }

   int64 getSubZoneIndex(A5Zone parent, A5Zone subZone)
   {
      int64 ix = -1;
      int level = RhombicIcosahedral7H::getZoneLevel(parent), szLevel = RhombicIcosahedral7H::getZoneLevel(subZone);

      if(szLevel == level)
         ix = 0;
      else if(szLevel > level && RhombicIcosahedral7H::zoneHasSubZone(parent, subZone))
      {
         Pointd zCentroid;

         canonicalize5x6(subZone.centroid, zCentroid);
         ix = parent.iterateI7HSubZones(szLevel - level, &zCentroid, findSubZone, -1);
      }
      return ix;
   }

   DGGRSZone getSubZoneAtIndex(A5Zone parent, int relativeDepth, int64 index)
   {
      A5Zone subZone = nullZone;
      if(index >= 0 && index < RhombicIcosahedral7H::countSubZones(parent, relativeDepth))
      {
         if(index == 0)
            return RhombicIcosahedral7H::getFirstSubZone(parent, relativeDepth);
         else if(parent.level + relativeDepth <= 19)
         {
            Pointd centroid;
            parent.iterateI7HSubZones(relativeDepth, &centroid, findByIndex, index);
            subZone = A5Zone::fromCentroid(parent.level + relativeDepth, centroid);
         }
      }
      return subZone;
   }

   bool zoneHasSubZone(A5Zone hayStack, A5Zone needle)
   {
      bool result = false;
      int zLevel = hayStack.level, szLevel = needle.level;
      if(szLevel > zLevel)
      {
         Pointd v[6], c;
         int n, i;

         RhombicIcosahedral7H::getZoneCRSCentroid(needle, 0, c);
         n = needle.getVerticesDirections(v);

         for(i = 0; i < n; i++)
         {
            DGGRSZone tz;
            Pointd m;

            /*
            double dx = v[i].x, dy = v[i].y;
            Pointd vc;
            double dx = v[i].x - c.x;
            double dy = v[i].y - c.y;
            if(dx > 3 || dy > 3)
               dx -= 5, dy -= 5;
            else if(dx < -3 || dy <- 3)
               dx += 5, dy += 5;

            move5x6(vc, c, Sgn(dx) * 2E-11, Sgn(dy) * 2E-11, 1, null, null, true);

            dx = v[i].x - vc.x;
            dy = v[i].y - vc.y;

            if(dx > 3 || dy > 3)
               dx -= 5, dy -= 5;
            else if(dx < -3 || dy <- 3)
               dx += 5, dy += 5;

             move5x6(m, v[i], -dx * 0.01, -dy * 0.01, 1, null, null, false);
            */
            move5x6(m, c, v[i].x * 0.99, v[i].y * 0.99, 1, null, null, false);

            tz = RhombicIcosahedral7H::getZoneFromCRSCentroid(zLevel, 0, m);
            if(tz == hayStack)
            {
               result = true;
               break;
            }
         }
      }
      return result;
   }

   A5Zone getZoneFromWGS84Centroid(int level, const GeoPoint centroid)
   {
      if(level <= 19)
      {
         Pointd v;
         pj.forward(centroid, v);
         return A5Zone::fromCentroid(level, v);
      }
      return nullZone;
   }
#endif

   void getZoneCRSCentroid(A5Zone zone, CRS crs, Pointd centroid)
   {
      switch(crs)
      {
         case 0: case CRS { ogc, 153456 }:
            centroid = zone.centroid;
            break;
         case CRS { ogc, 1534 }:
         {
            Pointd c5x6 = zone.centroid;
            RI5x6Projection::toIcosahedronNet({c5x6.x, c5x6.y }, centroid);
            break;
         }
         case CRS { epsg, 4326 }:
         case CRS { ogc, 84 }:
         {
            GeoPoint geo;
            pj.inverse(zone.centroid, geo, false);
            centroid = crs == { ogc, 84 } ?
               { geo.lon, geo.lat } :
               { geo.lat, geo.lon };
            break;
         }
      }
   }

   void getZoneWGS84Centroid(A5Zone zone, GeoPoint centroid)
   {
      pj.inverse(zone.centroid, centroid, false); //zone.level & 1);
   }

#if 0
   void getZoneCRSExtent(A5Zone zone, CRS crs, CRSExtent extent)
   {
      switch(crs)
      {
         case 0: case CRS { ogc, 153456 }:
            extent = zone.ri5x6Extent;
            break;
         case CRS { ogc, 1534 }:
            getIcoNetExtentFromVertices(zone, extent);
            break;
         case CRS { epsg, 4326 }:
         case CRS { ogc, 84 }:
         {
            GeoExtent geo;
            RhombicIcosahedral7H::getZoneWGS84Extent(zone, geo);
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

   void getZoneWGS84Extent(A5Zone zone, GeoExtent extent)
   {
      int i;
      GeoPoint centroid;
      Radians minDLon = 99999, maxDLon = -99999;
      Array<GeoPoint> vertices = (Array<GeoPoint>)getRefinedVertices(zone, { epsg, 4326 }, 0, true);
      int nVertices = vertices ? vertices.count : 0;

      RhombicIcosahedral7H::getZoneWGS84Centroid(zone, centroid);

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
#endif

   int getZoneCRSVertices(A5Zone zone, CRS crs, Pointd * vertices)
   {
      uint count = zone.getVertices(zone.level, zone.quintant, zone.triCurve, zone.centroid, zone.nPoints, vertices), i;
      int j;

      for(j = 0; j < count; j++)
         canonicalize5x6(vertices[j], vertices[j]);

      switch(crs)
      {
         case 0: case CRS { ogc, 153456 }:
            break;
         case CRS { ogc, 1534 }:
         {
            for(i = 0; i < count; i++)
               RI5x6Projection::toIcosahedronNet({ vertices[i].x, vertices[i].y }, vertices[i]);
            break;
         }
         case CRS { ogc, 84 }:
         case CRS { epsg, 4326 }:
         {
            bool oddGrid = zone.level & 1; // REVIEW:
            for(i = 0; i < count; i++)
            {
               GeoPoint geo;
               pj.inverse(vertices[i], geo, oddGrid);
               vertices[i] = crs == { ogc, 84 } ? { geo.lon, geo.lat } : { geo.lat, geo.lon };
            }
            break;
         }
         default:
            count = 0;
      }
      return count;
   }

#if 0
   int getZoneWGS84Vertices(A5Zone zone, GeoPoint * vertices)
   {
      Pointd v5x6[6];
      uint count = zone.getVertices(zone.levelI49R, zone.rootRhombus, zone.subHex, zone.centroid, zone.nPoints, v5x6), i;
      bool oddGrid = zone.level & 1; // REVIEW:
      int j;

      for(j = 0; j < count; j++)
         canonicalize5x6(v5x6[j], v5x6[j]);

      for(i = 0; i < count; i++)
         pj.inverse(v5x6[i], vertices[i], oddGrid);
      return count;
   }
#endif

   Array<Pointd> getZoneRefinedCRSVertices(A5Zone zone, CRS crs, int edgeRefinement)
   {
      if(crs == CRS { ogc, 1534 } || crs == CRS { ogc, 153456 })
         return getIcoNetRefinedVertices(zone, edgeRefinement, crs == CRS { ogc, 1534 });
      else
         return getRefinedVertices(zone, crs, edgeRefinement, false);
   }

   Array<GeoPoint> getZoneRefinedWGS84Vertices(A5Zone zone, int edgeRefinement)
   {
      return (Array<GeoPoint>)getRefinedVertices(zone, { epsg, 4326 }, edgeRefinement, true);
   }

#if 0
   void getApproxWGS84Extent(A5Zone zone, GeoExtent extent)
   {
      A5::getZoneWGS84Extent(zone, extent);
   }
#endif

   // NOTE: getRefinedVertices() is currently only ever called with CRS84 or EPSG:4326
   private static Array<Pointd> getRefinedVertices(A5Zone zone, CRS crs, int edgeRefinement, bool useGeoPoint) // 0 edgeRefinement for 1-20 based on level
   {
      Array<Pointd> rVertices = null;
      bool crs84 = crs == CRS { ogc, 84 } || crs == CRS { epsg, 4326 };
      int level = zone.level;
      // * 1024 results in level 2 zones areas accurate to 0.01 km^2
      int nDivisions = edgeRefinement ? edgeRefinement :                         // ISEA / RTEA have strong distortion in some areas where refinements matter
         level < 3 ? 20 : level < 5 ? 15 : level < 8 ? 12 : 12; //level < 10 ? 8 : level < 11 ? 5 : level < 12 ? 2 : 1;
      Array<Pointd> r = zone.getBaseRefinedVertices(crs84, nDivisions);
      if(r && r.count)
      {
         int i;
         if(crs84)
         {
            GeoPoint centroid;
            bool wrap = true;
            bool oddGrid = true; //(level & 1); // REVIEW: ALways setting this to true fixes flipped South pole in BA-0-C
            Array<Pointd> ap = useGeoPoint ? (Array<Pointd>)Array<GeoPoint> { } : Array<Pointd> { };

            ap./*size*/minAllocSize = r.count;

            A5::getZoneWGS84Centroid(zone, centroid);

            // REVIEW: Should centroid ever be outside -Pi..Pi?
            if(centroid.lon < - Pi - 1E-9)
               centroid.lon += 2*Pi;

            if(centroid.lon > Pi + 1E-9)
               centroid.lon -= 2*Pi;

            for(i = 0; i < r.count; i++)
            {
               GeoPoint point;

               if(pj.inverse(r[i], point, oddGrid))
               {
                  if(wrap)
                  {
                     point.lon = wrapLonAt(-1, point.lon, centroid.lon - Degrees { 0.05 }) + centroid.lon - Degrees { 0.05 }; // REVIEW: wrapLonAt() doesn't add back centroid.lon ?

                     // REVIEW: Why isn't wrapLonAt() handling these cases?
                     if(oddGrid)
                     {
                        if(((double)point.lon - (double)centroid.lon) < -120)
                           point.lon += 180;
                        else if(((double)point.lon - (double)centroid.lon) > 120)
                           point.lon -= 180;
                     }
                  }

                  ap.Add(useGeoPoint ? { (Radians) point.lat, (Radians) point.lon } :
                     crs == { ogc, 84 } ? { point.lon, point.lat } : { point.lat, point.lon });
                  if(ap.count >= 2 &&
                     fabs(ap[ap.count-1].x - ap[ap.count-2].x) < 1E-11 &&
                     fabs(ap[ap.count-1].y - ap[ap.count-2].y) < 1E-11)
                     ap.size--; // We rely on both interruptions during interpolation, but they map to the same CRS84 point
                  if(ap.count >= 2 && i == r.count - 1 &&
                     fabs(ap[0].x - ap[ap.count-1].x) < 1E-11 &&
                     fabs(ap[0].y - ap[ap.count-1].y) < 1E-11)
                     ap.size--;
               }
#ifdef _DEBUG
               else
                  ; //PrintLn("WARNING: Failed to inverse project ", r[i]);
#endif
            }
            delete r;
            ap.minAllocSize = 0;
            rVertices = ap;
         }
         else
         {
            rVertices.minAllocSize = 0;
            rVertices = r;
         }
      }
      return rVertices;
   }

#if 0
   // Sub-zone Order
   Array<Pointd> getSubZoneCRSCentroids(A5Zone parent, CRS crs, int depth)
   {
      Array<Pointd> centroids = parent.getSubZoneCentroids(depth);
      if(centroids)
      {
         uint count = centroids.count, i;
         switch(crs)
         {
            case 0: case CRS { ogc, 153456 }: break;
            case CRS { ogc, 1534 }:
               for(i = 0; i < count; i++)
                  RI5x6Projection::toIcosahedronNet({ centroids[i].x, centroids[i].y }, centroids[i]);
               break;
            case CRS { epsg, 4326 }:
            case CRS { ogc, 84 }:
            {
               bool oddGrid = (parent.level + depth) & 1; // REVIEW:
               for(i = 0; i < count; i++)
               {
                  GeoPoint geo;
                  pj.inverse(centroids[i], geo, oddGrid);
                  centroids[i] = crs == { ogc, 84 } ? { geo.lon, geo.lat } : { geo.lat, geo.lon };
               }
               break;
            }
            default: delete centroids;
         }
      }
      return centroids;
   }

   Array<GeoPoint> getSubZoneWGS84Centroids(A5Zone parent, int depth)
   {
      Array<GeoPoint> geo = null;
      Array<Pointd> centroids = parent.getSubZoneCentroids(depth);
      if(centroids)
      {
         uint count = centroids.count;
         int i;
         bool oddGrid = (parent.level + depth) & 1; // REVIEW:

         geo = { size = count };
         for(i = 0; i < count; i++)
            pj.inverse(centroids[i], geo[i], oddGrid);
         delete centroids;
      }
      return geo;
   }
#endif

   static Array<DGGRSZone> listZones(int zoneLevel, const GeoExtent bbox)
   {
      Array<A5Zone> zones = null;
      AVLTree<A5Zone> tsZones { };
      //int level = 0;
      int quintant;
      // TODO: bool extentCheck = bbox != null && bbox.OnCompare(wholeWorld);

      if(zoneLevel == 0)
      {
         for(quintant = 0; quintant < 12; quintant++)
            tsZones.Add({ 0, quintant, 0 });
      }
      else if(zoneLevel == 1)
      {
         for(quintant = 0; quintant < 60; quintant++)
            tsZones.Add({ 1, quintant, 0 });
      }
      else if(zoneLevel == 2)
      {
         for(quintant = 0; quintant < 60; quintant ++)
         {
            int tri;

            for(tri = 0; tri < 4; tri++)
               tsZones.Add({ 2, quintant, tri });
         }
      }

      /*
      if(zoneLevel == 0 && extentCheck)
      {
         AVLTree<A5Zone> tmp { };

         for(z : tsZones)
         {
            A5Zone zone = (A5Zone)z;
            GeoExtent e;
            A5::getZoneWGS84Extent(zone, e);
            if(e.intersects(bbox))
               tmp.Add(zone);
         }
         delete tsZones;
         tsZones = tmp;
      }

      for(level = 1; level <= zoneLevel; level++)
      {
         AVLTree<A5Zone> tmp { };

         for(z : tsZones)
         {
            A5Zone zone = (A5Zone)z;
            A5Zone children[7];
            int n = 0; // TODO: zone.getChildren(children), i;

            for(i = 0; i < n; i++)
            {
               A5Zone c = children[i];
               if(extentCheck)
               {
                  GeoExtent e;
                  if(!tmp.Find(c))
                  {
                     A5::getZoneWGS84Extent(c, e);
                     if(!e.intersects(bbox))
                        continue;
                  }
                  else
                     continue;
               }
               tmp.Add(c);
            }
         }
         delete tsZones;
         tsZones = tmp;
      }
      */
      if(tsZones.count)
      {
         zones = Array<A5Zone> { minAllocSize = tsZones.count };
         for(t : tsZones)
         {
            A5Zone zone = t;
            zones[zones.count++] = zone;
         }
         zones.Sort(true);
      }

      delete tsZones;
      return (Array<DGGRSZone>)zones;
   }
}

enum A5Neighbor
{
   first,
   second,
   third,
   fourth,
   fifth
};

// Public for use in tests...
public class A5Zone : private DGGRSZone
{
public:
   uint level:5;
   uint quintant:8;
   uint64 triCurve:51;

   int OnCompare(A5Zone b)
   {
      if(this == b)
         return 0;
      else
      {
         uint l = level, bl = b.level;
         if(l < bl) return -1;
         else if(l > bl) return 1;
         else
            return this < b ? -1 : 1;
      }
   }

private:

   property int nPoints
   {
      get
      {
         return level == 1 ? 3 : 5;
      }
   }

   A5Zone ::fromZoneID(const String zoneID)
   {
      A5Zone result = nullZone;
      int level;
      uint64 tri = 0;
      uint quintant;

      if(sscanf(zoneID, __runtimePlatform == win32 ? "%d-%d-%I64X" : "%d-%d-%lld",
         &level, &quintant, &tri) == 3 &&
         quintant >= 0 && quintant < 12 && level >= 0 && level <= 20 && tri >= 0)
         result = { level, quintant, tri };
      return result;
   }

#if 0
   bool containsPoint(const Pointd v)
   {
      bool result = false;
      int i;
      Pointd v5x6[24];
      int n = getBaseRefinedVerticesNoAlloc(false, 1, v5x6);
      CRSExtent bbox { };
      CRSExtent pBBOX { };

      if(!n)
         return false;

      pBBOX.tl.x = (int) (v.x + 1E-11);
      pBBOX.tl.y = (int) (v.y + 1E-11);
      pBBOX.br.x = pBBOX.tl.x + 1;
      pBBOX.br.y = pBBOX.tl.y + 1;

      bbox.br = { -100, -100 };
      bbox.tl = {  100,  100 };
      for(i = 0; i < n; i++)
      {
         double x = v5x6[i].x, y = v5x6[i].y;

         if(x > bbox.br.x) bbox.br.x = x;
         if(y > bbox.br.y) bbox.br.y = y;
         if(x < bbox.tl.x) bbox.tl.x = x;
         if(y < bbox.tl.y) bbox.tl.y = y;
      }

      if(v.x - bbox.br.x > 3 && v.y - bbox.br.y > 3 &&
         v.x - bbox.tl.x > 3 && v.y - bbox.tl.y > 3)
      {
         bbox.tl.x += 5;
         bbox.tl.y += 5;
         bbox.br.x += 5;
         bbox.br.y += 5;
      }

      if(v.x - bbox.br.x <-3 && v.y - bbox.br.y <-3 &&
         v.x - bbox.tl.x <-3 && v.y - bbox.tl.y <-3)
      {
         bbox.tl.x -= 5;
         bbox.tl.y -= 5;
         bbox.br.x -= 5;
         bbox.br.y -= 5;
      }

#if 0 //def _DEBUG
      PrintLn("Zone  BBOX: ", bbox.tl.x, ", ", bbox.tl.y, " - ", bbox.br.x, ", ", bbox.br.y);
      PrintLn("Point     : ", v.x, ", ", v.y);
#endif

      if(v.x < bbox.tl.x - 1E-11 ||
         v.y < bbox.tl.y - 1E-11 ||
         v.x > bbox.br.x + 1E-11 ||
         v.y > bbox.br.y + 1E-11)
      {
#if 0 //def _DEBUG
         PrintLn("  Skipping this zone");
#endif
         return false;
      }

#if 0 // def _DEBUG
      PrintLn("  Considering this zone");
      PrintLn("Point BBOX: ", pBBOX.tl.x, ", ", pBBOX.tl.y, " - ", pBBOX.br.x, ", ", pBBOX.br.y);
#endif

      for(i = 0; i < n; i++)
      {
         int j = i < n-1 ? i + 1 : 0;
         Pointd a = v5x6[i], b = v5x6[j];
         double sa;
         Pointd aa = a, bb = b;

         if(fabs(aa.x - v.x) > 3 &&
            fabs(aa.y - v.y) > 3)
         {
            if(aa.x > 3 && aa.y > 3)
               aa.x -= 5, aa.y -= 5;
            else
               aa.x += 5, aa.y += 5;
         }
         if(fabs(bb.x - v.x) > 3 &&
            fabs(bb.y - v.y) > 3)
         {
            if(bb.x > 3 && bb.y > 3)
               bb.x -= 5, bb.y -= 5;
            else
               bb.x += 5, bb.y += 5;
         }
#if 0 //def _DEBUG
         PrintLn("  Segment: ", aa.x, ", ", aa.y, " - ", bb.x, ", ", bb.y);
#endif

         if((aa.x - 1E-11 < pBBOX.tl.x ||
             aa.y - 1E-11 < pBBOX.tl.y ||
             aa.x + 1E-11 > pBBOX.br.x ||
             aa.y + 1E-11 > pBBOX.br.y) &&
            (bb.x - 1E-11 < pBBOX.tl.x ||
             bb.y - 1E-11 < pBBOX.tl.y ||
             bb.x + 1E-11 > pBBOX.br.x ||
             bb.y + 1E-11 > pBBOX.br.y))
         {
#if 0 //def _DEBUG
            PrintLn("  Skipping this segment (B point)");
#endif
            continue;
         }

         sa = pointLineSide(v.x, v.y, aa, bb);

         if(sa < 0)
         {
#if 0 // def _DEBUG
            PrintLn("  We're outside this segment!");
#endif
            result = false;
            break;
         }
         else
            result = true; // At least one edge segment should be checked
                           // (BBOX check only gives false positive on e.g., B0-0-C)
      }
#if 0 //def _DEBUG
      if(result)
         PrintLn("  Zone Contains point!");
#endif
      return result;
   }

#endif

   // This function generates the proposed I7H DGGRS Zone ID string
   // in the form {LevelChar}{RootPentagon}-{HierarchicalIndexFromPentagon}
   void getZoneID(String zoneID)
   {
      if(this == nullZone)
         sprintf(zoneID, "(null)");
      else
         PrintBuf(zoneID, 256, level, "-", quintant, "-", triCurve);
   }

#if 0
   property A5Zone parent0
   {
      // ivea7h info C2-8-A
      // ivea7h zone 28.6888849753227,-69.0934671650866 1
      get
      {
         A5Zone key = nullZone;
         if(this != nullZone)
         {
            int l49r = levelI49R;
            if(l49r || subHex)
            {
               if(subHex)
                  key = { l49r, rootRhombus, rhombusIX, 0 };
               else
                  key = A5Zone::fromEvenLevelPrimaryChild(this);
            }
         }
         return key;
      }
   }

   int getNeighbors(A5Zone neighbors[6], I7HNeighbor i7hNB[6])
   {
      int nLevel = this.level;
      int numNeighbors = 0;
      Pointd c, cVerts[6];
      int nv = 0;

      if(nLevel <= 19)
      {
         // This is conceptually the centroid child, but allows representation of Level 20 which A5Zone cannot represent
         int root, cLevel49R, cSH;
         c = centroid;
         if(nLevel & 1)
         {
            int64 row, col;
            root = getOddLevelCentroidChildRootRowCol(&row, &col, null);
            cLevel49R = levelI49R + 1;
            cSH = 0;
         }
         else
         {
            root = rootRhombus;
            cLevel49R = levelI49R;
            cSH = 1;
         }
         nv = A5Zone::getVertices(cLevel49R, root, cSH, c, nPoints, cVerts);
      }

      if(nv)
      {
         int i;
         int cx = Min(4, (int)(c.x + 1E-11)), cy = Min(5, (int)(c.y + 1E-11));  // Coordinate of root rhombus
         bool north = c.x - c.y - 1E-11 > 0;

         for(i = 0; i < nv; i++)
         {
            Pointd v;
            Pointd cc = c;
            double dx = cVerts[i].x - cc.x;
            double dy = cVerts[i].y - cc.y;

            if(dx > 3 && dy > 3)
            {
               dx = cVerts[i].x - 5 - cc.x;
               dy = cVerts[i].y - 5 - cc.y;
            }
            else if(dx < -3 && dy < -3)
            {
               dx = cVerts[i].x + 5 - cc.x;
               dy = cVerts[i].y + 5 - cc.y;
            }

            if(fabs(dx) < 1 && fabs(dy) < 1)
            {
               // We need to avoid computing dx and dy across interuptions
               if(( north && fabs(c.y - cy) < 1E-11) ||
                  (!north && fabs(c.x - cx) < 1E-11))
               {
                  double x, y;
                  Pointd ci;
                  cross5x6Interruption(c, ci, !north, true);

                  x = cVerts[i].x - ci.x;
                  y = cVerts[i].y - ci.y;

                  if(x > 3 && dy > 3)
                  {
                     x = cVerts[i].x - 5 - ci.x;
                     y = cVerts[i].y - 5 - ci.y;
                  }
                  else if(x < -3 && y < -3)
                  {
                     x = cVerts[i].x + 5 - ci.x;
                     y = cVerts[i].y + 5 - ci.y;
                  }

                  if(fabs(x) < fabs(dx) && fabs(y) < fabs(dy))
                  {
                     cc = ci;
                     dx = x;
                     dy = y;
                  }
               }
            }

            move5x6Vertex2(v, cc, dx * 3, dy * 3, false);

            canonicalize5x6(v, v);
            if(i7hNB) i7hNB[numNeighbors] = (I7HNeighbor)i;
            neighbors[numNeighbors++] = fromCentroid(nLevel, v);
#if 0 //def _DEBUG
            if(neighbors[numNeighbors-1] == nullZone)
               fromCentroid(nLevel, v);
#endif
         }
      }
      return numNeighbors;
   }

   int getContainingGrandParents(A5Zone cgParents[2])
   {
      int n = 0;
      int level = this.level;
      if(level >= 2)
      {
         if(isCentroidChild)
            // Scenario A: centroid child
            cgParents[0] = parent0.parent0, n = 1;
         else
         {
            A5Zone parents[2], gParentA, gParentB;

            getParents(parents);
            gParentA = parents[0].parent0;
            gParentB = parents[1].parent0;

            // Scenario B: both parents have same primary parent -- containing grandparent is that primary grandparent
            if(gParentA == gParentB)
               cgParents[0] = gParentA, n = 1;
            else
            {
               // REVIEW:
               // Parents have different primary parents
               Pointd c = centroid, cA = gParentA.centroid, cB = gParentB.centroid;
               A5Zone z1 = A5Zone::fromCentroid(level - 2, { c.x + 1E-9 * Sgn(cA.x - c.x), c.x + 1E-9 * Sgn(cA.y - c.y) });
               A5Zone z2 = A5Zone::fromCentroid(level - 2, { c.x + 1E-9 * Sgn(cB.x - c.x), c.x + 1E-9 * Sgn(cB.y - c.y) });

               // Scenario C: whole zone inside one of these (this sub-zone of only one grandparent): one containing grandparent
               if(z1 == z2)
                  cgParents[0] = z1, n = 1;

               // Scenario D: zone on the edge of both of these (this sub-zone of both): two containing grandparents
               else
                  cgParents[0] = gParentA, cgParents[1] = gParentB, n = 2;
            }
         }
      }
      return n;
   }

   int getParents(A5Zone parents[2])
   {
      A5Zone parent0 = this.parent0;

      parents[0] = parent0;
      if(parent0 == nullZone)
         return 0;
      else
      {
         A5Zone cChild = parent0.centroidChild;
         if(cChild == this)
            return 1;
         else
         {
            Pointd c = centroid;
            int i;
            Pointd vertices[6];

            int n = getVertices(levelI49R, rootRhombus, subHex, c, nPoints, vertices);
            int pLevel = parent0.level;

            for(i = 0; i < n; i++)
            {
               Pointd acc = c;
               double dx = vertices[i].x - acc.x;
               double dy = vertices[i].y - acc.y;
               A5Zone z;
               Pointd v;

               if(dx > 3 || dy > 3)
               {
                  dx = vertices[i].x - 5 - acc.x;
                  dy = vertices[i].y - 5 - acc.y;
               }
               else if(dx < -3 || dy < -3)
               {
                  dx = vertices[i].x + 5 - acc.x;
                  dy = vertices[i].y + 5 - acc.y;
               }

               if(fabs(dx) < 1 && fabs(dy) < 1)
               {
                  bool north = acc.x - acc.y - 1E-11 > 0;
                  int cy = (int)(acc.y + 1E-11);
                  int cx = (int)(acc.x + 1E-11);

                  // We need to avoid computing dx and dy across interuptions
                  if(( north && fabs(acc.y - cy) < 1E-11) ||
                     (!north && fabs(acc.x - cx) < 1E-11))
                  {
                     double x, y;
                     Pointd ci;
                     cross5x6Interruption(c, ci, !north, true);

                     x = vertices[i].x - ci.x;
                     y = vertices[i].y - ci.y;

                     if(x > 3 && dy > 3)
                     {
                        x = vertices[i].x - 5 - ci.x;
                        y = vertices[i].y - 5 - ci.y;
                     }
                     else if(x < -3 && y < -3)
                     {
                        x = vertices[i].x + 5 - ci.x;
                        y = vertices[i].y + 5 - ci.y;
                     }

                     if(fabs(x) < fabs(dx) && fabs(y) < fabs(dy))
                     {
                        acc = ci;
                        dx = x;
                        dy = y;
                     }
                  }
               }

               move5x6Vertex2(v, acc, .99 * dx, .99 * dy, false);

               z = fromCentroid(pLevel, v);
               if(z != nullZone && z != parent0)
               {
                  parents[1] = z;
                  return 2;
               }
            }
#ifdef _DEBUG
            {
               char zID[128];
               getZoneID(zID);
               // PrintLn((uint64)this);
               PrintLn("ERROR: Failed to determine second parent for ", zID);
            }
#endif
            return 1;
         }
      }
   }

   private static inline double ::pointLineSide(double x, double y, Pointd a, Pointd b)
   {
      double dx = b.x - a.x, dy = b.y - a.y;
      double A = dy, B = -dx, C = a.y * dx - dy * a.x;
      return A * x + B * y + C;
   }

   A5Zone ::calcCandidateParent(int l49r, int root, int64 row, int64 col, int addCol, int addRow)
   {
      uint64 p = POW7(l49r);
      bool south = (root & 1);
      uint64 cix;

      col += addCol;
      row += addRow;

      // REVIEW: REVIEW / Share this logic with getPrimaryChildren(), possibly centroidChild?
      if(col == (int64)p && row < (int64)p && !south) // Cross at top-dent to the right
      {
         col = p-row;
         row = 0;
         root += 2;
      }
      else if(row == (int64)p && col < (int64)p && south) // Cross at bottom-dent to the right
      {
         row = p-col;
         col = 0;
         root += 2;
      }
      else
      {
         if(row < 0 && col < 0)
            row += p, col += p, root -= 2;
         else if(row < 0)
            row += p, root -= 1;
         else if(col < 0)
            col += p, root -= 1;
         else if(col >= (int64)p && row >= (int64)p)
            row -= p, col -= p, root += 2;
         else if(row >= (int64)p)
            row -= p, root += 1;
         else if(col >= (int64)p)
            col -= p, root += 1;
      }
      if(root < 0) root += 10;
      else if(root > 9) root -= 10;

      south = (root & 1);

      if(!south && row == 0 && col == p)
         root = 0xA, cix = 0;
      else if(south && row == p && col == 0)
         root = 0xB, cix = 0;
      else
      {
         if(row < 0 || row >= p ||
            col < 0 || col >= p)
         {
#ifdef _DEBUG
            // PrintLn("WARNING: Invalid zone calculated");
#endif
            return nullZone;
         }
         else
            cix = row * p + col;
      }

      // REVIEW: Polar zones considerations?
      return A5Zone { l49r, root, cix, 0 };
   }

   A5Zone ::fromCentroid(uint level, const Pointd centroid) // in RI5x6
   {
      int l49r = level / 2;
      Pointd c = centroid;
      uint64 p = POW7(l49r);
      double oop =  1.0 / p;

      // bool isNorthPole = false, isSouthPole = false;
      if(fabs(c.x - c.y - 1) < 1E-10)
         ;//isNorthPole = true;
      else if(fabs(c.y - c.x - 2) < 1E-10)
         ;//isSouthPole = true;
      else if(c.y < -1E-11 && c.x > -1E-11)
         c.x -= c.y, c.y = 0;
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
         move5x6Vertex2(c, { 5, 5 }, c.x, c.y, false);

      if(c.x > 5 - 1E-11 && c.y > 5 - 1E-11 &&  // This handles bottom right wrap e.g., A9-0E and A9-0-F
         c.x + c.y > 5.0 + 5.0 - oop - 1E-11)
         c.x -= 5, c.y -= 5;

      // Vancouver:     49.2827,-123.1207 2  -- C0-17-A
      // Abbotsford:    49.0581,-122.2798 2  -- C0-1F-A
      // Ottawa:        45.4963,-75.7016 2   -- C0-29-A
      // Auckland:      -36.8543,174.7392 2  -- C9-1C-A
      // Whakatāne:     -37.9583,176.98428 2 -- C9-23-A

      {
         int cx = Min(4, (int)(c.x + 1E-11)), cy = Min(5, (int)(c.y + 1E-11));  // Coordinate of root rhombus
         uint root = cx + cy;
         double x = c.x - cx, y = c.y - cy;
         int64 col = Max(0, (int64)(x * p + 0.5));
         int64 row = Max(0, (int64)(y * p + 0.5));
         double dx = x * p + 0.5 - col;
         double dy = y * p + 0.5 - row;
         uint64 cix;
         bool southRhombus = (root & 1);
         // Review where this should be used...
         bool south = c.y - c.x - 1E-11 > 1; // Not counting pentagons as south or north
         bool north = c.x - c.y - 1E-11 > 0;
         bool northPole = north && fabs(c.x - c.y - 1.0) < 1E-11;
         bool southPole = south && fabs(c.y - c.x - 2.0) < 1E-11;

         if(level & 1)
         {
            // Odd level -- currently using a rather brute-force approach
            A5Zone zone = nullZone;
            if(northPole)
               zone = { l49r, 0xA, 0, 1 };
            else if(southPole)
               zone = { l49r, 0xB, 0, 1 };
            else
            {
               A5Zone candidateParents[7];
               int i;

               if(north && row == 0 && col == p)
                  candidateParents[0] = { l49r, 0xA, 0, 0 };
               else if(south && row == p && col == 0)
                  candidateParents[0] = { l49r, 0xB, 0, 0 };
               else
               {
                  // candidateParents[0] = { l49r, 2 + root * (p * p) + row * p + col, 0 };

                  candidateParents[0] = calcCandidateParent(l49r, root, row, col, 0, 0);
               }

#if 0 //def _DEBUG
               {
                  char pID[128];
                  candidateParents[0].getZoneID(pID);

                  // PrintLn("Main candidate parent: ", pID);
               }
#endif

               // Top (2 potential children including 1 secondary child of prime candidate)
               candidateParents[1] = calcCandidateParent(l49r, root, row, col, 0, -1);
               // Bottom (2 potential children including 1 secondary child of prime candidate)
               candidateParents[2] = calcCandidateParent(l49r, root, row, col, 0, 1);
               // Right (2 potential children including 1 secondary child of prime candidate)
               candidateParents[3] = calcCandidateParent(l49r, root, row, col, 1, 0);
               // Left (2 potential children including 1 secondary child of prime candidate)
               candidateParents[4] = calcCandidateParent(l49r, root, row, col, -1, 0);
               // Top-Left (1 potential child including 1 secondary child of prime candidate)
               candidateParents[5] = calcCandidateParent(l49r, root, row, col, -1, -1);
               // Bottom-Right (1 potential child including 1 secondary child of prime candidate)
               candidateParents[6] = calcCandidateParent(l49r, root, row, col, 1, 1);

               // int numMatches = 0;
               for(i = 0; i < 7; i++)
               {
                  A5Zone children[7];
                  int n, j;
#if 0 //def _DEBUG
                  char pID[128];
                  candidateParents[i].getZoneID(pID);
                  if(candidateParents[i] != fromZoneID(pID))
                  {
                     PrintLn("ERROR: Invalid candidate parent zone: ", pID);
                     candidateParents[1] = calcCandidateParent(l49r, root, row, col, 0, -1);
                  }
                  // PrintLn("Generating primary children for ", pID);
#endif

                  n = candidateParents[i].getPrimaryChildren(children);

                  for(j = 0; j < n; j++)
                  {
#if 0 //def _DEBUG
                     char zID[128];
                     children[j].getZoneID(zID);
                     if(children[j] != fromZoneID(zID))
                     {
                        PrintLn("ERROR: Invalid zone generated: ", zID);

                        children[j].getZoneID(zID);
                        fromZoneID(zID);

                        candidateParents[i].getPrimaryChildren(children);
                     }

                     // PrintLn("Testing whether child ", zID, " contains point ", centroid.x, ", ", centroid.y);
#endif
                     if(children[j].containsPoint(c))
                     {
                        zone = children[j];
                        // numMatches ++;
                        break;
                     }
                  }
                  if(zone != nullZone)
                     break;
               }
               // PrintLn("matches: ", numMatches);
            }

#if 0 //def _DEBUG
            if(zone == nullZone)
               PrintLn("WARNING: Unable to resolve zone for ", centroid.x, ", ", centroid.y);

            if(zone != nullZone)
            {
               char id[256];
               A5Zone z;
               zone.getZoneID(id);
               z = fromZoneID(id);
               if(z != zone)
                  PrintLn("ERROR: Invalid zone returned");
            }
#endif
            return zone;
         }
         else
         {
            if(northPole)
               return { l49r, 0xA, 0, 0 };
            else if(southPole)
               return { l49r, 0xB, 0, 0 };

            // Even level
            if(dx > 1 - dy)
            {
               // Bottom-Right diagonal
               if(dx > dy)
               {
                  // Top-Right diagonal (right triangle)
                  if(pointLineSide(dx, dy, { 1.0, 0.5 }, { 5/6.0, 1/6.0 }) < 0)
                     col++;
               }
               else
               {
                  // Bottom-Left diagonal (bottom triangle)
                  if(pointLineSide(dx, dy, { 1/6.0, 5/6.0 }, { 0.5, 1.0 }) < 0)
                     row++;
               }
            }
            else
            {
               // Top-Left diagonal
               if(dx > dy)
               {
                  // Top-Right diagonal (top triangle)
                  if(pointLineSide(dx, dy, { 5/6.0, 1/6.0 }, { 0.5, 0.0 }) < 0)
                     row--;
               }
               else
               {
                  // Bottom-Left diagonal (left triangle)
                  if(pointLineSide(dx, dy, { 0.0, 0.5 }, { 1/6.0, 5/6.0 }) < 0)
                     col--;
               }
            }

            if(north && col == p && row == 0)
               root = 0xA, cix = 0;
            else if(south && col == 0 && row == p)
               root = 0xB, cix = 0;
            else
            {
               // REVIEW: REVIEW / Share this logic with getPrimaryChildren(), possibly centroidChild?
               if(col == (int64)p && row < (int64)p && !southRhombus) // Cross at top-dent to the right
               {
                  col = p-row;
                  row = 0;
                  root += 2;
               }
               else if(row == (int64)p && col < (int64)p && southRhombus) // Cross at bottom-dent to the right
               {
                  row = p-col;
                  col = 0;
                  root += 2;
               }
               else
               {
                  if(row < 0 && col < 0)
                     row += p, col += p, root -= 2;
                  else if(row < 0)
                     row += p, root -= 1;
                  else if(col < 0)
                     col += p, root -= 1;
                  else if(col >= (int64)p && row >= (int64)p)
                     row -= p, col -= p, root += 2;
                  else if(row >= (int64)p)
                     row -= p, root += 1;
                  else if(col >= (int64)p)
                     col -= p, root += 1;
               }
               if(root < 0) root += 10;
               else if(root > 9) root -= 10;

               if(row < 0 || row >= p ||
                  col < 0 || col >= p)
               {
      #ifdef _DEBUG
                  // PrintLn("WARNING: Invalid zone calculated");
      #endif
                  return nullZone;
               }
               else
                  cix = row * p + col;
            }
            return A5Zone { l49r, root, cix, 0 };
         }
      }
   }

   A5Zone ::fromEvenLevelPrimaryChild(A5Zone child)
   {
      int l49r = child.levelI49R - 1;
      Pointd c = child.centroid;
      uint64 p = POW7(l49r);
      double oop =  1.0 / p;

      if(child.subHex || l49r < 0) return nullZone; // Invalid usage

      // bool isNorthPole = false, isSouthPole = false;
      if(fabs(c.x - c.y - 1) < 1E-10)
         ;//isNorthPole = true;
      else if(fabs(c.y - c.x - 2) < 1E-10)
         ;//isSouthPole = true;
      else if(c.y < -1E-11 && c.x > -1E-11)
         c.x -= c.y, c.y = 0;
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
         move5x6Vertex2(c, { 5, 5 }, c.x, c.y, false);

      if(c.x > 5 - 1E-11 && c.y > 5 - 1E-11 &&  // This handles bottom right wrap e.g., A9-0E and A9-0-F
         c.x + c.y > 5.0 + 5.0 - oop - 1E-11)
         c.x -= 5, c.y -= 5;

      {
         int cx = Min(4, (int)(c.x + 1E-11)), cy = Min(5, (int)(c.y + 1E-11));  // Coordinate of root rhombus
         uint root = cx + cy;
         double x = c.x - cx, y = c.y - cy;
         int64 col = Max(0, (int64)(x * p + 0.5));
         int64 row = Max(0, (int64)(y * p + 0.5));
         bool south = c.y - c.x - 1E-11 > 1; // Not counting pentagons as south or north
         bool north = c.x - c.y - 1E-11 > 0;
         bool northPole = north && fabs(c.x - c.y - 1.0) < 1E-11;
         bool southPole = south && fabs(c.y - c.x - 2.0) < 1E-11;
         // Odd level -- currently using a rather brute-force approach
         A5Zone zone = nullZone;
         if(northPole)
            zone = { l49r, 0xA, 0, 1 };
         else if(southPole)
            zone = { l49r, 0xB, 0, 1 };
         else
         {
            int i;

            for(i = 0; i < 7; i++)
            {
               A5Zone candidateParent, children[7];
               int n, j;

               switch(i)
               {
                  // Prime candidate
                  case 0:
                     if(north && row == 0 && col == p)
                        candidateParent = { l49r, 0xA, 0, 0 };
                     else if(south && row == p && col == 0)
                        candidateParent = { l49r, 0xB, 0, 0 };
                     else
                        candidateParent = calcCandidateParent(l49r, root, row, col, 0, 0);
                     break;
                  // Top (2 potential children including 1 secondary child of prime candidate)
                  case 1: candidateParent = calcCandidateParent(l49r, root, row, col, 0, -1); break;
                  // Bottom (2 potential children including 1 secondary child of prime candidate)
                  case 2: candidateParent = calcCandidateParent(l49r, root, row, col, 0, 1); break;
                  // Right (2 potential children including 1 secondary child of prime candidate)
                  case 3: candidateParent = calcCandidateParent(l49r, root, row, col, 1, 0); break;
                  // Left (2 potential children including 1 secondary child of prime candidate)
                  case 4: candidateParent = calcCandidateParent(l49r, root, row, col, -1, 0); break;
                  // Top-Left (1 potential child including 1 secondary child of prime candidate)
                  case 5: candidateParent = calcCandidateParent(l49r, root, row, col, -1, -1); break;
                  // Bottom-Right (1 potential child including 1 secondary child of prime candidate)
                  case 6: candidateParent = calcCandidateParent(l49r, root, row, col, 1, 1); break;
               }

               n = candidateParent.getPrimaryChildren(children);

               for(j = 0; j < n; j++)
               {
                  A5Zone grandChildren[7];
                  int ngc = children[j].getPrimaryChildren(grandChildren), k;

                  for(k = 0; k < ngc; k++)
                     if(grandChildren[k] == child)
                     {
                        zone = children[j];
                        break;
                     }
                  if(zone != nullZone)
                     break;
               }
               if(zone != nullZone)
                  break;
            }
         }
         return zone;
      }
   }
#endif

   int ::getVertices(uint level, uint quintant, uint64 triCurve, const Pointd centroid, int nPoints, Pointd * vertices)
   {
      Pointd c = centroid;
      uint count = 0;

      if(level == 0)
      {
         double A = 1 / 3.0;
         double B = 2 / 3.0;

         if(c.y > 6 + 1E-9 || c.x > 5 + 1E-9)
            c.x -= 5, c.y -= 5;
         else if(c.x < 0)
            c.x += 5, c.y += 5;

         if(quintant == 0xA) // North Pole
         {
            Pointd b { 1 - A, 0 + A };
            vertices[count++] = { b.x + 0, b.y + 0 };
            vertices[count++] = { b.x + 1, b.y + 1 };
            vertices[count++] = { b.x + 2, b.y + 2 };
            vertices[count++] = { b.x + 3, b.y + 3 };
            vertices[count++] = { b.x + 4, b.y + 4 };
         }
         else if(quintant == 0xB) // South Pole
         {
            Pointd b { 4 + A, 6 - A };
            vertices[count++] = { b.x - 0, b.y - 0 };
            vertices[count++] = { b.x - 1, b.y - 1 };
            vertices[count++] = { b.x - 2, b.y - 2 };
            vertices[count++] = { b.x - 3, b.y - 3 };
            vertices[count++] = { b.x - 4, b.y - 4 };
         }
         else
         {
            Pointd v[6] =
            {
               v[0] = { - A, - B },
               v[1] = { - B, - A },
               v[2] = { - A, + A },
               v[3] = { + A, + B },
               v[4] = { + B, + A },
               v[5] = { + A, - A }
            };
            count = addNonPolarBaseVertices(c, nPoints, v, vertices);
         }
      }
      else if(level == 1)
      {
         A5Zone root { 0, quintant / 5, 0 };
         Pointd rVerts[6];
         Pointd rc = root.centroid;
         int tri = quintant % 5;
         /*int nr = */root.getVertices(0, root.quintant, 0, rc, 5, rVerts);

         vertices[count++] = rc;
         vertices[count++] = rVerts[tri];
         vertices[count++] = rVerts[(tri + 1) % 5];
      }
      // TODO: level > 1
      return count;
   }

   private static inline void ::rotate5x6Offset(Pointd r, double dx, double dy, bool clockwise)
   {
      if(clockwise)
      {
         // 60 degrees clockwise rotation
         r.x = dx - dy;
         r.y = dx;
      }
      else
      {
         // 60 degrees counter-clockwise rotation
         r.x = dy;
         r.y = dy - dx;
      }
   }

   uint ::addNonPolarBaseVertices(Pointd c, int nPoints, const Pointd * v, Pointd * vertices)
   {
      int start = 0, prev, i;
      Pointd point, dir;
      uint count = 0;

      // Start with a point outside interruptions
      for(i = 0; i < 6; i++)
      {
         Pointd t { c.x + v[i].x, c.y + v[i].y };
         int tx = (int)floor(t.x + 1E-11);
         if(!(t.y - tx > 2 || t.y < tx))
         {
            start = i;
            break;
         }
      }

      point = { c.x + v[start].x, c.y + v[start].y };
      prev = (start + 5) % 6;
      dir = { point.x - (c.x + v[prev].x), point.y - (c.y + v[prev].y) };

      //vertices[count++] = point;

      for(i = start + 0; i < start + nPoints; i++)
      {
         bool north;
         Pointd i1, i2, n, p = point;

         rotate5x6Offset(dir, dir.x, dir.y, false);
         n = { point.x + dir.x, point.y + dir.y };

         if(p.x > 5 && p.y > 5)
            p.x -= 5, p.y -= 5;
         if(p.x < 0 || p.y < 0)
            p.x += 5, p.y += 5;

         if(crosses5x6InterruptionV2(p, dir.x, dir.y, i1, i2, &north))
         {
            bool crossingLeft;
            Pointd d;

            if(point.x - p.x > 4)
            {
               i1.x += 5, i1.y += 5;
               i2.x += 5, i2.y += 5;
            }
            if(p.x - point.x > 4)
            {
               i1.x -= 5, i1.y -= 5;
               i2.x -= 5, i2.y -= 5;
            }
            if(i2.y - i1.y > 4)
               i2.x -= 5, i2.y -= 5;
            if(i1.y - i2.y > 4)
               i2.x += 5, i2.y += 5;

            crossingLeft = north ? i2.x < i1.x : i2.x > i1.x;

            rotate5x6Offset(d, dir.x - (i1.x - point.x), dir.y - (i1.y - point.y), !crossingLeft);
            n = { i2.x + d.x, i2.y + d.y };
            rotate5x6Offset(dir, dir.x, dir.y, !crossingLeft);

            vertices[count++] = point;
         }
         else if(i < start + nPoints)
            vertices[count++] = point;
         point = n;
      }
      return count;
   }

   void addNonPolarVerticesRefined(Pointd c, const Pointd * v, Array<Pointd> vertices, bool crs84, int nDivisions, int count)
   {
      int start = 0, prev, i;
      Pointd point, dir;
      int nPoints = this.nPoints;

      // Start with a point outside interruptions
      for(i = 0; i < count /*6*/; i++)
      {
         Pointd t { c.x + v[i].x, c.y + v[i].y };
         int tx = (int)floor(t.x + 1E-11);
         if(!(t.y - tx > 2 || t.y < tx))
         {
            start = i;
            break;
         }
      }

      point = { c.x + v[start].x, c.y + v[start].y };
      prev = (start + 5) % 6;
      dir = { point.x - (c.x + v[prev].x), point.y - (c.y + v[prev].y) };
      // REVIEW: Is the first point is added twice?
      for(i = start + 1; i <= start + nPoints; i++)
      {
         bool north;
         Pointd i1, i2, n, p = point;

         rotate5x6Offset(dir, dir.x, dir.y, false);
         n = { point.x + dir.x, point.y + dir.y };

         if(p.x > 5 && p.y > 5)
            p.x -= 5, p.y -= 5;
         if(p.x < 0 || p.y < 0)
            p.x += 5, p.y += 5;

         if(crosses5x6InterruptionV2(p, dir.x, dir.y, i1, i2, &north))
         {
            bool crossingLeft;
            Pointd d;

            if(point.x - p.x > 4)
            {
               i1.x += 5, i1.y += 5;
               i2.x += 5, i2.y += 5;
            }
            if(p.x - point.x > 4)
            {
               i1.x -= 5, i1.y -= 5;
               i2.x -= 5, i2.y -= 5;
            }
            if(i2.y - i1.y > 4)
               i2.x -= 5, i2.y -= 5;
            if(i1.y - i2.y > 4)
               i2.x += 5, i2.y += 5;

            crossingLeft = north ? i2.x < i1.x : i2.x > i1.x;
            rotate5x6Offset(d, dir.x - (i1.x - point.x), dir.y - (i1.y - point.y), !crossingLeft);
            n = { i2.x + d.x, i2.y + d.y };
            rotate5x6Offset(dir, dir.x, dir.y, !crossingLeft);

            addIntermediatePoints(vertices, point, n, nDivisions, i1, i2, crs84);
         }
         else
         {
            if(!nDivisions)
               vertices.Add(point);
            else
               addIntermediatePoints(vertices, point, n, nDivisions, null, null, crs84);
         }
         point = n;
      }
   }

   void ::setupNBTri(const Pointd * qv, const Pointd * mid,
      Pointd * qq, Pointd * qm, int a, int b, int s)
   {
      int c = (b + 2) % 3;
      qq[s] = qv[b];
      qq[(s + 1) % 3].x = mid[b].x - (qv[c].x - mid[b].x);
      qq[(s + 1) % 3].y = mid[b].y - (qv[c].y - mid[b].y);
      qq[(s + 2) % 3] = qv[a];

      mid5x6(qm[0], qq[0], qq[1]);
      mid5x6(qm[1], qq[1], qq[2]);
      mid5x6(qm[2], qq[2], qq[0]);
   }

   void ::getTriPgonVertex(Pointd v,
      const Pointd * tv/*[3]*/, const Pointd * mid /*[3]*/, const Pointd qc, int ix)
   {
      switch(ix)
      {
         case 0:
            v = {
               qc.x + (mid[2].x - qc.x) * 0.175,
               qc.y + (mid[2].y - qc.y) * 0.175 };
            v = {
               v.x + (mid[1].x - v.x) * 0.065,
               v.y + (mid[1].y - v.y) * 0.065 };
            break;
         case 1:
            computeTriCentroid(v, mid[2], tv[0], mid[0]);
            v = {
               v.x + (mid[0].x - v.x) * 0.175,
               v.y + (mid[0].y - v.y) * 0.175 };
            v = {
               v.x + (tv[0].x - v.x) * 0.065,
               v.y + (tv[0].y - v.y) * 0.065 };
            break;
         case 2:
            computeTriCentroid(v, mid[0], tv[1], mid[1]);
            v = {
               v.x + (mid[1].x - v.x) * 0.105,
               v.y + (mid[1].y - v.y) * 0.105 };
            v = {
               v.x + (mid[2].x - v.x) * 0.032,
               v.y + (mid[2].y - v.y) * 0.032 };
            break;
         case 3:
            computeTriCentroid(v, mid[2], mid[1], tv[2]);
            v = {
               v.x + (tv[2].x - v.x) * 0.175,
               v.y + (tv[2].y - v.y) * 0.175 };
            v = {
               v.x + (mid[2].x - v.x) * 0.065,
               v.y + (mid[2].y - v.y) * 0.065 };
            break;
      }
   }

   void ::computeBasePgonVertices(Pointd * pentagon/*[5]*/, int quintant, int64 triCurve)
   {
      Pointd qc = A5Zone { 1, quintant }.centroid, qVerts[5], mid[5];
      Pointd qq[5], qm[5]; // For neighboring triangle
      int i;

      A5Zone::getVertices(1, quintant, 0, qc, 3, qVerts);

      for(i = 0; i < 3; i++)
      {
         if(qVerts[i].x - qc.x < -3)
            qVerts[i].x += 5, qVerts[i].y += 5;
         else if(qVerts[i].x - qc.x > 3)
            qVerts[i].x -= 5, qVerts[i].y -= 5;
      }

      // FIXME: Interruption crossing...
      mid5x6(mid[0], qVerts[0], qVerts[1]);
      mid5x6(mid[1], qVerts[1], qVerts[2]);
      mid5x6(mid[2], qVerts[2], qVerts[0]);

      switch(triCurve)
      {
         case 0:
            pentagon[0] = mid[1];
            getTriPgonVertex(pentagon[1], qVerts, mid, qc, 0);
            getTriPgonVertex(pentagon[2], qVerts, mid, qc, 1);
            pentagon[3] = mid[0];
            getTriPgonVertex(pentagon[4], qVerts, mid, qc, 2);
            break;
          case 1:
            getTriPgonVertex(pentagon[0], qVerts, mid, qc, 0);
            pentagon[1] = mid[2];
            setupNBTri(qVerts, mid, qq, qm, 0, 2, 1);
            getTriPgonVertex(pentagon[2], qq, qm, null, 1);
            pentagon[3] = qVerts[0];
            getTriPgonVertex(pentagon[4], qVerts, mid, qc, 1);
            break;
         case 2:
            pentagon[0] = mid[1];
            getTriPgonVertex(pentagon[1], qVerts, mid, qc, 0);
            pentagon[2] = mid[2];
            getTriPgonVertex(pentagon[3], qVerts, mid, qc, 3);
            setupNBTri(qVerts, mid, qq, qm, 2, 1, 2);
            getTriPgonVertex(pentagon[4], qq, qm, null, 2);
            break;
         case 3:
            getTriPgonVertex(pentagon[0], qVerts, mid, qc, 2);
            pentagon[1] = mid[0];
            setupNBTri(qVerts, mid, qq, qm, 1, 0, 0);
            getTriPgonVertex(pentagon[2], qq, qm, null, 3);
            pentagon[3] = qVerts[1];
            setupNBTri(qVerts, mid, qq, qm, 2, 1, 2);
            getTriPgonVertex(pentagon[4], qq, qm, null, 3);
            break;
      }
   }

   Array<Pointd> getBaseRefinedVertices(bool crs84, int nDivisions)
   {
      Array<Pointd> vertices { minAllocSize = Max(1, nDivisions) * 5 };
      if(level == 0)
      {
         // Dodecahedron pentagons
         Pointd c = centroid;
         uint root = quintant;
         double A = 1 / 3.0, B = 2 / 3.0;

         if(c.y > 6 + 1E-9 || c.x > 5 + 1E-9)
            c.x -= 5, c.y -= 5;
         else if(c.x < 0)
            c.x += 5, c.y += 5;

         if(root == 0xA) // North Pole
         {
            Pointd a { 1 - B, -A };
            Pointd b { 1 - A, +A };
            Pointd ab { (a.x + b.x) / 2, (a.y + b.y) / 2 };
            Pointd d;

            rotate5x6Offset(d, b.x - ab.x, b.y - ab.y, false);
            d.x += b.x, d.y += b.y;

            addIntermediatePoints(vertices, { b.x + 0, b.y + 0 }, { b.x + 1, b.y + 1 }, nDivisions, { d.x + 0, d.y + 0 }, { ab.x + 1, ab.y + 1 }, crs84);
            addIntermediatePoints(vertices, { b.x + 1, b.y + 1 }, { b.x + 2, b.y + 2 }, nDivisions, { d.x + 1, d.y + 1 }, { ab.x + 2, ab.y + 2 }, crs84);
            addIntermediatePoints(vertices, { b.x + 2, b.y + 2 }, { b.x + 3, b.y + 3 }, nDivisions, { d.x + 2, d.y + 2 }, { ab.x + 3, ab.y + 3 }, crs84);
            addIntermediatePoints(vertices, { b.x + 3, b.y + 3 }, { b.x + 4, b.y + 4 }, nDivisions, { d.x + 3, d.y + 3 }, { ab.x + 4, ab.y + 4 }, crs84);
            if(crs84)
               addIntermediatePoints(vertices, { b.x + 4, b.y + 4 }, { b.x + 0, b.y + 0 }, nDivisions, { d.x + 4, d.y + 4 }, { ab.x + 0, ab.y + 0 }, crs84);
            else
            {
               vertices.Add({ b.x + 4, b.y + 4 });
               vertices.Add({ d.x + 4, d.y + 4 });
               // These are the "North" pole
               vertices.Add({ 5, 4 });
               vertices.Add({ 1, 0 });
               vertices.Add(ab);
            }
         }
         else if(root == 0xB) // South Pole
         {
            Pointd a { 4 + B, 6 + A };
            Pointd b { 4 + A, 6 - A };
            Pointd ab { (a.x + b.x) / 2, (a.y + b.y) / 2 };
            Pointd d;

            rotate5x6Offset(d, b.x - ab.x, b.y - ab.y, false);
            d.x += b.x, d.y += b.y;

            addIntermediatePoints(vertices, { b.x - 0, b.y - 0 }, { b.x - 1, b.y - 1 }, nDivisions, { d.x - 0, d.y - 0 }, { ab.x - 1, ab.y - 1 }, crs84);
            addIntermediatePoints(vertices, { b.x - 1, b.y - 1 }, { b.x - 2, b.y - 2 }, nDivisions, { d.x - 1, d.y - 1 }, { ab.x - 2, ab.y - 2 }, crs84);
            addIntermediatePoints(vertices, { b.x - 2, b.y - 2 }, { b.x - 3, b.y - 3 }, nDivisions, { d.x - 2, d.y - 2 }, { ab.x - 3, ab.y - 3 }, crs84);
            addIntermediatePoints(vertices, { b.x - 3, b.y - 3 }, { b.x - 4, b.y - 4 }, nDivisions, { d.x - 3, d.y - 3 }, { ab.x - 4, ab.y - 4 }, crs84);

            if(crs84)
               addIntermediatePoints(vertices, { b.x - 4, b.y - 4 }, { b.x - 0, b.y - 0 }, nDivisions, { d.x - 4, d.y - 4 }, { ab.x - 0, ab.y - 0 }, crs84);
            else
            {
               vertices.Add({ b.x - 4, b.y - 4 });
               vertices.Add({ d.x - 4, d.y - 4 });
               // These are the "South" pole
               vertices.Add({ 0, 2 });
               vertices.Add({ 4, 6 });
               vertices.Add(ab);
            }
         }
         else
         {
            Pointd v[6] =
            {
               { -A, -B },
               { -B, -A },
               { -A, +A },
               { +A, +B },
               { +B, +A },
               { +A, -A }
            };
            addNonPolarVerticesRefined(c, v, vertices, crs84, nDivisions, 6);
         }
      }
      else if(level == 1)
      {
         // Pentakis dodecahedron triangles
         double A = 1 / 3.0, B = 2 / 3.0;
         A5Zone root { 0, quintant / 5, 0 };
         bool isSouth = (quintant / 5) & 1;
         Pointd rc = root.centroid;
         int tri = quintant % 5;
         Pointd v[6];

         if(quintant == 0 || quintant == 1 || quintant == 4 || quintant == 5 || quintant == 6)
         {
            rc.x += 5;
            rc.y += 5;
         }

         v[0] = { rc.x -A, rc.y -B };
         v[1] = { rc.x -B, rc.y -A };
         v[2] = { rc.x -A, rc.y +A };
         v[3] = { rc.x +A, rc.y +B };
         v[4] = { rc.x +B, rc.y +A };
         v[5] = { rc.x +A, rc.y -A };

         if(quintant >= 50 && quintant < 55)
         {
            // North pole
            Pointd v0 { 2/3.0 + tri, 1/3.0 + tri };
            Pointd v1 { 1 + tri, 0.5 + tri };
            Pointd v2 { 1.5 + tri, 1 + tri };
            Pointd v3 { 1 + 2/3.0 + tri, 1 + 1/3.0 + tri };
            Pointd v4 { 2 + tri, 1 + tri };
            Pointd v5 { 1 + tri, 0 + tri };

            addIntermediatePoints(vertices, v0, v1, nDivisions, null, null, crs84);
            addIntermediatePoints(vertices, v1, v2, nDivisions, null, null, crs84);
            addIntermediatePoints(vertices, v2, v3, nDivisions, null, null, crs84);
            addIntermediatePoints(vertices, v3, v4, nDivisions, null, null, crs84);
            addIntermediatePoints(vertices, v4, v5, nDivisions, null, null, crs84);
            addIntermediatePoints(vertices, v5, v0, nDivisions, null, null, crs84);
         }
         else if(quintant >= 55 && quintant < 60)
         {
            // South pole
            Pointd v0 { 4 + 1/3.0 - tri, 5 + 2/3.0 - tri };
            Pointd v1 { 4 - tri, 6   - tri };
            Pointd v2 { 4 + 1 - tri, 6  + 1  - tri };
            Pointd v3 { 4 + 1 + 1/3.0 - tri, 5 + 1 + 2/3.0 - tri };
            Pointd v4 { 4 + 1 - tri, 5.5 + 1 - tri };
            Pointd v5 { 4.5 - tri, 6 - tri };

            addIntermediatePoints(vertices, v0, v1, nDivisions, null, null, crs84);
            addIntermediatePoints(vertices, v1, v2, nDivisions, null, null, crs84);
            addIntermediatePoints(vertices, v2, v3, nDivisions, null, null, crs84);
            addIntermediatePoints(vertices, v3, v4, nDivisions, null, null, crs84);
            addIntermediatePoints(vertices, v4, v5, nDivisions, null, null, crs84);
            addIntermediatePoints(vertices, v5, v0, nDivisions, null, null, crs84);
         }
         else if(!isSouth && tri == 4)
         {
            bool inNorth;
            Pointd iSrc, iDst;

            crosses5x6InterruptionV2(v[4], A - B, -A - A, iSrc, iDst, &inNorth);

            if(crs84 && iDst.y < 0)
               iDst.x += 5, iDst.y += 5;
            else
            {
               if(iSrc.x - rc.x < -3)
                  iSrc.x += 5, iSrc.y += 5;
               if(iDst.x - rc.x < -3)
                  iDst.x += 5, iDst.y += 5;
            }

            addIntermediatePoints(vertices, v[4], iSrc, nDivisions, null, null, crs84); //iSrc, iDst, crs84);
            addIntermediatePoints(vertices, iSrc, iDst, nDivisions, null, null, crs84); //iSrc, iDst, crs84);
            addIntermediatePoints(vertices, iDst, v[0], nDivisions, null, null, crs84); //5iSrc, iDst, crs84);
            addIntermediatePoints(vertices, v[0], rc, nDivisions, null, null, crs84);
            addIntermediatePoints(vertices, rc, v[4], nDivisions, null, null, crs84);
         }
         else if(isSouth && tri == 1)
         {
            bool inNorth;
            Pointd iSrc, iDst;

            crosses5x6InterruptionV2(v[1], -A + B, 2 * A, iSrc, iDst, &inNorth);

            if(crs84 && iDst.y > 6)
            {
               iSrc.x -= 5, iSrc.y -= 5;
               iDst.x -= 5, iDst.y -= 5;
            }
            else
            {
               if(iSrc.x - rc.x > 3)
                  iSrc.x -= 5, iSrc.y -= 5;
               if(iDst.x - rc.x > 3)
                  iDst.x -= 5, iDst.y -= 5;
            }

            addIntermediatePoints(vertices, v[1], iSrc, nDivisions, null, null, crs84); //iSrc, iDst, crs84);
            addIntermediatePoints(vertices, iSrc, iDst, nDivisions, null, null, crs84); //iSrc, iDst, crs84);
            addIntermediatePoints(vertices, iDst, v[3], nDivisions, null, null, crs84); //iSrc, iDst, crs84);
            addIntermediatePoints(vertices, v[3], rc, nDivisions, null, null, crs84);
            addIntermediatePoints(vertices, rc, v[1], nDivisions, null, null, crs84);
         }
         else
         {
            if(isSouth && tri > 1)
               tri++;
            addIntermediatePoints(vertices, v[tri], v[(tri+1) % (isSouth ? 6 : 5)], nDivisions, null, null, crs84);
            addIntermediatePoints(vertices, v[(tri+1) % (isSouth ? 6 : 5)], rc, nDivisions, null, null, crs84);
            addIntermediatePoints(vertices, rc, v[tri], nDivisions, null, null, crs84);
         }
      }
      else if(level == 2)
      {
         // First level of A5 irregular equilateral pentagons
         Pointd pentagon[5];
         Pointd iSrc[5], iDst[5];
         bool crossing[5] = { false, false, false, false, false };
         Pointd rQC;
         int i;

         if(quintant >= 50)
         {
            // TODO: Polar caps
            return null;
         }
         else if((quintant % 10) == 4)
         {
            // Northern interrupted triangles of the pentakis dodecahedron below polar caps
            // For now handling interruption by computing in a different quintant then rotating
            int rotations[5] = { -1, -1, -1, -1, -1 };
            int aQuintant = quintant - 4;
            int trr = aQuintant / 10;
            int rr = trr == 0 ? 5 : trr;

            switch(triCurve)
            {
               case 0:
                  rotations[0] = -2, rotations[2] = -2, rotations[3] = -2, rotations[4] = -2;
                  rotations[1] = -1;
                  break;
               case 1: rotations[4] = -2; break;
               case 3:
                  rotations[0] = -2, rotations[1] = -2, rotations[2] = -2, rotations[3] = -2;
                  rotations[4] = -2;
                  break;
            }

            computeBasePgonVertices(pentagon, aQuintant, triCurve);

            for(i = 0; i < 5; i++)
            {
               int j;
               Pointd d;

               for(j = 0; j < Abs(rotations[i]); j++)
                  rotate5x6Offset(d,
                     j == 0 ? pentagon[i].x - rr : d.x,
                     j == 0 ? pentagon[i].y - rr : d.y, rotations[i] < 0);
               pentagon[i] = { trr + d.x, trr + d.y };
            }

            if(triCurve == 0)
            {
               crossing[0] = true;
               iSrc[0] = pentagon[0];
               iDst[0] = { pentagon[0].x - 0.5, pentagon[0].y - 0.5 };

               crossing[1] = true;
               iSrc[1].x = 1    + (trr + 4) % 5;
               iSrc[1].y = 0.75 + (trr + 4) % 5;
               iDst[1].x = 1.25 + (trr + 4) % 5;
               iDst[1].y = 1    + (trr + 4) % 5;
            }
            else if(triCurve == 1)
            {
               crossing[4] = true;
               iSrc[4].x = 1.25 + (trr + 4) % 5;
               iSrc[4].y = 1    + (trr + 4) % 5;
               iDst[4].x = 1    + (trr + 4) % 5;
               iDst[4].y = 0.75 + (trr + 4) % 5;
            }
         }
         else if((quintant % 10) == 6)
         {
            // Southern interrupted triangles of the pentakis dodecahedron above polar caps
            // For now handling interruption by computing in a different quintant then rotating
            int rotations[5] = { 1, 1, 1, 1, 1 };
            int aQuintant = quintant - 1;
            int trr = ((aQuintant + 5) / 10);
            int rr = (trr == 1 ? 6 : trr);

            switch(triCurve)
            {
               case 0: rotations[1] = 2; break;
               case 1:
                  rotations[0] = 2; rotations[1] = 2; rotations[2] = 2; rotations[3] = 2;
                  break;
               case 2:
                  rotations[0] = 2, rotations[1] = 2, rotations[2] = 2, rotations[3] = 2;
                  rotations[4] = 2;
                  break;
            }

            computeBasePgonVertices(pentagon, aQuintant, triCurve);

            for(i = 0; i < 5; i++)
            {
               int j;
               Pointd d;

               for(j = 0; j < Abs(rotations[i]); j++)
                  rotate5x6Offset(d,
                     j == 0 ? pentagon[i].x - (rr - 1) : d.x,
                     j == 0 ? pentagon[i].y - (rr) : d.y, rotations[i] < 0);
               pentagon[i] = { trr - 1 + d.x, trr + d.y };
            }

            if(triCurve == 0)
            {
               crossing[0] = true;
               iSrc[0] = pentagon[0];
               iDst[0] = { pentagon[0].x + 0.5, pentagon[0].y + 0.5 };

               crossing[1] = true;
               iSrc[1].x = 1    + (trr + 3) % 5;
               iSrc[1].y = 2.25 + (trr + 3) % 5;
               iDst[1].x = 0.75 + (trr + 3) % 5;
               iDst[1].y = 2    + (trr + 3) % 5;
            }
            else if(triCurve == 1)
            {
               crossing[4] = true;
               iDst[4].x = 1    + (trr + 3) % 5;
               iDst[4].y = 2.25 + (trr + 3) % 5;
               iSrc[4].x = 0.75 + (trr + 3) % 5;
               iSrc[4].y = 2    + (trr + 3) % 5;
            }
         }
         else
            //return null;
            computeBasePgonVertices(pentagon, quintant, triCurve);

         // Keep most of pentagon within icosahedral net
         computePgonCentroid(rQC, pentagon);

         if(rQC.x > 5.5 && rQC.y > 5.5)
            rQC.x -= 5, rQC.y -= 5;
         else if(rQC.x < -0.05 || rQC.y < -0.05)
            rQC.x += 5, rQC.y += 5;
         for(i = 0; i < 5; i++)
         {
            if(pentagon[i].x - rQC.x > 3 && pentagon[i].y - rQC.y > 3)
               pentagon[i].x -= 5, pentagon[i].y -= 5;
            else if(pentagon[i].x - rQC.x < -3 && pentagon[i].y - rQC.y < -3)
               pentagon[i].x += 5, pentagon[i].y += 5;
         }

         for(i = 0; i < 5; i++)
         {
            if(crossing[i])
            {
               // REVIEW: Source interruption need to be within 3 distance, even x = 5.5?
               if(iSrc[i].x - rQC.x > 3 && iSrc[i].y - rQC.y > 3)
                  iSrc[i].x -= 5, iSrc[i].y -= 5;
               else if(iSrc[i].x - rQC.x < -3 && iSrc[i].y - rQC.y < -3)
                  iSrc[i].x += 5, iSrc[i].y += 5;

               if(iDst[i].x - rQC.x > 3 && iDst[i].y - rQC.y > 3)
                  iDst[i].x -= 5, iDst[i].y -= 5;
               else if(iDst[i].x - rQC.x < -3 && iDst[i].y - rQC.y < -3)
                  iDst[i].x += 5, iDst[i].y += 5;
            }
         }

         addIntermediatePoints(vertices, pentagon[0], pentagon[1], nDivisions, crossing[0] ? iSrc[0] : null, crossing[0] ? iDst[0] : null, crs84);
         addIntermediatePoints(vertices, pentagon[1], pentagon[2], nDivisions, crossing[1] ? iSrc[1] : null, crossing[1] ? iDst[1] : null, crs84);
         addIntermediatePoints(vertices, pentagon[2], pentagon[3], nDivisions, crossing[2] ? iSrc[2] : null, crossing[2] ? iDst[2] : null, crs84);
         addIntermediatePoints(vertices, pentagon[3], pentagon[4], nDivisions, crossing[3] ? iSrc[3] : null, crossing[3] ? iDst[3] : null, crs84);
         addIntermediatePoints(vertices, pentagon[4], pentagon[0], nDivisions, crossing[4] ? iSrc[4] : null, crossing[4] ? iDst[4] : null, crs84);

         /*addIntermediatePoints(vertices, mid[0], mid[1], nDivisions, null, null, crs84);
         addIntermediatePoints(vertices, mid[1], mid[2], nDivisions, null, null, crs84);
         addIntermediatePoints(vertices, mid[2], mid[0], nDivisions, null, null, crs84);
         */
         /*
         addIntermediatePoints(vertices, qVerts[0], qVerts[1], nDivisions, null, null, crs84);
         addIntermediatePoints(vertices, qVerts[1], qVerts[2], nDivisions, null, null, crs84);
         addIntermediatePoints(vertices, qVerts[2], qVerts[0], nDivisions, null, null, crs84);
         */
      }
      return vertices;
   }

#if 0
   uint getBaseRefinedVerticesNoAlloc(bool crs84, int nDivisions, Pointd * vertices)
   {
      uint nVertices = 0;
      return nVertices;
   }

   property A5Zone centroidChild
   {
      get
      {
         if(this == nullZone || (levelI49R == 9 && subHex))
            return nullZone;
         else
         {
            uint root = rootRhombus;
            uint64 ix = rhombusIX;

            if(!subHex) // Odd level from even level
               return A5Zone { levelI49R, root, ix, 1 };
            else // Even level from odd level
            {
               int64 row, col;
               uint64 cp;
               int cRhombus = getOddLevelCentroidChildRootRowCol(&row, &col, &cp);
               return cRhombus == -1 ? nullZone : A5Zone { levelI49R + 1, cRhombus, (uint64)row * cp + col, 0 };
            }
         }
      }
   }

   int getChildren(A5Zone children[13])
   {
      int n = getPrimaryChildren(children);
      if(n)
      {
      }
      return n;
   }

   int getPrimaryChildren(A5Zone children[7])
   {
      int count = 0;
      return count;
   }

   property CRSExtent ri5x6Extent
   {
      get
      {
         Array<Pointd> vertices = getBaseRefinedVertices(false, 0);
         int nVertices = vertices ? vertices.count : 0, i;

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
#endif

   property Pointd centroid
   {
      get
      {
         if(this == nullZone)
            value = { -999, -999 };
         else if(level == 0)
         {
            int root = quintant;

            if(root == 0xA) // North pole
               value = { 1, 0 };
            else if(root == 0xB) // South pole
               value = { 4, 6 };
            else
            {
               int cx = root >> 1, cy = cx + (root & 1);
               value = { cx, cy };
            }

            if(fabs(value.y - 0) < 1E-6)
               value.y = 0;
            else if(fabs(value.x - 5) < 1E-6)
               value.x = 5;
            if(value.x > 5 - 1E-6 || value.y > 6 + 1E-6)
               value.x -= 5, value.y -= 5;
            if(value.x < 0 - 1E-6)
               value.x += 5, value.y += 5;
         }
         else if(level == 1)
         {
            int tri = quintant % 5;

            if(quintant >= 55)
               // South pole
               value = { 4 + 1/3.0 - tri, 6 - tri };
            else if(quintant >= 50)
               // North pole
               value = { 1 + 2/3.0 + tri, 1 + tri };
            else
            {
               double A = 1 / 3.0, B = 2 / 3.0;
               int rootPgon = quintant / 5;
               bool isSouth = rootPgon & 1;

               if(!isSouth && tri == 4)
               {
                  int k = ((rootPgon)/2 + 4) % 5;
                  value = { 1 + k, k + 2/3.0 };
               }
               else if(isSouth && tri == 1)
               {
                  int k = rootPgon == 1 ? 0 : 5 - rootPgon / 2;
                  value = { 4 + 2/3.0 - k, 6 - k };
               }
               else
               {
                  A5Zone root { 0, rootPgon, 0 };
                  Pointd rc = root.centroid;
                  Pointd v[6] =
                  {
                     { rc.x -A, rc.y -B },
                     { rc.x -B, rc.y -A },
                     { rc.x -A, rc.y +A },
                     { rc.x +A, rc.y +B },
                     { rc.x +B, rc.y +A },
                     { rc.x +A, rc.y -A }
                  };

                  if(isSouth && tri > 1)
                     tri++;
                  computeTriCentroid(value, v[tri], v[(tri+1) % (isSouth ? 6 : 5)], rc);

                  if(value.x < 0)
                     value.x += 5, value.y += 5;
               }
            }
         }
         else
         {
            // TODO: Proper centroid for higher levels...
            A5Zone root { 1, quintant, 0 };
            value = root.centroid;
            /*
            if(value.x > 5.5 && value.y > 5.5)
               value.x -= 5, value.y -= 5;
            else if(value.x < -0.05 || value.y < -0.05)
               value.x += 5, value.y += 5;
            */
         }
      }
   }

#if 0
   property bool isCentroidChild
   {
      get
      {
         if(subHex == 1)  // All '-B' are centroid children
            return true;
         else if(subHex == 0)
         {
            // Some '-A' are centroid children
            A5Zone parent0 = this.parent0;
            return parent0 != nullZone && this == parent0.centroidChild;
         }
         return false;
      }
   }

#if 0
   private static inline int64 ::triNumber(int64 n)
   {
      if (n <= 0) return 0;
      return n * (n + 1) / 2;
   }

   private static inline int64 ::sumArithmeticSeries(int64 n, int64 firstTerm, int64 commonDiff)
   {
       if (n <= 0) return 0;
       // return n * (2 * firstTerm + (n - 1) * commonDiff) / 2;
       return n * firstTerm + commonDiff * triNumber(n - 1);
   }

   private static inline int64 ::computeAddedContrib(uint64 nScanlines, int zonesAdded, uint nTimesAdditionRepeated,
      int scanlineGapBetweenRepetitions, int phaseOffsetFromFirstAddition)
   {
      int64 total = 0;

      if(nScanlines)
      {
         int patternSize = nTimesAdditionRepeated + scanlineGapBetweenRepetitions;
         int p;

         for(p = 0; p < nTimesAdditionRepeated; p++)
         {
            int64 firstIncLine = (p - phaseOffsetFromFirstAddition + patternSize) % patternSize;
            if(firstIncLine < nScanlines)
            {
               int64 n = (nScanlines - 1 - firstIncLine) / patternSize + 1;
               int64 firstTerm = (int64)zonesAdded * (nScanlines - firstIncLine);
               int64 commonDiff = -(int64)zonesAdded * patternSize;
               total += sumArithmeticSeries(n, firstTerm, commonDiff);
            }
         }
      }
      return total;
   }

   private static inline int64 ::computeHexOddDepthSubZones(int rDepth)
   {
       int64 nInterSL = POW7((rDepth - 1) / 2);
       int64 nCapSL = (int64)(ceil(nInterSL / 3.0) + 0.1);
       int64 nMidSL = (int64)(ceil(nInterSL * 2 / 3.0) + 0.1);

       // A..B
       int64 abRight = computeAddedContrib(nCapSL - 1, 5, 1, 0, 0);
       int64 abLeft = computeAddedContrib(nCapSL - 1, 1, 1, 3, 0);
       int64 nZonesAB = nCapSL * 1 + abLeft + abRight;
       int64 abLeftAddition = (nCapSL > 1 ? (nCapSL - 2) / 4 + 1 : 0);

       // B..C
       int64 bZonesPerSL = 1 + 5 * (nCapSL - 1) + abLeftAddition;
       int64 bcLeft = computeAddedContrib(nInterSL, 1, 1, 3, (int)((nCapSL - 1) % 4));
       int64 bcRight = 2 * nInterSL + computeAddedContrib(Max(0, nInterSL - 2), 1, 4, 1, 0);
       int64 nZonesBC = nInterSL * bZonesPerSL + bcLeft + bcRight;

       // C..D
       int64 bcLeftInc = (nCapSL + nInterSL - 1 + 3) / 4 - abLeftAddition;
       int64 bcRightInc = 2 + (nInterSL - 2)/5 * 4 + (nInterSL - 2) % 5;
       int64 cZonesPerSL = bZonesPerSL + bcLeftInc + bcRightInc;
       int64 cdLeft = -1 * nMidSL + computeAddedContrib(Max(0, nMidSL - 2), -1, 4, 1, 0);
       int64 cdRight = computeAddedContrib(nMidSL, 1, 4, 1, Max(0, nInterSL - 2) % 5);
       int64 nZonesCD = nMidSL * cZonesPerSL + cdLeft + cdRight;

       return 2 * nZonesAB + 2 * nZonesBC + nZonesCD;
   }
#endif

   int64 getSubZonesCount(int rDepth)
   {
      if(rDepth > 0)
      {
         int64 nHexSubZones;

         if(rDepth & 1)
         {
#if 0 // def _DEBUG
            int64 nInterSL = (int64)(POW7((rDepth-1) / 2));
            int64 nCapSL = (int64)(ceil(nInterSL / 3.0) + 0.5);
            int64 nMidSL = (int64)(ceil(nInterSL * 2 / 3.0) + 0.5);
            // int64 nScanlines = 2 * nCapSL + 2 * nInterSL + nMidSL;
            int64 s;
            int64 zonesPerSL = 1;
            int64 leftACCounter = 0, leftCECounter = 0, rightBDCounter = 0;
            int64 nZonesAB = 0, nZonesBC = 0, nZonesCD = 0;

            for(s = 0; s < nCapSL; s++)
            {
               int64 left = (s > 0 && (leftACCounter++) % 4 == 0) ? 1 : 0;
               int64 right = (s > 0) ? 5 : 0;
               zonesPerSL += left + right;
               nZonesAB += zonesPerSL;
            }

            for(s = 0; s < nInterSL; s++)
            {
               int64 left = ((leftACCounter++) % 4 == 0) ? 1 : 0;
               int64 right = (s == 0) ? 2 : ((rightBDCounter++) % 5 != 0) ? 1 : 0;
               zonesPerSL += left + right;
               nZonesBC += zonesPerSL;
            }

            for(s = 0; s < nMidSL; s++)
            {
               int64 left = (s == 0 || (leftCECounter++) % 5 != 0) ? -1 : 0;
               int64 right = ((rightBDCounter++) % 5 != 0) ? 1 : 0;
               zonesPerSL += left + right;
               nZonesCD += zonesPerSL;
            }
            nHexSubZones = 2 * nZonesAB + 2 * nZonesBC + nZonesCD;
#endif
            // nHexSubZones = computeHexOddDepthSubZones(rDepth);
            //                            https://oeis.org/A199422
            nHexSubZones = POW7(rDepth) + 5 * POW7((rDepth-1)/2) + 1;
         }
         else
                                       // https://oeis.org/A024075
            nHexSubZones = POW7(rDepth) + POW7(rDepth/2) - 1;
         return (nHexSubZones * nPoints + 5) / 6;
      }
      return 1;
   }

   A5Zone getFirstSubZone(int rDepth)
   {
      Pointd firstCentroid;

      getFirstSubZoneCentroid(rDepth, firstCentroid, null, null);
      return fromCentroid(level + rDepth, firstCentroid);
   }

   int getTopIcoVertex(Pointd v)
   {
      Pointd vertices[6];
      Pointd c = this.centroid;
      int n = getVertices(levelI49R, rootRhombus, subHex, c, nPoints, vertices);
      int i, top = 0;
      Pointd topIco;
      bool equalTop = false;
      bool north = c.x - c.y - 1E-11 > 0;
      int level = this.level;
      bool oddLevel = level & 1;
      bool specialOddCase = false;

      RI5x6Projection::toIcosahedronNet(vertices[0], topIco);

      for(i = 1; i < n; i++)
      {
         Pointd ico;

         RI5x6Projection::toIcosahedronNet(vertices[i], ico);

         if(ico.y > topIco.y + 1E-6)
         {
            equalTop = false;
            topIco = ico;
            top = i;
         }
         else if(ico.y >= topIco.y - 1E-6)
         {
            if(!oddLevel || north)
               equalTop = true;
            if(ico.x < topIco.x - 1E-8 && topIco.x - ico.x < 5*triWidthOver2)
            {
               topIco = ico;
               top = i;
            }
         }
      }

      // First vertex can't be to the right of northern interruption
      if(oddLevel && north &&
         floor(vertices[top].y + 1E-11) > floor(vertices[(top+1) % n].y + 1E-11) &&
         vertices[top].y - vertices[(top+1) % n].y < 3)
      {
         specialOddCase = true; // Special rotation case applies in this case...
         top = (top + 1) %  n;
      }

      if(v != null)
         v = vertices[top];
      return equalTop || specialOddCase ? 2 : 1;
   }

   void getFirstSubZoneCentroid(int rDepth, Pointd firstCentroid, double * sx, double * sy)
   {
      // TODO: Correctly handling polar cases
      Pointd v;
      int nTop = getTopIcoVertex(v);
      int level = this.level;
      int64 szp = POW7((level + 1 + rDepth) / 2);
      double dx, dy;

      if(rDepth & 1) // Odd depth
      {
         // First sub-zone centroid is one sub-zone edge length away from sub-zone's vertex preceding shared vertex
         if(level & 1)
         {
            dx = 2 / (3.0 * szp);
            dy = 1 / (3.0 * szp);
         }
         else
         {
            dx = -4 / (3.0 * szp);
            dy = -5 / (3.0 * szp);
         }
      }
      else // Even depth
      {
         // First sub-zone centroid is two sub-zone edges length towards next vertex
         if(level & 1)
         {
            dx = -10 / (3.0 * szp);
            dy =  -2 / (3.0 * szp);
         }
         else
         {
            dx = -4 / (3.0 * szp);
            dy = -2 / (3.0 * szp);
         }
      }

      if(nTop == 2 && ((level & 1) || v.x - v.y - 1E-11 > 0))
      {
         // Hexagon spanning interruption between 2 top vertices, rotate offset 60 degrees counter-clockwise
         double ndy = dy - dx;
         dx = dy;
         dy = ndy;

         if(sx && sy)
         {
            ndy = *sy - *sx;
            *sx = *sy;
            *sy = ndy;
         }
      }

      move5x6(firstCentroid, v, dx, dy, 1, null, null, false);
   }

   int64 iterateI7HSubZones(int rDepth, void * context,
      bool (* centroidCallback)(void * context, uint64 index, const Pointd centroid), int64 searchIndex)
   {
      if(rDepth == 0)
         return centroidCallback(context, 0, centroid) ? -1 : 0;
      else
      {
         bool keepGoing = true;
         int level = this.level;
         int szLevel = level + rDepth;
         bool oddAncestor = level & 1, oddDepth = rDepth & 1, oddLevelSZ = szLevel & 1;
         Pointd first;
         int64 szp = POW7((szLevel + oddLevelSZ)/2);
         double c2c = 1.0 / szp; // Centroid to centroid distance between sub-zones along 5x6 x and y axes
         int64 cStart = 0;
         int64 index = 0;
         int64 s;
         int64 zonesPerSL;
         int64 nScanlines;
         int64 left, right;
         int64 i;
         // Direction along scanlines:
         double sx = c2c * (oddLevelSZ ? 3 : 1);
         double sy = c2c * (oddLevelSZ ? 2 : 1);
         // Direction to the next scanline (hexagon immediately to the left -- 120 degrees clockwise)
         double nsx, nsy;

         // TODO: Handle pentagons / polar zones correctly
         if(nPoints == 5) return -1;

         getFirstSubZoneCentroid(rDepth, first, &sx, &sy);

         // Rotate scanline direction to get direction to next scanline
         if(!oddAncestor && oddDepth)
            nsy = sx, nsx = sx - sy;  // 60 degrees clockwise
         else
            nsy = sx - sy, nsx = -sy;  // 120 degrees clockwise

         if(oddDepth)
         {
            int64 nInterSL = (int64)(POW7((rDepth-1) / 2));
            int64 nCapSL = (int64)(ceil(nInterSL / 3.0) + 0.5);
            int64 nMidSL = (int64)(ceil(nInterSL * 2 / 3.0) + 0.5);
            int64 B = nCapSL, C = B + nInterSL, D = C + nMidSL, E = D + nInterSL;
            int64 leftACCounter = 0, leftCECounter = 0, rightBDCounter = 0, rightDFCounter = 0;

            nScanlines = 2 * nCapSL + 2 * nInterSL + nMidSL;
            zonesPerSL = 1;

            for(s = 0; s < nScanlines && keepGoing; s++)
            {
               double tsx = sx, tsy = sy;

               if(s < B)
               {
                  left = (s > 0 && (leftACCounter++) % 4 == 0) ? 1 : 0;
                  right = (s > 0) ? 5 : 0;
               }
               else if(s < C)
               {
                  left = ((leftACCounter++) % 4 == 0) ? 1 : 0;
                  right = (s == B) ? 2 : ((rightBDCounter++) % 5 != 0) ? 1 : 0;
               }
               else if(s < D)
               {
                  left = (s == C || (leftCECounter++) % 5 != 0) ? -1 : 0;
                  right = ((rightBDCounter++) % 5 != 0) ? 1 : 0;
               }
               else if(s < E)
               {
                  left = ((leftCECounter++) % 5 != 0) ? -1 : 0;
                  right = (s == D) ? 1 : ((rightDFCounter++) % 4) == 0 ? -1 : 0;
               }
               else
               {
                  left = s == E ? -2 : -5;
                  right = ((rightDFCounter++) % 4) == 0 ? -1 : 0;
               }

               cStart += oddAncestor ? left : right;
               zonesPerSL += left + right;

               if(searchIndex == -1 || (searchIndex >= index && searchIndex < index + zonesPerSL))
               {
                  Pointd sc; // Start of scanline

                  if(searchIndex != -1)
                  {
                     i = (int)(searchIndex - index);
                     index = searchIndex;
                  }
                  else
                     i = 0;

                  move5x6(sc, first, s * nsx - cStart * tsx, s * nsy - cStart * tsy, 1, &tsx, &tsy, true);

                  for(; i < zonesPerSL; i++)
                  {
                     Pointd centroid;
                     move5x6(centroid, sc, i * tsx, i * tsy, 1, null, null, true);
                     keepGoing = centroidCallback(context, index, centroid);
                     if(searchIndex != -1 || !keepGoing)
                        break;
                     index++;
                  }
                  if(!keepGoing)
                     break;
               }
               else
                  index += zonesPerSL;
            }
         }
         else // Even depths
         {
            int64 nCapSL = (POW7(rDepth/2) - 1) / 3;
            int64 nMidSL = (2*POW7(rDepth/2) + 1)/3;
            int64 B = nCapSL, C = B + nMidSL;

            nScanlines = 2 * nCapSL + nMidSL;
            zonesPerSL = 3;

            for(s = 0; s < nScanlines && keepGoing; s++)
            {
               double tsx = sx, tsy = sy;

               if(s < B)
               {
                  left = s > 0 ? 1 : 0;
                  right = s > 0 ? 2 : 0;
               }
               else if(s < C)
               {
                  left = s == B || ((s - B) & 1) ? 0 : -1;
                  right = (s == B) || ((s - B) & 1) ? 1 : 0;
               }
               else
               {
                  left = s == C ? -1 : -2;
                  right = s == C ? 0 : -1;
               }

               cStart += left;
               zonesPerSL += left + right;

               if(searchIndex == -1 || (searchIndex >= index && searchIndex < index + zonesPerSL))
               {
                  Pointd sc; // Start of scanline

                  move5x6(sc, first, s * nsx - cStart * tsx, s * nsy - cStart * tsy, 1, &tsx, &tsy, true);

                  if(searchIndex != -1)
                  {
                     i = searchIndex - index;
                     index = searchIndex;
                  }
                  else
                     i = 0;

                  for(; i < zonesPerSL; i++)
                  {
                     Pointd centroid;
                     move5x6(centroid, sc, i * tsx, i * tsy, 1, null, null, true);
                     keepGoing = centroidCallback(context, index, centroid);
                     if(searchIndex != -1 || !keepGoing)
                        break;
                     index++;
                  }
               }
               else
                  index += zonesPerSL;
            }
         }
         return keepGoing ? -1 : index;
      }
   }

   private static inline bool ::addCentroid(Array<Pointd> centroids, uint64 index, Pointd centroid)
   {
      centroids[(uint)index] = centroid;
      return true;
   }

   Array<Pointd> getSubZoneCentroids(int rDepth)
   {
      uint64 nSubZones = getSubZonesCount(rDepth);
      // Each centroid is 16 bytes and array memory allocation currently does not support more than 4G
      if(nSubZones < 1LL<< (32-4) && (nPoints == 6 || !rDepth))
      {
         Array<Pointd> centroids { size = (uint)nSubZones };
         if(rDepth > 0)
            iterateI7HSubZones(rDepth, centroids, addCentroid, -1);
         else
            centroids[0] = centroid;
         return centroids;
      }
      return null;
   }

   private /*static */bool orderZones(int zoneLevel, AVLTree<A5Zone> tsZones, Array<A5Zone> zones)
   {
      Array<Pointd> centroids = getSubZoneCentroids(zoneLevel - level);
      if(centroids)
      {
         int nSubZones = centroids.count;
         int i;

         for(i = 0; i < nSubZones; i++)
         {
            A5Zone key = A5Zone::fromCentroid(zoneLevel, centroids[i]);
            if(tsZones.Find(key))
               zones.Add(key);
      #ifdef _DEBUG
            else
               PrintLn("WARNING: mismatched sub-zone while re-ordering");
      #endif
         }
         delete centroids;
         return true;
      }
      else
         return false; // Work around until all sub-zone listing fully handled
   }
#endif
}

__attribute__((unused)) static void compactA5Zones(AVLTree<A5Zone> zones, int level)
{
   /*
   AVLTree<A5Zone> output { };
   AVLTree<A5Zone> next { };
   int l;

   for(l = level - 1; l >= 0; l -= 1)
   {
      int i;
      for(z : zones)
      {
         A5Zone zone = z, cParents[2];
         int nCParents = zone.getParents(cParents);
         int p;
         for(p = 0; p < nCParents; p++)
         {
            A5Zone cParent = cParents[p];
            if(cParent != nullZone && !next.Find(cParent))
            {
               A5Zone children[13];
               bool parentAllIn = true;
               int nChildren = cParent.getChildren(children);

               for(i = 0; i < nChildren; i++)
               {
                  A5Zone c = children[i];
                  if(c != nullZone && !zones.Find(c))
                  {
                     parentAllIn = false;
                     break;
                  }
               }

               if(parentAllIn)
                  next.Add(cParent);
            }
         }
      }

      for(z : zones)
      {
         A5Zone zone = z, cParents[2];
         int nCParents = zone.getParents(cParents), i;
         bool allIn = true;

         for(i = 0; i < nCParents; i++)
         {
            if(!next.Find(cParents[i]))
            {
               allIn = false;
               break;
            }
         }
         if(!allIn)
            output.Add(zone);
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

   if(zones.count >= 72 && zones.firstIterator.data.level == 1)
   {
      int nL1 = 0;

      for(z : zones)
      {
         A5Zone zone = z;
         int level = zone.level;
         if(level == 1)
            nL1++;
         else if(level > 1)
            break;
      }

      // if(allL1)
      if(nL1 == 72)
      {
         // Simplifying full globe to level 0 zones
         int r;
         zones.Free();
         for(r = 0; r < 12; r++)
            zones.Add({ r, 0 });
      }
   }
*/
}

/*
static void getIcoNetExtentFromVertices(A5Zone zone, CRSExtent value)
{
   int i;
   Array<Pointd> vertices = getIcoNetRefinedVertices(zone, 0, true);
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
*/

static Array<Pointd> getIcoNetRefinedVertices(A5Zone zone, int edgeRefinement, bool ico)   // 0 for 1-20 based on level
{
   Array<Pointd> rVertices = zone.getBaseRefinedVertices(false, edgeRefinement);
   if(rVertices && rVertices.count && ico)
   {
      int i;
      for(i = 0; i < rVertices.count; i++)
      {
         Pointd p;
         RI5x6Projection::toIcosahedronNet(rVertices[i], p);
         rVertices[i] = p;
      }
   }
   return rVertices;
}

void mid5x6(Pointd out, const Pointd a, const Pointd b)
{
   Pointd d { b.x - a.x, b.y - a.y };
   Pointd iSrc, iDst;
   bool inNorth;

   if(d.x > 3) d.x -= 5, d.y -= 5;
   if(d.x <-3) d.x += 5, d.y += 5;

   // TODO: Handling interruptions properly...
   if(crosses5x6InterruptionV2(a, d.x, d.y, iSrc, iDst, &inNorth))
   {
      // Assuming the midpoint is exactly on interruption for now...
      out = iSrc;
   }
   else
   {
      out.x = a.x + d.x / 2;
      out.y = a.y + d.y / 2;
   }
   /*
   if(out.x > 5)
      out.x -= 5, out.y -= 5;
   if(out.x < 0)
      out.x += 5, out.y += 5;
   */
}

A5 a5DGGRS {};
