public import IMPORT_STATIC "ecrt"
import IMPORT_STATIC "dggal"

int subZoneIndex(DGGRS dggrs, DGGRSZone parent, DGGRSZone subZone, Map<String, const String> options)
{
   int exitCode = 1;

   if(parent != nullZone && subZone != nullZone)
   {
      int levelParent = dggrs.getZoneLevel(parent), levelSub = dggrs.getZoneLevel(subZone);
      if(levelSub >= levelParent)
      {
         if(levelSub - levelParent <= dggrs.getIndexMaxDepth())
         {
            char zoneID[256], subZoneID[256];
            int64 index = dggrs.zoneHasSubZone(parent, subZone) ? dggrs.getSubZoneIndex(parent, subZone) : -1;

            dggrs.getZoneTextID(parent, zoneID);
            dggrs.getZoneTextID(subZone, subZoneID);

            if(index != -1)
            {
               PrintLn(subZoneID, $" is at index ", index, $" of ", zoneID, $" at depth ", levelSub - levelParent);
               exitCode = 0;
            }
            else
               PrintLn($"sub-zone ", subZoneID, $" not found within parent ", zoneID);
         }
         else
            PrintLn($"sub-zone is too many refinement level apart from parent (", levelSub - levelParent, ")");
      }
      else
         PrintLn($"invalid sub-zone at a coarser refinement level than parent");
   }
   else
      PrintLn($"index command requires a parent and a sub-zone");
   return exitCode;
}
