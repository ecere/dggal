public import IMPORT_STATIC "ecrt"
private:

import "RI7H"
import "icoVertexGreatCircle"

public class ISEA7H : RhombicIcosahedral7H
{
   equalArea = true;

   ISEA7H() { pj = ISEAProjection { }; incref pj; }
   ~ISEA7H() { delete pj; }
}
