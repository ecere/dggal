public import IMPORT_STATIC "ecrt"
private:

import "dggrs"

#include <stdio.h>

enum GGGNeighborType
{
   north, north2, west, east, south, south2
};

public class GNOSISGlobalGrid : DGGRS
{
   // DGGH
   uint64 countZones(int level)
   {
      return 8LL * GGGZone { }.getSubZonesCount(level);
   }

   int getMaxDGGRSZoneLevel() { return maxGGGZoomLevel; }
   int getRefinementRatio() { return 4; }
   int getMaxParents() { return 1; }
   int getMaxNeighbors() { return 5; }
   int getMaxChildren() { return 4; }

   uint64 countSubZones(GGGZone zone, int depth)
   {
      return zone.getSubZonesCount(depth);
   }

   int getZoneLevel(GGGZone zone)
   {
      return zone.level;
   }

   int countZoneEdges(GGGZone zone)
   {
      // Polar zone have 3 egdes
      int row = zone.row;
      return (!row || row == (2 << zone.level) - 1) ? 3 : 4;
   }

   __attribute__ ((optimize("-fno-unsafe-math-optimizations")))
   double getZoneArea(GGGZone zoneID)
   {
      GeoExtent extent = zoneID.toClassic().extent;
      //PrintLn(extent);
      return extent.geodeticArea;
   }

   DGGRSZone getZoneFromWGS84Centroid(int level, const GeoPoint centroid)
   {
      if(level <= maxGGGZoomLevel)
      {
         GeoExtent e { centroid, centroid };
         return ClassicGGGKey::fromExtent(e, level, true).toGGG();
      }
      return nullZone;
   }

   DGGRSZone getZoneFromCRSCentroid(int level, CRS crs, const Pointd centroid)
   {
      if(level <= maxGGGZoomLevel)
      {
         switch(crs)
         {
            case 0: return getZoneFromWGS84Centroid(level, (GeoPoint)centroid);
            case CRS { ogc, 84 }: return getZoneFromWGS84Centroid(level, { centroid.y, centroid.x });
            case CRS { epsg, 4326 }: return getZoneFromWGS84Centroid(level, { centroid.x, centroid.y });
            default: return nullZone;
         }
      }
      return nullZone;
   }

   void getZoneCRSCentroid(GGGZone zone, CRS crs, Pointd centroid)
   {
      GeoPoint c;
      getZoneWGS84Centroid(zone, c);
      switch(crs)
      {
         case 0: centroid = { (Radians)c.lat, (Radians)c.lon }; break;
         case CRS { ogc, 84 }: centroid = { c.lon, c.lat }; break;
         case CRS { epsg, 4326 }: centroid = { c.lat, c.lon }; break;
         default: centroid = { MAXDOUBLE, MAXDOUBLE }; break;
      }
   }

   void getZoneWGS84Centroid(GGGZone zone, GeoPoint centroid)
   {
      GeoExtent e = zone.toClassic().extent;
      centroid =
      {
         ((Radians)e.ll.lat + (Radians)e.ur.lat) / 2,
         ((Radians)e.ll.lon + (Radians)e.ur.lon) / 2
      };
   }

   int getZoneCRSVertices(GGGZone zone, CRS crs, Pointd * vertices)
   {
      int count = 0;
      if(!crs || crs == { ogc, 84 } || crs == { epsg, 4326 })
      {
         GeoPoint v[4];
         int i;

         count = getZoneWGS84Vertices(zone, v);

         switch(crs)
         {
            case 0: memcpy(vertices, v, sizeof(GeoPoint) * count); break;
            case CRS { ogc, 84 }:
               for(i = 0; i < count; i++)
                  vertices[i] = { v[i].lon, v[i].lat };
               break;
            case CRS { epsg, 4326 }:
               for(i = 0; i < count; i++)
                  vertices[i] = { v[i].lat, v[i].lon };
               break;
         }
      }
      return count;
   }

