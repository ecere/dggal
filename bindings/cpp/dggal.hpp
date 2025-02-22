// Preprocessor directives can be added at the beginning (Can't store them in AST)

/****************************************************************************
===========================================================================
   dggal Module
===========================================================================
****************************************************************************/
#if !defined(__DGGAL_HPP__)
#define __DGGAL_HPP__

#include "ecere.hpp"
#include "dggal.h"

class CRS_;
struct CRSExtent;
enum class CRSRegistry : int;
class DGGRS;
class DGGRSZone;
class DGGSJSON;
class DGGSJSONDepth;
class DGGSJSONGrid;
class DGGSJSONShape;
enum class FieldType : int;
class FieldTypeEx;
struct FieldValue;
enum class FieldValueFormat : int;
class GGGZone;
class GNOSISGlobalGrid;
struct GeoExtent;
struct GeoPoint;
class ISEA3H;
class ISEA3HZone;
class ISEA9R;
class ISEA9RZone;
class JSONSchema;
enum class JSONSchemaType : int;

int dggal_cpp_init(const Module & module);


class DGGRSZone
{
public:
   C(DGGRSZone) impl;
   constexpr DGGRSZone() : impl(0) { }
   constexpr DGGRSZone(C(DGGRSZone) impl) : impl(impl) { }
   DGGRSZone(uint level, uint row, uint col)
   {
      impl = DGGRSZONE(level, row, col);
   }
   operator C(DGGRSZone)() { return impl; }
   DGGRSZone & operator =(C(DGGRSZone) impl) { impl = impl; return *this; }
   bool operator ==(const DGGRSZone & value) const { return impl == value.impl; }
   // REVIEW: Should we add this automatically?
   bool operator ==(C(DGGRSZone) value) const { return impl == value; }
   bool operator !=(const DGGRSZone & value) const { return impl != value.impl; }
   bool operator !=(C(DGGRSZone) value) const { return impl != value; }
};

#define REG_DGGRS_compactZones(m, c)                REGVMETHOD(DGGRS, compactZones,                c::m, (/*1Ab*/DGGRS & self, /*1Ab*/TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)> & zones),           c, (/*4Im*/zones))
#define REG_DGGRS_countSubZones(m, c)               REGVMETHOD(DGGRS, countSubZones,               c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone, /*1Ab*/int depth),                                  c, (/*4Hm*/(DGGRSZone)zone, /*4Im*/depth))
#define REG_DGGRS_countZoneEdges(m, c)              REGVMETHOD(DGGRS, countZoneEdges,              c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone),                                                    c, (/*4Hm*/(DGGRSZone)zone))
#define REG_DGGRS_countZones(m, c)                  REGVMETHOD(DGGRS, countZones,                  c::m, (/*1Ab*/DGGRS & self, /*1Ab*/int level),                                                         c, (/*4Im*/level))
#define REG_DGGRS_getFirstSubZone(m, c)             REGVMETHOD(DGGRS, getFirstSubZone,             c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone, /*1Ab*/int relativeDepth),                          c, (/*4Hm*/(DGGRSZone)zone, /*4Im*/relativeDepth))
#define REG_DGGRS_getMaxDGGRSZoneLevel(m, c)        REGVMETHOD(DGGRS, getMaxDGGRSZoneLevel,        c::m, (/*1Ab*/DGGRS & self),                                                                           c, ())
#define REG_DGGRS_getRefinementRatio(m, c)          REGVMETHOD(DGGRS, getRefinementRatio,          c::m, (/*1Ab*/DGGRS & self),                                                                           c, ())
#define REG_DGGRS_getSubZoneCRSCentroids(m, c)      REGVMETHOD(DGGRS, getSubZoneCRSCentroids,      c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone parent, /*1Ab*/CRS crs, /*1Ab*/int relativeDepth),        c, (/*4Hm*/(DGGRSZone)parent, /*4Hm*/(CRS)crs, /*4Im*/relativeDepth))
#define REG_DGGRS_getSubZoneWGS84Centroids(m, c)    REGVMETHOD(DGGRS, getSubZoneWGS84Centroids,    c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone parent, /*1Ab*/int relativeDepth),                        c, (/*4Hm*/(DGGRSZone)parent, /*4Im*/relativeDepth))
#define REG_DGGRS_getZoneArea(m, c)                 REGVMETHOD(DGGRS, getZoneArea,                 c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone),                                                    c, (/*4Hm*/(DGGRSZone)zone))
#define REG_DGGRS_getZoneCRSCentroid(m, c)          REGVMETHOD(DGGRS, getZoneCRSCentroid,          c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone, /*1Ab*/CRS crs, /*1Ab*/Pointd & centroid),          c, (/*4Hm*/(DGGRSZone)zone, /*4Hm*/(CRS)crs, /*4Im*/centroid))
#define REG_DGGRS_getZoneCRSExtent(m, c)            REGVMETHOD(DGGRS, getZoneCRSExtent,            c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone, /*1Ab*/CRS crs, /*1Ab*/CRSExtent & extent),         c, (/*4Hm*/(DGGRSZone)zone, /*4Hm*/(CRS)crs, /*4Im*/extent))
#define REG_DGGRS_getZoneCRSVertices(m, c)          REGVMETHOD(DGGRS, getZoneCRSVertices,          c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone, /*1Ab*/CRS crs, /*1Ab*/Pointd * vertices),          c, (/*4Hm*/(DGGRSZone)zone, /*4Hm*/(CRS)crs, /*4Hm*/(Pointd *)vertices))
#define REG_DGGRS_getZoneCentroidChild(m, c)        REGVMETHOD(DGGRS, getZoneCentroidChild,        c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone),                                                    c, (/*4Hm*/(DGGRSZone)zone))
#define REG_DGGRS_getZoneCentroidParent(m, c)       REGVMETHOD(DGGRS, getZoneCentroidParent,       c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone),                                                    c, (/*4Hm*/(DGGRSZone)zone))
#define REG_DGGRS_getZoneChildren(m, c)             REGVMETHOD(DGGRS, getZoneChildren,             c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone, /*1Ab*/DGGRSZone * children),                       c, (/*4Hm*/(DGGRSZone)zone, /*4Hm*/(DGGRSZone *)children))
#define REG_DGGRS_getZoneFromCRSCentroid(m, c)      REGVMETHOD(DGGRS, getZoneFromCRSCentroid,      c::m, (/*1Ab*/DGGRS & self, /*1Ab*/int level, /*1Ab*/CRS crs, /*1Ab*/const Pointd & centroid),         c, (/*4Im*/level, /*4Hm*/(CRS)crs, /*4Im*/centroid))
#define REG_DGGRS_getZoneFromTextID(m, c)           REGVMETHOD(DGGRS, getZoneFromTextID,           c::m, (/*1Ab*/DGGRS & self, constString zoneID),                                                       c, (/*4Im*/zoneID))
#define REG_DGGRS_getZoneFromWGS84Centroid(m, c)    REGVMETHOD(DGGRS, getZoneFromWGS84Centroid,    c::m, (/*1Ab*/DGGRS & self, /*1Ab*/int level, /*1Ab*/const GeoPoint & centroid),                       c, (/*4Im*/level, /*4Im*/centroid))
#define REG_DGGRS_getZoneLevel(m, c)                REGVMETHOD(DGGRS, getZoneLevel,                c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone),                                                    c, (/*4Hm*/(DGGRSZone)zone))
#define REG_DGGRS_getZoneNeighbors(m, c)            REGVMETHOD(DGGRS, getZoneNeighbors,            c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone, /*1Ab*/DGGRSZone * neighbors, /*1Ab*/int * nbType), c, (/*4Hm*/(DGGRSZone)zone, /*4Hm*/(DGGRSZone *)neighbors, /*4Im*/nbType))
#define REG_DGGRS_getZoneParents(m, c)              REGVMETHOD(DGGRS, getZoneParents,              c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone, /*1Ab*/DGGRSZone * parents),                        c, (/*4Hm*/(DGGRSZone)zone, /*4Hm*/(DGGRSZone *)parents))
#define REG_DGGRS_getZoneRefinedCRSVertices(m, c)   REGVMETHOD(DGGRS, getZoneRefinedCRSVertices,   c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone, /*1Ab*/CRS crs, /*1Ab*/int edgeRefinement),         c, (/*4Hm*/(DGGRSZone)zone, /*4Hm*/(CRS)crs, /*4Im*/edgeRefinement))
#define REG_DGGRS_getZoneRefinedWGS84Vertices(m, c) REGVMETHOD(DGGRS, getZoneRefinedWGS84Vertices, c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone, /*1Ab*/int edgeRefinement),                         c, (/*4Hm*/(DGGRSZone)zone, /*4Im*/edgeRefinement))
#define REG_DGGRS_getZoneTextID(m, c)               REGVMETHOD(DGGRS, getZoneTextID,               c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone, C(String) zoneID),                                  c, (/*4Hm*/(DGGRSZone)zone, /*4Im*/zoneID))
#define REG_DGGRS_getZoneWGS84Centroid(m, c)        REGVMETHOD(DGGRS, getZoneWGS84Centroid,        c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone, /*1Ab*/GeoPoint & centroid),                        c, (/*4Hm*/(DGGRSZone)zone, /*4Im*/centroid))
#define REG_DGGRS_getZoneWGS84Extent(m, c)          REGVMETHOD(DGGRS, getZoneWGS84Extent,          c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone, /*1Ab*/GeoExtent & extent),                         c, (/*4Hm*/(DGGRSZone)zone, /*4Im*/extent))
#define REG_DGGRS_getZoneWGS84Vertices(m, c)        REGVMETHOD(DGGRS, getZoneWGS84Vertices,        c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone, /*1Ab*/GeoPoint * vertices),                        c, (/*4Hm*/(DGGRSZone)zone, /*4Hm*/(GeoPoint *)vertices))
#define REG_DGGRS_isZoneCentroidChild(m, c)         REGVMETHOD(DGGRS, isZoneCentroidChild,         c::m, (/*1Ab*/DGGRS & self, /*1Ab*/DGGRSZone zone),                                                    c, (/*4Hm*/(DGGRSZone)zone))
#define REG_DGGRS_listZones(m, c)                   REGVMETHOD(DGGRS, listZones,                   c::m, (/*1Ab*/DGGRS & self, /*1Ab*/int level, /*1Ab*/const GeoExtent & bbox),                          c, (/*4Im*/level, /*4Im*/bbox))

#define REG_DGGRS(c) \
      DGGRS::class_registration(_cpp_class); \
      REG_DGGRS_compactZones(compactZones, c); \
      REG_DGGRS_countSubZones(countSubZones, c); \
      REG_DGGRS_countZoneEdges(countZoneEdges, c); \
      REG_DGGRS_countZones(countZones, c); \
      REG_DGGRS_getFirstSubZone(getFirstSubZone, c); \
      REG_DGGRS_getMaxDGGRSZoneLevel(getMaxDGGRSZoneLevel, c); \
      REG_DGGRS_getRefinementRatio(getRefinementRatio, c); \
      REG_DGGRS_getSubZoneCRSCentroids(getSubZoneCRSCentroids, c); \
      REG_DGGRS_getSubZoneWGS84Centroids(getSubZoneWGS84Centroids, c); \
      REG_DGGRS_getZoneArea(getZoneArea, c); \
      REG_DGGRS_getZoneCRSCentroid(getZoneCRSCentroid, c); \
      REG_DGGRS_getZoneCRSExtent(getZoneCRSExtent, c); \
      REG_DGGRS_getZoneCRSVertices(getZoneCRSVertices, c); \
      REG_DGGRS_getZoneCentroidChild(getZoneCentroidChild, c); \
      REG_DGGRS_getZoneCentroidParent(getZoneCentroidParent, c); \
      REG_DGGRS_getZoneChildren(getZoneChildren, c); \
      REG_DGGRS_getZoneFromCRSCentroid(getZoneFromCRSCentroid, c); \
      REG_DGGRS_getZoneFromTextID(getZoneFromTextID, c); \
      REG_DGGRS_getZoneFromWGS84Centroid(getZoneFromWGS84Centroid, c); \
      REG_DGGRS_getZoneLevel(getZoneLevel, c); \
      REG_DGGRS_getZoneNeighbors(getZoneNeighbors, c); \
      REG_DGGRS_getZoneParents(getZoneParents, c); \
      REG_DGGRS_getZoneRefinedCRSVertices(getZoneRefinedCRSVertices, c); \
      REG_DGGRS_getZoneRefinedWGS84Vertices(getZoneRefinedWGS84Vertices, c); \
      REG_DGGRS_getZoneTextID(getZoneTextID, c); \
      REG_DGGRS_getZoneWGS84Centroid(getZoneWGS84Centroid, c); \
      REG_DGGRS_getZoneWGS84Extent(getZoneWGS84Extent, c); \
      REG_DGGRS_getZoneWGS84Vertices(getZoneWGS84Vertices, c); \
      REG_DGGRS_isZoneCentroidChild(isZoneCentroidChild, c); \
      REG_DGGRS_listZones(listZones, c);

#define DGGRS_VIRTUAL_METHODS_PROTO(c) \
   VIRTUAL_METHOD_PROTO(compactZones, compactZones, c, DGGRS, \
      void, c & _ARG, , /*6Fj*/TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)> & zones); \
   VIRTUAL_METHOD_PROTO(countSubZones, countSubZones, c, DGGRS, \
      uint64, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/int depth); \
   VIRTUAL_METHOD_PROTO(countZoneEdges, countZoneEdges, c, DGGRS, \
      int, c & _ARG, , /*6Fj*/DGGRSZone zone); \
   VIRTUAL_METHOD_PROTO(countZones, countZones, c, DGGRS, \
      uint64, c & _ARG, , /*6Fj*/int level); \
   VIRTUAL_METHOD_PROTO(getFirstSubZone, getFirstSubZone, c, DGGRS, \
      DGGRSZone, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/int relativeDepth); \
   VIRTUAL_METHOD_PROTO(getMaxDGGRSZoneLevel, getMaxDGGRSZoneLevel, c, DGGRS, \
      int, c &, , ); \
   VIRTUAL_METHOD_PROTO(getRefinementRatio, getRefinementRatio, c, DGGRS, \
      int, c &, , ); \
   VIRTUAL_METHOD_PROTO(getSubZoneCRSCentroids, getSubZoneCRSCentroids, c, DGGRS, \
      TArray<Pointd _ARG int _ARG Pointd>, c & _ARG, , /*6Fj*/DGGRSZone parent _ARG /*6Fj*/CRS_ crs _ARG /*6Fj*/int relativeDepth); \
   VIRTUAL_METHOD_PROTO(getSubZoneWGS84Centroids, getSubZoneWGS84Centroids, c, DGGRS, \
      TArray<GeoPoint _ARG int _ARG GeoPoint>, c & _ARG, , /*6Fj*/DGGRSZone parent _ARG /*6Fj*/int relativeDepth); \
   VIRTUAL_METHOD_PROTO(getZoneArea, getZoneArea, c, DGGRS, \
      double, c & _ARG, , /*6Fj*/DGGRSZone zone); \
   VIRTUAL_METHOD_PROTO(getZoneCRSCentroid, getZoneCRSCentroid, c, DGGRS, \
      void, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/CRS_ crs _ARG /*6Fj*/Pointd & centroid); \
   VIRTUAL_METHOD_PROTO(getZoneCRSExtent, getZoneCRSExtent, c, DGGRS, \
      void, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/CRS_ crs _ARG /*6Fj*/CRSExtent & extent); \
   VIRTUAL_METHOD_PROTO(getZoneCRSVertices, getZoneCRSVertices, c, DGGRS, \
      int, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/CRS_ crs _ARG /*6Fj*/Pointd * vertices); \
   VIRTUAL_METHOD_PROTO(getZoneCentroidChild, getZoneCentroidChild, c, DGGRS, \
      DGGRSZone, c & _ARG, , /*6Fj*/DGGRSZone zone); \
   VIRTUAL_METHOD_PROTO(getZoneCentroidParent, getZoneCentroidParent, c, DGGRS, \
      DGGRSZone, c & _ARG, , /*6Fj*/DGGRSZone zone); \
   VIRTUAL_METHOD_PROTO(getZoneChildren, getZoneChildren, c, DGGRS, \
      int, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/DGGRSZone * children); \
   VIRTUAL_METHOD_PROTO(getZoneFromCRSCentroid, getZoneFromCRSCentroid, c, DGGRS, \
      DGGRSZone, c & _ARG, , /*6Fj*/int level _ARG /*6Fj*/CRS_ crs _ARG /*6Fj*/const Pointd & centroid); \
   VIRTUAL_METHOD_PROTO(getZoneFromTextID, getZoneFromTextID, c, DGGRS, \
      DGGRSZone, c & _ARG, , /*6Fj*/constString zoneID); \
   VIRTUAL_METHOD_PROTO(getZoneFromWGS84Centroid, getZoneFromWGS84Centroid, c, DGGRS, \
      DGGRSZone, c & _ARG, , /*6Fj*/int level _ARG /*6Fj*/const GeoPoint & centroid); \
   VIRTUAL_METHOD_PROTO(getZoneLevel, getZoneLevel, c, DGGRS, \
      int, c & _ARG, , /*6Fj*/DGGRSZone zone); \
   VIRTUAL_METHOD_PROTO(getZoneNeighbors, getZoneNeighbors, c, DGGRS, \
      int, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/DGGRSZone * neighbors _ARG /*6Fj*/int * nbType); \
   VIRTUAL_METHOD_PROTO(getZoneParents, getZoneParents, c, DGGRS, \
      int, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/DGGRSZone * parents); \
   VIRTUAL_METHOD_PROTO(getZoneRefinedCRSVertices, getZoneRefinedCRSVertices, c, DGGRS, \
      TArray<Pointd _ARG int _ARG Pointd>, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/CRS_ crs _ARG /*6Fj*/int edgeRefinement); \
   VIRTUAL_METHOD_PROTO(getZoneRefinedWGS84Vertices, getZoneRefinedWGS84Vertices, c, DGGRS, \
      TArray<GeoPoint _ARG int _ARG GeoPoint>, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/int edgeRefinement); \
   VIRTUAL_METHOD_PROTO(getZoneTextID, getZoneTextID, c, DGGRS, \
      void, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/C(String) zoneID); \
   VIRTUAL_METHOD_PROTO(getZoneWGS84Centroid, getZoneWGS84Centroid, c, DGGRS, \
      void, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/GeoPoint & centroid); \
   VIRTUAL_METHOD_PROTO(getZoneWGS84Extent, getZoneWGS84Extent, c, DGGRS, \
      void, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/GeoExtent & extent); \
   VIRTUAL_METHOD_PROTO(getZoneWGS84Vertices, getZoneWGS84Vertices, c, DGGRS, \
      int, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/GeoPoint * vertices); \
   VIRTUAL_METHOD_PROTO(isZoneCentroidChild, isZoneCentroidChild, c, DGGRS, \
      bool, c & _ARG, , /*6Fj*/DGGRSZone zone); \
   VIRTUAL_METHOD_PROTO(listZones, listZones, c, DGGRS, \
      TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)>, c & _ARG, , /*6Fj*/int level _ARG /*6Fj*/const GeoExtent & bbox);

#define DGGRS_VIRTUAL_METHODS(c) \
VIRTUAL_METHOD(compactZones, compactZones, c, DGGRS, \
   void, c & _ARG, , /*6Fj*/TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)> & zones, \
   DGGRS_compactZones(self ? self->impl : (C(DGGRS))null, /*7Al*/((Instance&)zones).impl);); \
VIRTUAL_METHOD(countSubZones, countSubZones, c, DGGRS, \
   uint64, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/int depth, \
   return DGGRS_countSubZones(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/depth);); \
VIRTUAL_METHOD(countZoneEdges, countZoneEdges, c, DGGRS, \
   int, c & _ARG, , /*6Fj*/DGGRSZone zone, \
   return DGGRS_countZoneEdges(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone);); \
VIRTUAL_METHOD(countZones, countZones, c, DGGRS, \
   uint64, c & _ARG, , /*6Fj*/int level, \
   return DGGRS_countZones(self ? self->impl : (C(DGGRS))null, /*7Al*/level);); \
VIRTUAL_METHOD(getFirstSubZone, getFirstSubZone, c, DGGRS, \
   DGGRSZone, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/int relativeDepth, \
   return DGGRS_getFirstSubZone(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/relativeDepth);); \
VIRTUAL_METHOD(getMaxDGGRSZoneLevel, getMaxDGGRSZoneLevel, c, DGGRS, \
   int, c &, , , \
   return DGGRS_getMaxDGGRSZoneLevel(self ? self->impl : (C(DGGRS))null);); \
VIRTUAL_METHOD(getRefinementRatio, getRefinementRatio, c, DGGRS, \
   int, c &, , , \
   return DGGRS_getRefinementRatio(self ? self->impl : (C(DGGRS))null);); \
VIRTUAL_METHOD(getSubZoneCRSCentroids, getSubZoneCRSCentroids, c, DGGRS, \
   TArray<Pointd _ARG int _ARG Pointd>, c & _ARG, , /*6Fj*/DGGRSZone parent _ARG /*6Fj*/CRS_ crs _ARG /*6Fj*/int relativeDepth, \
   C(Array) retArray = DGGRS_getSubZoneCRSCentroids(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))parent, /*7Al*/(C(CRS))crs, /*7Al*/relativeDepth); \
      return TArray<Pointd _ARG int _ARG Pointd>(retArray);); \
VIRTUAL_METHOD(getSubZoneWGS84Centroids, getSubZoneWGS84Centroids, c, DGGRS, \
   TArray<GeoPoint _ARG int _ARG GeoPoint>, c & _ARG, , /*6Fj*/DGGRSZone parent _ARG /*6Fj*/int relativeDepth, \
   C(Array) retArray = DGGRS_getSubZoneWGS84Centroids(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))parent, /*7Al*/relativeDepth); \
      return TArray<GeoPoint _ARG int _ARG GeoPoint>(retArray);); \
VIRTUAL_METHOD(getZoneArea, getZoneArea, c, DGGRS, \
   double, c & _ARG, , /*6Fj*/DGGRSZone zone, \
   return DGGRS_getZoneArea(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone);); \
VIRTUAL_METHOD(getZoneCRSCentroid, getZoneCRSCentroid, c, DGGRS, \
   void, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/CRS_ crs _ARG /*6Fj*/Pointd & centroid, \
   DGGRS_getZoneCRSCentroid(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/(C(CRS))crs, /*7Al*/&centroid.impl);); \
VIRTUAL_METHOD(getZoneCRSExtent, getZoneCRSExtent, c, DGGRS, \
   void, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/CRS_ crs _ARG /*6Fj*/CRSExtent & extent, \
   DGGRS_getZoneCRSExtent(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/(C(CRS))crs, /*7Al*/&extent.impl);); \
VIRTUAL_METHOD(getZoneCRSVertices, getZoneCRSVertices, c, DGGRS, \
   int, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/CRS_ crs _ARG /*6Fj*/Pointd * vertices, \
   return DGGRS_getZoneCRSVertices(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/(C(CRS))crs, /*7Al*/(C(Pointd) *)vertices);); \
VIRTUAL_METHOD(getZoneCentroidChild, getZoneCentroidChild, c, DGGRS, \
   DGGRSZone, c & _ARG, , /*6Fj*/DGGRSZone zone, \
   return DGGRS_getZoneCentroidChild(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone);); \
VIRTUAL_METHOD(getZoneCentroidParent, getZoneCentroidParent, c, DGGRS, \
   DGGRSZone, c & _ARG, , /*6Fj*/DGGRSZone zone, \
   return DGGRS_getZoneCentroidParent(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone);); \
VIRTUAL_METHOD(getZoneChildren, getZoneChildren, c, DGGRS, \
   int, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/DGGRSZone * children, \
   return DGGRS_getZoneChildren(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/(C(DGGRSZone) *)children);); \
VIRTUAL_METHOD(getZoneFromCRSCentroid, getZoneFromCRSCentroid, c, DGGRS, \
   DGGRSZone, c & _ARG, , /*6Fj*/int level _ARG /*6Fj*/CRS_ crs _ARG /*6Fj*/const Pointd & centroid, \
   return DGGRS_getZoneFromCRSCentroid(self ? self->impl : (C(DGGRS))null, /*7Al*/level, /*7Al*/(C(CRS))crs, /*7Al*/&centroid.impl);); \
VIRTUAL_METHOD(getZoneFromTextID, getZoneFromTextID, c, DGGRS, \
   DGGRSZone, c & _ARG, , /*6Fj*/constString zoneID, \
   return DGGRS_getZoneFromTextID(self ? self->impl : (C(DGGRS))null, /*7Al*/zoneID);); \
VIRTUAL_METHOD(getZoneFromWGS84Centroid, getZoneFromWGS84Centroid, c, DGGRS, \
   DGGRSZone, c & _ARG, , /*6Fj*/int level _ARG /*6Fj*/const GeoPoint & centroid, \
   return DGGRS_getZoneFromWGS84Centroid(self ? self->impl : (C(DGGRS))null, /*7Al*/level, /*7Al*/&centroid.impl);); \
VIRTUAL_METHOD(getZoneLevel, getZoneLevel, c, DGGRS, \
   int, c & _ARG, , /*6Fj*/DGGRSZone zone, \
   return DGGRS_getZoneLevel(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone);); \
VIRTUAL_METHOD(getZoneNeighbors, getZoneNeighbors, c, DGGRS, \
   int, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/DGGRSZone * neighbors _ARG /*6Fj*/int * nbType, \
   return DGGRS_getZoneNeighbors(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/(C(DGGRSZone) *)neighbors, /*7Al*/nbType);); \
VIRTUAL_METHOD(getZoneParents, getZoneParents, c, DGGRS, \
   int, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/DGGRSZone * parents, \
   return DGGRS_getZoneParents(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/(C(DGGRSZone) *)parents);); \
VIRTUAL_METHOD(getZoneRefinedCRSVertices, getZoneRefinedCRSVertices, c, DGGRS, \
   TArray<Pointd _ARG int _ARG Pointd>, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/CRS_ crs _ARG /*6Fj*/int edgeRefinement, \
   C(Array) retArray = DGGRS_getZoneRefinedCRSVertices(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/(C(CRS))crs, /*7Al*/edgeRefinement); \
      return TArray<Pointd _ARG int _ARG Pointd>(retArray);); \
VIRTUAL_METHOD(getZoneRefinedWGS84Vertices, getZoneRefinedWGS84Vertices, c, DGGRS, \
   TArray<GeoPoint _ARG int _ARG GeoPoint>, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/int edgeRefinement, \
   C(Array) retArray = DGGRS_getZoneRefinedWGS84Vertices(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/edgeRefinement); \
      return TArray<GeoPoint _ARG int _ARG GeoPoint>(retArray);); \
VIRTUAL_METHOD(getZoneTextID, getZoneTextID, c, DGGRS, \
   void, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/C(String) zoneID, \
   DGGRS_getZoneTextID(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/zoneID);); \
VIRTUAL_METHOD(getZoneWGS84Centroid, getZoneWGS84Centroid, c, DGGRS, \
   void, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/GeoPoint & centroid, \
   DGGRS_getZoneWGS84Centroid(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/&centroid.impl);); \
VIRTUAL_METHOD(getZoneWGS84Extent, getZoneWGS84Extent, c, DGGRS, \
   void, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/GeoExtent & extent, \
   DGGRS_getZoneWGS84Extent(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/&extent.impl);); \
VIRTUAL_METHOD(getZoneWGS84Vertices, getZoneWGS84Vertices, c, DGGRS, \
   int, c & _ARG, , /*6Fj*/DGGRSZone zone _ARG /*6Fj*/GeoPoint * vertices, \
   return DGGRS_getZoneWGS84Vertices(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/(C(GeoPoint) *)vertices);); \
VIRTUAL_METHOD(isZoneCentroidChild, isZoneCentroidChild, c, DGGRS, \
   bool, c & _ARG, , /*6Fj*/DGGRSZone zone, \
   return (bool)DGGRS_isZoneCentroidChild(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone);); \
VIRTUAL_METHOD(listZones, listZones, c, DGGRS, \
   TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)>, c & _ARG, , /*6Fj*/int level _ARG /*6Fj*/const GeoExtent & bbox, \
   C(Array) retArray = DGGRS_listZones(self ? self->impl : (C(DGGRS))null, /*7Al*/level, /*7Al*/&bbox.impl); \
      return TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)>(retArray););

