public import IMPORT_STATIC "ecrt"
private:

import "ri5x6"

import "Vector3D"

public class GoldbergPolyhedraProjection : BarycentricSphericalTriAreaProjection
{
   projectedGP = true;
}

public class BarycentricSphericalTriAreaProjection : RI5x6Projection
{
   bool projectedGP; projectedGP = false;

   // TODO: This does not yet implement the foward projection (e.g., for 'zone' lookups)

   __attribute__ ((unused))
   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   void inverseIcoFace(const Pointd v,
      const Pointd p1, const Pointd p2, const Pointd p3,
      const Vector3D v1, const Vector3D v2, const Vector3D v3,
      Vector3D out)
   {
      double b[3];
      double u1, u2, u3;

      cartesianToBary(b, v, p1, p2, p3, 0);
      if(b[0] < 0) b[0] = 0;
      if(b[1] < 0) b[1] = 0;
      if(b[2] < 0) b[2] = 0;

      if(projectedGP)
      {
         // Directly using the barycentric coordinates here projects the grid as defined on the face of the icosahedron
         // directly to the sphere, and is equivalent to splitting the icosahedron into Goldberg Polyhedra
         u1 = b[0], u2 = b[1], u3 = b[2];
      }
      else
      {
         // https://math.stackexchange.com/questions/1151428/point-within-a-spherical-triangle-given-areas
         double omega = (4 * Pi) / 20, tau = tan(omega / 2), beta = phi - 1;
         double l1 = tan(b[0] * omega/2) / tau;
         double l2 = tan(b[1] * omega/2) / tau;
         double l3 = tan(b[2] * omega/2) / tau;
         double vi = l1 / ((1 + beta) + (1 - beta) * l1);
         double vj = l2 / ((1 + beta) + (1 - beta) * l2);
         double vk = l3 / ((1 + beta) + (1 - beta) * l3);
         double od = 1.0 / (1 - vi - vj - vk);

         u1 = vi * od;
         u2 = vj * od;
         u3 = vk * od;
      }
      out = {
         u1 * v1.x + u2 * v2.x + u3 * v3.x,
         u1 * v1.y + u2 * v2.y + u3 * v3.y,
         u1 * v1.z + u2 * v2.z + u3 * v3.z
      };
      out.Normalize(out);
   }

   void forwardIcoFace(const Vector3D v, const Vector3D v1, const Vector3D v2, const Vector3D v3, const Pointd p1, const Pointd p2, const Pointd p3, Pointd out)
   {
      double b[3];
      if(projectedGP)
      {
         Vector3D c12, c23, c31;
         double lv, lu;
         double u1, u2, u3;

         // NOTE: These could be constant for each of the 120 spherical triangles
         c12.CrossProduct(v1, v2);
         c23.CrossProduct(v2, v3);
         c31.CrossProduct(v3, v1);

         lv = 1.0 / v1.DotProduct(c23);

         u1 = c23.DotProduct(v) * lv;
         u2 = c31.DotProduct(v) * lv;
         u3 = c12.DotProduct(v) * lv;

         lu = 1.0 / (u1 + u2 + u3);
         u1 *= lu;
         u2 *= lu;
         u3 *= lu;

         b[0] = u1, b[1] = u2, b[2] = u3;
      }
      else
      {
         double omega = (4 * Pi) / 20;
         Vector3D mid12, mid23, mid31, midv1, midv2, midv3, cross32v, cross13v, cross21v;

         // NOTE: These first 3 could be constant for each of the 120 spherical triangles
         mid12.Normalize({ (v1.x + v2.x) / 2, (v1.y + v2.y) / 2, (v1.z + v2.z) / 2 });
         mid23.Normalize({ (v2.x + v3.x) / 2, (v2.y + v3.y) / 2, (v2.z + v3.z) / 2 });
         mid31.Normalize({ (v3.x + v1.x) / 2, (v3.y + v1.y) / 2, (v3.z + v1.z) / 2 });

         midv1.Normalize({ (v1.x + v.x) / 2, (v1.y + v.y) / 2, (v1.z + v.z) / 2 });
         midv2.Normalize({ (v2.x + v.x) / 2, (v2.y + v.y) / 2, (v2.z + v.z) / 2 });
         midv3.Normalize({ (v3.x + v.x) / 2, (v3.y + v.y) / 2, (v3.z + v.z) / 2 });

         cross32v.CrossProduct(midv3, midv2);
         cross13v.CrossProduct(midv1, midv3);
         cross21v.CrossProduct(midv2, midv1);

         // b[0] = sphericalTriArea(v2, v3, v) / omega;
         b[0] = asin(Max(-1.0, Min(1.0, mid23.DotProduct(cross32v)))) * 2 / omega;
         // b[1] = sphericalTriArea(v3, v1, v) / omega;
         b[1] = asin(Max(-1.0, Min(1.0, mid31.DotProduct(cross13v)))) * 2 / omega;
         // b[2] = sphericalTriArea(v1, v2, v) / omega;
         b[2] = asin(Max(-1.0, Min(1.0, mid12.DotProduct(cross21v)))) * 2 / omega;
      }
      baryToCartesian(b, out, p1, p2, p3);
   }
}