   int getZoneWGS84Vertices(GGGZone zone, GeoPoint * vertices)
   {
      int row = zone.row;
      bool np = !row, sp = row == (2 << zone.level) - 1;
      GeoExtent e = zone.toClassic().extent;
      int count = 0;

      vertices[count++] = { e.ur.lat, e.ll.lon };
      vertices[count++] = { e.ll.lat, e.ll.lon };
      if(!sp) vertices[count++] = { e.ll.lat, e.ur.lon };
      if(!np) vertices[count++] = { e.ur.lat, e.ur.lon };
      return count;
   }

   Array<Pointd> getZoneRefinedCRSVertices(GGGZone zone, CRS crs, int edgeRefinement)
   {
      if(!crs || crs == { ogc, 84 } || crs == { epsg, 4326 })
      {
         GeoPoint v[4];
         uint count = 4, i;
         Array<Pointd> vertices { size = count };
         GeoExtent e = zone.toClassic().extent;
         v[0] = { e.ur.lat, e.ll.lon };
         v[1] = { e.ll.lat, e.ll.lon };
         v[2] = { e.ll.lat, e.ur.lon };
         v[3] = { e.ur.lat, e.ur.lon };

         switch(crs)
         {
            case 0: memcpy(vertices.array, v, sizeof(GeoPoint) * count); break;
            case CRS { ogc, 84 }:
               for(i = 0; i < count; i++)
                  vertices[i] = { v[i].lon, v[i].lat };
               break;
            case CRS { epsg, 4326 }:
               for(i = 0; i < count; i++)
                  vertices[i] = { v[i].lat, v[i].lon };
               break;
         }
         return vertices;
      }
      return null;
   }

   Array<GeoPoint> getZoneRefinedWGS84Vertices(GGGZone zone, int edgeRefinement)
   {
      Array<GeoPoint> vertices { size = 4 };
      GeoExtent e = zone.toClassic().extent;
      vertices[0] = { e.ur.lat, e.ll.lon };
      vertices[1] = { e.ll.lat, e.ll.lon };
      vertices[2] = { e.ll.lat, e.ur.lon };
      vertices[3] = { e.ur.lat, e.ur.lon };
      return vertices;
   }

   void getZoneCRSExtent(GGGZone zone, CRS crs, CRSExtent extent)
   {
      GeoExtent geo = zone.toClassic().extent;

      switch(crs)
      {
         case 0:
            extent.crs = 0; // EPSG:4326 but in Radians
            extent.tl = { (Radians)geo.ur.lat, (Radians)geo.ll.lon };
            extent.br = { (Radians)geo.ll.lat, (Radians)geo.ur.lon };
            break;
         case CRS { ogc, 84 }:
            extent.crs = crs;
            extent.tl = { geo.ur.lat, geo.ll.lon };
            extent.br = { geo.ll.lat, geo.ur.lon };
            break;
         case CRS { epsg, 4326 }:
            extent.crs = crs;
            extent.tl = { geo.ll.lon, geo.ur.lat };
            extent.br = { geo.ur.lon, geo.ll.lat };
            break;
         default:
            extent.crs = 0;
            extent.tl = { MAXDOUBLE, MAXDOUBLE };
            extent.br = { -MAXDOUBLE, -MAXDOUBLE };
            break;
      }
   }

   void getZoneWGS84Extent(GGGZone zone, GeoExtent extent)
   {
      extent = zone.toClassic().extent;
   }

   int getZoneParents(GGGZone zone, GGGZone * parents)
   {
      int level = zone.level;
      GGGZone parent = level ? zone.toClassic().getLowerResZone(level-1).toGGG() : nullZone;
      parents[0] = parent;
      return parent != nullZone;
   }