class DGGRS : public Instance
{
public:
   inline DGGRS(DGGRS && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline DGGRS & operator= (DGGRS && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   DGGRS() : DGGRS((C(Instance))Instance_newEx(_cpp_class.impl, false), _cpp_class) { }
   struct Instance_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(Instance & , /*6Bj*/Instance & object);
      inline FunctionType operator= (FunctionType func);
      inline int operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & object);
   } onCompare;
   // inline static void register_onCompare(CPPClass & cl, Instance::Instance_onCompare_Functor::FunctionType func)

   struct Instance_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Bj*/Instance & newData);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & newData);
   } onCopy;
   // inline static void register_onCopy(CPPClass & cl, Instance::Instance_onCopy_Functor::FunctionType func)

   struct Instance_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;
   // inline static void register_onDisplay(CPPClass & cl, Instance::Instance_onDisplay_Functor::FunctionType func)

   struct Instance_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef Instance & (* FunctionType)(Instance & , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
      inline FunctionType operator= (FunctionType func);
      inline Instance & operator()(/*6Bk*/Instance & o_ , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;
   // inline static void register_onEdit(CPPClass & cl, Instance::Instance_onEdit_Functor::FunctionType func)

   struct Instance_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ );
   } onFree;
   // inline static void register_onFree(CPPClass & cl, Instance::Instance_onFree_Functor::FunctionType func)

   struct Instance_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/const char * string);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/const char * string);
   } onGetDataFromString;
   // inline static void register_onGetDataFromString(CPPClass & cl, Instance::Instance_onGetDataFromString_Functor::FunctionType func)

   struct Instance_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef const char * (* FunctionType)(Instance & , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
      inline FunctionType operator= (FunctionType func);
      inline const char * operator()(/*6Bk*/Instance & o_ , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;
   // inline static void register_onGetString(CPPClass & cl, Instance::Instance_onGetString_Functor::FunctionType func)

   struct Instance_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/Window & window, /*6Fj*/void * object);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/Window & window, /*6Fj*/void * object);
   } onSaveEdit;
   // inline static void register_onSaveEdit(CPPClass & cl, Instance::Instance_onSaveEdit_Functor::FunctionType func)

   struct Instance_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onSerialize;
   // inline static void register_onSerialize(CPPClass & cl, Instance::Instance_onSerialize_Functor::FunctionType func)

   struct Instance_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onUnserialize;
   // inline static void register_onUnserialize(CPPClass & cl, Instance::Instance_onUnserialize_Functor::FunctionType func)

   static TCPPClass<DGGRS> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         DGGRS * inst = new DGGRS(i, _cpp_class);
         if(inst)
         {
            /* printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      DGGRS * inst = (DGGRS *)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor)
            ((void (*)(DGGRS & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree)
            delete inst;
      }
   }
   explicit inline DGGRS(C(Instance) _impl, CPPClass & cl = _cpp_class) : Instance(_impl, cl) { }

   inline C(bool) areZonesNeighbors(/*1Ab*/DGGRSZone a, /*1Ab*/DGGRSZone b); // DGGRS_areZonesNeighbors
   inline C(bool) areZonesSiblings(/*1Ab*/DGGRSZone a, /*1Ab*/DGGRSZone b); // DGGRS_areZonesSiblings
   inline C(bool) doZonesOverlap(/*1Ab*/DGGRSZone a, /*1Ab*/DGGRSZone b); // DGGRS_doZonesOverlap
   inline C(bool) doesZoneContain(/*1Ab*/DGGRSZone hayStack, /*1Ab*/DGGRSZone needle); // DGGRS_doesZoneContain
   inline int get64KDepth(); // DGGRS_get64KDepth
   inline int getLevelFromMetersPerSubZone(/*1Ab*/double physicalMetersPerSubZone, /*1Ab*/int relativeDepth); // DGGRS_getLevelFromMetersPerSubZone
   inline int getLevelFromPixelsAndExtent(/*1Ab*/const GeoExtent & extent, /*1Ab*/Point & pixels, /*1Ab*/int relativeDepth); // DGGRS_getLevelFromPixelsAndExtent
   inline int getLevelFromPixelsAndExtent(/*1Ac*/const GeoExtent * extent, /*1Ac*/Point * pixels, /*1Ac*/int relativeDepth); // DGGRS_getLevelFromPixelsAndExtent
   inline int getLevelFromRefZoneArea(/*1Ab*/double metersSquared); // DGGRS_getLevelFromRefZoneArea
   inline int getLevelFromScaleDenominator(/*1Ab*/double scaleDenominator, /*1Ab*/int relativeDepth, /*1Ab*/double mmPerPixel); // DGGRS_getLevelFromScaleDenominator
   inline int getMaxDepth(); // DGGRS_getMaxDepth
   inline double getMetersPerSubZoneFromLevel(/*1Ab*/int parentLevel, /*1Ab*/int relativeDepth); // DGGRS_getMetersPerSubZoneFromLevel
   inline double getRefZoneArea(/*1Ab*/int level); // DGGRS_getRefZoneArea
   inline double getScaleDenominatorFromLevel(/*1Ab*/int parentLevel, /*1Ab*/int relativeDepth, /*1Ab*/double mmPerPixel); // DGGRS_getScaleDenominatorFromLevel
   inline C(DGGRSZone) getSubZoneAtIndex(/*1Ab*/DGGRSZone parent, /*1Ab*/int relativeDepth, /*1Ab*/int index); // DGGRS_getSubZoneAtIndex
   inline int getSubZoneIndex(/*1Ab*/DGGRSZone parent, /*1Ab*/DGGRSZone subZone); // DGGRS_getSubZoneIndex
   inline TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)> getSubZones(/*1Ab*/DGGRSZone parent, /*1Ab*/int relativeDepth); // DGGRS_getSubZones
   inline C(bool) isZoneAncestorOf(/*1Ab*/DGGRSZone ancestor, /*1Ab*/DGGRSZone descendant, /*1Ab*/int maxDepth); // DGGRS_isZoneAncestorOf
   inline C(bool) isZoneContainedIn(/*1Ab*/DGGRSZone needle, /*1Ab*/DGGRSZone hayStack); // DGGRS_isZoneContainedIn
   inline C(bool) isZoneDescendantOf(/*1Ab*/DGGRSZone descendant, /*1Ab*/DGGRSZone ancestor, /*1Ab*/int maxDepth); // DGGRS_isZoneDescendantOf
   inline C(bool) isZoneImmediateChildOf(/*1Ab*/DGGRSZone child, /*1Ab*/DGGRSZone parent); // DGGRS_isZoneImmediateChildOf
   inline C(bool) isZoneImmediateParentOf(/*1Ab*/DGGRSZone parent, /*1Ab*/DGGRSZone child); // DGGRS_isZoneImmediateParentOf

   struct DGGRS_compactZones_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(DGGRS & , /*6Fj*/TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)> & zones);
      inline FunctionType operator= (FunctionType func);
      inline void operator()( /*6Fj*/TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)> & zones);
   } compactZones;
   // inline static void register_compactZones(CPPClass & cl, DGGRS::DGGRS_compactZones_Functor::FunctionType func)

   struct DGGRS_countSubZones_Functor
   {
      [[no_unique_address]] int _[0];
      typedef uint64 (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone, /*6Fj*/int depth);
      inline FunctionType operator= (FunctionType func);
      inline uint64 operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/int depth);
   } countSubZones;
   // inline static void register_countSubZones(CPPClass & cl, DGGRS::DGGRS_countSubZones_Functor::FunctionType func)

   struct DGGRS_countZoneEdges_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone);
      inline FunctionType operator= (FunctionType func);
      inline int operator()( /*6Fj*/DGGRSZone zone);
   } countZoneEdges;
   // inline static void register_countZoneEdges(CPPClass & cl, DGGRS::DGGRS_countZoneEdges_Functor::FunctionType func)

   struct DGGRS_countZones_Functor
   {
      [[no_unique_address]] int _[0];
      typedef uint64 (* FunctionType)(DGGRS & , /*6Fj*/int level);
      inline FunctionType operator= (FunctionType func);
      inline uint64 operator()( /*6Fj*/int level);
   } countZones;
   // inline static void register_countZones(CPPClass & cl, DGGRS::DGGRS_countZones_Functor::FunctionType func)

   struct DGGRS_getFirstSubZone_Functor
   {
      [[no_unique_address]] int _[0];
      typedef DGGRSZone (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone, /*6Fj*/int relativeDepth);
      inline FunctionType operator= (FunctionType func);
      inline DGGRSZone operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/int relativeDepth);
   } getFirstSubZone;
   // inline static void register_getFirstSubZone(CPPClass & cl, DGGRS::DGGRS_getFirstSubZone_Functor::FunctionType func)

   struct DGGRS_getMaxDGGRSZoneLevel_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(DGGRS &);
      inline FunctionType operator= (FunctionType func);
      inline int operator()( );
   } getMaxDGGRSZoneLevel;
   // inline static void register_getMaxDGGRSZoneLevel(CPPClass & cl, DGGRS::DGGRS_getMaxDGGRSZoneLevel_Functor::FunctionType func)

   struct DGGRS_getRefinementRatio_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(DGGRS &);
      inline FunctionType operator= (FunctionType func);
      inline int operator()( );
   } getRefinementRatio;
   // inline static void register_getRefinementRatio(CPPClass & cl, DGGRS::DGGRS_getRefinementRatio_Functor::FunctionType func)

   struct DGGRS_getSubZoneCRSCentroids_Functor
   {
      [[no_unique_address]] int _[0];
      typedef TArray<Pointd _ARG int _ARG Pointd> (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone parent, /*6Fj*/CRS_ crs, /*6Fj*/int relativeDepth);
      inline FunctionType operator= (FunctionType func);
      inline TArray<Pointd _ARG int _ARG Pointd> operator()( /*6Fj*/DGGRSZone parent, /*6Fj*/CRS_ crs, /*6Fj*/int relativeDepth);
   } getSubZoneCRSCentroids;
   // inline static void register_getSubZoneCRSCentroids(CPPClass & cl, DGGRS::DGGRS_getSubZoneCRSCentroids_Functor::FunctionType func)

   struct DGGRS_getSubZoneWGS84Centroids_Functor
   {
      [[no_unique_address]] int _[0];
      typedef TArray<GeoPoint _ARG int _ARG GeoPoint> (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone parent, /*6Fj*/int relativeDepth);
      inline FunctionType operator= (FunctionType func);
      inline TArray<GeoPoint _ARG int _ARG GeoPoint> operator()( /*6Fj*/DGGRSZone parent, /*6Fj*/int relativeDepth);
   } getSubZoneWGS84Centroids;
   // inline static void register_getSubZoneWGS84Centroids(CPPClass & cl, DGGRS::DGGRS_getSubZoneWGS84Centroids_Functor::FunctionType func)

   struct DGGRS_getZoneArea_Functor
   {
      [[no_unique_address]] int _[0];
      typedef double (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone);
      inline FunctionType operator= (FunctionType func);
      inline double operator()( /*6Fj*/DGGRSZone zone);
   } getZoneArea;
   // inline static void register_getZoneArea(CPPClass & cl, DGGRS::DGGRS_getZoneArea_Functor::FunctionType func)

   struct DGGRS_getZoneCRSCentroid_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone, /*6Fj*/CRS_ crs, /*6Fj*/Pointd & centroid);
      inline FunctionType operator= (FunctionType func);
      inline void operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/CRS_ crs, /*6Fj*/Pointd & centroid);
   } getZoneCRSCentroid;
   // inline static void register_getZoneCRSCentroid(CPPClass & cl, DGGRS::DGGRS_getZoneCRSCentroid_Functor::FunctionType func)

   struct DGGRS_getZoneCRSExtent_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone, /*6Fj*/CRS_ crs, /*6Fj*/CRSExtent & extent);
      inline FunctionType operator= (FunctionType func);
      inline void operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/CRS_ crs, /*6Fj*/CRSExtent & extent);
   } getZoneCRSExtent;
   // inline static void register_getZoneCRSExtent(CPPClass & cl, DGGRS::DGGRS_getZoneCRSExtent_Functor::FunctionType func)

   struct DGGRS_getZoneCRSVertices_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone, /*6Fj*/CRS_ crs, /*6Fj*/Pointd * vertices);
      inline FunctionType operator= (FunctionType func);
      inline int operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/CRS_ crs, /*6Fj*/Pointd * vertices);
   } getZoneCRSVertices;
   // inline static void register_getZoneCRSVertices(CPPClass & cl, DGGRS::DGGRS_getZoneCRSVertices_Functor::FunctionType func)

   struct DGGRS_getZoneCentroidChild_Functor
   {
      [[no_unique_address]] int _[0];
      typedef DGGRSZone (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone);
      inline FunctionType operator= (FunctionType func);
      inline DGGRSZone operator()( /*6Fj*/DGGRSZone zone);
   } getZoneCentroidChild;
   // inline static void register_getZoneCentroidChild(CPPClass & cl, DGGRS::DGGRS_getZoneCentroidChild_Functor::FunctionType func)

   struct DGGRS_getZoneCentroidParent_Functor
   {
      [[no_unique_address]] int _[0];
      typedef DGGRSZone (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone);
      inline FunctionType operator= (FunctionType func);
      inline DGGRSZone operator()( /*6Fj*/DGGRSZone zone);
   } getZoneCentroidParent;
   // inline static void register_getZoneCentroidParent(CPPClass & cl, DGGRS::DGGRS_getZoneCentroidParent_Functor::FunctionType func)

   struct DGGRS_getZoneChildren_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone, /*6Fj*/DGGRSZone * children);
      inline FunctionType operator= (FunctionType func);
      inline int operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/DGGRSZone * children);
   } getZoneChildren;
   // inline static void register_getZoneChildren(CPPClass & cl, DGGRS::DGGRS_getZoneChildren_Functor::FunctionType func)

   struct DGGRS_getZoneFromCRSCentroid_Functor
   {
      [[no_unique_address]] int _[0];
      typedef DGGRSZone (* FunctionType)(DGGRS & , /*6Fj*/int level, /*6Fj*/CRS_ crs, /*6Fj*/const Pointd & centroid);
      inline FunctionType operator= (FunctionType func);
      inline DGGRSZone operator()( /*6Fj*/int level, /*6Fj*/CRS_ crs, /*6Fj*/const Pointd & centroid);
   } getZoneFromCRSCentroid;
   // inline static void register_getZoneFromCRSCentroid(CPPClass & cl, DGGRS::DGGRS_getZoneFromCRSCentroid_Functor::FunctionType func)

   struct DGGRS_getZoneFromTextID_Functor
   {
      [[no_unique_address]] int _[0];
      typedef DGGRSZone (* FunctionType)(DGGRS & , /*6Fj*/constString zoneID);
      inline FunctionType operator= (FunctionType func);
      inline DGGRSZone operator()( /*6Fj*/constString zoneID);
   } getZoneFromTextID;
   // inline static void register_getZoneFromTextID(CPPClass & cl, DGGRS::DGGRS_getZoneFromTextID_Functor::FunctionType func)

   struct DGGRS_getZoneFromWGS84Centroid_Functor
   {
      [[no_unique_address]] int _[0];
      typedef DGGRSZone (* FunctionType)(DGGRS & , /*6Fj*/int level, /*6Fj*/const GeoPoint & centroid);
      inline FunctionType operator= (FunctionType func);
      inline DGGRSZone operator()( /*6Fj*/int level, /*6Fj*/const GeoPoint & centroid);
   } getZoneFromWGS84Centroid;
   // inline static void register_getZoneFromWGS84Centroid(CPPClass & cl, DGGRS::DGGRS_getZoneFromWGS84Centroid_Functor::FunctionType func)

   struct DGGRS_getZoneLevel_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone);
      inline FunctionType operator= (FunctionType func);
      inline int operator()( /*6Fj*/DGGRSZone zone);
   } getZoneLevel;
   // inline static void register_getZoneLevel(CPPClass & cl, DGGRS::DGGRS_getZoneLevel_Functor::FunctionType func)

   struct DGGRS_getZoneNeighbors_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone, /*6Fj*/DGGRSZone * neighbors, /*6Fj*/int * nbType);
      inline FunctionType operator= (FunctionType func);
      inline int operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/DGGRSZone * neighbors, /*6Fj*/int * nbType);
   } getZoneNeighbors;
   // inline static void register_getZoneNeighbors(CPPClass & cl, DGGRS::DGGRS_getZoneNeighbors_Functor::FunctionType func)

   struct DGGRS_getZoneParents_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone, /*6Fj*/DGGRSZone * parents);
      inline FunctionType operator= (FunctionType func);
      inline int operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/DGGRSZone * parents);
   } getZoneParents;
   // inline static void register_getZoneParents(CPPClass & cl, DGGRS::DGGRS_getZoneParents_Functor::FunctionType func)

   struct DGGRS_getZoneRefinedCRSVertices_Functor
   {
      [[no_unique_address]] int _[0];
      typedef TArray<Pointd _ARG int _ARG Pointd> (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone, /*6Fj*/CRS_ crs, /*6Fj*/int edgeRefinement);
      inline FunctionType operator= (FunctionType func);
      inline TArray<Pointd _ARG int _ARG Pointd> operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/CRS_ crs, /*6Fj*/int edgeRefinement);
   } getZoneRefinedCRSVertices;
   // inline static void register_getZoneRefinedCRSVertices(CPPClass & cl, DGGRS::DGGRS_getZoneRefinedCRSVertices_Functor::FunctionType func)

   struct DGGRS_getZoneRefinedWGS84Vertices_Functor
   {
      [[no_unique_address]] int _[0];
      typedef TArray<GeoPoint _ARG int _ARG GeoPoint> (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone, /*6Fj*/int edgeRefinement);
      inline FunctionType operator= (FunctionType func);
      inline TArray<GeoPoint _ARG int _ARG GeoPoint> operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/int edgeRefinement);
   } getZoneRefinedWGS84Vertices;
   // inline static void register_getZoneRefinedWGS84Vertices(CPPClass & cl, DGGRS::DGGRS_getZoneRefinedWGS84Vertices_Functor::FunctionType func)

   struct DGGRS_getZoneTextID_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone, /*6Fj*/C(String) zoneID);
      inline FunctionType operator= (FunctionType func);
      inline void operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/C(String) zoneID);
   } getZoneTextID;
   // inline static void register_getZoneTextID(CPPClass & cl, DGGRS::DGGRS_getZoneTextID_Functor::FunctionType func)

   struct DGGRS_getZoneWGS84Centroid_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone, /*6Fj*/GeoPoint & centroid);
      inline FunctionType operator= (FunctionType func);
      inline void operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/GeoPoint & centroid);
   } getZoneWGS84Centroid;
   // inline static void register_getZoneWGS84Centroid(CPPClass & cl, DGGRS::DGGRS_getZoneWGS84Centroid_Functor::FunctionType func)

   struct DGGRS_getZoneWGS84Extent_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone, /*6Fj*/GeoExtent & extent);
      inline FunctionType operator= (FunctionType func);
      inline void operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/GeoExtent & extent);
   } getZoneWGS84Extent;
   // inline static void register_getZoneWGS84Extent(CPPClass & cl, DGGRS::DGGRS_getZoneWGS84Extent_Functor::FunctionType func)

   struct DGGRS_getZoneWGS84Vertices_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone, /*6Fj*/GeoPoint * vertices);
      inline FunctionType operator= (FunctionType func);
      inline int operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/GeoPoint * vertices);
   } getZoneWGS84Vertices;
   // inline static void register_getZoneWGS84Vertices(CPPClass & cl, DGGRS::DGGRS_getZoneWGS84Vertices_Functor::FunctionType func)

   struct DGGRS_isZoneCentroidChild_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(DGGRS & , /*6Fj*/DGGRSZone zone);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()( /*6Fj*/DGGRSZone zone);
   } isZoneCentroidChild;
   // inline static void register_isZoneCentroidChild(CPPClass & cl, DGGRS::DGGRS_isZoneCentroidChild_Functor::FunctionType func)

   struct DGGRS_listZones_Functor
   {
      [[no_unique_address]] int _[0];
      typedef TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)> (* FunctionType)(DGGRS & , /*6Fj*/int level, /*6Fj*/const GeoExtent & bbox);
      inline FunctionType operator= (FunctionType func);
      inline TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)> operator()( /*6Fj*/int level, /*6Fj*/const GeoExtent & bbox);
   } listZones;
   // inline static void register_listZones(CPPClass & cl, DGGRS::DGGRS_listZones_Functor::FunctionType func)

   static void class_registration(CPPClass & _cpp_class);
};

enum class FieldType : int
{
   integer = FieldType_integer,
   real = FieldType_real,
   text = FieldType_text,
   blob = FieldType_blob,
   nil = FieldType_nil,
   array = FieldType_array,
   map = FieldType_map
};

// REVIEW: Why does this get an underscore?
class CRS_
{
public:
   C(CRS) impl;
   constexpr CRS_() : impl(0) { }
   constexpr CRS_(C(CRS) impl) : impl(impl) { }
   CRS_(CRSRegistry registry, int crsID, bool h);
   operator C(CRS)() { return impl; }
   CRS_ & operator =(C(CRS) impl) { impl = impl; return *this; }
   bool operator ==(const CRS_ & value) const { return impl == value.impl; }
   bool operator !=(const CRS_ & value) const { return impl != value.impl; }

   struct registry_Prop
   {
      constexpr registry_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/CRSRegistry operator= (/*0H*/CRSRegistry v);
      /*regSet*/inline CRS_::registry_Prop & operator= (CRS_::registry_Prop & prop);
      /*regGet*/inline operator /*0I*/CRSRegistry () const;
   } registry;
   struct crsID_Prop
   {
      constexpr crsID_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline CRS_::crsID_Prop & operator= (CRS_::crsID_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } crsID;
   struct h_Prop
   {
      constexpr h_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline CRS_::h_Prop & operator= (CRS_::h_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } h;
};

struct CRSExtent : public TStruct<CRSExtent>
{
public:
   C(CRSExtent) impl;
   constexpr CRSExtent() : impl({}) { }
   constexpr CRSExtent(const C(CRSExtent) impl) : impl(impl) { }

   inline CRSExtent(CRS_ crs, Pointd tl, Pointd br);

   struct crs_Prop
   {
      constexpr crs_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/CRS_ operator= (/*0H*/CRS_ v);
      /*regSet*/inline CRSExtent::crs_Prop & operator= (CRSExtent::crs_Prop & prop);
      /*regGet*/inline operator /*0I*/CRS_ () const;
   } crs;
   struct tl_Prop
   {
      constexpr tl_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Pointd operator= (/*0H*/Pointd v);
      /*regSet*/inline CRSExtent::tl_Prop & operator= (CRSExtent::tl_Prop & prop);
      /*regGet*/inline operator /*0I*/Pointd () const;
   } tl;
   struct br_Prop
   {
      constexpr br_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Pointd operator= (/*0H*/Pointd v);
      /*regSet*/inline CRSExtent::br_Prop & operator= (CRSExtent::br_Prop & prop);
      /*regGet*/inline operator /*0I*/Pointd () const;
   } br;
};

enum class CRSRegistry : int
{
   epsg = CRSRegistry_epsg,
   ogc = CRSRegistry_ogc
};


#define REG_DGGSJSON(c) \
      DGGSJSON::class_registration(_cpp_class);

#define DGGSJSON_VIRTUAL_METHODS_PROTO(c) \

#define DGGSJSON_VIRTUAL_METHODS(c) \

class DGGSJSON : public Instance
{
public:
   inline DGGSJSON(DGGSJSON && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline DGGSJSON & operator= (DGGSJSON && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   DGGSJSON() : DGGSJSON((C(Instance))Instance_newEx(_cpp_class.impl, false), _cpp_class) { }
   struct Instance_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(Instance & , /*6Bj*/Instance & object);
      inline FunctionType operator= (FunctionType func);
      inline int operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & object);
   } onCompare;
   // inline static void register_onCompare(CPPClass & cl, Instance::Instance_onCompare_Functor::FunctionType func)

   struct Instance_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Bj*/Instance & newData);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & newData);
   } onCopy;
   // inline static void register_onCopy(CPPClass & cl, Instance::Instance_onCopy_Functor::FunctionType func)

   struct Instance_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;
   // inline static void register_onDisplay(CPPClass & cl, Instance::Instance_onDisplay_Functor::FunctionType func)

   struct Instance_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef Instance & (* FunctionType)(Instance & , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
      inline FunctionType operator= (FunctionType func);
      inline Instance & operator()(/*6Bk*/Instance & o_ , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;
   // inline static void register_onEdit(CPPClass & cl, Instance::Instance_onEdit_Functor::FunctionType func)

   struct Instance_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ );
   } onFree;
   // inline static void register_onFree(CPPClass & cl, Instance::Instance_onFree_Functor::FunctionType func)

   struct Instance_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/const char * string);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/const char * string);
   } onGetDataFromString;
   // inline static void register_onGetDataFromString(CPPClass & cl, Instance::Instance_onGetDataFromString_Functor::FunctionType func)

   struct Instance_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef const char * (* FunctionType)(Instance & , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
      inline FunctionType operator= (FunctionType func);
      inline const char * operator()(/*6Bk*/Instance & o_ , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;
   // inline static void register_onGetString(CPPClass & cl, Instance::Instance_onGetString_Functor::FunctionType func)

   struct Instance_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/Window & window, /*6Fj*/void * object);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/Window & window, /*6Fj*/void * object);
   } onSaveEdit;
   // inline static void register_onSaveEdit(CPPClass & cl, Instance::Instance_onSaveEdit_Functor::FunctionType func)

   struct Instance_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onSerialize;
   // inline static void register_onSerialize(CPPClass & cl, Instance::Instance_onSerialize_Functor::FunctionType func)

   struct Instance_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onUnserialize;
   // inline static void register_onUnserialize(CPPClass & cl, Instance::Instance_onUnserialize_Functor::FunctionType func)

   static TCPPClass<DGGSJSON> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         DGGSJSON * inst = new DGGSJSON(i, _cpp_class);
         if(inst)
         {
            /* printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      DGGSJSON * inst = (DGGSJSON *)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor)
            ((void (*)(DGGSJSON & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree)
            delete inst;
      }
   }
   explicit inline DGGSJSON(C(Instance) _impl, CPPClass & cl = _cpp_class) : Instance(_impl, cl) { }

   static void class_registration(CPPClass & _cpp_class);

   inline DGGSJSON(char * dggrs, char * zoneId);

   struct dggrs_Prop
   {
      constexpr dggrs_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline DGGSJSON::dggrs_Prop & operator= (DGGSJSON::dggrs_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } dggrs;
   struct zoneId_Prop
   {
      constexpr zoneId_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline DGGSJSON::zoneId_Prop & operator= (DGGSJSON::zoneId_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } zoneId;
   struct depths_Prop
   {
      constexpr depths_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TArray<int _ARG int _ARG int> operator= (/*0H*/TArray<int _ARG int _ARG int> v);
      /*regSet*/inline DGGSJSON::depths_Prop & operator= (DGGSJSON::depths_Prop & prop);
      /*regGet*/inline operator /*0I*/TArray<int _ARG int _ARG int> () const;
   } depths;
   struct representedValue_Prop
   {
      constexpr representedValue_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline DGGSJSON::representedValue_Prop & operator= (DGGSJSON::representedValue_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } representedValue;
   struct schema_Prop
   {
      constexpr schema_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/JSONSchema operator= (/*0H*/JSONSchema v);
      /*regSet*/inline DGGSJSON::schema_Prop & operator= (DGGSJSON::schema_Prop & prop);
      /*regGet*/inline operator /*0I*/JSONSchema () const;
   } schema;
   struct dimensions_Prop
   {
      constexpr dimensions_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Array operator= (/*0H*/Array v);
      /*regSet*/inline DGGSJSON::dimensions_Prop & operator= (DGGSJSON::dimensions_Prop & prop);
      /*regGet*/inline operator /*0I*/Array () const;
   } dimensions;
   struct values_Prop
   {
      constexpr values_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TMap<C(String) _ARG T(Array, DGGSJSONDepth)> operator= (/*0H*/TMap<C(String) _ARG T(Array, DGGSJSONDepth)> v);
      /*regSet*/inline DGGSJSON::values_Prop & operator= (DGGSJSON::values_Prop & prop);
      /*regGet*/inline operator /*0I*/TMap<C(String) _ARG T(Array, DGGSJSONDepth)> () const;
   } values;
};


#define REG_DGGSJSONDepth(c) \
      DGGSJSONDepth::class_registration(_cpp_class);

#define DGGSJSONDEPTH_VIRTUAL_METHODS_PROTO(c) \

#define DGGSJSONDEPTH_VIRTUAL_METHODS(c) \

