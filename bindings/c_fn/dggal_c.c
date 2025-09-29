#ifdef DGGAL_ALLINONE
   #undef BINDINGS_SHARED
#endif

#include <dggal.h>

#ifdef DGGAL_ALLINONE
   #if defined(__WIN32__)
      #define LIB_EXPORT __attribute__((dllexport)) __attribute__ ((visibility("default")))
      #define LIB_IMPORT __attribute__((dllimport))
   #else
      #define LIB_EXPORT __attribute__ ((visibility("default")))
      #define LIB_IMPORT
   #endif
#endif

static Class * class_Array_DGGRSZone;

LIB_EXPORT CRSRegistry DGGAL_CRS(CRSRegistry registry, unsigned int code, int h)
{
   return CRS(registry, code, h);
}

// Dynamic Arrays

//    of GeoPoint
LIB_EXPORT int DGGAL_Array_GeoPoint_getCount(const T(Array, GeoPoint) self)
{
   return Array_get_size(self);
}

LIB_EXPORT const GeoPoint * DGGAL_Array_GeoPoint_getPointer(const T(Array, GeoPoint) self)
{
   return self ? (GeoPoint *)((struct CM(Array) *)(((byte *)self) + CO(Array)->offset))->array : null;
}

LIB_EXPORT void DGGAL_Array_GeoPoint_delete(T(Array, GeoPoint) self)
{
   deletei(self);
}

//    of Pointd
LIB_EXPORT int DGGAL_Array_Pointd_getCount(const T(Array, Pointd) self)
{
   return Array_get_size(self);
}

LIB_EXPORT const Pointd * DGGAL_Array_Pointd_getPointer(const T(Array, Pointd) self)
{
   return self ? (Pointd *)((struct CM(Array) *)(((byte *)self) + CO(Array)->offset))->array : null;
}

LIB_EXPORT void DGGAL_Array_Pointd_delete(T(Array, Pointd) self)
{
   deletei(self);
}

//    of DGGRSZone
LIB_EXPORT T(Array, DGGRSZone) DGGAL_Array_DGGRSZone_new(uint size)
{
   T(Array, DGGRSZone) array = newi(Array, DGGRSZone);
   if(array)
      Array_set_size(array, size);
   return array;
}

LIB_EXPORT int DGGAL_Array_DGGRSZone_getCount(const T(Array, DGGRSZone) self)
{
   return Array_get_size(self);
}

LIB_EXPORT DGGRSZone * DGGAL_Array_DGGRSZone_getPointer(const T(Array, DGGRSZone) self)
{
   return self ? (DGGRSZone *)((struct CM(Array) *)(((byte *)self) + CO(Array)->offset))->array : null;
}

LIB_EXPORT void DGGAL_Array_DGGRSZone_delete(T(Array, DGGRSZone) self)
{
   deletei(self);
}

// DGGAL Initialization
LIB_EXPORT Module DGGAL_init()
{
   Module mDGGAL = null;
   Application app = ecrt_init(null, true, false, 0, null);
   if(app)
   {
      mDGGAL = dggal_init(app);
      if(!mDGGAL)
         deletei(app);
      else
         class_Array_DGGRSZone = eC_findClass(app, "Array<DGGRSZone>");
   }
   return mDGGAL;
}

LIB_EXPORT void DGGAL_terminate(Module mDGGAL)
{
   if(mDGGAL)
   {
      Application app = ((struct CM(Module) *)(((byte *)mDGGAL) + CO(Module)->offset))->application;
      deletei(app);
   }
}

// DGGRS Instantiation
LIB_EXPORT DGGRS DGGAL_DGGRS_new(Module mDGGAL, const String name)
{
   DGGRS dggrs = null;
   if(name)
   {
      Class * c = eC_findClass(mDGGAL, name);
      if(c != null)
         dggrs = Instance_new(c);
   }
   return dggrs;
}

LIB_EXPORT void DGGAL_DGGRS_delete(DGGRS self)
{
   deletei(self);
}

// DGGRS Class

// Virtual Methods
LIB_EXPORT DGGRSZone DGGAL_DGGRS_getZoneFromTextID(DGGRS self, const String zoneID)
{
   return DGGRS_getZoneFromTextID(self, zoneID);
}