   int getZoneChildren(GGGZone zone, GGGZone * children)
   {
      uint l = zone.level+1;
      if(l <= maxGGGZoomLevel)
      {
         int nChildren = 0;
         uint numRow = 2 << l;
         uint midRow = numRow >> 1;
         uint row1 = zone.row << 1, row2 = row1 | 1;
         uint c = 1 << GGGZone { l, zone.row >= midRow ? row1 : row2 }.getCoalesceShift();
         uint col1 = zone.col << 1, col2 = col1 + c;
         uint lastRow = numRow-1;
         children[nChildren++] = { l, row1, col1 };
         children[nChildren++] = { l, row2, col1 };
         if(zone.row)
            children[nChildren++] = { l, row1, col2 };
         if(zone.row < lastRow)
            children[nChildren++] = { l, row2, col2 };
         return nChildren;
      }
      return 0;
   }

   int getZoneNeighbors(GGGZone zone, GGGZone * neighbors, GGGNeighborType * nbType)
   {
      int l = zone.level;
      int numNeighbors = 0;
      uint c = 1 << zone.getCoalesceShift();
      uint northRow = zone.row - 1, southRow = zone.row + 1;
      uint cNorth = 1 << GGGZone { l, northRow }.getCoalesceShift();
      uint cSouth = 1 << GGGZone { l, southRow }.getCoalesceShift();
      uint numRows = 2 << l, numCol = 4 << l;
      uint lastRow = numRows-1; // uint midRow = numRows >> 1;

      // North
      if(zone.row > 0)
      {
         neighbors[numNeighbors] = { l, northRow, zone.col - (zone.col % cNorth) };
         if(nbType) nbType[numNeighbors] = north;
         numNeighbors++;
      }
      if(zone.row > 0 && cNorth < c)
      {
         neighbors[numNeighbors] = GGGZone { l, northRow, zone.col + (c >> 1) };
         if(nbType) nbType[numNeighbors] = north2;
         numNeighbors++;
      }

      // West
      neighbors[numNeighbors] = zone.col == 0 ? GGGZone { l, zone.row, numCol - c } : GGGZone { l, zone.row, zone.col - c };
      if(nbType) nbType[numNeighbors] = west;
      numNeighbors++;

      // East
      neighbors[numNeighbors] = zone.col == numCol - c ? GGGZone { l, zone.row, 0 } : GGGZone { l, zone.row, zone.col + c };
      if(nbType) nbType[numNeighbors] = east;
      numNeighbors++;

      // South
      if(zone.row < lastRow)
      {
         neighbors[numNeighbors] = GGGZone { l, southRow, zone.col - (zone.col % cSouth) };
         if(nbType) nbType[numNeighbors] = south;
         numNeighbors++;
      }
      if(zone.row < lastRow && cSouth < c)
      {
         neighbors[numNeighbors] = GGGZone { l, southRow, zone.col + (c >> 1) };
         if(nbType) nbType[numNeighbors] = south2;
         numNeighbors++;
      }
      return numNeighbors;
   }

   Array<DGGRSZone> listZones(int level, const GeoExtent bbox)
   {
      Array<GGGZone> zones { };
      listGGGZones(zones, level, bbox, 0);
      if(!zones.count)
         delete zones;
      return (Array<DGGRSZone>)zones;
   }

   // Text ZIRS
   void getZoneTextID(GGGZone zone, String zoneID)
   {
      sprintf(zoneID, "%X-%X-%X", zone.level, zone.row, zone.col);
   }

   GGGZone getZoneFromTextID(const String zoneID)
   {
      GGGZone result = nullZone;
      int reqLevel, tileRow, tileCol;
      int c = sscanf(zoneID, "%X-%X-%X", &reqLevel, &tileRow, &tileCol);
      GGGZone key { reqLevel, tileRow, tileCol };
      ClassicGGGKey cKey = key.toClassic();
      if(c == 3 && cKey.isValid() && !(tileCol % (1 << key.getCoalesceShift())))
         result = key;
      return result;
   }

   // Sub-zone Order
   GGGZone getFirstSubZone(GGGZone zone, int depth)
   {
      GeoExtent extent = zone.toClassic().extent;
      uint64 d = 2LL << depth;
      extent.ur.lon = extent.ll.lon + (extent.ur.lon - extent.ll.lon) / d;
      extent.ll.lat = extent.ur.lat + (extent.ll.lat - extent.ur.lat) / d;
      return ClassicGGGKey::fromExtent(extent, zone.level + depth, true).toGGG();
   }