class DGGSJSONDepth : public Instance
{
public:
   inline DGGSJSONDepth(DGGSJSONDepth && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline DGGSJSONDepth & operator= (DGGSJSONDepth && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   DGGSJSONDepth() : DGGSJSONDepth((C(Instance))Instance_newEx(_cpp_class.impl, false), _cpp_class) { }
   struct Instance_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(Instance & , /*6Bj*/Instance & object);
      inline FunctionType operator= (FunctionType func);
      inline int operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & object);
   } onCompare;
   // inline static void register_onCompare(CPPClass & cl, Instance::Instance_onCompare_Functor::FunctionType func)

   struct Instance_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Bj*/Instance & newData);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & newData);
   } onCopy;
   // inline static void register_onCopy(CPPClass & cl, Instance::Instance_onCopy_Functor::FunctionType func)

   struct Instance_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;
   // inline static void register_onDisplay(CPPClass & cl, Instance::Instance_onDisplay_Functor::FunctionType func)

   struct Instance_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef Instance & (* FunctionType)(Instance & , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
      inline FunctionType operator= (FunctionType func);
      inline Instance & operator()(/*6Bk*/Instance & o_ , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;
   // inline static void register_onEdit(CPPClass & cl, Instance::Instance_onEdit_Functor::FunctionType func)

   struct Instance_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ );
   } onFree;
   // inline static void register_onFree(CPPClass & cl, Instance::Instance_onFree_Functor::FunctionType func)

   struct Instance_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/const char * string);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/const char * string);
   } onGetDataFromString;
   // inline static void register_onGetDataFromString(CPPClass & cl, Instance::Instance_onGetDataFromString_Functor::FunctionType func)

   struct Instance_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef const char * (* FunctionType)(Instance & , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
      inline FunctionType operator= (FunctionType func);
      inline const char * operator()(/*6Bk*/Instance & o_ , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;
   // inline static void register_onGetString(CPPClass & cl, Instance::Instance_onGetString_Functor::FunctionType func)

   struct Instance_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/Window & window, /*6Fj*/void * object);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/Window & window, /*6Fj*/void * object);
   } onSaveEdit;
   // inline static void register_onSaveEdit(CPPClass & cl, Instance::Instance_onSaveEdit_Functor::FunctionType func)

   struct Instance_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onSerialize;
   // inline static void register_onSerialize(CPPClass & cl, Instance::Instance_onSerialize_Functor::FunctionType func)

   struct Instance_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onUnserialize;
   // inline static void register_onUnserialize(CPPClass & cl, Instance::Instance_onUnserialize_Functor::FunctionType func)

   static TCPPClass<DGGSJSONDepth> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         DGGSJSONDepth * inst = new DGGSJSONDepth(i, _cpp_class);
         if(inst)
         {
            /* printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      DGGSJSONDepth * inst = (DGGSJSONDepth *)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor)
            ((void (*)(DGGSJSONDepth & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree)
            delete inst;
      }
   }
   explicit inline DGGSJSONDepth(C(Instance) _impl, CPPClass & cl = _cpp_class) : Instance(_impl, cl) { }

   static void class_registration(CPPClass & _cpp_class);

   inline DGGSJSONDepth(int depth);

   struct depth_Prop
   {
      constexpr depth_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline DGGSJSONDepth::depth_Prop & operator= (DGGSJSONDepth::depth_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } depth;
   struct shape_Prop
   {
      constexpr shape_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/DGGSJSONShape operator= (/*0H*/DGGSJSONShape v);
      /*regSet*/inline DGGSJSONDepth::shape_Prop & operator= (DGGSJSONDepth::shape_Prop & prop);
      /*regGet*/inline operator /*0I*/DGGSJSONShape () const;
   } shape;
   struct data_Prop
   {
      constexpr data_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> operator= (/*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v);
      /*regSet*/inline DGGSJSONDepth::data_Prop & operator= (DGGSJSONDepth::data_Prop & prop);
      /*regGet*/inline operator /*0I*/TArray<FieldValue _ARG int _ARG FieldValue> () const;
   } data;
};


#define REG_DGGSJSONGrid(c) \
      DGGSJSONGrid::class_registration(_cpp_class);

#define DGGSJSONGRID_VIRTUAL_METHODS_PROTO(c) \

#define DGGSJSONGRID_VIRTUAL_METHODS(c) \

class DGGSJSONGrid : public Instance
{
public:
   inline DGGSJSONGrid(DGGSJSONGrid && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline DGGSJSONGrid & operator= (DGGSJSONGrid && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   DGGSJSONGrid() : DGGSJSONGrid((C(Instance))Instance_newEx(_cpp_class.impl, false), _cpp_class) { }
   struct Instance_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(Instance & , /*6Bj*/Instance & object);
      inline FunctionType operator= (FunctionType func);
      inline int operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & object);
   } onCompare;
   // inline static void register_onCompare(CPPClass & cl, Instance::Instance_onCompare_Functor::FunctionType func)

   struct Instance_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Bj*/Instance & newData);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & newData);
   } onCopy;
   // inline static void register_onCopy(CPPClass & cl, Instance::Instance_onCopy_Functor::FunctionType func)

   struct Instance_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;
   // inline static void register_onDisplay(CPPClass & cl, Instance::Instance_onDisplay_Functor::FunctionType func)

   struct Instance_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef Instance & (* FunctionType)(Instance & , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
      inline FunctionType operator= (FunctionType func);
      inline Instance & operator()(/*6Bk*/Instance & o_ , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;
   // inline static void register_onEdit(CPPClass & cl, Instance::Instance_onEdit_Functor::FunctionType func)

   struct Instance_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ );
   } onFree;
   // inline static void register_onFree(CPPClass & cl, Instance::Instance_onFree_Functor::FunctionType func)

   struct Instance_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/const char * string);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/const char * string);
   } onGetDataFromString;
   // inline static void register_onGetDataFromString(CPPClass & cl, Instance::Instance_onGetDataFromString_Functor::FunctionType func)

   struct Instance_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef const char * (* FunctionType)(Instance & , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
      inline FunctionType operator= (FunctionType func);
      inline const char * operator()(/*6Bk*/Instance & o_ , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;
   // inline static void register_onGetString(CPPClass & cl, Instance::Instance_onGetString_Functor::FunctionType func)

   struct Instance_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/Window & window, /*6Fj*/void * object);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/Window & window, /*6Fj*/void * object);
   } onSaveEdit;
   // inline static void register_onSaveEdit(CPPClass & cl, Instance::Instance_onSaveEdit_Functor::FunctionType func)

   struct Instance_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onSerialize;
   // inline static void register_onSerialize(CPPClass & cl, Instance::Instance_onSerialize_Functor::FunctionType func)

   struct Instance_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onUnserialize;
   // inline static void register_onUnserialize(CPPClass & cl, Instance::Instance_onUnserialize_Functor::FunctionType func)

   static TCPPClass<DGGSJSONGrid> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         DGGSJSONGrid * inst = new DGGSJSONGrid(i, _cpp_class);
         if(inst)
         {
            /* printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      DGGSJSONGrid * inst = (DGGSJSONGrid *)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor)
            ((void (*)(DGGSJSONGrid & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree)
            delete inst;
      }
   }
   explicit inline DGGSJSONGrid(C(Instance) _impl, CPPClass & cl = _cpp_class) : Instance(_impl, cl) { }

   static void class_registration(CPPClass & _cpp_class);

   inline DGGSJSONGrid(int cellsCount, double resolution);

   struct cellsCount_Prop
   {
      constexpr cellsCount_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline DGGSJSONGrid::cellsCount_Prop & operator= (DGGSJSONGrid::cellsCount_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } cellsCount;
   struct resolution_Prop
   {
      constexpr resolution_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/double operator= (/*0H*/double v);
      /*regSet*/inline DGGSJSONGrid::resolution_Prop & operator= (DGGSJSONGrid::resolution_Prop & prop);
      /*regGet*/inline operator /*0I*/double () const;
   } resolution;
   struct coordinates_Prop
   {
      constexpr coordinates_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> operator= (/*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v);
      /*regSet*/inline DGGSJSONGrid::coordinates_Prop & operator= (DGGSJSONGrid::coordinates_Prop & prop);
      /*regGet*/inline operator /*0I*/TArray<FieldValue _ARG int _ARG FieldValue> () const;
   } coordinates;
   struct boundsCoordinates_Prop
   {
      constexpr boundsCoordinates_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> operator= (/*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v);
      /*regSet*/inline DGGSJSONGrid::boundsCoordinates_Prop & operator= (DGGSJSONGrid::boundsCoordinates_Prop & prop);
      /*regGet*/inline operator /*0I*/TArray<FieldValue _ARG int _ARG FieldValue> () const;
   } boundsCoordinates;
   struct relativeBounds_Prop
   {
      constexpr relativeBounds_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> operator= (/*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v);
      /*regSet*/inline DGGSJSONGrid::relativeBounds_Prop & operator= (DGGSJSONGrid::relativeBounds_Prop & prop);
      /*regGet*/inline operator /*0I*/TArray<FieldValue _ARG int _ARG FieldValue> () const;
   } relativeBounds;
   struct firstCoordinate_Prop
   {
      constexpr firstCoordinate_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/FieldValue operator= (/*0H*/FieldValue v);
      /*regSet*/inline DGGSJSONGrid::firstCoordinate_Prop & operator= (DGGSJSONGrid::firstCoordinate_Prop & prop);
      /*regGet*/inline operator /*0I*/FieldValue () const;
   } firstCoordinate;
};


#define REG_DGGSJSONShape(c) \
      DGGSJSONShape::class_registration(_cpp_class);

#define DGGSJSONSHAPE_VIRTUAL_METHODS_PROTO(c) \

#define DGGSJSONSHAPE_VIRTUAL_METHODS(c) \

class DGGSJSONShape : public Instance
{
public:
   inline DGGSJSONShape(DGGSJSONShape && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline DGGSJSONShape & operator= (DGGSJSONShape && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   DGGSJSONShape() : DGGSJSONShape((C(Instance))Instance_newEx(_cpp_class.impl, false), _cpp_class) { }
   struct Instance_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(Instance & , /*6Bj*/Instance & object);
      inline FunctionType operator= (FunctionType func);
      inline int operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & object);
   } onCompare;
   // inline static void register_onCompare(CPPClass & cl, Instance::Instance_onCompare_Functor::FunctionType func)

   struct Instance_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Bj*/Instance & newData);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & newData);
   } onCopy;
   // inline static void register_onCopy(CPPClass & cl, Instance::Instance_onCopy_Functor::FunctionType func)

   struct Instance_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;
   // inline static void register_onDisplay(CPPClass & cl, Instance::Instance_onDisplay_Functor::FunctionType func)

   struct Instance_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef Instance & (* FunctionType)(Instance & , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
      inline FunctionType operator= (FunctionType func);
      inline Instance & operator()(/*6Bk*/Instance & o_ , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;
   // inline static void register_onEdit(CPPClass & cl, Instance::Instance_onEdit_Functor::FunctionType func)

   struct Instance_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ );
   } onFree;
   // inline static void register_onFree(CPPClass & cl, Instance::Instance_onFree_Functor::FunctionType func)

   struct Instance_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/const char * string);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/const char * string);
   } onGetDataFromString;
   // inline static void register_onGetDataFromString(CPPClass & cl, Instance::Instance_onGetDataFromString_Functor::FunctionType func)

   struct Instance_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef const char * (* FunctionType)(Instance & , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
      inline FunctionType operator= (FunctionType func);
      inline const char * operator()(/*6Bk*/Instance & o_ , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;
   // inline static void register_onGetString(CPPClass & cl, Instance::Instance_onGetString_Functor::FunctionType func)

   struct Instance_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/Window & window, /*6Fj*/void * object);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/Window & window, /*6Fj*/void * object);
   } onSaveEdit;
   // inline static void register_onSaveEdit(CPPClass & cl, Instance::Instance_onSaveEdit_Functor::FunctionType func)

   struct Instance_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onSerialize;
   // inline static void register_onSerialize(CPPClass & cl, Instance::Instance_onSerialize_Functor::FunctionType func)

   struct Instance_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onUnserialize;
   // inline static void register_onUnserialize(CPPClass & cl, Instance::Instance_onUnserialize_Functor::FunctionType func)

   static TCPPClass<DGGSJSONShape> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         DGGSJSONShape * inst = new DGGSJSONShape(i, _cpp_class);
         if(inst)
         {
            /* printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      DGGSJSONShape * inst = (DGGSJSONShape *)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor)
            ((void (*)(DGGSJSONShape & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree)
            delete inst;
      }
   }
   explicit inline DGGSJSONShape(C(Instance) _impl, CPPClass & cl = _cpp_class) : Instance(_impl, cl) { }

   static void class_registration(CPPClass & _cpp_class);

   inline DGGSJSONShape(int count, int subZones);

   struct count_Prop
   {
      constexpr count_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline DGGSJSONShape::count_Prop & operator= (DGGSJSONShape::count_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } count;
   struct subZones_Prop
   {
      constexpr subZones_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline DGGSJSONShape::subZones_Prop & operator= (DGGSJSONShape::subZones_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } subZones;
   struct dimensions_Prop
   {
      constexpr dimensions_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TMap<C(String) _ARG int> operator= (/*0H*/TMap<C(String) _ARG int> v);
      /*regSet*/inline DGGSJSONShape::dimensions_Prop & operator= (DGGSJSONShape::dimensions_Prop & prop);
      /*regGet*/inline operator /*0I*/TMap<C(String) _ARG int> () const;
   } dimensions;
};

class FieldTypeEx
{
public:
   C(FieldTypeEx) impl;
   constexpr FieldTypeEx() : impl(0) { }
   constexpr FieldTypeEx(C(FieldTypeEx) impl) : impl(impl) { }
   FieldTypeEx(FieldType type, bool mustFree, FieldValueFormat format, bool isUnsigned, bool isDateTime);
   operator C(FieldTypeEx)() { return impl; }
   FieldTypeEx & operator =(C(FieldTypeEx) impl) { impl = impl; return *this; }
   bool operator ==(const FieldTypeEx & value) const { return impl == value.impl; }
   bool operator !=(const FieldTypeEx & value) const { return impl != value.impl; }

   struct type_Prop
   {
      constexpr type_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/FieldType operator= (/*0H*/FieldType v);
      /*regSet*/inline FieldTypeEx::type_Prop & operator= (FieldTypeEx::type_Prop & prop);
      /*regGet*/inline operator /*0I*/FieldType () const;
   } type;
   struct mustFree_Prop
   {
      constexpr mustFree_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline FieldTypeEx::mustFree_Prop & operator= (FieldTypeEx::mustFree_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } mustFree;
   struct format_Prop
   {
      constexpr format_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/FieldValueFormat operator= (/*0H*/FieldValueFormat v);
      /*regSet*/inline FieldTypeEx::format_Prop & operator= (FieldTypeEx::format_Prop & prop);
      /*regGet*/inline operator /*0I*/FieldValueFormat () const;
   } format;
   struct isUnsigned_Prop
   {
      constexpr isUnsigned_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline FieldTypeEx::isUnsigned_Prop & operator= (FieldTypeEx::isUnsigned_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } isUnsigned;
   struct isDateTime_Prop
   {
      constexpr isDateTime_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline FieldTypeEx::isDateTime_Prop & operator= (FieldTypeEx::isDateTime_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } isDateTime;
};

struct FieldValue : public TStruct<FieldValue>
{
public:
   C(FieldValue) impl;
   constexpr FieldValue() : impl({}) { }
   constexpr FieldValue(const C(FieldValue) impl) : impl(impl) { }

   inline int compareInt(/*1Ab*/FieldValue & other); // FieldValue_compareInt
   inline int compareInt(/*1Ac*/FieldValue * other); // FieldValue_compareInt
   inline int compareReal(/*1Ab*/FieldValue & other); // FieldValue_compareReal
   inline int compareReal(/*1Ac*/FieldValue * other); // FieldValue_compareReal
   inline int compareText(/*1Ab*/FieldValue & other); // FieldValue_compareText
   inline int compareText(/*1Ac*/FieldValue * other); // FieldValue_compareText
   inline C(String) formatArray(/*1Ab*/char * tempString, /*1Ab*/void * fieldData, /*1Ab*/ObjectNotationType * onType); // FieldValue_formatArray
   inline C(String) formatFloat(/*1Ab*/char * stringOutput, /*1Ab*/bool fixDot); // FieldValue_formatFloat
   inline C(String) formatInteger(/*1Ab*/char * stringOutput); // FieldValue_formatInteger
   inline C(String) formatMap(/*1Ab*/char * tempString, /*1Ab*/void * fieldData, /*1Ab*/ObjectNotationType * onType); // FieldValue_formatMap
   static inline C(bool) getArrayOrMap(/*1Ab*/const char * string, /*1Ab*/Class destClass, /*1Ab*/void ** destination); // FieldValue_getArrayOrMap
   inline C(String) stringify(); // FieldValue_stringify

   inline FieldValue(FieldTypeEx type, int64 i, double r, char * s, void * b);

   struct type_Prop
   {
      constexpr type_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/FieldTypeEx operator= (/*0H*/FieldTypeEx v);
      /*regSet*/inline FieldValue::type_Prop & operator= (FieldValue::type_Prop & prop);
      /*regGet*/inline operator /*0I*/FieldTypeEx () const;
   } type;
   struct i_Prop
   {
      constexpr i_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int64 operator= (/*0H*/int64 v);
      /*regSet*/inline FieldValue::i_Prop & operator= (FieldValue::i_Prop & prop);
      /*regGet*/inline operator /*0I*/int64 () const;
   } i;
   struct r_Prop
   {
      constexpr r_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/double operator= (/*0H*/double v);
      /*regSet*/inline FieldValue::r_Prop & operator= (FieldValue::r_Prop & prop);
      /*regGet*/inline operator /*0I*/double () const;
   } r;
   struct s_Prop
   {
      constexpr s_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline FieldValue::s_Prop & operator= (FieldValue::s_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } s;
   struct b_Prop
   {
      constexpr b_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline FieldValue::b_Prop & operator= (FieldValue::b_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } b;
   struct a_Prop
   {
      constexpr a_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> operator= (/*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v);
      /*regSet*/inline FieldValue::a_Prop & operator= (FieldValue::a_Prop & prop);
      /*regGet*/inline operator /*0I*/TArray<FieldValue _ARG int _ARG FieldValue> () const;
   } a;
   struct m_Prop
   {
      constexpr m_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TMap<C(String) _ARG FieldValue> operator= (/*0H*/TMap<C(String) _ARG FieldValue> v);
      /*regSet*/inline FieldValue::m_Prop & operator= (FieldValue::m_Prop & prop);
      /*regGet*/inline operator /*0I*/TMap<C(String) _ARG FieldValue> () const;
   } m;
};

enum class FieldValueFormat : int
{
   decimal = FieldValueFormat_decimal,
   unset = FieldValueFormat_unset,
   hex = FieldValueFormat_hex,
   octal = FieldValueFormat_octal,
   binary = FieldValueFormat_binary,
   exponential = FieldValueFormat_exponential,
   boolean = FieldValueFormat_boolean,
   textObj = FieldValueFormat_textObj,
   color = FieldValueFormat_color
};

class GGGZone : public DGGRSZone
{
public:
   constexpr GGGZone() : DGGRSZone() { }
   GGGZone(C(GGGZone) impl) : DGGRSZone((C(DGGRSZone))impl) { }
   GGGZone(uint level, uint row, uint col)
   {
      impl = GGGZONE(level, row, col);
   }
   operator C(GGGZone)() { return impl; }
   GGGZone & operator =(C(GGGZone) impl) { impl = impl; return *this; }
   bool operator ==(const GGGZone & value) const { return impl == value.impl; }
   bool operator !=(const GGGZone & value) const { return impl != value.impl; }

   struct level_Prop
   {
      constexpr level_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint operator= (/*0H*/uint v);
      /*regSet*/inline GGGZone::level_Prop & operator= (GGGZone::level_Prop & prop);
      /*regGet*/inline operator /*0I*/uint () const;
   } level;
   struct row_Prop
   {
      constexpr row_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint operator= (/*0H*/uint v);
      /*regSet*/inline GGGZone::row_Prop & operator= (GGGZone::row_Prop & prop);
      /*regGet*/inline operator /*0I*/uint () const;
   } row;
   struct col_Prop
   {
      constexpr col_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint operator= (/*0H*/uint v);
      /*regSet*/inline GGGZone::col_Prop & operator= (GGGZone::col_Prop & prop);
      /*regGet*/inline operator /*0I*/uint () const;
   } col;
};


#define REG_GNOSISGlobalGrid(c) \
      GNOSISGlobalGrid::class_registration(_cpp_class); \
      REG_DGGRS_compactZones(compactZones, c); \
      REG_DGGRS_countSubZones(countSubZones, c); \
      REG_DGGRS_countZoneEdges(countZoneEdges, c); \
      REG_DGGRS_countZones(countZones, c); \
      REG_DGGRS_getFirstSubZone(getFirstSubZone, c); \
      REG_DGGRS_getMaxDGGRSZoneLevel(getMaxDGGRSZoneLevel, c); \
      REG_DGGRS_getRefinementRatio(getRefinementRatio, c); \
      REG_DGGRS_getSubZoneCRSCentroids(getSubZoneCRSCentroids, c); \
      REG_DGGRS_getSubZoneWGS84Centroids(getSubZoneWGS84Centroids, c); \
      REG_DGGRS_getZoneArea(getZoneArea, c); \
      REG_DGGRS_getZoneCRSCentroid(getZoneCRSCentroid, c); \
      REG_DGGRS_getZoneCRSExtent(getZoneCRSExtent, c); \
      REG_DGGRS_getZoneCRSVertices(getZoneCRSVertices, c); \
      REG_DGGRS_getZoneCentroidChild(getZoneCentroidChild, c); \
      REG_DGGRS_getZoneCentroidParent(getZoneCentroidParent, c); \
      REG_DGGRS_getZoneChildren(getZoneChildren, c); \
      REG_DGGRS_getZoneFromCRSCentroid(getZoneFromCRSCentroid, c); \
      REG_DGGRS_getZoneFromTextID(getZoneFromTextID, c); \
      REG_DGGRS_getZoneFromWGS84Centroid(getZoneFromWGS84Centroid, c); \
      REG_DGGRS_getZoneLevel(getZoneLevel, c); \
      REG_DGGRS_getZoneNeighbors(getZoneNeighbors, c); \
      REG_DGGRS_getZoneParents(getZoneParents, c); \
      REG_DGGRS_getZoneRefinedCRSVertices(getZoneRefinedCRSVertices, c); \
      REG_DGGRS_getZoneRefinedWGS84Vertices(getZoneRefinedWGS84Vertices, c); \
      REG_DGGRS_getZoneTextID(getZoneTextID, c); \
      REG_DGGRS_getZoneWGS84Centroid(getZoneWGS84Centroid, c); \
      REG_DGGRS_getZoneWGS84Extent(getZoneWGS84Extent, c); \
      REG_DGGRS_getZoneWGS84Vertices(getZoneWGS84Vertices, c); \
      REG_DGGRS_isZoneCentroidChild(isZoneCentroidChild, c); \
      REG_DGGRS_listZones(listZones, c);

#define GNOSISGLOBALGRID_VIRTUAL_METHODS_PROTO(c) \

#define GNOSISGLOBALGRID_VIRTUAL_METHODS(c) \

class GNOSISGlobalGrid : public DGGRS
{
public:
   inline GNOSISGlobalGrid(GNOSISGlobalGrid && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline GNOSISGlobalGrid & operator= (GNOSISGlobalGrid && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   GNOSISGlobalGrid() : GNOSISGlobalGrid((C(Instance))Instance_newEx(_cpp_class.impl, false), _cpp_class) { }
   struct Instance_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(Instance & , /*6Bj*/Instance & object);
      inline FunctionType operator= (FunctionType func);
      inline int operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & object);
   } onCompare;
   // inline static void register_onCompare(CPPClass & cl, Instance::Instance_onCompare_Functor::FunctionType func)

   struct Instance_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Bj*/Instance & newData);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & newData);
   } onCopy;
   // inline static void register_onCopy(CPPClass & cl, Instance::Instance_onCopy_Functor::FunctionType func)

   struct Instance_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;
   // inline static void register_onDisplay(CPPClass & cl, Instance::Instance_onDisplay_Functor::FunctionType func)

   struct Instance_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef Instance & (* FunctionType)(Instance & , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
      inline FunctionType operator= (FunctionType func);
      inline Instance & operator()(/*6Bk*/Instance & o_ , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;
   // inline static void register_onEdit(CPPClass & cl, Instance::Instance_onEdit_Functor::FunctionType func)

   struct Instance_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ );
   } onFree;
   // inline static void register_onFree(CPPClass & cl, Instance::Instance_onFree_Functor::FunctionType func)

   struct Instance_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/const char * string);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/const char * string);
   } onGetDataFromString;
   // inline static void register_onGetDataFromString(CPPClass & cl, Instance::Instance_onGetDataFromString_Functor::FunctionType func)

   struct Instance_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef const char * (* FunctionType)(Instance & , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
      inline FunctionType operator= (FunctionType func);
      inline const char * operator()(/*6Bk*/Instance & o_ , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;
   // inline static void register_onGetString(CPPClass & cl, Instance::Instance_onGetString_Functor::FunctionType func)

   struct Instance_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/Window & window, /*6Fj*/void * object);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/Window & window, /*6Fj*/void * object);
   } onSaveEdit;
   // inline static void register_onSaveEdit(CPPClass & cl, Instance::Instance_onSaveEdit_Functor::FunctionType func)

   struct Instance_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onSerialize;
   // inline static void register_onSerialize(CPPClass & cl, Instance::Instance_onSerialize_Functor::FunctionType func)

   struct Instance_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onUnserialize;
   // inline static void register_onUnserialize(CPPClass & cl, Instance::Instance_onUnserialize_Functor::FunctionType func)

   static TCPPClass<GNOSISGlobalGrid> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         GNOSISGlobalGrid * inst = new GNOSISGlobalGrid(i, _cpp_class);
         if(inst)
         {
            /* printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      GNOSISGlobalGrid * inst = (GNOSISGlobalGrid *)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor)
            ((void (*)(GNOSISGlobalGrid & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree)
            delete inst;
      }
   }
   explicit inline GNOSISGlobalGrid(C(Instance) _impl, CPPClass & cl = _cpp_class) : DGGRS(_impl, cl) { }

   static void class_registration(CPPClass & _cpp_class);
};

struct GeoExtent : public TStruct<GeoExtent>
{
public:
   C(GeoExtent) impl;
   constexpr GeoExtent() : impl({}) { }
   constexpr GeoExtent(const C(GeoExtent) impl) : impl(impl) { }

   inline void clear(); // GeoExtent_clear
   inline C(bool) intersects(/*1Ab*/const GeoExtent & b); // GeoExtent_intersects
   inline C(bool) intersects(/*1Ac*/const GeoExtent * b); // GeoExtent_intersects

   inline GeoExtent(GeoPoint ll, GeoPoint ur);

   struct geodeticArea_Prop
   {
      constexpr geodeticArea_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/double () const;
   } geodeticArea;

   struct ll_Prop
   {
      constexpr ll_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/GeoPoint operator= (/*0H*/GeoPoint v);
      /*regSet*/inline GeoExtent::ll_Prop & operator= (GeoExtent::ll_Prop & prop);
      /*regGet*/inline operator /*0I*/GeoPoint () const;
   } ll;
   struct ur_Prop
   {
      constexpr ur_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/GeoPoint operator= (/*0H*/GeoPoint v);
      /*regSet*/inline GeoExtent::ur_Prop & operator= (GeoExtent::ur_Prop & prop);
      /*regGet*/inline operator /*0I*/GeoPoint () const;
   } ur;
};

struct GeoPoint : public TStruct<GeoPoint>
{
public:
   C(GeoPoint) impl;
   constexpr GeoPoint() : impl({}) { }
   constexpr GeoPoint(const C(GeoPoint) impl) : impl(impl) { }

   inline GeoPoint(Degrees lat, Degrees lon);

   struct lat_Prop
   {
      constexpr lat_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Degrees operator= (/*0H*/Degrees v);
      /*regSet*/inline GeoPoint::lat_Prop & operator= (GeoPoint::lat_Prop & prop);
      /*regGet*/inline operator /*0I*/Degrees () const;
      /*regGet*/inline operator /*0J*/C(Angle) () const;
   } lat;
   struct lon_Prop
   {
      constexpr lon_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Degrees operator= (/*0H*/Degrees v);
      /*regSet*/inline GeoPoint::lon_Prop & operator= (GeoPoint::lon_Prop & prop);
      /*regGet*/inline operator /*0I*/Degrees () const;
      /*regGet*/inline operator /*0J*/C(Angle) () const;
   } lon;
};


