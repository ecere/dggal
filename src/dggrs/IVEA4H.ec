public import IMPORT_STATIC "ecrt"
private:

import "RI4H"
import "icoVertexGreatCircle"

public class IVEA4H : RhombicIcosahedral4H
{
   equalArea = true;

   IVEA4H() { pj = SliceAndDiceGreatCircleIcosahedralProjection { }; incref pj; }
   ~IVEA4H() { delete pj; }
}
