public import IMPORT_STATIC "ecrt"
private:

import "RI3H"
import "icoVertexGreatCircle"

public class ISEA3H : RhombicIcosahedral3H
{
   equalArea = true;

   ISEA3H() { pj = ISEAProjection { }; incref pj; }
   ~ISEA3H() { delete pj; }
}
