public import IMPORT_STATIC "ecrt"

import "Vector3D"
import "Quaternion"

public struct Quaternion
{
   double w, x, y, z;

   void YawPitch(Degrees yaw, Degrees pitch)
   {
      double sYaw   = sin( yaw / 2 );
      double cYaw   = cos( yaw / 2 );
      double sPitch = sin( pitch / 2 );
      double cPitch = cos( pitch / 2 );

      w = cPitch * cYaw;
      x = sPitch * cYaw;
      y = cPitch * sYaw;
      z = sPitch * sYaw;
   }
};
