//////////////////////////////////////////////////////////////////////////////// ////////////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////////////
////                                                                        //// ////////////////////////
////    dggal Module                                                        //// ////////////////////////
////                                                                        //// ////////////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////////////


#if !defined(__DGGAL_H__)
#define __DGGAL_H__

#ifndef CPP11
#if defined(__cplusplus) && __cplusplus >= 201103L
#define CPP11 1
#else
#define CPP11 0
#endif
#endif

#ifdef __cplusplus

extern "C" {

#endif

////////////////////////////////////////////////////////////// includes //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////

#include "ecrt.h"


#if !defined(DGGAL_MODULE_NAME)
#define DGGAL_MODULE_NAME "dggal"
#endif

#undef THIS_LIB_IMPORT
#ifdef DGGAL_EXPORT
#define THIS_LIB_IMPORT LIB_EXPORT
#elif defined(BINDINGS_SHARED)
#define THIS_LIB_IMPORT LIB_IMPORT
#else
#define THIS_LIB_IMPORT
#endif



// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// ////////    //////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////


// start -- moved backwards outputs
typedef uint64 C(CRS);
struct C(CRSExtent)
{
   C(CRS) crs;
   C(Pointd) tl;
   C(Pointd) br;
};
struct C(GeoPoint)
{
   C(Angle) lat;
   C(Angle) lon;
};
struct C(Vector3D)
{
   double x;
   double y;
   double z;
};
typedef C(Instance) C(RI5x6Projection);
typedef C(RI5x6Projection) C(BarycentricSphericalTriAreaProjection);
typedef struct C(CRSExtent) C(CRSExtent);
typedef C(Instance) C(DGGRS);
typedef uint64 C(DGGRSZone);
typedef C(Instance) C(DGGSJSON);
typedef C(Instance) C(DGGSJSONGrid);
typedef C(Instance) C(DGGSJSONShape);
typedef struct C(GeoExtent) C(GeoExtent);
typedef struct C(GeoPoint) C(GeoPoint);
typedef C(Instance) C(HEALPixProjection);
typedef uint64 C(I3HZone);
typedef uint64 C(I7HZone);
typedef uint64 C(I9RZone);
typedef C(Instance) C(JSONSchema);
#if CPP11
enum C(JSONSchemaType) : int
#else
typedef int C(JSONSchemaType);
enum C(JSONSchemaType)
#endif
{
   JSONSchemaType_unset = 0x0,
   JSONSchemaType_array = 0x1,
   JSONSchemaType_boolean = 0x2,
   JSONSchemaType_integer = 0x3,
   JSONSchemaType_null = 0x4,
   JSONSchemaType_number = 0x5,
   JSONSchemaType_object = 0x6,
   JSONSchemaType_string = 0x7
};

typedef struct C(Plane) C(Plane);
typedef struct C(Quaternion) C(Quaternion);
typedef C(DGGRS) C(RhombicIcosahedral7H);
typedef C(RhombicIcosahedral7H) C(RI7H_Z7);
typedef C(DGGRS) C(RhombicIcosahedral3H);
typedef C(DGGRS) C(RhombicIcosahedral4R);
typedef C(DGGRS) C(RhombicIcosahedral9R);
typedef C(RI5x6Projection) C(SliceAndDiceGreatCircleIcosahedralProjection);
typedef struct C(Vector3D) C(Vector3D);
typedef uint64 C(Z7Zone);
// end -- moved backwards outputs
#define AUTH_ORDER (6)

#define nullZone (0xFFFFFFFFFFFFFFFFLL)

#define wgs84InvFlattening (298.257223563)

#define wgs84Major (((C(Distance))(6378137.0)))

#define wgs84Minor (wgs84Major - (wgs84Major / wgs84InvFlattening))

#define wholeWorld (__extension__ ({ C(GeoExtent) __simpleStruct0 = {  { -1.5707963267948966, -3.1415926535897931 }, { 1.5707963267948966, 3.1415926535897931 } };  __simpleStruct0; }))

typedef C(RhombicIcosahedral3H) C(BCTA3H);
#if CPP11
enum C(CRSRegistry) : int
#else
typedef int C(CRSRegistry);
enum C(CRSRegistry)
#endif
{
   CRSRegistry_epsg = 0x0,
   CRSRegistry_ogc = 0x1
};

typedef C(Instance) C(DGGSJSONDepth);
typedef C(Instance) C(DGGSJSONDimension);
typedef uint64 C(GGGZone);
typedef C(DGGRS) C(GNOSISGlobalGrid);
typedef C(RhombicIcosahedral3H) C(GPP3H);
typedef C(BarycentricSphericalTriAreaProjection) C(GoldbergPolyhedraProjection);
typedef C(DGGRS) C(HEALPix);
typedef uint64 C(HPZone);
#if CPP11
enum C(I3HNeighbor) : int
#else
typedef int C(I3HNeighbor);
enum C(I3HNeighbor)
#endif
{
   I3HNeighbor_top = 0x0,
   I3HNeighbor_bottom = 0x1,
   I3HNeighbor_left = 0x2,
   I3HNeighbor_right = 0x3,
   I3HNeighbor_topLeft = 0x4,
   I3HNeighbor_topRight = 0x5,
   I3HNeighbor_bottomLeft = 0x6,
   I3HNeighbor_bottomRight = 0x7
};

typedef uint64 C(I4RZone);
typedef C(RhombicIcosahedral3H) C(ISEA3H);
typedef C(RhombicIcosahedral4R) C(ISEA4R);
typedef C(RhombicIcosahedral7H) C(ISEA7H);
typedef C(RI7H_Z7) C(ISEA7H_Z7);
typedef C(RhombicIcosahedral9R) C(ISEA9R);
typedef C(SliceAndDiceGreatCircleIcosahedralProjection) C(ISEAProjection);
typedef C(RhombicIcosahedral3H) C(IVEA3H);
typedef C(RhombicIcosahedral4R) C(IVEA4R);
typedef C(RhombicIcosahedral7H) C(IVEA7H);
typedef C(RI7H_Z7) C(IVEA7H_Z7);
typedef C(RhombicIcosahedral9R) C(IVEA9R);
typedef C(SliceAndDiceGreatCircleIcosahedralProjection) C(IVEAProjection);
typedef uint64 C(RHPZone);
typedef C(RhombicIcosahedral3H) C(RTEA3H);
typedef C(RhombicIcosahedral4R) C(RTEA4R);
typedef C(RhombicIcosahedral7H) C(RTEA7H);
typedef C(RI7H_Z7) C(RTEA7H_Z7);
typedef C(RhombicIcosahedral9R) C(RTEA9R);
typedef C(SliceAndDiceGreatCircleIcosahedralProjection) C(RTEAProjection);
#if CPP11
enum C(VGCRadialVertex) : int
#else
typedef int C(VGCRadialVertex);
enum C(VGCRadialVertex)
#endif
{
   VGCRadialVertex_isea = 0x0,
   VGCRadialVertex_ivea = 0x1,
   VGCRadialVertex_rtea = 0x2
};

typedef C(DGGRS) C(rHEALPix);
typedef C(HEALPixProjection) C(rHEALPixProjection);
typedef C(Array) T(Array, Pointd);
typedef C(Array) T(Array, JSONSchema);
typedef C(Map) T(Map, String, JSONSchema);
typedef C(Array) T(Array, String);
typedef C(Array) T(Array, FieldValue);
typedef C(Array) T(Array, double);
typedef C(Map) T(Map, String, int);
typedef C(Array) T(Array, DGGSJSONDepth);
typedef C(Map) T(Map, String, template_Array_DGGSJSONDepth);
typedef C(Array) T(Array, DGGSJSONDimension);
typedef C(Array) T(Array, int);
typedef C(Array) T(Array, DGGRSZone);
typedef C(Array) T(Array, GeoPoint);
#define CRS_registry_SHIFT                               0
#define CRS_registry_MASK                                0x3FFFFFFF
#define CRS_registry(x)                                  ((((C(CRS))(x)) & CRS_registry_MASK) >> CRS_registry_SHIFT)
#define CRS_SET_registry(x, registry)                           (x) = ((C(CRS))(x) & ~((C(CRS))CRS_registry_MASK)) | (((C(CRS))(registry)) << CRS_registry_SHIFT)
#define CRS_crsID_SHIFT                                  30
#define CRS_crsID_MASK                                   0x3FFFFFFFC0000000LL
#define CRS_crsID(x)                                     ((((C(CRS))(x)) & CRS_crsID_MASK) >> CRS_crsID_SHIFT)
#define CRS_SET_crsID(x, crsID)                              (x) = ((C(CRS))(x) & ~((C(CRS))CRS_crsID_MASK)) | (((C(CRS))(crsID)) << CRS_crsID_SHIFT)
#define CRS_h_SHIFT                                      62
#define CRS_h_MASK                                       0x4000000000000000LL
#define CRS_h(x)                                         ((((C(CRS))(x)) & CRS_h_MASK) >> CRS_h_SHIFT)
#define CRS_SET_h(x, h)                                  (x) = ((C(CRS))(x) & ~((C(CRS))CRS_h_MASK)) | (((C(CRS))(h)) << CRS_h_SHIFT)
#define CRS(registry, crsID, h)                                     (((((C(CRS))(registry)) << CRS_registry_SHIFT) | ((C(CRS))(crsID)) << CRS_crsID_SHIFT) | ((C(CRS))(h)) << CRS_h_SHIFT)


extern THIS_LIB_IMPORT C(bool) (* DGGRS_areZonesNeighbors)(C(DGGRS) __this, C(DGGRSZone) a, C(DGGRSZone) b);

extern THIS_LIB_IMPORT C(bool) (* DGGRS_areZonesSiblings)(C(DGGRS) __this, C(DGGRSZone) a, C(DGGRSZone) b);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, compactZones);
// void DGGRS_compactZones(C(DGGRS) __i, C(Array) zones);
#define DGGRS_compactZones(__i, zones) \
   VMETHOD(CO(DGGRS), DGGRS, compactZones, __i, void, \
      C(DGGRS) _ARG C(Array), \
      __i _ARG zones)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, compactZones);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, countSubZones);
