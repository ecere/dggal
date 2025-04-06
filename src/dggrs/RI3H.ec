// This forms a basis for a number of aperture 3 hexagonal grids
// using different projections based on the Rhombic Icosahedral 5x6 space
public import IMPORT_STATIC "ecrt"
private:

import "dggrs"
import "ri5x6"
import "I3HSubZones"
import "RI9R"

#include <stdio.h>

// These DGGRSs have the topology of Goldberg polyhedra class I and II with m = 3^n

// Goldberg polyhedra: https://en.wikipedia.org/wiki/Goldberg_polyhedron
/*                                                              dk = td (for next step)
I3H                                                             tk for two steps (= dkdk = tdtd); dktk = tkdk                (m + n)^2 − mn       10T + 2
level GP notation Name                             Class       Conway                                                            T                 Count
   0: GP(1,0)     dodecahedron                     1           D     dI                  dI               D                D     1                    12
   1: GP(1,1)     truncated icosahedron            2         dkD     tI                dkdI             dkD              tdD     3                    32
   2: GP(3,0)     truncated pentakis dodecahedron  1         tkD   dktI              dkdkdI           dkdkD            tdtdD     9                    92
   3: GP(3,3)                                      2       tkdkD   tktI            dkdkdkdI         dkdkdkD          tdtdtdD     27                  272
   4: GP(9,0)                                      1       tktkD dktktI          dkdkdkdkdI       dkdkdkdkD        tdtdtdtdD     81                  812
   5: GP(9,9)                                      2     dktktkD tktktI        dkdkdkdkdkdI     dkdkdkdkdkD      tdtdtdtdtdD    243                 2432
*/

#define POW3(x) ((x) < sizeof(powersOf3) / sizeof(powersOf3[0]) ? (uint64)powersOf3[x] : (uint64)pow(3, x))

public class RhombicIcosahedral3H : DGGRS
{
   RI5x6Projection pj;
   bool equalArea;

   // DGGH
   uint64 countZones(int level)
   {
      return (uint64)(10 * POW3(level) + 2);
   }

   int getMaxDGGRSZoneLevel() { return 33; }
   int getRefinementRatio() { return 3; }
   int getMaxParents() { return 3; }
   int getMaxNeighbors() { return 6; }
   int getMaxChildren() { return 7; }

   uint64 countSubZones(I3HZone zone, int depth)
   {
      return zone.getSubZonesCount(depth);
   }

   int getZoneLevel(I3HZone zone)
   {
      return zone.level;
   }

   int countZoneEdges(I3HZone zone) { return zone.nPoints; }

