public import IMPORT_STATIC "ecrt"

import "Vector3D"

public struct Plane
{
   union
   {
      struct { double a, b, c; };
      Vector3D normal;
   };
   double d;

   void FromPoints(const Vector3D v1, const Vector3D v2, const Vector3D v3)
   {
      Vector3D a, b;

      a.Subtract(v3, v1);
      b.Subtract(v2, v1);
      normal.CrossProduct(a, b);
      normal.Normalize(normal);

      d = -normal.DotProduct(v1);
   }
};