// uint64 DGGRS_countSubZones(C(DGGRS) __i, C(DGGRSZone) zone, int depth);
#define DGGRS_countSubZones(__i, zone, depth) \
   VMETHOD(CO(DGGRS), DGGRS, countSubZones, __i, uint64, \
      C(DGGRS) _ARG C(DGGRSZone) _ARG int, \
      __i _ARG zone _ARG depth)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, countSubZones);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, countZoneEdges);
// int DGGRS_countZoneEdges(C(DGGRS) __i, C(DGGRSZone) zone);
#define DGGRS_countZoneEdges(__i, zone) \
   VMETHOD(CO(DGGRS), DGGRS, countZoneEdges, __i, int, \
      C(DGGRS) _ARG C(DGGRSZone), \
      __i _ARG zone)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, countZoneEdges);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, countZones);
// uint64 DGGRS_countZones(C(DGGRS) __i, int level);
#define DGGRS_countZones(__i, level) \
   VMETHOD(CO(DGGRS), DGGRS, countZones, __i, uint64, \
      C(DGGRS) _ARG int, \
      __i _ARG level)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, countZones);

extern THIS_LIB_IMPORT C(bool) (* DGGRS_doZonesOverlap)(C(DGGRS) __this, C(DGGRSZone) a, C(DGGRSZone) b);

extern THIS_LIB_IMPORT C(bool) (* DGGRS_doesZoneContain)(C(DGGRS) __this, C(DGGRSZone) hayStack, C(DGGRSZone) needle);

extern THIS_LIB_IMPORT int (* DGGRS_get64KDepth)(C(DGGRS) __this);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getFirstSubZone);
// C(DGGRSZone) DGGRS_getFirstSubZone(C(DGGRS) __i, C(DGGRSZone) zone, int relativeDepth);
#define DGGRS_getFirstSubZone(__i, zone, relativeDepth) \
   VMETHOD(CO(DGGRS), DGGRS, getFirstSubZone, __i, C(DGGRSZone), \
      C(DGGRS) _ARG C(DGGRSZone) _ARG int, \
      __i _ARG zone _ARG relativeDepth)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getFirstSubZone);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getIndexMaxDepth);
// int DGGRS_getIndexMaxDepth(C(DGGRS) __i);
#define DGGRS_getIndexMaxDepth(__i) \
   VMETHOD(CO(DGGRS), DGGRS, getIndexMaxDepth, __i, int, \
      C(DGGRS), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getIndexMaxDepth);

extern THIS_LIB_IMPORT int (* DGGRS_getLevelFromMetersPerSubZone)(C(DGGRS) __this, double physicalMetersPerSubZone, int relativeDepth);

extern THIS_LIB_IMPORT int (* DGGRS_getLevelFromPixelsAndExtent)(C(DGGRS) __this, const C(GeoExtent) * extent, const C(Point) * pixels, int relativeDepth);

extern THIS_LIB_IMPORT int (* DGGRS_getLevelFromRefZoneArea)(C(DGGRS) __this, double metersSquared);

extern THIS_LIB_IMPORT int (* DGGRS_getLevelFromScaleDenominator)(C(DGGRS) __this, double scaleDenominator, int relativeDepth, double mmPerPixel);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getMaxChildren);
// int DGGRS_getMaxChildren(C(DGGRS) __i);
#define DGGRS_getMaxChildren(__i) \
   VMETHOD(CO(DGGRS), DGGRS, getMaxChildren, __i, int, \
      C(DGGRS), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getMaxChildren);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getMaxDGGRSZoneLevel);
// int DGGRS_getMaxDGGRSZoneLevel(C(DGGRS) __i);
#define DGGRS_getMaxDGGRSZoneLevel(__i) \
   VMETHOD(CO(DGGRS), DGGRS, getMaxDGGRSZoneLevel, __i, int, \
      C(DGGRS), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getMaxDGGRSZoneLevel);

extern THIS_LIB_IMPORT int (* DGGRS_getMaxDepth)(C(DGGRS) __this);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getMaxNeighbors);
// int DGGRS_getMaxNeighbors(C(DGGRS) __i);
#define DGGRS_getMaxNeighbors(__i) \
   VMETHOD(CO(DGGRS), DGGRS, getMaxNeighbors, __i, int, \
      C(DGGRS), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getMaxNeighbors);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getMaxParents);
// int DGGRS_getMaxParents(C(DGGRS) __i);
#define DGGRS_getMaxParents(__i) \
   VMETHOD(CO(DGGRS), DGGRS, getMaxParents, __i, int, \
      C(DGGRS), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getMaxParents);

extern THIS_LIB_IMPORT double (* DGGRS_getMetersPerSubZoneFromLevel)(C(DGGRS) __this, int parentLevel, int relativeDepth);

extern THIS_LIB_IMPORT double (* DGGRS_getRefZoneArea)(C(DGGRS) __this, int level);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getRefinementRatio);
// int DGGRS_getRefinementRatio(C(DGGRS) __i);
#define DGGRS_getRefinementRatio(__i) \
   VMETHOD(CO(DGGRS), DGGRS, getRefinementRatio, __i, int, \
      C(DGGRS), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getRefinementRatio);

extern THIS_LIB_IMPORT double (* DGGRS_getScaleDenominatorFromLevel)(C(DGGRS) __this, int parentLevel, int relativeDepth, double mmPerPixel);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getSubZoneAtIndex);
// C(DGGRSZone) DGGRS_getSubZoneAtIndex(C(DGGRS) __i, C(DGGRSZone) parent, int relativeDepth, int64 index);
#define DGGRS_getSubZoneAtIndex(__i, parent, relativeDepth, index) \
   VMETHOD(CO(DGGRS), DGGRS, getSubZoneAtIndex, __i, C(DGGRSZone), \
      C(DGGRS) _ARG C(DGGRSZone) _ARG int _ARG int64, \
      __i _ARG parent _ARG relativeDepth _ARG index)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getSubZoneAtIndex);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getSubZoneCRSCentroids);
// C(Array) DGGRS_getSubZoneCRSCentroids(C(DGGRS) __i, C(DGGRSZone) parent, C(CRS) crs, int relativeDepth);
#define DGGRS_getSubZoneCRSCentroids(__i, parent, crs, relativeDepth) \
   VMETHOD(CO(DGGRS), DGGRS, getSubZoneCRSCentroids, __i, C(Array), \
      C(DGGRS) _ARG C(DGGRSZone) _ARG C(CRS) _ARG int, \
      __i _ARG parent _ARG crs _ARG relativeDepth)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getSubZoneCRSCentroids);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getSubZoneIndex);
