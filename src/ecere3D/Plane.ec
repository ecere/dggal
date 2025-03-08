public import IMPORT_STATIC "ecere"

import "Vector3D"

public struct DGGPlane
{
   union
   {
      struct { double a, b, c; };
      DGGVector3D normal;
   };
   double d;

   void FromPoints(const DGGVector3D v1, const DGGVector3D v2, const DGGVector3D v3)
   {
      DGGVector3D a, b;

      a.Subtract(v3, v1);
      b.Subtract(v2, v1);
      normal.CrossProduct(a, b);
      normal.Normalize(normal);

      d = -normal.DotProduct(v1);
   }
};