   Array<Pointd> getSubZoneCRSCentroids(GGGZone parent, CRS crs, int depth)
   {
      Array<GeoPoint> geo = parent.toClassic().getSubZoneCentroids(depth);
      if(geo)
      {
         uint count = geo.count, i;
         Array<Pointd> centroids { size = count };

         switch(crs)
         {
            case 0:
               for(i = 0; i < count; i++)
                  centroids[i] = { (Radians)geo[i].lat, (Radians)geo[i].lon };
               break;
            case CRS { ogc, 84 }:
               for(i = 0; i < count; i++)
                  centroids[i] = { geo[i].lon, geo[i].lat };
               break;
            case CRS { epsg, 4326 }:
               for(i = 0; i < count; i++)
                  centroids[i] = { geo[i].lat, geo[i].lon };
               break;
            default:
               delete centroids;
         }
         return centroids;
      }
      return null;
   }

   Array<GeoPoint> getSubZoneWGS84Centroids(GGGZone parent, int depth)
   {
      return parent.toClassic().getSubZoneCentroids(depth);
   }

   void compactZones(Array<DGGRSZone> zones)
   {
      int maxLevel = 0, i, count = zones.count;

      for(i = 0; i < count; i++)
      {
         GGGZone zone = (GGGZone)zones[i];
         if(zone != nullZone)
         {
            int level = zone.level;
            if(level > maxLevel)
               maxLevel = level;
         }
      }

      compactGGGZones((Array<GGGZone>)zones, 0, maxLevel);
   }
}

// Methods and definitions for the GNOSIS Global Grid
static define firstZoomLevelRadians = Pi/2;
static define maxGGGZoomLevel = 28;     // Maximum level we can reach with a 64 bit GGGZone; A tile at zoom level 28 covers ~3 cm (~0.1 mm resolution with 256 tile resolution)

static define zoneEpsilon = Radians { 0.000000001 };

static Radians floorDelta(Radians value, Radians delta)
{
   return floor(value / delta + zoneEpsilon) * delta;
}

static Radians ceilDelta(Radians value, Radians delta)
{
   return ceil(value / delta - zoneEpsilon) * delta;
}

// These classic keys have rows flipped North-South, and columns numbering does not skip for coalescing regions
// TODO: Adapt code to always use GGGZone, implementing these methods directly in GGGZone instead
static class ClassicGGGKey : uint64
{
public:
   uint level:5:59, row:29:30, col:30:0;

   GGGZone toGGG()
   {
      int coalesce = getCoalesceShift();
      int numRows = 2 << level;
      if(row > numRows - 1) row = numRows - 1;
      return { level, numRows-1-row, col << coalesce };
   }

   int getCoalesceShift()
   {
      return ((GGGZone)this).getCoalesceShift();
   }

   Array<GeoPoint> getSubZoneCentroids(int rDepth)
   {
      // The same getSubZonesCount() implementation also works with classic key
      uint64 nSubZones = ((GGGZone)this).getSubZonesCount(rDepth);
      if(nSubZones < 1LL<<31)
      {
         int level = this.level + rDepth;
         GeoExtent extent = this.extent;
         int i = 0;
         Array<GeoPoint> centroids { size = (uint)nSubZones };
         Array<GGGZone> subZones { };

         // REVIEW: Optimize this
         listGGGZones(subZones, level, extent, 0);

   #ifdef _DEBUG
         if(nSubZones != subZones.count)
            PrintLn("WARNING: mismatched GGG sub-zone count");
   #endif

         for(i = 0; i < nSubZones && i < subZones.count; i++)
         {
            const GeoExtent e = subZones[i].toClassic().extent;
            centroids[i] =
            {
               ((Radians)e.ll.lat + (Radians)e.ur.lat) / 2,
               ((Radians)e.ll.lon + (Radians)e.ur.lon) / 2
            };
         }
         delete subZones;
         return centroids;
      }
      return null;
   }

