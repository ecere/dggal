public import IMPORT_STATIC "ecere"
private:

import "RI3H"
import "isea5x6"

public class RTEA3H : RhombicIcosahedral3H
{
   equalArea = true;

   RTEA3H() { pj = RTEAProjection { }; incref pj; }
   ~RTEA3H() { delete pj; }
}