LIB_EXPORT int DGGAL_DGGRS_getZoneLevel(DGGRS self, DGGRSZone zone)
{
   return DGGRS_getZoneLevel(self, zone);
}

LIB_EXPORT int DGGAL_DGGRS_countZoneEdges(DGGRS self, DGGRSZone zone)
{
   return DGGRS_countZoneEdges(self, zone);
}

LIB_EXPORT int DGGAL_DGGRS_getRefinementRatio(DGGRS self)
{
   return DGGRS_getRefinementRatio(self);
}

LIB_EXPORT int DGGAL_DGGRS_getMaxDGGRSZoneLevel(DGGRS self)
{
   return DGGRS_getMaxDGGRSZoneLevel(self);
}

LIB_EXPORT void DGGAL_DGGRS_getZoneWGS84Centroid(DGGRS self, DGGRSZone zone, GeoPoint * outCentroid)
{
   DGGRS_getZoneWGS84Centroid(self, zone, outCentroid);
}

LIB_EXPORT int DGGAL_DGGRS_getZoneWGS84Vertices(DGGRS self, DGGRSZone zone, GeoPoint * outVertices)
{
   return DGGRS_getZoneWGS84Vertices(self, zone, outVertices);
}

LIB_EXPORT double DGGAL_DGGRS_getZoneArea(DGGRS self, DGGRSZone zone)
{
   return DGGRS_getZoneArea(self, zone);
}

LIB_EXPORT uint64 DGGAL_DGGRS_countSubZones(DGGRS self, DGGRSZone zone, int depth)
{
   return DGGRS_countSubZones(self, zone, depth);
}

LIB_EXPORT void DGGAL_DGGRS_getZoneTextID(DGGRS self, DGGRSZone zone, char outId[256])
{
   DGGRS_getZoneTextID(self, zone, outId);
}

LIB_EXPORT int DGGAL_DGGRS_getZoneParents(DGGRS self, DGGRSZone zone, DGGRSZone outParents[3])
{
   return DGGRS_getZoneParents(self, zone, outParents);
}

LIB_EXPORT int DGGAL_DGGRS_getZoneChildren(DGGRS self, DGGRSZone zone, DGGRSZone outChildren[13])
{
   return DGGRS_getZoneChildren(self, zone, outChildren);
}

LIB_EXPORT int DGGAL_DGGRS_getZoneNeighbors(DGGRS self, DGGRSZone zone, DGGRSZone outNeighbors[6], int outNbTypes[6])
{
   return DGGRS_getZoneNeighbors(self, zone, outNeighbors, outNbTypes);
}

LIB_EXPORT DGGRSZone DGGAL_DGGRS_getZoneCentroidParent(DGGRS self, DGGRSZone zone)
{
   return DGGRS_getZoneCentroidParent(self, zone);
}

LIB_EXPORT DGGRSZone DGGAL_DGGRS_getZoneCentroidChild(DGGRS self, DGGRSZone zone)
{
   return DGGRS_getZoneCentroidChild(self, zone);
}

LIB_EXPORT int DGGAL_DGGRS_isZoneCentroidChild(DGGRS self, DGGRSZone zone)
{
   return DGGRS_isZoneCentroidChild(self, zone);
}

LIB_EXPORT void DGGAL_DGGRS_getZoneWGS84Extent(DGGRS self, DGGRSZone zone, GeoExtent * outExtent)
{
   return DGGRS_getZoneWGS84Extent(self, zone, outExtent);
}

LIB_EXPORT T(Array, DGGRSZone) DGGAL_DGGRS_listZones(DGGRS self, int level, const GeoExtent * bbox)
{
   return DGGRS_listZones(self, level, bbox);
}

LIB_EXPORT T(Array, GeoPoint) DGGAL_DGGRS_getZoneRefinedWGS84Vertices(DGGRS self, DGGRSZone zone, int refinement)
{
   return DGGRS_getZoneRefinedWGS84Vertices(self, zone, refinement);
}

LIB_EXPORT T(Array, DGGRSZone) DGGAL_DGGRS_getSubZones(DGGRS self, DGGRSZone zone, int depth)
{
   return DGGRS_getSubZones(self, zone, depth);
}