   property GeoExtent extent
   {
      get
      {
         Radians diffLat { Pi / (2 << level) };
         Radians diffLong = ((GGGZone)this).getTileDeltaLon();

         value.ll.lat = -Pi / 2 + row * diffLat;
         value.ur.lat = value.ll.lat + diffLat;
         value.ll.lon = wrapLon(col * diffLong - Pi);
         value.ur.lon = Min((Radians)Pi, (Radians)value.ll.lon + diffLong); // Values over Pi were problematic with wrapLon()
      }
   }

   ClassicGGGKey ::fromExtent(const GeoExtent extent, int level, bool wrap)
   {
      Radians diffLat { Pi / (2 << level) };
      GeoPoint middle
      {
         (extent.ll.lat + extent.ur.lat) / 2,
         wrap ? wrapLon((extent.ll.lon + extent.ur.lon) / 2) : (extent.ll.lon + extent.ur.lon) / 2
      };
      int row = (int)(double)((middle.lat + Pi / 2) / diffLat);
      Radians llLat = -Pi / 2 + row * diffLat, urLat = llLat + diffLat;
      // NOTE: This was breaking in Antarctica with identical extent ll & ur on exactly tile boundary
      Radians diffLong = GGGZone::getDeltaLon(llLat, urLat, level);
      int col;
      if(middle.lon >= Pi - radEpsilon) middle.lon -= 2*Pi;  // wrapLon() doesn't wrap exactly 180. Should it?
      col = (int)(double)((middle.lon + Pi) / diffLong);
      return { level, row, col };
   }

   // Returns a ClassicGGGKey for a lower level version of the ClassicGGGKey.
   // The level is determined by the lowLevel parameter.
   // lowLevel must be lower than the level of the ClassicGGGKey.
   ClassicGGGKey getLowerResZone(int lowLevel)
   {
      if(level < lowLevel)
         return nullZone;
      else if(level == lowLevel)
         return this;
      else
      {
         int coalesce = getCoalesceShift();
         int d = level - lowLevel;
         ClassicGGGKey key { lowLevel, row >> d, (col << coalesce) >> d };
         // key.lon >>= key.getCoalesceShift(); // FIXME: bit class bug?
         key.col = key.col >> key.getCoalesceShift();
         return key;
      }
   }

   bool isValid()
   {
      int level = this.level;
      Radians diffLat { Pi / (2 << level) };
      Radians lat = -Pi/2 + diffLat * row;
      if(lat < Pi/2)
      {
         Radians diffLon = ((GGGZone)this).getTileDeltaLon();
         Radians lon = -Pi + col * diffLon;
         if(lon < Pi)
            return true;
      }
      return false;
   }
}

// Public for use in test
public class GGGZone : DGGRSZone
{
public:
   uint level:5:59, row:29:30, col:30:0;

private:
   ClassicGGGKey toClassic()
   {
      int coalesce = getCoalesceShift();
      int numRows = 2 << level;
      return { level, numRows-1-row, col >> coalesce };
   }

   Radians ::getDeltaLon(Radians lat1, Radians lat2, int level)
   {
      Radians lat { Abs((lat1 + lat2) / 2 + zoneEpsilon) };
      int numRows = 2 << level;
      int row = (int)((Pi/2 - lat) * numRows / Pi);
      return GGGZone { level, row }.getTileDeltaLon();
   }

   Radians getTileDeltaLon()
   {
      return 2*Pi / (4 << level >> getCoalesceShift());
   }

   int getCoalesceShift()
   {
      uint row = this.row, level = this.level;
      int numRows = 2 << level;
      if(row >= 0 && row < numRows)
      {
         int hr = numRows >> 1, r = row >= hr ? numRows-1-row : row, coalesce = 0, i;
         for(i = 0; i < level; i++, r >>= 1)
            if(!r)
               coalesce++;
         return coalesce;
      }
      else
         return 0;
   }