#define REG_ISEA3H(c) \
      ISEA3H::class_registration(_cpp_class); \
      REG_DGGRS_compactZones(compactZones, c); \
      REG_DGGRS_countSubZones(countSubZones, c); \
      REG_DGGRS_countZoneEdges(countZoneEdges, c); \
      REG_DGGRS_countZones(countZones, c); \
      REG_DGGRS_getFirstSubZone(getFirstSubZone, c); \
      REG_DGGRS_getMaxDGGRSZoneLevel(getMaxDGGRSZoneLevel, c); \
      REG_DGGRS_getRefinementRatio(getRefinementRatio, c); \
      REG_DGGRS_getSubZoneCRSCentroids(getSubZoneCRSCentroids, c); \
      REG_DGGRS_getSubZoneWGS84Centroids(getSubZoneWGS84Centroids, c); \
      REG_DGGRS_getZoneArea(getZoneArea, c); \
      REG_DGGRS_getZoneCRSCentroid(getZoneCRSCentroid, c); \
      REG_DGGRS_getZoneCRSExtent(getZoneCRSExtent, c); \
      REG_DGGRS_getZoneCRSVertices(getZoneCRSVertices, c); \
      REG_DGGRS_getZoneCentroidChild(getZoneCentroidChild, c); \
      REG_DGGRS_getZoneCentroidParent(getZoneCentroidParent, c); \
      REG_DGGRS_getZoneChildren(getZoneChildren, c); \
      REG_DGGRS_getZoneFromCRSCentroid(getZoneFromCRSCentroid, c); \
      REG_DGGRS_getZoneFromTextID(getZoneFromTextID, c); \
      REG_DGGRS_getZoneFromWGS84Centroid(getZoneFromWGS84Centroid, c); \
      REG_DGGRS_getZoneLevel(getZoneLevel, c); \
      REG_DGGRS_getZoneNeighbors(getZoneNeighbors, c); \
      REG_DGGRS_getZoneParents(getZoneParents, c); \
      REG_DGGRS_getZoneRefinedCRSVertices(getZoneRefinedCRSVertices, c); \
      REG_DGGRS_getZoneRefinedWGS84Vertices(getZoneRefinedWGS84Vertices, c); \
      REG_DGGRS_getZoneTextID(getZoneTextID, c); \
      REG_DGGRS_getZoneWGS84Centroid(getZoneWGS84Centroid, c); \
      REG_DGGRS_getZoneWGS84Extent(getZoneWGS84Extent, c); \
      REG_DGGRS_getZoneWGS84Vertices(getZoneWGS84Vertices, c); \
      REG_DGGRS_isZoneCentroidChild(isZoneCentroidChild, c); \
      REG_DGGRS_listZones(listZones, c);

#define ISEA3H_VIRTUAL_METHODS_PROTO(c) \

#define ISEA3H_VIRTUAL_METHODS(c) \

class ISEA3H : public DGGRS
{
public:
   inline ISEA3H(ISEA3H && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline ISEA3H & operator= (ISEA3H && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   ISEA3H() : ISEA3H((C(Instance))Instance_newEx(_cpp_class.impl, false), _cpp_class) { }
   struct Instance_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(Instance & , /*6Bj*/Instance & object);
      inline FunctionType operator= (FunctionType func);
      inline int operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & object);
   } onCompare;
   // inline static void register_onCompare(CPPClass & cl, Instance::Instance_onCompare_Functor::FunctionType func)

   struct Instance_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Bj*/Instance & newData);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & newData);
   } onCopy;
   // inline static void register_onCopy(CPPClass & cl, Instance::Instance_onCopy_Functor::FunctionType func)

   struct Instance_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;
   // inline static void register_onDisplay(CPPClass & cl, Instance::Instance_onDisplay_Functor::FunctionType func)

   struct Instance_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef Instance & (* FunctionType)(Instance & , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
      inline FunctionType operator= (FunctionType func);
      inline Instance & operator()(/*6Bk*/Instance & o_ , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;
   // inline static void register_onEdit(CPPClass & cl, Instance::Instance_onEdit_Functor::FunctionType func)

   struct Instance_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ );
   } onFree;
   // inline static void register_onFree(CPPClass & cl, Instance::Instance_onFree_Functor::FunctionType func)

   struct Instance_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/const char * string);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/const char * string);
   } onGetDataFromString;
   // inline static void register_onGetDataFromString(CPPClass & cl, Instance::Instance_onGetDataFromString_Functor::FunctionType func)

   struct Instance_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef const char * (* FunctionType)(Instance & , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
      inline FunctionType operator= (FunctionType func);
      inline const char * operator()(/*6Bk*/Instance & o_ , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;
   // inline static void register_onGetString(CPPClass & cl, Instance::Instance_onGetString_Functor::FunctionType func)

   struct Instance_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/Window & window, /*6Fj*/void * object);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/Window & window, /*6Fj*/void * object);
   } onSaveEdit;
   // inline static void register_onSaveEdit(CPPClass & cl, Instance::Instance_onSaveEdit_Functor::FunctionType func)

   struct Instance_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onSerialize;
   // inline static void register_onSerialize(CPPClass & cl, Instance::Instance_onSerialize_Functor::FunctionType func)

   struct Instance_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onUnserialize;
   // inline static void register_onUnserialize(CPPClass & cl, Instance::Instance_onUnserialize_Functor::FunctionType func)

   static TCPPClass<ISEA3H> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         ISEA3H * inst = new ISEA3H(i, _cpp_class);
         if(inst)
         {
            /* printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      ISEA3H * inst = (ISEA3H *)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor)
            ((void (*)(ISEA3H & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree)
            delete inst;
      }
   }
   explicit inline ISEA3H(C(Instance) _impl, CPPClass & cl = _cpp_class) : DGGRS(_impl, cl) { }

   static void class_registration(CPPClass & _cpp_class);
};

class ISEA3HZone : public DGGRSZone
{
public:
   constexpr ISEA3HZone() : DGGRSZone() { }
   ISEA3HZone(C(ISEA3HZone) impl) : DGGRSZone((C(DGGRSZone))impl) { }
   ISEA3HZone(uint levelISEA9R, uint rootRhombus, /* FIXME: Why is this missing? */ uint64 rhombusIX, uint subHex)
   {
      impl = ISEA3HZONE(levelISEA9R, rootRhombus, rhombusIX, subHex);
   }
   operator C(ISEA3HZone)() { return impl; }
   ISEA3HZone & operator =(C(ISEA3HZone) impl) { impl = impl; return *this; }
   bool operator ==(const ISEA3HZone & value) const { return impl == value.impl; }
   bool operator !=(const ISEA3HZone & value) const { return impl != value.impl; }

   struct levelISEA9R_Prop
   {
      constexpr levelISEA9R_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint operator= (/*0H*/uint v);
      /*regSet*/inline ISEA3HZone::levelISEA9R_Prop & operator= (ISEA3HZone::levelISEA9R_Prop & prop);
      /*regGet*/inline operator /*0I*/uint () const;
   } levelISEA9R;
   struct rootRhombus_Prop
   {
      constexpr rootRhombus_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint operator= (/*0H*/uint v);
      /*regSet*/inline ISEA3HZone::rootRhombus_Prop & operator= (ISEA3HZone::rootRhombus_Prop & prop);
      /*regGet*/inline operator /*0I*/uint () const;
   } rootRhombus;
   struct rhombusIX_Prop
   {
      constexpr rhombusIX_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint64 operator= (/*0H*/uint64 v);
      /*regSet*/inline ISEA3HZone::rhombusIX_Prop & operator= (ISEA3HZone::rhombusIX_Prop & prop);
      /*regGet*/inline operator /*0I*/uint64 () const;
   } rhombusIX;
   struct subHex_Prop
   {
      constexpr subHex_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint operator= (/*0H*/uint v);
      /*regSet*/inline ISEA3HZone::subHex_Prop & operator= (ISEA3HZone::subHex_Prop & prop);
      /*regGet*/inline operator /*0I*/uint () const;
   } subHex;
};


#define REG_ISEA9R(c) \
      ISEA9R::class_registration(_cpp_class); \
      REG_DGGRS_compactZones(compactZones, c); \
      REG_DGGRS_countSubZones(countSubZones, c); \
      REG_DGGRS_countZoneEdges(countZoneEdges, c); \
      REG_DGGRS_countZones(countZones, c); \
      REG_DGGRS_getFirstSubZone(getFirstSubZone, c); \
      REG_DGGRS_getMaxDGGRSZoneLevel(getMaxDGGRSZoneLevel, c); \
      REG_DGGRS_getRefinementRatio(getRefinementRatio, c); \
      REG_DGGRS_getSubZoneCRSCentroids(getSubZoneCRSCentroids, c); \
      REG_DGGRS_getSubZoneWGS84Centroids(getSubZoneWGS84Centroids, c); \
      REG_DGGRS_getZoneArea(getZoneArea, c); \
      REG_DGGRS_getZoneCRSCentroid(getZoneCRSCentroid, c); \
      REG_DGGRS_getZoneCRSExtent(getZoneCRSExtent, c); \
      REG_DGGRS_getZoneCRSVertices(getZoneCRSVertices, c); \
      REG_DGGRS_getZoneCentroidChild(getZoneCentroidChild, c); \
      REG_DGGRS_getZoneCentroidParent(getZoneCentroidParent, c); \
      REG_DGGRS_getZoneChildren(getZoneChildren, c); \
      REG_DGGRS_getZoneFromCRSCentroid(getZoneFromCRSCentroid, c); \
      REG_DGGRS_getZoneFromTextID(getZoneFromTextID, c); \
      REG_DGGRS_getZoneFromWGS84Centroid(getZoneFromWGS84Centroid, c); \
      REG_DGGRS_getZoneLevel(getZoneLevel, c); \
      REG_DGGRS_getZoneNeighbors(getZoneNeighbors, c); \
      REG_DGGRS_getZoneParents(getZoneParents, c); \
      REG_DGGRS_getZoneRefinedCRSVertices(getZoneRefinedCRSVertices, c); \
      REG_DGGRS_getZoneRefinedWGS84Vertices(getZoneRefinedWGS84Vertices, c); \
      REG_DGGRS_getZoneTextID(getZoneTextID, c); \
      REG_DGGRS_getZoneWGS84Centroid(getZoneWGS84Centroid, c); \
      REG_DGGRS_getZoneWGS84Extent(getZoneWGS84Extent, c); \
      REG_DGGRS_getZoneWGS84Vertices(getZoneWGS84Vertices, c); \
      REG_DGGRS_isZoneCentroidChild(isZoneCentroidChild, c); \
      REG_DGGRS_listZones(listZones, c);

#define ISEA9R_VIRTUAL_METHODS_PROTO(c) \

#define ISEA9R_VIRTUAL_METHODS(c) \

class ISEA9R : public DGGRS
{
public:
   inline ISEA9R(ISEA9R && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline ISEA9R & operator= (ISEA9R && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   ISEA9R() : ISEA9R((C(Instance))Instance_newEx(_cpp_class.impl, false), _cpp_class) { }
   struct Instance_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(Instance & , /*6Bj*/Instance & object);
      inline FunctionType operator= (FunctionType func);
      inline int operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & object);
   } onCompare;
   // inline static void register_onCompare(CPPClass & cl, Instance::Instance_onCompare_Functor::FunctionType func)

   struct Instance_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Bj*/Instance & newData);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & newData);
   } onCopy;
   // inline static void register_onCopy(CPPClass & cl, Instance::Instance_onCopy_Functor::FunctionType func)

   struct Instance_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;
   // inline static void register_onDisplay(CPPClass & cl, Instance::Instance_onDisplay_Functor::FunctionType func)

   struct Instance_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef Instance & (* FunctionType)(Instance & , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
      inline FunctionType operator= (FunctionType func);
      inline Instance & operator()(/*6Bk*/Instance & o_ , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;
   // inline static void register_onEdit(CPPClass & cl, Instance::Instance_onEdit_Functor::FunctionType func)

   struct Instance_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ );
   } onFree;
   // inline static void register_onFree(CPPClass & cl, Instance::Instance_onFree_Functor::FunctionType func)

   struct Instance_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/const char * string);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/const char * string);
   } onGetDataFromString;
   // inline static void register_onGetDataFromString(CPPClass & cl, Instance::Instance_onGetDataFromString_Functor::FunctionType func)

   struct Instance_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef const char * (* FunctionType)(Instance & , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
      inline FunctionType operator= (FunctionType func);
      inline const char * operator()(/*6Bk*/Instance & o_ , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;
   // inline static void register_onGetString(CPPClass & cl, Instance::Instance_onGetString_Functor::FunctionType func)

   struct Instance_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/Window & window, /*6Fj*/void * object);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/Window & window, /*6Fj*/void * object);
   } onSaveEdit;
   // inline static void register_onSaveEdit(CPPClass & cl, Instance::Instance_onSaveEdit_Functor::FunctionType func)

   struct Instance_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onSerialize;
   // inline static void register_onSerialize(CPPClass & cl, Instance::Instance_onSerialize_Functor::FunctionType func)

   struct Instance_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onUnserialize;
   // inline static void register_onUnserialize(CPPClass & cl, Instance::Instance_onUnserialize_Functor::FunctionType func)

   static TCPPClass<ISEA9R> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         ISEA9R * inst = new ISEA9R(i, _cpp_class);
         if(inst)
         {
            /* printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      ISEA9R * inst = (ISEA9R *)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor)
            ((void (*)(ISEA9R & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree)
            delete inst;
      }
   }
   explicit inline ISEA9R(C(Instance) _impl, CPPClass & cl = _cpp_class) : DGGRS(_impl, cl) { }

   static void class_registration(CPPClass & _cpp_class);
};

class ISEA9RZone : public DGGRSZone
{
public:
   constexpr ISEA9RZone() : DGGRSZone() { }
   ISEA9RZone(C(ISEA9RZone) impl) : DGGRSZone((C(DGGRSZone))impl) { }
   ISEA9RZone(uint level, uint row, uint col)
   {
      impl = ISEA9RZONE(level, row, col);
   }
   operator C(ISEA9RZone)() { return impl; }
   ISEA9RZone & operator =(C(ISEA9RZone) impl) { impl = impl; return *this; }
   bool operator ==(const ISEA9RZone & value) const { return impl == value.impl; }
   bool operator !=(const ISEA9RZone & value) const { return impl != value.impl; }

   struct level_Prop
   {
      constexpr level_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint operator= (/*0H*/uint v);
      /*regSet*/inline ISEA9RZone::level_Prop & operator= (ISEA9RZone::level_Prop & prop);
      /*regGet*/inline operator /*0I*/uint () const;
   } level;
   struct row_Prop
   {
      constexpr row_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint operator= (/*0H*/uint v);
      /*regSet*/inline ISEA9RZone::row_Prop & operator= (ISEA9RZone::row_Prop & prop);
      /*regGet*/inline operator /*0I*/uint () const;
   } row;
   struct col_Prop
   {
      constexpr col_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint operator= (/*0H*/uint v);
      /*regSet*/inline ISEA9RZone::col_Prop & operator= (ISEA9RZone::col_Prop & prop);
      /*regGet*/inline operator /*0I*/uint () const;
   } col;
};


#define REG_JSONSchema(c) \
      JSONSchema::class_registration(_cpp_class);

#define JSONSCHEMA_VIRTUAL_METHODS_PROTO(c) \

#define JSONSCHEMA_VIRTUAL_METHODS(c) \

class JSONSchema : public Instance
{
public:
   inline JSONSchema(JSONSchema && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline JSONSchema & operator= (JSONSchema && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   JSONSchema() : JSONSchema((C(Instance))Instance_newEx(_cpp_class.impl, false), _cpp_class) { }
   struct Instance_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(Instance & , /*6Bj*/Instance & object);
      inline FunctionType operator= (FunctionType func);
      inline int operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & object);
   } onCompare;
   // inline static void register_onCompare(CPPClass & cl, Instance::Instance_onCompare_Functor::FunctionType func)

   struct Instance_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Bj*/Instance & newData);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & newData);
   } onCopy;
   // inline static void register_onCopy(CPPClass & cl, Instance::Instance_onCopy_Functor::FunctionType func)

   struct Instance_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/Surface & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;
   // inline static void register_onDisplay(CPPClass & cl, Instance::Instance_onDisplay_Functor::FunctionType func)

   struct Instance_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef Instance & (* FunctionType)(Instance & , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
      inline FunctionType operator= (FunctionType func);
      inline Instance & operator()(/*6Bk*/Instance & o_ , /*6Fj*/DataBox & dataBox, /*6Fj*/DataBox & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;
   // inline static void register_onEdit(CPPClass & cl, Instance::Instance_onEdit_Functor::FunctionType func)

   struct Instance_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ );
   } onFree;
   // inline static void register_onFree(CPPClass & cl, Instance::Instance_onFree_Functor::FunctionType func)

   struct Instance_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/const char * string);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/const char * string);
   } onGetDataFromString;
   // inline static void register_onGetDataFromString(CPPClass & cl, Instance::Instance_onGetDataFromString_Functor::FunctionType func)

   struct Instance_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef const char * (* FunctionType)(Instance & , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
      inline FunctionType operator= (FunctionType func);
      inline const char * operator()(/*6Bk*/Instance & o_ , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;
   // inline static void register_onGetString(CPPClass & cl, Instance::Instance_onGetString_Functor::FunctionType func)

   struct Instance_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/Window & window, /*6Fj*/void * object);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/Window & window, /*6Fj*/void * object);
   } onSaveEdit;
   // inline static void register_onSaveEdit(CPPClass & cl, Instance::Instance_onSaveEdit_Functor::FunctionType func)

   struct Instance_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onSerialize;
   // inline static void register_onSerialize(CPPClass & cl, Instance::Instance_onSerialize_Functor::FunctionType func)

   struct Instance_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Fj*/IOChannel & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Fj*/IOChannel & channel);
   } onUnserialize;
   // inline static void register_onUnserialize(CPPClass & cl, Instance::Instance_onUnserialize_Functor::FunctionType func)

   static TCPPClass<JSONSchema> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         JSONSchema * inst = new JSONSchema(i, _cpp_class);
         if(inst)
         {
            /* printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      JSONSchema * inst = (JSONSchema *)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor)
            ((void (*)(JSONSchema & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree)
            delete inst;
      }
   }
   explicit inline JSONSchema(C(Instance) _impl, CPPClass & cl = _cpp_class) : Instance(_impl, cl) { }

   static void class_registration(CPPClass & _cpp_class);

   inline JSONSchema(char * schema, char * id, char * title, char * comment, char * description, FieldValue Default, bool readOnly, bool writeOnly);

   struct maximum_Prop
   {
      constexpr maximum_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/double () const;
   } maximum;
   struct exclusiveMaximum_Prop
   {
      constexpr exclusiveMaximum_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/double () const;
   } exclusiveMaximum;
   struct minimum_Prop
   {
      constexpr minimum_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/double () const;
   } minimum;
   struct exclusiveMinimum_Prop
   {
      constexpr exclusiveMinimum_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/double () const;
   } exclusiveMinimum;
   struct maxItems_Prop
   {
      constexpr maxItems_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/int () const;
   } maxItems;
   struct minItems_Prop
   {
      constexpr minItems_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/int () const;
   } minItems;
   struct maxProperties_Prop
   {
      constexpr maxProperties_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/int () const;
   } maxProperties;
   struct minProperties_Prop
   {
      constexpr minProperties_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/int () const;
   } minProperties;
   struct xogcpropertySeq_Prop
   {
      constexpr xogcpropertySeq_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/int () const;
   } xogcpropertySeq;
   struct Default_Prop
   {
      constexpr Default_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/FieldValue () const;
   } Default;

   struct schema_Prop
   {
      constexpr schema_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline JSONSchema::schema_Prop & operator= (JSONSchema::schema_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } schema;
   struct id_Prop
   {
      constexpr id_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline JSONSchema::id_Prop & operator= (JSONSchema::id_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } id;
   struct title_Prop
   {
      constexpr title_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline JSONSchema::title_Prop & operator= (JSONSchema::title_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } title;
   struct comment_Prop
   {
      constexpr comment_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline JSONSchema::comment_Prop & operator= (JSONSchema::comment_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } comment;
   struct description_Prop
   {
      constexpr description_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline JSONSchema::description_Prop & operator= (JSONSchema::description_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } description;
   struct readOnly_Prop
   {
      constexpr readOnly_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline JSONSchema::readOnly_Prop & operator= (JSONSchema::readOnly_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } readOnly;
   struct writeOnly_Prop
   {
      constexpr writeOnly_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline JSONSchema::writeOnly_Prop & operator= (JSONSchema::writeOnly_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } writeOnly;
   struct examples_Prop
   {
      constexpr examples_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TArray<C(String) _ARG int _ARG C(String)> operator= (/*0H*/TArray<C(String) _ARG int _ARG C(String)> v);
      /*regSet*/inline JSONSchema::examples_Prop & operator= (JSONSchema::examples_Prop & prop);
      /*regGet*/inline operator /*0I*/TArray<C(String) _ARG int _ARG C(String)> () const;
   } examples;
   struct multipleOf_Prop
   {
      constexpr multipleOf_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TArray<double _ARG int _ARG double> operator= (/*0H*/TArray<double _ARG int _ARG double> v);
      /*regSet*/inline JSONSchema::multipleOf_Prop & operator= (JSONSchema::multipleOf_Prop & prop);
      /*regGet*/inline operator /*0I*/TArray<double _ARG int _ARG double> () const;
   } multipleOf;
   struct type_Prop
   {
      constexpr type_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/JSONSchemaType operator= (/*0H*/JSONSchemaType v);
      /*regSet*/inline JSONSchema::type_Prop & operator= (JSONSchema::type_Prop & prop);
      /*regGet*/inline operator /*0I*/JSONSchemaType () const;
   } type;
   struct Enum_Prop
   {
      constexpr Enum_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> operator= (/*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v);
      /*regSet*/inline JSONSchema::Enum_Prop & operator= (JSONSchema::Enum_Prop & prop);
      /*regGet*/inline operator /*0I*/TArray<FieldValue _ARG int _ARG FieldValue> () const;
   } Enum;
   struct format_Prop
   {
      constexpr format_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline JSONSchema::format_Prop & operator= (JSONSchema::format_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } format;
   struct contentMediaType_Prop
   {
      constexpr contentMediaType_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline JSONSchema::contentMediaType_Prop & operator= (JSONSchema::contentMediaType_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } contentMediaType;
   struct pattern_Prop
   {
      constexpr pattern_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline JSONSchema::pattern_Prop & operator= (JSONSchema::pattern_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } pattern;
   struct items_Prop
   {
      constexpr items_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/JSONSchema operator= (/*0H*/JSONSchema v);
      /*regSet*/inline JSONSchema::items_Prop & operator= (JSONSchema::items_Prop & prop);
      /*regGet*/inline operator /*0I*/JSONSchema () const;
   } items;
   struct uniqueItems_Prop
   {
      constexpr uniqueItems_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline JSONSchema::uniqueItems_Prop & operator= (JSONSchema::uniqueItems_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } uniqueItems;
   struct contains_Prop
   {
      constexpr contains_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline JSONSchema::contains_Prop & operator= (JSONSchema::contains_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } contains;
   struct required_Prop
   {
      constexpr required_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TArray<C(String) _ARG int _ARG C(String)> operator= (/*0H*/TArray<C(String) _ARG int _ARG C(String)> v);
      /*regSet*/inline JSONSchema::required_Prop & operator= (JSONSchema::required_Prop & prop);
      /*regGet*/inline operator /*0I*/TArray<C(String) _ARG int _ARG C(String)> () const;
   } required;
   struct additionalProperties_Prop
   {
      constexpr additionalProperties_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/JSONSchema operator= (/*0H*/JSONSchema v);
      /*regSet*/inline JSONSchema::additionalProperties_Prop & operator= (JSONSchema::additionalProperties_Prop & prop);
      /*regGet*/inline operator /*0I*/JSONSchema () const;
   } additionalProperties;
   struct definitions_Prop
   {
      constexpr definitions_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TMap<C(String) _ARG C(JSONSchema)> operator= (/*0H*/TMap<C(String) _ARG C(JSONSchema)> v);
      /*regSet*/inline JSONSchema::definitions_Prop & operator= (JSONSchema::definitions_Prop & prop);
      /*regGet*/inline operator /*0I*/TMap<C(String) _ARG C(JSONSchema)> () const;
   } definitions;
   struct properties_Prop
   {
      constexpr properties_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TMap<C(String) _ARG C(JSONSchema)> operator= (/*0H*/TMap<C(String) _ARG C(JSONSchema)> v);
      /*regSet*/inline JSONSchema::properties_Prop & operator= (JSONSchema::properties_Prop & prop);
      /*regGet*/inline operator /*0I*/TMap<C(String) _ARG C(JSONSchema)> () const;
   } properties;
   struct patternProperties_Prop
   {
      constexpr patternProperties_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TMap<C(String) _ARG C(JSONSchema)> operator= (/*0H*/TMap<C(String) _ARG C(JSONSchema)> v);
      /*regSet*/inline JSONSchema::patternProperties_Prop & operator= (JSONSchema::patternProperties_Prop & prop);
      /*regGet*/inline operator /*0I*/TMap<C(String) _ARG C(JSONSchema)> () const;
   } patternProperties;
   struct dependencies_Prop
   {
      constexpr dependencies_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TMap<C(String) _ARG C(JSONSchema)> operator= (/*0H*/TMap<C(String) _ARG C(JSONSchema)> v);
      /*regSet*/inline JSONSchema::dependencies_Prop & operator= (JSONSchema::dependencies_Prop & prop);
      /*regGet*/inline operator /*0I*/TMap<C(String) _ARG C(JSONSchema)> () const;
   } dependencies;
   struct propertyNames_Prop
   {
      constexpr propertyNames_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline JSONSchema::propertyNames_Prop & operator= (JSONSchema::propertyNames_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } propertyNames;
   struct contentEncoding_Prop
   {
      constexpr contentEncoding_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline JSONSchema::contentEncoding_Prop & operator= (JSONSchema::contentEncoding_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } contentEncoding;
   struct If_Prop
   {
      constexpr If_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/JSONSchema operator= (/*0H*/JSONSchema v);
      /*regSet*/inline JSONSchema::If_Prop & operator= (JSONSchema::If_Prop & prop);
      /*regGet*/inline operator /*0I*/JSONSchema () const;
   } If;
   struct Then_Prop
   {
      constexpr Then_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/JSONSchema operator= (/*0H*/JSONSchema v);
      /*regSet*/inline JSONSchema::Then_Prop & operator= (JSONSchema::Then_Prop & prop);
      /*regGet*/inline operator /*0I*/JSONSchema () const;
   } Then;
   struct Else_Prop
   {
      constexpr Else_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/JSONSchema operator= (/*0H*/JSONSchema v);
      /*regSet*/inline JSONSchema::Else_Prop & operator= (JSONSchema::Else_Prop & prop);
      /*regGet*/inline operator /*0I*/JSONSchema () const;
   } Else;
   struct allOf_Prop
   {
      constexpr allOf_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> operator= (/*0H*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> v);
      /*regSet*/inline JSONSchema::allOf_Prop & operator= (JSONSchema::allOf_Prop & prop);
      /*regGet*/inline operator /*0I*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> () const;
   } allOf;
   struct anyOf_Prop
   {
      constexpr anyOf_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> operator= (/*0H*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> v);
      /*regSet*/inline JSONSchema::anyOf_Prop & operator= (JSONSchema::anyOf_Prop & prop);
      /*regGet*/inline operator /*0I*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> () const;
   } anyOf;
   struct oneOf_Prop
   {
      constexpr oneOf_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> operator= (/*0H*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> v);
      /*regSet*/inline JSONSchema::oneOf_Prop & operator= (JSONSchema::oneOf_Prop & prop);
      /*regGet*/inline operator /*0I*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> () const;
   } oneOf;
   struct not_Prop
   {
      constexpr not_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/JSONSchema operator= (/*0H*/JSONSchema v);
      /*regSet*/inline JSONSchema::not_Prop & operator= (JSONSchema::not_Prop & prop);
      /*regGet*/inline operator /*0I*/JSONSchema () const;
   } _not;  // FIXME: C++ not?
   struct xogcrole_Prop
   {
      constexpr xogcrole_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline JSONSchema::xogcrole_Prop & operator= (JSONSchema::xogcrole_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } xogcrole;
};

enum class JSONSchemaType : int
{
   unset = JSONSchemaType_unset,
   array = JSONSchemaType_array,
   boolean = JSONSchemaType_boolean,
   integer = JSONSchemaType_integer,
   _null = JSONSchemaType_null,
   number = JSONSchemaType_number,
   object = JSONSchemaType_object,
   string = JSONSchemaType_string
};
//////////////////////////////////////////////////////////////////////////////// ////////////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////////////
////                                                                        //// ////////////////////////
////    split implementations                                               //// ////////////////////////
////                                                                        //// ////////////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////////////


////////////////////////////////////////////////////////////// [dggal]/ //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////


#undef wgs84Major
static constexpr Meters wgs84Major { 6378137.0 };



// member accessors: CRS_::registry
// (bits::enum|int)

/*nstSet*/inline /*0H*/CRSRegistry CRS_::registry_Prop::operator= (/*0H*/CRSRegistry v)
{
   SELF(CRS_, registry);
   CRS_SET_registry(self->impl, v);
   return v;
}

/*regSet*/inline CRS_::registry_Prop & CRS_::registry_Prop::operator= (CRS_::registry_Prop & prop)
{
   SELF(CRS_, registry);
   /*0H*/CRSRegistry v = prop;
   CRS_SET_registry(self->impl, v);
   return prop;
}
/*regGet*/inline CRS_::registry_Prop::operator /*0I*/CRSRegistry () const
{
   SELF(CRS_, registry);
   return (CRSRegistry)CRS_registry(self->impl);
}

// member accessors: CRS_::crsID
// (bits::int)

/*nstSet*/inline /*0H*/int CRS_::crsID_Prop::operator= (/*0H*/int v)
{
   SELF(CRS_, crsID);
   CRS_SET_crsID(self->impl, v);
   return v;
}

/*regSet*/inline CRS_::crsID_Prop & CRS_::crsID_Prop::operator= (CRS_::crsID_Prop & prop)
{
   SELF(CRS_, crsID);
   /*0H*/int v = prop;
   CRS_SET_crsID(self->impl, v);
   return prop;
}
/*regGet*/inline CRS_::crsID_Prop::operator /*0I*/int () const
{
   SELF(CRS_, crsID);
   return CRS_crsID(self->impl);
}

// member accessors: CRS_::h
// (bits::enum|uint)

/*nstSet*/inline /*0H*/bool CRS_::h_Prop::operator= (/*0H*/bool v)
{
   SELF(CRS_, h);
   CRS_SET_h(self->impl, v);
   return v;
}

/*regSet*/inline CRS_::h_Prop & CRS_::h_Prop::operator= (CRS_::h_Prop & prop)
{
   SELF(CRS_, h);
   /*0H*/bool v = prop;
   CRS_SET_h(self->impl, v);
   return prop;
}
/*regGet*/inline CRS_::h_Prop::operator /*0I*/bool () const
{
   SELF(CRS_, h);
   return (bool)CRS_h(self->impl);
}

inline CRSExtent::CRSExtent(CRS_ crs, Pointd tl = Pointd(), Pointd br = Pointd())
{
   impl.crs = crs;
   impl.tl = tl.impl;
   impl.br = br.impl;
}



// member accessors: CRSExtent::crs
// (struct::bits|uint64)

/*nstSet*/inline /*0H*/CRS_ CRSExtent::crs_Prop::operator= (/*0H*/CRS_ v)
{
   SELF(CRSExtent, crs);
   self->impl.crs = v;
   return v;
}

/*regSet*/inline CRSExtent::crs_Prop & CRSExtent::crs_Prop::operator= (CRSExtent::crs_Prop & prop)
{
   SELF(CRSExtent, crs);
   /*0H*/CRS_ v = prop;
   self->impl.crs = v;
   return prop;
}
/*regGet*/inline CRSExtent::crs_Prop::operator /*0I*/CRS_ () const
{
   SELF(CRSExtent, crs);
   return self ? CRS_(((C(CRSExtent) *)&self->impl)->crs) : CRS_();
}

// member accessors: CRSExtent::tl
// (struct::struct)

/*nstSet*/inline /*0H*/Pointd CRSExtent::tl_Prop::operator= (/*0H*/Pointd v)
{
   SELF(CRSExtent, tl);
   self->impl.tl = v.impl;
   return v;
}

/*regSet*/inline CRSExtent::tl_Prop & CRSExtent::tl_Prop::operator= (CRSExtent::tl_Prop & prop)
{
   SELF(CRSExtent, tl);
   /*0H*/Pointd v = prop;
   self->impl.tl = v.impl;
   return prop;
}
/*regGet*/inline CRSExtent::tl_Prop::operator /*0I*/Pointd () const
{
   SELF(CRSExtent, tl);
   Pointd value(((C(CRSExtent) *)&self->impl)->tl);
   return value;
}

// member accessors: CRSExtent::br
// (struct::struct)

/*nstSet*/inline /*0H*/Pointd CRSExtent::br_Prop::operator= (/*0H*/Pointd v)
{
   SELF(CRSExtent, br);
   self->impl.br = v.impl;
   return v;
}

/*regSet*/inline CRSExtent::br_Prop & CRSExtent::br_Prop::operator= (CRSExtent::br_Prop & prop)
{
   SELF(CRSExtent, br);
   /*0H*/Pointd v = prop;
   self->impl.br = v.impl;
   return prop;
}
/*regGet*/inline CRSExtent::br_Prop::operator /*0I*/Pointd () const
{
   SELF(CRSExtent, br);
   Pointd value(((C(CRSExtent) *)&self->impl)->br);
   return value;
}

inline DGGRS::DGGRS_compactZones_Functor::FunctionType DGGRS::DGGRS_compactZones_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, compactZones);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_compactZones_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_compactZones_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_compactZones_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, compactZones)] = func;
   return func;
}
inline void DGGRS::DGGRS_compactZones_Functor::operator()( /*6Fj*/TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)> & zones)
{
   SELF(DGGRS, compactZones);
   DGGRS_compactZones(self ? self->impl : (C(DGGRS))null, /*7Al*/((Instance&)zones).impl);
}
// inline void DGGRS::register_compactZones(CPPClass & cl, DGGRS::DGGRS_compactZones_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_compactZones_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, compactZones)] = func;
// }

inline DGGRS::DGGRS_countSubZones_Functor::FunctionType DGGRS::DGGRS_countSubZones_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, countSubZones);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_countSubZones_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_countSubZones_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_countSubZones_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, countSubZones)] = func;
   return func;
}
inline uint64 DGGRS::DGGRS_countSubZones_Functor::operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/int depth)
{
   SELF(DGGRS, countSubZones);
   return DGGRS_countSubZones(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/depth);
}
// inline void DGGRS::register_countSubZones(CPPClass & cl, DGGRS::DGGRS_countSubZones_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_countSubZones_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, countSubZones)] = func;
// }

