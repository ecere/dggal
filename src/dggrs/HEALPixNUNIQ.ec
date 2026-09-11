
public import IMPORT_STATIC "ecrt"
private:

import "HEALPixGrid"

#include <stdio.h>

default extern int __builtin_clzll(uint64 val);

public class HPNUNIQZone : private DGGRSZone
{
public:
   uint64 nuniq:64:0;

   property int level
   {
      get
      {
         uint64 val = nuniq;
         if(val >= 4)
         {
            // Instantly find the active level tier using a single hardware instruction
            int highestBit = 63 - __builtin_clzll(val);
            return (highestBit - 2) >> 1;
         }
         return -1;
      }
   }

   property HPZone hpZone
   {
      get
      {
         uint64 val = nuniq;
         if(val >= 4)
         {
            int highestBit = 63 - __builtin_clzll(val);
            int lvl = (highestBit - 2) >> 1;

            uint64 levelOffset = 1LL << (2 * lvl);
            uint64 baseIpix = val - (levelOffset << 2);
            uint rRhombus = (uint)(baseIpix >> (2 * lvl));
            uint64 sIndex = baseIpix - ((uint64)rRhombus << (2 * lvl));

            if(rRhombus < 4)        rRhombus = (rRhombus + 2) % 4;
            else if(rRhombus < 8)   rRhombus = 4 + ((rRhombus - 4 + 2) % 4);
            else                    rRhombus = 8 + ((rRhombus - 8 + 2) % 4);

            // De-interleave NUNIQ Z-curve bits directly into HPZone row/col blocks
            if(lvl > 0)
            {
               uint64 row = 0;
               uint64 col = 0;
               int step;
               for(step = 0; step < lvl; step++)
               {
                  // Extract the 2-bit quadrant token from coarsest to finest
                  uint64 quad = (sIndex >> (2 * (lvl - 1 - step))) & 3;

                  // Translate quad bits back to row and col bits using the true spec mapping
                  uint64 cBit = quad & 1;
                  uint64 rBit = ((quad >> 1) & 1) ^ 1;

                  row = (row << 1) | rBit;
                  col = (col << 1) | cBit;
               }
               // Re-pack into native HPZone concatenation format
               sIndex = (row << lvl) | col;
            }

            return HPZone { lvl, rRhombus, sIndex };
         }
         return nullZone;
      }
   }

   HPNUNIQZone ::fromHPZone(HPZone zone)
   {
      int lvl = zone.level;
      uint rRhombus = zone.rootRhombus;
      uint64 sIndex = zone.subIndex;

      if(rRhombus < 4)        rRhombus = (rRhombus + 2) % 4;
      else if(rRhombus < 8)   rRhombus = 4 + ((rRhombus - 4 + 2) % 4);
      else                    rRhombus = 8 + ((rRhombus - 8 + 2) % 4);

      // Interleave row/col bits into NUNIQ Z-curve format from coarsest to finest
      if(lvl > 0)
      {
         uint64 row = sIndex >> lvl;
         uint64 col = sIndex & ((1LL << lvl) - 1);
         uint64 cleanIndex = 0;
         int step;

         for(step = 0; step < lvl; step++)
         {
            uint64 rBit = (row >> (lvl - 1 - step)) & 1;
            uint64 cBit = (col >> (lvl - 1 - step)) & 1;

            // Encode row and col bits directly to the true standard quad position
            uint64 quad = ((rBit ^ 1) << 1) | cBit;
            cleanIndex = (cleanIndex << 2) | quad;
         }
         sIndex = cleanIndex;
      }

      return HPNUNIQZone { nuniq = ((1LL << (2 * lvl)) << 2) + (((uint64)rRhombus << (2 * lvl)) + sIndex) };
   }
}

