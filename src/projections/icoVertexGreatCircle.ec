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
      double beta[3];

      cartesianToBary(beta, pi, pai, pbi, pci, -6);

      if(beta[0] > 1 - 1E-15)
         P = A;
      else
      {
         double rho = 1 - beta[0]; // Also = b[1] + b[2]
         double alpha = (beta[2] / rho) * areaABC;

         // Note that these could be pre-computed per SDT
         double c1 = parallelepipedV * parallelepipedV;
         double v2dotv0 = C.x * A.x + C.y * A.y + C.z * A.z;
         double v1dotv2 = B.x * C.x + B.y * C.y + B.z * C.z;
         double v0dotv1 = A.x * B.x + A.y * B.y + A.z * B.z;
         double v0dotv1tv1dotv2mv2dotv0 = v0dotv1 * v1dotv2 - v2dotv0;
         double v2dotv0pv1dotv2 = v2dotv0 + v1dotv2;
         double v0dotv1p1 = v0dotv1 + 1, v0dotv1p1s = v0dotv1p1 * v0dotv1p1;
         double c2 = v2dotv0pv1dotv2 * v2dotv0pv1dotv2 - v0dotv1p1s;
         double c3 = 2 * v0dotv1p1 * v0dotv1tv1dotv2mv2dotv0;
         double c4 = (1 - v1dotv2 * v1dotv2) * v0dotv1p1s +
            v0dotv1tv1dotv2mv2dotv0 * v0dotv1tv1dotv2mv2dotv0;
         double c1o = c1 + c2, c2o = c3;
         double cMo = c1 + c4;
         double c1c = c1 - c2;
         double c2c = -c3;
         double cMc = c1 - c4;
         double c1s = -2 * parallelepipedV * v2dotv0pv1dotv2;
         double c2s =  2 * parallelepipedV * v0dotv1p1;
         double cMs =  2 * parallelepipedV * v0dotv1tv1dotv2mv2dotv0;
         /////// end of per SDT pre-computables

         double cosAlpha = cos(alpha), sinAlpha = sin(alpha);
         double N1 = c1o + c1c * cosAlpha + c1s * sinAlpha;
         double N2 = c2o + c2c * cosAlpha + c2s * sinAlpha;
         double M  = cMo + cMc * cosAlpha + cMs * sinAlpha;
         double ooM = 1.0 / M;
         double N1oM = ooM * N1, N2oM = ooM * N2;
         Vector3D Dv {
            N1oM * B.x + N2oM * C.x,
            N1oM * B.y + N2oM * C.y,
            N1oM * B.z + N2oM * C.z
         };
         double a = A.x * Dv.x + A.y * Dv.y + A.z * Dv.z;
         double b = 1 + rho * rho * (a - 1);
         double bp = rho * sqrt((1 + b)/(1 + a));
         double ap = b - a * bp;

         P = {
            ap * A.x + bp * Dv.x,
            ap * A.y + bp * Dv.y,
            ap * A.z + bp * Dv.z
         };
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
       // The SDT triangle area is always 6 degrees
      static const Radians areaABC = Degrees { 6 }; //sphericalTriArea(A, B, C);
      // P is v, A is v0, B is v1, C is v2
      // NOTE: c2 could be pre-computed for a given SDT...
      Vector3D c2 {
         B.y * C.z - B.z * C.y,
         B.z * C.x - B.x * C.z,
         B.x * C.y - B.y * C.x
      };
      double c2dotv = c2.x * v.x + c2.y * v.y + c2.z * v.z;
      Vector3D D0 {
         parallelepipedV * v.x - c2dotv * A.x,
         parallelepipedV * v.y - c2dotv * A.y,
         parallelepipedV * v.z - c2dotv * A.z
      };
      double D = D0.length, ooD = D ? 1.0 / D : 1;
      Vector3D Dv { D0.x * ooD, D0.y * ooD, D0.z * ooD };
      Radians areaABp = Max(0.0, sphericalTriArea(A, B, Dv));
      double alpha = areaABp / areaABC;
      double rho = D / parallelepipedV * sqrt(
         (1 + A.x * Dv.x + A.y * Dv.y + A.z * Dv.z) /
         (1 + A.x *  v.x + A.y *  v.y + A.z *  v.z)
      );
      double b[3] = {
         1 - rho,
         rho * (1 - alpha),
         alpha * rho
      };

      baryToCartesian(b, out, pai, pbi, pci);
   }

   private static inline int findSubTri(int face, const Vector3D v)
   {
      int best3rd = 0, best6th = 0, i;
      double d, bestDot = -MAXDOUBLE;
      const Vector3D * c;
      // This is the cosine of the half distance to the nearest centroid over all (which is on different icosahedron triangle)
      // cos(15.4210090071508551/2)
      // (sqrt(6)*sqrt(480 + 216*sqrt(5) + sqrt(3)*sqrt(2*sqrt(5) + 5)*(47*sqrt(5) + 109)))/(6*sqrt(80 + 36*sqrt(5) + sqrt(3)*sqrt(2*sqrt(5) + 5)*(8*sqrt(5) + 19)))

      // This is the cosine of the half distance to the nearest centroid within the same icosahedron face
      // cos(22.8025282605464/2)
      // sqrt((5 + 5/sqrt(5) + 8*(1 + 2/sqrt(5))/sqrt(3 + 6/sqrt(5))) / (2*(3 + 2/sqrt(5) + 4*(1 + 2/sqrt(5))/sqrt(3 + 6/sqrt(5)))))
      static const double earlyAccept3rd = 0.9802668134226932631948092150332116;

      // Cosine of the half distance between the two sibling SDT's centroids (within same 1/3rd face)
      // cos(21.2120235137118/2)
      /*{ // These are symbological representations of that value using radicals (which unfortunately does not seem to simplify further)
         double s5 = sqrt(5), r2 = sqrt(2), r3 = sqrt(3), r6 = sqrt(6), r10 = sqrt(10), r15 = sqrt(15), r30 = sqrt(30);
         double A = sqrt(s5 + 5), B = sqrt(2*s5 + 5), C = sqrt(3*s5 + 7), D = sqrt(4*s5 + 9), E = sqrt(11*s5 + 25);
         double G = sqrt(29*s5 + 65), H = sqrt((s5 + 3) * (s5 + 3) * (s5 + 3));
         double tv1 = 3 * r2 * C, tv2 = r6 * H, tv3 = 3 * (r2 + r10) * C, tv4 = 2 * r6 * H;
         double F1 = 6 * E + 12 * r2 * D + r6 * H * A;
         double F2 = 3 * r2 * (r2 + r10) * E + 2 * r6 * H * A + 12 * r2 * (1 + s5) * D;
         double L1 = 3 * r30 * E + 7 * r6 * E + 18 * s5 * B + 42 * B + 8 * r15 * D + 20 * r3 * D + 99 * s5 + 225;
         double M1 = r6 * (s5 + 3) * A + 3 * r2 * G + 3 * r2 * (1 + s5) * C;
         double M2 = s5 * A + 3 * A + 2 * r3 * C + r3 * G;
         double P = 25 * 11 * 509 * s5 + 5 * 59 * 1061 + (4 * r6 * (199*s5 + 445)) * B * E
              + 20 * D * ((r2 * (55*s5 + 123)) * E + (r3 * (110*s5 + 246)) * B)
              + 60 * ((r6 * (72*s5 + 161)) * E + ((432*s5 + 2 * 3 * 7 * 23)) * B + (r3 * (199*s5 + 445)) * D);
         double sqrtP = sqrt(P);
         double P_quarter = sqrt(sqrt(P));
         double S = r6 * r10 * (4 * (tv1 + tv2) * F1 + (tv3 + tv4) * F2);
         double inner = S + 48 * (24 * sqrtP + (r6 * M1 * M2 * sqrtP) / L1);
         double result = sqrt(inner) / (48 * P_quarter);
         printf("Final result = %.15f\n", result);
      }*/
      // sqrt(sqrt(6)*(sqrt(10)*(4*(3*sqrt(2)*sqrt(3*sqrt(5) + 7) + sqrt(6)*(sqrt(5) + 3)^(1.5))*(6*sqrt(11*sqrt(5) + 25) + 12*sqrt(2)*sqrt(4*sqrt(5) + 9) + sqrt(6)*(sqrt(5) + 3)^(1.5)*sqrt(sqrt(5) + 5)) + (3*(sqrt(2) + sqrt(10))*sqrt(3*sqrt(5) + 7) + 2*sqrt(6)*(sqrt(5) + 3)^(1.5))*(3*sqrt(2)*(sqrt(2) + sqrt(10))*sqrt(11*sqrt(5) + 25) + 2*sqrt(6)*(sqrt(5) + 3)^(1.5)*sqrt(sqrt(5) + 5) + 12*sqrt(2)*(1 + sqrt(5))*sqrt(4*sqrt(5) + 9)))*(3*sqrt(30)*sqrt(11*sqrt(5) + 25) + 7*sqrt(6)*sqrt(11*sqrt(5) + 25) + 18*sqrt(5)*sqrt(2*sqrt(5) + 5) + 42*sqrt(2*sqrt(5) + 5) + 8*sqrt(15)*sqrt(4*sqrt(5) + 9) + 20*sqrt(3)*sqrt(4*sqrt(5) + 9) + 99*sqrt(5) + 225) + 48*(sqrt(6)*(sqrt(5) + 3)*sqrt(sqrt(5) + 5) + 3*sqrt(2)*sqrt(29*sqrt(5) + 65) + 3*sqrt(2)*(1 + sqrt(5))*sqrt(3*sqrt(5) + 7))*(sqrt(5)*sqrt(sqrt(5) + 5) + 3*sqrt(sqrt(5) + 5) + 2*sqrt(3)*sqrt(3*sqrt(5) + 7) + sqrt(3)*sqrt(29*sqrt(5) + 65))*sqrt(398*sqrt(5)*(11*sqrt(5) + 25) + 2388*sqrt(5)*(2*sqrt(5) + 5) + 1520*sqrt(5)*(4*sqrt(5) + 9) + 796*sqrt(30)*sqrt(2*sqrt(5) + 5)*sqrt(11*sqrt(5) + 25) + 1780*sqrt(6)*sqrt(2*sqrt(5) + 5)*sqrt(11*sqrt(5) + 25) + 1100*sqrt(10)*sqrt(4*sqrt(5) + 9)*sqrt(11*sqrt(5) + 25) + 2460*sqrt(2)*sqrt(4*sqrt(5) + 9)*sqrt(11*sqrt(5) + 25) + 2200*sqrt(15)*sqrt(2*sqrt(5) + 5)*sqrt(4*sqrt(5) + 9) + 4920*sqrt(3)*sqrt(2*sqrt(5) + 5)*sqrt(4*sqrt(5) + 9) + 4320*sqrt(30)*sqrt(11*sqrt(5) + 25) + 9660*sqrt(6)*sqrt(11*sqrt(5) + 25) + 25920*sqrt(5)*sqrt(2*sqrt(5) + 5) + 57960*sqrt(2*sqrt(5) + 5) + 11940*sqrt(15)*sqrt(4*sqrt(5) + 9) + 26700*sqrt(3)*sqrt(4*sqrt(5) + 9) + 104405*sqrt(5) + 236825)) + 1152*(3*sqrt(30)*sqrt(11*sqrt(5) + 25) + 7*sqrt(6)*sqrt(11*sqrt(5) + 25) + 18*sqrt(5)*sqrt(2*sqrt(5) + 5) + 42*sqrt(2*sqrt(5) + 5) + 8*sqrt(15)*sqrt(4*sqrt(5) + 9) + 20*sqrt(3)*sqrt(4*sqrt(5) + 9) + 99*sqrt(5) + 225)*sqrt(398*sqrt(5)*(11*sqrt(5) + 25) + 2388*sqrt(5)*(2*sqrt(5) + 5) + 1520*sqrt(5)*(4*sqrt(5) + 9) + 796*sqrt(30)*sqrt(2*sqrt(5) + 5)*sqrt(11*sqrt(5) + 25) + 1780*sqrt(6)*sqrt(2*sqrt(5) + 5)*sqrt(11*sqrt(5) + 25) + 1100*sqrt(10)*sqrt(4*sqrt(5) + 9)*sqrt(11*sqrt(5) + 25) + 2460*sqrt(2)*sqrt(4*sqrt(5) + 9)*sqrt(11*sqrt(5) + 25) + 2200*sqrt(15)*sqrt(2*sqrt(5) + 5)*sqrt(4*sqrt(5) + 9) + 4920*sqrt(3)*sqrt(2*sqrt(5) + 5)*sqrt(4*sqrt(5) + 9) + 4320*sqrt(30)*sqrt(11*sqrt(5) + 25) + 9660*sqrt(6)*sqrt(11*sqrt(5) + 25) + 25920*sqrt(5)*sqrt(2*sqrt(5) + 5) + 57960*sqrt(2*sqrt(5) + 5) + 11940*sqrt(15)*sqrt(4*sqrt(5) + 9) + 26700*sqrt(3)*sqrt(4*sqrt(5) + 9) + 104405*sqrt(5) + 236825))/(48*sqrt(3*sqrt(30)*sqrt(11*sqrt(5) + 25) + 7*sqrt(6)*sqrt(11*sqrt(5) + 25) + 18*sqrt(5)*sqrt(2*sqrt(5) + 5) + 42*sqrt(2*sqrt(5) + 5) + 8*sqrt(15)*sqrt(4*sqrt(5) + 9) + 20*sqrt(3)*sqrt(4*sqrt(5) + 9) + 99*sqrt(5) + 225)*(398*sqrt(5)*(11*sqrt(5) + 25) + 2388*sqrt(5)*(2*sqrt(5) + 5) + 1520*sqrt(5)*(4*sqrt(5) + 9) + 796*sqrt(30)*sqrt(2*sqrt(5) + 5)*sqrt(11*sqrt(5) + 25) + 1780*sqrt(6)*sqrt(2*sqrt(5) + 5)*sqrt(11*sqrt(5) + 25) + 1100*sqrt(10)*sqrt(4*sqrt(5) + 9)*sqrt(11*sqrt(5) + 25) + 2460*sqrt(2)*sqrt(4*sqrt(5) + 9)*sqrt(11*sqrt(5) + 25) + 2200*sqrt(15)*sqrt(2*sqrt(5) + 5)*sqrt(4*sqrt(5) + 9) + 4920*sqrt(3)*sqrt(2*sqrt(5) + 5)*sqrt(4*sqrt(5) + 9) + 4320*sqrt(30)*sqrt(11*sqrt(5) + 25) + 9660*sqrt(6)*sqrt(11*sqrt(5) + 25) + 25920*sqrt(5)*sqrt(2*sqrt(5) + 5) + 57960*sqrt(2*sqrt(5) + 5) + 11940*sqrt(15)*sqrt(4*sqrt(5) + 9) + 26700*sqrt(3)*sqrt(4*sqrt(5) + 9) + 104405*sqrt(5) + 236825)^(0.25))
      static const double earlyAccept6th = 0.9829160426524585629980328985973081873244;

      for(i = 0; i < 3; i++)
      {
         c = &ico3rdCentroids[face][i];
         d = c->x * v.x + c->y * v.y + c->z * v.z;
         if(d > bestDot)
         {
            bestDot = d, best3rd = i;
            if(d > earlyAccept3rd)
               break;
         }
      }

      bestDot = -MAXDOUBLE;
      for(i = 0; i < 2; i++)
      {
         c = &ico6thCentroids[face][best3rd][i];
         d = c->x * v.x + c->y * v.y + c->z * v.z;
         if(d > bestDot)
         {
            bestDot = d, best6th = i;
            if(d > earlyAccept6th)
               break;
         }
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
