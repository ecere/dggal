public import IMPORT_STATIC "ecrt"
private:

#include <stdio.h>

import "ISEA7H"
import "IVEA7H"
import "RTEA7H"

static const int cMap   [7] = { 0, 3, 1, 5, 4, 6, 2 };
static const int invCMap[7] = { 0, 2, 6, 1, 4, 3, 5 };
static const int rootMap   [12] = { 1, 6, 2, 7, 3, 8, 4, 9, 5, 10, 0, 11 };
static const int invRootMap[12] = { 10, 0, 2, 4, 6, 8, 1, 3, 5, 7, 9, 11 };

public class Z7Zone : private DGGRSZone
{
public:
   uint rootPentagon:4:60;
   uint64 ancestry:60:0;

private:
   property Z7Zone centroidChild
   {
      get
      {
         if(this == nullZone || (ancestry & 7) != 7)
            return nullZone;
         else
         {
            uint64 ancestry = this.ancestry;
            int shift = 19 * 3;
            int l;

            for(l = 0; l < 20; l++, shift -= 3)
            {
               int b = (int)((ancestry & (7LL << shift)) >> shift);
               if(b == 7)
                  break;
            }
            return { rootPentagon, ancestry & ~(7LL << shift) };
         }
      }
   }

   property bool isCentroidChild
   {
      get
      {
         if(this == nullZone)
            return false;
         else
         {
            uint64 ancestry = this.ancestry;
            int shift = 19 * 3;
            int l, lastB = 7;

            for(l = 0; l < 20; l++, shift -= 3)
            {
               int b = (int)((ancestry & (7LL << shift)) >> shift);
               lastB = b;
               if(b == 7)
                  break;
            }
            return lastB == 0;
         }
      }
   }

   property int nPoints
   {
      get
      {
         if(this == nullZone)
            return 0;
         else
         {
            uint64 ancestry = this.ancestry;
            int shift = 19 * 3;
            int l;
            int nPoints = 5;

            for(l = 0; l < 20; l++, shift -= 3)
            {
               int b = (int)((ancestry & (7LL << shift)) >> shift);
               if(b == 7)
                  break;
               if(b != 0)
               {
                  nPoints = 6;
                  break;
               }
            }
            return nPoints;
         }
      }
   }

   private static int ::getChildPosition(I7HZone parent, I7HZone zone)
   {
      if(zone.level & 1)
         return zone.subHex - 1;
      else
      {
         I7HZone children[7];
         int nc = parent.getPrimaryChildren(children), i;

         for(i = 0; i < nc; i++)
            if(children[i] == zone)
               break;
         return i;
      }
   }

   private static int ::adjustZ7PentagonChildPosition(int i, int level, int pRoot)
   {
      if(i)
      {
         bool southPRhombus = pRoot & 1;
         bool oddLevel = level & 1;

         if(pRoot == 10) // North polar pentagons
            i = ((i + 1) % 5) + 1;
         else if(pRoot == 11) // South polar pentagons
            i = ((i + (oddLevel ? 3 : 4)) % 5) + 1;
         else if(!oddLevel && !southPRhombus) // Parent is an odd level northern non-polar pentagon
            i = ((i + 5) % 5) + 1;
         if(southPRhombus && i >= 3)
            i++;
      }
      return i;
   }

   private static int ::deadjustZ7PentagonChildPosition(int i, int level, int pRoot)
   {
      if(i)
      {
         bool southPRhombus = pRoot & 1;
         bool oddLevel = level & 1;

         if(southPRhombus && i >= 4)
            i--;

         if(pRoot == 10) // North polar pentagons
            i = ((i - 1 + 3) % 5) + 1;
         else if(pRoot == 11) // South polar pentagons
            i = ((i - 1 + (oddLevel ? 1 : 5)) % 5) + 1;
         else if(!oddLevel && !southPRhombus) // Parent is an odd level northern non-polar pentagon
            i = ((i - 1 + 4) % 5) + 1;
      }
      return i;
   }

   public int ::getParentRotationOffset(I7HZone zone)
   {
      I7HZone parents[19];
      computeParents(zone, parents);
      return getParentRotationOffsetInternal(zone, parents);
   }