// int64 DGGRS_getSubZoneIndex(C(DGGRS) __i, C(DGGRSZone) parent, C(DGGRSZone) subZone);
#define DGGRS_getSubZoneIndex(__i, parent, subZone) \
   VMETHOD(CO(DGGRS), DGGRS, getSubZoneIndex, __i, int64, \
      C(DGGRS) _ARG C(DGGRSZone) _ARG C(DGGRSZone), \
      __i _ARG parent _ARG subZone)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getSubZoneIndex);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getSubZoneWGS84Centroids);
// C(Array) DGGRS_getSubZoneWGS84Centroids(C(DGGRS) __i, C(DGGRSZone) parent, int relativeDepth);
#define DGGRS_getSubZoneWGS84Centroids(__i, parent, relativeDepth) \
   VMETHOD(CO(DGGRS), DGGRS, getSubZoneWGS84Centroids, __i, C(Array), \
      C(DGGRS) _ARG C(DGGRSZone) _ARG int, \
      __i _ARG parent _ARG relativeDepth)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getSubZoneWGS84Centroids);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getSubZones);
// C(Array) DGGRS_getSubZones(C(DGGRS) __i, C(DGGRSZone) parent, int relativeDepth);
#define DGGRS_getSubZones(__i, parent, relativeDepth) \
   VMETHOD(CO(DGGRS), DGGRS, getSubZones, __i, C(Array), \
      C(DGGRS) _ARG C(DGGRSZone) _ARG int, \
      __i _ARG parent _ARG relativeDepth)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getSubZones);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneArea);
// double DGGRS_getZoneArea(C(DGGRS) __i, C(DGGRSZone) zone);
#define DGGRS_getZoneArea(__i, zone) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneArea, __i, double, \
      C(DGGRS) _ARG C(DGGRSZone), \
      __i _ARG zone)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneArea);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneCRSCentroid);
// void DGGRS_getZoneCRSCentroid(C(DGGRS) __i, C(DGGRSZone) zone, C(CRS) crs, C(Pointd) * centroid);
#define DGGRS_getZoneCRSCentroid(__i, zone, crs, centroid) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneCRSCentroid, __i, void, \
      C(DGGRS) _ARG C(DGGRSZone) _ARG C(CRS) _ARG C(Pointd) *, \
      __i _ARG zone _ARG crs _ARG centroid)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneCRSCentroid);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneCRSExtent);
// void DGGRS_getZoneCRSExtent(C(DGGRS) __i, C(DGGRSZone) zone, C(CRS) crs, C(CRSExtent) * extent);
#define DGGRS_getZoneCRSExtent(__i, zone, crs, extent) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneCRSExtent, __i, void, \
      C(DGGRS) _ARG C(DGGRSZone) _ARG C(CRS) _ARG C(CRSExtent) *, \
      __i _ARG zone _ARG crs _ARG extent)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneCRSExtent);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneCRSVertices);
// int DGGRS_getZoneCRSVertices(C(DGGRS) __i, C(DGGRSZone) zone, C(CRS) crs, C(Pointd) * vertices);
#define DGGRS_getZoneCRSVertices(__i, zone, crs, vertices) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneCRSVertices, __i, int, \
      C(DGGRS) _ARG C(DGGRSZone) _ARG C(CRS) _ARG C(Pointd) *, \
      __i _ARG zone _ARG crs _ARG vertices)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneCRSVertices);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneCentroidChild);
// C(DGGRSZone) DGGRS_getZoneCentroidChild(C(DGGRS) __i, C(DGGRSZone) zone);
#define DGGRS_getZoneCentroidChild(__i, zone) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneCentroidChild, __i, C(DGGRSZone), \
      C(DGGRS) _ARG C(DGGRSZone), \
      __i _ARG zone)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneCentroidChild);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneCentroidParent);
// C(DGGRSZone) DGGRS_getZoneCentroidParent(C(DGGRS) __i, C(DGGRSZone) zone);
#define DGGRS_getZoneCentroidParent(__i, zone) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneCentroidParent, __i, C(DGGRSZone), \
      C(DGGRS) _ARG C(DGGRSZone), \
      __i _ARG zone)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneCentroidParent);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneChildren);
// int DGGRS_getZoneChildren(C(DGGRS) __i, C(DGGRSZone) zone, C(DGGRSZone) * children);
#define DGGRS_getZoneChildren(__i, zone, children) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneChildren, __i, int, \
      C(DGGRS) _ARG C(DGGRSZone) _ARG C(DGGRSZone) *, \
      __i _ARG zone _ARG children)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneChildren);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneFromCRSCentroid);
// C(DGGRSZone) DGGRS_getZoneFromCRSCentroid(C(DGGRS) __i, int level, C(CRS) crs, const C(Pointd) * centroid);
#define DGGRS_getZoneFromCRSCentroid(__i, level, crs, centroid) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneFromCRSCentroid, __i, C(DGGRSZone), \
      C(DGGRS) _ARG int _ARG C(CRS) _ARG const C(Pointd) *, \
      __i _ARG level _ARG crs _ARG centroid)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneFromCRSCentroid);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneFromTextID);
// C(DGGRSZone) DGGRS_getZoneFromTextID(C(DGGRS) __i, constString zoneID);
#define DGGRS_getZoneFromTextID(__i, zoneID) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneFromTextID, __i, C(DGGRSZone), \
      C(DGGRS) _ARG constString, \
      __i _ARG zoneID)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneFromTextID);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneFromWGS84Centroid);
// C(DGGRSZone) DGGRS_getZoneFromWGS84Centroid(C(DGGRS) __i, int level, const C(GeoPoint) * centroid);
#define DGGRS_getZoneFromWGS84Centroid(__i, level, centroid) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneFromWGS84Centroid, __i, C(DGGRSZone), \
      C(DGGRS) _ARG int _ARG const C(GeoPoint) *, \
      __i _ARG level _ARG centroid)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneFromWGS84Centroid);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneLevel);
// int DGGRS_getZoneLevel(C(DGGRS) __i, C(DGGRSZone) zone);
#define DGGRS_getZoneLevel(__i, zone) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneLevel, __i, int, \
      C(DGGRS) _ARG C(DGGRSZone), \
      __i _ARG zone)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneLevel);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneNeighbors);
// int DGGRS_getZoneNeighbors(C(DGGRS) __i, C(DGGRSZone) zone, C(DGGRSZone) * neighbors, int * nbType);
#define DGGRS_getZoneNeighbors(__i, zone, neighbors, nbType) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneNeighbors, __i, int, \
      C(DGGRS) _ARG C(DGGRSZone) _ARG C(DGGRSZone) * _ARG int *, \
      __i _ARG zone _ARG neighbors _ARG nbType)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneNeighbors);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneParents);
// int DGGRS_getZoneParents(C(DGGRS) __i, C(DGGRSZone) zone, C(DGGRSZone) * parents);
#define DGGRS_getZoneParents(__i, zone, parents) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneParents, __i, int, \
      C(DGGRS) _ARG C(DGGRSZone) _ARG C(DGGRSZone) *, \
      __i _ARG zone _ARG parents)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneParents);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneRefinedCRSVertices);
// C(Array) DGGRS_getZoneRefinedCRSVertices(C(DGGRS) __i, C(DGGRSZone) zone, C(CRS) crs, int edgeRefinement);
#define DGGRS_getZoneRefinedCRSVertices(__i, zone, crs, edgeRefinement) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneRefinedCRSVertices, __i, C(Array), \
      C(DGGRS) _ARG C(DGGRSZone) _ARG C(CRS) _ARG int, \
      __i _ARG zone _ARG crs _ARG edgeRefinement)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneRefinedCRSVertices);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneRefinedWGS84Vertices);
// C(Array) DGGRS_getZoneRefinedWGS84Vertices(C(DGGRS) __i, C(DGGRSZone) zone, int edgeRefinement);
#define DGGRS_getZoneRefinedWGS84Vertices(__i, zone, edgeRefinement) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneRefinedWGS84Vertices, __i, C(Array), \
      C(DGGRS) _ARG C(DGGRSZone) _ARG int, \
      __i _ARG zone _ARG edgeRefinement)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneRefinedWGS84Vertices);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneTextID);
