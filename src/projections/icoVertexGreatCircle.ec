// This implements the Slice & Dice Vertex-oriented great circle projection for an icosahedron
// https://doi.org/10.1559/152304006779500687
public import IMPORT_STATIC "ecere"
private:

import "ri5x6"

#ifdef ECERE_STATIC
// 3D Math is excluded from libecere's static config
import "Vector3D"

#define Vector3D DGGVector3D
#endif

public enum VGCRadialVertex { isea, ivea, rtea };

public class VertexGreatCircleIcosahedralProjection : RI5x6Projection
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
               alpha = Degrees { 90 };
               beta = Degrees { 60 };
               gamma = Degrees { 36 };
               AB = acos(sqrt((phi + 1)/3));
               AC = atan(1/phi);
               BC = atan(2/(phi*phi));
               break;
            case ivea:
               alpha = Degrees { 90 };
               beta = Degrees { 36 };
               gamma = Degrees { 60 };
               AB = atan(1/phi);
               AC = acos(sqrt((phi + 1)/3));
               BC = atan(2/(phi*phi));
               break;
            case rtea:
               alpha = Degrees { 36 };
               beta = Degrees { 90 };
               gamma = Degrees { 60 };
               AB = atan(1/phi);
               AC = atan(2/(phi*phi));
               BC = acos(sqrt((phi + 1)/3));
               break;
         }
         cosAB = cos(AB);
      }
   }

   Radians beta, gamma, alpha;
   Radians AB, AC, BC, cosAB;

#if 0
   void backup()
   {
      Radians minusRhoMinusDelta = upOverupPvp * (beta + gamma - Pi/2) - beta - gamma;
      Radians x, delta, rho = atan2(-cos(minusRhoMinusDelta), -(cosAB + sin(minusRhoMinusDelta)));

      if(rho < 0) rho = 0;
      else if(rho > beta) rho = beta;
      delta = -rho -minusRhoMinusDelta;
      if(delta < 0) delta = 0;
      else if(delta > Pi) delta = Pi;

      if(rho < 1E-9)
         cosXpY = cosAB;
      else
      {
         cosXpY = 1/(tan(rho) * tan(delta));
         if(cosXpY > 1) cosXpY = 1;
         else if(cosXpY < -1) cosXpY = -1;
      }
      x = acos(1 - xpOverxpPlusyp * xpOverxpPlusyp * (1 - cosXpY));
      {
         Vector3D D, P;
         Radians BD = acos(cosXpY);
         Radians AD = 2 * atan(tan((rho + delta - Pi/2) /2) / tan(AB/2));
         if(fabs(rho - beta) < 1E-9)
            AD = AC, BD = BC;

         slerpAngle(D, A, C, AC, AD);
         slerpAngle(P, B, D, BD, x);
         cartesianToGeo(P, out);
      }
   }
