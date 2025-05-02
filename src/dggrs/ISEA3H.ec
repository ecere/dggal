public import IMPORT_STATIC "ecere"
private:

import "RI3H"
import "isea5x6"

public class ISEA3H : RhombicIcosahedral3H
{
   equalArea = true;

   ISEA3H() { pj = ISEAProjection { }; incref pj; }
   ~ISEA3H() { delete pj; }
}
