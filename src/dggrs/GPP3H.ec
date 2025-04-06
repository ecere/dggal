public import IMPORT_STATIC "ecrt"
private:

import "RI3H"
import "barycentric5x6"

// This DGGRS projecting the aperture 3 hexagonal pattern on the faces of the icosahedron to the sphere
// corresponds to Golberg Polyhedra. It is not equal area.
public class GPP3H : RhombicIcosahedral3H
{
   GPP3H() { pj = GoldbergPolyhedraProjection { }; incref pj; }
   ~GPP3H() { delete pj; }
}