   int OnCompare(GGGZone b)
   {
      // NOTE: CustomAVLTree Optimization currently assume signed 64-bit integers and this would result in mismatch comparisons
      if((int64)this < (int64)b) return -1;
      if((int64)this > (int64)b) return 1;
      return 0;
   }

   bool isValidDGGRSZone()
   {
      return toClassic().isValid() && !(col % (1 << getCoalesceShift()));
   }

   int64 getSubZonesCount(int depth)
   {
      int64 count;

      if(depth > 0)
      {
         int level = this.level, row = this.row;

         if(!row || row == ((2LL << level)-1))
            // https://oeis.org/A007583
            /*
            count = 3;
            while(--depth)
               count += 2 * pow(4, depth);
            */
            count = ((1LL << (2*depth + 1)) + 1)/3;
         else
            // count = (int)pow(4, depth);
            count = 1LL << depth, count *= count;
      }
      else
         count = 1;
      return count;
   }

   // REVIEW: More efficient way to obtain deterministic order
   private bool orderGGGZones(int zoneLevel, AVLTree<GGGZone> tsZones, Array<GGGZone> zones)
   {
      Array<GeoPoint> centroids = toClassic().getSubZoneCentroids(zoneLevel - level);
      if(centroids)
      {
         int nSubZones = centroids.count;
         int i;

         tsZones.Free();
         for(z : zones)
            tsZones.Add(z);
         zones.Free();

         for(i = 0; i < nSubZones; i++)
         {
            GGGZone key = ClassicGGGKey::fromExtent( { centroids[i], centroids[i] }, zoneLevel, true).toGGG();

            if(tsZones.Find(key))
               zones.Add(key);
            else
            {
      #if 0 //def _DEBUG
               PrintLn("WARNING: mismatched sub-zone");
      #endif
            }
         }
         delete centroids;
      }
      return true;
   }
};

private static uint addGGGRow(Array<GGGZone> tiles, int level, GeoExtent origExtent, Radians diffLon)
{
   int count = (int) ceil(((Radians)origExtent.ur.lon - (Radians)origExtent.ll.lon) / diffLon - zoneEpsilon);
   if(tiles)
   {
      GeoExtent curExtent = origExtent;
      GGGZone * t;
      int i;

      if(tiles.count + count > tiles.minAllocSize)
         tiles.minAllocSize = tiles.count + count + (tiles.minAllocSize >> 1);
      t = tiles.array + tiles.count;
      for(i = 0; i < count; i++, t++)
      {
         curExtent.ur.lon = curExtent.ll.lon + diffLon;
         *t = ClassicGGGKey::fromExtent(curExtent, level, false).toGGG();
         curExtent.ll.lon = curExtent.ur.lon;
      }
      tiles.count += count;
   }
   return count;
}

