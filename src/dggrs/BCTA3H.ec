public import IMPORT_STATIC "ecrt"
private:

import "RI3H"
import "barycentric5x6"

// This DGGRS mapping barycentric coordinates to spherical triangle areas is not equal-area
public class BCTA3H : RhombicIcosahedral3H
{
   BCTA3H() { pj = BarycentricSphericalTriAreaProjection { }; incref pj; }
   ~BCTA3H() { delete pj; }
}