   private static inline int ::getLevelRotationOffset(int l, int i, I7HZone zone, I7HZone parent, I7HZone grandParent)
   {
      int offset = 0;

      if(i == -1)
         i = getChildPosition(parent, zone);
      if(i)
      {
         uint pRoot = parent.rootRhombus;
         uint pnPoints = parent.nPoints;
         bool oddLevel = l & 1;
         bool southPRhombus = pRoot & 1;
         bool isEdgeHex = !oddLevel && zone.isEdgeHex;
         bool pEdgeHex = oddLevel && parent.isEdgeHex;
         bool gpEdgeHex = !oddLevel && grandParent.isEdgeHex;

         if(pnPoints == 5)
            i = adjustZ7PentagonChildPosition(i, l, pRoot);

         if(pRoot >= 10)
         {
            if(pnPoints == 5)
               offset += i + (oddLevel ? (southPRhombus ? 0 : 3) : (southPRhombus ? 5 : 2));
            else if(isEdgeHex && (!southPRhombus || zone != parent.centroidChild))
               offset += 5;
         }

         if(southPRhombus && isEdgeHex)
            offset++;
         if(pEdgeHex)
         {
            // This rule is necessary starting from Level 4
            if(!southPRhombus && i >= 4)
               offset++;
            else if(southPRhombus && (i == 0 || (i >= 3 && i <= 5)))
               offset += 5;
         }
         else if(gpEdgeHex)
         {
            I7HZone c[7], pc[7];
            grandParent.getPrimaryChildren(pc);
            parent.getPrimaryChildren(c);
            if(southPRhombus ?
               pc[1] == parent && c[2].rootRhombus != c[5].rootRhombus && (i == 4 || i == 5) :
               pc[4] == parent && (i == 1 || i == 2)) // Root rhombuses are the same for northern case
               offset += 5;

            if(parent == grandParent.centroidChild)
            {
               if(southPRhombus)
               {
                  if(i > 2)
                     offset += 5;
               }
               else
               {
                  if(i == 5 || i == 6)
                     offset++;
               }
            }
            if(southPRhombus && isEdgeHex)
            {
               if(i == 4 || i == 5)
                  offset += 5;
            }
         }
      }
      return offset;
   }

   private static int ::getParentRotationOffsetInternal(I7HZone zone, const I7HZone * parents)
   {
      int offset = 0;
      int level = zone.level, l = level;
      int pIndex = 0;
      I7HZone parent = l > 0 ? parents[pIndex] : nullZone;

      while(l > 0)
      {
         I7HZone grandParent = l > 1 ? parents[pIndex + 1] : nullZone;
         offset += getLevelRotationOffset(l, -1, zone, parent, grandParent);
         offset %= 6;
         zone = parent;
         parent = grandParent;
         pIndex++;
         l--;
      }
      return offset;
   }

   private static int ::computeParents(I7HZone zone, I7HZone parents[19])
   {
      int level = zone.level, l = level, pIndex = 0;
      while(l > 0)
      {
         parents[pIndex] = (l == level ? zone : parents[pIndex-1]).parent0;
         pIndex++;
         l--;
      }
      return pIndex;
   }

   public I7HZone to7H()
   {
      I7HZone zone = nullZone;
      if(this != nullZone && rootPentagon < 12)
      {
         int level;
         I7HZone parents[19];
         int offset = 0;
         uint64 ancestry = this.ancestry;
         int shift = 19 * 3;
         int prevCIX = 0;

         if((this & 7) != 7)
            return nullZone; // I7HZone are only valid up to level 19

         zone = { 0, invRootMap[rootPentagon], 0, 0 };

         for(level = 0; level < 20; level++, shift -= 3)
         {
            int pStart = 18 - level;
            int nPoints = zone.nPoints;
            int b = (int)((ancestry & (7LL << shift)) >> shift);

            parents[pStart] = zone;

            if(b == 7)
               break;
            else
            {
               int cix = invCMap[b];

               if(cix || level < 19)
                  offset = (offset + getLevelRotationOffset(level, prevCIX,
                     zone,
                     level > 0 ? parents[pStart + 1] : nullZone,
                     level > 1 ? parents[pStart + 2] : nullZone)
                     ) % 6;
               if(cix)
               {
                  cix = cix - 1 - offset;
                  if(cix < 0)
                     cix += 6;
                  cix++;
                  if(nPoints == 5)
                     cix = deadjustZ7PentagonChildPosition(cix, level + 1, zone.rootRhombus);
               }
               prevCIX = cix;

               if(!(level & 1))
                  zone = { zone.levelI49R, zone.rootRhombus, zone.rhombusIX, 1 + cix };
               else if(level == 19)
               {
                  // 7H does not support level 20 zones
                  zone = nullZone;
                  break;
               }
               else
               {
                  I7HZone children[7];
                  int n = zone.getPrimaryChildren(children);
                  if(cix < n)
                     zone = children[cix];
                  else
                  {
                     zone = nullZone;
                     break;
                  }
               }
            }
         }
      }
      return zone;
   }

