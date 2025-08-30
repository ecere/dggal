public import IMPORT_STATIC "ecrt"
private:

import "RI4R"
import "icoVertexGreatCircle"

public class ISEA4R : RhombicIcosahedral4R
{
   equalArea = true;

   ISEA4R() { pj = ISEAProjection { }; incref pj; }
   ~ISEA4R() { delete pj; }
}
