public import IMPORT_STATIC "ecrt"
private:

import "RI7H"
import "icoVertexGreatCircle"

public class IVEA7H : RhombicIcosahedral7H
{
   equalArea = true;

   IVEA7H() { pj = SliceAndDiceGreatCircleIcosahedralProjection { }; incref pj; }
   ~IVEA7H() { delete pj; }
}