   public Z7Zone ::from7H(I7HZone zone)
   {
      Z7Zone result = nullZone;
      if(zone != nullZone)
      {
         int level = zone.level;
         I7HZone parents[19];
         uint64 ancestry = 0;
         int shift, pIndex, l;
         int offset = 0;
         int prevI = 0;

         if(level > 19) return nullZone; // This should never happen

         computeParents(zone, parents);
         for(l = 1, pIndex = level-1, shift = 3 * 19; l <= level; l++, pIndex--, shift -= 3)
         {
            I7HZone z = l == level ? zone : parents[pIndex - 1];
            I7HZone parent = parents[pIndex];
            I7HZone grandParent = l > 1 ? parents[pIndex + 1] : nullZone;
            int i = getChildPosition(parent, z);

            offset = (offset + getLevelRotationOffset(l-1, prevI, parent, grandParent, l > 2 ? parents[pIndex + 2] : nullZone)) % 6;
            prevI = i;

            if(i)
            {
               //int fullOffset = getParentRotationOffsetInternal(parent, parents + pIndex + 1);
               if(parent.nPoints == 5)
                  i = adjustZ7PentagonChildPosition(i, l, parent.rootRhombus);
               i = ((i - 1) + offset) % 6 + 1;
            }

            ancestry |= ((int64)cMap[i] << shift);
         }
         while(shift >= 0)
         {
            ancestry |= ((int64)7LL << shift);
            shift -= 3;
         }
         result.rootPentagon = rootMap[(level == 0 ? zone : parents[level-1]).rootRhombus];
         result.ancestry = ancestry;
      }
      return result;
   }

   public Z7Zone ::fromText(const String zoneID)
   {
      Z7Zone zone = nullZone;

      int len = zoneID ? strlen(zoneID) : 0;
      if(len >= 2 && len <= 22)
      {
         int root;
         int r = sscanf(zoneID, "%2d", &root);

         if(r && root >= 0 && root <= 11)
         {
            int i;
            uint64 ancestry = 0;
            int shift = 3 * 19;
            zone = { root };
            for(i = 2; i < len; i++)
            {
               char c = zoneID[i];
               if(c < '0' || c > '6')
               {
                  zone = nullZone;
                  break;
               }
               else
                  ancestry |= (uint64)(c - '0') << shift;
               shift -= 3;
            }
            while(shift >= 0)
            {
               ancestry |= ((int64)7LL << shift);
               shift -= 3;
            }
            if(zone != nullZone)
               zone.ancestry = ancestry;
         }
      }
      return zone;
   }

   property int level
   {
      get
      {
         if(this == nullZone)
            return -1;
         else
         {
            uint64 ancestry = this.ancestry;
            int shift = 19 * 3;
            int l;
            int level = 0;

            for(l = 0; l < 20; l++, shift -= 3)
            {
               int b = (int)((ancestry & (7LL << shift)) >> shift);
               if(b == 7)
                  break;
               level++;
            }
            return level;
         }
      }
   }

   public void getTextID(String zoneID)
   {
      if(this == nullZone)
         strcpy(zoneID, "(null)");
      else
      {
         uint64 ancestry = this.ancestry;
         int shift = 19 * 3;
         int l;

         sprintf(zoneID, "%02d", rootPentagon);

         for(l = 0; l < 20; l++, shift -= 3)
         {
            int b = (int)((ancestry & (7LL << shift)) >> shift);
            if(b == 7)
               break;
            zoneID[2 + l] = (byte)('0' + b);
         }
         zoneID[2 + l] = 0;
      }
   }
}

static define POW_EPSILON = 0.1;

#define POW7(x) ((x) < sizeof(powersOf7) / sizeof(powersOf7[0]) ? (uint64)powersOf7[x] : (uint64)(pow(7, x) + POW_EPSILON))

public class Z7 : RhombicIcosahedral7H
{
   uint64 countSubZones(Z7Zone zone, int rDepth)
   {
      if(rDepth > 0)
      {
         int64 nHexSubZones = POW7(rDepth) + ((rDepth & 1) ? 5 * POW7((rDepth-1)/2) + 1 : POW7(rDepth/2) - 1);
         return (nHexSubZones * zone.nPoints + 5) / 6;
      }
      return 1;
   }

