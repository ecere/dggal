// This forms a basis for a number of aperture 7 hexagonal grids
// using different projections based on the Rhombic Icosahedral 5x6 space
public import IMPORT_STATIC "ecrt"
private:

import "dggrs"
import "ri5x6"
// import "I7HSubZones"

#include <stdio.h>

// These DGGRSs have the topology of Goldberg polyhedra (https://en.wikipedia.org/wiki/Goldberg_polyhedron)
//    class   I for even levels (m = 7^(level/2), n = 0)
//    class III for odd levels  (m = 2n,          n = 7^((level-1)/2)))

/*
I7H

   T = (m+n)^2 - mn = m^2 + mn + n^2 = 7^level

   (face count)
     10T + 2        T   GP notation    Class  Name           Conway
   -------------------------------------------------------------------------------
   0:     12        1   GP(  1, 0)      1     dodecahedron        D
   1:     72        7   GP(  2, 1)      3                        wD
   2:    492       49   GP(  7, 0)      1                      wrwD
   3:   3432      343   GP( 14, 7)      3                     wrwwD
   4:  24012     2401   GP( 49, 0)      1                   wrwrwwD ?
   5: 168072    16807   GP( 98,49)      3                  wrwrwwwD ?
*/

static define POW_EPSILON = 0.1;

extern uint64 powersOf3[34]; // in RI3H.ec

#define POW3(x) ((x) < sizeof(powersOf3) / sizeof(powersOf3[0]) ? (uint64)powersOf3[x] : (uint64)(pow(3, x) + POW_EPSILON))
#define POW6(x) ((1LL << (x)) * POW3(x))
#define POW7(x) ((x) < sizeof(powersOf7) / sizeof(powersOf7[0]) ? (uint64)powersOf7[x] : (uint64)(pow(7, x) + POW_EPSILON))
#define POW8(x) (1LL << (3*(x)))

public class RhombicIcosahedral7H : DGGRS
{
   RI5x6Projection pj;
   bool equalArea;

   // DGGH
   uint64 countZones(int level)
   {
      return (uint64)(10 * POW7(level) + 2);
   }

   int getMaxDGGRSZoneLevel() { return 19; }
   int getRefinementRatio() { return 7; }
   int getMaxParents() { return 2; }
   int getMaxNeighbors() { return 6; }
   int getMaxChildren() { return 13; }

   uint64 countSubZones(I7HZone zone, int depth)
   {
      return zone.getSubZonesCount(depth);
   }

   int getZoneLevel(I7HZone zone)
   {
      return zone.level;
   }

   int countZoneEdges(I7HZone zone) { return zone.nPoints; }

   bool isZoneCentroidChild(I7HZone zone)
   {
      return zone.isCentroidChild;
   }

   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   double getZoneArea(I7HZone zone)
   {
      double area;
      if(equalArea)
      {
         uint64 zoneCount = countZones(zone.level);
         static double earthArea = 0;
         if(!earthArea) earthArea = wholeWorld.geodeticArea;
         // zoneCount - 12 is the number of hexagons; the 12 pentagons take up the area of 10 hexagons (5/6 * 12)
         area = earthArea / (zoneCount - 2) * (zone.nPoints == 5 ? 5/6.0 : 1);

#if 0
         PrintLn("Divided area: ", area);
         computeGeodesisZoneArea(zone);
#endif
      }
      else
      {
#ifdef USE_GEOGRAPHIC_LIB
         area = computeGeodesisZoneArea(zone);
#else
         // FIXME: Is there a simple way to directly compute the area for other RI7H ?
         area = 0;
#endif
      }
      return area;
   }

   I7HZone getZoneFromCRSCentroid(int level, CRS crs, const Pointd centroid)
   {
      if(level <= 19)
      {
         switch(crs)
         {
            case 0: case CRS { ogc, 153456 }:
               return I7HZone::fromCentroid(level, centroid);
            case CRS { ogc, 1534 }:
            {
               Pointd c5x6;
               RI5x6Projection::fromIcosahedronNet({ centroid.x, centroid.y }, c5x6);
               return I7HZone::fromCentroid(level, { c5x6.x, c5x6.y });
            }
            case CRS { epsg, 4326 }:
            case CRS { ogc, 84 }:
               return (I7HZone)getZoneFromWGS84Centroid(level,
                  crs == { ogc, 84 } ?
                     { centroid.y, centroid.x } :
                     { centroid.x, centroid.y });
         }
      }
      return nullZone;
   }

   int getZoneNeighbors(I7HZone zone, I7HZone * neighbors, I7HNeighbor * nbType)
   {
      return zone.getNeighbors(neighbors, nbType);
   }

   I7HZone getZoneCentroidParent(I7HZone zone)
   {
      // TODO: (low importance)
      // NOTE: In 7H, some parents are centroid children themselves, but not all zones have a
      //       centroid parent
      return nullZone; // REVIEW: zone.centroidParent;
   }

   I7HZone getZoneCentroidChild(I7HZone zone)
   {
      return zone.centroidChild;
   }

   int getZoneParents(I7HZone zone, I7HZone * parents)
   {
      return zone.getParents(parents);
   }

   int getZoneChildren(I7HZone zone, I7HZone * children)
   {
      return zone.getChildren(children);
   }

   // Text ZIRS
   void getZoneTextID(I7HZone zone, String zoneID)
   {
      zone.getZoneID(zoneID);
   }

   I7HZone getZoneFromTextID(const String zoneID)
   {
      return I7HZone::fromZoneID(zoneID);
   }

   // Sub-zone Order
   I7HZone getFirstSubZone(I7HZone zone, int depth)
   {
      return zone.getFirstSubZone(depth);
   }

   void compactZones(Array<DGGRSZone> zones)
   {
      int maxLevel = 0, i, count = zones.count;
      AVLTree<I7HZone> zonesTree { };

      for(i = 0; i < count; i++)
      {
         I7HZone zone = (I7HZone)zones[i];
         if(zone != nullZone)
         {
            int level = zone.level;
            if(level > maxLevel)
               maxLevel = level;
            zonesTree.Add(zone);
         }
      }

      compactI7HZones(zonesTree, maxLevel);
      zones.Free();

      count = zonesTree.count;
      zones.size = count;
      i = 0;
      for(z : zonesTree)
         zones[i++] = z;

      delete zonesTree;
   }

   int getIndexMaxDepth()
   {
      return 19;
   }

   int64 getSubZoneIndex(I7HZone parent, I7HZone subZone)
   {
      int64 ix = -1;
      int level = getZoneLevel(parent), szLevel = getZoneLevel(subZone);

      if(szLevel == level)
         ix = 0;
      else if(szLevel > level && zoneHasSubZone(parent, subZone))
      {
         Pointd zCentroid;

         canonicalize5x6(subZone.centroid, zCentroid);
         ix = -1; // TODO: iterateI7HSubZones(parent, szLevel - level, &zCentroid, findSubZone, -1);
      }
      return ix;
   }

   DGGRSZone getSubZoneAtIndex(I7HZone parent, int relativeDepth, int64 index)
   {
      I7HZone subZone = nullZone;
      if(index >= 0 && index < countSubZones(parent, relativeDepth))
      {
         if(index == 0)
            return getFirstSubZone(parent, relativeDepth);
         else
         {
            // TODO:
            // Pointd centroid;
            //iterateI7HSubZones(parent, relativeDepth, &centroid, findByIndex, index);
            subZone = nullZone; //I7HZone::fromCentroid(parent.level + relativeDepth, centroid);
         }
      }
      return subZone;
   }

   I7HZone getZoneFromWGS84Centroid(int level, const GeoPoint centroid)
   {
      if(level <= 19)
      {
         Pointd v;
         pj.forward(centroid, v);
         return I7HZone::fromCentroid(level, v);
      }
      return nullZone;
   }

   void getZoneCRSCentroid(I7HZone zone, CRS crs, Pointd centroid)
   {
      switch(crs)
      {
         case 0: case CRS { ogc, 153456 }:
            centroid = zone.centroid;
            break;
         case CRS { ogc, 1534 }:
         {
            Pointd c5x6 = zone.centroid;
            RI5x6Projection::toIcosahedronNet({c5x6.x, c5x6.y }, centroid);
            break;
         }
         case CRS { epsg, 4326 }:
         case CRS { ogc, 84 }:
         {
            GeoPoint geo;
            pj.inverse(zone.centroid, geo, false);
            centroid = crs == { ogc, 84 } ?
               { geo.lon, geo.lat } :
               { geo.lat, geo.lon };
            break;
         }
      }
   }

   void getZoneWGS84Centroid(I7HZone zone, GeoPoint centroid)
   {
      pj.inverse(zone.centroid, centroid, zone.subHex > 0);
   }

   void getZoneCRSExtent(I7HZone zone, CRS crs, CRSExtent extent)
   {
      switch(crs)
      {
         case 0: case CRS { ogc, 153456 }:
            extent = zone.ri5x6Extent;
            break;
         case CRS { ogc, 1534 }:
            getIcoNetExtentFromVertices(zone, extent);
            break;
         case CRS { epsg, 4326 }:
         case CRS { ogc, 84 }:
         {
            GeoExtent geo;
            getZoneWGS84Extent(zone, geo);
            extent.crs = crs;
            if(crs == { ogc, 84 })
            {
               extent.tl = { geo.ll.lon, geo.ur.lat };
               extent.br = { geo.ur.lon, geo.ll.lat };
            }
            else
            {
               extent.tl = { geo.ur.lat, geo.ll.lon };
               extent.br = { geo.ll.lat, geo.ur.lon };
            }
            break;
         }
      }
   }

   void getZoneWGS84Extent(I7HZone zone, GeoExtent extent)
   {
      int i;
      GeoPoint centroid;
      Radians minDLon = 99999, maxDLon = -99999;
      Array<GeoPoint> vertices = (Array<GeoPoint>)getRefinedVertices(zone, { epsg, 4326 }, 0, true);
      int nVertices = vertices ? vertices.count : 0;

      getZoneWGS84Centroid(zone, centroid);

      extent.clear();
      for(i = 0; i < nVertices; i++)
      {
         GeoPoint p = vertices[i];
         Radians dLon = p.lon - centroid.lon;

         if(dLon > Pi) dLon -= 2* Pi;
         if(dLon < -Pi) dLon += 2* Pi;

         if(p.lat > extent.ur.lat) extent.ur.lat = p.lat;
         if(p.lat < extent.ll.lat) extent.ll.lat = p.lat;

         if(dLon > maxDLon)
            maxDLon = dLon, extent.ur.lon = p.lon;
         if(dLon < minDLon)
            minDLon = dLon, extent.ll.lon = p.lon;
      }
      if((Radians)extent.ll.lon < -Pi)
         extent.ll.lon += 2*Pi;
      if((Radians)extent.ur.lon > Pi)
         extent.ur.lon -= 2*Pi;

      delete vertices;
   }

   int getZoneCRSVertices(I7HZone zone, CRS crs, Pointd * vertices)
   {
      uint count = zone.getVertices(vertices), i;
      int j;

      for(j = 0; j < count; j++)
         canonicalize5x6(vertices[j], vertices[j]);

      switch(crs)
      {
         case 0: case CRS { ogc, 153456 }:
            break;
         case CRS { ogc, 1534 }:
         {
            for(i = 0; i < count; i++)
               RI5x6Projection::toIcosahedronNet({ vertices[i].x, vertices[i].y }, vertices[i]);
            break;
         }
         case CRS { ogc, 84 }:
         case CRS { epsg, 4326 }:
         {
            bool oddGrid = zone.level & 1; // REVIEW:
            for(i = 0; i < count; i++)
            {
               GeoPoint geo;
               pj.inverse(vertices[i], geo, oddGrid);
               vertices[i] = crs == { ogc, 84 } ? { geo.lon, geo.lat } : { geo.lat, geo.lon };
            }
            break;
         }
         default:
            count = 0;
      }
      return count;
   }

   int getZoneWGS84Vertices(I7HZone zone, GeoPoint * vertices)
   {
      Pointd v5x6[6];
      uint count = zone.getVertices(v5x6), i;
      bool oddGrid = zone.level & 1; // REVIEW:
      int j;

      for(j = 0; j < count; j++)
         canonicalize5x6(v5x6[j], v5x6[j]);

      for(i = 0; i < count; i++)
         pj.inverse(v5x6[i], vertices[i], oddGrid);
      return count;
   }

   Array<Pointd> getZoneRefinedCRSVertices(I7HZone zone, CRS crs, int edgeRefinement)
   {
      if(crs == CRS { ogc, 1534 } || crs == CRS { ogc, 153456 })
         return getIcoNetRefinedVertices(zone, edgeRefinement, crs == CRS { ogc, 1534 });
      else
         return getRefinedVertices(zone, crs, edgeRefinement, false);
   }

   Array<GeoPoint> getZoneRefinedWGS84Vertices(I7HZone zone, int edgeRefinement)
   {
      return (Array<GeoPoint>)getRefinedVertices(zone, { epsg, 4326 }, edgeRefinement, true);
   }

