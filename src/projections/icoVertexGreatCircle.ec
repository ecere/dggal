/*
   This implements the Slice & Dice Vertex great circle equal area projection for an icosahedron.
   https://doi.org/10.1559/152304006779500687
   The 120 spherical triangles used correspond to those of a spherical disdyakis triacontahedron
   (the fundamental domain of the icosahedral spherical symmetry Ih).
   There are three options for the vertex from which great circles are mapped to straight lines:
   - IVEA is the vertex-oriented projection as described in the paper
   - ISEA (swapping vertices B and C) is equivalent to Snyder's 1992 projection on the icosahedron
   - RTEA (swapping vertices A and B) corresponds to extending Snyder's 1992 projection to the rhombic triacontahedron
     (the vertex in the center of the 30 rhombic faces is used as radial vertex B)

   For the trigonometric approach, most of the equations are based on basic spherical trigonometry,
   solving the spherical triangles for unknown sides and angles.

   Spherical excess:
      E = A + B + C - Pi

   Law of sines:
      sin A   sin B   sin C
      ----- = ----- = -----
      sin a   sin b   sin c

   Law of cosines (for sides):
      cos a = cos b cos c + sin b sin c cos A
      cos b = cos c cos a + sin c sin a cos B
      cos c = cos a cos b + sin a sin b cos C

   for angles:
      cos A = -cos B cos C + sin B sin C cos a
      cos B = -cos C cos A + sin C sin A cos b
      cos C = -cos A cos B + sin A sin B cos c

   Half angle formulas, e.g.:

      S = (A + B + C) / 2
                        cos(S - B) cos(S - C)
      cos(a/2) = sqrt( ---------------------- )
                            sin(B) sin(C)

   yielding, based on half angle identity:

           a               1 + cos(a)
      cos(---) = +/- sqrt( ---------- )
           2                   2

      2 cos^2(a/2) = 1 + cos(a)

      cos(a) = 2 cos^2(a/2) - 1

                            cos(S - B) cos(S - C)
      cos(a) = 2 * sqrt( ---------------------- ) ^ 2 - 1
                              sin(B) sin(C)

                2 * cos(S - B) cos(S - C)
      cos(a) = -------------------------- - 1
                      sin(B) sin(C)

   Half side formulas, e.g.:
                        sin(s) sin(s - a)
      cos(A/2) = sqrt( ---------------------- )
                          sin(b) sin(c)

   An interesting special case for RTEA (where ABD triangle does not have a right angle) is that it is possible to solve a spherical triangle when
   knowing the spherical excess (area E), one angle and its adjacent side:

    Cosine difference identity:
       cos(a - b) = cos a cos b + sin a sin b

    Y = E + Pi - B = A + C
    C = Y - A

    cos C                     = -cos A cos B + sin A sin B cos c (cosine rule for angle)
    cos(Y - A) =              = -cos A cos B + sin A sin B cos c
    cos Y cos A + sin Y sin A = -cos A cos B + sin A sin B cos c
    sin Y sin A - sin A sin B cos c = -cos A cos B - cos Y cos A
    sin A (sin Y - sin B) cos c = cos A (-cos B - cosY)
             sin A    -cos Y - cos B
    tan A = ------- = --------------------
             cos A     sin Y - sin B cos c

   which corresponds to the formula at bottom of page 39 in John Hall DT DGGS (https://ucalgary.scholaris.ca/items/1bd11f8c-5a71-48dc-a9a8-b4a8b9021008)
   citing S. Lee and D. Mortari, 2017
       Quasi‐equal area subdivision algorithm for uniform points on a sphere with application to any geographical data distribution. 103:142–151.

   However, the signs of the numerator and denumerator are negated here, which results in the correct angle when using atan2()
*/

public import IMPORT_STATIC "ecrt"
private:

import "ri5x6"
import "Vector3D"

// Define this to use the vectorial approach based on Brenton R S Recht's blog entry at
// https://brsr.github.io/2021/08/31/snyder-equal-area.html
// with further replacement of trigonometry by vector operation for the inverse as well
// The spherical trigonometry approach will still be used as a fallback for degenerate cases
// where the vectorial inverse cannot produce an accurate result.

public enum VGCRadialVertex { isea, ivea, rtea };