   int getZoneLevel(Z7Zone zone)
   {
      return zone.level;
   }

   int countZoneEdges(Z7Zone zone) { return zone.nPoints; }

   bool isZoneCentroidChild(Z7Zone zone)
   {
      return zone.isCentroidChild;
   }

   double getZoneArea(Z7Zone zone)
   {
      double area = 0;
      if(equalArea)
      {
         uint64 zoneCount = countZones(zone.level);
         static double earthArea = 0;
         if(!earthArea) earthArea = wholeWorld.geodeticArea;
         area = earthArea / (zoneCount - 2) * (zone.nPoints == 5 ? 5/6.0 : 1);
      }
      return area;
   }

   Z7Zone getZoneFromCRSCentroid(int level, CRS crs, const Pointd centroid)
   {
      return Z7Zone::from7H((I7HZone)RhombicIcosahedral7H::getZoneFromCRSCentroid(level, crs, centroid));
   }

   int getZoneNeighbors(Z7Zone zone, Z7Zone * neighbors, I7HNeighbor * nbType)
   {
      int n = RhombicIcosahedral7H::getZoneNeighbors(zone.to7H(), neighbors, nbType), i;
      for(i = 0; i < n; i++)
         neighbors[i] = Z7Zone::from7H((I7HZone)neighbors[i]);
      return n;
   }

   Z7Zone getZoneCentroidParent(Z7Zone zone)
   {
      return Z7Zone::from7H((I7HZone)RhombicIcosahedral7H::getZoneCentroidParent(zone.to7H()));
   }

   Z7Zone getZoneCentroidChild(Z7Zone zone)
   {
      return zone.centroidChild;
   }

   int getZoneParents(Z7Zone zone, Z7Zone * parents)
   {
      int n = RhombicIcosahedral7H::getZoneParents(zone.to7H(), parents), i;
      for(i = 0; i < n; i++)
         parents[i] = Z7Zone::from7H((I7HZone)parents[i]);
      return n;
   }

   int getZoneChildren(Z7Zone zone, Z7Zone * children)
   {
      int n = RhombicIcosahedral7H::getZoneChildren(zone.to7H(), children), i;
      for(i = 0; i < n; i++)
         children[i] = Z7Zone::from7H((I7HZone)children[i]);
      return n;
   }

   void getZoneTextID(Z7Zone zone, String zoneID)
   {
      zone.getTextID(zoneID);
   }

   Z7Zone getZoneFromTextID(const String zoneID)
   {
      return Z7Zone::fromText(zoneID);
   }

   Z7Zone getFirstSubZone(Z7Zone zone, int depth)
   {
      return Z7Zone::from7H((I7HZone)RhombicIcosahedral7H::getFirstSubZone(zone.to7H(), depth));
   }

   void compactZones(Array<DGGRSZone> zones)
   {
      if(zones)
      {
         int i, count = zones.count;

         for(i = 0; i < count; i++)
            zones[i] = ((Z7Zone)zones[i]).to7H();

         RhombicIcosahedral7H::compactZones(zones);

         count = zones.count;

         for(i = 0; i < count; i++)
            zones[i] = Z7Zone::from7H((I7HZone)zones[i]);
      }
   }

   int64 getSubZoneIndex(Z7Zone parent, Z7Zone subZone)
   {
      return RhombicIcosahedral7H::getSubZoneIndex(parent.to7H(), subZone.to7H());
   }

   Z7Zone getSubZoneAtIndex(Z7Zone parent, int relativeDepth, int64 index)
   {
      return Z7Zone::from7H((I7HZone)RhombicIcosahedral7H::getSubZoneAtIndex(parent.to7H(), relativeDepth, index));
   }

   bool zoneHasSubZone(Z7Zone hayStack, Z7Zone needle)
   {
      return RhombicIcosahedral7H::zoneHasSubZone(hayStack.to7H(), needle.to7H());
   }

   Z7Zone getZoneFromWGS84Centroid(int level, const GeoPoint centroid)
   {
      return Z7Zone::from7H((I7HZone)RhombicIcosahedral7H::getZoneFromWGS84Centroid(level, centroid));
   }

   void getZoneCRSCentroid(Z7Zone zone, CRS crs, Pointd centroid)
   {
      RhombicIcosahedral7H::getZoneCRSCentroid(zone.to7H(), crs, centroid);
   }

