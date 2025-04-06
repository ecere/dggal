public import IMPORT_STATIC "ecrt"

import "Vector3D"
import "Quaternion"

public struct Vector3D
{
   double x, y, z;

   void Subtract(const Vector3D vector1, const Vector3D vector2)
   {
      x = vector1.x - vector2.x;
      y = vector1.y - vector2.y;
      z = vector1.z - vector2.z;
   }

   double DotProduct(const Vector3D vector2)
   {
      return x * vector2.x + y * vector2.y + z * vector2.z;
   }

   void CrossProduct(const Vector3D vector1, const Vector3D vector2)
   {
      x = vector1.y * vector2.z - vector1.z * vector2.y;
      y = vector1.z * vector2.x - vector1.x * vector2.z;
      z = vector1.x * vector2.y - vector1.y * vector2.x;
   }

   void Normalize(const Vector3D source)
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

   void MultQuaternion(const Vector3D s, const Quaternion quat)
   {
      Vector3D v { quat.x, quat.y, quat.z };
      double w = quat.w, a = w*w - (v.x*v.x+v.y*v.y+v.z*v.z) /*DotProduct(v)*/, dotVS = v.x*s.x+v.y*s.y+v.z*s.z /*v.DotProduct(s)*/;
      Vector3D cross
      {
         s.y * v.z - s.z * v.y,
         s.z * v.x - s.x * v.z,
         s.x * v.y - s.y * v.x
      };
      //cross.CrossProduct(s, v);
      x = (float)(2 * dotVS * v.x + a * s.x + 2 * w * cross.x);
      y = (float)(2 * dotVS * v.y + a * s.y + 2 * w * cross.y);
      z = (float)(2 * dotVS * v.z + a * s.z + 2 * w * cross.z);
   }
};
