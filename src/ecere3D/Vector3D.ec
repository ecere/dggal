public import IMPORT_STATIC "ecere"

import "Vector3D"

public struct DGGVector3D
{
   double x, y, z;

   void Subtract(const DGGVector3D vector1, const DGGVector3D vector2)
   {
      x = vector1.x - vector2.x;
      y = vector1.y - vector2.y;
      z = vector1.z - vector2.z;
   }

   double DotProduct(const DGGVector3D vector2)
   {
      return x * vector2.x + y * vector2.y + z * vector2.z;
   }

   void CrossProduct(const DGGVector3D vector1, const DGGVector3D vector2)
   {
      x = vector1.y * vector2.z - vector1.z * vector2.y;
      y = vector1.z * vector2.x - vector1.x * vector2.z;
      z = vector1.x * vector2.y - vector1.y * vector2.x;
   }

   void Normalize(const DGGVector3D source)
   {
      double m = (double)sqrt(source.x * source.x + source.y * source.y + source.z * source.z);
      if(m)
      {
         x = source.x/m;
         y = source.y/m;
         z = source.z/m;
      }
      else
         x = y = z = 0;
   }

   property double length { get { return (double)sqrt(x * x + y * y + z * z); } };
};
