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

   int getExtraOffset(I7HZone z)
   {
      int offset = 0;
      I7HZone zone = z.parent0;
      int level = zone.level, l = level;

      while(l > 0)
      {
         I7HZone parent = zone.parent0;
         uint pRoot = parent.rootRhombus;
         if(pRoot >= 10)
         {
            uint pnPoints = parent.nPoints;
            if(pnPoints == 5 || zone.isEdgeHex)
            {
               bool southPRoot = pRoot & 1;
               bool oddLevel = l & 1;
               int i;

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
                  if(pnPoints == 5)
                  {
                     i = ((i + 1) % 5) + 1;
                     if(oddLevel)
                        offset += i + 3;
                     else if(l == 2)
                     {
                        offset += i + 2;
                        if(pRoot == 11)
                           offset += i == 5 ? 2 : 1;
                     }
                  }
                  else if(l == 2) // && zone.isEdgeHex)
                     offset += 5;
               }
               offset %= 6;
            }
         }

         zone = parent;
         l--;
      }
      return offset;
   }

   void getZoneTextID(I7HZone zone, String zoneID)
   {
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
         bool southPentagonParent = pnPoints == 5 && southPRhombus;
         bool oddLevel = l & 1;
         int offset = getExtraOffset(zone);

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
            if(pnPoints == 5)
            {
               if(pRoot == 10)
                  i = ((i + 1) % 5) + 1;
               else if(pRoot == 11)
                  i = ((i + (oddLevel ? 3 : 4)) % 5) + 1;
               else if(!oddLevel && !southPentagonParent)
                  i = ((i + 5) % 5) + 1;
               if(southPentagonParent && i >= 3)
                  i++;
            }
            i = (i - 1 + offset) % 6 + 1;
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