LIB_EXPORT DGGRSZone DGGAL_DGGRS_getZoneFromWGS84Centroid(DGGRS self, int level, const GeoPoint * point)
{
   return DGGRS_getZoneFromWGS84Centroid(self, level, point);
}

LIB_EXPORT uint64 DGGAL_DGGRS_countZones(DGGRS self, int level)
{
   return DGGRS_countZones(self, level);
}

LIB_EXPORT DGGRSZone DGGAL_DGGRS_getFirstSubZone(DGGRS self, DGGRSZone parent, int relativeDepth)
{
   return DGGRS_getFirstSubZone(self, parent, relativeDepth);
}

LIB_EXPORT int DGGAL_DGGRS_getIndexMaxDepth(DGGRS self)
{
   return DGGRS_getIndexMaxDepth(self);
}

LIB_EXPORT int DGGAL_DGGRS_getMaxChildren(DGGRS self)
{
   return DGGRS_getMaxChildren(self);
}

LIB_EXPORT int DGGAL_DGGRS_getMaxNeighbors(DGGRS self)
{
   return DGGRS_getMaxNeighbors(self);
}

LIB_EXPORT int DGGAL_DGGRS_getMaxParents(DGGRS self)
{
   return DGGRS_getMaxParents(self);
}

LIB_EXPORT DGGRSZone DGGAL_DGGRS_getSubZoneAtIndex(DGGRS self, DGGRSZone parent, int relativeDepth, int64 index)
{
   return DGGRS_getSubZoneAtIndex(self, parent, relativeDepth, index);
}

LIB_EXPORT int64 DGGAL_DGGRS_getSubZoneIndex(DGGRS self, DGGRSZone parent, DGGRSZone subZone)
{
   return DGGRS_getSubZoneIndex(self, parent, subZone);
}

LIB_EXPORT T(Array, Pointd) DGGAL_DGGRS_getSubZoneCRSCentroids(DGGRS self, DGGRSZone parent, CRS crs, int relativeDepth)
{
   return DGGRS_getSubZoneCRSCentroids(self, parent, crs, relativeDepth);
}

LIB_EXPORT T(Array, GeoPoint) DGGAL_DGGRS_getSubZoneWGS84Centroids(DGGRS self, DGGRSZone parent, int relativeDepth)
{
   return DGGRS_getSubZoneWGS84Centroids(self, parent, relativeDepth);
}

LIB_EXPORT int DGGAL_DGGRS_getZoneCRSVertices(DGGRS self, DGGRSZone zone, CRS crs, Pointd outVertices[6])
{
   return DGGRS_getZoneCRSVertices(self, zone, crs, outVertices);
}

LIB_EXPORT T(Array, Pointd) DGGAL_DGGRS_getZoneRefinedCRSVertices(DGGRS self, DGGRSZone zone, CRS crs, int refinement)
{
   return DGGRS_getZoneRefinedCRSVertices(self, zone, crs, refinement);
}

LIB_EXPORT void DGGAL_DGGRS_getZoneCRSCentroid(DGGRS self, DGGRSZone zone, CRS crs, Pointd * outCentroid)
{
   return DGGRS_getZoneCRSCentroid(self, zone, crs, outCentroid);
}

LIB_EXPORT void DGGAL_DGGRS_getZoneCRSExtent(DGGRS self, DGGRSZone zone, CRS crs, CRSExtent * outExtent)
{
   return DGGRS_getZoneCRSExtent(self, zone, crs, outExtent);
}

LIB_EXPORT void DGGAL_DGGRS_compactZones(DGGRS self, T(Array, DGGRSZone) zones)
{
   return DGGRS_compactZones(self, zones);
}

// Non-virtual methods
LIB_EXPORT int DGGAL_DGGRS_get64KDepth(DGGRS self)
{
   return DGGRS_get64KDepth(self);
}

LIB_EXPORT int DGGAL_DGGRS_getMaxDepth(DGGRS self)
{
   return DGGRS_getMaxDepth(self);
}

LIB_EXPORT int DGGAL_DGGRS_areZonesNeighbors(DGGRS self, DGGRSZone a, DGGRSZone b)
{
   return DGGRS_areZonesNeighbors(self, a, b);
}