   bool isZoneCentroidChild(I3HZone zone)
   {
      return zone.isCentroidChild;
   }

   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   double getZoneArea(I3HZone zone)
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
         // FIXME: Is there a simple way to directly compute the area for other RI3H ?
         area = 0;
#endif
      }
      return area;
   }

   I3HZone getZoneFromCRSCentroid(int level, CRS crs, const Pointd centroid)
   {
      if(level <= 33)
      {
         switch(crs)
         {
            case 0: case CRS { ogc, 153456 }:
               return I3HZone::fromCentroid(level, centroid);
            case CRS { ogc, 1534 }:
            {
               Pointd c5x6;
               RI5x6Projection::fromIcosahedronNet({ centroid.x, centroid.y }, c5x6);
               return I3HZone::fromCentroid(level, { c5x6.x, c5x6.y });
            }
            case CRS { epsg, 4326 }:
            case CRS { ogc, 84 }:
               return (I3HZone)getZoneFromWGS84Centroid(level,
                  crs == { ogc, 84 } ?
                     { centroid.y, centroid.x } :
                     { centroid.x, centroid.y });
         }
      }
      return nullZone;
   }

   int getZoneNeighbors(I3HZone zone, I3HZone * neighbors, I3HNeighbor * nbType)
   {
      return zone.getNeighbors(neighbors, nbType);
   }

   I3HZone getZoneCentroidParent(I3HZone zone)
   {
      return zone.centroidParent;
   }

   I3HZone getZoneCentroidChild(I3HZone zone)
   {
      return zone.centroidChild;
   }

   int getZoneParents(I3HZone zone, I3HZone * parents)
   {
      return zone.getParents(parents);
   }

   int getZoneChildren(I3HZone zone, I3HZone * children)
   {
      return zone.getChildren(children);
   }

   // Text ZIRS
   void getZoneTextID(I3HZone zone, String zoneID)
   {
      zone.getZoneID(zoneID);
   }

   I3HZone getZoneFromTextID(const String zoneID)
   {
      return I3HZone::fromZoneID(zoneID);
   }

   // Sub-zone Order
   I3HZone getFirstSubZone(I3HZone zone, int depth)
   {
      return zone.getFirstSubZone(depth);
   }

   void compactZones(Array<DGGRSZone> zones)
   {
      int maxLevel = 0, i, count = zones.count;
      AVLTree<I3HZone> zonesTree { };

      for(i = 0; i < count; i++)
      {
         I3HZone zone = (I3HZone)zones[i];
         if(zone != nullZone)
         {
            int level = zone.level;
            if(level > maxLevel)
               maxLevel = level;
            zonesTree.Add(zone);
         }
      }

      compactI3HZones(zonesTree, maxLevel);
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
      return 33;
   }

   int64 getSubZoneIndex(I3HZone parent, I3HZone subZone)
   {
      int64 ix = -1;
      int level = getZoneLevel(parent), szLevel = getZoneLevel(subZone);

      if(szLevel == level)
         ix = 0;
      else if(szLevel > level && zoneHasSubZone(parent, subZone))
      {
         Pointd zCentroid;

         canonicalize5x6(subZone.centroid, zCentroid);
         ix = iterateI3HSubZones(parent, szLevel - level, &zCentroid, findSubZone, -1);
      }
      return ix;
   }

   DGGRSZone getSubZoneAtIndex(I3HZone parent, int relativeDepth, int64 index)
   {
      I3HZone subZone = nullZone;
      if(index >= 0 && index < countSubZones(parent, relativeDepth))
      {
         if(index == 0)
            return getFirstSubZone(parent, relativeDepth);
         else
         {
            Pointd centroid;
            iterateI3HSubZones(parent, relativeDepth, &centroid, findByIndex, index);
            subZone = I3HZone::fromCentroid(parent.level + relativeDepth, centroid);
         }
      }
      return subZone;
   }

   I3HZone getZoneFromWGS84Centroid(int level, const GeoPoint centroid)
   {
      if(level <= 33)
      {
         Pointd v;
         pj.forward(centroid, v);
         return I3HZone::fromCentroid(level, v);
      }
      return nullZone;
   }

   void getZoneCRSCentroid(I3HZone zone, CRS crs, Pointd centroid)
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

   void getZoneWGS84Centroid(I3HZone zone, GeoPoint centroid)
   {
      pj.inverse(zone.centroid, centroid, zone.subHex > 2);
   }

   void getZoneCRSExtent(I3HZone zone, CRS crs, CRSExtent extent)
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

   void getZoneWGS84Extent(I3HZone zone, GeoExtent extent)
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

   int getZoneCRSVertices(I3HZone zone, CRS crs, Pointd * vertices)
   {
      uint count = zone.getVertices(vertices), i;

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
            bool oddGrid = zone.subHex > 2;
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

   int getZoneWGS84Vertices(I3HZone zone, GeoPoint * vertices)
   {
      Pointd v5x6[6];
      uint count = zone.getVertices(v5x6), i;
      bool oddGrid = zone.subHex > 2;
      for(i = 0; i < count; i++)
         pj.inverse(v5x6[i], vertices[i], oddGrid);
      return count;
   }

   Array<Pointd> getZoneRefinedCRSVertices(I3HZone zone, CRS crs, int edgeRefinement)
   {
      if(crs == CRS { ogc, 1534 })
         return getIcoNetRefinedVertices(zone, edgeRefinement);
      else
         return getRefinedVertices(zone, crs, edgeRefinement, false);
   }

   Array<GeoPoint> getZoneRefinedWGS84Vertices(I3HZone zone, int edgeRefinement)
   {
      return (Array<GeoPoint>)getRefinedVertices(zone, { epsg, 4326 }, edgeRefinement, true);
   }

   void getApproxWGS84Extent(I3HZone zone, GeoExtent extent)
   {
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
   }

   private static Array<Pointd> getRefinedVertices(I3HZone zone, CRS crs, int edgeRefinement, bool useGeoPoint) // 0 edgeRefinement for 1-20 based on level
   {
      Array<Pointd> rVertices = null;
      bool crs84 = crs == CRS { ogc, 84 } || crs == CRS { epsg, 4326 };
      Pointd vertices[9];
      int numPoints = zone.getBaseRefinedVertices(crs84, vertices);
      if(numPoints)
      {
         Array<Pointd> ap;
         bool geodesic = false; //true;
         int level = zone.level;
         bool refine = crs84 || zone.subHex < 3;  // Only use refinement for ISEA for even levels -- REVIEW: When should we refine here?
         int i;

         ap = useGeoPoint ? (Array<Pointd>)Array<GeoPoint> { } : Array<Pointd> { };
         if(crs84)
         {
            //GeoExtent e; // REVIEW: Currently only used to decide whether to wrap
            GeoPoint centroid;
            //Radians dLon;
            bool wrap = true;
            int lonQuad;
            bool oddGrid = zone.subHex > 2;

            //getApproxWGS84Extent(zone, e);
            //dLon = (Radians)e.ur.lon - (Radians)e.ll.lon;

            getZoneWGS84Centroid(zone, centroid);
            // wrap = (dLon < 0 || e.ll.lon > centroid.lon || dLon > Pi || (Radians)centroid.lon + 4*dLon > Pi || (Radians)centroid.lon - 4*dLon < -Pi);
            lonQuad = (int)(((Radians)centroid.lon + Pi) * (4 / (2*Pi)));

            if(geodesic)
            {
               ap.size = numPoints;
               for(i = 0; i < numPoints; i++)
               {
                  GeoPoint point;
                  pj.inverse(vertices[i], point, oddGrid);
                  if(wrap)
                     point.lon = wrapLonAt(lonQuad, point.lon, 0);
                  ap[i] = useGeoPoint ? { (Radians) point.lat, (Radians) point.lon } :
                     crs == { ogc, 84 } ? { point.lon, point.lat } : { point.lat, point.lon };
               }
            }
            else
            {
               int nDivisions = edgeRefinement ? edgeRefinement :
                  level < 3 ? 20 : level < 5 ? 15 : level < 8 ? 10 : level < 10 ? 8 : level < 11 ? 5 : level < 12 ? 2 : 1;
               Array<Pointd> r = refine5x6(numPoints, vertices, /*1024 * */ nDivisions, true); // * 1024 results in level 2 zones areas accurate to 0.01 km^2
               ap./*size*/minAllocSize = r.count;
               for(i = 0; i < r.count; i++)
               {
                  GeoPoint point;
                  // Imprecisions causes some failures... http://localhost:8080/ogcapi/collections/gebco/dggs/ISEA3H/zones/L0-2B3FA-G
                  if(pj.inverse(r[i], point, oddGrid))
                  {
                     if(wrap)
                     {
                        if(centroid.lon < - Pi - 1E-9)
                           centroid.lon += 2*Pi;

                        if(centroid.lon > Pi + 1E-9)
                           centroid.lon -= 2*Pi;

                        point.lon = wrapLonAt(-1, point.lon, centroid.lon - Degrees { 0.05 }) + centroid.lon - Degrees { 0.05 }; // REVIEW: wrapLonAt() doesn't add back centroid.lon ?
                     }
                     ap.Add(useGeoPoint ? { (Radians) point.lat, (Radians) point.lon } :
                        crs == { ogc, 84 } ? { point.lon, point.lat } : { point.lat, point.lon });
                  }
               }
               ap.minAllocSize = 0;
               delete r;
            }
         }
         else if(refine)
         {
            Array<Pointd> r = refine5x6(numPoints, vertices, 1, false);
            ap.size = r.count;
            for(i = 0; i < r.count; i++)
               ap[i] = { r[i].x, r[i].y };
            delete r;
         }
         else
         {
            ap.size = numPoints;
            for(i = 0; i < numPoints; i++)
               ap[i] = { vertices[i].x, vertices[i].y };
         }
         rVertices = ap;
      }
      return rVertices;
   }

   // Sub-zone Order
   Array<Pointd> getSubZoneCRSCentroids(I3HZone parent, CRS crs, int depth)
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
               bool oddGrid = parent.subHex > 2;
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

   Array<GeoPoint> getSubZoneWGS84Centroids(I3HZone parent, int depth)
   {
      Array<GeoPoint> geo = null;
      Array<Pointd> centroids = parent.getSubZoneCentroids(depth);
      if(centroids)
      {
         uint count = centroids.count;
         int i;
         bool oddGrid = parent.subHex > 2;

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
      AVLTree<I3HZone> tsZones { };
      int i9RLevel = zoneLevel / 2;
      uint64 power = POW3(i9RLevel);
      double z = 1.0 / power;
      int hexSubLevel = zoneLevel & 1;
      Pointd tl, br;
      //double x, y;
      bool extentCheck = true;
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
               I3HZone hexes[4];

               hexes[nHexes++] = I3HZone::fromI9R(i9RLevel, row, col, hexSubLevel ? 'D' : 'A');
               if(hexes[nHexes-1] == nullZone)
                  continue; // This should no longer happen...

               if(hexSubLevel)
               {
                  hexes[nHexes++] = I3HZone::fromI9R(i9RLevel, row, col, 'E');
                  hexes[nHexes++] = I3HZone::fromI9R(i9RLevel, row, col, 'F');
               }

               for(h = 0; h < nHexes; h++)
                  tsZones.Add(hexes[h]);
            }
         }
      }

      // Always add the poles since they are touched at multiple points and will be checked for intersections below
      // "North" pole
      tsZones.Add(I3HZone::fromI9R(i9RLevel, 0, (uint)(power-1), hexSubLevel ? 'G' : 'B'));
      // "South" pole
      tsZones.Add(I3HZone::fromI9R(i9RLevel, (uint)(6*power-1), (uint)(4*power), hexSubLevel ? 'H' : 'C'));

      if(tsZones.count)
      {
         zones = { minAllocSize = tsZones.count };
         for(t : tsZones)
         {
            I3HZone zone = t;
            if(extentCheck)
            {
               // REVIEW: Computing the detailed wgs84Extent is slow as it uses refined vertices and involves a large amount of inverse projections.
               //         Are we missing large numbers of hexagons first eliminating those outside the approximate extent?
               GeoExtent e;

               // REVIEW: Should we check 5x6 extent as well or instead of this approximate extent?
               getApproxWGS84Extent(zone, e);
               if(!e.intersects(bbox))
                  continue;

               getZoneWGS84Extent(zone, e);
               if(!e.intersects(bbox))
                  continue;
            }
            zones[zones.count++] = zone;
         }
         zones.Sort(true);
      }
      delete tsZones;
      return zones;
   }
}

/*static*/ uint64 powersOf3[] =
{
   1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683,
   59049, 177147, 531441, 1594323, 4782969, 14348907, 43046721, 129140163, 387420489, 1162261467,
   3486784401LL, 10460353203LL, 31381059609LL, 94143178827LL, 282429536481LL, 847288609443LL, 2541865828329LL, 7625597484987LL,
      22876792454961LL, 68630377364883LL,
   205891132094649LL, 617673396283947LL, 1853020188851841LL, 5559060566555523LL
};


enum I3HNeighbor
{
   // The names reflect the planar ISEA projection arrangement
   top,        // Odd level only, except when replacing topLeft/topRight in interruptions for even level
   bottom,     // Odd level only, except when replacing bottomLeft/bottomRight in interruptions for even level
   left,       // Even level only, except when bottomLeft/topLeft is used instead of bottom/top for even level
   right,      // Even level only
   topLeft,
   topRight,
   bottomLeft,
   bottomRight
};