   void getApproxWGS84Extent(I7HZone zone, GeoExtent extent)
   {
      getZoneWGS84Extent(zone, extent);
      /*
      int sh = zone.subHex;
      int i;
      GeoPoint centroid;
      Radians minDLon = 99999, maxDLon = -99999;
      Pointd vertices[7];  // REVIEW: Should this be 6? can't ever be 7?
      int nVertices = zone.getVertices(vertices);
      bool oddGrid = zone.subHex > 2;

      getZoneWGS84Centroid(zone, centroid);

      extent.clear();
      for(i = 0; i < nVertices; i++)
      {
         Pointd * cv = &vertices[i];
         GeoPoint p;
         if(pj.inverse(cv, p, oddGrid))
         {
            Radians dLon = p.lon - centroid.lon;

            if(dLon > Pi) dLon -= 2* Pi;
            if(dLon < -Pi) dLon += 2* Pi;

            if(p.lat > extent.ur.lat) extent.ur.lat = p.lat;
            if(p.lat < extent.ll.lat) extent.ll.lat = p.lat;

            if(dLon > maxDLon)
               maxDLon = dLon, extent.ur.lon = p.lon;
            if(dLon < minDLon)
               minDLon = dLon, extent.ll.lon = p.lon;
         }
      }

      if(sh == 1 || sh == 6)
      {
         // "North" pole
         extent.ll.lon = -Pi;
         extent.ur.lon = Pi;
         extent.ur.lat = Pi/2;
      }
      else if(sh == 2 || sh == 7)
      {
         // "South" pole
         extent.ll.lon = -Pi;
         extent.ur.lon = Pi;
         extent.ll.lat = -Pi/2;
      }
      */
   }

   // NOTE: getRefinedVertices() is currently only ever called with CRS84 or EPSG:4326
   private static Array<Pointd> getRefinedVertices(I7HZone zone, CRS crs, int edgeRefinement, bool useGeoPoint) // 0 edgeRefinement for 1-20 based on level
   {
      Array<Pointd> rVertices = null;
      bool crs84 = crs == CRS { ogc, 84 } || crs == CRS { epsg, 4326 };
      int level = zone.level;
      // * 1024 results in level 2 zones areas accurate to 0.01 km^2
      int nDivisions = edgeRefinement ? edgeRefinement :
         level < 3 ? 20 : level < 5 ? 15 : level < 8 ? 10 : level < 10 ? 8 : level < 11 ? 5 : level < 12 ? 2 : 1;
      Array<Pointd> r = zone.getBaseRefinedVertices(crs84, nDivisions);
      if(r)
      {
         int i;
         if(crs84)
         {
            GeoPoint centroid;
            bool wrap = true;
            bool oddGrid = true; //(level & 1); // REVIEW: ALways setting this to true fixes flipped South pole in BA-0-C
            Array<Pointd> ap = useGeoPoint ? (Array<Pointd>)Array<GeoPoint> { } : Array<Pointd> { };

            ap./*size*/minAllocSize = r.count;

            getZoneWGS84Centroid(zone, centroid);
            // REVIEW: Should centroid ever be outside -Pi..Pi?
            if(centroid.lon < - Pi - 1E-9)
               centroid.lon += 2*Pi;

            if(centroid.lon > Pi + 1E-9)
               centroid.lon -= 2*Pi;

            for(i = 0; i < r.count; i++)
            {
               GeoPoint point;
               // Imprecisions causes some failures... http://localhost:8080/ogcapi/collections/gebco/dggs/ISEA3H/zones/L0-2B3FA-G

               if(r[i].x > 5 + 1E-6 && r[i].y > 5 + 1E-6)
                  r[i].x -= 5, r[i].y -= 5;

               if(r[i].y < 0 && fabs(r[i].y - 0) < 1E-6)
                  r[i].y = 0;
               else if(r[i].x > 5 && fabs(r[i].x - 5) < 1E-6)
                  r[i].x = 5;
               if(r[i].x > 5 + 1E-6 || r[i].y > 6 + 1E-6)
                  r[i].x -= 5, r[i].y -= 5;
               if(r[i].x < 0 - 1E-6)
                  r[i].x += 5, r[i].y += 5;

               if(pj.inverse(r[i], point, oddGrid))
               {
                  if(wrap)
                  {
                     point.lon = wrapLonAt(-1, point.lon, centroid.lon - Degrees { 0.05 }) + centroid.lon - Degrees { 0.05 }; // REVIEW: wrapLonAt() doesn't add back centroid.lon ?

                     // REVIEW: Why isn't wrapLonAt() handling these cases?
                     if(oddGrid)
                     {
                        if(((double)point.lon - (double)centroid.lon) < -120)
                           point.lon += 180;
                        else if(((double)point.lon - (double)centroid.lon) > 120)
                           point.lon -= 180;
                     }
                  }

                  ap.Add(useGeoPoint ? { (Radians) point.lat, (Radians) point.lon } :
                     crs == { ogc, 84 } ? { point.lon, point.lat } : { point.lat, point.lon });
                  if(ap.count >= 2 &&
                     fabs(ap[ap.count-1].x - ap[ap.count-2].x) < 1E-11 &&
                     fabs(ap[ap.count-1].y - ap[ap.count-2].y) < 1E-11)
                     ap.size--; // We rely on both interruptions during interpolation, but they map to the same CRS84 point
               }
#ifdef _DEBUG
               else
                  ; //PrintLn("WARNING: Failed to inverse project ", r[i]);
#endif
            }
            delete r;
            ap.minAllocSize = 0;
            rVertices = ap;
         }
         else
         {
            rVertices.minAllocSize = 0;
            rVertices = r;
         }
      }
      return rVertices;
   }

   // Sub-zone Order
   Array<Pointd> getSubZoneCRSCentroids(I7HZone parent, CRS crs, int depth)
   {
      Array<Pointd> centroids = parent.getSubZoneCentroids(depth);
      if(centroids)
      {
         uint count = centroids.count, i;
         switch(crs)
         {
            case 0: case CRS { ogc, 153456 }: break;
            case CRS { ogc, 1534 }:
               for(i = 0; i < count; i++)
                  RI5x6Projection::toIcosahedronNet({ centroids[i].x, centroids[i].y }, centroids[i]);
               break;
            case CRS { epsg, 4326 }:
            case CRS { ogc, 84 }:
            {
               bool oddGrid = (parent.level + depth) & 1; // REVIEW:
               for(i = 0; i < count; i++)
               {
                  GeoPoint geo;
                  pj.inverse(centroids[i], geo, oddGrid);
                  centroids[i] = crs == { ogc, 84 } ? { geo.lon, geo.lat } : { geo.lat, geo.lon };
               }
               break;
            }
            default: delete centroids;
         }
      }
      return centroids;
   }

   Array<GeoPoint> getSubZoneWGS84Centroids(I7HZone parent, int depth)
   {
      Array<GeoPoint> geo = null;
      Array<Pointd> centroids = parent.getSubZoneCentroids(depth);
      if(centroids)
      {
         uint count = centroids.count;
         int i;
         bool oddGrid = (parent.level + depth) & 1; // REVIEW:

         geo = { size = count };
         for(i = 0; i < count; i++)
            pj.inverse(centroids[i], geo[i], oddGrid);
         delete centroids;
      }
      return geo;
   }

   static Array<DGGRSZone> listZones(int zoneLevel, const GeoExtent bbox)
   {
      Array<DGGRSZone> zones = null;
      AVLTree<I7HZone> tsZones { };
      int level = 0;
      int root;
      bool extentCheck = bbox != null && bbox.OnCompare(wholeWorld);

      for(root = 0; root < 12; root++)
         tsZones.Add({ 0, root, 0 });

      //tsZones.Add(I7HZone::fromZoneID("B3-0-G"));
      //tsZones.Add(I7HZone::fromZoneID("B3-0-B"));
      //tsZones.Add(I7HZone::fromZoneID("B4-0-B"));
      //tsZones.Add(I7HZone::fromZoneID("B4-0-C"));
      //tsZones.Add(I7HZone::fromZoneID("B6-0-C"));
      //tsZones.Add(I7HZone::fromZoneID("BA-0-C"));
      //tsZones.Add(I7HZone::fromZoneID("AA-0-A"));
      //tsZones.Add(I7HZone::fromZoneID("BA-0-D"));
      //tsZones.Add(I7HZone::fromZoneID("BA-0-E"));
      //tsZones.Add(I7HZone::fromZoneID("BA-0-F"));
      //tsZones.Add(I7HZone::fromZoneID("BB-0-E"));

      if(zoneLevel == 0 && extentCheck)
      {
         AVLTree<I7HZone> tmp { };

         for(z : tsZones)
         {
            I7HZone zone = (I7HZone)z;
            GeoExtent e;
            getZoneWGS84Extent(zone, e);
            if(e.intersects(bbox))
               tmp.Add(zone);
         }
         delete tsZones;
         tsZones = tmp;
      }

      for(level = 1; level <= zoneLevel; level++)
      {
         AVLTree<I7HZone> tmp { };

         for(z : tsZones)
         {
            I7HZone zone = (I7HZone)z;
            I7HZone children[13];
            // int n = zone.getPrimaryChildren(children), i;
            int n = zone.getChildren(children), i;

            for(i = 0; i < n; i++)
            {
               I7HZone c = children[i];
               if(extentCheck)
               {
                  GeoExtent e;
                  if(!tmp.Find(c))
                  {
                     getZoneWGS84Extent(c, e);
                     if(!e.intersects(bbox))
                        continue;
                  }
                  else
                     continue;
               }
               tmp.Add(c);
            }
         }
         delete tsZones;
         tsZones = tmp;
      }

      #if 0
      int i9RLevel = zoneLevel / 2;
      uint64 power = POW7(i9RLevel);
      double z = 1.0 / power;
      int hexSubLevel = zoneLevel & 1;
      Pointd tl, br;
      //double x, y;
      int64 yCount, xCount, yi, xi;

      if(bbox != null && bbox.OnCompare(wholeWorld))
      {
         // Avoid the possibility of including extra zones for single point boxes
         if(fabs((Radians)bbox.ur.lat - (Radians)bbox.ll.lat) < 1E-11 &&
            fabs((Radians)bbox.ur.lon - (Radians)bbox.ll.lon) < 1E-11)
         {
            DGGRSZone zone = getZoneFromWGS84Centroid(zoneLevel, bbox.ll);
            if(zone != nullZone)
               zones = { [ zone ] };
            return zones;
         }

         // fputs("WARNING: accurate bounding box not yet supported\n", stderr);
         pj.extent5x6FromWGS84(bbox, tl, br);
      }
      else
         extentCheck = false, pj.extent5x6FromWGS84(wholeWorld, tl, br);

      yCount = (int64)((br.y - tl.y + 1E-11) / z) + 2;
      xCount = (int64)((br.x - tl.x + 1E-11) / z) + 2;

      // These loops adding z were problematic at high level losing precision with the z additions
      //for(y = tl.y; y < br.y + 2*z; y += z)
      for(yi = 0; yi < yCount; yi++)
      {
         double y = tl.y + yi * z;
         int rootY = (int)(y + 1E-11);
         int row = (int)(y / z + 1E-11);
         //for(x = tl.x; x < br.x + 2*z; x += z)
         for(xi = 0; xi < xCount; xi++)
         {
            double x = tl.x + xi * z;
            int rootX = (int)(x + 1E-11);
            int col = (int)(x / z + 1E-11);
            int d = rootY - rootX;
            if(rootX < 5 && (d == 0 || d == 1))
            {
               int nHexes = 0, h;
               I7HZone hexes[4];

               hexes[nHexes++] = I7HZone::fromI9R(i9RLevel, row, col, hexSubLevel ? 'D' : 'A');
               if(hexes[nHexes-1] == nullZone)
                  continue; // This should no longer happen...

               if(hexSubLevel)
               {
                  hexes[nHexes++] = I7HZone::fromI9R(i9RLevel, row, col, 'E');
                  hexes[nHexes++] = I7HZone::fromI9R(i9RLevel, row, col, 'F');
               }

               for(h = 0; h < nHexes; h++)
                  tsZones.Add(hexes[h]);
            }
         }
      }
      #endif

      if(tsZones.count)
      {
         zones = { minAllocSize = tsZones.count };
         for(t : tsZones)
         {
            I7HZone zone = t;
#if 0 // Currently already checked earlier...
            if(extentCheck)
            {
               // REVIEW: Computing the detailed wgs84Extent is slow as it uses refined vertices and involves a large amount of inverse projections.
               //         Are we missing large numbers of hexagons first eliminating those outside the approximate extent?
               GeoExtent e;

               // REVIEW: Should we check 5x6 extent as well or instead of this approximate extent?
               /* TODO:
               getApproxWGS84Extent(zone, e);
               if(!e.intersects(bbox))
                  continue;
               */

               getZoneWGS84Extent(zone, e);
               if(!e.intersects(bbox))
                  continue;
            }
#endif
            zones[zones.count++] = zone;
         }
         zones.Sort(true);
      }

      delete tsZones;
      return zones;
   }
}

/*static*/ uint64 powersOf7[] = { 1, 7, 49, 343, 2401, 16807, 117649, 823543, 5764801, 40353607, 282475249, 1977326743,
   13841287201LL, 96889010407LL, 678223072849LL, 4747561509943LL, 33232930569601LL,
   232630513987207LL, 1628413597910449LL, 11398895185373143LL, 79792266297612001LL,
   558545864083284007LL, 3909821048582988049LL, 8922003266371364727LL, 7113790643470898241LL
};