static uint listGGGZones(Array<GGGZone> zones, int level, const GeoExtent _extent, uint max)
{
   uint count = 0;
   Radians dLat = firstZoomLevelRadians / (1 << level);
   GeoExtent snapped
   {
      ll.lat = floorDelta(Max((Radians)_extent.ll.lat + 0.00001*dLat, -Pi/2), dLat),
      ur.lat = ceilDelta (Min((Radians)_extent.ur.lat - 0.00001*dLat,  Pi/2), dLat)
   };
   int rowCount;

   if(snapped.ur.lat <= snapped.ll.lat && _extent.ur.lat > _extent.ll.lat)
      // Correct polar tile snapping that might result in a null extent
      // due to level's dLat being bigger than extent delta
      snapped.ll.lat = snapped.ur.lat - dLat;
   rowCount = (int)ceil((snapped.ur.lat - snapped.ll.lat) / dLat - zoneEpsilon);

   if(max && level > 0 && rowCount > max)
      return MAXINT;

   if(rowCount)
   {
      int i;
      bool dateLine = false;
      GeoExtent extent;
      Radians llLon = wrapLon(_extent.ll.lon), urLon = wrapLon(_extent.ur.lon);

      if(_extent.ur.lon - _extent.ll.lon > 2*Pi - radEpsilon)
         llLon = -Pi, urLon = Pi;
      else if(llLon > urLon + 0.000001)   // Turned a - into a + here as small extent caused date line to be true!
         dateLine = true;

      if(zones)
         zones.minAllocSize = 8;
      for(i = 0, extent.ur.lat = snapped.ur.lat; i < rowCount; i++)
      {
         bool pastSouth = (extent.ll.lat = extent.ur.lat - dLat) < -Pi/2 - 0.000001;
         bool pastNorth = extent.ll.lat > Pi/2 + 0.000001;
         Radians dLon = GGGZone::getDeltaLon(extent.ll.lat, extent.ur.lat, level);
         snapped.ll.lon = floorDelta(llLon + 0.00001*dLon, dLon);
         snapped.ur.lon = ceilDelta (urLon - 0.00001*dLon, dLon);
         if((pastSouth && (Radians)extent.ll.lat < -Pi/2 - 0.000001) ||
            (pastNorth && (Radians)extent.ur.lat >  Pi/2 + 0.000001))
            snapped.ll.lon = -Pi, snapped.ur.lon = Pi;
         else if(snapped.ur.lon <= snapped.ll.lon && urLon > llLon)
            // Correct polar tile snapping that might result in a null extent
            snapped.ur.lon = snapped.ll.lon + dLon;

         if(dateLine)
         {
            if(snapped.ur.lon > snapped.ll.lon - 0.000001)
               extent.ll.lon = -Pi, extent.ur.lon = Pi;
            else
            {
               extent.ll.lon = -Pi, extent.ur.lon = snapped.ur.lon;
               addGGGRow(zones, level, extent, dLon);

               extent.ll.lon = snapped.ll.lon, extent.ur.lon = Pi;
            }
            count += addGGGRow(zones, level, extent, dLon);
         }
         else
         {
            extent.ll.lon = snapped.ll.lon, extent.ur.lon = snapped.ur.lon;
            count += addGGGRow(zones, level, extent, dLon);
         }
         extent.ur.lat = extent.ll.lat;

         if(max && level > 0 && count > max)
            return MAXINT;
      }
   }
   return count;
}

static void compactGGGZones(Array<GGGZone> zones, int start, int maxLevel)
{
   HashTable<GGGZone> orig { initSize = zones.count - start};
   int l;

   for(l = maxLevel; l > 0; l--)
   {
      int i;
      uint numParentLat = 2 << (l-1);
      uint midLat = numParentLat >> 1;
      uint parentLastLat = numParentLat-1;

      for(i = start; i < zones.count; i++)
         orig.Add(zones[i]);

      for(i = start; i < zones.count; i++)
      {
         GGGZone key = zones[i];
         if(key.level == l && orig.Find(key))
         {
            GGGZone parent = key.toClassic().getLowerResZone(l-1).toGGG();
            uint lat1 = parent.row << 1, lat2 = lat1 | 1;
            uint c = 1 << GGGZone { l, parent.row >= midLat ? lat1 : lat2 }.getCoalesceShift();
            uint lon1 = parent.col << 1, lon2 = lon1 + c;
            GGGZone children[4] =
            {
               GGGZone { l, lat1, lon1 },
               GGGZone { l, lat2, lon1 },
               !parent.row                 ? nullZone : { l, lat1, lon2 },
               parent.row == parentLastLat ? nullZone : { l, lat2, lon2 }
            };
            bool missingSibling = false;
            int j;

            for(j = 0; j < 4; j++)
            {
               GGGZone sKey = children[j];
               if(sKey != nullZone && sKey != key && !orig.Find(sKey))
               {
                  missingSibling = true;
                  break;
               }
            }
            if(!missingSibling)
            {
               for(j = 0; j < 4; j++)
               {
                  GGGZone sKey = children[j];
                  if(sKey != nullZone)
                     orig.TakeOut(sKey);
               }
               orig.Add(parent);
            }
         }
      }
      i = start;
      for(k : orig)
         zones[i++] = k;
      zones.count = i;
      orig.Free();
   }
   delete orig;
}
