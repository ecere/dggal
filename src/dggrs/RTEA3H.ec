public import IMPORT_STATIC "ecrt"
private:

import "RI3H"
import "icoVertexGreatCircle"

public class RTEA3H : RhombicIcosahedral3H
{
   equalArea = true;

   RTEA3H() { pj = RTEAProjection { }; incref pj; }
   ~RTEA3H() { delete pj; }
}