inline DGGRS::DGGRS_countZoneEdges_Functor::FunctionType DGGRS::DGGRS_countZoneEdges_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, countZoneEdges);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_countZoneEdges_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_countZoneEdges_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_countZoneEdges_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, countZoneEdges)] = func;
   return func;
}
inline int DGGRS::DGGRS_countZoneEdges_Functor::operator()( /*6Fj*/DGGRSZone zone)
{
   SELF(DGGRS, countZoneEdges);
   return DGGRS_countZoneEdges(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone);
}
// inline void DGGRS::register_countZoneEdges(CPPClass & cl, DGGRS::DGGRS_countZoneEdges_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_countZoneEdges_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, countZoneEdges)] = func;
// }

inline DGGRS::DGGRS_countZones_Functor::FunctionType DGGRS::DGGRS_countZones_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, countZones);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_countZones_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_countZones_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_countZones_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, countZones)] = func;
   return func;
}
inline uint64 DGGRS::DGGRS_countZones_Functor::operator()( /*6Fj*/int level)
{
   SELF(DGGRS, countZones);
   return DGGRS_countZones(self ? self->impl : (C(DGGRS))null, /*7Al*/level);
}
// inline void DGGRS::register_countZones(CPPClass & cl, DGGRS::DGGRS_countZones_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_countZones_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, countZones)] = func;
// }

inline DGGRS::DGGRS_getFirstSubZone_Functor::FunctionType DGGRS::DGGRS_getFirstSubZone_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getFirstSubZone);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getFirstSubZone_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getFirstSubZone_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getFirstSubZone_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getFirstSubZone)] = func;
   return func;
}
inline DGGRSZone DGGRS::DGGRS_getFirstSubZone_Functor::operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/int relativeDepth)
{
   SELF(DGGRS, getFirstSubZone);
   return DGGRS_getFirstSubZone(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/relativeDepth);
}
// inline void DGGRS::register_getFirstSubZone(CPPClass & cl, DGGRS::DGGRS_getFirstSubZone_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getFirstSubZone_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getFirstSubZone)] = func;
// }

inline DGGRS::DGGRS_getMaxDGGRSZoneLevel_Functor::FunctionType DGGRS::DGGRS_getMaxDGGRSZoneLevel_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getMaxDGGRSZoneLevel);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getMaxDGGRSZoneLevel_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getMaxDGGRSZoneLevel_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getMaxDGGRSZoneLevel_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getMaxDGGRSZoneLevel)] = func;
   return func;
}
inline int DGGRS::DGGRS_getMaxDGGRSZoneLevel_Functor::operator()( )
{
   SELF(DGGRS, getMaxDGGRSZoneLevel);
   return DGGRS_getMaxDGGRSZoneLevel(self ? self->impl : (C(DGGRS))null);
}
// inline void DGGRS::register_getMaxDGGRSZoneLevel(CPPClass & cl, DGGRS::DGGRS_getMaxDGGRSZoneLevel_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getMaxDGGRSZoneLevel_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getMaxDGGRSZoneLevel)] = func;
// }

inline DGGRS::DGGRS_getRefinementRatio_Functor::FunctionType DGGRS::DGGRS_getRefinementRatio_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getRefinementRatio);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getRefinementRatio_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getRefinementRatio_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getRefinementRatio_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getRefinementRatio)] = func;
   return func;
}
inline int DGGRS::DGGRS_getRefinementRatio_Functor::operator()( )
{
   SELF(DGGRS, getRefinementRatio);
   return DGGRS_getRefinementRatio(self ? self->impl : (C(DGGRS))null);
}
// inline void DGGRS::register_getRefinementRatio(CPPClass & cl, DGGRS::DGGRS_getRefinementRatio_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getRefinementRatio_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getRefinementRatio)] = func;
// }

inline DGGRS::DGGRS_getSubZoneCRSCentroids_Functor::FunctionType DGGRS::DGGRS_getSubZoneCRSCentroids_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getSubZoneCRSCentroids);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getSubZoneCRSCentroids_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getSubZoneCRSCentroids_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getSubZoneCRSCentroids_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getSubZoneCRSCentroids)] = func;
   return func;
}
inline TArray<Pointd _ARG int _ARG Pointd> DGGRS::DGGRS_getSubZoneCRSCentroids_Functor::operator()( /*6Fj*/DGGRSZone parent, /*6Fj*/CRS_ crs, /*6Fj*/int relativeDepth)
{
   SELF(DGGRS, getSubZoneCRSCentroids);
   C(Array) retArray = DGGRS_getSubZoneCRSCentroids(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))parent, /*7Al*/(C(CRS))crs, /*7Al*/relativeDepth);
      return TArray<Pointd _ARG int _ARG Pointd>(retArray);
}
// inline void DGGRS::register_getSubZoneCRSCentroids(CPPClass & cl, DGGRS::DGGRS_getSubZoneCRSCentroids_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getSubZoneCRSCentroids_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getSubZoneCRSCentroids)] = func;
// }

inline DGGRS::DGGRS_getSubZoneWGS84Centroids_Functor::FunctionType DGGRS::DGGRS_getSubZoneWGS84Centroids_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getSubZoneWGS84Centroids);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getSubZoneWGS84Centroids_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getSubZoneWGS84Centroids_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getSubZoneWGS84Centroids_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getSubZoneWGS84Centroids)] = func;
   return func;
}
inline TArray<GeoPoint _ARG int _ARG GeoPoint> DGGRS::DGGRS_getSubZoneWGS84Centroids_Functor::operator()( /*6Fj*/DGGRSZone parent, /*6Fj*/int relativeDepth)
{
   SELF(DGGRS, getSubZoneWGS84Centroids);
   C(Array) retArray = DGGRS_getSubZoneWGS84Centroids(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))parent, /*7Al*/relativeDepth);
      return TArray<GeoPoint _ARG int _ARG GeoPoint>(retArray);
}
// inline void DGGRS::register_getSubZoneWGS84Centroids(CPPClass & cl, DGGRS::DGGRS_getSubZoneWGS84Centroids_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getSubZoneWGS84Centroids_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getSubZoneWGS84Centroids)] = func;
// }

inline DGGRS::DGGRS_getZoneArea_Functor::FunctionType DGGRS::DGGRS_getZoneArea_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneArea);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneArea_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneArea_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneArea_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneArea)] = func;
   return func;
}
inline double DGGRS::DGGRS_getZoneArea_Functor::operator()( /*6Fj*/DGGRSZone zone)
{
   SELF(DGGRS, getZoneArea);
   return DGGRS_getZoneArea(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone);
}
// inline void DGGRS::register_getZoneArea(CPPClass & cl, DGGRS::DGGRS_getZoneArea_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneArea_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneArea)] = func;
// }

inline DGGRS::DGGRS_getZoneCRSCentroid_Functor::FunctionType DGGRS::DGGRS_getZoneCRSCentroid_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneCRSCentroid);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneCRSCentroid_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneCRSCentroid_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneCRSCentroid_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneCRSCentroid)] = func;
   return func;
}
inline void DGGRS::DGGRS_getZoneCRSCentroid_Functor::operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/CRS_ crs, /*6Fj*/Pointd & centroid)
{
   SELF(DGGRS, getZoneCRSCentroid);
   DGGRS_getZoneCRSCentroid(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/(C(CRS))crs, /*7Al*/&centroid.impl);
}
// inline void DGGRS::register_getZoneCRSCentroid(CPPClass & cl, DGGRS::DGGRS_getZoneCRSCentroid_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneCRSCentroid_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneCRSCentroid)] = func;
// }

inline DGGRS::DGGRS_getZoneCRSExtent_Functor::FunctionType DGGRS::DGGRS_getZoneCRSExtent_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneCRSExtent);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneCRSExtent_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneCRSExtent_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneCRSExtent_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneCRSExtent)] = func;
   return func;
}
inline void DGGRS::DGGRS_getZoneCRSExtent_Functor::operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/CRS_ crs, /*6Fj*/CRSExtent & extent)
{
   SELF(DGGRS, getZoneCRSExtent);
   DGGRS_getZoneCRSExtent(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/(C(CRS))crs, /*7Al*/&extent.impl);
}
// inline void DGGRS::register_getZoneCRSExtent(CPPClass & cl, DGGRS::DGGRS_getZoneCRSExtent_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneCRSExtent_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneCRSExtent)] = func;
// }

inline DGGRS::DGGRS_getZoneCRSVertices_Functor::FunctionType DGGRS::DGGRS_getZoneCRSVertices_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneCRSVertices);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneCRSVertices_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneCRSVertices_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneCRSVertices_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneCRSVertices)] = func;
   return func;
}
inline int DGGRS::DGGRS_getZoneCRSVertices_Functor::operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/CRS_ crs, /*6Fj*/Pointd * vertices)
{
   SELF(DGGRS, getZoneCRSVertices);
   return DGGRS_getZoneCRSVertices(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/(C(CRS))crs, /*7Al*/(C(Pointd) *)vertices);
}
// inline void DGGRS::register_getZoneCRSVertices(CPPClass & cl, DGGRS::DGGRS_getZoneCRSVertices_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneCRSVertices_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneCRSVertices)] = func;
// }

inline DGGRS::DGGRS_getZoneCentroidChild_Functor::FunctionType DGGRS::DGGRS_getZoneCentroidChild_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneCentroidChild);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneCentroidChild_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneCentroidChild_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneCentroidChild_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneCentroidChild)] = func;
   return func;
}
inline DGGRSZone DGGRS::DGGRS_getZoneCentroidChild_Functor::operator()( /*6Fj*/DGGRSZone zone)
{
   SELF(DGGRS, getZoneCentroidChild);
   return DGGRS_getZoneCentroidChild(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone);
}
// inline void DGGRS::register_getZoneCentroidChild(CPPClass & cl, DGGRS::DGGRS_getZoneCentroidChild_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneCentroidChild_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneCentroidChild)] = func;
// }

inline DGGRS::DGGRS_getZoneCentroidParent_Functor::FunctionType DGGRS::DGGRS_getZoneCentroidParent_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneCentroidParent);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneCentroidParent_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneCentroidParent_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneCentroidParent_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneCentroidParent)] = func;
   return func;
}
inline DGGRSZone DGGRS::DGGRS_getZoneCentroidParent_Functor::operator()( /*6Fj*/DGGRSZone zone)
{
   SELF(DGGRS, getZoneCentroidParent);
   return DGGRS_getZoneCentroidParent(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone);
}
// inline void DGGRS::register_getZoneCentroidParent(CPPClass & cl, DGGRS::DGGRS_getZoneCentroidParent_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneCentroidParent_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneCentroidParent)] = func;
// }

inline DGGRS::DGGRS_getZoneChildren_Functor::FunctionType DGGRS::DGGRS_getZoneChildren_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneChildren);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneChildren_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneChildren_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneChildren_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneChildren)] = func;
   return func;
}
inline int DGGRS::DGGRS_getZoneChildren_Functor::operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/DGGRSZone * children)
{
   SELF(DGGRS, getZoneChildren);
   return DGGRS_getZoneChildren(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/(C(DGGRSZone) *)children);
}
// inline void DGGRS::register_getZoneChildren(CPPClass & cl, DGGRS::DGGRS_getZoneChildren_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneChildren_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneChildren)] = func;
// }

inline DGGRS::DGGRS_getZoneFromCRSCentroid_Functor::FunctionType DGGRS::DGGRS_getZoneFromCRSCentroid_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneFromCRSCentroid);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneFromCRSCentroid_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneFromCRSCentroid_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneFromCRSCentroid_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneFromCRSCentroid)] = func;
   return func;
}
inline DGGRSZone DGGRS::DGGRS_getZoneFromCRSCentroid_Functor::operator()( /*6Fj*/int level, /*6Fj*/CRS_ crs, /*6Fj*/const Pointd & centroid)
{
   SELF(DGGRS, getZoneFromCRSCentroid);
   return DGGRS_getZoneFromCRSCentroid(self ? self->impl : (C(DGGRS))null, /*7Al*/level, /*7Al*/(C(CRS))crs, /*7Al*/&centroid.impl);
}
// inline void DGGRS::register_getZoneFromCRSCentroid(CPPClass & cl, DGGRS::DGGRS_getZoneFromCRSCentroid_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneFromCRSCentroid_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneFromCRSCentroid)] = func;
// }

inline DGGRS::DGGRS_getZoneFromTextID_Functor::FunctionType DGGRS::DGGRS_getZoneFromTextID_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneFromTextID);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneFromTextID_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneFromTextID_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneFromTextID_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneFromTextID)] = func;
   return func;
}
inline DGGRSZone DGGRS::DGGRS_getZoneFromTextID_Functor::operator()( /*6Fj*/constString zoneID)
{
   SELF(DGGRS, getZoneFromTextID);
   return DGGRS_getZoneFromTextID(self ? self->impl : (C(DGGRS))null, /*7Al*/zoneID);
}
// inline void DGGRS::register_getZoneFromTextID(CPPClass & cl, DGGRS::DGGRS_getZoneFromTextID_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneFromTextID_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneFromTextID)] = func;
// }

inline DGGRS::DGGRS_getZoneFromWGS84Centroid_Functor::FunctionType DGGRS::DGGRS_getZoneFromWGS84Centroid_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneFromWGS84Centroid);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneFromWGS84Centroid_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneFromWGS84Centroid_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneFromWGS84Centroid_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneFromWGS84Centroid)] = func;
   return func;
}
inline DGGRSZone DGGRS::DGGRS_getZoneFromWGS84Centroid_Functor::operator()( /*6Fj*/int level, /*6Fj*/const GeoPoint & centroid)
{
   SELF(DGGRS, getZoneFromWGS84Centroid);
   return DGGRS_getZoneFromWGS84Centroid(self ? self->impl : (C(DGGRS))null, /*7Al*/level, /*7Al*/&centroid.impl);
}
// inline void DGGRS::register_getZoneFromWGS84Centroid(CPPClass & cl, DGGRS::DGGRS_getZoneFromWGS84Centroid_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneFromWGS84Centroid_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneFromWGS84Centroid)] = func;
// }

inline DGGRS::DGGRS_getZoneLevel_Functor::FunctionType DGGRS::DGGRS_getZoneLevel_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneLevel);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneLevel_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneLevel_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneLevel_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneLevel)] = func;
   return func;
}
inline int DGGRS::DGGRS_getZoneLevel_Functor::operator()( /*6Fj*/DGGRSZone zone)
{
   SELF(DGGRS, getZoneLevel);
   return DGGRS_getZoneLevel(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone);
}
// inline void DGGRS::register_getZoneLevel(CPPClass & cl, DGGRS::DGGRS_getZoneLevel_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneLevel_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneLevel)] = func;
// }

inline DGGRS::DGGRS_getZoneNeighbors_Functor::FunctionType DGGRS::DGGRS_getZoneNeighbors_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneNeighbors);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneNeighbors_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneNeighbors_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneNeighbors_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneNeighbors)] = func;
   return func;
}
inline int DGGRS::DGGRS_getZoneNeighbors_Functor::operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/DGGRSZone * neighbors, /*6Fj*/int * nbType)
{
   SELF(DGGRS, getZoneNeighbors);
   return DGGRS_getZoneNeighbors(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/(C(DGGRSZone) *)neighbors, /*7Al*/nbType);
}
// inline void DGGRS::register_getZoneNeighbors(CPPClass & cl, DGGRS::DGGRS_getZoneNeighbors_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneNeighbors_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneNeighbors)] = func;
// }

inline DGGRS::DGGRS_getZoneParents_Functor::FunctionType DGGRS::DGGRS_getZoneParents_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneParents);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneParents_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneParents_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneParents_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneParents)] = func;
   return func;
}
inline int DGGRS::DGGRS_getZoneParents_Functor::operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/DGGRSZone * parents)
{
   SELF(DGGRS, getZoneParents);
   return DGGRS_getZoneParents(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/(C(DGGRSZone) *)parents);
}
// inline void DGGRS::register_getZoneParents(CPPClass & cl, DGGRS::DGGRS_getZoneParents_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneParents_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneParents)] = func;
// }

inline DGGRS::DGGRS_getZoneRefinedCRSVertices_Functor::FunctionType DGGRS::DGGRS_getZoneRefinedCRSVertices_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneRefinedCRSVertices);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneRefinedCRSVertices_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneRefinedCRSVertices_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneRefinedCRSVertices_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneRefinedCRSVertices)] = func;
   return func;
}
inline TArray<Pointd _ARG int _ARG Pointd> DGGRS::DGGRS_getZoneRefinedCRSVertices_Functor::operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/CRS_ crs, /*6Fj*/int edgeRefinement)
{
   SELF(DGGRS, getZoneRefinedCRSVertices);
   C(Array) retArray = DGGRS_getZoneRefinedCRSVertices(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/(C(CRS))crs, /*7Al*/edgeRefinement);
      return TArray<Pointd _ARG int _ARG Pointd>(retArray);
}
// inline void DGGRS::register_getZoneRefinedCRSVertices(CPPClass & cl, DGGRS::DGGRS_getZoneRefinedCRSVertices_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneRefinedCRSVertices_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneRefinedCRSVertices)] = func;
// }

inline DGGRS::DGGRS_getZoneRefinedWGS84Vertices_Functor::FunctionType DGGRS::DGGRS_getZoneRefinedWGS84Vertices_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneRefinedWGS84Vertices);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneRefinedWGS84Vertices_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneRefinedWGS84Vertices_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneRefinedWGS84Vertices_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneRefinedWGS84Vertices)] = func;
   return func;
}
inline TArray<GeoPoint _ARG int _ARG GeoPoint> DGGRS::DGGRS_getZoneRefinedWGS84Vertices_Functor::operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/int edgeRefinement)
{
   SELF(DGGRS, getZoneRefinedWGS84Vertices);
   C(Array) retArray = DGGRS_getZoneRefinedWGS84Vertices(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/edgeRefinement);
      return TArray<GeoPoint _ARG int _ARG GeoPoint>(retArray);
}
// inline void DGGRS::register_getZoneRefinedWGS84Vertices(CPPClass & cl, DGGRS::DGGRS_getZoneRefinedWGS84Vertices_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneRefinedWGS84Vertices_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneRefinedWGS84Vertices)] = func;
// }

inline DGGRS::DGGRS_getZoneTextID_Functor::FunctionType DGGRS::DGGRS_getZoneTextID_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneTextID);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneTextID_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneTextID_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneTextID_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneTextID)] = func;
   return func;
}
inline void DGGRS::DGGRS_getZoneTextID_Functor::operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/C(String) zoneID)
{
   SELF(DGGRS, getZoneTextID);
   DGGRS_getZoneTextID(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/zoneID);
}
// inline void DGGRS::register_getZoneTextID(CPPClass & cl, DGGRS::DGGRS_getZoneTextID_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneTextID_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneTextID)] = func;
// }

inline DGGRS::DGGRS_getZoneWGS84Centroid_Functor::FunctionType DGGRS::DGGRS_getZoneWGS84Centroid_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneWGS84Centroid);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneWGS84Centroid_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneWGS84Centroid_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneWGS84Centroid_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneWGS84Centroid)] = func;
   return func;
}
inline void DGGRS::DGGRS_getZoneWGS84Centroid_Functor::operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/GeoPoint & centroid)
{
   SELF(DGGRS, getZoneWGS84Centroid);
   DGGRS_getZoneWGS84Centroid(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/&centroid.impl);
}
// inline void DGGRS::register_getZoneWGS84Centroid(CPPClass & cl, DGGRS::DGGRS_getZoneWGS84Centroid_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneWGS84Centroid_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneWGS84Centroid)] = func;
// }

inline DGGRS::DGGRS_getZoneWGS84Extent_Functor::FunctionType DGGRS::DGGRS_getZoneWGS84Extent_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneWGS84Extent);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneWGS84Extent_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneWGS84Extent_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneWGS84Extent_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneWGS84Extent)] = func;
   return func;
}
inline void DGGRS::DGGRS_getZoneWGS84Extent_Functor::operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/GeoExtent & extent)
{
   SELF(DGGRS, getZoneWGS84Extent);
   DGGRS_getZoneWGS84Extent(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/&extent.impl);
}
// inline void DGGRS::register_getZoneWGS84Extent(CPPClass & cl, DGGRS::DGGRS_getZoneWGS84Extent_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneWGS84Extent_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneWGS84Extent)] = func;
// }

inline DGGRS::DGGRS_getZoneWGS84Vertices_Functor::FunctionType DGGRS::DGGRS_getZoneWGS84Vertices_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, getZoneWGS84Vertices);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_getZoneWGS84Vertices_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_getZoneWGS84Vertices_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_getZoneWGS84Vertices_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, getZoneWGS84Vertices)] = func;
   return func;
}
inline int DGGRS::DGGRS_getZoneWGS84Vertices_Functor::operator()( /*6Fj*/DGGRSZone zone, /*6Fj*/GeoPoint * vertices)
{
   SELF(DGGRS, getZoneWGS84Vertices);
   return DGGRS_getZoneWGS84Vertices(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone, /*7Al*/(C(GeoPoint) *)vertices);
}
// inline void DGGRS::register_getZoneWGS84Vertices(CPPClass & cl, DGGRS::DGGRS_getZoneWGS84Vertices_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_getZoneWGS84Vertices_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, getZoneWGS84Vertices)] = func;
// }

inline DGGRS::DGGRS_isZoneCentroidChild_Functor::FunctionType DGGRS::DGGRS_isZoneCentroidChild_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, isZoneCentroidChild);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_isZoneCentroidChild_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_isZoneCentroidChild_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_isZoneCentroidChild_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, isZoneCentroidChild)] = func;
   return func;
}
inline bool DGGRS::DGGRS_isZoneCentroidChild_Functor::operator()( /*6Fj*/DGGRSZone zone)
{
   SELF(DGGRS, isZoneCentroidChild);
   return (bool)DGGRS_isZoneCentroidChild(self ? self->impl : (C(DGGRS))null, /*7Al*/(C(DGGRSZone))zone);
}
// inline void DGGRS::register_isZoneCentroidChild(CPPClass & cl, DGGRS::DGGRS_isZoneCentroidChild_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_isZoneCentroidChild_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, isZoneCentroidChild)] = func;
// }

