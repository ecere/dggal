public import IMPORT_STATIC "ecrt"
private:

import "RI4R"
import "icoVertexGreatCircle"

public class IVEA4R : RhombicIcosahedral4R
{
   equalArea = true;

   IVEA4R() { pj = SliceAndDiceGreatCircleIcosahedralProjection { }; incref pj; }
   ~IVEA4R() { delete pj; }
}
