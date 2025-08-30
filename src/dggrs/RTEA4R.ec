public import IMPORT_STATIC "ecrt"
private:

import "RI4R"
import "icoVertexGreatCircle"

public class RTEA4R : RhombicIcosahedral4R
{
   equalArea = true;

   RTEA4R() { pj = RTEAProjection { }; incref pj; }
   ~RTEA4R() { delete pj; }
}
