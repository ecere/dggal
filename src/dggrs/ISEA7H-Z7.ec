public import IMPORT_STATIC "ecrt"
private:

import "ISEA7H"

public class ISEA7HZ7 : ISEA7H
{
   equalArea = true;

   ISEA7HZ7() { pj = ISEAProjection { }; incref pj; }
   ~ISEA7HZ7() { delete pj; }

   // For now using I7HZone for 64-bit integer...
/*
   DGGRSZone getZoneFromTextID(const String zoneID)
   {
      return nullZone;
   }
*/

   int getCorrectedChildPosition(I7HZone parent, I7HZone grandParent, I7HZone zone)
   {
      int i;
      bool southPRhombus = parent.rootRhombus & 1;
      bool oddLevel = zone.level & 1;

      if(oddLevel)
         i = zone.subHex - 1;
      else
      {
         I7HZone children[7];
         int nc = parent.getPrimaryChildren(children);

         for(i = 0; i < nc; i++)
            if(children[i] == zone)
               break;

         if(i && parent.nPoints == 6 && parent == grandParent.centroidChild && grandParent.isEdgeHex)
         { // This is the second 7H children ordering issue, but not reflected in the indexing (all -A for even levels)
            I7HZone c[7];
            parent.getPrimaryChildren(c);
            if(c[2].rootRhombus != c[5].rootRhombus)
               i = (i - 1 + (southPRhombus ? 5 : 1)) % 6 + 1;
         }
      }
      return i;
   }

   int adjustZ7PentagonChildPosition(int i, int level, int pRoot)
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

   public int getParentRotationOffset(I7HZone zone)
   {
      I7HZone parents[19];
      computeParents(zone, parents);
      return getParentRotationOffsetInternal(zone, parents);
   }

   int getParentRotationOffsetInternal(I7HZone zone, const I7HZone * parents)
   {
      int offset = 0;
      int level = zone.level, l = level;
      int pIndex = 0;
      I7HZone parent = l > 0 ? parents[pIndex] : nullZone;

      while(l > 0)
      {
         I7HZone grandParent = l > 1 ? parents[pIndex + 1] : nullZone;
         uint pRoot = parent.rootRhombus;
         uint pnPoints = parent.nPoints;
         bool oddLevel = l & 1;
         bool southPRhombus = pRoot & 1;
         bool isEdgeHex = !oddLevel && zone.isEdgeHex;
         bool pEdgeHex = oddLevel && parent.isEdgeHex;
         bool gpEdgeHex = !oddLevel && grandParent.isEdgeHex;
         int i = getCorrectedChildPosition(parent, grandParent, zone);

         if(i)
         {
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

         offset %= 6;

         zone = parent;
         parent = grandParent;
         pIndex++;
         l--;
      }
      return offset;
   }

   int computeParents(I7HZone zone, I7HZone parents[19])
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

   void getZoneTextID(I7HZone z, String zoneID)
   {
      I7HZone zone = z;
      int level = zone.level, l = level;
      char tmp[256];
      int n;
      static const int cMap[7] = { 0, 3, 1, 5, 4, 6, 2 };
      static const int rootMap[12] = { 1, 6, 2, 7, 3, 8, 4, 9, 5, 10, 0, 11 };
      I7HZone parents[19], parent;
      int pIndex = 0;

      computeParents(zone, parents);
      zoneID[0] = 0;
      parent = l > 0 ? parents[pIndex] : nullZone;
      while(l > 0)
      {
         I7HZone grandParent = l > 1 ? parents[pIndex + 1] : nullZone;
         int i = getCorrectedChildPosition(parent, grandParent, zone);

         if(i)
         {
            int offset = getParentRotationOffsetInternal(parent, parents + pIndex + 1);
            if(parent.nPoints == 5)
               i = adjustZ7PentagonChildPosition(i, l, parent.rootRhombus);
            i = ((i - 1) + offset) % 6 + 1;
         }

         n = cMap[i];
         sprintf(tmp, "%d%s", n, zoneID);
         strcpy(zoneID, tmp);

         zone = parent;
         parent = grandParent;
         pIndex++;
         l--;
      }

      n = rootMap[zone.rootRhombus];
      sprintf(tmp, "%02d%s", n, zoneID);
      strcpy(zoneID, tmp);
   }
}
