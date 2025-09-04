public import IMPORT_STATIC "ecrt"
private:

import "RI3H"
import "icoVertexGreatCircle"

public class ISEA3H_Spherical : RhombicIcosahedral3H
{
   equalArea = true;

   ISEA3H_Spherical() { pj = ISEAProjection_Spherical { }; incref pj; }
   ~ISEA3H_Spherical() { delete pj; }
}

public class ISEAProjection_Spherical : ISEAProjection
{
   ISEAProjection_Spherical()
   {
      int i;

      orientation = { /*(E + F) / 2 /* 90 - 58.2825255885389 = */31.7174744114611, -11.25 };
      getVertices(icoVertices);
      sinOrientationLat = sin(orientation.lat); cosOrientationLat = cos(orientation.lat);
      authalicSetup(wgs84Authalic, wgs84Authalic, cp);

      for(i = 0; i < 20; i++)
      {
         const Vector3D * v1 = &icoVertices[icoIndices[i][0]];
         const Vector3D * v2 = &icoVertices[icoIndices[i][1]];
         const Vector3D * v3 = &icoVertices[icoIndices[i][2]];
         icoFacePlanes[i][0].FromPoints({ 0, 0, 0 }, v1, v2);
         icoFacePlanes[i][1].FromPoints({ 0, 0, 0 }, v2, v3);
         icoFacePlanes[i][2].FromPoints({ 0, 0, 0 }, v3, v1);
      }
   }
}
