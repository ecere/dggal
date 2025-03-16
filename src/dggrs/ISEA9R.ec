public import IMPORT_STATIC "ecere"
private:

import "isea5x6"
import "dggrs"
import "RI9R"

#include <stdio.h>

public class ISEA9R : RhombicIcosahedral9R
{
   equalArea = true;

   ISEA9R() { pj = ISEAProjection { }; incref pj; }
   ~ISEA9R() { delete pj; }

   // These functions support ISEA5x6 and ISEA as additional CRSs
   DGGRSZone getZoneFromCRSCentroid(int level, CRS crs, const Pointd centroid)
   {
      switch(crs)
      {
         case CRS { ogc, 1534 }:
         {
            if(level <= 16)
            {
               Vector3D c5x6;
               ISEA5x6Projection::fromISEAPlanar({ centroid.x, centroid.y }, c5x6);
               return I9RZone::fromCRSExtent({ c5x6.x, c5x6.y }, { c5x6.x, c5x6.y }, level);
            }
            return nullZone;
         }
         case CRS { ogc, 153456 }: crs = 0; // fallthrough
         default:
            return RhombicIcosahedral9R::getZoneFromCRSCentroid(level, crs, centroid);
      }
   }

   void getZoneCRSCentroid(I9RZone zone, CRS crs, Pointd centroid)
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
            return RhombicIcosahedral9R::getZoneCRSCentroid(zone, crs, centroid);
      }
   }

   int getZoneCRSVertices(I9RZone zone, CRS crs, Pointd * vertices)
   {
      switch(crs)
      {
         case CRS { ogc, 1534 }:
         {
            uint count = 4, i;
            CRSExtent extent = zone.ri5x6Extent;
            Pointd v[4] =
            {
               extent.tl,
               { extent.tl.x, extent.br.y },
               extent.br,
               { extent.br.x, extent.tl.y }
            };

            for(i = 0; i < count; i++)
            {
               Vector3D iv;
               ISEA5x6Projection::toISEAPlanar({ v[i].x, v[i].y }, iv);
               vertices[i] = { iv.x, iv.y };
            }
            return count;
         }
         case CRS { ogc, 153456 }: crs = 0; // fallthrough
         default:
            return RhombicIcosahedral9R::getZoneCRSVertices(zone, crs, vertices);
      }
   }

   // No refinement needed in ISEA CRSs
   Array<Pointd> getZoneRefinedCRSVertices(I9RZone zone, CRS crs, int edgeRefinement)
   {
      switch(crs)
      {
         case CRS { ogc, 1534 }:
         {
            Array<Pointd> vertices { size = 4 };
            getZoneCRSVertices(zone, crs, vertices.array);
            return vertices;
         }
         case CRS { ogc, 153456 }: crs = 0; // fallthrough
         default:
            return RhombicIcosahedral9R::getZoneRefinedCRSVertices(zone, crs, edgeRefinement);
      }
   }

   void getZoneCRSExtent(I9RZone zone, CRS crs, CRSExtent extent)
   {
      switch(crs)
      {
         case CRS { ogc, 1534 }: extent = zone.ri5x6Extent; break;
         case CRS { ogc, 153456 }: crs = 0; // fallthrough
         default:
            return RhombicIcosahedral9R::getZoneCRSExtent(zone, crs, extent);
      }
   }

   Array<Pointd> getSubZoneCRSCentroids(I9RZone parent, CRS crs, int depth)
   {
      switch(crs)
      {
         case CRS { ogc, 1534 }:
         {
            Array<Pointd> centroids = parent.getSubZoneCentroids(depth);
            if(centroids)
            {
               uint count = centroids.count;
               int i;
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
            return RhombicIcosahedral9R::getSubZoneCRSCentroids(parent, crs, depth);
      }
   }
}

/*
static void getISEA9RCRSExtent(I9RZone zone, CRSExtent value)
{
   CRSExtent k = zone.ri5x6Extent;
   Vector3D p[4];

   ISEA5x6Projection::toISEAPlanar({k.tl.x, k.tl.y }, p[0]);
   ISEA5x6Projection::toISEAPlanar({k.tl.x, k.br.y }, p[1]);
   ISEA5x6Projection::toISEAPlanar({k.br.x, k.br.y }, p[2]);
   ISEA5x6Projection::toISEAPlanar({k.br.x, k.tl.y }, p[3]);
   value.crs = { ogc, 1534 };
   value.tl.x = Min(Min(p[0].x, p[1].x), Min(p[2].x, p[3].x));
   value.tl.y = Max(Max(p[0].y, p[1].y), Max(p[2].y, p[3].y));
   value.br.x = Max(Max(p[0].x, p[1].x), Max(p[2].x, p[3].x));
   value.br.y = Min(Min(p[0].y, p[1].y), Min(p[2].y, p[3].y));
}
*/
