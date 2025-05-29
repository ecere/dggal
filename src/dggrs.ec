public import IMPORT_STATIC "ecrt"

private:

import "GeoExtent"

#include <stdio.h>

public define nullZone = 0xFFFFFFFFFFFFFFFFLL;

public struct CRSExtent
{
   CRS crs;
   Pointd tl, br;
};

public class DGGRSZone : uint64
{
   class_no_expansion;
private:
   uint level:5:59, row:29:30, col:30:0;
}

static double earthArea = 0; // 5.100656217240885092949E14;

static define stdMetersPerPixel = Meters { 0.00028 };       // 0.28 mm/pixels -- following standard WMS 1.3.0 [OGC 06-042], SE and WMTS
static define metersPerDegree = wgs84Major * (double)Pi/180;

define wgs84Authalic = 6371007.180918473897976252;

// #define USE_GEOGRAPHIC_LIB

#ifdef USE_GEOGRAPHIC_LIB
#include <geodesic.h>

struct geod_geodesic g;
struct geod_geodesic as;
#endif

public class DGGRS
{
#ifdef USE_GEOGRAPHIC_LIB
   DGGRS()
   {
      if(!g.a)
         geod_init(&g, wgs84Major, (wgs84Major - wgs84Minor) / wgs84Major);
      if(!as.a)
         geod_init(&as, wgs84Authalic, 0);
   }
#endif

public:
   // DGGH
   virtual uint64 countZones(int level) { return 0; }
   // This is the maximum level of the DGGRSZone type -- Sub-zones at greater depths relative from these zones may still be queried
   virtual int getMaxDGGRSZoneLevel() { return 0; }
   virtual int getRefinementRatio() { return 0; }

   virtual int getMaxParents() { return 0; }
   virtual int getMaxNeighbors() { return 0; }
   virtual int getMaxChildren() { return 0; }

   virtual DGGRSZone getZoneFromCRSCentroid(int level, CRS crs, const Pointd centroid) { return nullZone; }
   virtual DGGRSZone getZoneFromWGS84Centroid(int level, const GeoPoint centroid) { return nullZone; }

   virtual uint64 countSubZones(DGGRSZone zone, int depth) { return 0; }
   virtual int getZoneLevel(DGGRSZone zone) { return 0; }
   virtual int countZoneEdges(DGGRSZone zone) { return 0; } // This also corresponds to the number of vertices

   virtual void getZoneCRSCentroid(DGGRSZone zone, CRS crs, Pointd centroid) { centroid = { MAXDOUBLE, MAXDOUBLE }; }
   virtual void getZoneWGS84Centroid(DGGRSZone zone, GeoPoint centroid) { centroid = { MAXDOUBLE, MAXDOUBLE }; }

   virtual void getZoneCRSExtent(DGGRSZone zone, CRS crs, CRSExtent extent) { extent = { }; }
   virtual void getZoneWGS84Extent(DGGRSZone zone, GeoExtent extent) { extent.clear(); }

   virtual int getZoneCRSVertices(DGGRSZone zone, CRS crs, Pointd * vertices) { return 0; }
   virtual int getZoneWGS84Vertices(DGGRSZone zone, GeoPoint * vertices) { return 0; }

   // Automatic refinement selection if passing 0 for edgeRefinement (custom refinement not currently supported for ISEA9R)
   virtual Array<Pointd> getZoneRefinedCRSVertices(DGGRSZone zone, CRS crs, int edgeRefinement) { return 0; }
   virtual Array<GeoPoint> getZoneRefinedWGS84Vertices(DGGRSZone zone, int edgeRefinement) { return 0; }

   virtual double getZoneArea(DGGRSZone zone) { return 0; } // In meters square

   virtual int getZoneParents(DGGRSZone zone, DGGRSZone * parents) { return 0; }
   virtual int getZoneNeighbors(DGGRSZone zone, DGGRSZone * neighbors, int * nbType) { return 0; }
   virtual int getZoneChildren(DGGRSZone zone, DGGRSZone * children) { return 0; }

   // For DGGH with centroid parent concept (ISEA3H)
   virtual DGGRSZone getZoneCentroidParent(DGGRSZone zone) { return nullZone; }

   // For DGGH with centroid child concept (ISEA3H, ISEA9R?)
   // ISEA9R?:
   virtual DGGRSZone getZoneCentroidChild(DGGRSZone zone) { return nullZone; }
   // ISEA9R?:
   virtual bool isZoneCentroidChild(DGGRSZone zone) { return false; }