inline DGGRS::DGGRS_listZones_Functor::FunctionType DGGRS::DGGRS_listZones_Functor::operator= (FunctionType func)
{
   SELF(DGGRS, listZones);
   if(self->vTbl == DGGRS::_cpp_class.vTbl)
   {
      uint size = DGGRS::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(DGGRS::DGGRS_listZones_Functor::FunctionType, size);
      memcpy(self->vTbl, DGGRS::_cpp_class.vTbl, sizeof(DGGRS::DGGRS_listZones_Functor::FunctionType) * size);
   }
   ((DGGRS::DGGRS_listZones_Functor::FunctionType *)self->vTbl)[M_VTBLID(DGGRS, listZones)] = func;
   return func;
}
inline TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)> DGGRS::DGGRS_listZones_Functor::operator()( /*6Fj*/int level, /*6Fj*/const GeoExtent & bbox)
{
   SELF(DGGRS, listZones);
   C(Array) retArray = DGGRS_listZones(self ? self->impl : (C(DGGRS))null, /*7Al*/level, /*7Al*/&bbox.impl);
      return TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)>(retArray);
}
// inline void DGGRS::register_listZones(CPPClass & cl, DGGRS::DGGRS_listZones_Functor::FunctionType func)
// {
//    ((DGGRS::DGGRS_listZones_Functor::FunctionType *)cl.vTbl)[M_VTBLID(DGGRS, listZones)] = func;
// }


inline C(bool) DGGRS::areZonesNeighbors(/*1Ab*/DGGRSZone a, /*1Ab*/DGGRSZone b)
{
   return DGGRS_areZonesNeighbors(impl, /*5Ee*/(C(DGGRSZone))a.impl, /*5Ee*/(C(DGGRSZone))b.impl);
}
inline C(bool) DGGRS::areZonesSiblings(/*1Ab*/DGGRSZone a, /*1Ab*/DGGRSZone b)
{
   return DGGRS_areZonesSiblings(impl, /*5Ee*/(C(DGGRSZone))a.impl, /*5Ee*/(C(DGGRSZone))b.impl);
}
inline C(bool) DGGRS::doZonesOverlap(/*1Ab*/DGGRSZone a, /*1Ab*/DGGRSZone b)
{
   return DGGRS_doZonesOverlap(impl, /*5Ee*/(C(DGGRSZone))a.impl, /*5Ee*/(C(DGGRSZone))b.impl);
}
inline C(bool) DGGRS::doesZoneContain(/*1Ab*/DGGRSZone hayStack, /*1Ab*/DGGRSZone needle)
{
   return DGGRS_doesZoneContain(impl, /*5Ee*/(C(DGGRSZone))hayStack.impl, /*5Ee*/(C(DGGRSZone))needle.impl);
}
inline int DGGRS::get64KDepth()
{
   return DGGRS_get64KDepth(impl);
}
inline int DGGRS::getLevelFromMetersPerSubZone(/*1Ab*/double physicalMetersPerSubZone, /*1Ab*/int relativeDepth)
{
   return DGGRS_getLevelFromMetersPerSubZone(impl, /*5Ge*/physicalMetersPerSubZone, /*5Ge*/relativeDepth);
}
inline int DGGRS::getLevelFromPixelsAndExtent(/*1Ab*/const GeoExtent & extent, /*1Ab*/Point & pixels, /*1Ab*/int relativeDepth)
{
   return DGGRS_getLevelFromPixelsAndExtent(impl, /*5De*/&extent.impl, /*5De*/&pixels.impl, /*5Ge*/relativeDepth);
}
inline int DGGRS::getLevelFromPixelsAndExtent(/*1Ac*/const GeoExtent * extent, /*1Ac*/Point * pixels, /*1Ac*/int relativeDepth)
{
   return DGGRS_getLevelFromPixelsAndExtent(impl, /*5Cf*/(C(GeoExtent) *)extent, /*5Cf*/(C(Point) *)pixels, /*5Gf*/relativeDepth);
}
inline int DGGRS::getLevelFromRefZoneArea(/*1Ab*/double metersSquared)
{
   return DGGRS_getLevelFromRefZoneArea(impl, /*5Ge*/metersSquared);
}
inline int DGGRS::getLevelFromScaleDenominator(/*1Ab*/double scaleDenominator, /*1Ab*/int relativeDepth, /*1Ab*/double mmPerPixel)
{
   return DGGRS_getLevelFromScaleDenominator(impl, /*5Ge*/scaleDenominator, /*5Ge*/relativeDepth, /*5Ge*/mmPerPixel);
}
inline int DGGRS::getMaxDepth()
{
   return DGGRS_getMaxDepth(impl);
}
inline double DGGRS::getMetersPerSubZoneFromLevel(/*1Ab*/int parentLevel, /*1Ab*/int relativeDepth)
{
   return DGGRS_getMetersPerSubZoneFromLevel(impl, /*5Ge*/parentLevel, /*5Ge*/relativeDepth);
}
inline double DGGRS::getRefZoneArea(/*1Ab*/int level)
{
   return DGGRS_getRefZoneArea(impl, /*5Ge*/level);
}
inline double DGGRS::getScaleDenominatorFromLevel(/*1Ab*/int parentLevel, /*1Ab*/int relativeDepth, /*1Ab*/double mmPerPixel)
{
   return DGGRS_getScaleDenominatorFromLevel(impl, /*5Ge*/parentLevel, /*5Ge*/relativeDepth, /*5Ge*/mmPerPixel);
}
inline C(DGGRSZone) DGGRS::getSubZoneAtIndex(/*1Ab*/DGGRSZone parent, /*1Ab*/int relativeDepth, /*1Ab*/int index)
{
   return DGGRS_getSubZoneAtIndex(impl, /*5Ee*/(C(DGGRSZone))parent.impl, /*5Ge*/relativeDepth, /*5Ge*/index);
}
inline int DGGRS::getSubZoneIndex(/*1Ab*/DGGRSZone parent, /*1Ab*/DGGRSZone subZone)
{
   return DGGRS_getSubZoneIndex(impl, /*5Ee*/(C(DGGRSZone))parent.impl, /*5Ee*/(C(DGGRSZone))subZone.impl);
}
inline TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)> DGGRS::getSubZones(/*1Ab*/DGGRSZone parent, /*1Ab*/int relativeDepth)
{
   return TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)>(DGGRS_getSubZones(impl, /*5Ee*/(C(DGGRSZone))parent.impl, /*5Ge*/relativeDepth));
}
inline C(bool) DGGRS::isZoneAncestorOf(/*1Ab*/DGGRSZone ancestor, /*1Ab*/DGGRSZone descendant, /*1Ab*/int maxDepth)
{
   return DGGRS_isZoneAncestorOf(impl, /*5Ee*/(C(DGGRSZone))ancestor.impl, /*5Ee*/(C(DGGRSZone))descendant.impl, /*5Ge*/maxDepth);
}
inline C(bool) DGGRS::isZoneContainedIn(/*1Ab*/DGGRSZone needle, /*1Ab*/DGGRSZone hayStack)
{
   return DGGRS_isZoneContainedIn(impl, /*5Ee*/(C(DGGRSZone))needle.impl, /*5Ee*/(C(DGGRSZone))hayStack.impl);
}
inline C(bool) DGGRS::isZoneDescendantOf(/*1Ab*/DGGRSZone descendant, /*1Ab*/DGGRSZone ancestor, /*1Ab*/int maxDepth)
{
   return DGGRS_isZoneDescendantOf(impl, /*5Ee*/(C(DGGRSZone))descendant.impl, /*5Ee*/(C(DGGRSZone))ancestor.impl, /*5Ge*/maxDepth);
}
inline C(bool) DGGRS::isZoneImmediateChildOf(/*1Ab*/DGGRSZone child, /*1Ab*/DGGRSZone parent)
{
   return DGGRS_isZoneImmediateChildOf(impl, /*5Ee*/(C(DGGRSZone))child.impl, /*5Ee*/(C(DGGRSZone))parent.impl);
}
inline C(bool) DGGRS::isZoneImmediateParentOf(/*1Ab*/DGGRSZone parent, /*1Ab*/DGGRSZone child)
{
   return DGGRS_isZoneImmediateParentOf(impl, /*5Ee*/(C(DGGRSZone))parent.impl, /*5Ee*/(C(DGGRSZone))child.impl);
}


inline DGGSJSON::DGGSJSON(char * dggrs, char * zoneId = 0) : DGGSJSON()
{
   this->dggrs = dggrs;
   this->zoneId = zoneId;
}



// member accessors: DGGSJSON::dggrs
// (normal::string)

/*nstSet*/inline /*0H*/char * DGGSJSON::dggrs_Prop::operator= (/*0H*/char * v)
{
   SELF(DGGSJSON, dggrs);
   IPTR(self->impl, DGGSJSON)->dggrs = v;
   return v;
}

/*regSet*/inline DGGSJSON::dggrs_Prop & DGGSJSON::dggrs_Prop::operator= (DGGSJSON::dggrs_Prop & prop)
{
   SELF(DGGSJSON, dggrs);
   /*0H*/char * v = prop;
   IPTR(self->impl, DGGSJSON)->dggrs = v;
   return prop;
}
/*regGet*/inline DGGSJSON::dggrs_Prop::operator /*0I*/char * () const
{
   SELF(DGGSJSON, dggrs);
   return self ? IPTR(self->impl, DGGSJSON)->dggrs : 0;
}

// member accessors: DGGSJSON::zoneId
// (normal::string)

/*nstSet*/inline /*0H*/char * DGGSJSON::zoneId_Prop::operator= (/*0H*/char * v)
{
   SELF(DGGSJSON, zoneId);
   IPTR(self->impl, DGGSJSON)->zoneId = v;
   return v;
}

/*regSet*/inline DGGSJSON::zoneId_Prop & DGGSJSON::zoneId_Prop::operator= (DGGSJSON::zoneId_Prop & prop)
{
   SELF(DGGSJSON, zoneId);
   /*0H*/char * v = prop;
   IPTR(self->impl, DGGSJSON)->zoneId = v;
   return prop;
}
/*regGet*/inline DGGSJSON::zoneId_Prop::operator /*0I*/char * () const
{
   SELF(DGGSJSON, zoneId);
   return self ? IPTR(self->impl, DGGSJSON)->zoneId : 0;
}

// member accessors: DGGSJSON::depths
// (normal::normal)