#else
   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   static void inversePointIn6thTriangle(const Pointd pi,
      const Pointd pai, const Pointd pbi, const Pointd pci,
      const Vector3D A, const Vector3D B, const Vector3D C,
      GeoPoint out)
  {
      // Compute D' by finding interesection between line extending from B' through P' with A'C'
      // A = y1-y2, B = x2-x1, C = Ax1 + By1

      // a1, b1, c1 is P'B'
      // a1 * pbi.x + b1 * pbi.y = c1, a1 *  pi.x + b1 * pi.y = c1
      double a1 = pbi.y -  pi.y, b1 = pi.x  - pbi.x, c1 = a1 * pbi.x + b1 * pbi.y;

      // a2, b2, c2 is A'C'
      // a2 * pci.x + b2 * pci.y = c2, a2 * pai.x + b2* pai.y = c2
      // Constant for each icosahedron sub-triangle
      double a2 = pci.y - pai.y, b2 = pai.x - pci.x, c2 = a2 * pci.x + b2 * pci.y;
      /*
      double tc1 = a1 * pbi.x + b1 * pbi.y; // == c1 ?
      double tc2 = a1 * pi.x + b1 * pi.y;   // == c1 ?
      double tc3 = a2 * pci.x + b2 * pci.y; // == c2 ?
      double tc4 = a2 * pai.x + b2 * pai.y; // == c2 ?

      // if(!a2 && !b2) Print("");
      */
      double div = (a1*b2 - a2*b1);
      Pointd pdi { (c1*b2 - c2*b1) / div, (a1*c2 - a2*c1) / div };
      Pointd dcp { pci.x - pdi.x, pci.y - pdi.y };
      double up = sqrt(dcp.x * dcp.x + dcp.y * dcp.y);

      // uvp = 2215425.1811446822; // Constant if using planar ISEA (which might be more expensive than this sqrt)
      Pointd ac { pci.x - pai.x, pci.y - pai.y };
      double uvp = sqrt(ac.x * ac.x + ac.y * ac.y);
      double upOverupPvp = up / uvp;

      // if(upOverupPvp < -1E-9 || upOverupPvp > 1 + 1E-9) Print("bug");
      Pointd bp { pi.x - pbi.x, pi.y - pbi.y };
      Pointd pdp { pdi.x - pi.x, pdi.y - pi.y };
      double xp = sqrt(bp.x * bp.x + bp.y * bp.y);
      double yp = sqrt(pdp.x * pdp.x + pdp.y * pdp.y);
      double xpOverxpPlusyp = xp / (xp + yp);
      double cosXpY;
      // if(xpOverxpPlusyp < 0 || xpOverxpPlusyp > 1) Print("bug");
      // Area of spherical triangle: sum of angles - Pi

      // checkAreaDBC / checkAreaABC = upOverupPvp
      Radians areaABC = beta + gamma + alpha - Pi;
      Radians rhoPlusDelta = beta + gamma - upOverupPvp * (beta + gamma + alpha - Pi);
      Radians areaABD = rhoPlusDelta + alpha - Pi;  // T-U = rho + delta + alpha - Pi
      // Radians areaDBC = areaABC - areaABD; // U = beta + gamma + alpha - Pi - (rho + delta + alpha - Pi) = beta + gamma - rho - delta
      Radians x, delta, rho;
      Radians AD, BD;

      if(!a1 && !b1)
      {
         cartesianToGeo(B, out);
         return;
      }

      if(fabs(areaABD - 0) < 1E-9)
         rho = 0;
      else if(fabs(areaABD - areaABC) < 1E-9)
         rho = beta;
      else
      {
         #if 0
         // These formulas likely simplifies further to the one below
         Radians S = (areaABD + Pi) / 2;
         double tanABO2 = tan(AB/2);
         double cosS = cos(S), sinS = sin(S);
         double cosY = cos(rhoPlusDelta), sinY = sin(rhoPlusDelta);
         double K = tanABO2 * tanABO2 * cos(S - alpha) / -cosS;
         rho = atan2(
            -(K * cosS - (cosS * cosY + sinS * sinY)),
             (K * sinS - (cosS * sinY - sinS * cosY))
         );
         #else
         // From John Hall DT DGGS (page 39: https://ucalgary.scholaris.ca/items/1bd11f8c-5a71-48dc-a9a8-b4a8b9021008)
         // originally from: S. Lee and D. Mortari, 2017
         //    Quasi‐equal area subdivision algorithm for uniform points on a sphere with application to any geographical data distribution. 103:142–151.
         rho = atan2(
            -(cos(rhoPlusDelta) + cos(alpha)),
            -(sin(alpha) * cos(AB) - sin(rhoPlusDelta))
         );
         #endif
      }
      if(rho < 0) rho = 0;
      else if(rho > beta + 1E-5)
         rho = beta;

      delta = rhoPlusDelta - rho;

      if(delta < 0) delta = 0;
      else if(delta > Pi + 1E-5)
         delta = alpha;

      /*
      Radians checkAreaABD = alpha + rho + delta - Pi;
      Radians checkAreaDBC = (beta - rho) + (Pi - delta) + gamma - Pi;
      Radians checkAreaABC = checkAreaABD + checkAreaDBC;
      double checkRatio = checkAreaDBC / checkAreaABC; // Should be equal to upOverupPvp
      */

      if(fabs(rho - beta) < 1E-9)
         AD = AC;
      else if(fabs(rho - 0) < 1E-9)
         AD = 0;
      else
      {
         Radians S = (areaABD + Pi) / 2;
         AD = 2 * atan2(
            sqrt(-cos(S)         * cos(S - rho)),
            sqrt( cos(S - alpha) * cos(S - delta))
         );
      }

      if(fabs(rho - 0) < 1E-9)
         BD = AB;
      else if(fabs(rho - beta) < 1E-9)
         BD = BC;
      else
      {
         Radians S = (areaABD + Pi) / 2;
         BD = 2 * atan2(
            sqrt(-cos(S)       * cos(S - alpha)),
            sqrt( cos(S - rho) * cos(S - delta))
         );
      }
      cosXpY = cos(BD);

      /*
      double test = (beta + gamma - rho - delta) / (beta + gamma + alpha - Pi); // Should be same as upOverupPvp
      Radians abcArea = beta + gamma + alpha - Pi;
      Radians abdArea = rho + delta + alpha - Pi;
      Radians dbcArea = beta + gamma - rho - delta; // dbcArea + abdArea = abcArea
      */

      // Law of sines
      /*
      double ra = sin(alpha) / sin(BD);
      double rb = sin(rho) / sin(AD);
      double rc = sin(delta) / sin(AB);
      */

      //  (x' / (x' + y')) ^ 2 = ( 1 - cos x ) / (1 - cos (x + y))
      x = acos(1 - xpOverxpPlusyp * xpOverxpPlusyp * (1 - cosXpY));
      if(!x)
      {
         cartesianToGeo(B, out);
         return;
      }

      /* Cosine laws
      cos(a) = cos(b) cos(c) + sin(b) sin(c) cos(A)
      cos(b) = cos(c) cos(a) + sin(c) sin(a) cos(B)
      cos(c) = cos(a) cos(b) + sin(a) sin(b) cos(C)

      where:
         a = AB = atan(1/phi)
         b = AD
         c = BD = acos(cosXpY)
         A = delta
         B = rho
         C = alpha
      */
      {
         Vector3D D, P;

         slerpAngle(D, A, C, AC, AD);
         slerpAngle(P, B, D, BD, x);
         cartesianToGeo(P, out);

         /*
         Pointd test;
         Radians PA = angleBetweenUnitVectors(P, A);
         forwardPointIn6thTriangle(P, A, B, C, pai, pbi, pci, test);
         */
      }
  }