enum I7HNeighbor
{
   /*
   // The names reflect the planar ISEA projection arrangement
   top,        // Odd level only, except when replacing topLeft/topRight in interruptions for even level
   bottom,     // Odd level only, except when replacing bottomLeft/bottomRight in interruptions for even level
   left,       // Even level only, except when bottomLeft/topLeft is used instead of bottom/top for even level
   right,      // Even level only
   topLeft,
   topRight,
   bottomLeft,
   bottomRight*/
   first,
   second,
   third,
   fourth,
   fifth,
   sixth
};

// Public for use in tests...
public class I7HZone : private DGGRSZone
{
public:
   uint levelI49R:4:58;   //  4 bits  0..9: (Text ID use A-T 7H level)
                          // For each root rhombus: (level 0: 1x1, level 1: 7x7, level 2: 49x49..., level 9: 40,353,607 x 40,353,607 = 1,628,413,597,910,449 zones)
   uint rootRhombus:4:54; // 0 .. 9; 10 and 11 for North and South poles
   uint64 rhombusIX:51:3; // 51 bits  0..1,628,413,597,910,448
   uint subHex:3:0;       //  3 bits  0: A     -- even level
                          //             B     -- odd level centroid child, C..H: vertez child

private:
   property int level
   {
      get { return 2*levelI49R + (subHex > 0); }
   }

   property int nPoints
   {
      get
      {
         if(subHex > 1) // All vertex children are hexagons
            return 6;
         else
            // North and South Poles, Top-left corner of root rhombuses are pentagons
            return rhombusIX == 0 ? 5 : 6;
      }
   }

   property bool isEdgeHex
   {
      get
      {
         /* REVIEW:
         if(nPoints == 6)
         {
            return false;
         }
         */
         return false;
      }
   }

   I7HZone ::fromZoneID(const String zoneID)
   {
      I7HZone result = nullZone;
      char levelChar, subHex;
      uint64 ix = 0;
      uint root;

      if(sscanf(zoneID, __runtimePlatform == win32 ? "%c%X-%I64X-%c" : "%c%X-%llX-%c",
         &levelChar, &root, &ix, &subHex) == 4 && root < 12 && levelChar >= 'A' && levelChar <= 'V' && subHex >= 'A' && subHex <= 'H')
      {
         int l49r = (levelChar - 'A') / 2;
         uint64 p = POW7(l49r), rSize = p * p;
         result = { l49r, root, ix, subHex - 'A' };
         if((result.subHex > 0) != ((levelChar - 'A') & 1) || ix >= rSize || root > 11)
            result = nullZone;
      }
      return result;
   }

   bool containsPoint(const Pointd v)
   {
      bool result = false;
      int i;
      Array<Pointd> v5x6 = getBaseRefinedVertices(false, 1);
      int n = v5x6 ? v5x6.count : 0;
      CRSExtent bbox { };
      CRSExtent pBBOX { };

      pBBOX.tl.x = (int) (v.x + 1E-11);
      pBBOX.tl.y = (int) (v.y + 1E-11);
      pBBOX.br.x = pBBOX.tl.x + 1;
      pBBOX.br.y = pBBOX.tl.y + 1;

      if(!n)
      {
         delete v5x6;
         return false;
      }

      bbox.br = { -100, -100 };
      bbox.tl = {  100,  100 };
      for(i = 0; i < n; i++)
      {
         double x = v5x6[i].x, y = v5x6[i].y;

         if(x > bbox.br.x) bbox.br.x = x;
         if(y > bbox.br.y) bbox.br.y = y;
         if(x < bbox.tl.x) bbox.tl.x = x;
         if(y < bbox.tl.y) bbox.tl.y = y;
      }

      if(v.x - bbox.br.x > 3 && v.y - bbox.br.y > 3 &&
         v.x - bbox.tl.x > 3 && v.y - bbox.tl.y > 3)
      {
         bbox.tl.x += 5;
         bbox.tl.y += 5;
         bbox.br.x += 5;
         bbox.br.y += 5;
      }

      if(v.x - bbox.br.x <-3 && v.y - bbox.br.y <-3 &&
         v.x - bbox.tl.x <-3 && v.y - bbox.tl.y <-3)
      {
         bbox.tl.x -= 5;
         bbox.tl.y -= 5;
         bbox.br.x -= 5;
         bbox.br.y -= 5;
      }

#if 0 //def _DEBUG
      PrintLn("Zone  BBOX: ", bbox.tl.x, ", ", bbox.tl.y, " - ", bbox.br.x, ", ", bbox.br.y);
      PrintLn("Point     : ", v.x, ", ", v.y);
#endif

      if(v.x < bbox.tl.x - 1E-11 ||
         v.y < bbox.tl.y - 1E-11 ||
         v.x > bbox.br.x + 1E-11 ||
         v.y > bbox.br.y + 1E-11)
      {
#if 0 //def _DEBUG
         PrintLn("  Skipping this zone");
#endif
         delete v5x6;
         return false;
      }

#if 0 // def _DEBUG
      PrintLn("  Considering this zone");
      PrintLn("Point BBOX: ", pBBOX.tl.x, ", ", pBBOX.tl.y, " - ", pBBOX.br.x, ", ", pBBOX.br.y);
#endif

      for(i = 0; i < n; i++)
      {
         int j = i < n-1 ? i + 1 : 0;
         Pointd a = v5x6[i], b = v5x6[j];
         double sa;
         Pointd aa = a, bb = b;

         if(fabs(aa.x - v.x) > 3 &&
            fabs(aa.y - v.y) > 3)
         {
            if(aa.x > 3 && aa.y > 3)
               aa.x -= 5, aa.y -= 5;
            else
               aa.x += 5, aa.y += 5;
         }
         if(fabs(bb.x - v.x) > 3 &&
            fabs(bb.y - v.y) > 3)
         {
            if(bb.x > 3 && bb.y > 3)
               bb.x -= 5, bb.y -= 5;
            else
               bb.x += 5, bb.y += 5;
         }
#if 0 //def _DEBUG
         PrintLn("  Segment: ", aa.x, ", ", aa.y, " - ", bb.x, ", ", bb.y);
#endif

         if((aa.x - 1E-11 < pBBOX.tl.x ||
             aa.y - 1E-11 < pBBOX.tl.y ||
             aa.x + 1E-11 > pBBOX.br.x ||
             aa.y + 1E-11 > pBBOX.br.y) &&
            (bb.x - 1E-11 < pBBOX.tl.x ||
             bb.y - 1E-11 < pBBOX.tl.y ||
             bb.x + 1E-11 > pBBOX.br.x ||
             bb.y + 1E-11 > pBBOX.br.y))
         {
#if 0 //def _DEBUG
            PrintLn("  Skipping this segment (B point)");
#endif
            continue;
         }

         sa = pointLineSide(v.x, v.y, aa, bb);

         if(sa < 0)
         {
#if 0 // def _DEBUG
            PrintLn("  We're outside this segment!");
#endif
            result = false;
            break;
         }
         else
            result = true; // At least one edge segment should be checked
                           // (BBOX check only gives false positive on e.g., B0-0-C)
      }
#if 0 //def _DEBUG
      if(result)
         PrintLn("  Zone Contains point!");
#endif
      delete v5x6;
      return result;
   }

   // This function generates the proposed I7H DGGRS Zone ID string
   // in the form {LevelChar}{RootPentagon}-{HierarchicalIndexFromPentagon}
   void getZoneID(String zoneID)
   {
      if(this == nullZone)
         sprintf(zoneID, "(null)");
      else
      {
         uint level = 2 * levelI49R + (subHex > 0);
         uint root = rootRhombus;
         uint64 ix = rhombusIX;
         sprintf(zoneID,
            __runtimePlatform == win32 ? "%c%X-%I64X-%c" : "%c%X-%llX-%c",
            'A' + level, root, ix, 'A' + subHex);
      }
   }

   property I7HZone parent0
   {
      // ivea7h info C2-8-A
      // ivea7h zone 28.6888849753227,-69.0934671650866 1
      get
      {
         I7HZone key = nullZone;
         int l9r = levelI49R;
         if(l9r || subHex)
         {
            if(subHex)
               key = { l9r, rootRhombus, rhombusIX, 0 };
            else
               key = I7HZone::fromCentroid((l9r * 2) - 1, centroid);
         }
         return key;
      }
   }

   int getNeighbors(I7HZone neighbors[6], I7HNeighbor i7hNB[6])
   {
      I7HZone children[7];
      int numNeighbors = 0;
      int nc = getPrimaryChildren(children);

      if(nc)
      {
         Pointd c = children[0].centroid;
         Pointd cVerts[6];
         int nv = children[0].getVertices(cVerts);
         int i;
         int nLevel = this.level;
         int cx = Min(4, (int)(c.x + 1E-11)), cy = Min(5, (int)(c.y + 1E-11));  // Coordinate of root rhombus
         bool north = c.x - c.y - 1E-11 > 0;

         for(i = 0; i < nv; i++)
         {
            Pointd v;
            Pointd cc = c;
            double dx = cVerts[i].x - cc.x;
            double dy = cVerts[i].y - cc.y;

            if(dx > 3 && dy > 3)
            {
               dx = cVerts[i].x - 5 - cc.x;
               dy = cVerts[i].y - 5 - cc.y;
            }
            else if(dx < -3 && dy < -3)
            {
               dx = cVerts[i].x + 5 - cc.x;
               dy = cVerts[i].y + 5 - cc.y;
            }

            if(fabs(dx) < 1 && fabs(dy) < 1)
            {
               // We need to avoid computing dx and dy across interuptions
               if(( north && fabs(c.y - cy) < 1E-11) ||
                  (!north && fabs(c.x - cx) < 1E-11))
               {
                  double x, y;
                  Pointd ci;
                  cross5x6Interruption(c, ci, !north, true);

                  x = cVerts[i].x - ci.x;
                  y = cVerts[i].y - ci.y;

                  if(x > 3 && dy > 3)
                  {
                     x = cVerts[i].x - 5 - ci.x;
                     y = cVerts[i].y - 5 - ci.y;
                  }
                  else if(x < -3 && y < -3)
                  {
                     x = cVerts[i].x + 5 - ci.x;
                     y = cVerts[i].y + 5 - ci.y;
                  }

                  if(fabs(x) < fabs(dx) && fabs(y) < fabs(dy))
                  {
                     cc = ci;
                     dx = x;
                     dy = y;
                  }
               }
            }

            move5x6Vertex2(v, cc, dx * 3, dy * 3, false);

            canonicalize5x6(v, v);
            if(i7hNB) i7hNB[numNeighbors] = (I7HNeighbor)i;
            neighbors[numNeighbors++] = fromCentroid(nLevel, v);
#if 0 //def _DEBUG
            if(neighbors[numNeighbors-1] == nullZone)
               fromCentroid(nLevel, v);
#endif
         }
      }
      return numNeighbors;
   }

   int getContainingGrandParents(I7HZone cgParents[2])
   {
      int n = 0;
      int level = this.level;
      if(level >= 2)
      {
         if(isCentroidChild)
            // Scenario A: centroid child
            cgParents[0] = parent0.parent0, n = 1;
         else
         {
            I7HZone parents[2], gParentA, gParentB;

            getParents(parents);
            gParentA = parents[0].parent0;
            gParentB = parents[1].parent0;

            // Scenario B: both parents have same primary parent -- containing grandparent is that primary grandparent
            if(gParentA == gParentB)
               cgParents[0] = gParentA, n = 1;
            else
            {
               // REVIEW:
               // Parents have different primary parents
               Pointd c = centroid, cA = gParentA.centroid, cB = gParentB.centroid;
               I7HZone z1 = I7HZone::fromCentroid(level - 2, { c.x + 1E-9 * Sgn(cA.x - c.x), c.x + 1E-9 * Sgn(cA.y - c.y) });
               I7HZone z2 = I7HZone::fromCentroid(level - 2, { c.x + 1E-9 * Sgn(cB.x - c.x), c.x + 1E-9 * Sgn(cB.y - c.y) });

               // Scenario C: whole zone inside one of these (this sub-zone of only one grandparent): one containing grandparent
               if(z1 == z2)
                  cgParents[0] = z1, n = 1;

               // Scenario D: zone on the edge of both of these (this sub-zone of both): two containing grandparents
               else
                  cgParents[0] = gParentA, cgParents[1] = gParentB, n = 2;
            }
         }
      }
      return n;
   }

