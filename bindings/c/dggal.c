#include "dggal.h"

#if defined(EC_STATIC)
// REVIEW: Clashes between function pointers and functions building statically?
#define readDGGSJSON fnptr_readDGGSJSON
#endif

// Global Functions Pointers

LIB_EXPORT C(GlobalFunction) * FUNCTION(readDGGSJSON);



// Virtual Methods

LIB_EXPORT C(Method) * METHOD(DGGRS, areZonesNeighbors);
LIB_EXPORT C(Method) * METHOD(DGGRS, areZonesSiblings);
LIB_EXPORT C(Method) * METHOD(DGGRS, compactZones);
LIB_EXPORT C(Method) * METHOD(DGGRS, countSubZones);
LIB_EXPORT C(Method) * METHOD(DGGRS, countZoneEdges);
LIB_EXPORT C(Method) * METHOD(DGGRS, countZones);
LIB_EXPORT C(Method) * METHOD(DGGRS, doZonesOverlap);
LIB_EXPORT C(Method) * METHOD(DGGRS, doesZoneContain);
LIB_EXPORT C(Method) * METHOD(DGGRS, get64KDepth);
LIB_EXPORT C(Method) * METHOD(DGGRS, getFirstSubZone);
LIB_EXPORT C(Method) * METHOD(DGGRS, getIndexMaxDepth);
LIB_EXPORT C(Method) * METHOD(DGGRS, getLevelFromMetersPerSubZone);
LIB_EXPORT C(Method) * METHOD(DGGRS, getLevelFromPixelsAndExtent);
LIB_EXPORT C(Method) * METHOD(DGGRS, getLevelFromRefZoneArea);
LIB_EXPORT C(Method) * METHOD(DGGRS, getLevelFromScaleDenominator);
LIB_EXPORT C(Method) * METHOD(DGGRS, getMaxChildren);
LIB_EXPORT C(Method) * METHOD(DGGRS, getMaxDGGRSZoneLevel);
LIB_EXPORT C(Method) * METHOD(DGGRS, getMaxDepth);
LIB_EXPORT C(Method) * METHOD(DGGRS, getMaxNeighbors);
LIB_EXPORT C(Method) * METHOD(DGGRS, getMaxParents);
LIB_EXPORT C(Method) * METHOD(DGGRS, getMetersPerSubZoneFromLevel);
LIB_EXPORT C(Method) * METHOD(DGGRS, getRefZoneArea);
LIB_EXPORT C(Method) * METHOD(DGGRS, getRefinementRatio);
LIB_EXPORT C(Method) * METHOD(DGGRS, getScaleDenominatorFromLevel);
LIB_EXPORT C(Method) * METHOD(DGGRS, getSubZoneAtIndex);
LIB_EXPORT C(Method) * METHOD(DGGRS, getSubZoneCRSCentroids);
LIB_EXPORT C(Method) * METHOD(DGGRS, getSubZoneIndex);
LIB_EXPORT C(Method) * METHOD(DGGRS, getSubZoneWGS84Centroids);
LIB_EXPORT C(Method) * METHOD(DGGRS, getSubZones);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneArea);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneCRSCentroid);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneCRSExtent);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneCRSVertices);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneCentroidChild);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneCentroidParent);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneChildren);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneFromCRSCentroid);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneFromTextID);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneFromWGS84Centroid);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneLevel);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneNeighbors);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneParents);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneRefinedCRSVertices);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneRefinedWGS84Vertices);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneTextID);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneWGS84Centroid);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneWGS84Extent);
LIB_EXPORT C(Method) * METHOD(DGGRS, getZoneWGS84Vertices);
LIB_EXPORT C(Method) * METHOD(DGGRS, isZoneAncestorOf);
LIB_EXPORT C(Method) * METHOD(DGGRS, isZoneCentroidChild);
LIB_EXPORT C(Method) * METHOD(DGGRS, isZoneContainedIn);
LIB_EXPORT C(Method) * METHOD(DGGRS, isZoneDescendantOf);
LIB_EXPORT C(Method) * METHOD(DGGRS, isZoneImmediateChildOf);
LIB_EXPORT C(Method) * METHOD(DGGRS, isZoneImmediateParentOf);
LIB_EXPORT C(Method) * METHOD(DGGRS, listZones);
LIB_EXPORT C(Method) * METHOD(DGGRS, zoneHasSubZone);

LIB_EXPORT C(Method) * METHOD(GeoExtent, clear);
LIB_EXPORT C(Method) * METHOD(GeoExtent, intersects);

LIB_EXPORT C(Method) * METHOD(Plane, fromPoints);

LIB_EXPORT C(Method) * METHOD(Vector3D, crossProduct);
LIB_EXPORT C(Method) * METHOD(Vector3D, dotProduct);
LIB_EXPORT C(Method) * METHOD(Vector3D, normalize);
LIB_EXPORT C(Method) * METHOD(Vector3D, subtract);




// Methods Function Pointers

