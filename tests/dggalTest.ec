public import IMPORT_STATIC "ecrt"
import "testingFramework"
import IMPORT_STATIC "dggal"

import "isea3HTests"

public class DGGSUnitTest : eTest
{
   void test(DGGSTestCase c)
   {
      DGGRS dggrs = null;
      String testCase = PrintString(c.dggrs, "/", c.zoneID);
      char zoneID[100];
      Class dggrsClass = eSystem_FindClass(__thisModule, c.dggrs);

      if(dggrsClass)
         dggrs = eInstance_New(dggrsClass);

      if(!dggrs)
         fail("DGGRS", testCase, "of unrecognized DGGRS");

      dggrs.getZoneTextID(c.key, zoneID);

      if(strcmp(zoneID, c.zoneID))
         fail("ZoneID", testCase, "of mismatched ZoneID");
      else
      {
         GeoPoint centroid { };
         GeoExtent extent { };
         Array<DGGRSZone> parents = null, children = null, neighbors = null;
         double area = dggrs.getZoneArea(c.key);
         bool mismatchedParent = false, mismatchedChild = false, mismatchedNeighbor = false;
         bool badCentroidParent = false, badCentroidChild = false;
         int i;
         DGGRSZone key64bit = nullZone;
         DGGRSZone cParent = nullZone, cChild = nullZone;
         bool badIsCChild = false;
         bool badEdgeCount = false;
         bool mismatchedFirstSubZone = false, mismatchedSubZones = false;
         bool mismatchedExtent = false;
         DGGRSZone key = c.key;
         DGGRSZone p[3], n[6], ch[9];
         int num;

         key64bit = dggrs.getZoneFromTextID(c.zoneID);

         dggrs.getZoneWGS84Centroid(key, centroid);
         dggrs.getZoneWGS84Extent(key, extent);

         num = dggrs.getZoneParents(key, p);
         parents = { size = num };
         memcpy(parents.array, p, sizeof(DGGRSZone) * num);

         num = dggrs.getZoneNeighbors(key, n, null);
         if(dggrs.countZoneEdges(key) != num)
            badEdgeCount = true;
         neighbors = { size = num };
         memcpy(neighbors.array, n, sizeof(DGGRSZone) * num);

         num = dggrs.getZoneChildren(key, ch);
         children = { size = num };
         memcpy(children.array, ch, sizeof(DGGRSZone) * num);

         cParent = dggrs.getZoneCentroidParent(key);
         cChild = dggrs.getZoneCentroidChild(key);

         if(dggrs.isZoneCentroidChild(key) != (!c.parents || (c.parents && c.parents.GetCount() == 1)))
            badIsCChild = true;

         if(c.firstSubZones)
         {
            for(s : c.firstSubZones)
            {
               int d = &s;
               const String k = s;
               DGGRSZone sz = dggrs.getFirstSubZone(key, d);
               char szID[100];

               dggrs.getZoneTextID(sz, szID);
               if(strcmp(szID, k))
               {
                  PrintLn("Mismatched first sub zone of relative depth ", d, " (", szID, " where ", k, " expected)");
                  mismatchedFirstSubZone = true;
                  break;
               }
            }
         }

         if(c.subZones)
         {
            for(s : c.subZones)
            {
               int d = &s;
               Array<const String> expectedSZ = s;
               Array<DGGRSZone> sz = dggrs.getSubZones(key, d);
               int i;

               if(!sz || sz.count != expectedSZ.count)
               {
                  PrintLn("Mismatched number of sub zones for relative depth ", d, " (", sz ? sz.count : 0, " where ", expectedSZ.count, " expected)");
                  mismatchedSubZones = true;
                  break;
               }

               for(i = 0; i < sz.count; i++)
               {
                  DGGRSZone k = sz[i];
                  char szID[100];
                  dggrs.getZoneTextID(k, szID);
                  if(strcmp(szID, expectedSZ[i]))
                  {
                     PrintLn("Mismatched sub zone for relative depth ", d, " (", szID, " where ", expectedSZ[i], " expected)");
                     mismatchedSubZones = true;
                     break;
                  }
               }
            }
         }

         if((c.centroidChild == null) != (cChild == nullZone))
            badCentroidChild = true;
         else if(c.centroidChild)
         {
            char cZone[100];

            dggrs.getZoneTextID(cChild, cZone);
            if(!c.centroidChild || strcmp(cZone, c.centroidChild))
            {
               PrintLn("Centroid child is: ", cZone);
               badCentroidChild = true;
            }
         }

         if((c.centroidParent == null) != (cParent == nullZone))
            badCentroidParent = true;
         else if(c.centroidParent)
         {
            char pZone[100];

            dggrs.getZoneTextID(cParent, pZone);
            if(strcmp(pZone, c.centroidParent))
            {
               PrintLn("Centroid parent is: ", pZone);
               badCentroidParent = true;
            }
         }

         if((c.parents ? c.parents.GetCount() : 0) != (parents ? parents.count : 0))
            mismatchedParent = true;
         else if(c.parents)
         {
            for(i = 0; i < c.parents.GetCount(); i++)
            {
               char pZone[100];

               dggrs.getZoneTextID(parents[i], pZone);
               if(strcmp(pZone, c.parents[i]))
               {
                  mismatchedParent = true;
                  break;
               }
            }
         }

         if((c.children ? c.children.GetCount() : 0) != (children ? children.count : 0))
            mismatchedChild = true;
         else if(c.children)
         {
            for(i = 0; i < c.children.GetCount(); i++)
            {
               char cZone[100];

               dggrs.getZoneTextID(children[i], cZone);
               if(strcmp(cZone, c.children[i]))
               {
                  mismatchedChild = true;
                  break;
               }
            }
         }

         if((c.neighbors ? c.neighbors.GetCount() : 0) != (neighbors ? neighbors.count : 0))
            mismatchedNeighbor = true;
         else if(c.children)
         {
            for(i = 0; i < c.neighbors.GetCount(); i++)
            {
               char nZone[100];

               dggrs.getZoneTextID(neighbors[i], nZone);
               if(strcmp(nZone, c.neighbors[i]))
               {
                  mismatchedNeighbor = true;
                  break;
               }
            }
         }

         if(fabs((double)extent.ll.lat - (double)c.wgs84Extent.ll.lat) > epsilonExtent ||
            fabs((double)extent.ll.lon - (double)c.wgs84Extent.ll.lon) > epsilonExtent ||
            fabs((double)extent.ur.lat - (double)c.wgs84Extent.ur.lat) > epsilonExtent ||
            fabs((double)extent.ur.lon - (double)c.wgs84Extent.ur.lon) > epsilonExtent)
         {
            if((fabs((double)extent.ll.lat - -90) > 1E-11 && fabs((double)extent.ur.lat - 90) > 1E-11) ||
               fabs((double)extent.ll.lat - (double)c.wgs84Extent.ll.lat) > epsilonExtentPole ||
               fabs((double)extent.ll.lon - (double)c.wgs84Extent.ll.lon) > epsilonExtentPole ||
               fabs((double)extent.ur.lat - (double)c.wgs84Extent.ur.lat) > epsilonExtentPole ||
               fabs((double)extent.ur.lon - (double)c.wgs84Extent.ur.lon) > epsilonExtentPole)
               mismatchedExtent = true;
         }

         delete parents;
         delete children;
         delete neighbors;

         if(key64bit != c.key)
            fail("Zone key", testCase, "of mismatched 64-bit key");
         else if(badEdgeCount)
            fail("Edge Count", testCase, "of mismatched count of edges");
         else if(badIsCChild)
            fail("IsCentroidChild", testCase, "of wrong isCentroidChild property");
         else if(fabs(area - c.area) / c.area > epsilonArea)
         {
            PrintLn("area: ", area, " but expected ", c.area, " (", c.area - area, " delta)");
            fail("Zone area", testCase, "of mismatched zone area");
         }
         else if(fabs((double)centroid.lat - (double)c.centroid.lat) > epsilonCentroid ||
                 fabs((double)centroid.lon - (double)c.centroid.lon) > epsilonCentroid)
         {
            PrintLn("centroid: ", centroid, " but expected ", c.centroid,
               " (", (double)c.centroid.lat - (double)centroid.lat, ", ", (double)c.centroid.lon - (double)centroid.lon, " delta)");
            fail("Centroid", testCase, "of mismatched centroid");
         }
         else if(mismatchedExtent)
         {
            PrintLn("extent: ", extent, " but expected ", c.wgs84Extent);
            fail("Extent", testCase, "of mismatched extent");
         }
         else if(badCentroidParent)
            fail("Centroid Parent", testCase, "of mismatched centroid parent");
         else if(badCentroidChild)
            fail("Centroid Child", testCase, "of mismatched centroid child");
         else if(mismatchedParent)
            fail("Parents", testCase, "of mismatched parents");
         else if(mismatchedChild)
            fail("Children", testCase, "of mismatched children");
         else if(mismatchedNeighbor)
            fail("Neighbors", testCase, "of mismatched neighbors");
         else if(mismatchedFirstSubZone)
            fail("FirstSubZones", testCase, "of mismatched first subzones");
         else if(mismatchedSubZones)
            fail("SubZones", testCase, "of mismatched subzones");
         else
            pass("DGGS zone checks", testCase);
      }
      delete dggrs;
      delete testCase;
   }

