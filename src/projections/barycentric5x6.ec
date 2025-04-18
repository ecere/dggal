public import IMPORT_STATIC "ecrt"
private:

import "ri5x6"

import "Vector3D"

class GoldbergPolyhedraProjection : BarycentricSphericalTriAreaProjection
{
   projectedGP = true;
}

class BarycentricSphericalTriAreaProjection : RI5x6Projection
{
   bool projectedGP; projectedGP = false;

   // TODO: This does not yet implement the foward projection (e.g., for 'zone' lookups)

   __attribute__ ((unused))
   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   void inverseIcoFace(const Pointd v,
      const Pointd p1, const Pointd p2, const Pointd p3,
      const Vector3D v1, const Vector3D v2, const Vector3D v3,
      GeoPoint out)
   {
      double b[3];
      Vector3D c;
      double u1, u2, u3;

      cartesianToBary(b, v, p1, p2, p3);
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
      c = {
         u1 * v1.x + u2 * v2.x + u3 * v3.x,
         u1 * v1.y + u2 * v2.y + u3 * v3.y,
         u1 * v1.z + u2 * v2.z + u3 * v3.z
      };

      cartesianToGeo(c, out);

      revertOrientation(out, out);
      out.lat = latAuthalicToGeodetic(out.lat);
   }
}