public class IVEAProjection : SliceAndDiceGreatCircleIcosahedralProjection
{
}

public class ISEAProjection : SliceAndDiceGreatCircleIcosahedralProjection
{
   radialVertex = isea;
}

public class RTEAProjection : SliceAndDiceGreatCircleIcosahedralProjection
{
   radialVertex = rtea;
}

public class SliceAndDiceGreatCircleIcosahedralProjection : RI5x6Projection
{
   VGCRadialVertex radialVertex; property::radialVertex = ivea;

   property VGCRadialVertex radialVertex
   {
      set
      {
         radialVertex = value;
         switch(value)
         {
            case isea:
               va = 0, vb = 2, vc = 1;
               alpha = Degrees { 90 };
               beta = Degrees { 60 };
               gamma = Degrees { 36 };
               AB = acos(sqrt((phi + 1)/3));
               AC = atan(1/phi);
               BC = atan(2/(phi*phi));
               break;
            case ivea:
               va = 0, vb = 1, vc = 2;
               alpha = Degrees { 90 };
               beta = Degrees { 36 };
               gamma = Degrees { 60 };
               AB = atan(1/phi);
               AC = acos(sqrt((phi + 1)/3));
               BC = atan(2/(phi*phi));
               break;
            case rtea:
               va = 1, vb = 0, vc = 2;
               alpha = Degrees { 36 };
               beta = Degrees { 90 };
               gamma = Degrees { 60 };
               AB = atan(1/phi);
               AC = atan(2/(phi*phi));
               BC = acos(sqrt((phi + 1)/3));
               break;
         }
         // poleFixIVEA = value == ivea;
         cosAB = cos(AB);
         cosAC = cos(AC);
         sinAC = sin(AC);
         cosBC = cos(BC);

      }
   }