// void DGGRS_getZoneTextID(C(DGGRS) __i, C(DGGRSZone) zone, C(String) zoneID);
#define DGGRS_getZoneTextID(__i, zone, zoneID) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneTextID, __i, void, \
      C(DGGRS) _ARG C(DGGRSZone) _ARG C(String), \
      __i _ARG zone _ARG zoneID)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneTextID);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneWGS84Centroid);
// void DGGRS_getZoneWGS84Centroid(C(DGGRS) __i, C(DGGRSZone) zone, C(GeoPoint) * centroid);
#define DGGRS_getZoneWGS84Centroid(__i, zone, centroid) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneWGS84Centroid, __i, void, \
      C(DGGRS) _ARG C(DGGRSZone) _ARG C(GeoPoint) *, \
      __i _ARG zone _ARG centroid)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneWGS84Centroid);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneWGS84Extent);
// void DGGRS_getZoneWGS84Extent(C(DGGRS) __i, C(DGGRSZone) zone, C(GeoExtent) * extent);
#define DGGRS_getZoneWGS84Extent(__i, zone, extent) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneWGS84Extent, __i, void, \
      C(DGGRS) _ARG C(DGGRSZone) _ARG C(GeoExtent) *, \
      __i _ARG zone _ARG extent)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneWGS84Extent);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneWGS84ExtentApproximate);
// void DGGRS_getZoneWGS84ExtentApproximate(C(DGGRS) __i, C(DGGRSZone) zone, C(GeoExtent) * extent);
#define DGGRS_getZoneWGS84ExtentApproximate(__i, zone, extent) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneWGS84ExtentApproximate, __i, void, \
      C(DGGRS) _ARG C(DGGRSZone) _ARG C(GeoExtent) *, \
      __i _ARG zone _ARG extent)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneWGS84ExtentApproximate);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, getZoneWGS84Vertices);
// int DGGRS_getZoneWGS84Vertices(C(DGGRS) __i, C(DGGRSZone) zone, C(GeoPoint) * vertices);
#define DGGRS_getZoneWGS84Vertices(__i, zone, vertices) \
   VMETHOD(CO(DGGRS), DGGRS, getZoneWGS84Vertices, __i, int, \
      C(DGGRS) _ARG C(DGGRSZone) _ARG C(GeoPoint) *, \
      __i _ARG zone _ARG vertices)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, getZoneWGS84Vertices);

extern THIS_LIB_IMPORT C(bool) (* DGGRS_isZoneAncestorOf)(C(DGGRS) __this, C(DGGRSZone) ancestor, C(DGGRSZone) descendant, int maxDepth);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, isZoneCentroidChild);
// C(bool) DGGRS_isZoneCentroidChild(C(DGGRS) __i, C(DGGRSZone) zone);
#define DGGRS_isZoneCentroidChild(__i, zone) \
   VMETHOD(CO(DGGRS), DGGRS, isZoneCentroidChild, __i, C(bool), \
      C(DGGRS) _ARG C(DGGRSZone), \
      __i _ARG zone)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, isZoneCentroidChild);

extern THIS_LIB_IMPORT C(bool) (* DGGRS_isZoneContainedIn)(C(DGGRS) __this, C(DGGRSZone) needle, C(DGGRSZone) hayStack);

extern THIS_LIB_IMPORT C(bool) (* DGGRS_isZoneDescendantOf)(C(DGGRS) __this, C(DGGRSZone) descendant, C(DGGRSZone) ancestor, int maxDepth);

extern THIS_LIB_IMPORT C(bool) (* DGGRS_isZoneImmediateChildOf)(C(DGGRS) __this, C(DGGRSZone) child, C(DGGRSZone) parent);

extern THIS_LIB_IMPORT C(bool) (* DGGRS_isZoneImmediateParentOf)(C(DGGRS) __this, C(DGGRSZone) parent, C(DGGRSZone) child);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, listZones);
// C(Array) DGGRS_listZones(C(DGGRS) __i, int level, const C(GeoExtent) * bbox);
#define DGGRS_listZones(__i, level, bbox) \
   VMETHOD(CO(DGGRS), DGGRS, listZones, __i, C(Array), \
      C(DGGRS) _ARG int _ARG const C(GeoExtent) *, \
      __i _ARG level _ARG bbox)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, listZones);

extern THIS_LIB_IMPORT int M_VTBLID(DGGRS, zoneHasSubZone);
// C(bool) DGGRS_zoneHasSubZone(C(DGGRS) __i, C(DGGRSZone) hayStack, C(DGGRSZone) needle);
#define DGGRS_zoneHasSubZone(__i, hayStack, needle) \
   VMETHOD(CO(DGGRS), DGGRS, zoneHasSubZone, __i, C(bool), \
      C(DGGRS) _ARG C(DGGRSZone) _ARG C(DGGRSZone), \
      __i _ARG hayStack _ARG needle)
extern THIS_LIB_IMPORT C(Method) * METHOD(DGGRS, zoneHasSubZone);

#define DGGRSZONE_level_SHIFT                            59
#define DGGRSZONE_level_MASK                             0xF800000000000000LL
#define DGGRSZONE_level(x)                               ((((C(DGGRSZone))(x)) & DGGRSZONE_level_MASK) >> DGGRSZONE_level_SHIFT)
#define DGGRSZONE_SET_level(x, level)                        (x) = ((C(DGGRSZone))(x) & ~((C(DGGRSZone))DGGRSZONE_level_MASK)) | (((C(DGGRSZone))(level)) << DGGRSZONE_level_SHIFT)
#define DGGRSZONE_row_SHIFT                              30
#define DGGRSZONE_row_MASK                               0x7FFFFFFC0000000LL
#define DGGRSZONE_row(x)                                 ((((C(DGGRSZone))(x)) & DGGRSZONE_row_MASK) >> DGGRSZONE_row_SHIFT)
#define DGGRSZONE_SET_row(x, row)                          (x) = ((C(DGGRSZone))(x) & ~((C(DGGRSZone))DGGRSZONE_row_MASK)) | (((C(DGGRSZone))(row)) << DGGRSZONE_row_SHIFT)
#define DGGRSZONE_col_SHIFT                              0
#define DGGRSZONE_col_MASK                               0x3FFFFFFF
#define DGGRSZONE_col(x)                                 ((((C(DGGRSZone))(x)) & DGGRSZONE_col_MASK) >> DGGRSZONE_col_SHIFT)
#define DGGRSZONE_SET_col(x, col)                          (x) = ((C(DGGRSZone))(x) & ~((C(DGGRSZone))DGGRSZONE_col_MASK)) | (((C(DGGRSZone))(col)) << DGGRSZONE_col_SHIFT)
#define DGGRSZONE(level, row, col)                               (((((C(DGGRSZone))(level)) << DGGRSZONE_level_SHIFT) | ((C(DGGRSZone))(row)) << DGGRSZONE_row_SHIFT) | ((C(DGGRSZone))(col)) << DGGRSZONE_col_SHIFT)