   int getParents(I7HZone parents[2])
   {
      I7HZone parent0 = this.parent0;

      parents[0] = parent0;
      if(parent0 == nullZone)
         return 0;
      else
      {
         I7HZone cChild = parent0.centroidChild;
         if(cChild == this)
            return 1;
         else
         {
            // REVIEW: We should use the current zone's centroid -- not parent0's centroidChild's?
            //         This was breaking for calculating C8-14-A 's second parent
            Pointd cc = /*cChild.*/centroid;
            int i;
            Pointd vertices[6];

            int n = getVertices(vertices);
            int pLevel = parent0.level;

            for(i = 0; i < n; i++)
            {
               Pointd acc = cc;
               double dx = vertices[i].x - acc.x;
               double dy = vertices[i].y - acc.y;
               I7HZone z;
               Pointd v;

               if(dx > 3 || dy > 3)
               {
                  dx = vertices[i].x - 5 - acc.x;
                  dy = vertices[i].y - 5 - acc.y;
               }
               else if(dx < -3 || dy < -3)
               {
                  dx = vertices[i].x + 5 - acc.x;
                  dy = vertices[i].y + 5 - acc.y;
               }

               if(fabs(dx) < 1 && fabs(dy) < 1)
               {
                  bool north = acc.x - acc.y - 1E-11 > 0;
                  int cy = (int)(acc.y + 1E-11);
                  int cx = (int)(acc.x + 1E-11);

                  // We need to avoid computing dx and dy across interuptions
                  if(( north && fabs(acc.y - cy) < 1E-11) ||
                     (!north && fabs(acc.x - cx) < 1E-11))
                  {
                     double x, y;
                     Pointd ci;
                     cross5x6Interruption(cc, ci, !north, true);

                     x = vertices[i].x - ci.x;
                     y = vertices[i].y - ci.y;

                     if(x > 3 && dy > 3)
                     {
                        x = vertices[i].x - 5 - ci.x;
                        y = vertices[i].y - 5 - ci.y;
                     }
                     else if(x < -3 && y < -3)
                     {
                        x = vertices[i].x + 5 - ci.x;
                        y = vertices[i].y + 5 - ci.y;
                     }

                     if(fabs(x) < fabs(dx) && fabs(y) < fabs(dy))
                     {
                        acc = ci;
                        dx = x;
                        dy = y;
                     }
                  }
               }

               move5x6Vertex2(v, acc, .99 * dx, .99 * dy, false);

               z = fromCentroid(pLevel, v);
               if(z != nullZone && z != parent0)
               {
                  parents[1] = z;
                  return 2;
               }
            }
#ifdef _DEBUG
            {
               char zID[128];
               getZoneID(zID);
               // PrintLn((uint64)this);
               PrintLn("ERROR: Failed to determine second parent for ", zID);
            }
#endif
            return 1;
         }
      }
   }

   double ::pointLineSide(double x, double y, Pointd a, Pointd b)
   {
      double dx = b.x - a.x, dy = b.y - a.y;
      double A = dy, B = -dx, C = a.y * dx - dy * a.x;
      return A * x + B * y + C;
   }

   I7HZone ::calcCandidateParent(int l9r, int root, int64 row, int64 col, int addCol, int addRow)
   {
      uint64 p = POW7(l9r);
      bool south = (root & 1);
      uint64 cix;

      col += addCol;
      row += addRow;

      // REVIEW: REVIEW / Share this logic with getPrimaryChildren(), possibly centroidChild?
      if(col == (int64)p && row < (int64)p && !south) // Cross at top-dent to the right
      {
         col = p-row;
         row = 0;
         root += 2;
      }
      else if(row == (int64)p && col < (int64)p && south) // Cross at bottom-dent to the right
      {
         row = p-col;
         col = 0;
         root += 2;
      }
      else
      {
         if(row < 0 && col < 0)
            row += p, col += p, root -= 2;
         else if(row < 0)
            row += p, root -= 1;
         else if(col < 0)
            col += p, root -= 1;
         else if(col >= (int64)p && row >= (int64)p)
            row -= p, col -= p, root += 2;
         else if(row >= (int64)p)
            row -= p, root += 1;
         else if(col >= (int64)p)
            col -= p, root += 1;
      }
      if(root < 0) root += 10;
      else if(root > 9) root -= 10;

      south = (root & 1);

      if(!south && row == 0 && col == p)
         root = 0xA, cix = 0;
      else if(south && row == p && col == 0)
         root = 0xB, cix = 0;
      else
      {
         if(row < 0 || row >= p ||
            col < 0 || col >= p)
         {
#ifdef _DEBUG
            // PrintLn("WARNING: Invalid zone calculated");
#endif
            return nullZone;
         }
         else
            cix = row * p + col;
      }

      // REVIEW: Polar zones considerations?
      return I7HZone { l9r, root, cix, 0 };
   }

   I7HZone ::fromCentroid(uint level, const Pointd centroid) // in RI5x6
   {
      int l9r = level / 2;
      Pointd c = centroid;
      uint64 p = POW7(l9r);
      double oop =  1.0 / p;

      // bool isNorthPole = false, isSouthPole = false;
      if(fabs(c.x - c.y - 1) < 1E-10)
         ;//isNorthPole = true;
      else if(fabs(c.y - c.x - 2) < 1E-10)
         ;//isSouthPole = true;
      else if(c.y < -1E-11 && c.x > -1E-11)
         c.x -= c.y, c.y = 0;
      else if((int)floor(c.x + 1E-11) > (int)floor(c.y + 1E-11))
      {
         // Over top dent to the right
         int cy = Min(5, (int)floor(c.y + 1E-11));
         c.x += (cy+1 - c.y), c.y = cy+1;
      }
      else if((int)floor(c.y + 1E-11) - (int)floor(c.x + 1E-11) > 1)
      {
         // Over bottom dent to the right -- REVIEW: This may no longer be necessary?
         int cx = Min(4, (int)floor(c.x + 1E-11));
         c.y += (cx+1 - c.x), c.x = cx+1;
      }
      else if(c.x < -1E-11 || c.y < -1E-11)
         move5x6Vertex2(c, { 5, 5 }, c.x, c.y, false);

      if(c.x > 5 - 1E-11 && c.y > 5 - 1E-11 &&  // This handles bottom right wrap e.g., A9-0E and A9-0-F
         c.x + c.y > 5.0 + 5.0 - oop - 1E-11)
         c.x -= 5, c.y -= 5;

      // Vancouver:     49.2827,-123.1207 2  -- C0-17-A
      // Abbotsford:    49.0581,-122.2798 2  -- C0-1F-A
      // Ottawa:        45.4963,-75.7016 2   -- C0-29-A
      // Auckland:      -36.8543,174.7392 2  -- C9-1C-A
      // Whakatāne:     -37.9583,176.98428 2 -- C9-23-A

      {
         int cx = Min(4, (int)(c.x + 1E-11)), cy = Min(5, (int)(c.y + 1E-11));  // Coordinate of root rhombus
         uint root = cx + cy;
         double x = c.x - cx, y = c.y - cy;
         int64 col = Max(0, (int64)(x * p + 0.5));
         int64 row = Max(0, (int64)(y * p + 0.5));
         double dx = x * p + 0.5 - col;
         double dy = y * p + 0.5 - row;
         uint64 cix;
         bool southRhombus = (root & 1);
         // Review where this should be used...
         bool south = c.y - c.x - 1E-11 > 1; // Not counting pentagons as south or north
         bool north = c.x - c.y - 1E-11 > 0;
         bool northPole = north && fabs(c.x - c.y - 1.0) < 1E-11;
         bool southPole = south && fabs(c.y - c.x - 2.0) < 1E-11;

         if(level & 1)
         {
            // Odd level -- currently using a rather brute-force approach
            I7HZone zone = nullZone;
            if(northPole)
               zone = { l9r, 0xA, 0, 1 };
            else if(southPole)
               zone = { l9r, 0xB, 0, 1 };
            else
            {
               I7HZone candidateParents[7];
               int i;

               if(north && row == 0 && col == p)
                  candidateParents[0] = { l9r, 0xA, 0, 0 };
               else if(south && row == p && col == 0)
                  candidateParents[0] = { l9r, 0xB, 0, 0 };
               else
               {
                  // candidateParents[0] = { l9r, 2 + root * (p * p) + row * p + col, 0 };

                  candidateParents[0] = calcCandidateParent(l9r, root, row, col, 0, 0);
               }

#if 0 //def _DEBUG
               {
                  char pID[128];
                  candidateParents[0].getZoneID(pID);

                  // PrintLn("Main candidate parent: ", pID);
               }
#endif

               // Top (2 potential children including 1 secondary child of prime candidate)
               candidateParents[1] = calcCandidateParent(l9r, root, row, col, 0, -1);
               // Bottom (2 potential children including 1 secondary child of prime candidate)
               candidateParents[2] = calcCandidateParent(l9r, root, row, col, 0, 1);
               // Right (2 potential children including 1 secondary child of prime candidate)
               candidateParents[3] = calcCandidateParent(l9r, root, row, col, 1, 0);
               // Left (2 potential children including 1 secondary child of prime candidate)
               candidateParents[4] = calcCandidateParent(l9r, root, row, col, -1, 0);
               // Top-Left (1 potential child including 1 secondary child of prime candidate)
               candidateParents[5] = calcCandidateParent(l9r, root, row, col, -1, -1);
               // Bottom-Right (1 potential child including 1 secondary child of prime candidate)
               candidateParents[6] = calcCandidateParent(l9r, root, row, col, 1, 1);

               // int numMatches = 0;
               for(i = 0; i < 7; i++)
               {
                  I7HZone children[7];
                  int n, j;
#if 0 //def _DEBUG
                  char pID[128];
                  candidateParents[i].getZoneID(pID);
                  if(candidateParents[i] != fromZoneID(pID))
                  {
                     PrintLn("ERROR: Invalid candidate parent zone: ", pID);
                     candidateParents[1] = calcCandidateParent(l9r, root, row, col, 0, -1);
                  }
                  // PrintLn("Generating primary children for ", pID);
#endif

                  n = candidateParents[i].getPrimaryChildren(children);

                  for(j = 0; j < n; j++)
                  {
#if 0 //def _DEBUG
                     char zID[128];
                     children[j].getZoneID(zID);
                     if(children[j] != fromZoneID(zID))
                     {
                        PrintLn("ERROR: Invalid zone generated: ", zID);

                        children[j].getZoneID(zID);
                        fromZoneID(zID);

                        candidateParents[i].getPrimaryChildren(children);
                     }

                     // PrintLn("Testing whether child ", zID, " contains point ", centroid.x, ", ", centroid.y);
#endif
                     if(children[j].containsPoint(c))
                     {
                        zone = children[j];
                        // numMatches ++;
                        break;
                     }
                  }
                  if(zone != nullZone)
                     break;
               }
               // PrintLn("matches: ", numMatches);
            }

#if 0 //def _DEBUG
            if(zone == nullZone)
               PrintLn("WARNING: Unable to resolve zone for ", centroid.x, ", ", centroid.y);

            if(zone != nullZone)
            {
               char id[256];
               I7HZone z;
               zone.getZoneID(id);
               z = fromZoneID(id);
               if(z != zone)
                  PrintLn("ERROR: Invalid zone returned");
            }
#endif
            return zone;
         }
         else
         {
            if(northPole)
               return { l9r, 0xA, 0, 0 };
            else if(southPole)
               return { l9r, 0xB, 0, 0 };

            // Even level
            if(dx > 1 - dy)
            {
               // Bottom-Right diagonal
               if(dx > dy)
               {
                  // Top-Right diagonal (right triangle)
                  if(pointLineSide(dx, dy, { 1.0, 0.5 }, { 5/6.0, 1/6.0 }) < 0)
                     col++;
               }
               else
               {
                  // Bottom-Left diagonal (bottom triangle)
                  if(pointLineSide(dx, dy, { 1/6.0, 5/6.0 }, { 0.5, 1.0 }) < 0)
                     row++;
               }
            }
            else
            {
               // Top-Left diagonal
               if(dx > dy)
               {
                  // Top-Right diagonal (top triangle)
                  if(pointLineSide(dx, dy, { 5/6.0, 1/6.0 }, { 0.5, 0.0 }) < 0)
                     row--;
               }
               else
               {
                  // Bottom-Left diagonal (left triangle)
                  if(pointLineSide(dx, dy, { 0.0, 0.5 }, { 1/6.0, 5/6.0 }) < 0)
                     col--;
               }
            }

            if(north && col == p && row == 0)
               root = 0xA, cix = 0;
            else if(south && col == 0 && row == p)
               root = 0xB, cix = 0;
            else
            {
               // REVIEW: REVIEW / Share this logic with getPrimaryChildren(), possibly centroidChild?
               if(col == (int64)p && row < (int64)p && !southRhombus) // Cross at top-dent to the right
               {
                  col = p-row;
                  row = 0;
                  root += 2;
               }
               else if(row == (int64)p && col < (int64)p && southRhombus) // Cross at bottom-dent to the right
               {
                  row = p-col;
                  col = 0;
                  root += 2;
               }
               else
               {
                  if(row < 0 && col < 0)
                     row += p, col += p, root -= 2;
                  else if(row < 0)
                     row += p, root -= 1;
                  else if(col < 0)
                     col += p, root -= 1;
                  else if(col >= (int64)p && row >= (int64)p)
                     row -= p, col -= p, root += 2;
                  else if(row >= (int64)p)
                     row -= p, root += 1;
                  else if(col >= (int64)p)
                     col -= p, root += 1;
               }
               if(root < 0) root += 10;
               else if(root > 9) root -= 10;

               if(row < 0 || row >= p ||
                  col < 0 || col >= p)
               {
      #ifdef _DEBUG
                  // PrintLn("WARNING: Invalid zone calculated");
      #endif
                  return nullZone;
               }
               else
                  cix = row * p + col;
            }
            return I7HZone { l9r, root, cix, 0 };
         }
      }
   }

