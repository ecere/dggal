public import IMPORT_STATIC "ecrt"
private:

import "RI7H_Z7"

#define Z7_DGGRSZONE

#ifdef Z7_DGGRSZONE
// Using Z7Zone natively for DGGRSZone, at the cost of some performance impact
import "icoVertexGreatCircle"

public class RTEA7H_Z7 : RI7H_Z7
{
   equalArea = true;

   RTEA7H_Z7() { pj = RTEAProjection { }; incref pj; }
   ~RTEA7H_Z7() { delete pj; }
}

#else
// To still use I7HZone for 64-bit integer DGGRSZone...
import "RTEA7H"

public class RTEA7H_Z7 : RTEA7H
{
   I7HZone getZoneFromTextID(const String zoneID)
   {
      return Z7Zone::fromTextID(zoneID).to7H();
   }

   void getZoneTextID(I7HZone zone, String zoneID)
   {
      Z7Zone::from7H(zone).getTextID(zoneID);
   }
}

#endif
