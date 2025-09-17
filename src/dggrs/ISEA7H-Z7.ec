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

   int getParentRotationOffset(I7HZone zone)
   {
      int offset = 0;
      int level = zone.level, l = level;

      while(l > 0)
      {
         I7HZone parent = zone.parent0;
         uint pRoot = parent.rootRhombus;
         uint pnPoints = parent.nPoints;
         bool southPRoot = pRoot & 1;
         bool oddLevel = l & 1;
         int i;
         bool southPRhombus = pRoot & 1;
         bool edgeHexCentroidChildParent = !oddLevel &&
            parent == parent.parent0.centroidChild && parent.parent0.isEdgeHex;

         if(oddLevel)
         {
            i = zone.subHex - 1;
            if(i && !southPRoot && parent.isEdgeHex)
               i = (i % 6) + 1; // Top-Left ends up being Left crossing interruption to the left in the north
         }
         else
         {
            I7HZone children[7];
            int nc = parent.getPrimaryChildren(children);

            for(i = 0; i < nc; i++)
               if(children[i] == zone)
                  break;
         }

         if(i)
         {
            if(pRoot >= 10)
            {
               if(pnPoints == 5)
               {
                  i = ((i + 1) % 5) + 1;
                  if(oddLevel)
                     offset += i + 3;
                  else if(l >= 2)
                  {
                     offset += i + 2;
                     if(pRoot == 11)
                        offset += i == 5 ? 2 : 1;
                  }
               }
               else if(!oddLevel && zone.isEdgeHex)
               {
                  if(!southPRhombus || zone != parent.centroidChild)
                     offset += 5;
               }
            }
            if(edgeHexCentroidChildParent)
            {
               I7HZone c[7];
               parent.getPrimaryChildren(c);
               if(c[2].rootRhombus != c[5].rootRhombus)
               {
                  i += southPRhombus ? 5 : 1;
                  i = (i - 1) % 6 + 1;
               }
            }

            if(oddLevel && parent.isEdgeHex)
            {
               // This rule is necessary starting from Level 4
               if(!southPRhombus && i >= 4)
                  offset++;
               else if(southPRhombus && i < 4)
                  offset++;
            }
            else if(!oddLevel && parent.parent0.isEdgeHex)
            {
               I7HZone c[7], pc[7];
               parent.parent0.getPrimaryChildren(pc);
               parent.getPrimaryChildren(c);
               if(southPRoot && pc[1] == parent && c[2].rootRhombus != c[5].rootRhombus)
               {
                  if(i == 4 || i == 5)
                     offset += 5;
               }
               else if(!southPRoot && pc[3] == parent) // Root rhombuses are the same in this case
               {
                  if(i == 1 || i == 2)
                     offset += 5;
               }
            }
         }
         else if(oddLevel && parent.isEdgeHex && southPRhombus) // This rule is necessary starting from Level 4
            offset++;

         if(edgeHexCentroidChildParent)
         {
            if(southPRoot)
            {
               if(i != 1 && i != 2)
                  offset += 5;
            }
            else
            {
               if(i == 5 || i == 6)
                  offset += 1;
            }
         }

         offset %= 6;

         zone = parent;
         l--;
      }
      return offset;
   }

   void getZoneTextID(I7HZone z, String zoneID)
   {
      I7HZone zone = z;
      int level = zone.level, l = level;
      char tmp[256];
      int n;
      static const int cMap[7] = { 0, 3, 1, 5, 4, 6, 2 };
      static const int rootMap[12] = { 1, 6, 2, 7, 3, 8, 4, 9, 5, 10, 0, 11 };

      zoneID[0] = 0;
      while(l > 0)
      {
         I7HZone parent = zone.parent0;
         int i;
         uint pRoot = parent.rootRhombus;
         uint pnPoints = parent.nPoints;
         bool southPRhombus = pRoot & 1;
         bool oddLevel = l & 1;

         if(oddLevel)
         {
            i = zone.subHex - 1;
            if(i && !southPRhombus && parent.isEdgeHex)
               i = (i % 6) + 1; // Top-Left ends up being Left crossing interruption to the left in the north
         }
         else
         {
            I7HZone children[7];
            int nc = parent.getPrimaryChildren(children);

            for(i = 0; i < nc; i++)
               if(children[i] == zone)
                  break;
         }

         if(i)
         {
            int offset = getParentRotationOffset(parent);

            if(pnPoints == 5)
            {
               if(pRoot == 10)
                  i = ((i + 1) % 5) + 1;
               else if(pRoot == 11)
                  i = ((i + (oddLevel ? 3 : 4)) % 5) + 1;
               else if(!oddLevel && !southPRhombus) // Parent is an odd level northern non-polar pentagon
                  i = ((i + 5) % 5) + 1;
               if(southPRhombus && i >= 3)
                  i++;
            }
            if(parent == parent.parent0.centroidChild && parent.parent0.isEdgeHex)
            {
               I7HZone c[7];
               parent.getPrimaryChildren(c);
               if(c[2].rootRhombus != c[5].rootRhombus)
                  i += southPRhombus ? 5 : 1;
            }

            i += offset;

            i = (i - 1) % 6 + 1;
         }

         n = cMap[i];
         sprintf(tmp, "%d%s", n, zoneID);
         strcpy(zoneID, tmp);

         zone = parent;
         l--;
      }

      n = rootMap[zone.rootRhombus];
      sprintf(tmp, "%02d%s", n, zoneID);
      strcpy(zoneID, tmp);
   }
}
