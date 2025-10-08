#ifndef __DGGAL_C_H__
#define __DGGAL_C_H__

#include <stdint.h>

#if !defined(BINDINGS_SHARED)
#define LIB_EXPORT
#define LIB_IMPORT
#elif defined(__WIN32__)
#define LIB_EXPORT __attribute__((dllexport)) __attribute__ ((visibility("default")))
#define LIB_IMPORT __attribute__((dllimport))
#else
#define LIB_EXPORT __attribute__ ((visibility("default")))
#define LIB_IMPORT
#endif

struct DGGALModule;
typedef struct DGGALModule * DGGALModule;

// DGGAL Initialization -- Setup a single DGGAL module per application (unsafe to call multiple times)
LIB_IMPORT DGGALModule DGGAL_init();
LIB_IMPORT void DGGAL_terminate(DGGALModule mDGGAL);

// Data Types
struct DGGRS;
typedef struct DGGRS * DGGRS;
typedef uint64_t DGGRSZone;
static const uint64_t nullZone = 0xFFFFFFFFFFFFFFFFLL;

typedef char * String;
typedef const char * constString;
typedef uint64_t CRS;
enum CRSRegistry
{
   epsg = 0x0,
   ogc = 0x1
};
typedef enum CRSRegistry CRSRegistry;
LIB_IMPORT CRSRegistry DGGAL_CRS(CRSRegistry registry, unsigned int code, int h);

struct Pointd
{
   double x, y;
};
typedef struct Pointd Pointd;
struct CRSExtent
{
   CRS crs;
   Pointd tl;
   Pointd br;
};
typedef struct CRSExtent CRSExtent;
typedef double Radians;
struct GeoPoint
{
   Radians lat, lon;
};
typedef struct GeoPoint GeoPoint;
struct GeoExtent
{
   GeoPoint ll, ur;
};
typedef struct GeoExtent GeoExtent;

// Dynamic Arrays
struct Array_GeoPoint;
typedef struct Array_GeoPoint * Array_GeoPoint;
LIB_IMPORT int DGGAL_Array_GeoPoint_getCount(const Array_GeoPoint self);
LIB_IMPORT const GeoPoint * DGGAL_Array_GeoPoint_getPointer(const Array_GeoPoint self);
LIB_IMPORT void DGGAL_Array_GeoPoint_delete(Array_GeoPoint self);

struct Array_Pointd;
typedef struct Array_Pointd * Array_Pointd;
LIB_IMPORT int DGGAL_Array_Pointd_getCount(const Array_Pointd self);
LIB_IMPORT const Pointd * DGGAL_Array_Pointd_getPointer(const Array_Pointd self);
LIB_IMPORT void DGGAL_Array_Pointd_delete(Array_Pointd self);

struct Array_DGGRSZone;
typedef struct Array_DGGRSZone * Array_DGGRSZone;
LIB_IMPORT Array_DGGRSZone DGGAL_Array_DGGRSZone_new(unsigned int size);
LIB_IMPORT int DGGAL_Array_DGGRSZone_getCount(const Array_DGGRSZone self);
LIB_IMPORT DGGRSZone * DGGAL_Array_DGGRSZone_getPointer(const Array_DGGRSZone self);
LIB_IMPORT void DGGAL_Array_DGGRSZone_delete(Array_DGGRSZone self);

// DGGRS Instantiation -- Setup and re-use single instance per DGGRS (e.g., IVEA3H, HEALPix, GNOSISGlobalGrid)
LIB_IMPORT const char ** DGGAL_DGGRS_list(unsigned int * count);
LIB_IMPORT DGGRS DGGAL_DGGRS_new(DGGALModule mDGGAL, constString name);
LIB_IMPORT void DGGAL_DGGRS_delete(DGGRS self);