// Public for use in tests...
public class I3HZone : private DGGRSZone
{
public:
   uint levelI9R:5:58;  // 0 .. 16
   uint rootRhombus:4:54;  // 0 .. 9
   uint64 rhombusIX:51:3; // (left to right, top to bottom)   0 .. 205,891,132,094,648 (3^16 * 3^16 - 1)
   uint subHex:3:0;       // 0..2 for even level: (1 and 2 for special pole cases)
                        // 3..5 for odd level:  (3,4,5 for normal cases, 6 and 7 for special pole cases)

private:
   property int level
   {
      get { return levelI9R * 2 + (subHex > 2); }
   }

   property int nPoints
   {
      get
      {
         int sh = subHex;
         if(sh == 1 || sh == 2 || sh == 6 || sh == 7 || (rhombusIX == 0 && (sh == 0 || sh == 3)))
            return 5; // Polar zones and 0 rhombus index 0 (A and D) are pentagons
         return 6;
      }
   }

   property bool isEdgeHex
   {
      get
      {
         if(nPoints == 6)
         {
            int divs = (int)POW3(levelI9R);
            bool southRhombus = rootRhombus & 1;
            return rhombusIX && (southRhombus ? ((rhombusIX % divs) == 0) : ((rhombusIX / divs) == 0));
         }
         return false;
      }
   }

   I3HZone ::fromZoneID(const String zoneID)
   {
      I3HZone result = nullZone;
      char levelChar;
      uint root;
      uint64 ix;
      char subHex;
      int row, col, l9r = -1;

      if(sscanf(zoneID, __runtimePlatform == win32 ? "%c%d-%I64X-%c" : "%c%d-%llX-%c",
         &levelChar, &root, &ix, &subHex) == 4)
         l9r = iLRCFromLRtI(levelChar, root, ix, &row, &col);
      if(l9r != -1 && validate(l9r, row, col, subHex))
      {
         char id[256];

         result = fromI9R(l9r, row, col, subHex);

         result.getZoneID(id);
         if(strcmp(id, zoneID))
            result = nullZone;
      }
      return result;
   }

   // This function generates the proposed ISEA3H DGGS Zone ID string
   // in the form {ISEA9RLevelChar}{RootRhombus}-{HexIndexWithinRootRhombus}-{SubHex}
   void getZoneID(String zoneID)
   {
      if(this == nullZone)
         sprintf(zoneID, "(null)");
      else
      {
         uint l9r = this.levelI9R;
         uint root = this.rootRhombus;
         uint64 ix = this.rhombusIX;
         char subHex = (char)(this.subHex + 'A');
         sprintf(zoneID,
            __runtimePlatform == win32 ? "%c%d-%I64X-%c" : "%c%d-%llX-%c",
            'A' + l9r, root, ix, subHex);
      }
   }

   I3HZone ::fromI9R(int level, uint row, uint col, char subHex)
   {
      uint64 p = POW3(level);
      uint rowOP = (uint)(row / p), colOP = (uint)(col / p);
      int root = rowOP + colOP;
      int y = (int)(row - rowOP * p), x = (int)(col - colOP * p);
      uint64 ix = y * p + x;

      // Avoid returning bad key
      if(subHex < 'A' || subHex > 'H' || root > 9 || rowOP < colOP || rowOP - colOP > 1 || y >= p || x >= p)
         return nullZone;
      else if(subHex != 'A' && subHex != 'D' && subHex != 'E' && subHex != 'F' &&
              !((subHex == 'B' || subHex == 'G') && row == 0 && col == p-1) &&
              !((subHex == 'C' || subHex == 'H') && col == 4*p && row == 6*p-1))
         return nullZone;
      return { level, root, ix, subHex - 'A'};
   }

   bool ::validate(uint levelI9R, uint row, uint col, char subHex)
   {
      uint64 p = POW3(levelI9R);
      uint rowOP = (uint)(row / p), colOP = (uint)(col / p);
      int y = (int)(row - rowOP * p), x = (int)(col - colOP * p);
      uint root = rowOP + colOP;
      if(subHex < 'A' || subHex > 'H' || root > 9 || rowOP < colOP || rowOP - colOP > 1 || y >= p || x >= p)
         return false;
      else if(subHex != 'A' && subHex != 'D' && subHex != 'E' && subHex != 'F' &&
              !((subHex == 'B' || subHex == 'G') && row == 0 && col == p-1) &&
              !((subHex == 'C' || subHex == 'H') && col == 4*p && row == 6*p-1))
         return false;
      return true;
   }

   property I3HZone parent0
   {
      get
      {
         int sh = this.subHex;
         if(!levelI9R && sh < 3)
            return nullZone;
         else
         {
            I3HZone key { };
            if(sh >= 3)
            {
               key = this;
               key.subHex = sh == 6 ? 1 : sh == 7 ? 2 : 0;
            }
            else
            {
               int row, col, level = iLRCFromLRtI((char)('A' + levelI9R), rootRhombus, rhombusIX, &row, &col);
               uint64 p = POW3(level);
               uint64 r = rhombusIX / p, c = rhombusIX % p;
               uint rm3 = (uint)(r % 3), cm3 = (uint)(c % 3);
               key = fromI9R(level - 1, row / 3, col / 3, (char)('A' + (sh == 1 ? 6 : sh == 2 ? 7 : (cm3 > 1 ? 4 : rm3 > 1 ? 5 : 3))));
            }
            return key;
         }
      }
   }

   I3HZone getNeighbor(I3HNeighbor which)
   {
      Pointd centroid = this.centroid;
      int cx = (int)floor(centroid.x + 1E-11);
      int cy = (int)floor(centroid.y + 1E-11);
      bool south = centroid.y - centroid.x - 1E-11 > 1; // Not counting pentagons as south or north
      bool north = centroid.x - centroid.y - 1E-11 > 0;
      bool northPole = north && fabs(centroid.x - centroid.y - 1.0) < 1E-11;
      bool southPole = south && fabs(centroid.y - centroid.x - 2.0) < 1E-11;
      uint l9r = levelI9R;
      uint64 p = POW3(l9r);
      double d = 1.0 / p, x = 0, y = 0;
      int sh = subHex;
      Pointd v;
      bool crossEarly = true;

      if(sh < 3)
      {
         // Even level

         // NOTE: See getNeighbors() for special interruption cases
         switch(which)
         {
            case top:
               if(south && centroid.x - cx < 1E-11)
               {
                  crossEarly = false;
                  if(southPole)
                     x = -3, y = -3-d;
                  else // Extra top neighbor at south interruptions
                     y = -d;
               }
               break;
            case bottom:
               if(north && centroid.y - cy < 1E-11)
               {
                  crossEarly = false;
                  if(northPole)
                     x = 2-d, y = 2;
                  else // Extra bottom neighbor at north interruptions
                     x = -d;
               }
               break;
            case left:        x = -d, y = -d; break;
            case right:       x =  d, y =  d; break;
            case topLeft:
               if(northPole)
                  crossEarly = false, x = 3-d, y = 3;
               else if(southPole)
                  crossEarly = false, y = -d;
               else
                  y = -d;
               break;
            case bottomLeft:
               if(southPole)
                  crossEarly = false, x = -2, y = -2-d;
               else
                  x = -d;
               break;
            case topRight:
               if(northPole)
                  crossEarly = false, x = 4-d, y = 4;
               else if(southPole)
                  crossEarly = false, x = -4, y = -d - 4;
               else
                  x = d;
               break;
            case bottomRight:
               if(southPole)
                  crossEarly = false, x = -1, y = -1-d;
               else
                  y =  d;
               break;
         }
      }
      else
      {
         // Odd level
         double do3 = d/3;

         // NOTE: See getNeighbors() for special interruption cases
         switch(which)
         {
            case top:
               if(southPole)
                  x =   do3 - 5, y = -do3 - 5, crossEarly = false;
               else if(!northPole)
                  x =   do3, y = -do3;
               break;
            case bottom:
               if(northPole)
                  x = 1-do3, y = 1+do3, crossEarly = false;
               else if(!southPole)
                  x =  -do3, y =  do3;
               break;
            case topLeft:
               if(northPole)
                  x = 2-do3, y = 2+do3, crossEarly = false;
               else if(southPole)
                  x =   do3, y = -do3;
               else
                  x = -do3, y =-2*do3;
               break;
            case bottomLeft:
               if(northPole)
                  x = 4-do3, y = 4+do3, crossEarly = false;
               else if(southPole)
                  x = do3 - 2, y = -do3 - 2;
               else
                  x = -2*do3, y = -do3;
               break;
            case topRight:
               if(northPole)
                  x = 3-do3, y = 3+do3, crossEarly = false;
               else if(southPole)
                  x = do3 - 4, y = -do3 - 4;
               else
                  x =  2*do3, y = do3;
               break;
            case bottomRight:
               if(northPole)
                  x = 5-do3, y = 5+do3, crossEarly = false;
               else if(southPole)
                  x = do3 - 1, y = -do3 - 1;
               else
                  x = do3, y = 2*do3;
               break;
            case right: // Currently stand-in for second bottom / top neighbor
               // Extra bottom neighbor at north interruptions
               if(north && !northPole && centroid.y - cy < 1E-11)
                  crossEarly = false, y = do3, x = -do3;
               // Extra bottom neighbor at south interruptions
               else if(south && !southPole && centroid.x - cx < 1E-11)
                  crossEarly = false, x = do3, y = -do3;
               break;
         }
      }
      if(x || y)
      {
         I3HZone result;
         // REVIEW: This is the only place we use moveISEAVertex2()
         move5x6Vertex2(v, centroid, x, y, crossEarly);
         result = fromCentroid(2*l9r + (sh >= 3), v);
         if(result == this)
            return nullZone; // This should not happen
         return result;
      }
      else
         return nullZone;
      /*
      TODO: more direct path?
      int row, col, level = iLRCFromLRtI((char)('A' + levelI9R), rootRhombus, rhombusIX, &row, &col);
      int sh = 0;
      col += x, row += y;
      return fromI9R(level, row, col, (char)('A' + sh));
      */
   }