   int getVertices(Pointd * vertices)
   {
      Pointd c = centroid;
      uint l49R = levelI49R;
      uint64 p = POW7(l49R);
      uint root = rootRhombus;
      uint count = 0;
      double oonp = 1.0 / (7 * p);

      if(c.y > 6 + 1E-9 || c.x > 5 + 1E-9)
         c.x -= 5, c.y -= 5;
      else if(c.x < 0)
         c.x += 5, c.y += 5;

      if(subHex == 0)
      {
         // Even level
         double A =  7 / 3.0;
         double B = 14 / 3.0;

         if(root == 0xA) // North Pole
         {
            Pointd b { 1 - oonp * A, 0 + oonp * A };
            vertices[count++] = { b.x + 0, b.y + 0 };
            vertices[count++] = { b.x + 1, b.y + 1 };
            vertices[count++] = { b.x + 2, b.y + 2 };
            vertices[count++] = { b.x + 3, b.y + 3 };
            vertices[count++] = { b.x + 4, b.y + 4 };
         }
         else if(root == 0xB) // South Pole
         {
            Pointd b { 4 + oonp * A, 6 - oonp * A };
            vertices[count++] = { b.x - 0, b.y - 0 };
            vertices[count++] = { b.x - 1, b.y - 1 };
            vertices[count++] = { b.x - 2, b.y - 2 };
            vertices[count++] = { b.x - 3, b.y - 3 };
            vertices[count++] = { b.x - 4, b.y - 4 };
         }
         else
         {
            Pointd v[6];

            v[0] = { - oonp * A, - oonp * B };
            v[1] = { - oonp * B, - oonp * A };
            v[2] = { - oonp * A, + oonp * A };
            v[3] = { + oonp * A, + oonp * B };
            v[4] = { + oonp * B, + oonp * A };
            v[5] = { + oonp * A, - oonp * A };

            count = addNonPolarBaseVertices(c, v, vertices);
         }
      }
      else
      {
         // Odd level
         double A =  4 / 3.0;
         double B =  5 / 3.0;
         double C =  1 / 3.0;

         if(root > 9 && subHex == 1) // Polar pentagons
         {
            if(root == 0xA) // North pole
            {
               Pointd b { 1 - oonp * C, 0 + oonp * A };

               vertices[count++] = { b.x + 0, b.y + 0 };
               vertices[count++] = { b.x + 1, b.y + 1 };
               vertices[count++] = { b.x + 2, b.y + 2 };
               vertices[count++] = { b.x + 3, b.y + 3 };
               vertices[count++] = { b.x + 4, b.y + 4 };
            }
            else if(root == 0xB) // South pole
            {
               Pointd b { 4 + oonp * C, 6 - oonp * A };

               vertices[count++] = { b.x - 0, b.y - 0 };
               vertices[count++] = { b.x - 1, b.y - 1 };
               vertices[count++] = { b.x - 2, b.y - 2 };
               vertices[count++] = { b.x - 3, b.y - 3 };
               vertices[count++] = { b.x - 4, b.y - 4 };
            }
         }
         else
         {
            // Odd level
            Pointd v[6];

            v[0] = { - oonp * A, - oonp * B };
            v[1] = { - oonp * B, - oonp * C };
            v[2] = { - oonp * C, + oonp * A };
            v[3] = { + oonp * A, + oonp * B };
            v[4] = { + oonp * B, + oonp * C };
            v[5] = { + oonp * C, - oonp * A };

            count = addNonPolarBaseVertices(c, v, vertices);
         }
      }
      return count;
   }

   private static inline void rotate5x6Offset(Pointd r, double dx, double dy, bool clockwise)
   {
      if(clockwise)
      {
         // 60 degrees clockwise rotation
         r.x = dx - dy;
         r.y = dx;
      }
      else
      {
         // 60 degrees counter-clockwise rotation
         r.x = dy;
         r.y = dy - dx;
      }
   }

   private static inline void ::addPointCheckingPole(bool crs84, Array<Pointd> points, const Pointd p)
   {
      /*
      int c = points.count;
      if(c)
      {
         const Pointd * l = &points[c-1];
         double dx = fabs(p.x - l->x), dy = fabs(p.y - l->y);
         bool addExtra = crs84 && (
            (fabs(p.x - 0.5) < dx && fabs(p.y - 0) < dx) ||
            (fabs(p.x - 5) < dx && fabs(p.y - 4.5) < dy) ||
            (fabs(p.x - 2) < dx && fabs(p.y - 3.5) < dy) ||
            (fabs(p.x - 1.5) < dx && fabs(p.y - 3) < dy) ||
            (fabs(l->x - 0.5) < dx && fabs(l->y - 0) < dx) ||
            (fabs(l->x - 5) < dx && fabs(l->y - 4.5) < dy) ||
            (fabs(l->x - 2) < dx && fabs(l->y - 3.5) < dy) ||
            (fabs(l->x - 1.5) < dx && fabs(l->y - 3) < dy));
         if(addExtra)
         {
            #define EXTRA_POLAR_POINTS  10
            int i;
            double dx = p.x - points[c-1].x, dy = p.y - points[c-1].y;
            for(i = 1; i <= EXTRA_POLAR_POINTS; i++)
               points.Add({ l->x + i * dx/EXTRA_POLAR_POINTS, l->y + i * dy/EXTRA_POLAR_POINTS });
         }
         else
            points.Add(p);
      }
      else*/
         points.Add(p);
   }

   void ::addIntermediatePoints(Array<Pointd> points, const Pointd p, const Pointd n, int nDivisions, Pointd i1, Pointd i2, bool crs84)
   {
      double dx = n.x - p.x, dy = n.y - p.y;
      int j;
      bool interruptionNearPole = crs84 && i1 != null && (
         (fabs(i1.x - 0.5) < 1E-6 && fabs(i1.y - 0) < 1E-6) ||
         (fabs(i1.x - 5) < 1E-6 && fabs(i1.y - 4.5) < 1E-6) ||
         (fabs(i1.x - 2) < 1E-6 && fabs(i1.y - 3.5) < 1E-6) ||
         (fabs(i1.x - 1.5) < 1E-6 && fabs(i1.y - 3) < 1E-6) ||
         (fabs(i2.x - 0.5) < 1E-6 && fabs(i2.y - 0) < 1E-6) ||
         (fabs(i2.x - 5) < 1E-6 && fabs(i2.y - 4.5) < 1E-6) ||
         (fabs(i2.x - 2) < 1E-6 && fabs(i2.y - 3.5) < 1E-6) ||
         (fabs(i2.x - 1.5) < 1E-6 && fabs(i2.y - 3) < 1E-6));

      if(!nDivisions) nDivisions = 1;
      if(interruptionNearPole)
         nDivisions *= 20;

      if(dx < -3)
         dx += 5, dy += 5;

      if(i1 != null)
      {
         Pointd pi1 { i1.x - p.x, i1.y - p.y };
         Pointd i2n { n.x - i2.x, n.y - i2.y };
         double l1 = sqrt(pi1.x * pi1.x + pi1.y * pi1.y);
         double l2 = sqrt(i2n.x * i2n.x + i2n.y * i2n.y);
         double length = l1 + l2;
         double t = nDivisions * l1 / length;

         points.Add(p);

         if(nDivisions == 1)
            if(!crs84)
               points.Add(i1), points.Add(i2);

         for(j = 1; j < nDivisions; j += (interruptionNearPole && fabs(j - t) >= 20 ? 20 : 1))
         {
            if(j < t)
               addPointCheckingPole(crs84, points, {
                  p.x + j * pi1.x / t,
                  p.y + j * pi1.y / t
               });

            if((j == (int)t || (j == 1 && !(int)t)))
            {
               addPointCheckingPole(crs84, points, i1);
               points.Add(i2);
            }

            if(j > t)
               addPointCheckingPole(crs84, points, {
                  i2.x + (j - t) * i2n.x / (nDivisions - t),
                  i2.y + (j - t) * i2n.y / (nDivisions - t)
               });
         }
      }
      else
         for(j = 0; j < nDivisions; j++)
            addPointCheckingPole(crs84, points, { p.x + j * dx / nDivisions, p.y + j * dy / nDivisions });
   }

   uint addNonPolarBaseVertices(Pointd c, const Pointd * v, Pointd * vertices)
   {
      int start = 0, prev, i;
      Pointd point, dir;
      int nPoints = this.nPoints;
      uint count = 0;

      // Start with a point outside interruptions
      for(i = 0; i < 6; i++)
      {
         Pointd t { c.x + v[i].x, c.y + v[i].y };
         int tx = (int)floor(t.x + 1E-11);
         if(!(t.y - tx > 2 || t.y < tx))
         {
            start = i;
            break;
         }
      }

      point = { c.x + v[start].x, c.y + v[start].y };
      prev = (start + 5) % 6;
      dir = { point.x - (c.x + v[prev].x), point.y - (c.y + v[prev].y) };

      //vertices[count++] = point;

      for(i = start + 0; i < start + nPoints; i++)
      {
         bool north;
         Pointd i1, i2, n, p = point;

         rotate5x6Offset(dir, dir.x, dir.y, false);
         n = { point.x + dir.x, point.y + dir.y };

         if(p.x > 5 && p.y > 5)
            p.x -= 5, p.y -= 5;
         if(p.x < 0 || p.y < 0)
            p.x += 5, p.y += 5;

         if(crosses5x6Interruption(p, dir.x, dir.y, i1, i2, &north))
         {
            bool crossingLeft;
            Pointd d;

            if(point.x - p.x > 4)
            {
               i1.x += 5, i1.y += 5;
               i2.x += 5, i2.y += 5;
            }
            if(p.x - point.x > 4)
            {
               i1.x -= 5, i1.y -= 5;
               i2.x -= 5, i2.y -= 5;
            }
            if(i2.y - i1.y > 4)
               i2.x -= 5, i2.y -= 5;
            if(i1.y - i2.y > 4)
               i2.x += 5, i2.y += 5;

            crossingLeft = north ? i2.x < i1.x : i2.x > i1.x;

            rotate5x6Offset(d, dir.x - (i1.x - point.x), dir.y - (i1.y - point.y), !crossingLeft);
            n = { i2.x + d.x, i2.y + d.y };
            rotate5x6Offset(dir, dir.x, dir.y, !crossingLeft);

            vertices[count++] = point;
         }
         else if(i < start + nPoints)
            vertices[count++] = point;
         point = n;
      }
      return count;
   }

   void addNonPolarVerticesRefined(Pointd c, const Pointd * v, Array<Pointd> vertices, bool crs84, int nDivisions)
   {
      int start = 0, prev, i;
      Pointd point, dir;
      int nPoints = this.nPoints;

      // Start with a point outside interruptions
      for(i = 0; i < 6; i++)
      {
         Pointd t { c.x + v[i].x, c.y + v[i].y };
         int tx = (int)floor(t.x + 1E-11);
         if(!(t.y - tx > 2 || t.y < tx))
         {
            start = i;
            break;
         }
      }

      point = { c.x + v[start].x, c.y + v[start].y };
      prev = (start + 5) % 6;
      dir = { point.x - (c.x + v[prev].x), point.y - (c.y + v[prev].y) };
      // REVIEW: Is the first point is added twice?
      for(i = start + 1; i <= start + nPoints; i++)
      {
         bool north;
         Pointd i1, i2, n, p = point;

         rotate5x6Offset(dir, dir.x, dir.y, false);
         n = { point.x + dir.x, point.y + dir.y };

         if(p.x > 5 && p.y > 5)
            p.x -= 5, p.y -= 5;
         if(p.x < 0 || p.y < 0)
            p.x += 5, p.y += 5;

         if(crosses5x6Interruption(p, dir.x, dir.y, i1, i2, &north))
         {
            bool crossingLeft;
            Pointd d;

            if(point.x - p.x > 4)
            {
               i1.x += 5, i1.y += 5;
               i2.x += 5, i2.y += 5;
            }
            if(p.x - point.x > 4)
            {
               i1.x -= 5, i1.y -= 5;
               i2.x -= 5, i2.y -= 5;
            }
            if(i2.y - i1.y > 4)
               i2.x -= 5, i2.y -= 5;
            if(i1.y - i2.y > 4)
               i2.x += 5, i2.y += 5;

            crossingLeft = north ? i2.x < i1.x : i2.x > i1.x;
            rotate5x6Offset(d, dir.x - (i1.x - point.x), dir.y - (i1.y - point.y), !crossingLeft);
            n = { i2.x + d.x, i2.y + d.y };
            rotate5x6Offset(dir, dir.x, dir.y, !crossingLeft);

            addIntermediatePoints(vertices, point, n, nDivisions, i1, i2, crs84);
         }
         else
         {
            if(!nDivisions)
               vertices.Add(point);
            else
               addIntermediatePoints(vertices, point, n, nDivisions, null, null, crs84);
         }
         point = n;
      }
   }

