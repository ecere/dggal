public import IMPORT_STATIC "ecrt"
import IMPORT_STATIC "dggal"

int relationInfo(DGGRS dggrs, DGGRSZone a, DGGRSZone b, Map<String, const String> options)
{
   int exitCode = 1;
   if(a != nullZone && b != nullZone)
   {
      if(a == b)
      {
         PrintLn($"These two zones are the same");
         exitCode = 0;
      }
      else
      {
         char zoneID[2][256];
         int levelA = dggrs.getZoneLevel(a), levelB = dggrs.getZoneLevel(b);
         double areaA = dggrs.getZoneArea(a), areaB = dggrs.getZoneArea(b);
         bool neighbors = dggrs.areZonesNeighbors(a, b);
         bool siblings = dggrs.areZonesSiblings(a, b);
         bool aChildOfB = dggrs.isZoneImmediateChildOf(a, b);
         bool aParentOfB = dggrs.isZoneImmediateParentOf(a, b);
         bool aAncestorOfB = dggrs.isZoneAncestorOf(a, b, 0);
         bool aDescendantOfB = dggrs.isZoneDescendantOf(a, b, 0);
         bool aContainedInB = dggrs.isZoneContainedIn(a, b);
         bool aContainsB = dggrs.doesZoneContain(a, b);
         bool aSubZoneOfB = dggrs.zoneHasSubZone(b, a);
         bool bSubZoneOfA = dggrs.zoneHasSubZone(a, b);
         int maxDepth = dggrs.getMaxDepth(), maxIndexDepth = dggrs.getIndexMaxDepth();
         int64 aIndexInB = aSubZoneOfB && levelA - levelB <= maxDepth ? dggrs.getSubZoneIndex(b, a) : -1;
         int64 bIndexInA = bSubZoneOfA && levelB - levelA <= maxDepth ? dggrs.getSubZoneIndex(a, b) : -1;
         bool overlap = dggrs.doZonesOverlap(a, b);

         dggrs.getZoneTextID(a, zoneID[0]);
         dggrs.getZoneTextID(b, zoneID[1]);
         PrintLn($"Relationships between zones ", zoneID[0], " (A) and ", zoneID[1], " (B):\n");

         if(levelA == levelB)
            PrintLn($"These zones are at the same refinement level (", levelA, ")");
         else if(levelA < levelB)
            PrintLn($"Zone A is coarser than zone B by ", levelB - levelA, $" refinement level", levelB - levelA > 1 ? $"s" : "");
         else
            PrintLn($"Zone A is finer than zone B by ", levelA - levelB, $" refinement level", levelA - levelB > 1 ? $"s" : "");

         if(areaA == areaB)
            PrintLn($"The areas of these zones are exactly the same");
         else if(areaA < areaB)
            PrintLn($"The area of zone A is smaller than the area of zone B (area of A is ", areaA * 100 / areaB, " % of zone B)");
         else
            PrintLn($"The area of zone A is greater than the area of zone B (area of B is ", areaB * 100 / areaA, " % of zone A)");

         PrintLn($"Zone A is ", aChildOfB ? "" : $"NOT ", $"an immediate child of zone B");
         PrintLn($"Zone A is ", aParentOfB ? "" : $"NOT ", $"an immediate parent of zone B");
         PrintLn($"Zone A is ", aDescendantOfB ? "" : $"NOT ", $"a descendant of zone B");
         PrintLn($"Zone A is ", aAncestorOfB ? "" : $"NOT ", $"an ancestor of zone B");

         Print($"Zone A is ", aSubZoneOfB ? "" : $"NOT ", $"a sub-zone of zone B");
         if(aIndexInB != -1)
            Print($" (at depth ", levelA - levelB, ", index ", aIndexInB, ")");
         else if(aSubZoneOfB)
            Print($" (depth ", levelA - levelB, $" is too many levels apart -- max: ", maxDepth,
               levelA - levelB <= maxIndexDepth ?
                  $", use index command to compute index -- currently slow)" :
                  $", not currently able to compute index)");
         PrintLn("");
         Print($"Zone A ", bSubZoneOfA ? "has" : $"does NOT have", $" B as a sub-zone");
         if(bIndexInA != -1)
            Print(" (at depth ", levelB - levelA, ", index ", bIndexInA, ")");
         else if(bSubZoneOfA)
            Print($" (depth ", levelB - levelA, $" is too many levels apart -- max: ", maxDepth,
               levelB - levelA <= maxIndexDepth ?
                  $", use index command to compute index -- currently slow)" :
                  $", not currently able to compute index)");
         PrintLn("");
         PrintLn($"These zones are ", neighbors ? "" : $"NOT ", $"neighbors");
         PrintLn($"These zones are ", siblings ? "" : $"NOT ", $"siblings");
         PrintLn($"Zone A is ", aContainedInB ? "" : $"NOT ", $"contained in zone B");
         PrintLn($"Zone A ", aContainsB ? $"contains" : $"does not contain", $" zone B");
         PrintLn($"Zone A and B ", !overlap ? $"do NOT " : "", $"overlap");

         exitCode = 0;
      }
   }
   else
      PrintLn($"rel command requires two zones");
   return exitCode;
}