struct CM(DGGSJSON)
{
   C(String) dggrs;
   C(String) zoneId;
   C(Array) depths;
   C(String) representedValue;
   C(JSONSchema) schema;
   C(Array) dimensions;
   C(Map) values;
};
struct CM(DGGSJSONDepth)
{
   int depth;
   C(DGGSJSONShape) shape;
   C(Array) data;
};
struct CM(DGGSJSONDimension)
{
   C(String) name;
   C(Array) interval;
   C(DGGSJSONGrid) grid;
   C(String) definition;
   C(String) unit;
   C(String) unitLang;
};
struct CM(DGGSJSONGrid)
{
   int cellsCount;
   double resolution;
   C(Array) coordinates;
   C(Array) boundsCoordinates;
   C(Array) relativeBounds;
   C(FieldValue) firstCoordinate;
};
struct CM(DGGSJSONShape)
{
   int count;
   int subZones;
   C(Map) dimensions;
};
#define GGGZONE_level_SHIFT                              59
#define GGGZONE_level_MASK                               0xF800000000000000LL
#define GGGZONE_level(x)                                 ((((C(GGGZone))(x)) & GGGZONE_level_MASK) >> GGGZONE_level_SHIFT)
#define GGGZONE_SET_level(x, level)                          (x) = ((C(GGGZone))(x) & ~((C(GGGZone))GGGZONE_level_MASK)) | (((C(GGGZone))(level)) << GGGZONE_level_SHIFT)
#define GGGZONE_row_SHIFT                                30
#define GGGZONE_row_MASK                                 0x7FFFFFFC0000000LL
#define GGGZONE_row(x)                                   ((((C(GGGZone))(x)) & GGGZONE_row_MASK) >> GGGZONE_row_SHIFT)
#define GGGZONE_SET_row(x, row)                            (x) = ((C(GGGZone))(x) & ~((C(GGGZone))GGGZONE_row_MASK)) | (((C(GGGZone))(row)) << GGGZONE_row_SHIFT)
#define GGGZONE_col_SHIFT                                0
#define GGGZONE_col_MASK                                 0x3FFFFFFF
#define GGGZONE_col(x)                                   ((((C(GGGZone))(x)) & GGGZONE_col_MASK) >> GGGZONE_col_SHIFT)
#define GGGZONE_SET_col(x, col)                            (x) = ((C(GGGZone))(x) & ~((C(GGGZone))GGGZONE_col_MASK)) | (((C(GGGZone))(col)) << GGGZONE_col_SHIFT)
#define GGGZONE(level, row, col)                                 (((((C(GGGZone))(level)) << GGGZONE_level_SHIFT) | ((C(GGGZone))(row)) << GGGZONE_row_SHIFT) | ((C(GGGZone))(col)) << GGGZONE_col_SHIFT)


struct C(GeoExtent)
{
   C(GeoPoint) ll;
   C(GeoPoint) ur;
};
extern THIS_LIB_IMPORT void (* GeoExtent_clear)(C(GeoExtent) * __this);

extern THIS_LIB_IMPORT C(bool) (* GeoExtent_clip)(C(GeoExtent) * __this, const C(GeoExtent) * e, const C(GeoExtent) * clipExtent);

extern THIS_LIB_IMPORT C(bool) (* GeoExtent_clipHandlingDateline)(C(GeoExtent) * __this, const C(GeoExtent) * e, const C(GeoExtent) * clipExtent);

extern THIS_LIB_IMPORT void (* GeoExtent_doUnionDL)(C(GeoExtent) * __this, const C(GeoExtent) * e);

extern THIS_LIB_IMPORT C(bool) (* GeoExtent_intersects)(C(GeoExtent) * __this, const C(GeoExtent) * b);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(GeoExtent, nonNull);
extern THIS_LIB_IMPORT C(bool) (* GeoExtent_get_nonNull)(const C(GeoExtent) * g);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(GeoExtent, geodeticArea);
extern THIS_LIB_IMPORT double (* GeoExtent_get_geodeticArea)(const C(GeoExtent) * g);

extern THIS_LIB_IMPORT int M_VTBLID(HEALPixProjection, forward);
// C(bool) HEALPixProjection_forward(C(HEALPixProjection) __i, const C(GeoPoint) * p, C(Pointd) * v);
#define HEALPixProjection_forward(__i, p, v) \
   VMETHOD(CO(HEALPixProjection), HEALPixProjection, forward, __i, C(bool), \
      C(HEALPixProjection) _ARG const C(GeoPoint) * _ARG C(Pointd) *, \
      __i _ARG p _ARG v)
extern THIS_LIB_IMPORT C(Method) * METHOD(HEALPixProjection, forward);

extern THIS_LIB_IMPORT int M_VTBLID(HEALPixProjection, inverse);
// C(bool) HEALPixProjection_inverse(C(HEALPixProjection) __i, const C(Pointd) * v, C(GeoPoint) * result, C(bool) oddGrid);
#define HEALPixProjection_inverse(__i, v, result, oddGrid) \
   VMETHOD(CO(HEALPixProjection), HEALPixProjection, inverse, __i, C(bool), \
      C(HEALPixProjection) _ARG const C(Pointd) * _ARG C(GeoPoint) * _ARG C(bool), \
      __i _ARG v _ARG result _ARG oddGrid)
extern THIS_LIB_IMPORT C(Method) * METHOD(HEALPixProjection, inverse);

#define HPZONE_level_SHIFT                               56
#define HPZONE_level_MASK                                0x1F00000000000000LL
#define HPZONE_level(x)                                  ((((C(HPZone))(x)) & HPZONE_level_MASK) >> HPZONE_level_SHIFT)
#define HPZONE_SET_level(x, level)                           (x) = ((C(HPZone))(x) & ~((C(HPZone))HPZONE_level_MASK)) | (((C(HPZone))(level)) << HPZONE_level_SHIFT)
#define HPZONE_rootRhombus_SHIFT                         52
#define HPZONE_rootRhombus_MASK                          0xF0000000000000LL
#define HPZONE_rootRhombus(x)                            ((((C(HPZone))(x)) & HPZONE_rootRhombus_MASK) >> HPZONE_rootRhombus_SHIFT)
#define HPZONE_SET_rootRhombus(x, rootRhombus)                     (x) = ((C(HPZone))(x) & ~((C(HPZone))HPZONE_rootRhombus_MASK)) | (((C(HPZone))(rootRhombus)) << HPZONE_rootRhombus_SHIFT)
#define HPZONE_subIndex_SHIFT                            0
#define HPZONE_subIndex_MASK                             0xFFFFFFFFFFFFFLL
#define HPZONE_subIndex(x)                               ((((C(HPZone))(x)) & HPZONE_subIndex_MASK) >> HPZONE_subIndex_SHIFT)
#define HPZONE_SET_subIndex(x, subIndex)                        (x) = ((C(HPZone))(x) & ~((C(HPZone))HPZONE_subIndex_MASK)) | (((C(HPZone))(subIndex)) << HPZONE_subIndex_SHIFT)
#define HPZONE(level, rootRhombus, subIndex)                                  (((((C(HPZone))(level)) << HPZONE_level_SHIFT) | ((C(HPZone))(rootRhombus)) << HPZONE_rootRhombus_SHIFT) | ((C(HPZone))(subIndex)) << HPZONE_subIndex_SHIFT)


#define I3HZONE_levelI9R_SHIFT                           57
#define I3HZONE_levelI9R_MASK                            0x3E00000000000000LL
#define I3HZONE_levelI9R(x)                              ((((C(I3HZone))(x)) & I3HZONE_levelI9R_MASK) >> I3HZONE_levelI9R_SHIFT)
#define I3HZONE_SET_levelI9R(x, levelI9R)                       (x) = ((C(I3HZone))(x) & ~((C(I3HZone))I3HZONE_levelI9R_MASK)) | (((C(I3HZone))(levelI9R)) << I3HZONE_levelI9R_SHIFT)
#define I3HZONE_rootRhombus_SHIFT                        53
#define I3HZONE_rootRhombus_MASK                         0x1E0000000000000LL
#define I3HZONE_rootRhombus(x)                           ((((C(I3HZone))(x)) & I3HZONE_rootRhombus_MASK) >> I3HZONE_rootRhombus_SHIFT)
#define I3HZONE_SET_rootRhombus(x, rootRhombus)                    (x) = ((C(I3HZone))(x) & ~((C(I3HZone))I3HZONE_rootRhombus_MASK)) | (((C(I3HZone))(rootRhombus)) << I3HZONE_rootRhombus_SHIFT)
#define I3HZONE_rhombusIX_SHIFT                          2
#define I3HZONE_rhombusIX_MASK                           0x1FFFFFFFFFFFFCLL
#define I3HZONE_rhombusIX(x)                             ((((C(I3HZone))(x)) & I3HZONE_rhombusIX_MASK) >> I3HZONE_rhombusIX_SHIFT)
#define I3HZONE_SET_rhombusIX(x, rhombusIX)                      (x) = ((C(I3HZone))(x) & ~((C(I3HZone))I3HZONE_rhombusIX_MASK)) | (((C(I3HZone))(rhombusIX)) << I3HZONE_rhombusIX_SHIFT)
#define I3HZONE_subHex_SHIFT                             0
#define I3HZONE_subHex_MASK                              0x3
#define I3HZONE_subHex(x)                                ((((C(I3HZone))(x)) & I3HZONE_subHex_MASK) >> I3HZONE_subHex_SHIFT)
#define I3HZONE_SET_subHex(x, subHex)                         (x) = ((C(I3HZone))(x) & ~((C(I3HZone))I3HZONE_subHex_MASK)) | (((C(I3HZone))(subHex)) << I3HZONE_subHex_SHIFT)
#define I3HZONE(levelI9R, rootRhombus, rhombusIX, subHex)                               ((((((C(I3HZone))(levelI9R)) << I3HZONE_levelI9R_SHIFT) | ((C(I3HZone))(rootRhombus)) << I3HZONE_rootRhombus_SHIFT) | ((C(I3HZone))(rhombusIX)) << I3HZONE_rhombusIX_SHIFT) | ((C(I3HZone))(subHex)) << I3HZONE_subHex_SHIFT)