   int getNeighbors(I3HZone neighbors[6], I3HNeighbor i3hNB[6])
   {
      int numNeighbors = 0;
      I3HNeighbor n;
      I3HNeighbor localNB[6];

      if(i3hNB == null) i3hNB = localNB;

      for(n = 0; n < I3HNeighbor::enumSize; n++)
      {
         I3HZone nb = getNeighbor(n);
         if(nb != nullZone)
         {
            I3HNeighbor which = n;
            if(numNeighbors)
            {
               // Handle special cases here so that getNeighbor()
               // can still return same neighbor for multiple directions
               if(n == topRight && i3hNB[numNeighbors-1] == topLeft && neighbors[numNeighbors-1] == nb)
               {
                  i3hNB[numNeighbors-1] = top;
                  continue;
               }
               else if(n == bottomRight && i3hNB[numNeighbors-1] == bottomLeft && neighbors[numNeighbors-1] == nb)
               {
                  i3hNB[numNeighbors-1] = bottom;
                  continue;
               }
               else if(n == topRight && i3hNB[numNeighbors-1] != topLeft)
                  which = top;
               else if(n == bottomRight && i3hNB[numNeighbors-1] != bottomLeft)
                  which = bottom;
            }
            i3hNB[numNeighbors] = which;
            neighbors[numNeighbors++] = nb;
         }
      }
      return numNeighbors;
   }

   property I3HZone centroidParent
   {
      get
      {
         I3HZone cParent = parent0;
         if(cParent != nullZone && cParent.isCentroidChild)
            return cParent; // At least one of vertex children's parent is a centroid child,
                            // but few centroid child parent are themselves a centroid child
         else
         {
            // TODO: directly compute centroid parent?
            I3HZone parents[3];
            int n = getParents(parents), i;

            for(i = 1; i < n; i++)
               if(parents[i].isCentroidChild)
                  return parents[i];
            return nullZone;
         }
      }
   }

   int getContainingGrandParents(I3HZone cgParents[3])
   {
      I3HZone cParent = centroidParent;
      int n;

      if(isCentroidChild)
      {
         if(cParent != nullZone)
            cgParents[0] = cParent.parent0, n = 1;
         else
            n = parent0.getParents(cgParents);
      }
      else
         n = cParent.getParents(cgParents);

#ifdef _DEBUG
      if(n != 3 && n != 1)
         Print("WARNING: Wrong assumption");
#endif
      return n;
   }

   int getParents(I3HZone parents[3])
   {
      I3HZone parent0 = this.parent0;

      parents[0] = parent0;
      if(isCentroidChild)
         return parent0 == nullZone ? 0 : 1;
      else
      {
         int sh = subHex;

         if(sh >= 3)
         {
            // Odd level
            parents[1] = parent0.getNeighbor(right);
            parents[2] = parent0.getNeighbor(sh == 4 /* E */ ? topRight : bottomRight /* F */);
         }
         else
         {
            // Even level
            Pointd centroid = this.centroid;
            Pointd p0Centroid = parent0.centroid;
            double dx = centroid.x - p0Centroid.x;
            double dy = centroid.y - p0Centroid.y;
            int p0cx = (int)floor(p0Centroid.x + 1E-11);
            int p0cy = (int)floor(p0Centroid.y + 1E-11);
            bool onBottomCrossingLeft = p0Centroid.y - p0Centroid.x + 1E-11 > 1 && p0Centroid.x - p0cx < 1E-11;
            bool onTopCrossingRight = p0Centroid.x - p0Centroid.y + 1E-11 > 0 && p0Centroid.y - p0cy < 1E-11;

            if(fabs(dx) < 1E-11)
            {
               if(dy > 0)
               {
                  // Bottom-Right vertex child of p0
                  parents[1] = parent0.getNeighbor(bottomRight);
                  parents[2] = parent0.getNeighbor(onBottomCrossingLeft ? bottomLeft : bottom);
               }
               else
               {
                  // Top-Left vertex child of p0
                  parents[1] = parent0.getNeighbor(topLeft);
                  parents[2] = parent0.getNeighbor(top);
               }
            }
            else if(fabs(dy) < 1E-11)
            {
               if(dx > 0)
               {
                  // Top-Right vertex child of p0
                  parents[1] = parent0.getNeighbor(topRight);
                  parents[2] = parent0.getNeighbor(onTopCrossingRight ? topLeft : top);
               }
               else
               {
                  // Bottom-Left vertex child of p0
                  parents[1] = parent0.getNeighbor(bottomLeft);
                  parents[2] = parent0.getNeighbor(bottom);
               }
            }
            else
            {
               if(dx > 0)
               {
                  // Right vertex child of p0
                  parents[1] = parent0.getNeighbor(topRight);
                  parents[2] = parent0.getNeighbor(bottomRight);
               }
               else
               {
                  // Left vertex child of p0
                  parents[1] = parent0.getNeighbor(topLeft);
                  parents[2] = parent0.getNeighbor(bottomLeft);
               }
            }
         }
         return 3;
      }
   }

