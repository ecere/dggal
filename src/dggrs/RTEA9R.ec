public import IMPORT_STATIC "ecere"
private:

import "RI9R"
import "icoVertexGreatCircle"

public class RTEA9R : RhombicIcosahedral9R
{
   equalArea = true;

   RTEA9R() { pj = RTEAProjection { }; incref pj; }
   ~RTEA9R() { delete pj; }
}