LIB_EXPORT C(bool) (* DGGRS_areZonesNeighbors)(C(DGGRS) __this, C(DGGRSZone) a, C(DGGRSZone) b);
LIB_EXPORT C(bool) (* DGGRS_areZonesSiblings)(C(DGGRS) __this, C(DGGRSZone) a, C(DGGRSZone) b);
LIB_EXPORT C(bool) (* DGGRS_doZonesOverlap)(C(DGGRS) __this, C(DGGRSZone) a, C(DGGRSZone) b);
LIB_EXPORT C(bool) (* DGGRS_doesZoneContain)(C(DGGRS) __this, C(DGGRSZone) hayStack, C(DGGRSZone) needle);
LIB_EXPORT int (* DGGRS_get64KDepth)(C(DGGRS) __this);
LIB_EXPORT int (* DGGRS_getLevelFromMetersPerSubZone)(C(DGGRS) __this, double physicalMetersPerSubZone, int relativeDepth);
LIB_EXPORT int (* DGGRS_getLevelFromPixelsAndExtent)(C(DGGRS) __this, const C(GeoExtent) * extent, C(Point) * pixels, int relativeDepth);
LIB_EXPORT int (* DGGRS_getLevelFromRefZoneArea)(C(DGGRS) __this, double metersSquared);
LIB_EXPORT int (* DGGRS_getLevelFromScaleDenominator)(C(DGGRS) __this, double scaleDenominator, int relativeDepth, double mmPerPixel);
LIB_EXPORT int (* DGGRS_getMaxDepth)(C(DGGRS) __this);
LIB_EXPORT double (* DGGRS_getMetersPerSubZoneFromLevel)(C(DGGRS) __this, int parentLevel, int relativeDepth);
LIB_EXPORT double (* DGGRS_getRefZoneArea)(C(DGGRS) __this, int level);
LIB_EXPORT double (* DGGRS_getScaleDenominatorFromLevel)(C(DGGRS) __this, int parentLevel, int relativeDepth, double mmPerPixel);
LIB_EXPORT C(bool) (* DGGRS_isZoneAncestorOf)(C(DGGRS) __this, C(DGGRSZone) ancestor, C(DGGRSZone) descendant, int maxDepth);
LIB_EXPORT C(bool) (* DGGRS_isZoneContainedIn)(C(DGGRS) __this, C(DGGRSZone) needle, C(DGGRSZone) hayStack);
LIB_EXPORT C(bool) (* DGGRS_isZoneDescendantOf)(C(DGGRS) __this, C(DGGRSZone) descendant, C(DGGRSZone) ancestor, int maxDepth);
LIB_EXPORT C(bool) (* DGGRS_isZoneImmediateChildOf)(C(DGGRS) __this, C(DGGRSZone) child, C(DGGRSZone) parent);
LIB_EXPORT C(bool) (* DGGRS_isZoneImmediateParentOf)(C(DGGRS) __this, C(DGGRSZone) parent, C(DGGRSZone) child);
LIB_EXPORT C(bool) (* DGGRS_zoneHasSubZone)(C(DGGRS) __this, C(DGGRSZone) hayStack, C(DGGRSZone) needle);

LIB_EXPORT void (* GeoExtent_clear)(C(GeoExtent) * __this);
LIB_EXPORT C(bool) (* GeoExtent_intersects)(C(GeoExtent) * __this, const C(GeoExtent) * b);

LIB_EXPORT void (* Plane_fromPoints)(C(Plane) * __this, const C(Vector3D) * v1, const C(Vector3D) * v2, const C(Vector3D) * v3);

LIB_EXPORT void (* Vector3D_crossProduct)(C(Vector3D) * __this, const C(Vector3D) * vector1, const C(Vector3D) * vector2);
LIB_EXPORT double (* Vector3D_dotProduct)(C(Vector3D) * __this, const C(Vector3D) * vector2);
LIB_EXPORT void (* Vector3D_normalize)(C(Vector3D) * __this, const C(Vector3D) * source);
LIB_EXPORT void (* Vector3D_subtract)(C(Vector3D) * __this, const C(Vector3D) * vector1, const C(Vector3D) * vector2);



LIB_EXPORT C(Property) * PROPERTY(GeoExtent, geodeticArea);
LIB_EXPORT double (* GeoExtent_get_geodeticArea)(const C(GeoExtent) * g);

LIB_EXPORT C(Property) * PROPERTY(JSONSchema, maximum);
LIB_EXPORT double (* JSONSchema_get_maximum)(const C(JSONSchema) j);
LIB_EXPORT C(bool) (* JSONSchema_isSet_maximum)(const C(JSONSchema) j);

LIB_EXPORT C(Property) * PROPERTY(JSONSchema, exclusiveMaximum);
LIB_EXPORT double (* JSONSchema_get_exclusiveMaximum)(const C(JSONSchema) j);
LIB_EXPORT C(bool) (* JSONSchema_isSet_exclusiveMaximum)(const C(JSONSchema) j);

LIB_EXPORT C(Property) * PROPERTY(JSONSchema, minimum);
LIB_EXPORT double (* JSONSchema_get_minimum)(const C(JSONSchema) j);
LIB_EXPORT C(bool) (* JSONSchema_isSet_minimum)(const C(JSONSchema) j);

LIB_EXPORT C(Property) * PROPERTY(JSONSchema, exclusiveMinimum);
LIB_EXPORT double (* JSONSchema_get_exclusiveMinimum)(const C(JSONSchema) j);
LIB_EXPORT C(bool) (* JSONSchema_isSet_exclusiveMinimum)(const C(JSONSchema) j);

LIB_EXPORT C(Property) * PROPERTY(JSONSchema, maxItems);
LIB_EXPORT int (* JSONSchema_get_maxItems)(const C(JSONSchema) j);
LIB_EXPORT C(bool) (* JSONSchema_isSet_maxItems)(const C(JSONSchema) j);

LIB_EXPORT C(Property) * PROPERTY(JSONSchema, minItems);
LIB_EXPORT int (* JSONSchema_get_minItems)(const C(JSONSchema) j);
LIB_EXPORT C(bool) (* JSONSchema_isSet_minItems)(const C(JSONSchema) j);

LIB_EXPORT C(Property) * PROPERTY(JSONSchema, maxProperties);
LIB_EXPORT int (* JSONSchema_get_maxProperties)(const C(JSONSchema) j);
LIB_EXPORT C(bool) (* JSONSchema_isSet_maxProperties)(const C(JSONSchema) j);

LIB_EXPORT C(Property) * PROPERTY(JSONSchema, minProperties);
LIB_EXPORT int (* JSONSchema_get_minProperties)(const C(JSONSchema) j);
LIB_EXPORT C(bool) (* JSONSchema_isSet_minProperties)(const C(JSONSchema) j);