   I3HZone ::fromCentroid(uint level, const Pointd centroid) // in RI5x6
   {
      Pointd c = centroid;
      uint l9r = level / 2;
      uint64 p = POW3(l9r);
      double d =  1.0 / p;
      bool isNorthPole = false, isSouthPole = false;
      if(fabs(c.x - c.y - 1) < 1E-10)
         isNorthPole = true;
      else if(fabs(c.y - c.x - 2) < 1E-10)
         isSouthPole = true;
      else if(c.y < -1E-11 && c.x > -1E-11)
         c.x -= c.y, c.y = 0;
      /*
      else if(c.y > 5.0 + 1E-11 && c.x < 4.0 + 1E-11) // B7-7-A -> B9-3-A, B9-6-A, but what about B5-3-A ?
         c.x += (c.y - 5.0), c.y = 5.0; // REVIEW: This may no longer be necessary?
      */
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
         move5x6Vertex(c, { 5, 5 }, c.x, c.y);
      if(c.x > 5 - 1E-11 && c.y > 5 - 1E-11 &&  // This handles bottom right wrap e.g., A9-0E and A9-0-F
         c.x + c.y > 5.0 + 5.0 - d - 1E-11)
         c.x -= 5, c.y -= 5;
      {
         int cx = Min(4, (int)(c.x + 1E-11)), cy = Min(5, (int)(c.y + 1E-11));  // Coordinate of root rhombus
         uint root = cx + cy;
         uint64 x = (uint64)((c.x - cx) * p + 1E-6 /*11*/); // Row and Column within root rhombus
         uint64 y = (uint64)((c.y - cy) * p + 1E-6 /*11*/);
         uint64 rix = y * p + x;  // Index within root rhombus
         double xd = (c.x - cx) * p - x;
         double yd = (c.y - cy) * p - y;
         uint sh;

         if(isNorthPole)
            sh = (level & 1) ? 6 : 1, root = 0, rix = p-1;
         else if(isSouthPole)
            sh = (level & 1) ? 7 : 2, root = 9, rix = p * (p-1);
         else
         {
            bool rightSR = x == p-1, topSR = y == 0;
            bool leftSR = x == 0, bottomSR = y == p-1;
            bool npSubRhombus = rightSR  && topSR  && !(root & 1);
            bool spSubRhombus = bottomSR && leftSR &&  (root & 1);

            if(cy < cx || xd < -1 || yd < -1 || x >= p || y >= p || rix >= p*p)
               return nullZone; // y cannot be smaller than x

            // PrintLn("   rix: ", rix, ", x: ", x, ", y: ", y, ", xd: ", xd, ", yd: ", yd);
            if(level & 1) // Odd level
            {
               bool leftThird = 3*xd < 1, topThird = 3*yd < 1;

               if(leftThird && topThird)
                  sh = 3; // D
               else
               {
                  bool rightThird = 3*xd > 2, bottomThird = 3*yd > 2;
                  if(rightThird && bottomThird)
                  {
                     if(bottomSR && rightSR)
                     {
                        // Non-polar pentagon
                        root = (root + 2) % 10;
                        rix = 0;
                     }
                     else if(bottomSR)
                     {
                        // Indexed to another root rhombus
                        if(root & 1)
                        {
                           // Crossing South interruption to the right
                           root = (root + 2) % 10;
                           rix = (p-1-x) * p;
                        }
                        else
                        {
                           root++;
                           rix = x + 1;
                        }
                     }
                     else if(rightSR)
                     {
                        // Indexed to another root rhombus
                        if(!(root & 1))
                        {
                           // Crossing North interruption to the right
                           root = (root + 2) % 10;
                           rix = p-1-y;
                        }
                        else
                        {
                           root = (root + 1) % 10;
                           rix = p * (y + 1);
                        }
                     }
                     else
                        rix += p + 1;
                     sh = 3; // D
                  }
                  else if(bottomThird)
                  {
                     if(3 * (yd - xd) > 2)
                     {
                        if(spSubRhombus)
                           root = 9, sh = 7; // "South" pole H
                        else
                        {
                           if(bottomSR)
                           {
                              // Indexed to another root rhombus
                              if(root & 1)
                              {
                                 // Crossing South interruption to the right
                                 root = (root + 2) % 10;
                                 rix = (p-x) * p;
                              }
                              else
                              {
                                 rix = x;
                                 root++;
                              }
                           }
                           else
                              rix += p;
                           sh = 3; // D
                        }
                     }
                     else
                        sh = 5; // F
                  }
                  else if(rightThird)
                  {
                     if(3 * (xd - yd) > 2)
                     {
                        if(npSubRhombus)
                           root = 0, sh = 6; // "North" pole G
                        else
                        {
                           if(rightSR)
                           {
                              if(!(root & 1))
                              {
                                 // Crossing North interruption to the right
                                 root = (root + 2) % 10;
                                 rix = p-y;
                              }
                              else
                              {
                                 root = (root + 1) % 10;
                                 rix = p * y;
                              }
                           }
                           else
                              rix++;
                           sh = 3; // D
                        }
                     }
                     else
                        sh = 4; // E
                  }
                  else if(xd > yd)
                     sh = 4; // E
                  else
                     sh = 5; // F
               }
            }
            else          // Even level
            {
               bool topRight = false, bottomLeft = false, bottomRight = false;
               if(xd - 1 > -yd) // Bottom-Right portion
               {
                  // Top-right hexagon
                  if(xd > yd * 2)
                     topRight = true;
                  // Bottom-left hexagon
                  else if(2 * xd < yd)
                     bottomLeft = true;
                  // Bottom-right hexagon
                  else
                     bottomRight = true;
               }
               else // Top-Left portion
               {
                  // Top-right hexagon
                  if(2 * xd > yd + 1)
                     topRight = true;
                  // Bottom-left hexagon
                  else if(xd + 1 < 2 * yd)
                     bottomLeft = true;
               }

               sh = 0; // A
               if(topRight)
               {
                  if(npSubRhombus)
                     root = 0, sh = 1; // "North" pole B
                  else
                  {
                     if(rightSR)
                     {
                        if(!(root & 1))
                        {
                           // Crossing North interruption to the right
                           root = (root + 2) % 10;
                           rix = p-y;
                        }
                        else
                        {
                           root = (root + 1) % 10;
                           rix = p * y;
                        }
                     }
                     else
                        rix++;
                  }
               }
               else if(bottomLeft)
               {
                  if(spSubRhombus)
                     root = 9, sh = 2; // "South" pole C
                  else
                  {
                     if(bottomSR)
                     {
                        // Indexed to another root rhombus
                        if(root & 1)
                        {
                           // Crossing South interruption to the right
                           root = (root + 2) % 10;
                           rix = (p-x) * p;
                        }
                        else
                        {
                           rix = x;
                           root++;
                        }
                     }
                     else
                        rix += p;
                  }
               }
               else if(bottomRight)
               {
                  if(bottomSR && rightSR)
                  {
                     // Non-polar pentagon
                     root = (root + 2) % 10;
                     rix = 0;
                  }
                  else if(bottomSR)
                  {
                     // Indexed to another root rhombus
                     if(root & 1)
                     {
                        // Crossing South interruption to the right
                        root = (root + 2) % 10;
                        rix = (p-1-x) * p;
                     }
                     else
                     {
                        root++;
                        rix = x + 1;
                     }
                  }
                  else if(rightSR)
                  {
                     // Indexed to another root rhombus
                     if(!(root & 1))
                     {
                        // Crossing North interruption to the right
                        root = (root + 2) % 10;
                        rix = p-1-y;
                     }
                     else
                     {
                        root = (root + 1) % 10;
                        rix = p * (y + 1);
                     }
                  }
                  else
                     rix += p + 1;
               }
            }
         }
         return { l9r, root, rix, sh };
      }
   }