   virtual Array<DGGRSZone> listZones(int level, const GeoExtent bbox) { return null; }

   // Text ZIRS
   virtual void getZoneTextID(DGGRSZone zone, String zoneID) { zoneID[0] = 0; }
   virtual DGGRSZone getZoneFromTextID(const String zoneID) { return nullZone; }

   // Sub-zone Order
   virtual DGGRSZone getFirstSubZone(DGGRSZone zone, int relativeDepth) { return nullZone; }

   virtual Array<Pointd> getSubZoneCRSCentroids(DGGRSZone parent, CRS crs, int relativeDepth) { return null; }
   virtual Array<GeoPoint> getSubZoneWGS84Centroids(DGGRSZone parent, int relativeDepth) { return null; }

   // Compaction
   virtual void compactZones(Array<DGGRSZone> zones);

   // Utility methods or virtual methods with default implementations
   virtual int64 getSubZoneIndex(DGGRSZone parent, DGGRSZone subZone)
   {
      int64 ix = -1;
      int level = getZoneLevel(parent), szLevel = getZoneLevel(subZone);
      if(szLevel > level)
      {
         Array<DGGRSZone> subZones = getSubZones(parent, szLevel - level);
         if(subZones)
         {
            DGGRSZone * itPtr = (DGGRSZone *)subZones.Find(subZone);
            if(itPtr)
               ix = itPtr - subZones.array;
            delete subZones;
         }
      }
      return ix;
   }

   virtual DGGRSZone getSubZoneAtIndex(DGGRSZone parent, int relativeDepth, int64 index)
   {
      DGGRSZone subZone = nullZone;
      if(index >= 0 && index < countSubZones(parent, relativeDepth))
      {
         if(index == 0)
            return getFirstSubZone(parent, relativeDepth);
         else
         {
            Array<DGGRSZone> subZones = getSubZones(parent, relativeDepth);
            if(subZones && index < subZones.count)
               subZone = subZones[(uint)index];
            delete subZones;
         }
      }
      return subZone;
   }

   /*virtual */bool zoneHasSubZone(DGGRSZone hayStack, DGGRSZone needle)
   {
      bool result = false;
      int zLevel = getZoneLevel(hayStack), szLevel = getZoneLevel(needle);
      if(szLevel > zLevel)
      {
         Pointd v[6], c;
         int n, i;
         DGGRSZone cChild = getZoneCentroidChild(needle);

         if(cChild == nullZone)
         {
            getZoneCRSCentroid(needle, 0, c);
            n = getZoneCRSVertices(needle, 0, v);
         }
         else
            n = getZoneCRSVertices(cChild, 0, v);

         for(i = 0; i < n; i++)
         {
            DGGRSZone tz;

            if(cChild == nullZone)
            {
               Pointd m {
                  (c.x + v[i].x) / 2,
                  (c.y + v[i].y) / 2
               };
               tz = getZoneFromCRSCentroid(zLevel, 0, m);
            }
            else
               tz = getZoneFromCRSCentroid(zLevel, 0, v[i]);

            if(tz == hayStack)
            {
               result = true;
               break;
            }
         }
      }
      return result;
   }

   // REVIEW: Allow override for faster implementation?
   virtual Array<DGGRSZone> getSubZones(DGGRSZone parent, int relativeDepth)
   {
      Array<DGGRSZone> result = null;
      int szLevel = getZoneLevel(parent) + relativeDepth;

      if(szLevel <= getMaxDGGRSZoneLevel())
      {
         Array<Pointd> centroids = getSubZoneCRSCentroids(parent, 0, relativeDepth);
         if(centroids)
         {
            int nSubZones = centroids.count;
            Array<DGGRSZone> zones { size = nSubZones };
            int i;

            for(i = 0; i < nSubZones; i++)
            {
               zones[i] = getZoneFromCRSCentroid(szLevel, 0, centroids[i]);
   #ifdef _DEBUG
               if(zones[i] == nullZone)
                  PrintLn("WARNING: fromCentroid() returned null tile key");
   #endif
            }
            delete centroids;
            result = zones;
         }
      }
      return result;
   }

   int get64KDepth()
   {
      return (int)(log(65536) / log(getRefinementRatio()) + 0.5);
   }