LIB_EXPORT C(Property) * PROPERTY(JSONSchema, xogcpropertySeq);
LIB_EXPORT C(bool) (* JSONSchema_isSet_xogcpropertySeq)(const C(JSONSchema) j);

LIB_EXPORT C(Property) * PROPERTY(JSONSchema, Default);
LIB_EXPORT C(bool) (* JSONSchema_isSet_Default)(const C(JSONSchema) j);

LIB_EXPORT C(Property) * PROPERTY(Vector3D, length);
LIB_EXPORT double (* Vector3D_get_length)(const C(Vector3D) * v);


// Properties




// Classes

// bitClass
LIB_EXPORT C(Class) * CO(CRS);
LIB_EXPORT C(Class) * CO(DGGRSZone);
LIB_EXPORT C(Class) * CO(GGGZone);
LIB_EXPORT C(Class) * CO(I3HZone);
LIB_EXPORT C(Class) * CO(I9RZone);
// enumClass
LIB_EXPORT C(Class) * CO(CRSRegistry);
LIB_EXPORT C(Class) * CO(JSONSchemaType);
// unitClass
// systemClass
// structClass
LIB_EXPORT C(Class) * CO(CRSExtent);
LIB_EXPORT C(Class) * CO(GeoExtent);
LIB_EXPORT C(Class) * CO(GeoPoint);
LIB_EXPORT C(Class) * CO(Plane);
LIB_EXPORT C(Class) * CO(Vector3D);
// noHeadClass
// normalClass
LIB_EXPORT C(Class) * CO(BCTA3H);
LIB_EXPORT C(Class) * CO(DGGRS);
LIB_EXPORT C(Class) * CO(DGGSJSON);
LIB_EXPORT C(Class) * CO(DGGSJSONDepth);
LIB_EXPORT C(Class) * CO(DGGSJSONGrid);
LIB_EXPORT C(Class) * CO(DGGSJSONShape);
LIB_EXPORT C(Class) * CO(GNOSISGlobalGrid);
LIB_EXPORT C(Class) * CO(GPP3H);
LIB_EXPORT C(Class) * CO(ISEA3H);
LIB_EXPORT C(Class) * CO(ISEA9R);
LIB_EXPORT C(Class) * CO(IVEA3H);
LIB_EXPORT C(Class) * CO(IVEA9R);
LIB_EXPORT C(Class) * CO(RTEA3H);
LIB_EXPORT C(Class) * CO(RTEA9R);
LIB_EXPORT C(Class) * CO(rHEALPix);
LIB_EXPORT C(Class) * CO(JSONSchema);
LIB_EXPORT C(Class) * CO(RhombicIcosahedral3H);
LIB_EXPORT C(Class) * CO(RhombicIcosahedral9R);



// Virtual Method IDs

LIB_EXPORT int M_VTBLID(DGGRS, compactZones);
LIB_EXPORT int M_VTBLID(DGGRS, countSubZones);
LIB_EXPORT int M_VTBLID(DGGRS, countZoneEdges);
LIB_EXPORT int M_VTBLID(DGGRS, countZones);
LIB_EXPORT int M_VTBLID(DGGRS, getFirstSubZone);
LIB_EXPORT int M_VTBLID(DGGRS, getIndexMaxDepth);
LIB_EXPORT int M_VTBLID(DGGRS, getMaxChildren);
LIB_EXPORT int M_VTBLID(DGGRS, getMaxDGGRSZoneLevel);
LIB_EXPORT int M_VTBLID(DGGRS, getMaxNeighbors);
LIB_EXPORT int M_VTBLID(DGGRS, getMaxParents);
LIB_EXPORT int M_VTBLID(DGGRS, getRefinementRatio);
LIB_EXPORT int M_VTBLID(DGGRS, getSubZoneAtIndex);
LIB_EXPORT int M_VTBLID(DGGRS, getSubZoneCRSCentroids);
LIB_EXPORT int M_VTBLID(DGGRS, getSubZoneIndex);
LIB_EXPORT int M_VTBLID(DGGRS, getSubZoneWGS84Centroids);
LIB_EXPORT int M_VTBLID(DGGRS, getSubZones);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneArea);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneCRSCentroid);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneCRSExtent);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneCRSVertices);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneCentroidChild);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneCentroidParent);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneChildren);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneFromCRSCentroid);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneFromTextID);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneFromWGS84Centroid);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneLevel);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneNeighbors);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneParents);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneRefinedCRSVertices);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneRefinedWGS84Vertices);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneTextID);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneWGS84Centroid);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneWGS84Extent);
LIB_EXPORT int M_VTBLID(DGGRS, getZoneWGS84Vertices);
LIB_EXPORT int M_VTBLID(DGGRS, isZoneCentroidChild);
LIB_EXPORT int M_VTBLID(DGGRS, listZones);


#ifdef EC_STATIC
unsigned int __eCDll_Load_dggal(C(Module) * module);
unsigned int __eCDll_Unload_dggal(C(Module) * module);
#endif


// Global Functions

LIB_EXPORT C(DGGSJSON) (* F(readDGGSJSON))(C(File) f);