   int getVertices(Pointd * vertices)
   {
      int sh = subHex;
      uint level = levelI9R, root = rootRhombus;
      uint64 rix = rhombusIX;
      uint64 p = POW3(level);
      uint64 rowOP = (root + 1) >> 1, colOP = root >> 1;
      uint64 ixOP = (uint64)(rix / p);
      uint64 row = (uint64)(rowOP * p + ixOP);
      uint64 col = (uint64)((colOP - ixOP) * p + rix); // distributivity on: rix - (ixOP * p) for (rix % p)
      double d =  1.0 / p;
      Pointd tl { col * d, row * d };
      int i = 0;
      bool south = root & 1;
      Pointd v;

      switch(sh)
      {
         case 0:  // Even level -- regular
            move5x6Vertex(vertices[i++], tl,  2*d/3,    d/3);
            move5x6Vertex(vertices[i++], tl,    d/3,  2*d/3);
            if(!south || rix) // 0 rhombusIndex are pentagons
               move5x6Vertex(vertices[i++], tl, -  d/3,    d/3);
            move5x6Vertex(vertices[i++], tl, -2*d/3, -  d/3);
            if(south || rix) // 0 rhombusIndex are pentagons
               move5x6Vertex(vertices[i++], tl, -  d/3, -2*d/3);
            move5x6Vertex(vertices[i++], tl,    d/3, -  d/3);
            break;
         case 1:  // Even level -- "North" pole
            if(sh == 1 && row == 0 && col == p-1)
            {
               move5x6Vertex(v, tl, d/3, -d/3);
               vertices[i++] = { v.x + 1, v.y + 1 };
               vertices[i++] = { v.x + 2, v.y + 2 };
               vertices[i++] = { v.x + 3, v.y + 3 };
               vertices[i++] = { v.x + 4, v.y + 4 };
               vertices[i++] = { v.x + 5, v.y + 5 };
            }
            break;
         case 2: // Even level -- "South" pole
            if(col == 4*p && row == 6*p-1)
            {
               move5x6Vertex(v, tl, -d/3, d/3);
               vertices[i++] = { v.x - 0, v.y - 0 };
               vertices[i++] = { v.x - 1, v.y - 1 };
               vertices[i++] = { v.x - 2, v.y - 2 };
               vertices[i++] = { v.x - 3, v.y - 3 };
               vertices[i++] = { v.x + 1, v.y + 1 };
            }
            break;
         case 3:  // Odd level -- type D
            if(south || rix) // 0 rhombusIndex are pentagons
               move5x6Vertex(vertices[i++], tl, d/3,0);
            move5x6Vertex(vertices[i++], tl, d/3, d/3);
            move5x6Vertex(vertices[i++], tl,0, d/3);
            if(!south || rix) // 0 rhombusIndex are pentagons
               move5x6Vertex(vertices[i++], tl,-d/3,    0);
            move5x6Vertex(vertices[i++], tl,-d/3,-d/3);
            move5x6Vertex(vertices[i++], tl,    0,-d/3);
            break;
         case 4:  // Odd level -- type E
            move5x6Vertex(vertices[i++], tl,  d/3,0);
            move5x6Vertex(vertices[i++], tl,2*d/3,0);
            move5x6Vertex(vertices[i++], tl,    d,  d/3);
            move5x6Vertex(vertices[i++], tl,    d,2*d/3);
            move5x6Vertex(vertices[i++], tl,2*d/3,2*d/3);
            move5x6Vertex(vertices[i++], tl,  d/3,  d/3);
            break;
         case 5:  // Odd level -- type F
            move5x6Vertex(vertices[i++], tl,0,   d/3);
            move5x6Vertex(vertices[i++], tl, d/3,   d/3);
            move5x6Vertex(vertices[i++], tl,2*d/3,2*d/3);
            move5x6Vertex(vertices[i++], tl,2*d/3,    d);
            move5x6Vertex(vertices[i++], tl,  d/3,    d);
            move5x6Vertex(vertices[i++], tl,0,2*d/3);
            break;
         case 6:  // Odd level -- "North" pole
            if(row == 0 && col == p-1)
            {
               move5x6Vertex(v, tl, 2*d/3, 0);
               // vertices[i++] = { v.x + 1 - d/3, v.y + 1 - d/3 };
               vertices[i++] = { v.x + 1, v.y + 1 };
               vertices[i++] = { v.x + 2, v.y + 2 };
               vertices[i++] = { v.x + 3, v.y + 3 };
               vertices[i++] = { v.x + 4, v.y + 4 };
               vertices[i++] = { v.x + 5, v.y + 5 };
            }
            break;
         case 7:  // Odd level -- "South" pole
            if(col == 4*p && row == 6*p-1)
            {
               move5x6Vertex(v, tl, d/3, d);
               vertices[i++] = { v.x - 0, v.y - 0 };
               vertices[i++] = { v.x - 1, v.y - 1 };
               vertices[i++] = { v.x - 2, v.y - 2 };
               vertices[i++] = { v.x - 3, v.y - 3 };
               vertices[i++] = { v.x - 4, v.y - 4 };
               // vertices[i++] = { v.x - 4 - d/3, v.y - 4 - d/3 };
            }
            break;
      }
      return i;
   }