   // for getZone*Centroids*(), getSubZones()
   int getMaxDepth()
   {
      int depth64k = get64KDepth();
      // Avoiding ISEA3H pentagon at 0,0 at level 1 and 2
      DGGRSZone testZone = getZoneFromWGS84Centroid(2, { 0, 10 });
      int maxDepth = 2 * depth64k - 1;
      uint64 nSubZones;

      while(((nSubZones = countSubZones(testZone, maxDepth) << 4) > (1LL<<32)))
         maxDepth--;
      return maxDepth;
   }

   // For getSubZoneIndex(), getSubZoneAtIndex()
   virtual int getIndexMaxDepth()
   {
      return getMaxDepth();
   }

   // Refinement Levels
   int getLevelFromRefZoneArea(double metersSquared)
   {
      int maxLevel = getMaxDGGRSZoneLevel() + get64KDepth();
      int level;
      double targetZoneCount;
      if(!earthArea) earthArea = wholeWorld.geodeticArea;
      targetZoneCount = earthArea / metersSquared;

      for(level = 0; level < maxLevel; level++)
      {
         double zoneCount = countZones(level);
         if(zoneCount >= targetZoneCount)
            return level;
      }
      return level;
   }
   double getRefZoneArea(int level) // In meters squared
   {
      int maxLevel = getMaxDGGRSZoneLevel();
      // Counting zones above maximum level will overflow 64-bit integers
      double zoneCount = countZones(Min(level, maxLevel));
      if(zoneCount)
      {
         if(!earthArea) earthArea = wholeWorld.geodeticArea;
         if(level < maxLevel)
            return earthArea / zoneCount;
         else
         {
            DGGRSZone testZone = getZoneFromWGS84Centroid(maxLevel, { 0, 10 });
            int64 nSubZones = countSubZones(testZone, level - maxLevel);
            return earthArea / (zoneCount * nSubZones);
         }
      }
      return 0;
   }

   int getLevelFromScaleDenominator(double scaleDenominator, int relativeDepth, double mmPerPixel) // defaults to 0.28 mm/pixel if 0
   {
      double displayMetersPerPixel = mmPerPixel ? mmPerPixel / 1000.0 : stdMetersPerPixel;
      double physicalMetersPerSubZone = scaleDenominator * displayMetersPerPixel;
      return Max(0, getLevelFromRefZoneArea(physicalMetersPerSubZone * physicalMetersPerSubZone) - relativeDepth);
   }

   double getScaleDenominatorFromLevel(int parentLevel, int relativeDepth, double mmPerPixel) // defaults to 0.28 mm/pixel if 0
   {
      double physicalMetersPerSubZone = sqrt(getRefZoneArea(parentLevel + relativeDepth));
      double displayMetersPerPixel = mmPerPixel ? mmPerPixel / 1000.0 : stdMetersPerPixel;
      return physicalMetersPerSubZone / displayMetersPerPixel;
   }

   int getLevelFromMetersPerSubZone(double physicalMetersPerSubZone, int relativeDepth)
   {
      return Max(0, getLevelFromRefZoneArea(physicalMetersPerSubZone * physicalMetersPerSubZone) - relativeDepth);
   }

   double getMetersPerSubZoneFromLevel(int parentLevel, int relativeDepth)
   {
      return sqrt(getRefZoneArea(parentLevel + relativeDepth));
   }

   int getLevelFromPixelsAndExtent(const GeoExtent extent, const Point pixels, int relativeDepth)
   {
      // REVIEW: Fix support for extent crossing dateline
      double diffLat = (double)(Degrees)(extent.ur.lat - extent.ll.lat);
      double diffLon = (double)(Degrees)(extent.ur.lon - extent.ll.lon);
      if(diffLat <= radEpsilon || diffLon <= radEpsilon)
      {
         int maxLevel = getMaxDGGRSZoneLevel() + get64KDepth();
         return Max(0, maxLevel - relativeDepth);
      }
      else
      {
         double latPixPerD = pixels.y  / diffLat;
         double lonPixPerD = pixels.x  / diffLon;
         return getLevelFromMetersPerSubZone(metersPerDegree / Max(latPixPerD, lonPixPerD), relativeDepth);
      }
   }

   private static bool isZoneAncestorOfWithTree(DGGRSZone ancestor, DGGRSZone descendant, int maxDepth, AVLTree<DGGRSZone> tree)
   {
      int aLevel = getZoneLevel(ancestor), dLevel = getZoneLevel(descendant);
      if(dLevel > aLevel && (!maxDepth || dLevel - aLevel <= maxDepth))
      {
         DGGRSZone parents[3];
         int nParents = getZoneParents(descendant, parents), i;
         for(i = 0; i < nParents; i++)
         {
            DGGRSZone parent = parents[i];
            if(parent == ancestor)
               return true;
            if(dLevel - aLevel > 1 && (!tree || !tree.Find(parent)))
            {
               if(tree) tree.Add(parent);
               if(isZoneAncestorOfWithTree(ancestor, parent, maxDepth ? maxDepth - 1 : 0, tree))
                  return true;
            }
         }
      }
      return false;
   }