   Array<Pointd> getBaseRefinedVertices(bool crs84, int nDivisions)
   {
      Array<Pointd> vertices { minAllocSize = Max(1, nDivisions) * 6 };
      Pointd c = centroid;
      uint l49R = levelI49R;
      uint64 p = POW7(l49R);
      uint root = rootRhombus;
      double oonp = 1.0 / (7 * p);

      if(c.y > 6 + 1E-9 || c.x > 5 + 1E-9)
         c.x -= 5, c.y -= 5;
      else if(c.x < 0)
         c.x += 5, c.y += 5;

      if(subHex == 0)
      {
         // Even level
         double A =  7 / 3.0;
         double B = 14 / 3.0;

         if(root == 0xA) // North Pole
         {
            Pointd a { 1 - oonp * B, 0 - oonp * A };
            Pointd b { 1 - oonp * A, 0 + oonp * A };
            Pointd ab { (a.x + b.x) / 2, (a.y + b.y) / 2 };
            Pointd d;

            rotate5x6Offset(d, b.x - ab.x, b.y - ab.y, false);
            d.x += b.x, d.y += b.y;

            addIntermediatePoints(vertices, { b.x + 0, b.y + 0 }, { b.x + 1, b.y + 1 }, nDivisions, { d.x + 0, d.y + 0 }, { ab.x + 1, ab.y + 1 }, crs84);
            addIntermediatePoints(vertices, { b.x + 1, b.y + 1 }, { b.x + 2, b.y + 2 }, nDivisions, { d.x + 1, d.y + 1 }, { ab.x + 2, ab.y + 2 }, crs84);
            addIntermediatePoints(vertices, { b.x + 2, b.y + 2 }, { b.x + 3, b.y + 3 }, nDivisions, { d.x + 2, d.y + 2 }, { ab.x + 3, ab.y + 3 }, crs84);
            addIntermediatePoints(vertices, { b.x + 3, b.y + 3 }, { b.x + 4, b.y + 4 }, nDivisions, { d.x + 3, d.y + 3 }, { ab.x + 4, ab.y + 4 }, crs84);
            if(crs84)
               addIntermediatePoints(vertices, { b.x + 4, b.y + 4 }, { b.x + 0, b.y + 0 }, nDivisions, { d.x + 4, d.y + 4 }, { ab.x + 0, ab.y + 0 }, crs84);
            else
            {
               vertices.Add({ b.x + 4, b.y + 4 });
               vertices.Add({ d.x + 4, d.y + 4 });
               // These are the "North" pole
               vertices.Add({ 5, 4 });
               vertices.Add({ 1, 0 });
               vertices.Add(ab);
            }
         }
         else if(root == 0xB) // South Pole
         {
            Pointd a { 4 + oonp * B, 6 + oonp * A };
            Pointd b { 4 + oonp * A, 6 - oonp * A };
            Pointd ab { (a.x + b.x) / 2, (a.y + b.y) / 2 };
            Pointd d;

            rotate5x6Offset(d, b.x - ab.x, b.y - ab.y, false);
            d.x += b.x, d.y += b.y;

            addIntermediatePoints(vertices, { b.x - 0, b.y - 0 }, { b.x - 1, b.y - 1 }, nDivisions, { d.x - 0, d.y - 0 }, { ab.x - 1, ab.y - 1 }, crs84);
            addIntermediatePoints(vertices, { b.x - 1, b.y - 1 }, { b.x - 2, b.y - 2 }, nDivisions, { d.x - 1, d.y - 1 }, { ab.x - 2, ab.y - 2 }, crs84);
            addIntermediatePoints(vertices, { b.x - 2, b.y - 2 }, { b.x - 3, b.y - 3 }, nDivisions, { d.x - 2, d.y - 2 }, { ab.x - 3, ab.y - 3 }, crs84);
            addIntermediatePoints(vertices, { b.x - 3, b.y - 3 }, { b.x - 4, b.y - 4 }, nDivisions, { d.x - 3, d.y - 3 }, { ab.x - 4, ab.y - 4 }, crs84);

            if(crs84)
               addIntermediatePoints(vertices, { b.x - 4, b.y - 4 }, { b.x - 0, b.y - 0 }, nDivisions, { d.x - 4, d.y - 4 }, { ab.x - 0, ab.y - 0 }, crs84);
            else
            {
               vertices.Add({ b.x - 4, b.y - 4 });
               vertices.Add({ d.x - 4, d.y - 4 });
               // These are the "South" pole
               vertices.Add({ 0, 2 });
               vertices.Add({ 4, 6 });
               vertices.Add(ab);
            }
         }
         else
         {
            Pointd v[6];

            v[0] = { - oonp * A, - oonp * B };
            v[1] = { - oonp * B, - oonp * A };
            v[2] = { - oonp * A, + oonp * A };
            v[3] = { + oonp * A, + oonp * B };
            v[4] = { + oonp * B, + oonp * A };
            v[5] = { + oonp * A, - oonp * A };

            addNonPolarVerticesRefined(c, v, vertices, crs84, nDivisions);
         }
      }
      else
      {
         // Odd level
         double A =  4 / 3.0;
         double B =  5 / 3.0;
         double C =  1 / 3.0;

         if(root > 9 && subHex == 1) // Polar pentagons
         {
            double r = 1 / 5.0;
            if(root == 0xA) // North pole
            {
               Pointd a { 1 - oonp * B, 0 - oonp * C };
               Pointd b { 1 - oonp * C, 0 + oonp * A };
               Pointd ab { a.x + (b.x - a.x) * r, a.y + (b.y - a.y) * r };
               Pointd c { 1 + oonp * A, 0 + oonp * B };
               Pointd d { b.x + (c.x - b.x) * r, b.y + (c.y - b.y) * r };

               addIntermediatePoints(vertices, { b.x + 0, b.y + 0 }, { b.x + 1, b.y + 1 }, nDivisions, { d.x + 0, d.y + 0 }, { ab.x + 1, ab.y + 1 }, crs84);
               addIntermediatePoints(vertices, { b.x + 1, b.y + 1 }, { b.x + 2, b.y + 2 }, nDivisions, { d.x + 1, d.y + 1 }, { ab.x + 2, ab.y + 2 }, crs84);
               addIntermediatePoints(vertices, { b.x + 2, b.y + 2 }, { b.x + 3, b.y + 3 }, nDivisions, { d.x + 2, d.y + 2 }, { ab.x + 3, ab.y + 3 }, crs84);
               addIntermediatePoints(vertices, { b.x + 3, b.y + 3 }, { b.x + 4, b.y + 4 }, nDivisions, { d.x + 3, d.y + 3 }, { ab.x + 4, ab.y + 4 }, crs84);
               if(crs84)
                  addIntermediatePoints(vertices, { b.x + 4, b.y + 4 }, { b.x + 0, b.y + 0 }, nDivisions, { d.x + 4, d.y + 4 }, { ab.x + 0, ab.y + 0 }, crs84);
               else
               {
                  vertices.Add({ b.x + 4, b.y + 4 });
                  // This extends to right border of last triangle
                  vertices.Add({ d.x + 4, d.y + 4 });
                  // These are the "North" pole
                  vertices.Add({ 5, 4 });
                  vertices.Add({ 1, 0 });
                  vertices.Add(ab);
               }
            }
            else if(root == 0xB) // South pole
            {
               Pointd a { 4 + oonp * B, 6 + oonp * C };
               Pointd b { 4 + oonp * C, 6 - oonp * A };
               Pointd ab { a.x + (b.x - a.x) * r, a.y + (b.y - a.y) * r };
               Pointd c { 4 - oonp * A, 6 - oonp * B };
               Pointd d { b.x + (c.x - b.x) * r, b.y + (c.y - b.y) * r };

               addIntermediatePoints(vertices, { b.x - 0, b.y - 0 }, { b.x - 1, b.y - 1 }, nDivisions, { d.x - 0, d.y - 0 }, { ab.x - 1, ab.y - 1 }, crs84);
               addIntermediatePoints(vertices, { b.x - 1, b.y - 1 }, { b.x - 2, b.y - 2 }, nDivisions, { d.x - 1, d.y - 1 }, { ab.x - 2, ab.y - 2 }, crs84);
               addIntermediatePoints(vertices, { b.x - 2, b.y - 2 }, { b.x - 3, b.y - 3 }, nDivisions, { d.x - 2, d.y - 2 }, { ab.x - 3, ab.y - 3 }, crs84);
               addIntermediatePoints(vertices, { b.x - 3, b.y - 3 }, { b.x - 4, b.y - 4 }, nDivisions, { d.x - 3, d.y - 3 }, { ab.x - 4, ab.y - 4 }, crs84);
               if(crs84)
                  addIntermediatePoints(vertices, { b.x - 4, b.y - 4 }, { b.x - 0, b.y - 0 }, nDivisions, { d.x - 4, d.y - 4 }, { ab.x - 0, ab.y - 0 }, crs84);
               else
               {
                  vertices.Add({ b.x - 4, b.y - 4 });
                  // This extends to left wrapping point
                  vertices.Add({ d.x - 4, d.y - 4 });
                  // These are the "South" pole
                  vertices.Add({ 0, 2 });
                  vertices.Add({ 4, 6 });
                  vertices.Add(ab);
               }
            }
         }
         else
         {
            // Odd level
            Pointd v[6];

            v[0] = { - oonp * A, - oonp * B };
            v[1] = { - oonp * B, - oonp * C };
            v[2] = { - oonp * C, + oonp * A };
            v[3] = { + oonp * A, + oonp * B };
            v[4] = { + oonp * B, + oonp * C };
            v[5] = { + oonp * C, - oonp * A };

            addNonPolarVerticesRefined(c, v, vertices, crs84, nDivisions);
         }
      }
      return vertices;
   }

   property I7HZone centroidChild
   {
      get
      {
         if(this == nullZone)
            return nullZone;
         else
         {
            uint root = rootRhombus;
            uint64 ix = rhombusIX;

            if(!subHex) // Odd level from even level
               return I7HZone { levelI49R, root, ix, 1 };
            else // Even level from odd level
            {
               uint64 p = POW7(levelI49R), cp = p * 7;
               int64 row, col;
               int cRhombus;

               if(root == 0xA)
               {
                  // North pole
                  if(subHex == 1)
                     return I7HZone { levelI49R + 1, 0xA, 0, 0 };

                  row = 1, col = cp - 2;
                  cRhombus = 2*(subHex - 2);
               }
               else if(root == 0xB)
               {
                  // South pole
                  if(subHex == 1)
                     return I7HZone { levelI49R + 1, 0xB, 0, 0 };

                  row = cp - 1, col = 2;
                  cRhombus = 9 - 2*(subHex - 2);
               }
               else
               {
                  // Regular case
                  uint sh = subHex;
                  bool south;

                  cRhombus = root;

                  south = (cRhombus & 1);
                  row = 7LL * (ix / p), col = 7LL * (ix % p);

                  if(ix == 0 && south && sh >= 4)
                     sh++;

                  switch(sh)
                  {
                     case 2: row -= 3; col -= 1; break;
                     case 3: row -= 2; col -= 3; break;
                     case 4: row += 1; col -= 2; break;
                     case 5: row += 3; col += 1; break;
                     case 6: row += 2; col += 3; break;
                     case 7: row -= 1; col += 2; break;
                  }

                  if(col == (int64)cp && row < (int64)cp && !south) // Cross at top-dent to the right
                  {
                     col = cp-row;
                     row = 0;
                     cRhombus += 2;
                  }
                  else if(row == (int64)cp && col < (int64)cp && south) // Cross at bottom-dent to the right
                  {
                     row = cp-col;
                     col = 0;
                     cRhombus += 2;
                  }
                  else if(col > 0 && col < (int64)cp && row < 0 && !south) // Cross at top-dent to the left
                  {
                     int64 ncol = cp + row;
                     int64 nrow = cp - col;
                     col = ncol;
                     row += nrow;
                     cRhombus -= 2;
                  }
                  else if(row > 0 && row < (int64)cp && col < 0 && south) // Cross at bottom-dent to the left
                  {
                     int64 nrow = cp + col;
                     int64 ncol = cp - row;
                     row = nrow;
                     col += ncol;
                     cRhombus -= 2;
                  }

                  if(row < 0 && col < 0)
                     row += cp, col += cp, cRhombus -= 2;
                  else if(row < 0)
                     row += cp, cRhombus -= 1;
                  else if(col < 0)
                     col += cp, cRhombus -= 1;
                  else if(col >= cp && row >= cp)
                     row -= cp, col -= cp, cRhombus += 2;
                  else if(row >= cp)
                     row -= cp, cRhombus += 1;
                  else if(col >= cp)
                     col -= cp, cRhombus += 1;
               }
               if(cRhombus < 0) cRhombus += 10;
               else if(cRhombus > 9) cRhombus -= 10;

               if(row >= 0 && col >= 0 && row < cp && col < cp)
                  return I7HZone { levelI49R + 1, cRhombus, (uint64)row * cp + col, 0 };
               return nullZone;
            }
         }
      }
   }