LIB_EXPORT int DGGAL_DGGRS_areZonesSiblings(DGGRS self, DGGRSZone a, DGGRSZone b)
{
   return DGGRS_areZonesSiblings(self, a, b);
}

LIB_EXPORT int DGGAL_DGGRS_doZonesOverlap(DGGRS self, DGGRSZone a, DGGRSZone b)
{
   return DGGRS_doZonesOverlap(self, a, b);
}

LIB_EXPORT int DGGAL_DGGRS_doesZoneContain(DGGRS self, DGGRSZone hayStack, DGGRSZone needle)
{
   return DGGRS_doesZoneContain(self, hayStack, needle);
}

LIB_EXPORT int DGGAL_DGGRS_isZoneAncestorOf(DGGRS self, DGGRSZone ancestor, DGGRSZone descendant, int maxDepth)
{
   return DGGRS_isZoneAncestorOf(self, ancestor, descendant, maxDepth);
}

LIB_EXPORT int DGGAL_DGGRS_isZoneContainedIn(DGGRS self, DGGRSZone needle, DGGRSZone hayStack)
{
   return DGGRS_isZoneContainedIn(self, hayStack, needle);
}

LIB_EXPORT int DGGAL_DGGRS_isZoneDescendantOf(DGGRS self, DGGRSZone descendant, DGGRSZone ancestor, int maxDepth)
{
   return DGGRS_isZoneDescendantOf(self, ancestor, descendant, maxDepth);
}

LIB_EXPORT int DGGAL_DGGRS_isZoneImmediateChildOf(DGGRS self, DGGRSZone child, DGGRSZone parent)
{
   return DGGRS_isZoneImmediateChildOf(self, child, parent);
}

LIB_EXPORT int DGGAL_DGGRS_isZoneImmediateParentOf(DGGRS self, DGGRSZone parent, DGGRSZone child)
{
   return DGGRS_isZoneImmediateParentOf(self, child, parent);
}

LIB_EXPORT int DGGAL_DGGRS_zoneHasSubZone(DGGRS self, DGGRSZone hayStack, DGGRSZone needle)
{
   return DGGRS_zoneHasSubZone(self, hayStack, needle);
}

LIB_EXPORT int DGGAL_DGGRS_getLevelFromMetersPerSubZone(DGGRS self, double physicalMetersPerSubZone, int relativeDepth)
{
   return DGGRS_getLevelFromMetersPerSubZone(self, physicalMetersPerSubZone, relativeDepth);
}

LIB_EXPORT int DGGAL_DGGRS_getLevelFromPixelsAndExtent(DGGRS self, const GeoExtent * extent, int width, int height, int relativeDepth)
{
   Point pixels = { width, height };
   return DGGRS_getLevelFromPixelsAndExtent(self, extent, &pixels, relativeDepth);
}

LIB_EXPORT int DGGAL_DGGRS_getLevelFromRefZoneArea(DGGRS self, double metersSquared)
{
   return DGGRS_getLevelFromRefZoneArea(self, metersSquared);
}

LIB_EXPORT int DGGAL_DGGRS_getLevelFromScaleDenominator(DGGRS self, double scaleDenominator, int relativeDepth, double mmPerPixel)
{
   return DGGRS_getLevelFromScaleDenominator(self, scaleDenominator, relativeDepth, mmPerPixel);
}

LIB_EXPORT double DGGAL_DGGRS_getMetersPerSubZoneFromLevel(DGGRS self, int parentLevel, int relativeDepth)
{
   return DGGRS_getMetersPerSubZoneFromLevel(self, parentLevel, relativeDepth);
}

LIB_EXPORT double DGGAL_DGGRS_getRefZoneArea(DGGRS self, int level)
{
   return DGGRS_getRefZoneArea(self, level);
}

LIB_EXPORT double DGGAL_DGGRS_getScaleDenominatorFromLevel(DGGRS self, int parentLevel, int relativeDepth, double mmPerPixel)
{
   return DGGRS_getScaleDenominatorFromLevel(self, parentLevel, relativeDepth, mmPerPixel);
}