public class HEALPixNUNIQ : HEALPix
{
   public uint64 countSubZones(DGGRSZone zone, int rDepth)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      return HEALPix::countSubZones(nuZone.hpZone, rDepth);
   }

   public int getZoneLevel(DGGRSZone zone)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      return nuZone.level;
   }

   public int countZoneEdges(DGGRSZone zone)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      return HEALPix::countZoneEdges(nuZone.hpZone);
   }

   public bool isZoneCentroidChild(DGGRSZone zone)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      return HEALPix::isZoneCentroidChild(nuZone.hpZone);
   }

   public double getZoneArea(DGGRSZone zone)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      return HEALPix::getZoneArea(nuZone.hpZone);
   }

   public DGGRSZone getZoneFromCRSCentroid(int level, CRS crs, const Pointd centroid)
   {
      HPZone nativeZone = (HPZone)HEALPix::getZoneFromCRSCentroid(level, crs, centroid);
      if(nativeZone != nullZone)
         return HPNUNIQZone::fromHPZone(nativeZone);
      return nullZone;
   }

   public int getZoneNeighbors(DGGRSZone zone, DGGRSZone * neighbors, int * nbType)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      HPZone nativeNeighbors[10];
      int count = HEALPix::getZoneNeighbors(nuZone.hpZone, (DGGRSZone *)nativeNeighbors, nbType);
      if(count > 0)
      {
         int i;
         for(i = 0; i < count; i++)
            neighbors[i] = HPNUNIQZone::fromHPZone(nativeNeighbors[i]);
      }
      return count;
   }

   public DGGRSZone getZoneCentroidParent(DGGRSZone zone)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      HPZone nativeZone = (HPZone)HEALPix::getZoneCentroidParent(nuZone.hpZone);
      if(nativeZone != nullZone)
         return HPNUNIQZone::fromHPZone(nativeZone);
      return nullZone;
   }

   public DGGRSZone getZoneCentroidChild(DGGRSZone zone)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      HPZone nativeZone = (HPZone)HEALPix::getZoneCentroidChild(nuZone.hpZone);
      if(nativeZone != nullZone)
         return HPNUNIQZone::fromHPZone(nativeZone);
      return nullZone;
   }

   public int getZoneParents(DGGRSZone zone, DGGRSZone * parents)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      HPZone nativeParents[10];
      int count = HEALPix::getZoneParents(nuZone.hpZone, (DGGRSZone *)nativeParents);
      if(count > 0)
      {
         int i;
         for(i = 0; i < count; i++)
            parents[i] = HPNUNIQZone::fromHPZone(nativeParents[i]);
      }
      return count;
   }

   public DGGRSZone getZonePrimaryParent(DGGRSZone zone)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      HPZone nativeZone = (HPZone)HEALPix::getZonePrimaryParent(nuZone.hpZone);
      if(nativeZone != nullZone)
         HPNUNIQZone::fromHPZone(nativeZone);
      return nullZone;
   }

   public int getZoneChildren(DGGRSZone zone, DGGRSZone * children)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      HPZone nativeChildren[10];
      int count = HEALPix::getZoneChildren(nuZone.hpZone, (DGGRSZone *)nativeChildren);
      if(count > 0)
      {
         int i;
         for(i = 0; i < count; i++)
            children[i] = HPNUNIQZone::fromHPZone(nativeChildren[i]);
      }
      return count;
   }

   public void getZoneTextID(DGGRSZone zone, String zoneID)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      sprintf(zoneID, FORMAT64U, nuZone.nuniq);
   }

   public DGGRSZone getZoneFromTextID(const String zoneID)
   {
      uint64 val = 0;
      if(zoneID && sscanf(zoneID, FORMAT64U, &val) == 1)
      {
         return (DGGRSZone)HPNUNIQZone { nuniq = val };
      }
      return nullZone;
   }

   public DGGRSZone getFirstSubZone(DGGRSZone zone, int depth)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      int pLvl = nuZone.level;
      int targetLvl = pLvl + depth;

      if(targetLvl >= pLvl && targetLvl < 30)
      {
         uint64 pOffset = 1LL << (2 * pLvl);
         uint64 targetOffset = 1LL << (2 * targetLvl);

         uint64 pPath = nuZone.nuniq - (pOffset << 2);
         uint rRhombus = (uint)(pPath >> (2 * pLvl));
         uint64 pIndex = pPath & (pOffset - 1);

         // The first sub-zone under a Z-curve path simply appends trailing zero bits
         uint64 targetIndex = pIndex << (2 * depth);
         uint64 uniqVal = (targetOffset << 2) + ((uint64)rRhombus << (2 * targetLvl)) + targetIndex;

         return (DGGRSZone)HPNUNIQZone { nuniq = uniqVal };
      }
      return nullZone;
   }

   public bool zoneHasSubZone(DGGRSZone hayStack, DGGRSZone needle)
   {
      HPNUNIQZone nuHay = (HPNUNIQZone)hayStack;
      HPNUNIQZone nuNeedle = (HPNUNIQZone)needle;

      int pLvl = nuHay.level;
      int sLvl = nuNeedle.level;

      // A parent cannot contain a cell that sits at a coarser resolution tier
      if(sLvl >= pLvl)
      {
         uint64 pOffset = 1LL << (2 * pLvl);
         uint64 sOffset = 1LL << (2 * sLvl);

         uint64 pPath = nuHay.nuniq - (pOffset << 2);
         uint64 sPath = nuNeedle.nuniq - (sOffset << 2);

         uint pRhombus = (uint)(pPath >> (2 * pLvl));
         uint sRhombus = (uint)(sPath >> (2 * sLvl));

         // 1. Verify they belong to the exact same base face diamond family branch
         if(pRhombus == sRhombus)
         {
            uint64 pIndex = pPath & (pOffset - 1);
            uint64 sIndex = sPath & (sOffset - 1);

            // 2. Clear out the child depth bits to see if the leading quadtree path matches the parent index
            uint64 shiftedSubIndex = sIndex >> (2 * (sLvl - pLvl));
            return (pIndex == shiftedSubIndex);
         }
      }
      return false;
   }

   public void compactZones(Array<DGGRSZone> zones)
   {
      if(zones)
      {
         int i, count = zones.size;
         Array<DGGRSZone> nativeZones { size = count };
         for(i = 0; i < count; i++)
         {
            HPNUNIQZone nuZone = (HPNUNIQZone)zones[i];
            nativeZones[i] = nuZone.hpZone;
         }
         HEALPix::compactZones(nativeZones);
         for(i = 0; i < count; i++)
         {
            HPZone nativeZone = (HPZone)nativeZones[i];
            if(nativeZone != nullZone)
               zones[i] = HPNUNIQZone::fromHPZone(nativeZone);
            else
               zones[i] = nullZone;
         }
         delete nativeZones;
      }
   }

   public DGGRSZone getZoneFromWGS84Centroid(int level, const GeoPoint centroid)
   {
      HPZone nativeZone = (HPZone)HEALPix::getZoneFromWGS84Centroid(level, centroid);
      if(nativeZone != nullZone)
         return HPNUNIQZone::fromHPZone(nativeZone);
      return nullZone;
   }

   public void getZoneCRSCentroid(DGGRSZone zone, CRS crs, Pointd centroid)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      HEALPix::getZoneCRSCentroid(nuZone.hpZone, crs, centroid);
   }

   public void getZoneWGS84Centroid(DGGRSZone zone, GeoPoint centroid)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      HEALPix::getZoneWGS84Centroid(nuZone.hpZone, centroid);
   }

   public void getZoneCRSExtent(DGGRSZone zone, CRS crs, CRSExtent extent)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      HEALPix::getZoneCRSExtent(nuZone.hpZone, crs, extent);
   }

   public void getZoneWGS84Extent(DGGRSZone zone, GeoExtent extent)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      HEALPix::getZoneWGS84Extent(nuZone.hpZone, extent);
   }

   public int getZoneCRSVertices(DGGRSZone zone, CRS crs, Pointd * vertices)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      return HEALPix::getZoneCRSVertices(nuZone.hpZone, crs, vertices);
   }

   public int getZoneWGS84Vertices(DGGRSZone zone, GeoPoint * vertices)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      return HEALPix::getZoneWGS84Vertices(nuZone.hpZone, vertices);
   }

   public Array<Pointd> getZoneRefinedCRSVertices(DGGRSZone zone, CRS crs, int edgeRefinement)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      return HEALPix::getZoneRefinedCRSVertices(nuZone.hpZone, crs, edgeRefinement);
   }

   public Array<GeoPoint> getZoneRefinedWGS84Vertices(DGGRSZone zone, int edgeRefinement)
   {
      HPNUNIQZone nuZone = (HPNUNIQZone)zone;
      return HEALPix::getZoneRefinedWGS84Vertices(nuZone.hpZone, edgeRefinement);
   }


   public Array<DGGRSZone> listZones(int level, const GeoExtent bbox)
   {
      Array<DGGRSZone> nativeZones = HEALPix::listZones(level, bbox);
      if(nativeZones)
      {
         int i, count = nativeZones.size;
         Array<DGGRSZone> uniqZones { size = count };

         for(i = 0; i < count; i++)
         {
            HPZone nativeZone = (HPZone)nativeZones[i];
            if(nativeZone != nullZone)
               uniqZones[i] = HPNUNIQZone::fromHPZone(nativeZone);
            else
               uniqZones[i] = nullZone;
         }
         delete nativeZones;
         return uniqZones;
      }
      return null;
   }

   public int64 getSubZoneIndex(DGGRSZone parent, DGGRSZone subZone)
   {
      HPNUNIQZone nuParent = (HPNUNIQZone)parent;
      HPNUNIQZone nuSub = (HPNUNIQZone)subZone;

      int pLvl = nuParent.level;
      int sLvl = nuSub.level;

      if(sLvl >= pLvl)
      {
         uint64 pOffset = 1LL << (2 * pLvl);
         uint64 sOffset = 1LL << (2 * sLvl);

         // Isolate the pure interleaved quadtree path segment from the NUNIQ values
         uint64 pPath = nuParent.nuniq - (pOffset << 2);
         uint64 sPath = nuSub.nuniq - (sOffset << 2);

         // Remove the base face multiplier bits to isolate the local sub-grid layout
         uint64 pIndex = pPath & (pOffset - 1);
         uint64 sIndex = sPath & (sOffset - 1);

         // The relative space-filling index is exactly the shifted remainder
         return (int64)(sIndex - (pIndex << (2 * (sLvl - pLvl))));
      }
      return -1;
   }

   public DGGRSZone getSubZoneAtIndex(DGGRSZone parent, int relativeDepth, int64 index)
   {
      HPNUNIQZone nuParent = (HPNUNIQZone)parent;
      int pLvl = nuParent.level;
      int targetLvl = pLvl + relativeDepth;

      if(targetLvl >= pLvl && targetLvl < 30)
      {
         uint64 pOffset = 1LL << (2 * pLvl);
         uint64 targetOffset = 1LL << (2 * targetLvl);

         uint64 pPath = nuParent.nuniq - (pOffset << 2);
         uint rRhombus = (uint)(pPath >> (2 * pLvl));
         uint64 pIndex = pPath & (pOffset - 1);

         // Append the relative index bits directly behind the parent's Z-curve path
         uint64 targetIndex = (pIndex << (2 * relativeDepth)) | (uint64)index;
         uint64 uniqVal = (targetOffset << 2) + ((uint64)rRhombus << (2 * targetLvl)) + targetIndex;

         return (DGGRSZone)HPNUNIQZone { nuniq = uniqVal };
      }
      return nullZone;
   }

   public Array<DGGRSZone> getSubZones(DGGRSZone parent, int relativeDepth)
   {
      HPNUNIQZone nuParent = (HPNUNIQZone)parent;
      int pLvl = nuParent.level;
      int targetLvl = pLvl + relativeDepth;

      if(targetLvl >= pLvl && targetLvl < 30)
      {
         int64 count = 1LL << (2 * relativeDepth);
         Array<DGGRSZone> subZones { size = (int)count };

         uint64 pOffset = 1LL << (2 * pLvl);
         uint64 targetOffset = 1LL << (2 * targetLvl);

         uint64 pPath = nuParent.nuniq - (pOffset << 2);
         uint rRhombus = (uint)(pPath >> (2 * pLvl));
         uint64 pIndex = pPath & (pOffset - 1);

         uint64 baseIndex = pIndex << (2 * relativeDepth);
         uint64 baseUniq = (targetOffset << 2) + ((uint64)rRhombus << (2 * targetLvl));
         int i;

         for(i = 0; i < count; i++)
            subZones[i] = HPNUNIQZone { nuniq = baseUniq + baseIndex + i };
         return subZones;
      }
      return null;
   }

   public Array<Pointd> getSubZoneCRSCentroids(DGGRSZone parent, CRS crs, int depth)
   {
      Array<DGGRSZone> subZones = getSubZones(parent, depth);
      if(subZones)
      {
         int i, count = subZones.size;
         Array<Pointd> centroids { size = count };
         for(i = 0; i < count; i++)
            HEALPix::getZoneCRSCentroid(((HPNUNIQZone)subZones[i]).hpZone, crs, centroids[i]);
         delete subZones;
         return centroids;
      }
      return null;
   }

   Array<GeoPoint> getSubZoneWGS84Centroids(DGGRSZone parent, int depth)
   {
      Array<DGGRSZone> subZones = getSubZones(parent, depth);
      if(subZones)
      {
         int i, count = subZones.size;
         Array<GeoPoint> centroids { size = count };
         for(i = 0; i < count; i++)
            HEALPix::getZoneWGS84Centroid(((HPNUNIQZone)subZones[i]).hpZone, centroids[i]);
         delete subZones;
         return centroids;
      }
      return null;
   }
}