#define I4RZONE_level_SHIFT                              59
#define I4RZONE_level_MASK                               0xF800000000000000LL
#define I4RZONE_level(x)                                 ((((C(I4RZone))(x)) & I4RZONE_level_MASK) >> I4RZONE_level_SHIFT)
#define I4RZONE_SET_level(x, level)                          (x) = ((C(I4RZone))(x) & ~((C(I4RZone))I4RZONE_level_MASK)) | (((C(I4RZone))(level)) << I4RZONE_level_SHIFT)
#define I4RZONE_row_SHIFT                                30
#define I4RZONE_row_MASK                                 0x7FFFFFFC0000000LL
#define I4RZONE_row(x)                                   ((((C(I4RZone))(x)) & I4RZONE_row_MASK) >> I4RZONE_row_SHIFT)
#define I4RZONE_SET_row(x, row)                            (x) = ((C(I4RZone))(x) & ~((C(I4RZone))I4RZONE_row_MASK)) | (((C(I4RZone))(row)) << I4RZONE_row_SHIFT)
#define I4RZONE_col_SHIFT                                0
#define I4RZONE_col_MASK                                 0x3FFFFFFF
#define I4RZONE_col(x)                                   ((((C(I4RZone))(x)) & I4RZONE_col_MASK) >> I4RZONE_col_SHIFT)
#define I4RZONE_SET_col(x, col)                            (x) = ((C(I4RZone))(x) & ~((C(I4RZone))I4RZONE_col_MASK)) | (((C(I4RZone))(col)) << I4RZONE_col_SHIFT)
#define I4RZONE(level, row, col)                                 (((((C(I4RZone))(level)) << I4RZONE_level_SHIFT) | ((C(I4RZone))(row)) << I4RZONE_row_SHIFT) | ((C(I4RZone))(col)) << I4RZONE_col_SHIFT)


#define I7HZONE_levelI49R_SHIFT                          58
#define I7HZONE_levelI49R_MASK                           0x3C00000000000000LL
#define I7HZONE_levelI49R(x)                             ((((C(I7HZone))(x)) & I7HZONE_levelI49R_MASK) >> I7HZONE_levelI49R_SHIFT)
#define I7HZONE_SET_levelI49R(x, levelI49R)                      (x) = ((C(I7HZone))(x) & ~((C(I7HZone))I7HZONE_levelI49R_MASK)) | (((C(I7HZone))(levelI49R)) << I7HZONE_levelI49R_SHIFT)
#define I7HZONE_rootRhombus_SHIFT                        54
#define I7HZONE_rootRhombus_MASK                         0x3C0000000000000LL
#define I7HZONE_rootRhombus(x)                           ((((C(I7HZone))(x)) & I7HZONE_rootRhombus_MASK) >> I7HZONE_rootRhombus_SHIFT)
#define I7HZONE_SET_rootRhombus(x, rootRhombus)                    (x) = ((C(I7HZone))(x) & ~((C(I7HZone))I7HZONE_rootRhombus_MASK)) | (((C(I7HZone))(rootRhombus)) << I7HZONE_rootRhombus_SHIFT)
#define I7HZONE_rhombusIX_SHIFT                          3
#define I7HZONE_rhombusIX_MASK                           0x3FFFFFFFFFFFF8LL
#define I7HZONE_rhombusIX(x)                             ((((C(I7HZone))(x)) & I7HZONE_rhombusIX_MASK) >> I7HZONE_rhombusIX_SHIFT)
#define I7HZONE_SET_rhombusIX(x, rhombusIX)                      (x) = ((C(I7HZone))(x) & ~((C(I7HZone))I7HZONE_rhombusIX_MASK)) | (((C(I7HZone))(rhombusIX)) << I7HZONE_rhombusIX_SHIFT)
#define I7HZONE_subHex_SHIFT                             0
#define I7HZONE_subHex_MASK                              0x7
#define I7HZONE_subHex(x)                                ((((C(I7HZone))(x)) & I7HZONE_subHex_MASK) >> I7HZONE_subHex_SHIFT)
#define I7HZONE_SET_subHex(x, subHex)                         (x) = ((C(I7HZone))(x) & ~((C(I7HZone))I7HZONE_subHex_MASK)) | (((C(I7HZone))(subHex)) << I7HZONE_subHex_SHIFT)
#define I7HZONE(levelI49R, rootRhombus, rhombusIX, subHex)                               ((((((C(I7HZone))(levelI49R)) << I7HZONE_levelI49R_SHIFT) | ((C(I7HZone))(rootRhombus)) << I7HZONE_rootRhombus_SHIFT) | ((C(I7HZone))(rhombusIX)) << I7HZONE_rhombusIX_SHIFT) | ((C(I7HZone))(subHex)) << I7HZONE_subHex_SHIFT)


#define I9RZONE_level_SHIFT                              59
#define I9RZONE_level_MASK                               0xF800000000000000LL
#define I9RZONE_level(x)                                 ((((C(I9RZone))(x)) & I9RZONE_level_MASK) >> I9RZONE_level_SHIFT)
#define I9RZONE_SET_level(x, level)                          (x) = ((C(I9RZone))(x) & ~((C(I9RZone))I9RZONE_level_MASK)) | (((C(I9RZone))(level)) << I9RZONE_level_SHIFT)
#define I9RZONE_row_SHIFT                                30
#define I9RZONE_row_MASK                                 0x7FFFFFFC0000000LL
#define I9RZONE_row(x)                                   ((((C(I9RZone))(x)) & I9RZONE_row_MASK) >> I9RZONE_row_SHIFT)
#define I9RZONE_SET_row(x, row)                            (x) = ((C(I9RZone))(x) & ~((C(I9RZone))I9RZONE_row_MASK)) | (((C(I9RZone))(row)) << I9RZONE_row_SHIFT)
#define I9RZONE_col_SHIFT                                0
#define I9RZONE_col_MASK                                 0x3FFFFFFF
#define I9RZONE_col(x)                                   ((((C(I9RZone))(x)) & I9RZONE_col_MASK) >> I9RZONE_col_SHIFT)
#define I9RZONE_SET_col(x, col)                            (x) = ((C(I9RZone))(x) & ~((C(I9RZone))I9RZONE_col_MASK)) | (((C(I9RZone))(col)) << I9RZONE_col_SHIFT)
#define I9RZONE(level, row, col)                                 (((((C(I9RZone))(level)) << I9RZONE_level_SHIFT) | ((C(I9RZone))(row)) << I9RZONE_row_SHIFT) | ((C(I9RZone))(col)) << I9RZONE_col_SHIFT)