   int getChildren(I7HZone children[13])
   {
      int n = getPrimaryChildren(children);
      if(n)
      {
         Pointd c = children[0].centroid;
         Pointd cVerts[6];
         int nv = children[0].getVertices(cVerts);
         int i;
         int cLevel = children[0].level;
         bool north = c.x - c.y - 1E-11 > 0;
         int cy = (int)(c.y + 1E-11);
         int cx = (int)(c.x + 1E-11);

#if 0 //def _DEBUG
         char pID[128];
         getZoneID(pID);
         //PrintLn("Calculating children of ", pID);
#endif

         for(i = 0; i < nv; i++)
         {
            Pointd cc = c;
            Pointd v;

            double dx = cVerts[i].x - cc.x;
            double dy = cVerts[i].y - cc.y;

            if(dx > 3 && dy > 3)
            {
               dx = cVerts[i].x - 5 - cc.x;
               dy = cVerts[i].y - 5 - cc.y;
            }
            else if(dx < -3 && dy < -3)
            {
               dx = cVerts[i].x + 5 - cc.x;
               dy = cVerts[i].y + 5 - cc.y;
            }

            if(fabs(dx) < 1 && fabs(dy) < 1)
            {
               // We need to avoid computing dx and dy across interuptions
               if(( north && fabs(c.y - cy) < 1E-11) ||
                  (!north && fabs(c.x - cx) < 1E-11))
               {
                  double x, y;
                  Pointd ci;
                  cross5x6Interruption(c, ci, !north, true);

                  x = cVerts[i].x - ci.x;
                  y = cVerts[i].y - ci.y;

                  if(x > 3 && dy > 3)
                  {
                     x = cVerts[i].x - 5 - ci.x;
                     y = cVerts[i].y - 5 - ci.y;
                  }
                  else if(x < -3 && y < -3)
                  {
                     x = cVerts[i].x + 5 - ci.x;
                     y = cVerts[i].y + 5 - ci.y;
                  }

                  if(fabs(x) < fabs(dx) && fabs(y) < fabs(dy))
                  {
                     cc = ci;
                     dx = x;
                     dy = y;
                  }
               }
            }

            move5x6Vertex2(v, cc, dx * 3, dy * 3, false);

            canonicalize5x6(v, v);

            children[n++] = fromCentroid(cLevel, v);
#if 0 //def _DEBUG
            char cID[256];
            children[n-1].getZoneID(cID);
            Print(""); //Ln(cID);
#endif
         }
      }
      return n;
   }

   int getPrimaryChildren(I7HZone children[7])
   {
      int count = 0;
      uint l49r = levelI49R;
      uint root = rootRhombus;
      uint64 rix = rhombusIX;

      if(this == nullZone)
         return 0;

      if(subHex == 0)
      {
         // Odd levels from even level
         children[count++] = { l49r, root, rix, 1 };
         children[count++] = { l49r, root, rix, 2 };
         children[count++] = { l49r, root, rix, 3 };
         children[count++] = { l49r, root, rix, 4 };
         children[count++] = { l49r, root, rix, 5 };
         children[count++] = { l49r, root, rix, 6 };
         if(rix)
            children[count++] = { l49r, root, rix, 7 };
      }
      else
      {
         // Even levels from odd level
         I7HZone cChild = centroidChild;
         uint cRoot = cChild.rootRhombus;
         uint64 ccix = cChild.rhombusIX;
         uint64 p = POW7(l49r), cp = p * 7;

         children[count++] = cChild;

         if(cRoot == 0xA)
         {
            // The new centroid child is the North pole
            children[count++] = { l49r + 1, 0, 0 * cp + cp - 1 };
            children[count++] = { l49r + 1, 2, 0 * cp + cp - 1 };
            children[count++] = { l49r + 1, 4, 0 * cp + cp - 1 };
            children[count++] = { l49r + 1, 6, 0 * cp + cp - 1 };
            children[count++] = { l49r + 1, 8, 0 * cp + cp - 1 };
         }
         else if(cRoot == 0xB)
         {
            // The new centroid child is the South pole
            children[count++] = { l49r + 1, 9, (cp - 1) * cp + 0 };
            children[count++] = { l49r + 1, 7, (cp - 1) * cp + 0 };
            children[count++] = { l49r + 1, 5, (cp - 1) * cp + 0 };
            children[count++] = { l49r + 1, 3, (cp - 1) * cp + 0 };
            children[count++] = { l49r + 1, 1, (cp - 1) * cp + 0 };
         }
         else
         {
            int ccRhombus = cRoot;
            uint64 crix = ccix;
            int64 crow = crix / cp;
            int64 ccol = crix % cp;
            int i;
            static const int cOffsets[6][2] = { // row, col offsets from centroid child
               { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }
            };
            int nPoints = (subHex > 1) ? 6 : (rix == 0) ? 5 : 6;
            bool south = (ccRhombus & 1);

            for(i = 0; i < 6; i++)
            {
               int64 row = crow + cOffsets[i][0], col = ccol + cOffsets[i][1];
               int cRhombus = ccRhombus;
               uint64 cix;

               if(nPoints == 5)
               {
                  if(south && i == 2)
                  {
                     continue;
                  }
                  else if(!south && i == 0)
                  {
                     continue;
                  }
               }

               // REVIEW: Handling crossing interruptions correctly here...
               if(col == (int64)cp && row < (int64)cp && !south) // Cross at top-dent to the right
               {
                  col = cp-row;
                  row = 0;
                  cRhombus += 2;
               }
               else if(row == (int64)cp && col < (int64)cp && south) // Cross at bottom-dent to the right
               {
                  row = cp-col;
                  col = 0;
                  cRhombus += 2;
               }
               else if(col > 0 && col < (int64)cp && row < 0 && !south) // Cross at top-dent to the left
               {
                  int64 ncol = cp + row;
                  int64 nrow = cp - col;
                  col = ncol;
                  row += nrow;
                  cRhombus -= 2;
               }
               else if(row > 0 && row < (int64)cp && col < 0 && south) // Cross at bottom-dent to the left
               {
                  int64 nrow = cp + col;
                  int64 ncol = cp - row;
                  row = nrow;
                  col += ncol;
                  cRhombus -= 2;
               }
               // REVIEW: Wrapping without crossing interruption
               else
               {
                  if(row < 0 && col < 0)
                     row += cp, col += cp, cRhombus -= 2;
                  else if(row < 0)
                     row += cp, cRhombus -= 1;
                  else if(col < 0)
                     col += cp, cRhombus -= 1;
                  else if(col >= (int64)cp && row >= (int64)cp)
                     row -= cp, col -= cp, cRhombus += 2;
                  else if(row >= (int64)cp)
                     row -= cp, cRhombus += 1;
                  else if(col >= (int64)cp)
                     col -= cp, cRhombus += 1;
               }

               if(cRhombus < 0) cRhombus += 10;
               else if(cRhombus > 9) cRhombus -= 10;

               if(row >= 0 && col >= 0 && row < cp && col < cp)
               {
                  cix = row * cp + col;
                  children[count++] = { l49r + 1, cRhombus, cix, 0 };
               }
#ifdef _DEBUG
               else
                  PrintLn("WARNING: out of range row/col");
#endif
            }
         }
      }
      return count;
   }

   property CRSExtent ri5x6Extent
   {
      get
      {
         Array<Pointd> vertices = getBaseRefinedVertices(false, 0);
         int nVertices = vertices ? vertices.count : 0, i;

         value.tl.x = MAXDOUBLE, value.tl.y = MAXDOUBLE;
         value.br.x = -MAXDOUBLE, value.br.y = -MAXDOUBLE;
         for(i = 0; i < nVertices; i++)
         {
            const Pointd * v = &vertices[i];
            double x = v->x, y = v->y;

            if(y > value.br.y) value.br.y = y;
            if(y < value.tl.y) value.tl.y = y;
            if(x > value.br.x) value.br.x = x;
            if(x < value.tl.x) value.tl.x = x;
         }
         delete vertices;
      }
   }

   property Pointd centroid
   {
      get
      {
         int l49r = levelI49R;
         uint64 p = POW7(l49r);
         Pointd v;
         double oop = 1.0 / p;
         int root = rootRhombus;
         uint64 rix = rhombusIX;
         bool south = root & 1;
         int sh = subHex;

         if(root == 0xA) // North pole
         {
            v = { 1, 0 };

            if(sh > 1)
            {
               v.x += sh - 2 - 2 * oop/7;
               v.y += sh - 2 + 1 * oop/7;
            }
         }
         else if(root == 0xB) // South pole
         {
            v = { 4, 6 };

            if(sh > 1)
            {
               v.x -= sh - 2 - 2 * oop/7;
               v.y -= sh - 2 + 1 * oop/7;
            }
         }
         else
         {
            int cx = (root >> 1), cy = cx + (root & 1);
            int64 row = rix / p, col = rix % p;

            v.x = cx + col * oop;
            v.y = cy + row * oop;
         }

         if(subHex && root < 10)
         {
            // Odd level
            if(rix == 0 && south && sh >= 4)
               sh++;

            oop /= 7;

            switch(sh)
            {
               case 1: value = v; break; // Centroid child
               // NOTE: move5x6Vertex2() does not generate correct geometry at level 2
               case 2: move5x6Vertex(value, v, - 1 * oop, - 3 * oop); break;
               case 3: move5x6Vertex(value, v, - 3 * oop, - 2 * oop); break;
               case 4: move5x6Vertex(value, v, - 2 * oop, + 1 * oop); break;
               case 5: move5x6Vertex(value, v, + 1 * oop, + 3 * oop); break;
               case 6: move5x6Vertex(value, v, + 3 * oop, + 2 * oop); break;
               case 7: move5x6Vertex(value, v, + 2 * oop, - 1 * oop); break;
            }
         }
         else  // Even level
            value = v;

         if(fabs(value.y - 0) < 1E-6)
            value.y = 0;
         else if(fabs(value.x - 5) < 1E-6)
            value.x = 5;
         if(value.x > 5 - 1E-6 || value.y > 6 + 1E-6)
            value.x -= 5, value.y -= 5;
         if(value.x < 0 - 1E-6)
            value.x += 5, value.y += 5;
      }
   }

   property bool isCentroidChild
   {
      get
      {
         if(subHex == 1)  // All '-B' are centroid children
            return true;
         else if(subHex == 0)
         {
            // Some '-A' are centroid children
            I7HZone parent0 = this.parent0;
            return parent0 != nullZone && this == parent0.centroidChild;
         }
         return false;
      }
   }

#if 0
   private static inline int64 ::triNumber(int64 n)
   {
      if (n <= 0) return 0;
      return n * (n + 1) / 2;
   }

   private static inline int64 ::sumArithmeticSeries(int64 n, int64 firstTerm, int64 commonDiff)
   {
       if (n <= 0) return 0;
       // return n * (2 * firstTerm + (n - 1) * commonDiff) / 2;
       return n * firstTerm + commonDiff * triNumber(n - 1);
   }

   private static inline int64 ::computeAddedContrib(uint64 nScanlines, int zonesAdded, uint nTimesAdditionRepeated,
      int scanlineGapBetweenRepetitions, int phaseOffsetFromFirstAddition)
   {
      int64 total = 0;

      if(nScanlines)
      {
         int patternSize = nTimesAdditionRepeated + scanlineGapBetweenRepetitions;
         int p;

         for(p = 0; p < nTimesAdditionRepeated; p++)
         {
            int64 firstIncLine = (p - phaseOffsetFromFirstAddition + patternSize) % patternSize;
            if(firstIncLine < nScanlines)
            {
               int64 n = (nScanlines - 1 - firstIncLine) / patternSize + 1;
               int64 firstTerm = (int64)zonesAdded * (nScanlines - firstIncLine);
               int64 commonDiff = -(int64)zonesAdded * patternSize;
               total += sumArithmeticSeries(n, firstTerm, commonDiff);
            }
         }
      }
      return total;
   }

   private static inline int64 ::computeHexOddDepthSubZones(int rDepth)
   {
       int64 nInterSL = POW7((rDepth - 1) / 2);
       int64 nCapSL = (int64)(ceil(nInterSL / 3.0) + 0.1);
       int64 nMidSL = (int64)(ceil(nInterSL * 2 / 3.0) + 0.1);

       // A..B
       int64 abRight = computeAddedContrib(nCapSL - 1, 5, 1, 0, 0);
       int64 abLeft = computeAddedContrib(nCapSL - 1, 1, 1, 3, 0);
       int64 nZonesAB = nCapSL * 1 + abLeft + abRight;
       int64 abLeftAddition = (nCapSL > 1 ? (nCapSL - 2) / 4 + 1 : 0);

       // B..C
       int64 bZonesPerSL = 1 + 5 * (nCapSL - 1) + abLeftAddition;
       int64 bcLeft = computeAddedContrib(nInterSL, 1, 1, 3, (int)((nCapSL - 1) % 4));
       int64 bcRight = 2 * nInterSL + computeAddedContrib(Max(0, nInterSL - 2), 1, 4, 1, 0);
       int64 nZonesBC = nInterSL * bZonesPerSL + bcLeft + bcRight;

       // C..D
       int64 bcLeftInc = (nCapSL + nInterSL - 1 + 3) / 4 - abLeftAddition;
       int64 bcRightInc = 2 + (nInterSL - 2)/5 * 4 + (nInterSL - 2) % 5;
       int64 cZonesPerSL = bZonesPerSL + bcLeftInc + bcRightInc;
       int64 cdLeft = -1 * nMidSL + computeAddedContrib(Max(0, nMidSL - 2), -1, 4, 1, 0);
       int64 cdRight = computeAddedContrib(nMidSL, 1, 4, 1, Max(0, nInterSL - 2) % 5);
       int64 nZonesCD = nMidSL * cZonesPerSL + cdLeft + cdRight;

       return 2 * nZonesAB + 2 * nZonesBC + nZonesCD;
   }
#endif

