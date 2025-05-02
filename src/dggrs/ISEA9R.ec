public import IMPORT_STATIC "ecere"
private:

import "isea5x6"
import "dggrs"
import "RI9R"

#include <stdio.h>

public class ISEA9R : RhombicIcosahedral9R
{
   equalArea = true;

   ISEA9R() { pj = ISEAProjection { }; incref pj; }
   ~ISEA9R() { delete pj; }
}
