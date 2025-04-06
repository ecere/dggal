public import IMPORT_STATIC "ecrt"
private:

import "RI9R"
import "icoVertexGreatCircle"

public class IVEA9R : RhombicIcosahedral9R
{
   equalArea = true;

   IVEA9R() { pj = SliceAndDiceGreatCircleIcosahedralProjection { }; incref pj; }
   ~IVEA9R() { delete pj; }
}
