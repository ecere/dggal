public import IMPORT_STATIC "ecere"
private:

import "RI3H"
import "isea5x6"

public class ISEA3H : RhombicIcosahedral3H
{
   equalArea = true;

   ISEA3H() { pj = ISEAProjection { }; incref pj; }
   ~ISEA3H() { delete pj; }

   // These functions support ISEA5x6 and ISEA as additional CRSs
   I3HZone getZoneFromCRSCentroid(int level, CRS crs, const Pointd centroid)
   {
      switch(crs)
      {
         case CRS { ogc, 1534 }:
         {
            Vector3D c5x6;
            ISEA5x6Projection::fromISEAPlanar({ centroid.x, centroid.y }, c5x6);
            return (I3HZone)RhombicIcosahedral3H::getZoneFromCRSCentroid(level, 0, { c5x6.x, c5x6.y });
         }
         case CRS { ogc, 153456 }: crs = 0; // fallthrough
         default:
            return (I3HZone)RhombicIcosahedral3H::getZoneFromCRSCentroid(level, crs, centroid);
      }
      return nullZone;
   }

   void getZoneCRSCentroid(I3HZone zone, CRS crs, Pointd centroid)
   {
      switch(crs)
      {
         case CRS { ogc, 1534 }:
         {
            Pointd c5x6 = zone.centroid;
            Vector3D c;
            ISEA5x6Projection::toISEAPlanar({c5x6.x, c5x6.y }, c);
            centroid = { c.x, c.y };
            break;
         }
         case CRS { ogc, 153456 }: crs = 0; // fallthrough
         default:
            RhombicIcosahedral3H::getZoneCRSCentroid(zone, crs, centroid);
      }
   }

   int getZoneCRSVertices(I3HZone zone, CRS crs, Pointd * vertices)
   {
      switch(crs)
      {
         case CRS { ogc, 1534 }:
         {
            uint i, count = zone.getVertices(vertices);
            for(i = 0; i < count; i++)
            {
               Vector3D v;
               ISEA5x6Projection::toISEAPlanar({ vertices[i].x, vertices[i].y }, v);
               vertices[i] = { v.x, v.y };
            }
            return count;
         }
         case CRS { ogc, 153456 }: crs = 0; // fallthrough
         default: return RhombicIcosahedral3H::getZoneCRSVertices(zone, crs, vertices);
      }
   }

   Array<Pointd> getSubZoneCRSCentroids(I3HZone parent, CRS crs, int depth)
   {
      switch(crs)
      {
         case CRS { ogc, 1534 }:
         {
            int i;
            Array<Pointd> centroids = parent.getSubZoneCentroids(depth);
            if(centroids)
            {
               uint count = centroids.count;
               for(i = 0; i < count; i++)
               {
                  Vector3D c;
                  ISEA5x6Projection::toISEAPlanar({ centroids[i].x, centroids[i].y }, c);
                  centroids[i] = { c.x, c.y };
               }
            }
            return centroids;
         }
         case CRS { ogc, 153456 }: crs = 0; // fallthrough
         default:
            return RhombicIcosahedral3H::getSubZoneCRSCentroids(parent, crs, depth);
      }
   }

   void getZoneCRSExtent(I3HZone zone, CRS crs, CRSExtent extent)
   {
      switch(crs)
      {
         case CRS { ogc, 1534 }: getCRSExtentFromVertices(this, zone, crs, extent); break;
         case CRS { ogc, 153456 }: crs = 0; // fallthrough
         default:
            return RhombicIcosahedral3H::getZoneCRSExtent(zone, crs, extent);
      }
   }

   Array<Pointd> getZoneRefinedCRSVertices(I3HZone zone, CRS crs, int edgeRefinement)
   {
      switch(crs)
      {
         case CRS { ogc, 1534 }:
            return getISEAExtentFromVertices(this, zone, edgeRefinement);
         case CRS { ogc, 153456 }: crs = 0; // fallthrough
         default:
            return RhombicIcosahedral3H::getZoneRefinedCRSVertices(zone, crs, edgeRefinement);
      }
   }
}

static void getCRSExtentFromVertices(DGGRS dggrs, I3HZone zone, CRS crs, CRSExtent value)
{
   int i;
   Array<Pointd> vertices = dggrs.getZoneRefinedCRSVertices(zone, crs, 0); //, false);
   int nVertices = vertices ? vertices.count : 0;

   value.tl.x = MAXDOUBLE, value.tl.y = -MAXDOUBLE;
   value.br.x = -MAXDOUBLE, value.br.y = MAXDOUBLE;
   for(i = 0; i < nVertices; i++)
   {
      const Pointd * v = &vertices[i];
      double x = v->x, y = v->y;

      if(y < value.br.y) value.br.y = y;
      if(y > value.tl.y) value.tl.y = y;
      if(x > value.br.x) value.br.x = x;
      if(x < value.tl.x) value.tl.x = x;
   }
   delete vertices;
}

static Array<Pointd> getISEAExtentFromVertices(RhombicIcosahedral3H dggrs, I3HZone zone, int edgeRefinement)   // 0 for 1-20 based on level
{
   Array<Pointd> rVertices = null;
   Pointd vertices[9];
   int numPoints = zone.getBaseRefinedVertices(false, vertices);
   if(numPoints)
   {
      Array<Pointd> ap = null;
      bool refine = zone.subHex < 3;  // Only use refinement for ISEA for even levels -- REVIEW: Why and when do we want to refine?
      int i;

      if(refine)
      {
         Array<Pointd> r = refine5x6(numPoints, vertices, 1, false);
         ap = { size = r.count };
         for(i = 0; i < r.count; i++)
         {
            Vector3D v { };
            isea5x6PJ.toISEAPlanar({ r[i].x, r[i].y }, v);
            ap[i] = { v.x, v.y };
         }
         delete r;
      }
      else
      {
         ap = { size = numPoints };
         for(i = 0; i < numPoints; i++)
         {
            Vector3D v { };
            isea5x6PJ.toISEAPlanar({ vertices[i].x, vertices[i].y }, v);
            ap[i] = { v.x, v.y };
         }
      }
      rVertices = ap;
   }
   return rVertices;
}