LIB_EXPORT C(Module) dggal_init(C(Module) fromModule)
{
#ifdef EC_STATIC
   C(Module) module = Module_loadStatic(fromModule, DGGAL_MODULE_NAME, true, (void *)(__eCDll_Load_dggal), (void *)(__eCDll_Unload_dggal));
#else
   C(Module) module = Module_load(fromModule, DGGAL_MODULE_NAME, AccessMode_publicAccess);
#endif

#ifdef _DEBUG
   // printf("%s_init\n", "dggal");
#endif

   if(module)
   {
      // Set up all the CO(x) *, property, method, ...


      CO(BCTA3H) = eC_findClass(module, "BCTA3H");
      CO(CRS) = eC_findClass(module, "CRS");
      CO(CRSExtent) = eC_findClass(module, "CRSExtent");
      CO(CRSRegistry) = eC_findClass(module, "CRSRegistry");
      CO(DGGRS) = eC_findClass(module, "DGGRS");
      if(CO(DGGRS))
      {
         METHOD(DGGRS, areZonesNeighbors) = Class_findMethod(CO(DGGRS), "areZonesNeighbors", module);
         if(METHOD(DGGRS, areZonesNeighbors))
            DGGRS_areZonesNeighbors = (C(bool) (*)(C(DGGRS), C(DGGRSZone), C(DGGRSZone)))METHOD(DGGRS, areZonesNeighbors)->function;

         METHOD(DGGRS, areZonesSiblings) = Class_findMethod(CO(DGGRS), "areZonesSiblings", module);
         if(METHOD(DGGRS, areZonesSiblings))
            DGGRS_areZonesSiblings = (C(bool) (*)(C(DGGRS), C(DGGRSZone), C(DGGRSZone)))METHOD(DGGRS, areZonesSiblings)->function;

         METHOD(DGGRS, compactZones) = Class_findMethod(CO(DGGRS), "compactZones", module);
         if(METHOD(DGGRS, compactZones))
            M_VTBLID(DGGRS, compactZones) = METHOD(DGGRS, compactZones)->vid;

         METHOD(DGGRS, countSubZones) = Class_findMethod(CO(DGGRS), "countSubZones", module);
         if(METHOD(DGGRS, countSubZones))
            M_VTBLID(DGGRS, countSubZones) = METHOD(DGGRS, countSubZones)->vid;

         METHOD(DGGRS, countZoneEdges) = Class_findMethod(CO(DGGRS), "countZoneEdges", module);
         if(METHOD(DGGRS, countZoneEdges))
            M_VTBLID(DGGRS, countZoneEdges) = METHOD(DGGRS, countZoneEdges)->vid;

         METHOD(DGGRS, countZones) = Class_findMethod(CO(DGGRS), "countZones", module);
         if(METHOD(DGGRS, countZones))
            M_VTBLID(DGGRS, countZones) = METHOD(DGGRS, countZones)->vid;

         METHOD(DGGRS, doZonesOverlap) = Class_findMethod(CO(DGGRS), "doZonesOverlap", module);
         if(METHOD(DGGRS, doZonesOverlap))
            DGGRS_doZonesOverlap = (C(bool) (*)(C(DGGRS), C(DGGRSZone), C(DGGRSZone)))METHOD(DGGRS, doZonesOverlap)->function;

         METHOD(DGGRS, doesZoneContain) = Class_findMethod(CO(DGGRS), "doesZoneContain", module);
         if(METHOD(DGGRS, doesZoneContain))
            DGGRS_doesZoneContain = (C(bool) (*)(C(DGGRS), C(DGGRSZone), C(DGGRSZone)))METHOD(DGGRS, doesZoneContain)->function;

         METHOD(DGGRS, get64KDepth) = Class_findMethod(CO(DGGRS), "get64KDepth", module);
         if(METHOD(DGGRS, get64KDepth))
            DGGRS_get64KDepth = (int (*)(C(DGGRS)))METHOD(DGGRS, get64KDepth)->function;

         METHOD(DGGRS, getFirstSubZone) = Class_findMethod(CO(DGGRS), "getFirstSubZone", module);
         if(METHOD(DGGRS, getFirstSubZone))
            M_VTBLID(DGGRS, getFirstSubZone) = METHOD(DGGRS, getFirstSubZone)->vid;

         METHOD(DGGRS, getIndexMaxDepth) = Class_findMethod(CO(DGGRS), "getIndexMaxDepth", module);
         if(METHOD(DGGRS, getIndexMaxDepth))
            M_VTBLID(DGGRS, getIndexMaxDepth) = METHOD(DGGRS, getIndexMaxDepth)->vid;

         METHOD(DGGRS, getLevelFromMetersPerSubZone) = Class_findMethod(CO(DGGRS), "getLevelFromMetersPerSubZone", module);
         if(METHOD(DGGRS, getLevelFromMetersPerSubZone))
            DGGRS_getLevelFromMetersPerSubZone = (int (*)(C(DGGRS), double, int))METHOD(DGGRS, getLevelFromMetersPerSubZone)->function;

         METHOD(DGGRS, getLevelFromPixelsAndExtent) = Class_findMethod(CO(DGGRS), "getLevelFromPixelsAndExtent", module);
         if(METHOD(DGGRS, getLevelFromPixelsAndExtent))
            DGGRS_getLevelFromPixelsAndExtent = (int (*)(C(DGGRS), const C(GeoExtent) *, C(Point) *, int))METHOD(DGGRS, getLevelFromPixelsAndExtent)->function;

         METHOD(DGGRS, getLevelFromRefZoneArea) = Class_findMethod(CO(DGGRS), "getLevelFromRefZoneArea", module);
         if(METHOD(DGGRS, getLevelFromRefZoneArea))
            DGGRS_getLevelFromRefZoneArea = (int (*)(C(DGGRS), double))METHOD(DGGRS, getLevelFromRefZoneArea)->function;

         METHOD(DGGRS, getLevelFromScaleDenominator) = Class_findMethod(CO(DGGRS), "getLevelFromScaleDenominator", module);
         if(METHOD(DGGRS, getLevelFromScaleDenominator))
            DGGRS_getLevelFromScaleDenominator = (int (*)(C(DGGRS), double, int, double))METHOD(DGGRS, getLevelFromScaleDenominator)->function;

         METHOD(DGGRS, getMaxChildren) = Class_findMethod(CO(DGGRS), "getMaxChildren", module);
         if(METHOD(DGGRS, getMaxChildren))
            M_VTBLID(DGGRS, getMaxChildren) = METHOD(DGGRS, getMaxChildren)->vid;

         METHOD(DGGRS, getMaxDGGRSZoneLevel) = Class_findMethod(CO(DGGRS), "getMaxDGGRSZoneLevel", module);
         if(METHOD(DGGRS, getMaxDGGRSZoneLevel))
            M_VTBLID(DGGRS, getMaxDGGRSZoneLevel) = METHOD(DGGRS, getMaxDGGRSZoneLevel)->vid;

         METHOD(DGGRS, getMaxDepth) = Class_findMethod(CO(DGGRS), "getMaxDepth", module);
         if(METHOD(DGGRS, getMaxDepth))
            DGGRS_getMaxDepth = (int (*)(C(DGGRS)))METHOD(DGGRS, getMaxDepth)->function;

         METHOD(DGGRS, getMaxNeighbors) = Class_findMethod(CO(DGGRS), "getMaxNeighbors", module);
         if(METHOD(DGGRS, getMaxNeighbors))
            M_VTBLID(DGGRS, getMaxNeighbors) = METHOD(DGGRS, getMaxNeighbors)->vid;

         METHOD(DGGRS, getMaxParents) = Class_findMethod(CO(DGGRS), "getMaxParents", module);
         if(METHOD(DGGRS, getMaxParents))
            M_VTBLID(DGGRS, getMaxParents) = METHOD(DGGRS, getMaxParents)->vid;

         METHOD(DGGRS, getMetersPerSubZoneFromLevel) = Class_findMethod(CO(DGGRS), "getMetersPerSubZoneFromLevel", module);
         if(METHOD(DGGRS, getMetersPerSubZoneFromLevel))
            DGGRS_getMetersPerSubZoneFromLevel = (double (*)(C(DGGRS), int, int))METHOD(DGGRS, getMetersPerSubZoneFromLevel)->function;

         METHOD(DGGRS, getRefZoneArea) = Class_findMethod(CO(DGGRS), "getRefZoneArea", module);
         if(METHOD(DGGRS, getRefZoneArea))
            DGGRS_getRefZoneArea = (double (*)(C(DGGRS), int))METHOD(DGGRS, getRefZoneArea)->function;

         METHOD(DGGRS, getRefinementRatio) = Class_findMethod(CO(DGGRS), "getRefinementRatio", module);
         if(METHOD(DGGRS, getRefinementRatio))
            M_VTBLID(DGGRS, getRefinementRatio) = METHOD(DGGRS, getRefinementRatio)->vid;

         METHOD(DGGRS, getScaleDenominatorFromLevel) = Class_findMethod(CO(DGGRS), "getScaleDenominatorFromLevel", module);
         if(METHOD(DGGRS, getScaleDenominatorFromLevel))
            DGGRS_getScaleDenominatorFromLevel = (double (*)(C(DGGRS), int, int, double))METHOD(DGGRS, getScaleDenominatorFromLevel)->function;

         METHOD(DGGRS, getSubZoneAtIndex) = Class_findMethod(CO(DGGRS), "getSubZoneAtIndex", module);
         if(METHOD(DGGRS, getSubZoneAtIndex))
            M_VTBLID(DGGRS, getSubZoneAtIndex) = METHOD(DGGRS, getSubZoneAtIndex)->vid;

         METHOD(DGGRS, getSubZoneCRSCentroids) = Class_findMethod(CO(DGGRS), "getSubZoneCRSCentroids", module);
         if(METHOD(DGGRS, getSubZoneCRSCentroids))
            M_VTBLID(DGGRS, getSubZoneCRSCentroids) = METHOD(DGGRS, getSubZoneCRSCentroids)->vid;

         METHOD(DGGRS, getSubZoneIndex) = Class_findMethod(CO(DGGRS), "getSubZoneIndex", module);
         if(METHOD(DGGRS, getSubZoneIndex))
            M_VTBLID(DGGRS, getSubZoneIndex) = METHOD(DGGRS, getSubZoneIndex)->vid;

         METHOD(DGGRS, getSubZoneWGS84Centroids) = Class_findMethod(CO(DGGRS), "getSubZoneWGS84Centroids", module);
         if(METHOD(DGGRS, getSubZoneWGS84Centroids))
            M_VTBLID(DGGRS, getSubZoneWGS84Centroids) = METHOD(DGGRS, getSubZoneWGS84Centroids)->vid;

         METHOD(DGGRS, getSubZones) = Class_findMethod(CO(DGGRS), "getSubZones", module);
         if(METHOD(DGGRS, getSubZones))
            M_VTBLID(DGGRS, getSubZones) = METHOD(DGGRS, getSubZones)->vid;

         METHOD(DGGRS, getZoneArea) = Class_findMethod(CO(DGGRS), "getZoneArea", module);
         if(METHOD(DGGRS, getZoneArea))
            M_VTBLID(DGGRS, getZoneArea) = METHOD(DGGRS, getZoneArea)->vid;

         METHOD(DGGRS, getZoneCRSCentroid) = Class_findMethod(CO(DGGRS), "getZoneCRSCentroid", module);
         if(METHOD(DGGRS, getZoneCRSCentroid))
            M_VTBLID(DGGRS, getZoneCRSCentroid) = METHOD(DGGRS, getZoneCRSCentroid)->vid;

         METHOD(DGGRS, getZoneCRSExtent) = Class_findMethod(CO(DGGRS), "getZoneCRSExtent", module);
         if(METHOD(DGGRS, getZoneCRSExtent))
            M_VTBLID(DGGRS, getZoneCRSExtent) = METHOD(DGGRS, getZoneCRSExtent)->vid;

         METHOD(DGGRS, getZoneCRSVertices) = Class_findMethod(CO(DGGRS), "getZoneCRSVertices", module);
         if(METHOD(DGGRS, getZoneCRSVertices))
            M_VTBLID(DGGRS, getZoneCRSVertices) = METHOD(DGGRS, getZoneCRSVertices)->vid;

         METHOD(DGGRS, getZoneCentroidChild) = Class_findMethod(CO(DGGRS), "getZoneCentroidChild", module);
         if(METHOD(DGGRS, getZoneCentroidChild))
            M_VTBLID(DGGRS, getZoneCentroidChild) = METHOD(DGGRS, getZoneCentroidChild)->vid;

         METHOD(DGGRS, getZoneCentroidParent) = Class_findMethod(CO(DGGRS), "getZoneCentroidParent", module);
         if(METHOD(DGGRS, getZoneCentroidParent))
            M_VTBLID(DGGRS, getZoneCentroidParent) = METHOD(DGGRS, getZoneCentroidParent)->vid;

         METHOD(DGGRS, getZoneChildren) = Class_findMethod(CO(DGGRS), "getZoneChildren", module);
         if(METHOD(DGGRS, getZoneChildren))
            M_VTBLID(DGGRS, getZoneChildren) = METHOD(DGGRS, getZoneChildren)->vid;

         METHOD(DGGRS, getZoneFromCRSCentroid) = Class_findMethod(CO(DGGRS), "getZoneFromCRSCentroid", module);
         if(METHOD(DGGRS, getZoneFromCRSCentroid))
            M_VTBLID(DGGRS, getZoneFromCRSCentroid) = METHOD(DGGRS, getZoneFromCRSCentroid)->vid;

         METHOD(DGGRS, getZoneFromTextID) = Class_findMethod(CO(DGGRS), "getZoneFromTextID", module);
         if(METHOD(DGGRS, getZoneFromTextID))
            M_VTBLID(DGGRS, getZoneFromTextID) = METHOD(DGGRS, getZoneFromTextID)->vid;

         METHOD(DGGRS, getZoneFromWGS84Centroid) = Class_findMethod(CO(DGGRS), "getZoneFromWGS84Centroid", module);
         if(METHOD(DGGRS, getZoneFromWGS84Centroid))
            M_VTBLID(DGGRS, getZoneFromWGS84Centroid) = METHOD(DGGRS, getZoneFromWGS84Centroid)->vid;

         METHOD(DGGRS, getZoneLevel) = Class_findMethod(CO(DGGRS), "getZoneLevel", module);
         if(METHOD(DGGRS, getZoneLevel))
            M_VTBLID(DGGRS, getZoneLevel) = METHOD(DGGRS, getZoneLevel)->vid;

         METHOD(DGGRS, getZoneNeighbors) = Class_findMethod(CO(DGGRS), "getZoneNeighbors", module);
         if(METHOD(DGGRS, getZoneNeighbors))
            M_VTBLID(DGGRS, getZoneNeighbors) = METHOD(DGGRS, getZoneNeighbors)->vid;

         METHOD(DGGRS, getZoneParents) = Class_findMethod(CO(DGGRS), "getZoneParents", module);
         if(METHOD(DGGRS, getZoneParents))
            M_VTBLID(DGGRS, getZoneParents) = METHOD(DGGRS, getZoneParents)->vid;

         METHOD(DGGRS, getZoneRefinedCRSVertices) = Class_findMethod(CO(DGGRS), "getZoneRefinedCRSVertices", module);
         if(METHOD(DGGRS, getZoneRefinedCRSVertices))
            M_VTBLID(DGGRS, getZoneRefinedCRSVertices) = METHOD(DGGRS, getZoneRefinedCRSVertices)->vid;

         METHOD(DGGRS, getZoneRefinedWGS84Vertices) = Class_findMethod(CO(DGGRS), "getZoneRefinedWGS84Vertices", module);
         if(METHOD(DGGRS, getZoneRefinedWGS84Vertices))
            M_VTBLID(DGGRS, getZoneRefinedWGS84Vertices) = METHOD(DGGRS, getZoneRefinedWGS84Vertices)->vid;

         METHOD(DGGRS, getZoneTextID) = Class_findMethod(CO(DGGRS), "getZoneTextID", module);
         if(METHOD(DGGRS, getZoneTextID))
            M_VTBLID(DGGRS, getZoneTextID) = METHOD(DGGRS, getZoneTextID)->vid;

         METHOD(DGGRS, getZoneWGS84Centroid) = Class_findMethod(CO(DGGRS), "getZoneWGS84Centroid", module);
         if(METHOD(DGGRS, getZoneWGS84Centroid))
            M_VTBLID(DGGRS, getZoneWGS84Centroid) = METHOD(DGGRS, getZoneWGS84Centroid)->vid;

         METHOD(DGGRS, getZoneWGS84Extent) = Class_findMethod(CO(DGGRS), "getZoneWGS84Extent", module);
         if(METHOD(DGGRS, getZoneWGS84Extent))
            M_VTBLID(DGGRS, getZoneWGS84Extent) = METHOD(DGGRS, getZoneWGS84Extent)->vid;

         METHOD(DGGRS, getZoneWGS84Vertices) = Class_findMethod(CO(DGGRS), "getZoneWGS84Vertices", module);
         if(METHOD(DGGRS, getZoneWGS84Vertices))
            M_VTBLID(DGGRS, getZoneWGS84Vertices) = METHOD(DGGRS, getZoneWGS84Vertices)->vid;

         METHOD(DGGRS, isZoneAncestorOf) = Class_findMethod(CO(DGGRS), "isZoneAncestorOf", module);
         if(METHOD(DGGRS, isZoneAncestorOf))
            DGGRS_isZoneAncestorOf = (C(bool) (*)(C(DGGRS), C(DGGRSZone), C(DGGRSZone), int))METHOD(DGGRS, isZoneAncestorOf)->function;

         METHOD(DGGRS, isZoneCentroidChild) = Class_findMethod(CO(DGGRS), "isZoneCentroidChild", module);
         if(METHOD(DGGRS, isZoneCentroidChild))
            M_VTBLID(DGGRS, isZoneCentroidChild) = METHOD(DGGRS, isZoneCentroidChild)->vid;

         METHOD(DGGRS, isZoneContainedIn) = Class_findMethod(CO(DGGRS), "isZoneContainedIn", module);
         if(METHOD(DGGRS, isZoneContainedIn))
            DGGRS_isZoneContainedIn = (C(bool) (*)(C(DGGRS), C(DGGRSZone), C(DGGRSZone)))METHOD(DGGRS, isZoneContainedIn)->function;

         METHOD(DGGRS, isZoneDescendantOf) = Class_findMethod(CO(DGGRS), "isZoneDescendantOf", module);
         if(METHOD(DGGRS, isZoneDescendantOf))
            DGGRS_isZoneDescendantOf = (C(bool) (*)(C(DGGRS), C(DGGRSZone), C(DGGRSZone), int))METHOD(DGGRS, isZoneDescendantOf)->function;

         METHOD(DGGRS, isZoneImmediateChildOf) = Class_findMethod(CO(DGGRS), "isZoneImmediateChildOf", module);
         if(METHOD(DGGRS, isZoneImmediateChildOf))
            DGGRS_isZoneImmediateChildOf = (C(bool) (*)(C(DGGRS), C(DGGRSZone), C(DGGRSZone)))METHOD(DGGRS, isZoneImmediateChildOf)->function;

         METHOD(DGGRS, isZoneImmediateParentOf) = Class_findMethod(CO(DGGRS), "isZoneImmediateParentOf", module);
         if(METHOD(DGGRS, isZoneImmediateParentOf))
            DGGRS_isZoneImmediateParentOf = (C(bool) (*)(C(DGGRS), C(DGGRSZone), C(DGGRSZone)))METHOD(DGGRS, isZoneImmediateParentOf)->function;

         METHOD(DGGRS, listZones) = Class_findMethod(CO(DGGRS), "listZones", module);
         if(METHOD(DGGRS, listZones))
            M_VTBLID(DGGRS, listZones) = METHOD(DGGRS, listZones)->vid;

         METHOD(DGGRS, zoneHasSubZone) = Class_findMethod(CO(DGGRS), "zoneHasSubZone", module);
         if(METHOD(DGGRS, zoneHasSubZone))
            DGGRS_zoneHasSubZone = (C(bool) (*)(C(DGGRS), C(DGGRSZone), C(DGGRSZone)))METHOD(DGGRS, zoneHasSubZone)->function;
      }
      CO(DGGRSZone) = eC_findClass(module, "DGGRSZone");
      CO(DGGSJSON) = eC_findClass(module, "DGGSJSON");
      CO(DGGSJSONDepth) = eC_findClass(module, "DGGSJSONDepth");
      CO(DGGSJSONGrid) = eC_findClass(module, "DGGSJSONGrid");
      CO(DGGSJSONShape) = eC_findClass(module, "DGGSJSONShape");
      CO(GGGZone) = eC_findClass(module, "GGGZone");
      CO(GNOSISGlobalGrid) = eC_findClass(module, "GNOSISGlobalGrid");
      CO(GPP3H) = eC_findClass(module, "GPP3H");
      CO(GeoExtent) = eC_findClass(module, "GeoExtent");
      if(CO(GeoExtent))
      {
         METHOD(GeoExtent, clear) = Class_findMethod(CO(GeoExtent), "clear", module);
         if(METHOD(GeoExtent, clear))
            GeoExtent_clear = (void (*)(C(GeoExtent) *))METHOD(GeoExtent, clear)->function;

         METHOD(GeoExtent, intersects) = Class_findMethod(CO(GeoExtent), "intersects", module);
         if(METHOD(GeoExtent, intersects))
            GeoExtent_intersects = (C(bool) (*)(C(GeoExtent) *, const C(GeoExtent) *))METHOD(GeoExtent, intersects)->function;

         PROPERTY(GeoExtent, geodeticArea) = Class_findProperty(CO(GeoExtent), "geodeticArea", module);
         if(PROPERTY(GeoExtent, geodeticArea))
            GeoExtent_get_geodeticArea = (void *)PROPERTY(GeoExtent, geodeticArea)->Get;
      }
      CO(GeoPoint) = eC_findClass(module, "GeoPoint");
      CO(I3HZone) = eC_findClass(module, "I3HZone");
      CO(I9RZone) = eC_findClass(module, "I9RZone");
      CO(ISEA3H) = eC_findClass(module, "ISEA3H");
      CO(ISEA9R) = eC_findClass(module, "ISEA9R");
      CO(IVEA3H) = eC_findClass(module, "IVEA3H");
      CO(IVEA9R) = eC_findClass(module, "IVEA9R");
      CO(RTEA3H) = eC_findClass(module, "RTEA3H");
      CO(RTEA9R) = eC_findClass(module, "RTEA9R");
      CO(rHEALPix) = eC_findClass(module, "rHEALPix");
      CO(JSONSchema) = eC_findClass(module, "JSONSchema");
      if(CO(JSONSchema))
      {
         PROPERTY(JSONSchema, maximum) = Class_findProperty(CO(JSONSchema), "maximum", module);
         if(PROPERTY(JSONSchema, maximum))
         {
            JSONSchema_get_maximum = (void *)PROPERTY(JSONSchema, maximum)->Get;
            JSONSchema_isSet_maximum = (void *)PROPERTY(JSONSchema, maximum)->IsSet;
         }

         PROPERTY(JSONSchema, exclusiveMaximum) = Class_findProperty(CO(JSONSchema), "exclusiveMaximum", module);
         if(PROPERTY(JSONSchema, exclusiveMaximum))
         {
            JSONSchema_get_exclusiveMaximum = (void *)PROPERTY(JSONSchema, exclusiveMaximum)->Get;
            JSONSchema_isSet_exclusiveMaximum = (void *)PROPERTY(JSONSchema, exclusiveMaximum)->IsSet;
         }

         PROPERTY(JSONSchema, minimum) = Class_findProperty(CO(JSONSchema), "minimum", module);
         if(PROPERTY(JSONSchema, minimum))
         {
            JSONSchema_get_minimum = (void *)PROPERTY(JSONSchema, minimum)->Get;
            JSONSchema_isSet_minimum = (void *)PROPERTY(JSONSchema, minimum)->IsSet;
         }

         PROPERTY(JSONSchema, exclusiveMinimum) = Class_findProperty(CO(JSONSchema), "exclusiveMinimum", module);
         if(PROPERTY(JSONSchema, exclusiveMinimum))
         {
            JSONSchema_get_exclusiveMinimum = (void *)PROPERTY(JSONSchema, exclusiveMinimum)->Get;
            JSONSchema_isSet_exclusiveMinimum = (void *)PROPERTY(JSONSchema, exclusiveMinimum)->IsSet;
         }

         PROPERTY(JSONSchema, maxItems) = Class_findProperty(CO(JSONSchema), "maxItems", module);
         if(PROPERTY(JSONSchema, maxItems))
         {
            JSONSchema_get_maxItems = (void *)PROPERTY(JSONSchema, maxItems)->Get;
            JSONSchema_isSet_maxItems = (void *)PROPERTY(JSONSchema, maxItems)->IsSet;
         }

         PROPERTY(JSONSchema, minItems) = Class_findProperty(CO(JSONSchema), "minItems", module);
         if(PROPERTY(JSONSchema, minItems))
         {
            JSONSchema_get_minItems = (void *)PROPERTY(JSONSchema, minItems)->Get;
            JSONSchema_isSet_minItems = (void *)PROPERTY(JSONSchema, minItems)->IsSet;
         }

         PROPERTY(JSONSchema, maxProperties) = Class_findProperty(CO(JSONSchema), "maxProperties", module);
         if(PROPERTY(JSONSchema, maxProperties))
         {
            JSONSchema_get_maxProperties = (void *)PROPERTY(JSONSchema, maxProperties)->Get;
            JSONSchema_isSet_maxProperties = (void *)PROPERTY(JSONSchema, maxProperties)->IsSet;
         }

         PROPERTY(JSONSchema, minProperties) = Class_findProperty(CO(JSONSchema), "minProperties", module);
         if(PROPERTY(JSONSchema, minProperties))
         {
            JSONSchema_get_minProperties = (void *)PROPERTY(JSONSchema, minProperties)->Get;
            JSONSchema_isSet_minProperties = (void *)PROPERTY(JSONSchema, minProperties)->IsSet;
         }

         PROPERTY(JSONSchema, xogcpropertySeq) = Class_findProperty(CO(JSONSchema), "xogcpropertySeq", module);
         if(PROPERTY(JSONSchema, xogcpropertySeq))
            JSONSchema_isSet_xogcpropertySeq = (void *)PROPERTY(JSONSchema, xogcpropertySeq)->IsSet;

         PROPERTY(JSONSchema, Default) = Class_findProperty(CO(JSONSchema), "Default", module);
         if(PROPERTY(JSONSchema, Default))
            JSONSchema_isSet_Default = (void *)PROPERTY(JSONSchema, Default)->IsSet;
      }
      CO(JSONSchemaType) = eC_findClass(module, "JSONSchemaType");
      CO(Plane) = eC_findClass(module, "Plane");
      if(CO(Plane))
      {
         METHOD(Plane, fromPoints) = Class_findMethod(CO(Plane), "FromPoints", module);
         if(METHOD(Plane, fromPoints))
            Plane_fromPoints = (void (*)(C(Plane) *, const C(Vector3D) *, const C(Vector3D) *, const C(Vector3D) *))METHOD(Plane, fromPoints)->function;
      }
      CO(RhombicIcosahedral3H) = eC_findClass(module, "RhombicIcosahedral3H");
      CO(RhombicIcosahedral9R) = eC_findClass(module, "RhombicIcosahedral9R");
      CO(Vector3D) = eC_findClass(module, "Vector3D");
      if(CO(Vector3D))
      {
         METHOD(Vector3D, crossProduct) = Class_findMethod(CO(Vector3D), "CrossProduct", module);
         if(METHOD(Vector3D, crossProduct))
            Vector3D_crossProduct = (void (*)(C(Vector3D) *, const C(Vector3D) *, const C(Vector3D) *))METHOD(Vector3D, crossProduct)->function;

         METHOD(Vector3D, dotProduct) = Class_findMethod(CO(Vector3D), "DotProduct", module);
         if(METHOD(Vector3D, dotProduct))
            Vector3D_dotProduct = (double (*)(C(Vector3D) *, const C(Vector3D) *))METHOD(Vector3D, dotProduct)->function;

         METHOD(Vector3D, normalize) = Class_findMethod(CO(Vector3D), "Normalize", module);
         if(METHOD(Vector3D, normalize))
            Vector3D_normalize = (void (*)(C(Vector3D) *, const C(Vector3D) *))METHOD(Vector3D, normalize)->function;

         METHOD(Vector3D, subtract) = Class_findMethod(CO(Vector3D), "Subtract", module);
         if(METHOD(Vector3D, subtract))
            Vector3D_subtract = (void (*)(C(Vector3D) *, const C(Vector3D) *, const C(Vector3D) *))METHOD(Vector3D, subtract)->function;

         PROPERTY(Vector3D, length) = Class_findProperty(CO(Vector3D), "length", module);
         if(PROPERTY(Vector3D, length))
            Vector3D_get_length = (void *)PROPERTY(Vector3D, length)->Get;
      }



         // Set up all the function pointers, ...

      FUNCTION(readDGGSJSON) = eC_findFunction(module, "readDGGSJSON");
      if(FUNCTION(readDGGSJSON))
         F(readDGGSJSON) = (void *)FUNCTION(readDGGSJSON)->function;

   }
   else
      printf("Unable to load eC module: %s\n", DGGAL_MODULE_NAME);
   return module;
}
