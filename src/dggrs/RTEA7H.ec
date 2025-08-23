public import IMPORT_STATIC "ecrt"
private:

import "RI7H"
import "icoVertexGreatCircle"

public class RTEA7H : RhombicIcosahedral7H
{
   equalArea = true;

   RTEA7H() { pj = RTEAProjection { }; incref pj; }
   ~RTEA7H() { delete pj; }
}