   Radians beta, gamma, alpha;
   Radians AB, AC, BC;
   double cosAB, sinAC, cosAC, cosBC;
   int va, vb, vc;

   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   static void inverseVector(const Pointd pi,
      const Pointd pai, const Pointd pbi, const Pointd pci,
      const Vector3D A, const Vector3D B, const Vector3D C,
      Vector3D P, bool bIsA)
   {
      static const Radians areaABC = Degrees { 6 }; //sphericalTriArea(A, B, C);
      double b[3];
      Vector3D c1;

      cartesianToBary(b, pi, pai, pbi, pci, -6);

           if(b[0] > 1 - 1E-15) { P = A; return; }
      else if(b[1] > 1 - 1E-15) { P = B; return; }
      else if(b[2] > 1 - 1E-15) { P = C; return; }

      // c1.CrossProduct(B, C); constant per spherical tri
      c1.x = B.y * C.z - B.z * C.y;
      c1.y = B.z * C.x - B.x * C.z;
      c1.z = B.x * C.y - B.y * C.x;

      {
         const double h = 1 - b[0];
         const double b2oh = b[2] / h;
         const Radians b2ohABC = b2oh * areaABC;
         const double halfC = sin(b2ohABC / 2);
         const double halfC2 = halfC * halfC;
         const double CC = 2 * halfC2; // Half-angle identity; 1 - sqrt(1 - S * S) is quite imprecise
         const double S = 2 * halfC * sqrt(1 - halfC2);
         const double c01 = bIsA ? cosAB : cosBC; //A.x * B.x + A.y * B.y + A.z * B.z; //A.DotProduct(B);
         const double c12 = cosAC; //B.x * C.x + B.y * C.y + B.z * C.z; //B.DotProduct(C);
         const double c20 = bIsA ? cosBC : cosAB; //C.x * A.x + C.y * A.y + C.z * A.z; //C.DotProduct(A);
         const double s12 = sinAC; // also sqrt(1 - c12*c12) and c1.length
         const double V = A.x * c1.x + A.y * c1.y + A.z * c1.z; //A.DotProduct(c1); // Scalar triple-product of A, B, C; constant per spherical tri
         const double f = S * V + CC * (c01 * c12 - c20);
         const double g = CC * s12 * (1 + c01);
         const double f2 = f * f, g2 = g * g, gf = g * f;
         const double numerator = s12 * (f2 - g2) - 2 * gf * c12;
         const double divisor = s12 * (f2 + g2);

         if((fabs(numerator) > 1E-9 && fabs(divisor) > 1E-9))
         {
            // Optimized trigonometry-free branch equivalent to 2 SLERPs for non-degnerate cases
            const double oODivisor = 1.0 / divisor;
            const double ap = Max(0.0, numerator * oODivisor);
            const double bp = Min(1.0, 2 * gf * oODivisor);
            const Vector3D p
            {
               ap * B.x + bp * C.x,
               ap * B.y + bp * C.y,
               ap * B.z + bp * C.z
            };

            const double av = A.x * p.x + A.y * p.y + A.z * p.z; //A.DotProduct(p);
            const double bv = 1 + h*h * (av - 1);
            const double bvp = h * sqrt((1 + bv) / (1 + av));
            const double avp = bv - av * bvp;

            P =
            {
               avp * A.x + bvp * p.x,
               avp * A.y + bvp * p.y,
               avp * A.z + bvp * p.z
            };
         }
         else
         {
            // 2-SLERPs fallback for the degenerate cases where the optimized linear algebra version breaks down
            Vector3D D;
            const Radians areaABC = Degrees { 6 }; //beta + gamma + alpha - Pi;
            const double b1pb2 = b[1] + b[2];
            const double upOverupPvp = b1pb2 < 1E-11 ? 0 : b[bIsA ? 1 : 2] / b1pb2;
            const Radians rhoPlusDelta = beta + gamma - upOverupPvp * areaABC; //(beta + gamma + alpha - Pi);
            const Radians areaABD = rhoPlusDelta + alpha - Pi;  // T-U = rho + delta + alpha - Pi
            Radians BD, x;

            if(fabs(areaABD) < 1E-11)
            {
               // (B or C) vertex at angle alpha (90 degrees for ISEA and IVEA)
               if(bIsA) D = B; else D = C;
               BD = AB;
            }
            else if(fabs(areaABD - areaABC) < 1E-13)
            {
               // (B or C) vertex at angle gamma
               if(bIsA) D = C; else D = B;
               BD = BC;
            }
            else
            {
               Radians AD = 2 * atan2(g, f);
               slerpAngle(D, B, C, AC, AD);
               BD = acos(Max(-1.0, Min(1.0, A.x * D.x + A.y * D.y + A.z * D.z)));
            }

            // A is the vertex from which great circles radiate (angle beta)
            x = 2 * asin((1 - b[0]) * sin(BD/2)); // x' / (x' + y') = 1 - b_0
            slerpAngle(P, A, D, BD, x);
         }
      }
   }

   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   void inverseIcoFace(int face, const Pointd v,
      const Pointd p1, const Pointd p2, const Pointd p3,
      const Vector3D v1, const Vector3D v2, const Vector3D v3,
      Vector3D out)
   {
      double b[3];
      cartesianToBary(b, v, p1, p2, p3, -1);
      {
         int subTri =
            (b[0] <= b[1] && b[0] <= b[2]) ? (b[1] < b[2] ? 0 : 1) :
            (b[1] <= b[0] && b[1] <= b[2]) ? (b[0] < b[2] ? 2 : 3) :
          /*(b[2] <= b[0] && b[2] <= b[1]) ?*/b[0] < b[1] ? 4 : 5;
         int tri3rd = subTri >> 1;
         const Pointd * p5x6[3] =
         {
            &ico56Mids[face][tri3rd],
            subTri == 0 || subTri == 2 ? p3 :
            subTri == 1 || subTri == 4 ? p2 : p1, //subTri == 3 || subTri == 5
            &ico56Center[face]
         };
         const Vector3D * v3D[3] =
         {
            &ico3rdMids[face][tri3rd],
            subTri == 0 || subTri == 2 ? v3 :
            subTri == 1 || subTri == 4 ? v2 : v1, //subTri == 3 || subTri == 5
            &icoCentroids[face]
         };
         bool bIsA = (radialVertex == ivea) ^ (subTri == 0 || subTri == 3 || subTri == 4);
         {
            int a = vb, b = bIsA ? va : vc, c = bIsA ? vc : va;
            inverseVector(v, p5x6[a], p5x6[b], p5x6[c], v3D[a], v3D[b], v3D[c], out, bIsA);
         }
      }
   }