// DGGRS Class -- All DGGRS methods can safely be called on the same DGGRS instance from multiple threads
LIB_IMPORT DGGRSZone DGGAL_DGGRS_getZoneFromTextID(const DGGRS self, constString zoneID);
LIB_IMPORT int DGGAL_DGGRS_getZoneLevel(const DGGRS self, DGGRSZone zone);
LIB_IMPORT int DGGAL_DGGRS_countZoneEdges(const DGGRS self, DGGRSZone zone);
LIB_IMPORT int DGGAL_DGGRS_getRefinementRatio(const DGGRS self);
LIB_IMPORT int DGGAL_DGGRS_getMaxDGGRSZoneLevel(const DGGRS self);
LIB_IMPORT void DGGAL_DGGRS_getZoneWGS84Centroid(const DGGRS self, DGGRSZone zone, GeoPoint * outCentroid);
LIB_IMPORT int DGGAL_DGGRS_getZoneWGS84Vertices(const DGGRS self, DGGRSZone zone, GeoPoint outVertices[6]);
LIB_EXPORT int DGGAL_DGGRS_getZoneCRSVertices(DGGRS self, DGGRSZone zone, CRS crs, Pointd outVertices[6]);
LIB_IMPORT double DGGAL_DGGRS_getZoneArea(const DGGRS self, DGGRSZone zone);
LIB_IMPORT uint64_t DGGAL_DGGRS_countSubZones(const DGGRS self, DGGRSZone zone, int depth);
LIB_IMPORT void DGGAL_DGGRS_getZoneTextID(const DGGRS self, DGGRSZone zone, char outId[256]);
LIB_IMPORT int DGGAL_DGGRS_getZoneParents(const DGGRS self, DGGRSZone zone, DGGRSZone outParents[3]);
LIB_IMPORT int DGGAL_DGGRS_getZoneChildren(const DGGRS self, DGGRSZone zone, DGGRSZone outChildren[13]);
LIB_IMPORT int DGGAL_DGGRS_getZoneNeighbors(const DGGRS self, DGGRSZone zone, DGGRSZone outNeighbors[6], int outNbTypes[6]);
LIB_IMPORT DGGRSZone DGGAL_DGGRS_getZoneCentroidParent(const DGGRS self, DGGRSZone zone);
LIB_IMPORT DGGRSZone DGGAL_DGGRS_getZoneCentroidChild(const DGGRS self, DGGRSZone zone);
LIB_IMPORT int DGGAL_DGGRS_isZoneCentroidChild(const DGGRS self, DGGRSZone zone);
LIB_IMPORT void DGGAL_DGGRS_getZoneWGS84Extent(const DGGRS self, DGGRSZone zone, GeoExtent * outExtent);
LIB_IMPORT Array_DGGRSZone DGGAL_DGGRS_listZones(const DGGRS self, int level, const GeoExtent * bbox);
LIB_IMPORT Array_GeoPoint DGGAL_DGGRS_getZoneRefinedWGS84Vertices(const DGGRS self, DGGRSZone zone, int refinement);
LIB_IMPORT Array_DGGRSZone DGGAL_DGGRS_getSubZones(const DGGRS self, DGGRSZone zone, int depth);
LIB_IMPORT DGGRSZone DGGAL_DGGRS_getZoneFromWGS84Centroid(const DGGRS self, int level, const GeoPoint * point);
LIB_IMPORT uint64_t DGGAL_DGGRS_countZones(const DGGRS self, int level);
LIB_IMPORT DGGRSZone DGGAL_DGGRS_getFirstSubZone(const DGGRS self, DGGRSZone parent, int relativeDepth);
LIB_IMPORT int DGGAL_DGGRS_getIndexMaxDepth(const DGGRS self);
LIB_IMPORT int DGGAL_DGGRS_getMaxChildren(const DGGRS self);
LIB_IMPORT int DGGAL_DGGRS_getMaxNeighbors(const DGGRS self);
LIB_IMPORT int DGGAL_DGGRS_getMaxParents(const DGGRS self);
LIB_IMPORT DGGRSZone DGGAL_DGGRS_getSubZoneAtIndex(const DGGRS self, DGGRSZone parent, int relativeDepth, int64_t index);
LIB_IMPORT int64_t DGGAL_DGGRS_getSubZoneIndex(const DGGRS self, DGGRSZone parent, DGGRSZone subZone);
LIB_IMPORT Array_Pointd DGGAL_DGGRS_getSubZoneCRSCentroids(const DGGRS self, DGGRSZone parent, CRS crs, int relativeDepth);
LIB_IMPORT Array_GeoPoint DGGAL_DGGRS_getSubZoneWGS84Centroids(const DGGRS self, DGGRSZone parent, int relativeDepth);
LIB_IMPORT Array_Pointd DGGAL_DGGRS_getZoneRefinedCRSVertices(const DGGRS self, DGGRSZone zone, CRS crs, int refinement);
LIB_IMPORT void DGGAL_DGGRS_getZoneCRSCentroid(const DGGRS self, DGGRSZone zone, CRS crs, Pointd * outCentroid);
LIB_IMPORT void DGGAL_DGGRS_getZoneCRSExtent(const DGGRS self, DGGRSZone zone, CRS crs, CRSExtent * outExtent);
LIB_IMPORT void DGGAL_DGGRS_compactZones(const DGGRS self, Array_DGGRSZone zones);
LIB_IMPORT int DGGAL_DGGRS_get64KDepth(const DGGRS self);
LIB_IMPORT int DGGAL_DGGRS_getMaxDepth(const DGGRS self);
LIB_IMPORT int DGGAL_DGGRS_areZonesNeighbors(const DGGRS self, DGGRSZone a, DGGRSZone b);
LIB_IMPORT int DGGAL_DGGRS_areZonesSiblings(const DGGRS self, DGGRSZone a, DGGRSZone b);
LIB_IMPORT int DGGAL_DGGRS_doZonesOverlap(const DGGRS self, DGGRSZone a, DGGRSZone b);
LIB_IMPORT int DGGAL_DGGRS_doesZoneContain(const DGGRS self, DGGRSZone hayStack, DGGRSZone needle);
LIB_IMPORT int DGGAL_DGGRS_isZoneAncestorOf(const DGGRS self, DGGRSZone ancestor, DGGRSZone descendant, int maxDepth);
LIB_IMPORT int DGGAL_DGGRS_isZoneContainedIn(const DGGRS self, DGGRSZone needle, DGGRSZone hayStack);
LIB_IMPORT int DGGAL_DGGRS_isZoneDescendantOf(const DGGRS self, DGGRSZone descendant, DGGRSZone ancestor, int maxDepth);
LIB_IMPORT int DGGAL_DGGRS_isZoneImmediateChildOf(const DGGRS self, DGGRSZone child, DGGRSZone parent);
LIB_IMPORT int DGGAL_DGGRS_isZoneImmediateParentOf(const DGGRS self, DGGRSZone parent, DGGRSZone child);
LIB_IMPORT int DGGAL_DGGRS_zoneHasSubZone(const DGGRS self, DGGRSZone hayStack, DGGRSZone needle);
LIB_IMPORT int DGGAL_DGGRS_getLevelFromMetersPerSubZone(const DGGRS self, double physicalMetersPerSubZone, int relativeDepth);
LIB_IMPORT int DGGAL_DGGRS_getLevelFromPixelsAndExtent(const DGGRS self, const GeoExtent * extent, int width, int height, int relativeDepth);
LIB_IMPORT int DGGAL_DGGRS_getLevelFromRefZoneArea(const DGGRS self, double metersSquared);
LIB_IMPORT int DGGAL_DGGRS_getLevelFromScaleDenominator(const DGGRS self, double scaleDenominator, int relativeDepth, double mmPerPixel);
LIB_IMPORT double DGGAL_DGGRS_getMetersPerSubZoneFromLevel(const DGGRS self, int parentLevel, int relativeDepth);
LIB_IMPORT double DGGAL_DGGRS_getRefZoneArea(const DGGRS self, int level);
LIB_IMPORT double DGGAL_DGGRS_getScaleDenominatorFromLevel(const DGGRS self, int parentLevel, int relativeDepth, double mmPerPixel);

#endif // __DGGAL_C_H__