struct CM(JSONSchema)
{
   C(String) schema;
   C(String) id;
   C(String) title;
   C(String) comment;
   C(String) description;
   C(FieldValue) Default;
   C(bool) readOnly;
   C(bool) writeOnly;
   C(Array) examples;
   C(Array) multipleOf;
   C(JSONSchemaType) type;
   C(Array) Enum;
   C(String) format;
   C(String) contentMediaType;
   double maximum;
   double exclusiveMaximum;
   double minimum;
   double exclusiveMinimum;
   C(String) pattern;
   C(JSONSchema) items;
   int maxItems;
   int minItems;
   C(bool) uniqueItems;
   C(String) contains;
   int maxProperties;
   int minProperties;
   C(Array) required;
   C(JSONSchema) additionalProperties;
   C(Map) definitions;
   C(Map) properties;
   C(Map) patternProperties;
   C(Map) dependencies;
   C(String) propertyNames;
   C(String) contentEncoding;
   C(JSONSchema) If;
   C(JSONSchema) Then;
   C(JSONSchema) Else;
   C(Array) allOf;
   C(Array) anyOf;
   C(Array) oneOf;
   C(JSONSchema) Not;
   C(String) xogcrole;
   int xogcpropertySeq;
};
extern THIS_LIB_IMPORT C(Property) * PROPERTY(JSONSchema, maximum);
extern THIS_LIB_IMPORT double (* JSONSchema_get_maximum)(const C(JSONSchema) j);
extern THIS_LIB_IMPORT C(bool) (* JSONSchema_isSet_maximum)(const C(JSONSchema) j);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(JSONSchema, exclusiveMaximum);
extern THIS_LIB_IMPORT double (* JSONSchema_get_exclusiveMaximum)(const C(JSONSchema) j);
extern THIS_LIB_IMPORT C(bool) (* JSONSchema_isSet_exclusiveMaximum)(const C(JSONSchema) j);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(JSONSchema, minimum);
extern THIS_LIB_IMPORT double (* JSONSchema_get_minimum)(const C(JSONSchema) j);
extern THIS_LIB_IMPORT C(bool) (* JSONSchema_isSet_minimum)(const C(JSONSchema) j);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(JSONSchema, exclusiveMinimum);
extern THIS_LIB_IMPORT double (* JSONSchema_get_exclusiveMinimum)(const C(JSONSchema) j);
extern THIS_LIB_IMPORT C(bool) (* JSONSchema_isSet_exclusiveMinimum)(const C(JSONSchema) j);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(JSONSchema, maxItems);
extern THIS_LIB_IMPORT int (* JSONSchema_get_maxItems)(const C(JSONSchema) j);
extern THIS_LIB_IMPORT C(bool) (* JSONSchema_isSet_maxItems)(const C(JSONSchema) j);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(JSONSchema, minItems);
extern THIS_LIB_IMPORT int (* JSONSchema_get_minItems)(const C(JSONSchema) j);
extern THIS_LIB_IMPORT C(bool) (* JSONSchema_isSet_minItems)(const C(JSONSchema) j);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(JSONSchema, maxProperties);
extern THIS_LIB_IMPORT int (* JSONSchema_get_maxProperties)(const C(JSONSchema) j);
extern THIS_LIB_IMPORT C(bool) (* JSONSchema_isSet_maxProperties)(const C(JSONSchema) j);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(JSONSchema, minProperties);
extern THIS_LIB_IMPORT int (* JSONSchema_get_minProperties)(const C(JSONSchema) j);
extern THIS_LIB_IMPORT C(bool) (* JSONSchema_isSet_minProperties)(const C(JSONSchema) j);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(JSONSchema, xogcpropertySeq);
extern THIS_LIB_IMPORT C(bool) (* JSONSchema_isSet_xogcpropertySeq)(const C(JSONSchema) j);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(JSONSchema, Default);
extern THIS_LIB_IMPORT C(bool) (* JSONSchema_isSet_Default)(const C(JSONSchema) j);

struct C(Plane)
{
   union
   {
      struct
      {
         double a;
         double b;
         double c;
      };
      C(Vector3D) normal;
   };
   double d;
};
extern THIS_LIB_IMPORT void (* Plane_fromPoints)(C(Plane) * __this, const C(Vector3D) * v1, const C(Vector3D) * v2, const C(Vector3D) * v3);

struct C(Quaternion)
{
   double w;
   double x;
   double y;
   double z;
};
extern THIS_LIB_IMPORT void (* Quaternion_yawPitch)(C(Quaternion) * __this, C(Angle) yaw, C(Angle) pitch);

#define RHPZONE_level_SHIFT                              59
#define RHPZONE_level_MASK                               0xF800000000000000LL
#define RHPZONE_level(x)                                 ((((C(RHPZone))(x)) & RHPZONE_level_MASK) >> RHPZONE_level_SHIFT)
#define RHPZONE_SET_level(x, level)                          (x) = ((C(RHPZone))(x) & ~((C(RHPZone))RHPZONE_level_MASK)) | (((C(RHPZone))(level)) << RHPZONE_level_SHIFT)
#define RHPZONE_row_SHIFT                                30
#define RHPZONE_row_MASK                                 0x7FFFFFFC0000000LL
#define RHPZONE_row(x)                                   ((((C(RHPZone))(x)) & RHPZONE_row_MASK) >> RHPZONE_row_SHIFT)
#define RHPZONE_SET_row(x, row)                            (x) = ((C(RHPZone))(x) & ~((C(RHPZone))RHPZONE_row_MASK)) | (((C(RHPZone))(row)) << RHPZONE_row_SHIFT)
#define RHPZONE_col_SHIFT                                0
#define RHPZONE_col_MASK                                 0x3FFFFFFF
#define RHPZONE_col(x)                                   ((((C(RHPZone))(x)) & RHPZONE_col_MASK) >> RHPZONE_col_SHIFT)
#define RHPZONE_SET_col(x, col)                            (x) = ((C(RHPZone))(x) & ~((C(RHPZone))RHPZONE_col_MASK)) | (((C(RHPZone))(col)) << RHPZONE_col_SHIFT)
#define RHPZONE(level, row, col)                                 (((((C(RHPZone))(level)) << RHPZONE_level_SHIFT) | ((C(RHPZone))(row)) << RHPZONE_row_SHIFT) | ((C(RHPZone))(col)) << RHPZONE_col_SHIFT)


extern THIS_LIB_IMPORT void (* RI5x6Projection_extent5x6FromWGS84)(C(RI5x6Projection) __this, const C(GeoExtent) * wgs84Extent, C(Pointd) * topLeft, C(Pointd) * bottomRight);

extern THIS_LIB_IMPORT int M_VTBLID(RI5x6Projection, forward);
// C(bool) RI5x6Projection_forward(C(RI5x6Projection) __i, const C(GeoPoint) * p, C(Pointd) * v);
#define RI5x6Projection_forward(__i, p, v) \
   VMETHOD(CO(RI5x6Projection), RI5x6Projection, forward, __i, C(bool), \
      C(RI5x6Projection) _ARG const C(GeoPoint) * _ARG C(Pointd) *, \
      __i _ARG p _ARG v)
extern THIS_LIB_IMPORT C(Method) * METHOD(RI5x6Projection, forward);

extern THIS_LIB_IMPORT C(bool) (* RI5x6Projection_fromIcosahedronNet)(const C(Pointd) * v, C(Pointd) * result);

extern THIS_LIB_IMPORT int M_VTBLID(RI5x6Projection, inverse);
// C(bool) RI5x6Projection_inverse(C(RI5x6Projection) __i, const C(Pointd) * _v, C(GeoPoint) * result, C(bool) oddGrid);
#define RI5x6Projection_inverse(__i, _v, result, oddGrid) \
   VMETHOD(CO(RI5x6Projection), RI5x6Projection, inverse, __i, C(bool), \
      C(RI5x6Projection) _ARG const C(Pointd) * _ARG C(GeoPoint) * _ARG C(bool), \
      __i _ARG _v _ARG result _ARG oddGrid)
extern THIS_LIB_IMPORT C(Method) * METHOD(RI5x6Projection, inverse);

extern THIS_LIB_IMPORT C(bool) (* RI5x6Projection_toIcosahedronNet)(const C(Pointd) * v, C(Pointd) * result);

extern THIS_LIB_IMPORT void (* Vector3D_crossProduct)(C(Vector3D) * __this, const C(Vector3D) * vector1, const C(Vector3D) * vector2);

extern THIS_LIB_IMPORT double (* Vector3D_dotProduct)(C(Vector3D) * __this, const C(Vector3D) * vector2);

extern THIS_LIB_IMPORT void (* Vector3D_multQuaternion)(C(Vector3D) * __this, const C(Vector3D) * s, const C(Quaternion) * quat);

extern THIS_LIB_IMPORT void (* Vector3D_normalize)(C(Vector3D) * __this, const C(Vector3D) * source);

extern THIS_LIB_IMPORT void (* Vector3D_subtract)(C(Vector3D) * __this, const C(Vector3D) * vector1, const C(Vector3D) * vector2);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Vector3D, length);
extern THIS_LIB_IMPORT double (* Vector3D_get_length)(const C(Vector3D) * v);