   // Topological queries
   bool isZoneAncestorOf(DGGRSZone ancestor, DGGRSZone descendant, int maxDepth)
   {
      AVLTree<DGGRSZone> tree = getMaxParents() > 1 ? { } : null;
      bool result = isZoneAncestorOfWithTree(ancestor, descendant, maxDepth, tree);
      delete tree;
      return result;
   }

   bool areZonesSiblings(DGGRSZone a, DGGRSZone b)
   {
      int aLevel = getZoneLevel(a), bLevel = getZoneLevel(b);
      if(aLevel == bLevel && a != b)
      {
         DGGRSZone aParents[3], bParents[3];
         int nParentsA = getZoneParents(a, aParents), nParentsB = getZoneParents(b, bParents), i, j;
         for(i = 0; i < nParentsA; i++)
         {
            DGGRSZone pa = aParents[i];
            for(j = 0; j < nParentsB; j++)
            {
               if(bParents[j] == pa)
                  return true;
            }
         }
      }
      return false;
   }

   // Only considers neighbors of same level
   bool areZonesNeighbors(DGGRSZone a, DGGRSZone b)
   {
      int aLevel = getZoneLevel(a), bLevel = getZoneLevel(b);
      if(aLevel == bLevel && a != b)
      {
         DGGRSZone neighbors[6];
         int nNeighbors = getZoneNeighbors(a, neighbors, null), i;
         for(i = 0; i < nNeighbors; i++)
            if(neighbors[i] == b)
               return true;
      }
      return false;
   }

   bool isZoneDescendantOf(DGGRSZone descendant, DGGRSZone ancestor, int maxDepth)
   {
      return isZoneAncestorOf(ancestor, descendant, maxDepth);
   }

   bool isZoneImmediateParentOf(DGGRSZone parent, DGGRSZone child)
   {
      return isZoneAncestorOf(parent, child, 1);
   }

   bool isZoneImmediateChildOf(DGGRSZone child, DGGRSZone parent)
   {
      return isZoneAncestorOf(parent, child, 1);
   }

   bool doZonesOverlap(DGGRSZone a, DGGRSZone b)
   {
      bool result = false;
      int aLevel = getZoneLevel(a), bLevel = getZoneLevel(b);
      if(aLevel > bLevel)
         result = zoneHasSubZone(b, a);
      else if(aLevel < bLevel)
         result = zoneHasSubZone(a, b);
      return result;
   }

   bool doesZoneContain(DGGRSZone hayStack, DGGRSZone needle)
   {
      bool contains = false;
      if(zoneHasSubZone(hayStack, needle))
      {
         // For non-congruent grids: sub-zones are not contained if they are on the edge and overlap a neighbor
         DGGRSZone neighbors[6];
         int nNeighbors = getZoneNeighbors(hayStack, neighbors, null), i;
         for(i = 0; i < nNeighbors; i++)
            if(zoneHasSubZone(neighbors[i], needle))
               break;
         if(i == nNeighbors)
            contains = true;
      }
      return contains;
   }

   bool isZoneContainedIn(DGGRSZone needle, DGGRSZone hayStack)
   {
      return doesZoneContain(hayStack, needle);
   }

#ifdef USE_GEOGRAPHIC_LIB
   // DGGH
   private
   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   double computeGeodesisZoneArea(DGGRSZone zone)
   {
      // REVIEW: Is this Goldberg Polyhedra space exactly equal area?
      double area = 0, perimeter = 0;

      Array<Pointd> points = getZoneRefinedCRSVertices(zone, CRS { ogc, 84 }, 1024);
      int i;
      double * lats = new double[points.count];
      double * lons = new double[points.count];

      for(i = 0; i < points.count; i++)
         lats[i] = points[i].y, lons[i] = points[i].x;

      geod_polygonarea(&g, lats, lons, points.count, &area, &perimeter);
      if(area < 0)
         area = -area;   // FIXME: Polar zones are in opposite order
      delete points;
      delete lats;
      delete lons;

      PrintLn("Computed geodesic area: ", area);
      return area;
   }
#endif
}