#endif

   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   void inverseIcoFace(const Pointd v,
      const Pointd p1, const Pointd p2, const Pointd p3,
      const Vector3D v1, const Vector3D v2, const Vector3D v3,
      GeoPoint out)
   {
      double b[3];
      Pointd pCenter {
         (p1.x + p2.x + p3.x) / 3,
         (p1.y + p2.y + p3.y) / 3
      };
      Pointd pMid;
      Vector3D vCenter {
         (v1.x + v2.x + v3.x) / 3,
         (v1.y + v2.y + v3.y) / 3,
         (v1.z + v2.z + v3.z) / 3
      };
      Vector3D vMid;
      const Pointd * p5x6[3] = { &pMid, null, &pCenter };
      const Vector3D * v3D[3] = { &vMid, null, &vCenter };

      vCenter.Normalize(vCenter);
      cartesianToBary(b, v, p1, p2, p3);

      if(b[0] <= b[1] && b[0] <= b[2])
      {
         pMid = { (p2.x + p3.x) / 2, (p2.y + p3.y) / 2 };
         vMid = { (v2.x + v3.x) / 2, (v2.y + v3.y) / 2, (v2.z + v3.z) / 2 };

         if(b[1] < b[2])
            p5x6[1] = p3, v3D[1] = v3;
         else
            p5x6[1] = p2, v3D[1] = v2;
      }
      else if(b[1] <= b[0] && b[1] <= b[2])
      {
         pMid = { (p3.x + p1.x) / 2, (p3.y + p1.y) / 2 };
         vMid = { (v3.x + v1.x) / 2, (v3.y + v1.y) / 2, (v3.z + v1.z) / 2 };

         if(b[0] < b[2])
            p5x6[1] = p3, v3D[1] = v3;
         else
            p5x6[1] = p1, v3D[1] = v1;
      }
      else // if(b[2] <= b[0] && b[2] <= b[1])
      {
         pMid = { (p1.x + p2.x) / 2, (p1.y + p2.y) / 2 };
         vMid = { (v1.x + v2.x) / 2, (v1.y + v2.y) / 2, (v1.z + v2.z) / 2 };

         if(b[0] < b[1])
            p5x6[1] = p2, v3D[1] = v2;
         else
            p5x6[1] = p1, v3D[1] = v1;
      }
      vMid.Normalize(vMid);

      switch(radialVertex)
      {
         case ivea: inversePointIn6thTriangle(v, p5x6[0], p5x6[1], p5x6[2], v3D[0], v3D[1], v3D[2], out); break;
         case isea: inversePointIn6thTriangle(v, p5x6[0], p5x6[2], p5x6[1], v3D[0], v3D[2], v3D[1], out); break;
         case rtea: inversePointIn6thTriangle(v, p5x6[1], p5x6[0], p5x6[2], v3D[1], v3D[0], v3D[2], out); break;
      }
      revertOrientation(out, out);
      out.lat = latAuthalicToGeodetic(out.lat);
      out.lon = wrapLon(out.lon);
   }

   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   static void forwardPointIn6thTriangle(const Vector3D P,
      const Vector3D A, const Vector3D B, const Vector3D C,
      const Pointd pai, const Pointd pbi, const Pointd pci,
      Pointd out)
  {
      Radians x = angleBetweenUnitVectors(B, P); // x should be < AB < BC
      if(fabs(x) < 1E-9)
         out = pbi;
      else
      {
         Radians PA = angleBetweenUnitVectors(P, A);
         //double EABP = PA + x + AB - Pi;
         // Cosine formula does not work here -- rho = acos(cos(PA) - cos(AB) * cos(x)) / sin(AB) * sin(x)
         // Half-angle formula:
         double s = (x + PA + AB) / 2;
         Radians rho = fabs(PA) < 1E-9 ? 0 : 2*asin(sqrt(sin(s - x) * sin(s - AB) / (sin(x) * sin(AB))));
         Radians delta = acos(sin(rho) * cosAB);
         double upOverupPvp = (beta + gamma - rho - delta) / (beta + gamma - Pi/2); // This should be between 0 and 1
         double cosXpY = rho < 1E-9 ? cosAB : 1/(tan(rho) * tan(delta));
         // double y = acos(cosXpY) - x;
         double xpOverxpPlusyp = sqrt((1 - cos(x)) / (1 - cosXpY)); // This should be between 0 and 1
         Pointd pdi { pci.x + (pai.x - pci.x) * upOverupPvp, pci.y + (pai.y - pci.y) * upOverupPvp };

         out = { pbi.x + (pdi.x - pbi.x) * xpOverxpPlusyp, pbi.y + (pdi.y - pbi.y) * xpOverxpPlusyp };
      }
  }

   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   void forwardIcoFace(const Vector3D v,
      const Vector3D v1, const Vector3D v2, const Vector3D v3,
      const Pointd p1, const Pointd p2, const Pointd p3,
      Pointd out)
   {
      Pointd pCenter = {
         (p1.x + p2.x + p3.x) / 3,
         (p1.y + p2.y + p3.y) / 3
      };
      Vector3D vCenter {
         (v1.x + v2.x + v3.x) / 3,
         (v1.y + v2.y + v3.y) / 3,
         (v1.z + v2.z + v3.z) / 3
      };
      vCenter.Normalize(vCenter);

      if(vertexWithinSphericalTri(v, vCenter, v2, v3))
      {
         Pointd p23 { (p2.x + p3.x) / 2, (p2.y + p3.y) / 2 };
         Vector3D v23 { (v2.x + v3.x) / 2, (v2.y + v3.y) / 2, (v2.z + v3.z) / 2 };
         v23.Normalize(v23);

         if(vertexWithinSphericalTri(v, vCenter, v23, v3))
            forwardPointIn6thTriangle(v, v23, v3, vCenter, p23, p3, pCenter, out);
         else
            forwardPointIn6thTriangle(v, v23, v2, vCenter, p23, p2, pCenter, out);
      }
      else if(vertexWithinSphericalTri(v, vCenter, v3, v1))
      {
         Pointd p31 { (p3.x + p1.x) / 2, (p3.y + p1.y) / 2 };
         Vector3D v31 { (v3.x + v1.x) / 2, (v3.y + v1.y) / 2, (v3.z + v1.z) / 2 };
         v31.Normalize(v31);

         if(vertexWithinSphericalTri(v, vCenter, v31, v3))
            forwardPointIn6thTriangle(v, v31, v3, vCenter, p31, p3, pCenter, out);
         else
            forwardPointIn6thTriangle(v, v31, v1, vCenter, p31, p1, pCenter, out);
      }
      else // if(vertexWithinSphericalTri(v, vCenter, v1, v2))
      {
         Pointd p12 { (p1.x + p2.x) / 2, (p1.y + p2.y) / 2 };
         Vector3D v12 { (v1.x + v2.x) / 2, (v1.y + v2.y) / 2, (v1.z + v2.z) / 2 };
         v12.Normalize(v12);

         if(vertexWithinSphericalTri(v, vCenter, v12, v2))
            forwardPointIn6thTriangle(v, v12, v2, vCenter, p12, p2, pCenter, out);
         else
            forwardPointIn6thTriangle(v, v12, v1, vCenter, p12, p1, pCenter, out);
      }
   }
}

class RTEAProjection : VertexGreatCircleIcosahedralProjection
{
   radialVertex = rtea;
}