   int64 getSubZonesCount(int rDepth)
   {
      if(rDepth > 0)
      {
         int64 nHexSubZones;

         if(rDepth & 1)
         {
#if 0 // def _DEBUG
            int64 nInterSL = (int64)(POW7((rDepth-1) / 2));
            int64 nCapSL = (int64)(ceil(nInterSL / 3.0) + 0.5);
            int64 nMidSL = (int64)(ceil(nInterSL * 2 / 3.0) + 0.5);
            // int64 nScanlines = 2 * nCapSL + 2 * nInterSL + nMidSL;
            int64 s;
            int64 zonesPerSL = 1;
            int64 leftACCounter = 0, leftCECounter = 0, rightBDCounter = 0;
            int64 nZonesAB = 0, nZonesBC = 0, nZonesCD = 0;

            for(s = 0; s < nCapSL; s++)
            {
               int64 left = (s > 0 && (leftACCounter++) % 4 == 0) ? 1 : 0;
               int64 right = (s > 0) ? 5 : 0;
               zonesPerSL += left + right;
               nZonesAB += zonesPerSL;
            }

            for(s = 0; s < nInterSL; s++)
            {
               int64 left = ((leftACCounter++) % 4 == 0) ? 1 : 0;
               int64 right = (s == 0) ? 2 : ((rightBDCounter++) % 5 != 0) ? 1 : 0;
               zonesPerSL += left + right;
               nZonesBC += zonesPerSL;
            }

            for(s = 0; s < nMidSL; s++)
            {
               int64 left = (s == 0 || (leftCECounter++) % 5 != 0) ? -1 : 0;
               int64 right = ((rightBDCounter++) % 5 != 0) ? 1 : 0;
               zonesPerSL += left + right;
               nZonesCD += zonesPerSL;
            }
            nHexSubZones = 2 * nZonesAB + 2 * nZonesBC + nZonesCD;
#endif
            // nHexSubZones = computeHexOddDepthSubZones(rDepth);
            //                            https://oeis.org/A199422
            nHexSubZones = POW7(rDepth) + 5 * POW7((rDepth-1)/2) + 1;
         }
         else
                                       // https://oeis.org/A024075
            nHexSubZones = POW7(rDepth) + POW7(rDepth/2) - 1;
         return (nHexSubZones * nPoints + 5) / 6;
      }
      return 1;
   }

   I7HZone getFirstSubZone(int rDepth)
   {
      Pointd firstCentroid;

      getFirstSubZoneCentroid(rDepth, firstCentroid);
      return fromCentroid(level + rDepth, firstCentroid);
   }

   void getFirstSubZoneCentroid(int rDepth, Pointd firstCentroid)
   {
      // TODO: Correctly handling polar cases
      Pointd vertices[6];
      int n = getVertices(vertices);
      int i, top = 0;
      Pointd topIco;
      Pointd * v;
      double dx, dy;
      int level = this.level;
      int64 szp = POW7((level + 1 + rDepth) / 2);

      RI5x6Projection::toIcosahedronNet(vertices[0], topIco);

      for(i = 1; i < n; i++)
      {
         Pointd ico;
         RI5x6Projection::toIcosahedronNet(vertices[i], ico);
         if(ico.y > topIco.y)
         {
            topIco = ico;
            top = i;
         }
      }
      v = &vertices[top];

      if(rDepth & 1) // Odd depth
      {
         // First sub-zone centroid is one sub-zone edge length away from sub-zone's vertex preceding shared vertex
         if(level & 1)
         {
            dx = 2 / (3.0 * szp);
            dy = 1 / (3.0 * szp);
         }
         else
         {
            dx = -4 / (3.0 * szp);
            dy = -5 / (3.0 * szp);
         }
      }
      else // Even depth
      {
         // First sub-zone centroid is two sub-zone edges length towards next vertex
         if(level & 1)
         {
            dx = -10 / (3.0 * szp);
            dy =  -2 / (3.0 * szp);
         }
         else
         {
            dx = -4 / (3.0 * szp);
            dy = -2 / (3.0 * szp);
         }
      }
      move5x6(firstCentroid, v, dx, dy, 1);
   }

   Array<Pointd> getSubZoneCentroids(int rDepth)
   {
      int64 count = getSubZonesCount(rDepth);
      Array<Pointd> centroids { size = (uint) count };

      if(rDepth == 0)
         centroids[0] = centroid;
      else
      {
         int level = this.level;
         bool oddAncestor = level & 1;
         Pointd first;
         int szLevel = level + rDepth;
         int64 szp = POW7((szLevel + (oddAncestor^(rDepth&1)))/2);
         double c2c = 1.0 / szp; // Centroid to centroid distance between sub-zones along 5x6 x and y axes
         int64 cStart = 0;
         int64 index = 0;
         int64 s;
         int64 zonesPerSL;
         int64 nScanlines;
         int64 left, right;
         Pointd sc; // Start of scanline
         int64 i;

         // TODO: Handle pentagons / polar zones correctly
         if(nPoints == 5) { delete centroids; return null; }

         // TODO: Verify interrupted hexagons
         getFirstSubZoneCentroid(rDepth, first);

         if(rDepth & 1) // Odd depths
         {
            int64 nInterSL = (int64)(POW7((rDepth-1) / 2));
            int64 nCapSL = (int64)(ceil(nInterSL / 3.0) + 0.5);
            int64 nMidSL = (int64)(ceil(nInterSL * 2 / 3.0) + 0.5);
            int64 B = nCapSL, C = B + nInterSL, D = C + nMidSL, E = D + nInterSL;
            int64 leftACCounter = 0, leftCECounter = 0, rightBDCounter = 0, rightDFCounter = 0;

            nScanlines = 2 * nCapSL + 2 * nInterSL + nMidSL;
            zonesPerSL = 1;

            for(s = 0; s < nScanlines; s++)
            {
               if(s < B)
               {
                  left = (s > 0 && (leftACCounter++) % 4 == 0) ? 1 : 0;
                  right = (s > 0) ? 5 : 0;
               }
               else if(s < C)
               {
                  left = ((leftACCounter++) % 4 == 0) ? 1 : 0;
                  right = (s == B) ? 2 : ((rightBDCounter++) % 5 != 0) ? 1 : 0;
               }
               else if(s < D)
               {
                  left = (s == C || (leftCECounter++) % 5 != 0) ? -1 : 0;
                  right = ((rightBDCounter++) % 5 != 0) ? 1 : 0;
               }
               else if(s < E)
               {
                  left = ((leftCECounter++) % 5 != 0) ? -1 : 0;
                  right = (s == D) ? 1 : ((rightDFCounter++) % 4) == 0 ? -1 : 0;
               }
               else
               {
                  left = s == E ? -2 : -5;
                  right = ((rightDFCounter++) % 4) == 0 ? -1 : 0;
               }

               if(oddAncestor)
               {
                  cStart += left;
                  move5x6(sc, first, -(s + cStart) * c2c, -cStart * c2c, 1);
               }
               else
               {
                  cStart += right;
                  move5x6(sc, first, s * c2c - cStart * c2c*3, s * c2c*3 - cStart * 2*c2c, 1);
               }
               zonesPerSL += left + right;

               for(i = 0; i < zonesPerSL; i++)
                  move5x6(centroids[(int)(index++)], sc, i * c2c * (oddAncestor ? 1 : 3), i * c2c * (oddAncestor ? 1 : 2), 1);
            }
         }
         else // Even depths
         {
            int64 nCapSL = (POW7(rDepth/2) - 1) / 3;
            int64 nMidSL = (2*POW7(rDepth/2) + 1)/3;
            int64 B = nCapSL, C = B + nMidSL;

            nScanlines = 2 * nCapSL + nMidSL;
            zonesPerSL = 3;

            for(s = 0; s < nScanlines; s++)
            {
               if(s < B)
               {
                  left = s > 0 ? 1 : 0;
                  right = s > 0 ? 2 : 0;
               }
               else if(s < C)
               {
                  left = s == B || ((s - B) & 1) ? 0 : -1;
                  right = (s == B) || ((s - B) & 1) ? 1 : 0;
               }
               else
               {
                  left = s == C ? -1 : -2;
                  right = s == C ? 0 : -1;
               }
               cStart += left;

               if(oddAncestor)
                  move5x6(sc, first, -2*s * c2c - cStart * c2c*3, s * c2c - cStart * 2*c2c, 1);
               else
                  move5x6(sc, first, -(s + cStart) * c2c, -cStart * c2c, 1);
               zonesPerSL += left + right;

               for(i = 0; i < zonesPerSL; i++)
                  move5x6(centroids[(int)(index++)], sc, i * c2c * (oddAncestor ? 3 : 1), i * c2c * (oddAncestor ? 2 : 1), 1);
            }
         }
      }
      return centroids;
   }

   private /*static */bool orderZones(int zoneLevel, AVLTree<I7HZone> tsZones, Array<I7HZone> zones)
   {
      Array<Pointd> centroids = getSubZoneCentroids(zoneLevel - level);
      if(centroids)
      {
         int nSubZones = centroids.count;
         int i;

         for(i = 0; i < nSubZones; i++)
         {
            I7HZone key = I7HZone::fromCentroid(zoneLevel, centroids[i]);
            if(tsZones.Find(key))
               zones.Add(key);
      #ifdef _DEBUG
            else
               PrintLn("WARNING: mismatched sub-zone while re-ordering");
      #endif
         }
         delete centroids;
         return true;
      }
      else
         return false; // Work around until all sub-zone listing fully handled
   }
}

// TODO: Review / Test / Adapt this logic for 7H
__attribute__((unused)) static void compactI7HZones(AVLTree<I7HZone> zones, int level)
{
   AVLTree<I7HZone> output { };
   AVLTree<I7HZone> next { };
   int l;

   for(l = level - 1; l >= 0; l -= 1)
   {
      int i;
      for(z : zones)
      {
         I7HZone zone = z, cParents[2];
         int nCParents = zone.getParents(cParents);
         int p;
         for(p = 0; p < nCParents; p++)
         {
            I7HZone cParent = cParents[p];
            if(cParent != nullZone && !next.Find(cParent))
            {
               I7HZone children[13];
               bool parentAllIn = true;
               int nChildren = cParent.getChildren(children);

               for(i = 0; i < nChildren; i++)
               {
                  I7HZone c = children[i];
                  if(c != nullZone && !zones.Find(c))
                  {
                     parentAllIn = false;
                     break;
                  }
               }

               if(parentAllIn)
                  next.Add(cParent);
            }
         }
      }

      for(z : zones)
      {
         I7HZone zone = z, cParents[2];
         int nCParents = zone.getParents(cParents), i;
         bool allIn = true;

         for(i = 0; i < nCParents; i++)
         {
            if(!next.Find(cParents[i]))
            {
               allIn = false;
               break;
            }
         }
         if(!allIn)
            output.Add(zone);
      }

      if(/*0 && */l - 1 >= 0 && next.count)
      {
         // Not done -- next level becomes zones to compact
         zones.copySrc = next;
         next.Free();
      }
      else
      {
         // Done -- next is combined with output into final zones
         zones.copySrc = output;
         for(z : next)
            zones.Add(z);
         //break;
      }
   }

   delete output;
   delete next;

   if(zones.count >= 72 && zones.firstIterator.data.level == 1)
   {
      int nL1 = 0;
      /*
      // REVIEW: Sometimes all level 1 zones are included, but extra sub zones as well
      bool allL1 = true;
      for(z : zones; z.level != 1)
      {
         allL1 = false;
         break;
      }
      */
      for(z : zones)
      {
         I7HZone zone = z;
         int level = zone.level;
         if(level == 1)
            nL1++;
         else if(level > 1)
            break;
      }

      // if(allL1)
      if(nL1 == 72)
      {
         // Simplifying full globe to level 0 zones
         int r;
         zones.Free();
         for(r = 0; r < 12; r++)
            zones.Add({ r, 0 });
      }
   }
}

/* TODO:
static bool findByIndex(Pointd centroid, int64 index, const Pointd c)
{
   centroid = c;
   return false;
}

static bool findSubZone(const Pointd szCentroid, int64 index, const Pointd c)
{
   Pointd centroid;

   canonicalize5x6(c, centroid);
   if(fabs(centroid.x - szCentroid.x) < 1E-11 &&
      fabs(centroid.y - szCentroid.y) < 1E-11)
      return false;
   return true;
   // return *zone != I7HZone::fromCentroid(zone->level, centroid);
}
*/

static void getIcoNetExtentFromVertices(I7HZone zone, CRSExtent value)
{
   int i;
   Array<Pointd> vertices = getIcoNetRefinedVertices(zone, 0, true);
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

static Array<Pointd> getIcoNetRefinedVertices(I7HZone zone, int edgeRefinement, bool ico)   // 0 for 1-20 based on level
{
   Array<Pointd> rVertices = zone.getBaseRefinedVertices(false, edgeRefinement);
   if(rVertices && rVertices.count && ico)
   {
      int i;
      for(i = 0; i < rVertices.count; i++)
      {
         Pointd p;
         RI5x6Projection::toIcosahedronNet(rVertices[i], p);
         rVertices[i] = p;
      }
   }
   return rVertices;
}