   private static inline double ::sqrtOneMinusDotOver2(const Vector3D a, const Vector3D b)
   {
      // This returns the equivalent of √((1 - (a ⋅ b)) / 2)
      // avoid is used to avoid catastrophic cancellation between 1 and a dot product close to 1
      // Credits to Felix Palmer @ a5geo.org for this approach using the normalized midpoint
      Vector3D midAB, c;
      double D;

      midAB.Normalize({ (a.x + b.x) / 2, (a.y + b.y) / 2, (a.z + b.z) / 2 });
      c.CrossProduct(a, midAB);
      D = c.length;
      if(D < 1E-8)
         D = Vector3D { a.x - b.x, a.y - b.y, a.z - b.z }.length / 2;
      return D;
   }

   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   static void forwardVector(const Vector3D v,
      const Vector3D A, const Vector3D B, const Vector3D C,
      const Pointd pai, const Pointd pbi, const Pointd pci,
      Pointd out)
   {
      Vector3D c1, c2, p;
      double h, b[3];
       // The SDT triangle area is always 6 degrees
      static const Radians areaABC = Degrees { 6 }; //sphericalTriArea(A, B, C);
      Radians areaABp;

      // c1.CrossProduct(A, v);
      c1.x = A.y * v.z - A.z * v.y;
      c1.y = A.z * v.x - A.x * v.z;
      c1.z = A.x * v.y - A.y * v.x;

      // c2.CrossProduct(B, C);
      c2.x = B.y * C.z - B.z * C.y;
      c2.y = B.z * C.x - B.x * C.z;
      c2.z = B.x * C.y - B.y * C.x;

      // p.CrossProduct(c1, c2);
      p.x = c1.y * c2.z - c1.z * c2.y;
      p.y = c1.z * c2.x - c1.x * c2.z;
      p.z = c1.x * c2.y - c1.y * c2.x;

      p.Normalize(p);

      areaABp = Max(0.0, sphericalTriArea(A, B, p));

      h = sqrtOneMinusDotOver2(A, v) / sqrtOneMinusDotOver2(A, p);

      b[0] = 1 - h;
      b[2] = Min(h, h * areaABp / areaABC);
      b[1] = h - b[2];
      baryToCartesian(b, out, pai, pbi, pci);
   }

   private static inline int findSubTri(int face, const Vector3D v)
   {
      int best3rd = 0, best6th = 0, i;
      double d, bestDot = -MAXDOUBLE;
      const Vector3D * c;

      for(i = 0; i < 3; i++)
      {
         c = &ico3rdCentroids[face][i];
         d = c->x * v.x + c->y * v.y + c->z * v.z;
         if(d > bestDot)
            bestDot = d, best3rd = i;
      }

      bestDot = -MAXDOUBLE;
      for(i = 0; i < 2; i++)
      {
         c = &ico6thCentroids[face][best3rd][i];
         d = c->x * v.x + c->y * v.y + c->z * v.z;
         if(d > bestDot)
            bestDot = d, best6th = i;
      }
      return 2 * best3rd + best6th;
   }

   void forwardIcoFace(int face, const Vector3D v,
      const Vector3D v1, const Vector3D v2, const Vector3D v3,
      const Pointd p1, const Pointd p2, const Pointd p3,
      Pointd out)
   {
      int subTri = findSubTri(face, v), tri3rd = subTri >> 1;
      const Pointd * p5x6[3] =
      {
         &ico56Mids[face][tri3rd],
         subTri == 0 || subTri == 2 ? p3 :
         subTri == 1 || subTri == 4 ? p2 : p1, //subTri == 3 || subTri == 5
         &ico56Center[face]
      };
      const Vector3D * v3D[3] =
      {
         &ico3rdMids[face][tri3rd],
         subTri == 0 || subTri == 2 ? v3 :
         subTri == 1 || subTri == 4 ? v2 : v1, //subTri == 3 || subTri == 5
         &icoCentroids[face]
      };
      bool bIsA = (radialVertex == ivea) ^ (subTri == 0 || subTri == 3 || subTri == 4);
      int a = vb, b = bIsA ? va : vc, c = bIsA ? vc : va;
      forwardVector(v, v3D[a], v3D[b], v3D[c], p5x6[a], p5x6[b], p5x6[c], out);
   }
}