   void testSubZones(subclass(DGGRS) dggrsClass, int maxParentLevel, int maxDepth)
   {
      int pLevel;
      bool success = true;
      DGGRS dggrs = eInstance_New(dggrsClass);
      char thisTest[256];

      sprintf(thisTest, "%s parent level 0..%d / depth 0..%d",
         ((Class)dggrsClass).name, maxParentLevel, maxDepth);

      PrintLn("Testing ", ((Class)dggrsClass).name, "...");

      for(pLevel = 0; pLevel <= maxParentLevel; pLevel++)
      {
         Array<DGGRSZone> allZones = dggrs.listZones(pLevel, wholeWorld);
         if(allZones)
         {
            uint64 expectedZones = dggrs.countZones(pLevel);
            int depth;
            PrintLn("Testing sub-zones of level ", pLevel, " zones");

            if(allZones.count != expectedZones)
            {
               PrintLn("Expected: ", expectedZones, " zones, but ", allZones.count, " returned");
               fail("DGGS sub-zones", thisTest, "of unexpected unmber of parent zones listed");
               success = false;
            }

            for(depth = 0; depth <= maxDepth; depth++)
            {
               for(z : allZones)
               {
                  DGGRSZone zone = z;
                  int64 nz = dggrs.countSubZones(zone, depth);
                  Array<DGGRSZone> subZones = dggrs.getSubZones(zone, depth);
                  char zoneID[100];

                  dggrs.getZoneTextID(zone, zoneID);

                  if(!subZones || nz != subZones.count)
                  {
                     PrintLn("Parent Level ", pLevel, ", Depth ", depth, ", Zone { ", zoneID, " }: "
                        "subZones count: ", subZones ? subZones.count : 0, ", expected: ", nz);
                     fail("DGGS sub-zones", thisTest, "of mismatched sub-zones count");
                     success = false;
                  }
                  else
                  {
                     HashTable<DGGRSZone> table { initSize = subZones.count };
                     int i;
                     for(i = 0; i < subZones.count; i++)
                     {
                        char szID[256];
                        dggrs.getZoneTextID(subZones[i], szID);

                        if(subZones[i] == nullZone)
                        {
                           PrintLn("Parent Level ", pLevel, ", Depth ", depth, ", Zone ", zoneID, ": null sub-zone at index ", i);
                           fail("DGGS sub-zones", thisTest, "of bad sub-zone");
                           success = false;
                           break;
                        }
                        else if(!table.Add(subZones[i]))
                        {
                           PrintLn("Parent Level ", pLevel, ", Depth ", depth, ", Zone ", zoneID, ": duplicate sub-zone at index ", i);
                           fail("DGGS sub-zones", thisTest, "of duplicate sub-zone");
                           success = false;
                           break;
                        }
                        else if(depth > 0 && !dggrs.zoneHasSubZone(zone, subZones[i]))
                        {
                           PrintLn("Parent Level ", pLevel, ", Depth ", depth, ", Zone ", zoneID, ": sub-zone ", szID, " not recognized");
                           fail("DGGS sub-zones", thisTest, "of undetected sub-zone");
                           // dggrs.zoneHasSubZone(zone, subZones[i]);
                           success = false;
                        }
                        /*else if(depth > 0 && dggrs.getSubZoneAtIndex(zone, depth, i) != subZones[i])
                        {
                           // DGGRSZone sz2 = dggrs.DGGRS::getSubZoneAtIndex(zone, depth, i);
                           // DGGRSZone sz = dggrs.getSubZoneAtIndex(zone, depth, i);
                           fail("DGGS sub-zones", thisTest, "of unexpected result from getSubZoneAtIndex()");
                           success = false;
                        }
                        else if(depth > 0 && dggrs.getSubZoneIndex(zone, subZones[i]) != i)
                        {
                           // int64 index2 = dggrs.DGGRS::getSubZoneIndex(zone, subZones[i]);
                           // int64 index = dggrs.getSubZoneIndex(zone, subZones[i]);
                           fail("DGGS sub-zones", thisTest, "of unexpected result from getSubZoneIndex()");
                           success = false;
                        }*/
                     }
                     if(success && depth > 0)
                     {
                        for(i = 0; i < subZones.count; i++)
                        {
                           DGGRSZone sz = subZones[i];
                           DGGRSZone nb[6];
                           int j, n;

                           n = dggrs.getZoneNeighbors(sz, nb, null);

                           for(j = 0; j < n; j++)
                           {
                              if(!table.Find(nb[j]))
                              {
                                 if(dggrs.zoneHasSubZone(zone, nb[j]))
                                 {
                                    char szID[256];
                                    dggrs.getZoneTextID(nb[j], szID);

                                    PrintLn("Parent Level ", pLevel, ", Depth ", depth, ", Zone ", zoneID, ": sub-zone ", szID, " wrongly recognized");
                                    fail("DGGS sub-zones", thisTest, "of bad sub-zone result");

                                    dggrs.zoneHasSubZone(zone, nb[j]);
                                    success = false;
                                 }
                              }
                           }
                        }
                     }
                     delete table;
                  }

                  delete subZones;
               }
            }
            delete allZones;
         }
         else
         {
            PrintLn("Sub Zones for parent level ", pLevel);
            fail("DGGS sub-zones", thisTest, "of failure to list parent zones");
         }
      }
      if(success)
         pass("DGGS sub-zones", thisTest);

      delete dggrs;
   }

   void executeTests()
   {
      for(t : isea3HTestCases)
         test(t);

      testSubZones(class(ISEA3H), 4, 8);
      testSubZones(class(ISEA9R), 2, 4);
      testSubZones(class(GNOSISGlobalGrid), 6, 3);
   }
}