#define Z7ZONE_rootPentagon_SHIFT                        60
#define Z7ZONE_rootPentagon_MASK                         0xF000000000000000LL
#define Z7ZONE_rootPentagon(x)                           ((((C(Z7Zone))(x)) & Z7ZONE_rootPentagon_MASK) >> Z7ZONE_rootPentagon_SHIFT)
#define Z7ZONE_SET_rootPentagon(x, rootPentagon)                    (x) = ((C(Z7Zone))(x) & ~((C(Z7Zone))Z7ZONE_rootPentagon_MASK)) | (((C(Z7Zone))(rootPentagon)) << Z7ZONE_rootPentagon_SHIFT)
#define Z7ZONE_ancestry_SHIFT                            0
#define Z7ZONE_ancestry_MASK                             0xFFFFFFFFFFFFFFFLL
#define Z7ZONE_ancestry(x)                               ((((C(Z7Zone))(x)) & Z7ZONE_ancestry_MASK) >> Z7ZONE_ancestry_SHIFT)
#define Z7ZONE_SET_ancestry(x, ancestry)                        (x) = ((C(Z7Zone))(x) & ~((C(Z7Zone))Z7ZONE_ancestry_MASK)) | (((C(Z7Zone))(ancestry)) << Z7ZONE_ancestry_SHIFT)
#define Z7ZONE(rootPentagon, ancestry)                                    ((((C(Z7Zone))(rootPentagon)) << Z7ZONE_rootPentagon_SHIFT) | ((C(Z7Zone))(ancestry)) << Z7ZONE_ancestry_SHIFT)


extern THIS_LIB_IMPORT C(Z7Zone) (* Z7Zone_from7H)(C(I7HZone) zone);

extern THIS_LIB_IMPORT C(Z7Zone) (* Z7Zone_fromTextID)(constString zoneID);

extern THIS_LIB_IMPORT int (* Z7Zone_getParentRotationOffset)(C(I7HZone) zone);

extern THIS_LIB_IMPORT void (* Z7Zone_getTextID)(C(Z7Zone) __this, C(String) zoneID);

extern THIS_LIB_IMPORT C(I7HZone) (* Z7Zone_to7H)(C(Z7Zone) __this);

extern THIS_LIB_IMPORT C(I3HZone) (* F(i3HZoneFromI9R))(C(I9RZone) zone, char subHex);
extern THIS_LIB_IMPORT C(I9RZone) (* F(i9RZoneFromI3H))(C(I3HZone) zone);
extern THIS_LIB_IMPORT void (* F(authalicSetup))(double a, double b, double cp[2][6]);
extern THIS_LIB_IMPORT void (* F(canonicalize5x6))(const C(Pointd) * _src, C(Pointd) * out);
extern THIS_LIB_IMPORT void (* F(compactGGGZones))(C(Array) zones, int start, int maxLevel);
extern THIS_LIB_IMPORT C(Angle) (* F(latAuthalicToGeodetic))(const double cp[2][6], C(Angle) phi);
extern THIS_LIB_IMPORT C(Angle) (* F(latGeodeticToAuthalic))(const double cp[2][6], C(Angle) phi);
extern THIS_LIB_IMPORT C(DGGSJSON) (* F(readDGGSJSON))(C(File) f);
extern THIS_LIB_IMPORT C(Class) * CO(BCTA3H);
extern THIS_LIB_IMPORT C(Class) * CO(BarycentricSphericalTriAreaProjection);
extern THIS_LIB_IMPORT C(Class) * CO(CRS);
extern THIS_LIB_IMPORT C(Class) * CO(CRSExtent);
extern THIS_LIB_IMPORT C(Class) * CO(CRSRegistry);
extern THIS_LIB_IMPORT C(Class) * CO(DGGRS);
extern THIS_LIB_IMPORT C(Class) * CO(DGGRSZone);
extern THIS_LIB_IMPORT C(Class) * CO(DGGSJSON);
extern THIS_LIB_IMPORT C(Class) * CO(DGGSJSONDepth);
extern THIS_LIB_IMPORT C(Class) * CO(DGGSJSONDimension);
extern THIS_LIB_IMPORT C(Class) * CO(DGGSJSONGrid);
extern THIS_LIB_IMPORT C(Class) * CO(DGGSJSONShape);
extern THIS_LIB_IMPORT C(Class) * CO(GGGZone);
extern THIS_LIB_IMPORT C(Class) * CO(GNOSISGlobalGrid);
extern THIS_LIB_IMPORT C(Class) * CO(GPP3H);
extern THIS_LIB_IMPORT C(Class) * CO(GeoExtent);
extern THIS_LIB_IMPORT C(Class) * CO(GeoPoint);
extern THIS_LIB_IMPORT C(Class) * CO(GoldbergPolyhedraProjection);
extern THIS_LIB_IMPORT C(Class) * CO(HEALPix);
extern THIS_LIB_IMPORT C(Class) * CO(HEALPixProjection);
extern THIS_LIB_IMPORT C(Class) * CO(HPZone);
extern THIS_LIB_IMPORT C(Class) * CO(I3HNeighbor);
extern THIS_LIB_IMPORT C(Class) * CO(I3HZone);
extern THIS_LIB_IMPORT C(Class) * CO(I4RZone);
extern THIS_LIB_IMPORT C(Class) * CO(I7HZone);
extern THIS_LIB_IMPORT C(Class) * CO(I9RZone);
extern THIS_LIB_IMPORT C(Class) * CO(ISEA3H);
extern THIS_LIB_IMPORT C(Class) * CO(ISEA4R);
extern THIS_LIB_IMPORT C(Class) * CO(ISEA7H);
extern THIS_LIB_IMPORT C(Class) * CO(ISEA7H_Z7);
extern THIS_LIB_IMPORT C(Class) * CO(ISEA9R);
extern THIS_LIB_IMPORT C(Class) * CO(ISEAProjection);
extern THIS_LIB_IMPORT C(Class) * CO(IVEA3H);
extern THIS_LIB_IMPORT C(Class) * CO(IVEA4R);
extern THIS_LIB_IMPORT C(Class) * CO(IVEA7H);
extern THIS_LIB_IMPORT C(Class) * CO(IVEA7H_Z7);
extern THIS_LIB_IMPORT C(Class) * CO(IVEA9R);
extern THIS_LIB_IMPORT C(Class) * CO(IVEAProjection);
extern THIS_LIB_IMPORT C(Class) * CO(JSONSchema);
extern THIS_LIB_IMPORT C(Class) * CO(JSONSchemaType);
extern THIS_LIB_IMPORT C(Class) * CO(Plane);
extern THIS_LIB_IMPORT C(Class) * CO(Quaternion);
extern THIS_LIB_IMPORT C(Class) * CO(RHPZone);
extern THIS_LIB_IMPORT C(Class) * CO(RI5x6Projection);
extern THIS_LIB_IMPORT C(Class) * CO(RI7H_Z7);
extern THIS_LIB_IMPORT C(Class) * CO(RTEA3H);
extern THIS_LIB_IMPORT C(Class) * CO(RTEA4R);
extern THIS_LIB_IMPORT C(Class) * CO(RTEA7H);
extern THIS_LIB_IMPORT C(Class) * CO(RTEA7H_Z7);
extern THIS_LIB_IMPORT C(Class) * CO(RTEA9R);
extern THIS_LIB_IMPORT C(Class) * CO(RTEAProjection);
extern THIS_LIB_IMPORT C(Class) * CO(RhombicIcosahedral3H);
extern THIS_LIB_IMPORT C(Class) * CO(RhombicIcosahedral4R);
extern THIS_LIB_IMPORT C(Class) * CO(RhombicIcosahedral7H);
extern THIS_LIB_IMPORT C(Class) * CO(RhombicIcosahedral9R);
extern THIS_LIB_IMPORT C(Class) * CO(SliceAndDiceGreatCircleIcosahedralProjection);
extern THIS_LIB_IMPORT C(Class) * CO(VGCRadialVertex);
extern THIS_LIB_IMPORT C(Class) * CO(Vector3D);
extern THIS_LIB_IMPORT C(Class) * CO(Z7Zone);
extern THIS_LIB_IMPORT C(Class) * CO(rHEALPix);
extern THIS_LIB_IMPORT C(Class) * CO(rHEALPixProjection);

extern THIS_LIB_IMPORT C(Module) dggal_init(C(Module) fromModule);



#ifdef __cplusplus

};

#endif

#endif // !defined(__DGGAL_H__)
