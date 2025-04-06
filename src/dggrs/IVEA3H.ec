public import IMPORT_STATIC "ecrt"
private:

import "RI3H"
import "icoVertexGreatCircle"

public class IVEA3H : RhombicIcosahedral3H
{
   equalArea = true;

   IVEA3H() { pj = SliceAndDiceGreatCircleIcosahedralProjection { }; incref pj; }
   ~IVEA3H() { delete pj; }
}