   int getBaseRefinedVertices(bool crs84, Pointd * vertices)
   {
      int numPoints = 0;
      int row, col, level = iLRCFromLRtI((char)('A' + levelI9R), rootRhombus, rhombusIX, &row, &col);
      int subHex = this.subHex;
      bool result = true;
      uint64 p = POW3(level);
      double d =  1.0 / p;
      Pointd v;
      Pointd tl = I9RZone { level, row, col }.ri5x6Extent.tl;

      //static const double sqrt3_2 = 0.8660254037844;  // a = √3/2 × s  (0.5 / tan(30°))
      // double a = d, s = a / sqrt3_2;

      switch(subHex)
      {
         case 0:  // Even level -- regular A
            move5x6Vertex(vertices[numPoints++], tl,  2*d/3,    d/3);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in move5x6Vertex()?
            move5x6Vertex(vertices[numPoints++], tl,    d/3,  2*d/3);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in move5x6Vertex()?
            move5x6Vertex(vertices[numPoints++], tl, -  d/3,    d/3);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in move5x6Vertex()?
            move5x6Vertex(vertices[numPoints++], tl, -2*d/3, -  d/3);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in move5x6Vertex()?
            move5x6Vertex(vertices[numPoints++], tl, -  d/3, -2*d/3);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in move5x6Vertex()?
            move5x6Vertex(vertices[numPoints++], tl,    d/3, -  d/3);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in move5x6Vertex()?
            break;
         case 1:  // Even level -- "North" pole B
            if(row == 0 && col == p-1)
            {
               move5x6Vertex(v, tl, d/3, -d/3);

               // These are the pentagon's 5 vertices
               vertices[numPoints++] = { v.x + 5, v.y + 5 };

               if(!crs84)
               {
                  // Trapezoidal cap
                  vertices[numPoints++] = { 5, v.y + 5 + 0.5 * d/3 };
                  vertices[numPoints++] = { 5, 4 }; // This is the "north" pole
                  vertices[numPoints++] = { 1, 0 }; // This is also the "north" pole
                  vertices[numPoints++] = { v.x + 1 - 0.5*d/3, 0 };

                  /*
                  // Rectangular cap
                  vertices[numPoints++] = { 5, 4 }; // "North" pole
                  vertices[numPoints++] = { 0, -1 }; // Also "North" pole
                  vertices[numPoints++] = { v.x, v.y }; // Extra vertex to fill polygon
                  */
               }
               vertices[numPoints++] = { v.x + 1, v.y + 1 };
               vertices[numPoints++] = { v.x + 2, v.y + 2 };
               vertices[numPoints++] = { v.x + 3, v.y + 3 };
               vertices[numPoints++] = { v.x + 4, v.y + 4 };
            }
            else
               result = false;
            break;
         case 2:  // Even level -- "South" pole C
            if(col == 4*p && row == 6*p-1)
            {
               move5x6Vertex(v, tl, -d/3, d/3);
               vertices[numPoints++] = { v.x - 0, v.y - 0 };
               vertices[numPoints++] = { v.x - 1, v.y - 1 };
               vertices[numPoints++] = { v.x - 2, v.y - 2 };
               vertices[numPoints++] = { v.x - 3, v.y - 3 };
               if(!crs84)
               {
                  // Trapezoidal cap
                  vertices[numPoints++] = { 0, v.y - 3 - 0.5 * d/3 };
                  vertices[numPoints++] = { 0, 2 }; // "South" pole
                  vertices[numPoints++] = { 4, 6 }; // Also "South" pole
                  vertices[numPoints++] = { v.x + 1 + 0.5 *d/3, 6 };

                  // Rectangular cap Extra vertices to fill polygon in ISEA CRSs
                  /*
                  vertices[numPoints++] = { v.x - 4, v.y - 4 };
                  vertices[numPoints++] = { -1, 1 }; // "South" pole
                  vertices[numPoints++] = { 4, 6 }; // Also "South" pole
                  */
               }
               vertices[numPoints++] = { v.x + 1,  v.y + 1 };
            }
            else
               result = false;
            break;
         case 3:  // Odd level -- type D
            if(crs84)
            {
               move5x6Vertex(vertices[numPoints++], tl, d/3,0);
               if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
                  vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in move5x6Vertex()?
            }
            else
            {
               move5x6Vertex(vertices[numPoints++], tl, d/3,-2E-11);
               move5x6Vertex(vertices[numPoints++], tl, d/3,2E-11);
            }
            move5x6Vertex(vertices[numPoints++], tl, d/3, d/3);
            if(crs84)
            {
               move5x6Vertex(vertices[numPoints++], tl, 0, d/3);
               if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
                  vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in move5x6Vertex()?
            }
            else
            {
               move5x6Vertex(vertices[numPoints++], tl,2E-11, d/3);
               move5x6Vertex(vertices[numPoints++], tl,-2E-11, d/3);
            }
            move5x6Vertex(vertices[numPoints++], tl,-d/3,    0);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in move5x6Vertex()?
            move5x6Vertex(vertices[numPoints++], tl,-d/3,-d/3);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in move5x6Vertex()?
            move5x6Vertex(vertices[numPoints++], tl,    0,-d/3);
            if(crs84 && (vertices[numPoints-1].y < 0 || vertices[numPoints-1].x < 0))
               vertices[numPoints-1].x += 5, vertices[numPoints-1].y += 5; // REVIEW: Can we always do this in move5x6Vertex()?
            break;
         case 4:  // Odd level -- type E
            move5x6Vertex(vertices[numPoints++], tl,  d/3,2E-11);
            move5x6Vertex(vertices[numPoints++], tl,2*d/3,2E-11);
            move5x6Vertex(vertices[numPoints++], tl,    d,  d/3);
            move5x6Vertex(vertices[numPoints++], tl,    d,2*d/3);
            move5x6Vertex(vertices[numPoints++], tl,2*d/3,2*d/3);
            move5x6Vertex(vertices[numPoints++], tl,  d/3,  d/3);
            break;
         case 5:  // Odd level -- type F
            move5x6Vertex(vertices[numPoints++], tl,2E-11,   d/3);
            move5x6Vertex(vertices[numPoints++], tl, d/3,   d/3);
            move5x6Vertex(vertices[numPoints++], tl,2*d/3,2*d/3);
            move5x6Vertex(vertices[numPoints++], tl,2*d/3,    d);
            move5x6Vertex(vertices[numPoints++], tl,  d/3,    d);
            move5x6Vertex(vertices[numPoints++], tl,2E-11,2*d/3);
            break;
         case 6:  // Odd level -- "North" pole G
            if(row == 0 && col == p-1)
            {
               move5x6Vertex(v, tl, 2*d/3, 0);
               // These are the pentagon's 5 vertices
               // vertices[numPoints++] = { v.x + 1 - d/3, v.y + 1 - d/3 }; -- For version before fix that crossed the interruption
               vertices[numPoints++] = { v.x + 0, v.y + 0 };
               vertices[numPoints++] = { v.x + 1, v.y + 1 };
               vertices[numPoints++] = { v.x + 2, v.y + 2 };
               vertices[numPoints++] = { v.x + 3, v.y + 3 };
               vertices[numPoints++] = { v.x + 4, v.y + 4 };
               if(!crs84)
               {
                  // Extra vertices to fill polygon in ISEA CRSs
                  vertices[numPoints++] = { 5, 4 + d/3 }; // This extends to right border of last triangle
                  vertices[numPoints++] = { 5, 4 }; // This is the "north" pole
                  vertices[numPoints++] = { 1, 0 }; // This is also the "north" pole
               }
            }
            else
               result = false;
            break;
         case 7:
            if(col == 4*p && row == 6*p-1)
            {
               // Odd level -- "South" pole H
               move5x6Vertex(v, tl, d/3, d);
               // These are the pentagon's 5 vertices
               vertices[numPoints++] = { v.x - 0, v.y - 0 };
               vertices[numPoints++] = { v.x - 1, v.y - 1 };
               vertices[numPoints++] = { v.x - 2, v.y - 2 };
               vertices[numPoints++] = { v.x - 3, v.y - 3 };
               vertices[numPoints++] = { v.x - 4, v.y - 4 };
               if(!crs84)
               {
                  // Extra vertices to fill polygon in ISEA CRSs
                  vertices[numPoints++] = { 0, 2 - d/3 }; // This extends to the left wrapping point
                  vertices[numPoints++] = { 0, 2 }; // This is the "south" pole
                  vertices[numPoints++] = { 4, 6 }; // This is also the "south" pole
               }
            }
            else
               result = false;
            break;
         default:
            result = false;
      }
      return result ? numPoints : 0;
   }

   property I3HZone centroidChild
   {
      get
      {
         if(this == nullZone)
            return nullZone;
         else
         {
            I3HZone centroidChild;
            uint l9r = levelI9R, sh = subHex;
            uint root = rootRhombus;
            uint64 rix = rhombusIX;

            if(sh == 0) // Centroid child for Even level
               centroidChild = { l9r, root, rix, 3 };
            else
            {
               uint64 p = POW3(l9r);
               switch(sh)
               {
                  case 1: centroidChild = { l9r, 0, p-1, 6 }; break; // Even level "North" Pole
                  case 2: centroidChild = { l9r, 9, p*(p-1), 7 }; break; // Even level "South" Pole
                  case 6: centroidChild = { l9r+1, 0, 3*p-1, 1 }; break; // Odd level "North" Pole
                  case 7: centroidChild = { l9r+1, 9, 3*p*(3*p-1), 2 }; break; // Odd level "South" Pole
                  default:
                  {
                     // Centroid child for Odd level
                     int rowOP = (root + 1) >> 1, colOP = root >> 1, ixOP = (int)(rix / p);
                     int row = (int)(rowOP * p + ixOP), col = (int)((colOP - ixOP) * p + rix); // distributivity on: ix - (ixOP * p) for (ix % p)
                     int r = row * 3 + ((sh == 5) ? 2 : (sh == 4) ? 1 : 0);
                     int c = col * 3 + ((sh == 4) ? 2 : (sh == 5) ? 1 : 0);
                     centroidChild = I3HZone::fromI9R(l9r + 1, r, c, 'A');
                     break;
                  }
               }
            }
            return centroidChild;
         }
      }
   }

   int getChildren(I3HZone children[7])
   {
      uint l9r = levelI9R, sh = subHex;
      int i = 0;

      children[i++] = centroidChild;

      if(sh == 1 || sh == 2 || sh == 6 || sh == 7)
      {
         // Special cases for the poles
         uint64 p = POW3(l9r);
         switch(sh)
         {
            case 1: // Even level "North" Pole
               for(; i < 6; i++)
                  children[i] = { l9r, (i-1)*2, p-1, 4 };
               break;
            case 2: // Even level "South" Pole
               for(; i < 6; i++)
                  children[i] = { l9r, (i-1)*2 + 1, p*(p-1), 5 };
               break;
            case 6: // Odd level "North" Pole
               for(; i < 6; i++)
                  children[i] = { l9r+1, (i-1)*2, 3*p-1, 0 };
               break;
            case 7: // Odd level "South" Pole
               for(; i < 6; i++)
                  children[i] = { l9r+1, (i-1)*2+1, 3*p*(3*p-1), 0 };
               break;
         }
      }
      else
      {
         Pointd vertices[6];
         int nVertices = getVertices(vertices);
         int nextLevel = 2*l9r+1 + (sh > 0);

         for(; i < nVertices + 1; i++)
            children[i] = fromCentroid(nextLevel, vertices[i-1]);
      }
      return i;
   }


