public import IMPORT_STATIC "ecrt"
private:

import "RI9R"
import "icoVertexGreatCircle"

#include <stdio.h>

public class ISEA9R : RhombicIcosahedral9R
{
   equalArea = true;

   ISEA9R() { pj = ISEAProjection { }; incref pj; }
   ~ISEA9R() { delete pj; }
}