/*nstSet*/inline /*0H*/TArray<int _ARG int _ARG int> DGGSJSON::depths_Prop::operator= (/*0H*/TArray<int _ARG int _ARG int> v)
{
   SELF(DGGSJSON, depths);
   IPTR(self->impl, DGGSJSON)->depths = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline DGGSJSON::depths_Prop & DGGSJSON::depths_Prop::operator= (DGGSJSON::depths_Prop & prop)
{
   SELF(DGGSJSON, depths);
   /*0H*/TArray<int _ARG int _ARG int> v = prop;
   IPTR(self->impl, DGGSJSON)->depths = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline DGGSJSON::depths_Prop::operator /*0I*/TArray<int _ARG int _ARG int> () const
{
   SELF(DGGSJSON, depths);
   TArray<int _ARG int _ARG int> value(IPTR(self->impl, DGGSJSON)->depths, DGGSJSON::_cpp_class);
   return value;
}

// member accessors: DGGSJSON::representedValue
// (normal::string)

/*nstSet*/inline /*0H*/char * DGGSJSON::representedValue_Prop::operator= (/*0H*/char * v)
{
   SELF(DGGSJSON, representedValue);
   IPTR(self->impl, DGGSJSON)->representedValue = v;
   return v;
}

/*regSet*/inline DGGSJSON::representedValue_Prop & DGGSJSON::representedValue_Prop::operator= (DGGSJSON::representedValue_Prop & prop)
{
   SELF(DGGSJSON, representedValue);
   /*0H*/char * v = prop;
   IPTR(self->impl, DGGSJSON)->representedValue = v;
   return prop;
}
/*regGet*/inline DGGSJSON::representedValue_Prop::operator /*0I*/char * () const
{
   SELF(DGGSJSON, representedValue);
   return self ? IPTR(self->impl, DGGSJSON)->representedValue : 0;
}

// member accessors: DGGSJSON::schema
// (normal::normal)

/*nstSet*/inline /*0H*/JSONSchema DGGSJSON::schema_Prop::operator= (/*0H*/JSONSchema v)
{
   SELF(DGGSJSON, schema);
   IPTR(self->impl, DGGSJSON)->schema = v.impl;
   return v;
}

/*regSet*/inline DGGSJSON::schema_Prop & DGGSJSON::schema_Prop::operator= (DGGSJSON::schema_Prop & prop)
{
   SELF(DGGSJSON, schema);
   /*0H*/JSONSchema v = prop;
   IPTR(self->impl, DGGSJSON)->schema = v.impl;
   return prop;
}
/*regGet*/inline DGGSJSON::schema_Prop::operator /*0I*/JSONSchema () const
{
   SELF(DGGSJSON, schema);
   JSONSchema value(IPTR(self->impl, DGGSJSON)->schema, DGGSJSON::_cpp_class);
   return value;
}

// member accessors: DGGSJSON::dimensions
// (normal::normal)

/*nstSet*/inline /*0H*/Array DGGSJSON::dimensions_Prop::operator= (/*0H*/Array v)
{
   SELF(DGGSJSON, dimensions);
   Instance * vInstance = (Instance *)&v; // REVIEW: Warning strict aliasing warning if casting directly
   IPTR(self->impl, DGGSJSON)->dimensions = vInstance->impl;
   return v;
}

/*regSet*/inline DGGSJSON::dimensions_Prop & DGGSJSON::dimensions_Prop::operator= (DGGSJSON::dimensions_Prop & prop)
{
   SELF(DGGSJSON, dimensions);
   /*0H*/Array v = prop;
   Instance * vInstance = (Instance *)&v; // REVIEW: Warning strict aliasing warning if casting directly
   IPTR(self->impl, DGGSJSON)->dimensions = vInstance->impl;
   return prop;
}
/*regGet*/inline DGGSJSON::dimensions_Prop::operator /*0I*/Array () const
{
   SELF(DGGSJSON, dimensions);
   Array value; // FIXME: (IPTR(self->impl, DGGSJSON)->dimensions, DGGSJSON::_cpp_class);
   return value;
}

// member accessors: DGGSJSON::values
// (normal::normal)

/*nstSet*/inline /*0H*/TMap<C(String) _ARG T(Array, DGGSJSONDepth)> DGGSJSON::values_Prop::operator= (/*0H*/TMap<C(String) _ARG T(Array, DGGSJSONDepth)> v)
{
   SELF(DGGSJSON, values);
   IPTR(self->impl, DGGSJSON)->values = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline DGGSJSON::values_Prop & DGGSJSON::values_Prop::operator= (DGGSJSON::values_Prop & prop)
{
   SELF(DGGSJSON, values);
   /*0H*/TMap<C(String) _ARG T(Array, DGGSJSONDepth)> v = prop;
   IPTR(self->impl, DGGSJSON)->values = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline DGGSJSON::values_Prop::operator /*0I*/TMap<C(String) _ARG T(Array, DGGSJSONDepth)> () const
{
   SELF(DGGSJSON, values);
   TMap<C(String) _ARG T(Array, DGGSJSONDepth)> value(IPTR(self->impl, DGGSJSON)->values, DGGSJSON::_cpp_class);
   return value;
}

inline DGGSJSONDepth::DGGSJSONDepth(int depth) : DGGSJSONDepth()
{
   this->depth = depth;
}



// member accessors: DGGSJSONDepth::depth
// (normal::int)

/*nstSet*/inline /*0H*/int DGGSJSONDepth::depth_Prop::operator= (/*0H*/int v)
{
   SELF(DGGSJSONDepth, depth);
   IPTR(self->impl, DGGSJSONDepth)->depth = v;
   return v;
}

/*regSet*/inline DGGSJSONDepth::depth_Prop & DGGSJSONDepth::depth_Prop::operator= (DGGSJSONDepth::depth_Prop & prop)
{
   SELF(DGGSJSONDepth, depth);
   /*0H*/int v = prop;
   IPTR(self->impl, DGGSJSONDepth)->depth = v;
   return prop;
}
/*regGet*/inline DGGSJSONDepth::depth_Prop::operator /*0I*/int () const
{
   SELF(DGGSJSONDepth, depth);
   return self ? IPTR(self->impl, DGGSJSONDepth)->depth : 0;
}

// member accessors: DGGSJSONDepth::shape
// (normal::normal)

/*nstSet*/inline /*0H*/DGGSJSONShape DGGSJSONDepth::shape_Prop::operator= (/*0H*/DGGSJSONShape v)
{
   SELF(DGGSJSONDepth, shape);
   IPTR(self->impl, DGGSJSONDepth)->shape = v.impl;
   return v;
}

/*regSet*/inline DGGSJSONDepth::shape_Prop & DGGSJSONDepth::shape_Prop::operator= (DGGSJSONDepth::shape_Prop & prop)
{
   SELF(DGGSJSONDepth, shape);
   /*0H*/DGGSJSONShape v = prop;
   IPTR(self->impl, DGGSJSONDepth)->shape = v.impl;
   return prop;
}
/*regGet*/inline DGGSJSONDepth::shape_Prop::operator /*0I*/DGGSJSONShape () const
{
   SELF(DGGSJSONDepth, shape);
   DGGSJSONShape value(IPTR(self->impl, DGGSJSONDepth)->shape, DGGSJSONDepth::_cpp_class);
   return value;
}

// member accessors: DGGSJSONDepth::data
// (normal::normal)

/*nstSet*/inline /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> DGGSJSONDepth::data_Prop::operator= (/*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v)
{
   SELF(DGGSJSONDepth, data);
   IPTR(self->impl, DGGSJSONDepth)->data = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline DGGSJSONDepth::data_Prop & DGGSJSONDepth::data_Prop::operator= (DGGSJSONDepth::data_Prop & prop)
{
   SELF(DGGSJSONDepth, data);
   /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v = prop;
   IPTR(self->impl, DGGSJSONDepth)->data = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline DGGSJSONDepth::data_Prop::operator /*0I*/TArray<FieldValue _ARG int _ARG FieldValue> () const
{
   SELF(DGGSJSONDepth, data);
   TArray<FieldValue _ARG int _ARG FieldValue> value(IPTR(self->impl, DGGSJSONDepth)->data, DGGSJSONDepth::_cpp_class);
   return value;
}

inline DGGSJSONGrid::DGGSJSONGrid(int cellsCount, double resolution = 0) : DGGSJSONGrid()
{
   this->cellsCount = cellsCount;
   this->resolution = resolution;
}



// member accessors: DGGSJSONGrid::cellsCount
// (normal::int)

/*nstSet*/inline /*0H*/int DGGSJSONGrid::cellsCount_Prop::operator= (/*0H*/int v)
{
   SELF(DGGSJSONGrid, cellsCount);
   IPTR(self->impl, DGGSJSONGrid)->cellsCount = v;
   return v;
}

/*regSet*/inline DGGSJSONGrid::cellsCount_Prop & DGGSJSONGrid::cellsCount_Prop::operator= (DGGSJSONGrid::cellsCount_Prop & prop)
{
   SELF(DGGSJSONGrid, cellsCount);
   /*0H*/int v = prop;
   IPTR(self->impl, DGGSJSONGrid)->cellsCount = v;
   return prop;
}
/*regGet*/inline DGGSJSONGrid::cellsCount_Prop::operator /*0I*/int () const
{
   SELF(DGGSJSONGrid, cellsCount);
   return self ? IPTR(self->impl, DGGSJSONGrid)->cellsCount : 0;
}

// member accessors: DGGSJSONGrid::resolution
// (normal::double)

/*nstSet*/inline /*0H*/double DGGSJSONGrid::resolution_Prop::operator= (/*0H*/double v)
{
   SELF(DGGSJSONGrid, resolution);
   IPTR(self->impl, DGGSJSONGrid)->resolution = v;
   return v;
}

/*regSet*/inline DGGSJSONGrid::resolution_Prop & DGGSJSONGrid::resolution_Prop::operator= (DGGSJSONGrid::resolution_Prop & prop)
{
   SELF(DGGSJSONGrid, resolution);
   /*0H*/double v = prop;
   IPTR(self->impl, DGGSJSONGrid)->resolution = v;
   return prop;
}
/*regGet*/inline DGGSJSONGrid::resolution_Prop::operator /*0I*/double () const
{
   SELF(DGGSJSONGrid, resolution);
   return self ? IPTR(self->impl, DGGSJSONGrid)->resolution : 0;
}

// member accessors: DGGSJSONGrid::coordinates
// (normal::normal)

/*nstSet*/inline /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> DGGSJSONGrid::coordinates_Prop::operator= (/*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v)
{
   SELF(DGGSJSONGrid, coordinates);
   IPTR(self->impl, DGGSJSONGrid)->coordinates = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline DGGSJSONGrid::coordinates_Prop & DGGSJSONGrid::coordinates_Prop::operator= (DGGSJSONGrid::coordinates_Prop & prop)
{
   SELF(DGGSJSONGrid, coordinates);
   /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v = prop;
   IPTR(self->impl, DGGSJSONGrid)->coordinates = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline DGGSJSONGrid::coordinates_Prop::operator /*0I*/TArray<FieldValue _ARG int _ARG FieldValue> () const
{
   SELF(DGGSJSONGrid, coordinates);
   TArray<FieldValue _ARG int _ARG FieldValue> value(IPTR(self->impl, DGGSJSONGrid)->coordinates, DGGSJSONGrid::_cpp_class);
   return value;
}

// member accessors: DGGSJSONGrid::boundsCoordinates
// (normal::normal)

/*nstSet*/inline /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> DGGSJSONGrid::boundsCoordinates_Prop::operator= (/*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v)
{
   SELF(DGGSJSONGrid, boundsCoordinates);
   IPTR(self->impl, DGGSJSONGrid)->boundsCoordinates = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline DGGSJSONGrid::boundsCoordinates_Prop & DGGSJSONGrid::boundsCoordinates_Prop::operator= (DGGSJSONGrid::boundsCoordinates_Prop & prop)
{
   SELF(DGGSJSONGrid, boundsCoordinates);
   /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v = prop;
   IPTR(self->impl, DGGSJSONGrid)->boundsCoordinates = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline DGGSJSONGrid::boundsCoordinates_Prop::operator /*0I*/TArray<FieldValue _ARG int _ARG FieldValue> () const
{
   SELF(DGGSJSONGrid, boundsCoordinates);
   TArray<FieldValue _ARG int _ARG FieldValue> value(IPTR(self->impl, DGGSJSONGrid)->boundsCoordinates, DGGSJSONGrid::_cpp_class);
   return value;
}

// member accessors: DGGSJSONGrid::relativeBounds
// (normal::normal)

/*nstSet*/inline /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> DGGSJSONGrid::relativeBounds_Prop::operator= (/*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v)
{
   SELF(DGGSJSONGrid, relativeBounds);
   IPTR(self->impl, DGGSJSONGrid)->relativeBounds = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline DGGSJSONGrid::relativeBounds_Prop & DGGSJSONGrid::relativeBounds_Prop::operator= (DGGSJSONGrid::relativeBounds_Prop & prop)
{
   SELF(DGGSJSONGrid, relativeBounds);
   /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v = prop;
   IPTR(self->impl, DGGSJSONGrid)->relativeBounds = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline DGGSJSONGrid::relativeBounds_Prop::operator /*0I*/TArray<FieldValue _ARG int _ARG FieldValue> () const
{
   SELF(DGGSJSONGrid, relativeBounds);
   TArray<FieldValue _ARG int _ARG FieldValue> value(IPTR(self->impl, DGGSJSONGrid)->relativeBounds, DGGSJSONGrid::_cpp_class);
   return value;
}

// member accessors: DGGSJSONGrid::firstCoordinate
// (normal::struct)

/*nstSet*/inline /*0H*/FieldValue DGGSJSONGrid::firstCoordinate_Prop::operator= (/*0H*/FieldValue v)
{
   SELF(DGGSJSONGrid, firstCoordinate);
   IPTR(self->impl, DGGSJSONGrid)->firstCoordinate = v.impl;
   return v;
}

/*regSet*/inline DGGSJSONGrid::firstCoordinate_Prop & DGGSJSONGrid::firstCoordinate_Prop::operator= (DGGSJSONGrid::firstCoordinate_Prop & prop)
{
   SELF(DGGSJSONGrid, firstCoordinate);
   /*0H*/FieldValue v = prop;
   IPTR(self->impl, DGGSJSONGrid)->firstCoordinate = v.impl;
   return prop;
}
/*regGet*/inline DGGSJSONGrid::firstCoordinate_Prop::operator /*0I*/FieldValue () const
{
   SELF(DGGSJSONGrid, firstCoordinate);
   FieldValue value(IPTR(self->impl, DGGSJSONGrid)->firstCoordinate);
   return value;
}

inline DGGSJSONShape::DGGSJSONShape(int count, int subZones = 0) : DGGSJSONShape()
{
   this->count = count;
   this->subZones = subZones;
}



// member accessors: DGGSJSONShape::count
// (normal::int)

/*nstSet*/inline /*0H*/int DGGSJSONShape::count_Prop::operator= (/*0H*/int v)
{
   SELF(DGGSJSONShape, count);
   IPTR(self->impl, DGGSJSONShape)->count = v;
   return v;
}

/*regSet*/inline DGGSJSONShape::count_Prop & DGGSJSONShape::count_Prop::operator= (DGGSJSONShape::count_Prop & prop)
{
   SELF(DGGSJSONShape, count);
   /*0H*/int v = prop;
   IPTR(self->impl, DGGSJSONShape)->count = v;
   return prop;
}
/*regGet*/inline DGGSJSONShape::count_Prop::operator /*0I*/int () const
{
   SELF(DGGSJSONShape, count);
   return self ? IPTR(self->impl, DGGSJSONShape)->count : 0;
}

// member accessors: DGGSJSONShape::subZones
// (normal::int)

/*nstSet*/inline /*0H*/int DGGSJSONShape::subZones_Prop::operator= (/*0H*/int v)
{
   SELF(DGGSJSONShape, subZones);
   IPTR(self->impl, DGGSJSONShape)->subZones = v;
   return v;
}

/*regSet*/inline DGGSJSONShape::subZones_Prop & DGGSJSONShape::subZones_Prop::operator= (DGGSJSONShape::subZones_Prop & prop)
{
   SELF(DGGSJSONShape, subZones);
   /*0H*/int v = prop;
   IPTR(self->impl, DGGSJSONShape)->subZones = v;
   return prop;
}
/*regGet*/inline DGGSJSONShape::subZones_Prop::operator /*0I*/int () const
{
   SELF(DGGSJSONShape, subZones);
   return self ? IPTR(self->impl, DGGSJSONShape)->subZones : 0;
}

// member accessors: DGGSJSONShape::dimensions
// (normal::normal)

/*nstSet*/inline /*0H*/TMap<C(String) _ARG int> DGGSJSONShape::dimensions_Prop::operator= (/*0H*/TMap<C(String) _ARG int> v)
{
   SELF(DGGSJSONShape, dimensions);
   IPTR(self->impl, DGGSJSONShape)->dimensions = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline DGGSJSONShape::dimensions_Prop & DGGSJSONShape::dimensions_Prop::operator= (DGGSJSONShape::dimensions_Prop & prop)
{
   SELF(DGGSJSONShape, dimensions);
   /*0H*/TMap<C(String) _ARG int> v = prop;
   IPTR(self->impl, DGGSJSONShape)->dimensions = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline DGGSJSONShape::dimensions_Prop::operator /*0I*/TMap<C(String) _ARG int> () const
{
   SELF(DGGSJSONShape, dimensions);
   TMap<C(String) _ARG int> value(IPTR(self->impl, DGGSJSONShape)->dimensions, DGGSJSONShape::_cpp_class);
   return value;
}



// member accessors: FieldTypeEx::type
// (bits::enum|int)

/*nstSet*/inline /*0H*/FieldType FieldTypeEx::type_Prop::operator= (/*0H*/FieldType v)
{
   SELF(FieldTypeEx, type);
   FIELDTYPEEX_SET_type(self->impl, v);
   return v;
}

/*regSet*/inline FieldTypeEx::type_Prop & FieldTypeEx::type_Prop::operator= (FieldTypeEx::type_Prop & prop)
{
   SELF(FieldTypeEx, type);
   /*0H*/FieldType v = prop;
   FIELDTYPEEX_SET_type(self->impl, v);
   return prop;
}
/*regGet*/inline FieldTypeEx::type_Prop::operator /*0I*/FieldType () const
{
   SELF(FieldTypeEx, type);
   return (FieldType)FIELDTYPEEX_type(self->impl);
}

// member accessors: FieldTypeEx::mustFree
// (bits::enum|uint)

/*nstSet*/inline /*0H*/bool FieldTypeEx::mustFree_Prop::operator= (/*0H*/bool v)
{
   SELF(FieldTypeEx, mustFree);
   FIELDTYPEEX_SET_mustFree(self->impl, v);
   return v;
}

/*regSet*/inline FieldTypeEx::mustFree_Prop & FieldTypeEx::mustFree_Prop::operator= (FieldTypeEx::mustFree_Prop & prop)
{
   SELF(FieldTypeEx, mustFree);
   /*0H*/bool v = prop;
   FIELDTYPEEX_SET_mustFree(self->impl, v);
   return prop;
}
/*regGet*/inline FieldTypeEx::mustFree_Prop::operator /*0I*/bool () const
{
   SELF(FieldTypeEx, mustFree);
   return (bool)FIELDTYPEEX_mustFree(self->impl);
}

// member accessors: FieldTypeEx::format
// (bits::enum|int)

/*nstSet*/inline /*0H*/FieldValueFormat FieldTypeEx::format_Prop::operator= (/*0H*/FieldValueFormat v)
{
   SELF(FieldTypeEx, format);
   FIELDTYPEEX_SET_format(self->impl, v);
   return v;
}

/*regSet*/inline FieldTypeEx::format_Prop & FieldTypeEx::format_Prop::operator= (FieldTypeEx::format_Prop & prop)
{
   SELF(FieldTypeEx, format);
   /*0H*/FieldValueFormat v = prop;
   FIELDTYPEEX_SET_format(self->impl, v);
   return prop;
}
/*regGet*/inline FieldTypeEx::format_Prop::operator /*0I*/FieldValueFormat () const
{
   SELF(FieldTypeEx, format);
   return (FieldValueFormat)FIELDTYPEEX_format(self->impl);
}

// member accessors: FieldTypeEx::isUnsigned
// (bits::enum|uint)

/*nstSet*/inline /*0H*/bool FieldTypeEx::isUnsigned_Prop::operator= (/*0H*/bool v)
{
   SELF(FieldTypeEx, isUnsigned);
   FIELDTYPEEX_SET_isUnsigned(self->impl, v);
   return v;
}

/*regSet*/inline FieldTypeEx::isUnsigned_Prop & FieldTypeEx::isUnsigned_Prop::operator= (FieldTypeEx::isUnsigned_Prop & prop)
{
   SELF(FieldTypeEx, isUnsigned);
   /*0H*/bool v = prop;
   FIELDTYPEEX_SET_isUnsigned(self->impl, v);
   return prop;
}
/*regGet*/inline FieldTypeEx::isUnsigned_Prop::operator /*0I*/bool () const
{
   SELF(FieldTypeEx, isUnsigned);
   return (bool)FIELDTYPEEX_isUnsigned(self->impl);
}

// member accessors: FieldTypeEx::isDateTime
// (bits::enum|uint)

/*nstSet*/inline /*0H*/bool FieldTypeEx::isDateTime_Prop::operator= (/*0H*/bool v)
{
   SELF(FieldTypeEx, isDateTime);
   FIELDTYPEEX_SET_isDateTime(self->impl, v);
   return v;
}

/*regSet*/inline FieldTypeEx::isDateTime_Prop & FieldTypeEx::isDateTime_Prop::operator= (FieldTypeEx::isDateTime_Prop & prop)
{
   SELF(FieldTypeEx, isDateTime);
   /*0H*/bool v = prop;
   FIELDTYPEEX_SET_isDateTime(self->impl, v);
   return prop;
}
/*regGet*/inline FieldTypeEx::isDateTime_Prop::operator /*0I*/bool () const
{
   SELF(FieldTypeEx, isDateTime);
   return (bool)FIELDTYPEEX_isDateTime(self->impl);
}

inline FieldValue::FieldValue(FieldTypeEx type, int64 i = 0, double r = 0, char * s = 0, void * b = null)
{
   impl.type = type;
   impl.i = i;
   impl.r = r;
   impl.s = s;
   impl.b = b;
}


inline int FieldValue::compareInt(/*1Ab*/FieldValue & other)
{
   return FieldValue_compareInt(&impl, /*5De*/&other.impl);
}
inline int FieldValue::compareInt(/*1Ac*/FieldValue * other)
{
   return FieldValue_compareInt(&impl, /*5Cf*/(C(FieldValue) *)other);
}
inline int FieldValue::compareReal(/*1Ab*/FieldValue & other)
{
   return FieldValue_compareReal(&impl, /*5De*/&other.impl);
}
inline int FieldValue::compareReal(/*1Ac*/FieldValue * other)
{
   return FieldValue_compareReal(&impl, /*5Cf*/(C(FieldValue) *)other);
}
inline int FieldValue::compareText(/*1Ab*/FieldValue & other)
{
   return FieldValue_compareText(&impl, /*5De*/&other.impl);
}
inline int FieldValue::compareText(/*1Ac*/FieldValue * other)
{
   return FieldValue_compareText(&impl, /*5Cf*/(C(FieldValue) *)other);
}
inline C(String) FieldValue::formatArray(/*1Ab*/char * tempString, /*1Ab*/void * fieldData, /*1Ab*/ObjectNotationType * onType)
{
   return FieldValue_formatArray(&impl, /*5Ge*/tempString, /*5Ge*/fieldData, /*5Ee*/(C(ObjectNotationType) *)onType);
}
inline C(String) FieldValue::formatFloat(/*1Ab*/char * stringOutput, /*1Ab*/bool fixDot)
{
   return FieldValue_formatFloat(&impl, /*5Ge*/stringOutput, /*5Ee*/(C(bool))fixDot);
}
inline C(String) FieldValue::formatInteger(/*1Ab*/char * stringOutput)
{
   return FieldValue_formatInteger(&impl, /*5Ge*/stringOutput);
}
inline C(String) FieldValue::formatMap(/*1Ab*/char * tempString, /*1Ab*/void * fieldData, /*1Ab*/ObjectNotationType * onType)
{
   return FieldValue_formatMap(&impl, /*5Ge*/tempString, /*5Ge*/fieldData, /*5Ee*/(C(ObjectNotationType) *)onType);
}
inline C(bool) FieldValue::getArrayOrMap(/*1Ab*/const char * string, /*1Ab*/Class destClass, /*1Ab*/void ** destination)
{
   return FieldValue_getArrayOrMap(/*5Ge*/string, /*5De*/destClass.impl, /*5Ge*/destination);
}
inline C(String) FieldValue::stringify()
{
   return FieldValue_stringify(&impl);
}


// member accessors: FieldValue::type
// (struct::bits|int)

/*nstSet*/inline /*0H*/FieldTypeEx FieldValue::type_Prop::operator= (/*0H*/FieldTypeEx v)
{
   SELF(FieldValue, type);
   self->impl.type = v;
   return v;
}

/*regSet*/inline FieldValue::type_Prop & FieldValue::type_Prop::operator= (FieldValue::type_Prop & prop)
{
   SELF(FieldValue, type);
   /*0H*/FieldTypeEx v = prop;
   self->impl.type = v;
   return prop;
}
/*regGet*/inline FieldValue::type_Prop::operator /*0I*/FieldTypeEx () const
{
   SELF(FieldValue, type);
   return self ? FieldTypeEx(((C(FieldValue) *)&self->impl)->type) : FieldTypeEx();
}

// member accessors: FieldValue::i
// (struct::int64)

/*nstSet*/inline /*0H*/int64 FieldValue::i_Prop::operator= (/*0H*/int64 v)
{
   SELF(FieldValue, i);
   self->impl.i = v;
   return v;
}

/*regSet*/inline FieldValue::i_Prop & FieldValue::i_Prop::operator= (FieldValue::i_Prop & prop)
{
   SELF(FieldValue, i);
   /*0H*/int64 v = prop;
   self->impl.i = v;
   return prop;
}
/*regGet*/inline FieldValue::i_Prop::operator /*0I*/int64 () const
{
   SELF(FieldValue, i);
   return self ? ((C(FieldValue) *)&self->impl)->i : 0;
}

// member accessors: FieldValue::r
// (struct::double)

/*nstSet*/inline /*0H*/double FieldValue::r_Prop::operator= (/*0H*/double v)
{
   SELF(FieldValue, r);
   self->impl.r = v;
   return v;
}

/*regSet*/inline FieldValue::r_Prop & FieldValue::r_Prop::operator= (FieldValue::r_Prop & prop)
{
   SELF(FieldValue, r);
   /*0H*/double v = prop;
   self->impl.r = v;
   return prop;
}
/*regGet*/inline FieldValue::r_Prop::operator /*0I*/double () const
{
   SELF(FieldValue, r);
   return self ? ((C(FieldValue) *)&self->impl)->r : 0;
}

// member accessors: FieldValue::s
// (struct::string)

/*nstSet*/inline /*0H*/char * FieldValue::s_Prop::operator= (/*0H*/char * v)
{
   SELF(FieldValue, s);
   self->impl.s = v;
   return v;
}

/*regSet*/inline FieldValue::s_Prop & FieldValue::s_Prop::operator= (FieldValue::s_Prop & prop)
{
   SELF(FieldValue, s);
   /*0H*/char * v = prop;
   self->impl.s = v;
   return prop;
}
/*regGet*/inline FieldValue::s_Prop::operator /*0I*/char * () const
{
   SELF(FieldValue, s);
   return self ? ((C(FieldValue) *)&self->impl)->s : 0;
}

// member accessors: FieldValue::b
// (struct::void *)

/*nstSet*/inline /*0H*/void * FieldValue::b_Prop::operator= (/*0H*/void * v)
{
   SELF(FieldValue, b);
   self->impl.b = v;
   return v;
}

/*regSet*/inline FieldValue::b_Prop & FieldValue::b_Prop::operator= (FieldValue::b_Prop & prop)
{
   SELF(FieldValue, b);
   /*0H*/void * v = prop;
   self->impl.b = v;
   return prop;
}
/*regGet*/inline FieldValue::b_Prop::operator /*0I*/void * () const
{
   SELF(FieldValue, b);
   return self ? ((C(FieldValue) *)&self->impl)->b : null;
}

// member accessors: FieldValue::a
// (struct::normal)

/*nstSet*/inline /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> FieldValue::a_Prop::operator= (/*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v)
{
   SELF(FieldValue, a);
   self->impl.a = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline FieldValue::a_Prop & FieldValue::a_Prop::operator= (FieldValue::a_Prop & prop)
{
   SELF(FieldValue, a);
   /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v = prop;
   self->impl.a = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline FieldValue::a_Prop::operator /*0I*/TArray<FieldValue _ARG int _ARG FieldValue> () const
{
   SELF(FieldValue, a);
   TArray<FieldValue _ARG int _ARG FieldValue> value(((C(FieldValue) *)&self->impl)->a);
   return value;
}

// member accessors: FieldValue::m
// (struct::normal)

/*nstSet*/inline /*0H*/TMap<C(String) _ARG FieldValue> FieldValue::m_Prop::operator= (/*0H*/TMap<C(String) _ARG FieldValue> v)
{
   SELF(FieldValue, m);
   self->impl.m = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline FieldValue::m_Prop & FieldValue::m_Prop::operator= (FieldValue::m_Prop & prop)
{
   SELF(FieldValue, m);
   /*0H*/TMap<C(String) _ARG FieldValue> v = prop;
   self->impl.m = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline FieldValue::m_Prop::operator /*0I*/TMap<C(String) _ARG FieldValue> () const
{
   SELF(FieldValue, m);
   TMap<C(String) _ARG FieldValue> value(((C(FieldValue) *)&self->impl)->m);
   return value;
}



// member accessors: GGGZone::level
// (bits::uint)

/*nstSet*/inline /*0H*/uint GGGZone::level_Prop::operator= (/*0H*/uint v)
{
   SELF(GGGZone, level);
   GGGZONE_SET_level(self->impl, v);
   return v;
}

/*regSet*/inline GGGZone::level_Prop & GGGZone::level_Prop::operator= (GGGZone::level_Prop & prop)
{
   SELF(GGGZone, level);
   /*0H*/uint v = prop;
   GGGZONE_SET_level(self->impl, v);
   return prop;
}
/*regGet*/inline GGGZone::level_Prop::operator /*0I*/uint () const
{
   SELF(GGGZone, level);
   return GGGZONE_level(self->impl);
}

// member accessors: GGGZone::row
// (bits::uint)

/*nstSet*/inline /*0H*/uint GGGZone::row_Prop::operator= (/*0H*/uint v)
{
   SELF(GGGZone, row);
   GGGZONE_SET_row(self->impl, v);
   return v;
}

/*regSet*/inline GGGZone::row_Prop & GGGZone::row_Prop::operator= (GGGZone::row_Prop & prop)
{
   SELF(GGGZone, row);
   /*0H*/uint v = prop;
   GGGZONE_SET_row(self->impl, v);
   return prop;
}
/*regGet*/inline GGGZone::row_Prop::operator /*0I*/uint () const
{
   SELF(GGGZone, row);
   return GGGZONE_row(self->impl);
}

// member accessors: GGGZone::col
// (bits::uint)

/*nstSet*/inline /*0H*/uint GGGZone::col_Prop::operator= (/*0H*/uint v)
{
   SELF(GGGZone, col);
   GGGZONE_SET_col(self->impl, v);
   return v;
}

/*regSet*/inline GGGZone::col_Prop & GGGZone::col_Prop::operator= (GGGZone::col_Prop & prop)
{
   SELF(GGGZone, col);
   /*0H*/uint v = prop;
   GGGZONE_SET_col(self->impl, v);
   return prop;
}
/*regGet*/inline GGGZone::col_Prop::operator /*0I*/uint () const
{
   SELF(GGGZone, col);
   return GGGZONE_col(self->impl);
}


inline GeoExtent::GeoExtent(GeoPoint ll, GeoPoint ur = GeoPoint())
{
   impl.ll = ll.impl;
   impl.ur = ur.impl;
}


inline void GeoExtent::clear()
{
   GeoExtent_clear(&impl);
}
inline C(bool) GeoExtent::intersects(/*1Ab*/const GeoExtent & b)
{
   return GeoExtent_intersects(&impl, /*5De*/&b.impl);
}
inline C(bool) GeoExtent::intersects(/*1Ac*/const GeoExtent * b)
{
   return GeoExtent_intersects(&impl, /*5Cf*/(C(GeoExtent) *)b);
}


// property callers: GeoExtent::geodeticArea   // get only
// (struct::double)

/*regGet*/inline GeoExtent::geodeticArea_Prop::operator /*0I*/double () const
{
   SELF(GeoExtent, geodeticArea);
   return GeoExtent_get_geodeticArea(self ? &self->impl : null);
}


// member accessors: GeoExtent::ll
// (struct::struct)

/*nstSet*/inline /*0H*/GeoPoint GeoExtent::ll_Prop::operator= (/*0H*/GeoPoint v)
{
   SELF(GeoExtent, ll);
   self->impl.ll = v.impl;
   return v;
}

/*regSet*/inline GeoExtent::ll_Prop & GeoExtent::ll_Prop::operator= (GeoExtent::ll_Prop & prop)
{
   SELF(GeoExtent, ll);
   /*0H*/GeoPoint v = prop;
   self->impl.ll = v.impl;
   return prop;
}
/*regGet*/inline GeoExtent::ll_Prop::operator /*0I*/GeoPoint () const
{
   SELF(GeoExtent, ll);
   GeoPoint value(((C(GeoExtent) *)&self->impl)->ll);
   return value;
}

// member accessors: GeoExtent::ur
// (struct::struct)

/*nstSet*/inline /*0H*/GeoPoint GeoExtent::ur_Prop::operator= (/*0H*/GeoPoint v)
{
   SELF(GeoExtent, ur);
   self->impl.ur = v.impl;
   return v;
}

/*regSet*/inline GeoExtent::ur_Prop & GeoExtent::ur_Prop::operator= (GeoExtent::ur_Prop & prop)
{
   SELF(GeoExtent, ur);
   /*0H*/GeoPoint v = prop;
   self->impl.ur = v.impl;
   return prop;
}
/*regGet*/inline GeoExtent::ur_Prop::operator /*0I*/GeoPoint () const
{
   SELF(GeoExtent, ur);
   GeoPoint value(((C(GeoExtent) *)&self->impl)->ur);
   return value;
}

inline GeoPoint::GeoPoint(Degrees lat, Degrees lon = 0)
{
   impl.lat = lat.impl;
   impl.lon = lon.impl;
}



// member accessors: GeoPoint::lat
// (struct::unit|double)

/*nstSet*/inline /*0H*/Degrees GeoPoint::lat_Prop::operator= (/*0H*/Degrees v)
{
   SELF(GeoPoint, lat);
   self->impl.lat = (double)v.impl;
   return v;
}

/*regSet*/inline GeoPoint::lat_Prop & GeoPoint::lat_Prop::operator= (GeoPoint::lat_Prop & prop)
{
   SELF(GeoPoint, lat);
   /*0H*/Degrees v = prop;
   self->impl.lat = (double)v.impl;
   return prop;
}
/*regGet*/inline GeoPoint::lat_Prop::operator /*0I*/Degrees () const
{
   SELF(GeoPoint, lat);
   return Degrees(Angle(self->impl.lat));
}
/*regGet*/inline GeoPoint::lat_Prop::operator /*0J*/C(Angle) () const
{
   SELF(GeoPoint, lat);
   return Degrees(self->lat);
}

// member accessors: GeoPoint::lon
// (struct::unit|double)

/*nstSet*/inline /*0H*/Degrees GeoPoint::lon_Prop::operator= (/*0H*/Degrees v)
{
   SELF(GeoPoint, lon);
   self->impl.lon = (double)v.impl;
   return v;
}

/*regSet*/inline GeoPoint::lon_Prop & GeoPoint::lon_Prop::operator= (GeoPoint::lon_Prop & prop)
{
   SELF(GeoPoint, lon);
   /*0H*/Degrees v = prop;
   self->impl.lon = (double)v.impl;
   return prop;
}
/*regGet*/inline GeoPoint::lon_Prop::operator /*0I*/Degrees () const
{
   SELF(GeoPoint, lon);
   return Degrees(Angle(self->impl.lon));
}
/*regGet*/inline GeoPoint::lon_Prop::operator /*0J*/C(Angle) () const
{
   SELF(GeoPoint, lon);
   return Degrees(self->lon);
}




// member accessors: ISEA3HZone::levelISEA9R
// (bits::uint)

/*nstSet*/inline /*0H*/uint ISEA3HZone::levelISEA9R_Prop::operator= (/*0H*/uint v)
{
   SELF(ISEA3HZone, levelISEA9R);
   ISEA3HZONE_SET_levelISEA9R(self->impl, v);
   return v;
}

/*regSet*/inline ISEA3HZone::levelISEA9R_Prop & ISEA3HZone::levelISEA9R_Prop::operator= (ISEA3HZone::levelISEA9R_Prop & prop)
{
   SELF(ISEA3HZone, levelISEA9R);
   /*0H*/uint v = prop;
   ISEA3HZONE_SET_levelISEA9R(self->impl, v);
   return prop;
}
/*regGet*/inline ISEA3HZone::levelISEA9R_Prop::operator /*0I*/uint () const
{
   SELF(ISEA3HZone, levelISEA9R);
   return ISEA3HZONE_levelISEA9R(self->impl);
}

// member accessors: ISEA3HZone::rootRhombus
// (bits::uint)

/*nstSet*/inline /*0H*/uint ISEA3HZone::rootRhombus_Prop::operator= (/*0H*/uint v)
{
   SELF(ISEA3HZone, rootRhombus);
   ISEA3HZONE_SET_rootRhombus(self->impl, v);
   return v;
}

/*regSet*/inline ISEA3HZone::rootRhombus_Prop & ISEA3HZone::rootRhombus_Prop::operator= (ISEA3HZone::rootRhombus_Prop & prop)
{
   SELF(ISEA3HZone, rootRhombus);
   /*0H*/uint v = prop;
   ISEA3HZONE_SET_rootRhombus(self->impl, v);
   return prop;
}
/*regGet*/inline ISEA3HZone::rootRhombus_Prop::operator /*0I*/uint () const
{
   SELF(ISEA3HZone, rootRhombus);
   return ISEA3HZONE_rootRhombus(self->impl);
}

// member accessors: ISEA3HZone::rhombusIX
// (bits::uint64)

/*nstSet*/inline /*0H*/uint64 ISEA3HZone::rhombusIX_Prop::operator= (/*0H*/uint64 v)
{
   SELF(ISEA3HZone, rhombusIX);
   ISEA3HZONE_SET_rhombusIX(self->impl, v);
   return v;
}

/*regSet*/inline ISEA3HZone::rhombusIX_Prop & ISEA3HZone::rhombusIX_Prop::operator= (ISEA3HZone::rhombusIX_Prop & prop)
{
   SELF(ISEA3HZone, rhombusIX);
   /*0H*/uint64 v = prop;
   ISEA3HZONE_SET_rhombusIX(self->impl, v);
   return prop;
}
/*regGet*/inline ISEA3HZone::rhombusIX_Prop::operator /*0I*/uint64 () const
{
   SELF(ISEA3HZone, rhombusIX);
   return ISEA3HZONE_rhombusIX(self->impl);
}

// member accessors: ISEA3HZone::subHex
// (bits::uint)

/*nstSet*/inline /*0H*/uint ISEA3HZone::subHex_Prop::operator= (/*0H*/uint v)
{
   SELF(ISEA3HZone, subHex);
   ISEA3HZONE_SET_subHex(self->impl, v);
   return v;
}

/*regSet*/inline ISEA3HZone::subHex_Prop & ISEA3HZone::subHex_Prop::operator= (ISEA3HZone::subHex_Prop & prop)
{
   SELF(ISEA3HZone, subHex);
   /*0H*/uint v = prop;
   ISEA3HZONE_SET_subHex(self->impl, v);
   return prop;
}
/*regGet*/inline ISEA3HZone::subHex_Prop::operator /*0I*/uint () const
{
   SELF(ISEA3HZone, subHex);
   return ISEA3HZONE_subHex(self->impl);
}




// member accessors: ISEA9RZone::level
// (bits::uint)

/*nstSet*/inline /*0H*/uint ISEA9RZone::level_Prop::operator= (/*0H*/uint v)
{
   SELF(ISEA9RZone, level);
   ISEA9RZONE_SET_level(self->impl, v);
   return v;
}

/*regSet*/inline ISEA9RZone::level_Prop & ISEA9RZone::level_Prop::operator= (ISEA9RZone::level_Prop & prop)
{
   SELF(ISEA9RZone, level);
   /*0H*/uint v = prop;
   ISEA9RZONE_SET_level(self->impl, v);
   return prop;
}
/*regGet*/inline ISEA9RZone::level_Prop::operator /*0I*/uint () const
{
   SELF(ISEA9RZone, level);
   return ISEA9RZONE_level(self->impl);
}

// member accessors: ISEA9RZone::row
// (bits::uint)

/*nstSet*/inline /*0H*/uint ISEA9RZone::row_Prop::operator= (/*0H*/uint v)
{
   SELF(ISEA9RZone, row);
   ISEA9RZONE_SET_row(self->impl, v);
   return v;
}

/*regSet*/inline ISEA9RZone::row_Prop & ISEA9RZone::row_Prop::operator= (ISEA9RZone::row_Prop & prop)
{
   SELF(ISEA9RZone, row);
   /*0H*/uint v = prop;
   ISEA9RZONE_SET_row(self->impl, v);
   return prop;
}
/*regGet*/inline ISEA9RZone::row_Prop::operator /*0I*/uint () const
{
   SELF(ISEA9RZone, row);
   return ISEA9RZONE_row(self->impl);
}

// member accessors: ISEA9RZone::col
// (bits::uint)

/*nstSet*/inline /*0H*/uint ISEA9RZone::col_Prop::operator= (/*0H*/uint v)
{
   SELF(ISEA9RZone, col);
   ISEA9RZONE_SET_col(self->impl, v);
   return v;
}

/*regSet*/inline ISEA9RZone::col_Prop & ISEA9RZone::col_Prop::operator= (ISEA9RZone::col_Prop & prop)
{
   SELF(ISEA9RZone, col);
   /*0H*/uint v = prop;
   ISEA9RZONE_SET_col(self->impl, v);
   return prop;
}
/*regGet*/inline ISEA9RZone::col_Prop::operator /*0I*/uint () const
{
   SELF(ISEA9RZone, col);
   return ISEA9RZONE_col(self->impl);
}

inline JSONSchema::JSONSchema(char * schema, char * id = 0, char * title = 0, char * comment = 0, char * description = 0, FieldValue Default = FieldValue(), bool readOnly = (bool)0, bool writeOnly = (bool)0) : JSONSchema()
{
   this->schema = schema;
   this->id = id;
   this->title = title;
   this->comment = comment;
   this->description = description;
   // FIXME: this->Default = Default.impl;
   this->readOnly = readOnly;
   this->writeOnly = writeOnly;
}



// property callers: JSONSchema::maximum   // get only
// (normal::double)

/*regGet*/inline JSONSchema::maximum_Prop::operator /*0I*/double () const
{
   SELF(JSONSchema, maximum);
   return self ? IPTR(self->impl, JSONSchema)->maximum : 0;
}

// property callers: JSONSchema::exclusiveMaximum   // get only
// (normal::double)

/*regGet*/inline JSONSchema::exclusiveMaximum_Prop::operator /*0I*/double () const
{
   SELF(JSONSchema, exclusiveMaximum);
   return self ? IPTR(self->impl, JSONSchema)->exclusiveMaximum : 0;
}

// property callers: JSONSchema::minimum   // get only
// (normal::double)

/*regGet*/inline JSONSchema::minimum_Prop::operator /*0I*/double () const
{
   SELF(JSONSchema, minimum);
   return self ? IPTR(self->impl, JSONSchema)->minimum : 0;
}

// property callers: JSONSchema::exclusiveMinimum   // get only
// (normal::double)

/*regGet*/inline JSONSchema::exclusiveMinimum_Prop::operator /*0I*/double () const
{
   SELF(JSONSchema, exclusiveMinimum);
   return self ? IPTR(self->impl, JSONSchema)->exclusiveMinimum : 0;
}

// property callers: JSONSchema::maxItems   // get only
// (normal::int)

/*regGet*/inline JSONSchema::maxItems_Prop::operator /*0I*/int () const
{
   SELF(JSONSchema, maxItems);
   return self ? IPTR(self->impl, JSONSchema)->maxItems : 0;
}

// property callers: JSONSchema::minItems   // get only
// (normal::int)

/*regGet*/inline JSONSchema::minItems_Prop::operator /*0I*/int () const
{
   SELF(JSONSchema, minItems);
   return self ? IPTR(self->impl, JSONSchema)->minItems : 0;
}

// property callers: JSONSchema::maxProperties   // get only
// (normal::int)

/*regGet*/inline JSONSchema::maxProperties_Prop::operator /*0I*/int () const
{
   SELF(JSONSchema, maxProperties);
   return self ? IPTR(self->impl, JSONSchema)->maxProperties : 0;
}

// property callers: JSONSchema::minProperties   // get only
// (normal::int)

/*regGet*/inline JSONSchema::minProperties_Prop::operator /*0I*/int () const
{
   SELF(JSONSchema, minProperties);
   return self ? IPTR(self->impl, JSONSchema)->minProperties : 0;
}

// property callers: JSONSchema::xogcpropertySeq   // set only
// (normal::int)

/*regGet*/inline JSONSchema::xogcpropertySeq_Prop::operator /*0I*/int () const
{
   SELF(JSONSchema, xogcpropertySeq);
   return self ? IPTR(self->impl, JSONSchema)->xogcpropertySeq : 0;
}

// property callers: JSONSchema::Default   // set only
// (normal::struct)

/*regGet*/inline JSONSchema::Default_Prop::operator /*0I*/FieldValue () const
{
   SELF(JSONSchema, Default);
   FieldValue value(IPTR(self->impl, JSONSchema)->Default);
   return value;
}


// member accessors: JSONSchema::schema
// (normal::string)

/*nstSet*/inline /*0H*/char * JSONSchema::schema_Prop::operator= (/*0H*/char * v)
{
   SELF(JSONSchema, schema);
   IPTR(self->impl, JSONSchema)->schema = v;
   return v;
}

/*regSet*/inline JSONSchema::schema_Prop & JSONSchema::schema_Prop::operator= (JSONSchema::schema_Prop & prop)
{
   SELF(JSONSchema, schema);
   /*0H*/char * v = prop;
   IPTR(self->impl, JSONSchema)->schema = v;
   return prop;
}
/*regGet*/inline JSONSchema::schema_Prop::operator /*0I*/char * () const
{
   SELF(JSONSchema, schema);
   return self ? IPTR(self->impl, JSONSchema)->schema : 0;
}

// member accessors: JSONSchema::id
// (normal::string)

/*nstSet*/inline /*0H*/char * JSONSchema::id_Prop::operator= (/*0H*/char * v)
{
   SELF(JSONSchema, id);
   IPTR(self->impl, JSONSchema)->id = v;
   return v;
}

/*regSet*/inline JSONSchema::id_Prop & JSONSchema::id_Prop::operator= (JSONSchema::id_Prop & prop)
{
   SELF(JSONSchema, id);
   /*0H*/char * v = prop;
   IPTR(self->impl, JSONSchema)->id = v;
   return prop;
}
/*regGet*/inline JSONSchema::id_Prop::operator /*0I*/char * () const
{
   SELF(JSONSchema, id);
   return self ? IPTR(self->impl, JSONSchema)->id : 0;
}

// member accessors: JSONSchema::title
// (normal::string)

/*nstSet*/inline /*0H*/char * JSONSchema::title_Prop::operator= (/*0H*/char * v)
{
   SELF(JSONSchema, title);
   IPTR(self->impl, JSONSchema)->title = v;
   return v;
}

/*regSet*/inline JSONSchema::title_Prop & JSONSchema::title_Prop::operator= (JSONSchema::title_Prop & prop)
{
   SELF(JSONSchema, title);
   /*0H*/char * v = prop;
   IPTR(self->impl, JSONSchema)->title = v;
   return prop;
}
/*regGet*/inline JSONSchema::title_Prop::operator /*0I*/char * () const
{
   SELF(JSONSchema, title);
   return self ? IPTR(self->impl, JSONSchema)->title : 0;
}

// member accessors: JSONSchema::comment
// (normal::string)

/*nstSet*/inline /*0H*/char * JSONSchema::comment_Prop::operator= (/*0H*/char * v)
{
   SELF(JSONSchema, comment);
   IPTR(self->impl, JSONSchema)->comment = v;
   return v;
}

/*regSet*/inline JSONSchema::comment_Prop & JSONSchema::comment_Prop::operator= (JSONSchema::comment_Prop & prop)
{
   SELF(JSONSchema, comment);
   /*0H*/char * v = prop;
   IPTR(self->impl, JSONSchema)->comment = v;
   return prop;
}
/*regGet*/inline JSONSchema::comment_Prop::operator /*0I*/char * () const
{
   SELF(JSONSchema, comment);
   return self ? IPTR(self->impl, JSONSchema)->comment : 0;
}

// member accessors: JSONSchema::description
// (normal::string)

/*nstSet*/inline /*0H*/char * JSONSchema::description_Prop::operator= (/*0H*/char * v)
{
   SELF(JSONSchema, description);
   IPTR(self->impl, JSONSchema)->description = v;
   return v;
}

/*regSet*/inline JSONSchema::description_Prop & JSONSchema::description_Prop::operator= (JSONSchema::description_Prop & prop)
{
   SELF(JSONSchema, description);
   /*0H*/char * v = prop;
   IPTR(self->impl, JSONSchema)->description = v;
   return prop;
}
/*regGet*/inline JSONSchema::description_Prop::operator /*0I*/char * () const
{
   SELF(JSONSchema, description);
   return self ? IPTR(self->impl, JSONSchema)->description : 0;
}

// member accessors: JSONSchema::readOnly
// (normal::enum|uint)

/*nstSet*/inline /*0H*/bool JSONSchema::readOnly_Prop::operator= (/*0H*/bool v)
{
   SELF(JSONSchema, readOnly);
   IPTR(self->impl, JSONSchema)->readOnly = (C(bool))v;
   return v;
}

/*regSet*/inline JSONSchema::readOnly_Prop & JSONSchema::readOnly_Prop::operator= (JSONSchema::readOnly_Prop & prop)
{
   SELF(JSONSchema, readOnly);
   /*0H*/bool v = prop;
   IPTR(self->impl, JSONSchema)->readOnly = (C(bool))v;
   return prop;
}
/*regGet*/inline JSONSchema::readOnly_Prop::operator /*0I*/bool () const
{
   SELF(JSONSchema, readOnly);
   return self ? (bool)IPTR(self->impl, JSONSchema)->readOnly : (bool)0;
}

// member accessors: JSONSchema::writeOnly
// (normal::enum|uint)

/*nstSet*/inline /*0H*/bool JSONSchema::writeOnly_Prop::operator= (/*0H*/bool v)
{
   SELF(JSONSchema, writeOnly);
   IPTR(self->impl, JSONSchema)->writeOnly = (C(bool))v;
   return v;
}

/*regSet*/inline JSONSchema::writeOnly_Prop & JSONSchema::writeOnly_Prop::operator= (JSONSchema::writeOnly_Prop & prop)
{
   SELF(JSONSchema, writeOnly);
   /*0H*/bool v = prop;
   IPTR(self->impl, JSONSchema)->writeOnly = (C(bool))v;
   return prop;
}
/*regGet*/inline JSONSchema::writeOnly_Prop::operator /*0I*/bool () const
{
   SELF(JSONSchema, writeOnly);
   return self ? (bool)IPTR(self->impl, JSONSchema)->writeOnly : (bool)0;
}

// member accessors: JSONSchema::examples
// (normal::normal)

/*nstSet*/inline /*0H*/TArray<C(String) _ARG int _ARG C(String)> JSONSchema::examples_Prop::operator= (/*0H*/TArray<C(String) _ARG int _ARG C(String)> v)
{
   SELF(JSONSchema, examples);
   IPTR(self->impl, JSONSchema)->examples = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline JSONSchema::examples_Prop & JSONSchema::examples_Prop::operator= (JSONSchema::examples_Prop & prop)
{
   SELF(JSONSchema, examples);
   /*0H*/TArray<C(String) _ARG int _ARG C(String)> v = prop;
   IPTR(self->impl, JSONSchema)->examples = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline JSONSchema::examples_Prop::operator /*0I*/TArray<C(String) _ARG int _ARG C(String)> () const
{
   SELF(JSONSchema, examples);
   TArray<C(String) _ARG int _ARG C(String)> value(IPTR(self->impl, JSONSchema)->examples, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::multipleOf
// (normal::normal)

/*nstSet*/inline /*0H*/TArray<double _ARG int _ARG double> JSONSchema::multipleOf_Prop::operator= (/*0H*/TArray<double _ARG int _ARG double> v)
{
   SELF(JSONSchema, multipleOf);
   IPTR(self->impl, JSONSchema)->multipleOf = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline JSONSchema::multipleOf_Prop & JSONSchema::multipleOf_Prop::operator= (JSONSchema::multipleOf_Prop & prop)
{
   SELF(JSONSchema, multipleOf);
   /*0H*/TArray<double _ARG int _ARG double> v = prop;
   IPTR(self->impl, JSONSchema)->multipleOf = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline JSONSchema::multipleOf_Prop::operator /*0I*/TArray<double _ARG int _ARG double> () const
{
   SELF(JSONSchema, multipleOf);
   TArray<double _ARG int _ARG double> value(IPTR(self->impl, JSONSchema)->multipleOf, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::type
// (normal::enum|int)

/*nstSet*/inline /*0H*/JSONSchemaType JSONSchema::type_Prop::operator= (/*0H*/JSONSchemaType v)
{
   SELF(JSONSchema, type);
   IPTR(self->impl, JSONSchema)->type = (C(JSONSchemaType))v;
   return v;
}

/*regSet*/inline JSONSchema::type_Prop & JSONSchema::type_Prop::operator= (JSONSchema::type_Prop & prop)
{
   SELF(JSONSchema, type);
   /*0H*/JSONSchemaType v = prop;
   IPTR(self->impl, JSONSchema)->type = (C(JSONSchemaType))v;
   return prop;
}
/*regGet*/inline JSONSchema::type_Prop::operator /*0I*/JSONSchemaType () const
{
   SELF(JSONSchema, type);
   return self ? (JSONSchemaType)IPTR(self->impl, JSONSchema)->type : (JSONSchemaType)0;
}

// member accessors: JSONSchema::Enum
// (normal::normal)

/*nstSet*/inline /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> JSONSchema::Enum_Prop::operator= (/*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v)
{
   SELF(JSONSchema, Enum);
   IPTR(self->impl, JSONSchema)->Enum = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline JSONSchema::Enum_Prop & JSONSchema::Enum_Prop::operator= (JSONSchema::Enum_Prop & prop)
{
   SELF(JSONSchema, Enum);
   /*0H*/TArray<FieldValue _ARG int _ARG FieldValue> v = prop;
   IPTR(self->impl, JSONSchema)->Enum = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline JSONSchema::Enum_Prop::operator /*0I*/TArray<FieldValue _ARG int _ARG FieldValue> () const
{
   SELF(JSONSchema, Enum);
   TArray<FieldValue _ARG int _ARG FieldValue> value(IPTR(self->impl, JSONSchema)->Enum, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::format
// (normal::string)

/*nstSet*/inline /*0H*/char * JSONSchema::format_Prop::operator= (/*0H*/char * v)
{
   SELF(JSONSchema, format);
   IPTR(self->impl, JSONSchema)->format = v;
   return v;
}

/*regSet*/inline JSONSchema::format_Prop & JSONSchema::format_Prop::operator= (JSONSchema::format_Prop & prop)
{
   SELF(JSONSchema, format);
   /*0H*/char * v = prop;
   IPTR(self->impl, JSONSchema)->format = v;
   return prop;
}
/*regGet*/inline JSONSchema::format_Prop::operator /*0I*/char * () const
{
   SELF(JSONSchema, format);
   return self ? IPTR(self->impl, JSONSchema)->format : 0;
}

// member accessors: JSONSchema::contentMediaType
// (normal::string)

/*nstSet*/inline /*0H*/char * JSONSchema::contentMediaType_Prop::operator= (/*0H*/char * v)
{
   SELF(JSONSchema, contentMediaType);
   IPTR(self->impl, JSONSchema)->contentMediaType = v;
   return v;
}

/*regSet*/inline JSONSchema::contentMediaType_Prop & JSONSchema::contentMediaType_Prop::operator= (JSONSchema::contentMediaType_Prop & prop)
{
   SELF(JSONSchema, contentMediaType);
   /*0H*/char * v = prop;
   IPTR(self->impl, JSONSchema)->contentMediaType = v;
   return prop;
}
/*regGet*/inline JSONSchema::contentMediaType_Prop::operator /*0I*/char * () const
{
   SELF(JSONSchema, contentMediaType);
   return self ? IPTR(self->impl, JSONSchema)->contentMediaType : 0;
}

// member accessors: JSONSchema::pattern
// (normal::string)

/*nstSet*/inline /*0H*/char * JSONSchema::pattern_Prop::operator= (/*0H*/char * v)
{
   SELF(JSONSchema, pattern);
   IPTR(self->impl, JSONSchema)->pattern = v;
   return v;
}

/*regSet*/inline JSONSchema::pattern_Prop & JSONSchema::pattern_Prop::operator= (JSONSchema::pattern_Prop & prop)
{
   SELF(JSONSchema, pattern);
   /*0H*/char * v = prop;
   IPTR(self->impl, JSONSchema)->pattern = v;
   return prop;
}
/*regGet*/inline JSONSchema::pattern_Prop::operator /*0I*/char * () const
{
   SELF(JSONSchema, pattern);
   return self ? IPTR(self->impl, JSONSchema)->pattern : 0;
}

// member accessors: JSONSchema::items
// (normal::normal)

/*nstSet*/inline /*0H*/JSONSchema JSONSchema::items_Prop::operator= (/*0H*/JSONSchema v)
{
   SELF(JSONSchema, items);
   IPTR(self->impl, JSONSchema)->items = v.impl;
   return v;
}

/*regSet*/inline JSONSchema::items_Prop & JSONSchema::items_Prop::operator= (JSONSchema::items_Prop & prop)
{
   SELF(JSONSchema, items);
   /*0H*/JSONSchema v = prop;
   IPTR(self->impl, JSONSchema)->items = v.impl;
   return prop;
}
/*regGet*/inline JSONSchema::items_Prop::operator /*0I*/JSONSchema () const
{
   SELF(JSONSchema, items);
   JSONSchema value(IPTR(self->impl, JSONSchema)->items, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::uniqueItems
// (normal::enum|uint)

/*nstSet*/inline /*0H*/bool JSONSchema::uniqueItems_Prop::operator= (/*0H*/bool v)
{
   SELF(JSONSchema, uniqueItems);
   IPTR(self->impl, JSONSchema)->uniqueItems = (C(bool))v;
   return v;
}

/*regSet*/inline JSONSchema::uniqueItems_Prop & JSONSchema::uniqueItems_Prop::operator= (JSONSchema::uniqueItems_Prop & prop)
{
   SELF(JSONSchema, uniqueItems);
   /*0H*/bool v = prop;
   IPTR(self->impl, JSONSchema)->uniqueItems = (C(bool))v;
   return prop;
}
/*regGet*/inline JSONSchema::uniqueItems_Prop::operator /*0I*/bool () const
{
   SELF(JSONSchema, uniqueItems);
   return self ? (bool)IPTR(self->impl, JSONSchema)->uniqueItems : (bool)0;
}

// member accessors: JSONSchema::contains
// (normal::string)

/*nstSet*/inline /*0H*/char * JSONSchema::contains_Prop::operator= (/*0H*/char * v)
{
   SELF(JSONSchema, contains);
   IPTR(self->impl, JSONSchema)->contains = v;
   return v;
}

/*regSet*/inline JSONSchema::contains_Prop & JSONSchema::contains_Prop::operator= (JSONSchema::contains_Prop & prop)
{
   SELF(JSONSchema, contains);
   /*0H*/char * v = prop;
   IPTR(self->impl, JSONSchema)->contains = v;
   return prop;
}
/*regGet*/inline JSONSchema::contains_Prop::operator /*0I*/char * () const
{
   SELF(JSONSchema, contains);
   return self ? IPTR(self->impl, JSONSchema)->contains : 0;
}

// member accessors: JSONSchema::required
// (normal::normal)

/*nstSet*/inline /*0H*/TArray<C(String) _ARG int _ARG C(String)> JSONSchema::required_Prop::operator= (/*0H*/TArray<C(String) _ARG int _ARG C(String)> v)
{
   SELF(JSONSchema, required);
   IPTR(self->impl, JSONSchema)->required = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline JSONSchema::required_Prop & JSONSchema::required_Prop::operator= (JSONSchema::required_Prop & prop)
{
   SELF(JSONSchema, required);
   /*0H*/TArray<C(String) _ARG int _ARG C(String)> v = prop;
   IPTR(self->impl, JSONSchema)->required = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline JSONSchema::required_Prop::operator /*0I*/TArray<C(String) _ARG int _ARG C(String)> () const
{
   SELF(JSONSchema, required);
   TArray<C(String) _ARG int _ARG C(String)> value(IPTR(self->impl, JSONSchema)->required, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::additionalProperties
// (normal::normal)

/*nstSet*/inline /*0H*/JSONSchema JSONSchema::additionalProperties_Prop::operator= (/*0H*/JSONSchema v)
{
   SELF(JSONSchema, additionalProperties);
   IPTR(self->impl, JSONSchema)->additionalProperties = v.impl;
   return v;
}

/*regSet*/inline JSONSchema::additionalProperties_Prop & JSONSchema::additionalProperties_Prop::operator= (JSONSchema::additionalProperties_Prop & prop)
{
   SELF(JSONSchema, additionalProperties);
   /*0H*/JSONSchema v = prop;
   IPTR(self->impl, JSONSchema)->additionalProperties = v.impl;
   return prop;
}
/*regGet*/inline JSONSchema::additionalProperties_Prop::operator /*0I*/JSONSchema () const
{
   SELF(JSONSchema, additionalProperties);
   JSONSchema value(IPTR(self->impl, JSONSchema)->additionalProperties, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::definitions
// (normal::normal)

/*nstSet*/inline /*0H*/TMap<C(String) _ARG C(JSONSchema)> JSONSchema::definitions_Prop::operator= (/*0H*/TMap<C(String) _ARG C(JSONSchema)> v)
{
   SELF(JSONSchema, definitions);
   IPTR(self->impl, JSONSchema)->definitions = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline JSONSchema::definitions_Prop & JSONSchema::definitions_Prop::operator= (JSONSchema::definitions_Prop & prop)
{
   SELF(JSONSchema, definitions);
   /*0H*/TMap<C(String) _ARG C(JSONSchema)> v = prop;
   IPTR(self->impl, JSONSchema)->definitions = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline JSONSchema::definitions_Prop::operator /*0I*/TMap<C(String) _ARG C(JSONSchema)> () const
{
   SELF(JSONSchema, definitions);
   TMap<C(String) _ARG C(JSONSchema)> value(IPTR(self->impl, JSONSchema)->definitions, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::properties
// (normal::normal)

/*nstSet*/inline /*0H*/TMap<C(String) _ARG C(JSONSchema)> JSONSchema::properties_Prop::operator= (/*0H*/TMap<C(String) _ARG C(JSONSchema)> v)
{
   SELF(JSONSchema, properties);
   IPTR(self->impl, JSONSchema)->properties = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline JSONSchema::properties_Prop & JSONSchema::properties_Prop::operator= (JSONSchema::properties_Prop & prop)
{
   SELF(JSONSchema, properties);
   /*0H*/TMap<C(String) _ARG C(JSONSchema)> v = prop;
   IPTR(self->impl, JSONSchema)->properties = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline JSONSchema::properties_Prop::operator /*0I*/TMap<C(String) _ARG C(JSONSchema)> () const
{
   SELF(JSONSchema, properties);
   TMap<C(String) _ARG C(JSONSchema)> value(IPTR(self->impl, JSONSchema)->properties, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::patternProperties
// (normal::normal)

/*nstSet*/inline /*0H*/TMap<C(String) _ARG C(JSONSchema)> JSONSchema::patternProperties_Prop::operator= (/*0H*/TMap<C(String) _ARG C(JSONSchema)> v)
{
   SELF(JSONSchema, patternProperties);
   IPTR(self->impl, JSONSchema)->patternProperties = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline JSONSchema::patternProperties_Prop & JSONSchema::patternProperties_Prop::operator= (JSONSchema::patternProperties_Prop & prop)
{
   SELF(JSONSchema, patternProperties);
   /*0H*/TMap<C(String) _ARG C(JSONSchema)> v = prop;
   IPTR(self->impl, JSONSchema)->patternProperties = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline JSONSchema::patternProperties_Prop::operator /*0I*/TMap<C(String) _ARG C(JSONSchema)> () const
{
   SELF(JSONSchema, patternProperties);
   TMap<C(String) _ARG C(JSONSchema)> value(IPTR(self->impl, JSONSchema)->patternProperties, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::dependencies
// (normal::normal)

/*nstSet*/inline /*0H*/TMap<C(String) _ARG C(JSONSchema)> JSONSchema::dependencies_Prop::operator= (/*0H*/TMap<C(String) _ARG C(JSONSchema)> v)
{
   SELF(JSONSchema, dependencies);
   IPTR(self->impl, JSONSchema)->dependencies = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline JSONSchema::dependencies_Prop & JSONSchema::dependencies_Prop::operator= (JSONSchema::dependencies_Prop & prop)
{
   SELF(JSONSchema, dependencies);
   /*0H*/TMap<C(String) _ARG C(JSONSchema)> v = prop;
   IPTR(self->impl, JSONSchema)->dependencies = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline JSONSchema::dependencies_Prop::operator /*0I*/TMap<C(String) _ARG C(JSONSchema)> () const
{
   SELF(JSONSchema, dependencies);
   TMap<C(String) _ARG C(JSONSchema)> value(IPTR(self->impl, JSONSchema)->dependencies, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::propertyNames
// (normal::string)

/*nstSet*/inline /*0H*/char * JSONSchema::propertyNames_Prop::operator= (/*0H*/char * v)
{
   SELF(JSONSchema, propertyNames);
   IPTR(self->impl, JSONSchema)->propertyNames = v;
   return v;
}

/*regSet*/inline JSONSchema::propertyNames_Prop & JSONSchema::propertyNames_Prop::operator= (JSONSchema::propertyNames_Prop & prop)
{
   SELF(JSONSchema, propertyNames);
   /*0H*/char * v = prop;
   IPTR(self->impl, JSONSchema)->propertyNames = v;
   return prop;
}
/*regGet*/inline JSONSchema::propertyNames_Prop::operator /*0I*/char * () const
{
   SELF(JSONSchema, propertyNames);
   return self ? IPTR(self->impl, JSONSchema)->propertyNames : 0;
}

// member accessors: JSONSchema::contentEncoding
// (normal::string)

/*nstSet*/inline /*0H*/char * JSONSchema::contentEncoding_Prop::operator= (/*0H*/char * v)
{
   SELF(JSONSchema, contentEncoding);
   IPTR(self->impl, JSONSchema)->contentEncoding = v;
   return v;
}

/*regSet*/inline JSONSchema::contentEncoding_Prop & JSONSchema::contentEncoding_Prop::operator= (JSONSchema::contentEncoding_Prop & prop)
{
   SELF(JSONSchema, contentEncoding);
   /*0H*/char * v = prop;
   IPTR(self->impl, JSONSchema)->contentEncoding = v;
   return prop;
}
/*regGet*/inline JSONSchema::contentEncoding_Prop::operator /*0I*/char * () const
{
   SELF(JSONSchema, contentEncoding);
   return self ? IPTR(self->impl, JSONSchema)->contentEncoding : 0;
}

// member accessors: JSONSchema::If
// (normal::normal)

/*nstSet*/inline /*0H*/JSONSchema JSONSchema::If_Prop::operator= (/*0H*/JSONSchema v)
{
   SELF(JSONSchema, If);
   IPTR(self->impl, JSONSchema)->If = v.impl;
   return v;
}

/*regSet*/inline JSONSchema::If_Prop & JSONSchema::If_Prop::operator= (JSONSchema::If_Prop & prop)
{
   SELF(JSONSchema, If);
   /*0H*/JSONSchema v = prop;
   IPTR(self->impl, JSONSchema)->If = v.impl;
   return prop;
}
/*regGet*/inline JSONSchema::If_Prop::operator /*0I*/JSONSchema () const
{
   SELF(JSONSchema, If);
   JSONSchema value(IPTR(self->impl, JSONSchema)->If, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::Then
// (normal::normal)

/*nstSet*/inline /*0H*/JSONSchema JSONSchema::Then_Prop::operator= (/*0H*/JSONSchema v)
{
   SELF(JSONSchema, Then);
   IPTR(self->impl, JSONSchema)->Then = v.impl;
   return v;
}

/*regSet*/inline JSONSchema::Then_Prop & JSONSchema::Then_Prop::operator= (JSONSchema::Then_Prop & prop)
{
   SELF(JSONSchema, Then);
   /*0H*/JSONSchema v = prop;
   IPTR(self->impl, JSONSchema)->Then = v.impl;
   return prop;
}
/*regGet*/inline JSONSchema::Then_Prop::operator /*0I*/JSONSchema () const
{
   SELF(JSONSchema, Then);
   JSONSchema value(IPTR(self->impl, JSONSchema)->Then, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::Else
// (normal::normal)

/*nstSet*/inline /*0H*/JSONSchema JSONSchema::Else_Prop::operator= (/*0H*/JSONSchema v)
{
   SELF(JSONSchema, Else);
   IPTR(self->impl, JSONSchema)->Else = v.impl;
   return v;
}

/*regSet*/inline JSONSchema::Else_Prop & JSONSchema::Else_Prop::operator= (JSONSchema::Else_Prop & prop)
{
   SELF(JSONSchema, Else);
   /*0H*/JSONSchema v = prop;
   IPTR(self->impl, JSONSchema)->Else = v.impl;
   return prop;
}
/*regGet*/inline JSONSchema::Else_Prop::operator /*0I*/JSONSchema () const
{
   SELF(JSONSchema, Else);
   JSONSchema value(IPTR(self->impl, JSONSchema)->Else, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::allOf
// (normal::normal)

/*nstSet*/inline /*0H*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> JSONSchema::allOf_Prop::operator= (/*0H*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> v)
{
   SELF(JSONSchema, allOf);
   IPTR(self->impl, JSONSchema)->allOf = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline JSONSchema::allOf_Prop & JSONSchema::allOf_Prop::operator= (JSONSchema::allOf_Prop & prop)
{
   SELF(JSONSchema, allOf);
   /*0H*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> v = prop;
   IPTR(self->impl, JSONSchema)->allOf = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline JSONSchema::allOf_Prop::operator /*0I*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> () const
{
   SELF(JSONSchema, allOf);
   TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> value(IPTR(self->impl, JSONSchema)->allOf, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::anyOf
// (normal::normal)

/*nstSet*/inline /*0H*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> JSONSchema::anyOf_Prop::operator= (/*0H*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> v)
{
   SELF(JSONSchema, anyOf);
   IPTR(self->impl, JSONSchema)->anyOf = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline JSONSchema::anyOf_Prop & JSONSchema::anyOf_Prop::operator= (JSONSchema::anyOf_Prop & prop)
{
   SELF(JSONSchema, anyOf);
   /*0H*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> v = prop;
   IPTR(self->impl, JSONSchema)->anyOf = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline JSONSchema::anyOf_Prop::operator /*0I*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> () const
{
   SELF(JSONSchema, anyOf);
   TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> value(IPTR(self->impl, JSONSchema)->anyOf, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::oneOf
// (normal::normal)

/*nstSet*/inline /*0H*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> JSONSchema::oneOf_Prop::operator= (/*0H*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> v)
{
   SELF(JSONSchema, oneOf);
   IPTR(self->impl, JSONSchema)->oneOf = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/inline JSONSchema::oneOf_Prop & JSONSchema::oneOf_Prop::operator= (JSONSchema::oneOf_Prop & prop)
{
   SELF(JSONSchema, oneOf);
   /*0H*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> v = prop;
   IPTR(self->impl, JSONSchema)->oneOf = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/inline JSONSchema::oneOf_Prop::operator /*0I*/TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> () const
{
   SELF(JSONSchema, oneOf);
   TArray<C(JSONSchema) _ARG int _ARG C(JSONSchema)> value(IPTR(self->impl, JSONSchema)->oneOf, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::not
// (normal::normal)

/*nstSet*/inline /*0H*/JSONSchema JSONSchema::not_Prop::operator= (/*0H*/JSONSchema v)
{
   SELF(JSONSchema, _not);
   IPTR(self->impl, JSONSchema)->_not = v.impl;
   return v;
}

/*regSet*/inline JSONSchema::not_Prop & JSONSchema::not_Prop::operator= (JSONSchema::not_Prop & prop)
{
   SELF(JSONSchema, _not);
   /*0H*/JSONSchema v = prop;
   IPTR(self->impl, JSONSchema)->_not = v.impl;
   return prop;
}
/*regGet*/inline JSONSchema::not_Prop::operator /*0I*/JSONSchema () const
{
   SELF(JSONSchema, _not);
   JSONSchema value(IPTR(self->impl, JSONSchema)->_not, JSONSchema::_cpp_class);
   return value;
}

// member accessors: JSONSchema::xogcrole
// (normal::string)

/*nstSet*/inline /*0H*/char * JSONSchema::xogcrole_Prop::operator= (/*0H*/char * v)
{
   SELF(JSONSchema, xogcrole);
   IPTR(self->impl, JSONSchema)->xogcrole = v;
   return v;
}

/*regSet*/inline JSONSchema::xogcrole_Prop & JSONSchema::xogcrole_Prop::operator= (JSONSchema::xogcrole_Prop & prop)
{
   SELF(JSONSchema, xogcrole);
   /*0H*/char * v = prop;
   IPTR(self->impl, JSONSchema)->xogcrole = v;
   return prop;
}
/*regGet*/inline JSONSchema::xogcrole_Prop::operator /*0I*/char * () const
{
   SELF(JSONSchema, xogcrole);
   return self ? IPTR(self->impl, JSONSchema)->xogcrole : 0;
}
// function: readDGGSJSON
inline DGGSJSON readDGGSJSON(/*1Ab*/File & f)
{
   return DGGSJSON(F(readDGGSJSON)(/*5De*/f.impl));
}


#endif // !defined(__DGGAL_HPP__)