   property CRSExtent ri5x6Extent
   {
      get
      {
         int i;
         Array<Pointd> vertices = null;
         int nVertices;
         Pointd kVertices[9];
         int numPoints = getBaseRefinedVertices(false, kVertices);
         if(numPoints)
         {
            Array<Pointd> ap = null;
            //bool geodesic = false; //true;
            bool refine = true; //zone.subHex < 3;  // Only use refinement for ISEA for even levels -- REVIEW: When and why do we need refinement here?
            int i;

            if(refine)
            {
               Array<Pointd> r = refine5x6(numPoints, kVertices, 1, false);
               ap = { size = r.count };
               for(i = 0; i < r.count; i++)
                  ap[i] = { r[i].x, r[i].y };
               delete r;
            }
            else
            {
               ap = { size = numPoints };
               for(i = 0; i < numPoints; i++)
                  ap[i] = { kVertices[i].x, kVertices[i].y };
            }
            vertices = ap;
         }
         nVertices = vertices ? vertices.count : 0;

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
         int sh = subHex;
         if(sh == 1 || sh == 6)
            value = { 1, 0 }; // "North" pole (Even level B and Odd level G)
         else if(sh == 2 || sh == 7)
            value = { 4, 6 }; // "South" pole (Even level C and Odd level H)
         else
         {
            uint level = levelI9R, root = rootRhombus;
            uint64 rix = rhombusIX;
            uint64 p = POW3(level);
            uint64 rowOP = (root + 1) >> 1, colOP = root >> 1;
            uint64 ixOP = (uint64)(rix / p);
            uint64 row = (uint64)(rowOP * p + ixOP);
            uint64 col = (uint64)((colOP - ixOP) * p + rix); // distributivity on: rix - (ixOP * p) for (rix % p)
            double d =  1.0 / p;
            Pointd tl { col * d, row * d };

            if(sh == 0 || sh == 3)
               value = { tl.x, tl.y }; // Even level A or Odd level D hex
            else if(sh == 4) // Odd level E hex
               value = { tl.x + 2*d/3, tl.y + d/3 };
            else if(sh == 5) // Odd level F hex
               value = { tl.x + d/3, tl.y + 2*d/3 };
         }
      }
   }

   property bool isCentroidChild
   {
      get
      {
         int sh = subHex;
         if(this == nullZone)
            return false;
         else if(sh > 2)  // Odd level
            return sh == 3 || sh == 6 || sh == 7; // D, G and H are centroid children
         else if(sh == 1 || sh == 2) // B and C are centroid children
            return true;
         else
         {
            // Even level -- some A are centroid children
            int level = this.levelI9R;
            uint64 rix = this.rhombusIX;
            uint64 p = POW3(level);
            uint64 r = rix / p, c = rix % p;
            if(!(r % 3) && !(c % 3))
               return true; // Both row & column multiple of 3 are centroid children
            else if(!((r + c) % 3))
               return true; // (row + column) multiple of 3 are centroid children
         }
         return false;
      }
   }

   int64 getSubZonesCount(int rDepth)
   {
      int64 nHexSubZones = rDepth > 0 ? POW3(rDepth) + POW3((rDepth + 1)/2) + 1 : 1;
      return (nHexSubZones * nPoints + 5) / 6;
   }

   I3HZone getFirstSubZone(int rDepth)
   {
      Pointd firstCentroid;

      getFirstSubZoneCentroid(rDepth, firstCentroid);
      return fromCentroid(level + rDepth, firstCentroid);
   }

   void getFirstSubZoneCentroid(int rDepth, Pointd firstCentroid)
   {
      getI3HFirstSubZoneCentroid(this, rDepth, firstCentroid);
   }

   Array<Pointd> getSubZoneCentroids(int rDepth)
   {
      return getI3HSubZoneCentroids(this, rDepth);
   }

   private /*static */bool orderZones(int zoneLevel, AVLTree<I3HZone> tsZones, Array<I3HZone> zones)
   {
      Array<Pointd> centroids = getSubZoneCentroids(zoneLevel - level);
      if(centroids)
      {
         int nSubZones = centroids.count;
         int i;

         for(i = 0; i < nSubZones; i++)
         {
            I3HZone key = I3HZone::fromCentroid(zoneLevel, centroids[i]);
            if(tsZones.Find(key))
               zones.Add(key);
            else
            {
      #ifdef _DEBUG
               PrintLn("WARNING: mismatched sub-zone while re-ordering");
      #endif
            }
         }
         delete centroids;
         return true;
      }
      else
         return false; // Work around until all sub-zone listing fully handled
   }
}

static void compactI3HZones(AVLTree<I3HZone> zones, int level)
{
   AVLTree<I3HZone> output { };
   AVLTree<I3HZone> next { };
   int l;

   for(l = level - 2; l >= 0; l -= 2)
   {
      int i;
      for(z : zones)
      {
         I3HZone zone = z, cgParents[3];
         int nCGParents = zone.getContainingGrandParents(cgParents);
         int p;
         for(p = 0; p < nCGParents; p++)
         {
            I3HZone gParent = cgParents[p];
            if(gParent != nullZone && !next.Find(gParent))
            {
               I3HZone cZone = gParent.centroidChild.centroidChild;
               I3HZone neighbors[6];
               int nNeighbors = cZone.getNeighbors(neighbors, null);
               bool parentAllIn = true;

               for(i = 0; i < nNeighbors; i++)
               {
                  I3HZone nb = neighbors[i];
                  if(nb != nullZone && !zones.Find(nb))
                  {
                     parentAllIn = false;
                     break;
                  }
               }

               if(parentAllIn)
               {
                  // Grandparent vertex children's centroid children are partially within it
                  // and must be present to perform replacement
                  I3HZone children[7];
                  int nChildren = gParent.getChildren(children);

                  for(i = 1; i < nChildren; i++)
                  {
                     I3HZone ch = children[i];
                     if(ch != nullZone)
                     {
                        I3HZone cChild = ch.centroidChild;

                        if(!zones.Find(cChild))
                        {
                           Pointd cv = cChild.centroid;
                           int cl = 2*cChild.levelI9R + (cChild.subHex >= 3);
                           I3HZone sub = I3HZone::fromCentroid(cl + 2, cv);
                           if(!output.Find(sub))
                              parentAllIn = false;
                        }
                     }
                  }
                  if(parentAllIn)
                     next.Add(gParent);
               }
            }
         }
      }

      for(z : zones)
      {
         I3HZone zone = z, cgParents[3];
         int nCGParents = zone.getContainingGrandParents(cgParents), i;
         bool allIn = true;

         for(i = 0; i < nCGParents; i++)
         {
            if(!next.Find(cgParents[i]))
            {
               allIn = false;
               break;
            }
         }
         if(!allIn)
            output.Add(zone);
      }

      if(/*0 && */l - 2 >= 0 && next.count)
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

   if(zones.count >= 32 && zones.firstIterator.data.level == 1)
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
         I3HZone zone = z;
         int level = zone.level;
         if(level == 1)
            nL1++;
         else if(level > 1)
            break;
      }

      // if(allL1)
      if(nL1 == 32)
      {
         // Simplifying full globe to level 0 zones
         int r;
         zones.Free();
         for(r = 0; r < 10; r++)
            zones.Add({ 0, r, 0, 0 });
         zones.Add({ 0, 0, 0, 1 });
         zones.Add({ 0, 9, 0, 2 });
      }
   }
}

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
   // return *zone != I3HZone::fromCentroid(zone->level, centroid);
}

static void getIcoNetExtentFromVertices(I3HZone zone, CRSExtent value)
{
   int i;
   Array<Pointd> vertices = getIcoNetRefinedVertices(zone, 0);
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

static Array<Pointd> getIcoNetRefinedVertices(I3HZone zone, int edgeRefinement)   // 0 for 1-20 based on level
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
            RI5x6Projection::toIcosahedronNet({ r[i].x, r[i].y }, ap[i]);
         delete r;
      }
      else
      {
         ap = { size = numPoints };
         for(i = 0; i < numPoints; i++)
            RI5x6Projection::toIcosahedronNet({ vertices[i].x, vertices[i].y }, ap[i]);
      }
      rVertices = ap;
   }
   return rVertices;
}