   void getZoneWGS84Centroid(Z7Zone zone, GeoPoint centroid)
   {
      RhombicIcosahedral7H::getZoneWGS84Centroid(zone.to7H(), centroid);
   }

   void getZoneCRSExtent(Z7Zone zone, CRS crs, CRSExtent extent)
   {
      RhombicIcosahedral7H::getZoneCRSExtent(zone.to7H(), crs, extent);
   }

   void getZoneWGS84Extent(Z7Zone zone, GeoExtent extent)
   {
      RhombicIcosahedral7H::getZoneWGS84Extent(zone.to7H(), extent);
   }

   int getZoneCRSVertices(Z7Zone zone, CRS crs, Pointd * vertices)
   {
      return RhombicIcosahedral7H::getZoneCRSVertices(zone.to7H(), crs, vertices);
   }

   int getZoneWGS84Vertices(Z7Zone zone, GeoPoint * vertices)
   {
      return RhombicIcosahedral7H::getZoneWGS84Vertices(zone.to7H(), vertices);
   }

   Array<Pointd> getZoneRefinedCRSVertices(Z7Zone zone, CRS crs, int edgeRefinement)
   {
      return RhombicIcosahedral7H::getZoneRefinedCRSVertices(zone.to7H(), crs, edgeRefinement);
   }

   Array<GeoPoint> getZoneRefinedWGS84Vertices(Z7Zone zone, int edgeRefinement)
   {
      return RhombicIcosahedral7H::getZoneRefinedWGS84Vertices(zone.to7H(), edgeRefinement);
   }

   void getApproxWGS84Extent(Z7Zone zone, GeoExtent extent)
   {
      return RhombicIcosahedral7H::getApproxWGS84Extent(zone.to7H(), extent);
   }

   Array<Pointd> getSubZoneCRSCentroids(Z7Zone parent, CRS crs, int depth)
   {
      return RhombicIcosahedral7H::getSubZoneCRSCentroids(parent.to7H(), crs, depth);
   }

   Array<GeoPoint> getSubZoneWGS84Centroids(Z7Zone parent, int depth)
   {
      return RhombicIcosahedral7H::getSubZoneWGS84Centroids(parent.to7H(), depth);
   }

   static Array<DGGRSZone> listZones(int zoneLevel, const GeoExtent bbox)
   {
      Array<DGGRSZone> zones = RhombicIcosahedral7H::listZones(zoneLevel, bbox);
      if(zones)
      {
         int i;
         for(i = 0; i < zones.count; i++)
            zones[i] = Z7Zone::from7H((I7HZone)zones[i]);
      }
      return zones;
   }
}

#if 1
// These DGGRSs natively use Z7Zone for DGGRSZone, at the cost of some performance impact

public class ISEA7H_Z7 : Z7
{
   equalArea = true;

   ISEA7H_Z7() { pj = ISEAProjection { }; incref pj; }
   ~ISEA7H_Z7() { delete pj; }
}

public class IVEA7H_Z7 : Z7
{
   equalArea = true;

   IVEA7H_Z7() { pj = SliceAndDiceGreatCircleIcosahedralProjection { }; incref pj; }
   ~IVEA7H_Z7() { delete pj; }
}

public class RTEA7H_Z7 : Z7
{
   equalArea = true;

   RTEA7H_Z7() { pj = RTEAProjection { }; incref pj; }
   ~RTEA7H_Z7() { delete pj; }
}

#else

// To still use using I7HZone for 64-bit integer DGGRSZone...
public class ISEA7H_Z7 : ISEA7H
{
   I7HZone getZoneFromTextID(const String zoneID)
   {
      return Z7Zone::fromText(zoneID).to7H();
   }

   void getZoneTextID(I7HZone zone, String zoneID)
   {
      Z7Zone::from7H(zone).getTextID(zoneID);
   }
}

public class IVEA7H_Z7 : IVEA7H
{
   I7HZone getZoneFromTextID(const String zoneID)
   {
      return Z7Zone::fromText(zoneID).to7H();
   }

   void getZoneTextID(I7HZone zone, String zoneID)
   {
      Z7Zone::from7H(zone).getTextID(zoneID);
   }
}

public class RTEA7H_Z7 : RTEA7H
{
   I7HZone getZoneFromTextID(const String zoneID)
   {
      return Z7Zone::fromText(zoneID).to7H();
   }

   void getZoneTextID(I7HZone zone, String zoneID)
   {
      Z7Zone::from7H(zone).getTextID(zoneID);
   }
}
#endif
