typedef uint64 eC_CRS;
struct eC_CRSExtent
{
   eC_CRS crs;
   eC_Pointd tl;
   eC_Pointd br;
};

struct eC_GeoPoint
{
   eC_Angle lat;
   eC_Angle lon;
};
struct eC_Vector3D
{
   double x;
   double y;
   double z;
};
typedef eC_Instance eC_RI5x6Projection;
typedef eC_RI5x6Projection eC_BarycentricSphericalTriAreaProjection;
typedef struct eC_CRSExtent eC_CRSExtent;
typedef eC_Instance eC_DGGRS;
typedef uint64 eC_DGGRSZone;
typedef eC_Instance eC_DGGSJSON;
typedef eC_Instance eC_DGGSJSONGrid;
typedef eC_Instance eC_DGGSJSONShape;
typedef struct eC_GeoExtent eC_GeoExtent;
typedef struct eC_GeoPoint eC_GeoPoint;
typedef eC_Instance eC_HEALPixProjection;
typedef uint64 eC_I3HZone;
typedef uint64 eC_I7HZone;
typedef uint64 eC_I9RZone;
typedef eC_Instance eC_JSONSchema;
typedef int eC_JSONSchemaType;
enum
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

typedef struct eC_Plane eC_Plane;
typedef struct eC_Quaternion eC_Quaternion;
typedef eC_DGGRS eC_RhombicIcosahedral7H;
typedef eC_RhombicIcosahedral7H eC_RI7H_Z7;
typedef eC_DGGRS eC_RhombicIcosahedral3H;
typedef eC_DGGRS eC_RhombicIcosahedral4R;
typedef eC_DGGRS eC_RhombicIcosahedral9R;
typedef eC_RI5x6Projection eC_SliceAndDiceGreatCircleIcosahedralProjection;
typedef struct eC_Vector3D eC_Vector3D;
typedef uint64 eC_Z7Zone;
#define AUTH_ORDER 6

static const uint64 nullZone;

static const double wgs84InvFlattening;

static const eC_Distance wgs84Minor;

typedef eC_RhombicIcosahedral3H eC_BCTA3H;
typedef int eC_CRSRegistry;
enum
{
   CRSRegistry_epsg = 0x0,
   CRSRegistry_ogc = 0x1
};

typedef eC_Instance eC_DGGSJSONDepth;
typedef eC_Instance eC_DGGSJSONDimension;
typedef uint64 eC_GGGZone;
typedef eC_DGGRS eC_GNOSISGlobalGrid;
typedef eC_RhombicIcosahedral3H eC_GPP3H;
typedef eC_BarycentricSphericalTriAreaProjection eC_GoldbergPolyhedraProjection;
typedef eC_DGGRS eC_HEALPix;
typedef uint64 eC_HPZone;
typedef int eC_I3HNeighbor;
enum
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

typedef uint64 eC_I4RZone;
typedef eC_RhombicIcosahedral3H eC_ISEA3H;
typedef eC_RhombicIcosahedral4R eC_ISEA4R;
typedef eC_RhombicIcosahedral7H eC_ISEA7H;
typedef eC_RI7H_Z7 eC_ISEA7H_Z7;
typedef eC_RhombicIcosahedral9R eC_ISEA9R;
typedef eC_SliceAndDiceGreatCircleIcosahedralProjection eC_ISEAProjection;
typedef eC_RhombicIcosahedral3H eC_IVEA3H;
typedef eC_RhombicIcosahedral4R eC_IVEA4R;
typedef eC_RhombicIcosahedral7H eC_IVEA7H;
typedef eC_RI7H_Z7 eC_IVEA7H_Z7;
typedef eC_RhombicIcosahedral9R eC_IVEA9R;
typedef eC_SliceAndDiceGreatCircleIcosahedralProjection eC_IVEAProjection;
typedef uint64 eC_RHPZone;
typedef eC_RhombicIcosahedral3H eC_RTEA3H;
typedef eC_RhombicIcosahedral4R eC_RTEA4R;
typedef eC_RhombicIcosahedral7H eC_RTEA7H;
typedef eC_RI7H_Z7 eC_RTEA7H_Z7;
typedef eC_RhombicIcosahedral9R eC_RTEA9R;
typedef eC_SliceAndDiceGreatCircleIcosahedralProjection eC_RTEAProjection;
typedef int eC_VGCRadialVertex;
enum
{
   VGCRadialVertex_isea = 0x0,
   VGCRadialVertex_ivea = 0x1,
   VGCRadialVertex_rtea = 0x2
};

typedef eC_DGGRS eC_rHEALPix;
typedef eC_HEALPixProjection eC_rHEALPixProjection;
typedef eC_Array template_Array_JSONSchema;
typedef eC_Map template_Map_String_JSONSchema;
typedef eC_Array template_Array_String;
typedef eC_Array template_Array_FieldValue;
typedef eC_Array template_Array_double;
typedef eC_Map template_Map_String_int;
typedef eC_Array template_Array_DGGSJSONDepth;
typedef eC_Map template_Map_String_template_Array_DGGSJSONDepth;
typedef eC_Array template_Array_DGGSJSONDimension;
typedef eC_Array template_Array_int;
typedef eC_Array template_Array_DGGRSZone;
typedef eC_Array template_Array_GeoPoint;
typedef eC_Array template_Array_Pointd;
#define CRS_registry_SHIFT                               0
#define CRS_registry_MASK                                0x3FFFFFFF
#define CRS_crsID_SHIFT                                  30
#define CRS_crsID_MASK                                   0x3FFFFFFFC0000000LL
#define CRS_h_SHIFT                                      62
#define CRS_h_MASK                                       0x4000000000000000LL


extern eC_bool (* DGGRS_areZonesNeighbors)(eC_DGGRS __this, eC_DGGRSZone a, eC_DGGRSZone b);

extern eC_bool (* DGGRS_areZonesSiblings)(eC_DGGRS __this, eC_DGGRSZone a, eC_DGGRSZone b);

extern int DGGRS_compactZones_vTblID;
void DGGRS_compactZones(eC_DGGRS __i, eC_Array zones);
extern eC_Method * method_DGGRS_compactZones;

extern int DGGRS_countSubZones_vTblID;
uint64 DGGRS_countSubZones(eC_DGGRS __i, eC_DGGRSZone zone, int depth);
extern eC_Method * method_DGGRS_countSubZones;

extern int DGGRS_countZoneEdges_vTblID;
int DGGRS_countZoneEdges(eC_DGGRS __i, eC_DGGRSZone zone);
extern eC_Method * method_DGGRS_countZoneEdges;

extern int DGGRS_countZones_vTblID;
uint64 DGGRS_countZones(eC_DGGRS __i, int level);
extern eC_Method * method_DGGRS_countZones;

extern eC_bool (* DGGRS_doZonesOverlap)(eC_DGGRS __this, eC_DGGRSZone a, eC_DGGRSZone b);

extern eC_bool (* DGGRS_doesZoneContain)(eC_DGGRS __this, eC_DGGRSZone hayStack, eC_DGGRSZone needle);

extern int (* DGGRS_get64KDepth)(eC_DGGRS __this);

extern int DGGRS_getFirstSubZone_vTblID;
eC_DGGRSZone DGGRS_getFirstSubZone(eC_DGGRS __i, eC_DGGRSZone zone, int relativeDepth);
extern eC_Method * method_DGGRS_getFirstSubZone;

extern int DGGRS_getIndexMaxDepth_vTblID;
int DGGRS_getIndexMaxDepth(eC_DGGRS __i);
extern eC_Method * method_DGGRS_getIndexMaxDepth;

extern int (* DGGRS_getLevelFromMetersPerSubZone)(eC_DGGRS __this, double physicalMetersPerSubZone, int relativeDepth);

extern int (* DGGRS_getLevelFromPixelsAndExtent)(eC_DGGRS __this, const eC_GeoExtent * extent, const eC_Point * pixels, int relativeDepth);

extern int (* DGGRS_getLevelFromRefZoneArea)(eC_DGGRS __this, double metersSquared);

extern int (* DGGRS_getLevelFromScaleDenominator)(eC_DGGRS __this, double scaleDenominator, int relativeDepth, double mmPerPixel);

extern int DGGRS_getMaxChildren_vTblID;
int DGGRS_getMaxChildren(eC_DGGRS __i);
extern eC_Method * method_DGGRS_getMaxChildren;

extern int DGGRS_getMaxDGGRSZoneLevel_vTblID;
int DGGRS_getMaxDGGRSZoneLevel(eC_DGGRS __i);
extern eC_Method * method_DGGRS_getMaxDGGRSZoneLevel;

extern int (* DGGRS_getMaxDepth)(eC_DGGRS __this);

extern int DGGRS_getMaxNeighbors_vTblID;
int DGGRS_getMaxNeighbors(eC_DGGRS __i);
extern eC_Method * method_DGGRS_getMaxNeighbors;

extern int DGGRS_getMaxParents_vTblID;
int DGGRS_getMaxParents(eC_DGGRS __i);
extern eC_Method * method_DGGRS_getMaxParents;

extern double (* DGGRS_getMetersPerSubZoneFromLevel)(eC_DGGRS __this, int parentLevel, int relativeDepth);

extern double (* DGGRS_getRefZoneArea)(eC_DGGRS __this, int level);

extern int DGGRS_getRefinementRatio_vTblID;
int DGGRS_getRefinementRatio(eC_DGGRS __i);
extern eC_Method * method_DGGRS_getRefinementRatio;

extern double (* DGGRS_getScaleDenominatorFromLevel)(eC_DGGRS __this, int parentLevel, int relativeDepth, double mmPerPixel);

extern int DGGRS_getSubZoneAtIndex_vTblID;
eC_DGGRSZone DGGRS_getSubZoneAtIndex(eC_DGGRS __i, eC_DGGRSZone parent, int relativeDepth, int64 index);
extern eC_Method * method_DGGRS_getSubZoneAtIndex;

extern int DGGRS_getSubZoneCRSCentroids_vTblID;
eC_Array DGGRS_getSubZoneCRSCentroids(eC_DGGRS __i, eC_DGGRSZone parent, eC_CRS crs, int relativeDepth);
extern eC_Method * method_DGGRS_getSubZoneCRSCentroids;

extern int DGGRS_getSubZoneIndex_vTblID;
int64 DGGRS_getSubZoneIndex(eC_DGGRS __i, eC_DGGRSZone parent, eC_DGGRSZone subZone);
extern eC_Method * method_DGGRS_getSubZoneIndex;

extern int DGGRS_getSubZoneWGS84Centroids_vTblID;
eC_Array DGGRS_getSubZoneWGS84Centroids(eC_DGGRS __i, eC_DGGRSZone parent, int relativeDepth);
extern eC_Method * method_DGGRS_getSubZoneWGS84Centroids;

extern int DGGRS_getSubZones_vTblID;
eC_Array DGGRS_getSubZones(eC_DGGRS __i, eC_DGGRSZone parent, int relativeDepth);
extern eC_Method * method_DGGRS_getSubZones;

extern int DGGRS_getZoneArea_vTblID;
double DGGRS_getZoneArea(eC_DGGRS __i, eC_DGGRSZone zone);
extern eC_Method * method_DGGRS_getZoneArea;

extern int DGGRS_getZoneCRSCentroid_vTblID;
void DGGRS_getZoneCRSCentroid(eC_DGGRS __i, eC_DGGRSZone zone, eC_CRS crs, eC_Pointd * centroid);
extern eC_Method * method_DGGRS_getZoneCRSCentroid;

extern int DGGRS_getZoneCRSExtent_vTblID;
void DGGRS_getZoneCRSExtent(eC_DGGRS __i, eC_DGGRSZone zone, eC_CRS crs, eC_CRSExtent * extent);
extern eC_Method * method_DGGRS_getZoneCRSExtent;

extern int DGGRS_getZoneCRSVertices_vTblID;
int DGGRS_getZoneCRSVertices(eC_DGGRS __i, eC_DGGRSZone zone, eC_CRS crs, eC_Pointd * vertices);
extern eC_Method * method_DGGRS_getZoneCRSVertices;

extern int DGGRS_getZoneCentroidChild_vTblID;
eC_DGGRSZone DGGRS_getZoneCentroidChild(eC_DGGRS __i, eC_DGGRSZone zone);
extern eC_Method * method_DGGRS_getZoneCentroidChild;

extern int DGGRS_getZoneCentroidParent_vTblID;
eC_DGGRSZone DGGRS_getZoneCentroidParent(eC_DGGRS __i, eC_DGGRSZone zone);
extern eC_Method * method_DGGRS_getZoneCentroidParent;

extern int DGGRS_getZoneChildren_vTblID;
int DGGRS_getZoneChildren(eC_DGGRS __i, eC_DGGRSZone zone, eC_DGGRSZone * children);
extern eC_Method * method_DGGRS_getZoneChildren;

extern int DGGRS_getZoneFromCRSCentroid_vTblID;
eC_DGGRSZone DGGRS_getZoneFromCRSCentroid(eC_DGGRS __i, int level, eC_CRS crs, const eC_Pointd * centroid);
extern eC_Method * method_DGGRS_getZoneFromCRSCentroid;

extern int DGGRS_getZoneFromTextID_vTblID;
eC_DGGRSZone DGGRS_getZoneFromTextID(eC_DGGRS __i, constString zoneID);
extern eC_Method * method_DGGRS_getZoneFromTextID;

extern int DGGRS_getZoneFromWGS84Centroid_vTblID;
eC_DGGRSZone DGGRS_getZoneFromWGS84Centroid(eC_DGGRS __i, int level, const eC_GeoPoint * centroid);
extern eC_Method * method_DGGRS_getZoneFromWGS84Centroid;

extern int DGGRS_getZoneLevel_vTblID;
int DGGRS_getZoneLevel(eC_DGGRS __i, eC_DGGRSZone zone);
extern eC_Method * method_DGGRS_getZoneLevel;

extern int DGGRS_getZoneNeighbors_vTblID;
int DGGRS_getZoneNeighbors(eC_DGGRS __i, eC_DGGRSZone zone, eC_DGGRSZone * neighbors, int * nbType);
extern eC_Method * method_DGGRS_getZoneNeighbors;

extern int DGGRS_getZoneParents_vTblID;
int DGGRS_getZoneParents(eC_DGGRS __i, eC_DGGRSZone zone, eC_DGGRSZone * parents);
extern eC_Method * method_DGGRS_getZoneParents;

extern int DGGRS_getZoneRefinedCRSVertices_vTblID;
eC_Array DGGRS_getZoneRefinedCRSVertices(eC_DGGRS __i, eC_DGGRSZone zone, eC_CRS crs, int edgeRefinement);
extern eC_Method * method_DGGRS_getZoneRefinedCRSVertices;

extern int DGGRS_getZoneRefinedWGS84Vertices_vTblID;
eC_Array DGGRS_getZoneRefinedWGS84Vertices(eC_DGGRS __i, eC_DGGRSZone zone, int edgeRefinement);
extern eC_Method * method_DGGRS_getZoneRefinedWGS84Vertices;

extern int DGGRS_getZoneTextID_vTblID;
void DGGRS_getZoneTextID(eC_DGGRS __i, eC_DGGRSZone zone, eC_String zoneID);
extern eC_Method * method_DGGRS_getZoneTextID;

extern int DGGRS_getZoneWGS84Centroid_vTblID;
void DGGRS_getZoneWGS84Centroid(eC_DGGRS __i, eC_DGGRSZone zone, eC_GeoPoint * centroid);
extern eC_Method * method_DGGRS_getZoneWGS84Centroid;

extern int DGGRS_getZoneWGS84Extent_vTblID;
void DGGRS_getZoneWGS84Extent(eC_DGGRS __i, eC_DGGRSZone zone, eC_GeoExtent * extent);
extern eC_Method * method_DGGRS_getZoneWGS84Extent;

extern int DGGRS_getZoneWGS84ExtentApproximate_vTblID;
void DGGRS_getZoneWGS84ExtentApproximate(eC_DGGRS __i, eC_DGGRSZone zone, eC_GeoExtent * extent);
extern eC_Method * method_DGGRS_getZoneWGS84ExtentApproximate;

extern int DGGRS_getZoneWGS84Vertices_vTblID;
int DGGRS_getZoneWGS84Vertices(eC_DGGRS __i, eC_DGGRSZone zone, eC_GeoPoint * vertices);
extern eC_Method * method_DGGRS_getZoneWGS84Vertices;

extern eC_bool (* DGGRS_isZoneAncestorOf)(eC_DGGRS __this, eC_DGGRSZone ancestor, eC_DGGRSZone descendant, int maxDepth);

extern int DGGRS_isZoneCentroidChild_vTblID;
eC_bool DGGRS_isZoneCentroidChild(eC_DGGRS __i, eC_DGGRSZone zone);
extern eC_Method * method_DGGRS_isZoneCentroidChild;

extern eC_bool (* DGGRS_isZoneContainedIn)(eC_DGGRS __this, eC_DGGRSZone needle, eC_DGGRSZone hayStack);

extern eC_bool (* DGGRS_isZoneDescendantOf)(eC_DGGRS __this, eC_DGGRSZone descendant, eC_DGGRSZone ancestor, int maxDepth);

extern eC_bool (* DGGRS_isZoneImmediateChildOf)(eC_DGGRS __this, eC_DGGRSZone child, eC_DGGRSZone parent);

extern eC_bool (* DGGRS_isZoneImmediateParentOf)(eC_DGGRS __this, eC_DGGRSZone parent, eC_DGGRSZone child);

extern int DGGRS_listZones_vTblID;
eC_Array DGGRS_listZones(eC_DGGRS __i, int level, const eC_GeoExtent * bbox);
extern eC_Method * method_DGGRS_listZones;

extern int DGGRS_zoneHasSubZone_vTblID;
eC_bool DGGRS_zoneHasSubZone(eC_DGGRS __i, eC_DGGRSZone hayStack, eC_DGGRSZone needle);
extern eC_Method * method_DGGRS_zoneHasSubZone;

#define DGGRSZONE_level_SHIFT                            59
#define DGGRSZONE_level_MASK                             0xF800000000000000LL
#define DGGRSZONE_row_SHIFT                              30
#define DGGRSZONE_row_MASK                               0x7FFFFFFC0000000LL
#define DGGRSZONE_col_SHIFT                              0
#define DGGRSZONE_col_MASK                               0x3FFFFFFF


struct class_members_DGGSJSON
{
   eC_String dggrs;
   eC_String zoneId;
   eC_Array depths;
   eC_String representedValue;
   eC_JSONSchema schema;
   eC_Array dimensions;
   eC_Map values;
};
struct class_members_DGGSJSONDepth
{
   int depth;
   eC_DGGSJSONShape shape;
   eC_Array data;
};
struct class_members_DGGSJSONDimension
{
   eC_String name;
   eC_Array interval;
   eC_DGGSJSONGrid grid;
   eC_String definition;
   eC_String unit;
   eC_String unitLang;
};
struct class_members_DGGSJSONGrid
{
   int cellsCount;
   double resolution;
   eC_Array coordinates;
   eC_Array boundsCoordinates;
   eC_Array relativeBounds;
   eC_FieldValue firstCoordinate;
};
struct class_members_DGGSJSONShape
{
   int count;
   int subZones;
   eC_Map dimensions;
};
#define GGGZONE_level_SHIFT                              59
#define GGGZONE_level_MASK                               0xF800000000000000LL
#define GGGZONE_row_SHIFT                                30
#define GGGZONE_row_MASK                                 0x7FFFFFFC0000000LL
#define GGGZONE_col_SHIFT                                0
#define GGGZONE_col_MASK                                 0x3FFFFFFF


struct eC_GeoExtent
{
   eC_GeoPoint ll;
   eC_GeoPoint ur;
};
extern void (* GeoExtent_clear)(eC_GeoExtent * __this);

extern eC_bool (* GeoExtent_clip)(eC_GeoExtent * __this, const eC_GeoExtent * e, const eC_GeoExtent * clipExtent);

extern eC_bool (* GeoExtent_clipHandlingDateline)(eC_GeoExtent * __this, const eC_GeoExtent * e, const eC_GeoExtent * clipExtent);

extern void (* GeoExtent_doUnionDL)(eC_GeoExtent * __this, const eC_GeoExtent * e);

extern eC_bool (* GeoExtent_intersects)(eC_GeoExtent * __this, const eC_GeoExtent * b);

extern eC_Property * property_GeoExtent_nonNull;
extern eC_bool (* GeoExtent_get_nonNull)(const eC_GeoExtent * g);

extern eC_Property * property_GeoExtent_geodeticArea;
extern double (* GeoExtent_get_geodeticArea)(const eC_GeoExtent * g);

extern int HEALPixProjection_forward_vTblID;
eC_bool HEALPixProjection_forward(eC_HEALPixProjection __i, const eC_GeoPoint * p, eC_Pointd * v);
extern eC_Method * method_HEALPixProjection_forward;

extern int HEALPixProjection_inverse_vTblID;
eC_bool HEALPixProjection_inverse(eC_HEALPixProjection __i, const eC_Pointd * v, eC_GeoPoint * result, eC_bool oddGrid);
extern eC_Method * method_HEALPixProjection_inverse;

#define HPZONE_level_SHIFT                               56
#define HPZONE_level_MASK                                0x1F00000000000000LL
#define HPZONE_rootRhombus_SHIFT                         52
#define HPZONE_rootRhombus_MASK                          0xF0000000000000LL
#define HPZONE_subIndex_SHIFT                            0
#define HPZONE_subIndex_MASK                             0xFFFFFFFFFFFFFLL


#define I3HZONE_levelI9R_SHIFT                           57
#define I3HZONE_levelI9R_MASK                            0x3E00000000000000LL
#define I3HZONE_rootRhombus_SHIFT                        53
#define I3HZONE_rootRhombus_MASK                         0x1E0000000000000LL
#define I3HZONE_rhombusIX_SHIFT                          2
#define I3HZONE_rhombusIX_MASK                           0x1FFFFFFFFFFFFCLL
#define I3HZONE_subHex_SHIFT                             0
#define I3HZONE_subHex_MASK                              0x3


#define I4RZONE_level_SHIFT                              59
#define I4RZONE_level_MASK                               0xF800000000000000LL
#define I4RZONE_row_SHIFT                                30
#define I4RZONE_row_MASK                                 0x7FFFFFFC0000000LL
#define I4RZONE_col_SHIFT                                0
#define I4RZONE_col_MASK                                 0x3FFFFFFF


#define I7HZONE_levelI49R_SHIFT                          58
#define I7HZONE_levelI49R_MASK                           0x3C00000000000000LL
#define I7HZONE_rootRhombus_SHIFT                        54
#define I7HZONE_rootRhombus_MASK                         0x3C0000000000000LL
#define I7HZONE_rhombusIX_SHIFT                          3
#define I7HZONE_rhombusIX_MASK                           0x3FFFFFFFFFFFF8LL
#define I7HZONE_subHex_SHIFT                             0
#define I7HZONE_subHex_MASK                              0x7


#define I9RZONE_level_SHIFT                              59
#define I9RZONE_level_MASK                               0xF800000000000000LL
#define I9RZONE_row_SHIFT                                30
#define I9RZONE_row_MASK                                 0x7FFFFFFC0000000LL
#define I9RZONE_col_SHIFT                                0
#define I9RZONE_col_MASK                                 0x3FFFFFFF


struct class_members_JSONSchema
{
   eC_String schema;
   eC_String id;
   eC_String title;
   eC_String comment;
   eC_String description;
   eC_FieldValue Default;
   eC_bool readOnly;
   eC_bool writeOnly;
   eC_Array examples;
   eC_Array multipleOf;
   eC_JSONSchemaType type;
   eC_Array Enum;
   eC_String format;
   eC_String contentMediaType;
   double maximum;
   double exclusiveMaximum;
   double minimum;
   double exclusiveMinimum;
   eC_String pattern;
   eC_JSONSchema items;
   int maxItems;
   int minItems;
   eC_bool uniqueItems;
   eC_String contains;
   int maxProperties;
   int minProperties;
   eC_Array required;
   eC_JSONSchema additionalProperties;
   eC_Map definitions;
   eC_Map properties;
   eC_Map patternProperties;
   eC_Map dependencies;
   eC_String propertyNames;
   eC_String contentEncoding;
   eC_JSONSchema If;
   eC_JSONSchema Then;
   eC_JSONSchema Else;
   eC_Array allOf;
   eC_Array anyOf;
   eC_Array oneOf;
   eC_JSONSchema Not;
   eC_String xogcrole;
   int xogcpropertySeq;
};
extern eC_Property * property_JSONSchema_maximum;
extern double (* JSONSchema_get_maximum)(const eC_JSONSchema j);
extern eC_bool (* JSONSchema_isSet_maximum)(const eC_JSONSchema j);

extern eC_Property * property_JSONSchema_exclusiveMaximum;
extern double (* JSONSchema_get_exclusiveMaximum)(const eC_JSONSchema j);
extern eC_bool (* JSONSchema_isSet_exclusiveMaximum)(const eC_JSONSchema j);

extern eC_Property * property_JSONSchema_minimum;
extern double (* JSONSchema_get_minimum)(const eC_JSONSchema j);
extern eC_bool (* JSONSchema_isSet_minimum)(const eC_JSONSchema j);

extern eC_Property * property_JSONSchema_exclusiveMinimum;
extern double (* JSONSchema_get_exclusiveMinimum)(const eC_JSONSchema j);
extern eC_bool (* JSONSchema_isSet_exclusiveMinimum)(const eC_JSONSchema j);

extern eC_Property * property_JSONSchema_maxItems;
extern int (* JSONSchema_get_maxItems)(const eC_JSONSchema j);
extern eC_bool (* JSONSchema_isSet_maxItems)(const eC_JSONSchema j);

extern eC_Property * property_JSONSchema_minItems;
extern int (* JSONSchema_get_minItems)(const eC_JSONSchema j);
extern eC_bool (* JSONSchema_isSet_minItems)(const eC_JSONSchema j);

extern eC_Property * property_JSONSchema_maxProperties;
extern int (* JSONSchema_get_maxProperties)(const eC_JSONSchema j);
extern eC_bool (* JSONSchema_isSet_maxProperties)(const eC_JSONSchema j);

extern eC_Property * property_JSONSchema_minProperties;
extern int (* JSONSchema_get_minProperties)(const eC_JSONSchema j);
extern eC_bool (* JSONSchema_isSet_minProperties)(const eC_JSONSchema j);

extern eC_Property * property_JSONSchema_xogcpropertySeq;
extern eC_bool (* JSONSchema_isSet_xogcpropertySeq)(const eC_JSONSchema j);

extern eC_Property * property_JSONSchema_Default;
extern eC_bool (* JSONSchema_isSet_Default)(const eC_JSONSchema j);

struct eC_Plane
{
   union
   {
      struct
      {
         double a;
         double b;
         double c;
      };
      eC_Vector3D normal;
   };
   double d;
};
extern void (* Plane_fromPoints)(eC_Plane * __this, const eC_Vector3D * v1, const eC_Vector3D * v2, const eC_Vector3D * v3);

struct eC_Quaternion
{
   double w;
   double x;
   double y;
   double z;
};
extern void (* Quaternion_yawPitch)(eC_Quaternion * __this, eC_Angle yaw, eC_Angle pitch);

#define RHPZONE_level_SHIFT                              59
#define RHPZONE_level_MASK                               0xF800000000000000LL
#define RHPZONE_row_SHIFT                                30
#define RHPZONE_row_MASK                                 0x7FFFFFFC0000000LL
#define RHPZONE_col_SHIFT                                0
#define RHPZONE_col_MASK                                 0x3FFFFFFF


extern void (* RI5x6Projection_extent5x6FromWGS84)(eC_RI5x6Projection __this, const eC_GeoExtent * wgs84Extent, eC_Pointd * topLeft, eC_Pointd * bottomRight);

extern int RI5x6Projection_forward_vTblID;
eC_bool RI5x6Projection_forward(eC_RI5x6Projection __i, const eC_GeoPoint * p, eC_Pointd * v);
extern eC_Method * method_RI5x6Projection_forward;

extern eC_bool (* RI5x6Projection_fromIcosahedronNet)(const eC_Pointd * v, eC_Pointd * result);

extern int RI5x6Projection_inverse_vTblID;
eC_bool RI5x6Projection_inverse(eC_RI5x6Projection __i, const eC_Pointd * _v, eC_GeoPoint * result, eC_bool oddGrid);
extern eC_Method * method_RI5x6Projection_inverse;

extern eC_bool (* RI5x6Projection_toIcosahedronNet)(const eC_Pointd * v, eC_Pointd * result);

extern void (* Vector3D_crossProduct)(eC_Vector3D * __this, const eC_Vector3D * vector1, const eC_Vector3D * vector2);

extern double (* Vector3D_dotProduct)(eC_Vector3D * __this, const eC_Vector3D * vector2);

extern void (* Vector3D_multQuaternion)(eC_Vector3D * __this, const eC_Vector3D * s, const eC_Quaternion * quat);

extern void (* Vector3D_normalize)(eC_Vector3D * __this, const eC_Vector3D * source);

extern void (* Vector3D_subtract)(eC_Vector3D * __this, const eC_Vector3D * vector1, const eC_Vector3D * vector2);

extern eC_Property * property_Vector3D_length;
extern double (* Vector3D_get_length)(const eC_Vector3D * v);

#define Z7ZONE_rootPentagon_SHIFT                        60
#define Z7ZONE_rootPentagon_MASK                         0xF000000000000000LL
#define Z7ZONE_ancestry_SHIFT                            0
#define Z7ZONE_ancestry_MASK                             0xFFFFFFFFFFFFFFFLL


extern eC_Z7Zone (* Z7Zone_from7H)(eC_I7HZone zone);

extern eC_Z7Zone (* Z7Zone_fromTextID)(constString zoneID);

extern int (* Z7Zone_getParentRotationOffset)(eC_I7HZone zone);

extern void (* Z7Zone_getTextID)(eC_Z7Zone __this, eC_String zoneID);

extern eC_I7HZone (* Z7Zone_to7H)(eC_Z7Zone __this);

extern eC_I3HZone (* eC_i3HZoneFromI9R)(eC_I9RZone zone, char subHex);
extern eC_I9RZone (* eC_i9RZoneFromI3H)(eC_I3HZone zone);
extern void (* eC_authalicSetup)(double a, double b, double cp[][]);
extern void (* eC_canonicalize5x6)(const eC_Pointd * _src, eC_Pointd * out);
extern void (* eC_compactGGGZones)(eC_Array zones, int start, int maxLevel);
extern eC_Angle (* eC_latAuthalicToGeodetic)(const double cp[][], eC_Angle phi);
extern eC_Angle (* eC_latGeodeticToAuthalic)(const double cp[][], eC_Angle phi);
extern eC_DGGSJSON (* eC_readDGGSJSON)(eC_File f);
extern eC_Class * class_BCTA3H;
extern eC_Class * class_BarycentricSphericalTriAreaProjection;
extern eC_Class * class_CRS;
extern eC_Class * class_CRSExtent;
extern eC_Class * class_CRSRegistry;
extern eC_Class * class_DGGRS;
extern eC_Class * class_DGGRSZone;
extern eC_Class * class_DGGSJSON;
extern eC_Class * class_DGGSJSONDepth;
extern eC_Class * class_DGGSJSONDimension;
extern eC_Class * class_DGGSJSONGrid;
extern eC_Class * class_DGGSJSONShape;
extern eC_Class * class_GGGZone;
extern eC_Class * class_GNOSISGlobalGrid;
extern eC_Class * class_GPP3H;
extern eC_Class * class_GeoExtent;
extern eC_Class * class_GeoPoint;
extern eC_Class * class_GoldbergPolyhedraProjection;
extern eC_Class * class_HEALPix;
extern eC_Class * class_HEALPixProjection;
extern eC_Class * class_HPZone;
extern eC_Class * class_I3HNeighbor;
extern eC_Class * class_I3HZone;
extern eC_Class * class_I4RZone;
extern eC_Class * class_I7HZone;
extern eC_Class * class_I9RZone;
extern eC_Class * class_ISEA3H;
extern eC_Class * class_ISEA4R;
extern eC_Class * class_ISEA7H;
extern eC_Class * class_ISEA7H_Z7;
extern eC_Class * class_ISEA9R;
extern eC_Class * class_ISEAProjection;
extern eC_Class * class_IVEA3H;
extern eC_Class * class_IVEA4R;
extern eC_Class * class_IVEA7H;
extern eC_Class * class_IVEA7H_Z7;
extern eC_Class * class_IVEA9R;
extern eC_Class * class_IVEAProjection;
extern eC_Class * class_JSONSchema;
extern eC_Class * class_JSONSchemaType;
extern eC_Class * class_Plane;
extern eC_Class * class_Quaternion;
extern eC_Class * class_RHPZone;
extern eC_Class * class_RI5x6Projection;
extern eC_Class * class_RI7H_Z7;
extern eC_Class * class_RTEA3H;
extern eC_Class * class_RTEA4R;
extern eC_Class * class_RTEA7H;
extern eC_Class * class_RTEA7H_Z7;
extern eC_Class * class_RTEA9R;
extern eC_Class * class_RTEAProjection;
extern eC_Class * class_RhombicIcosahedral3H;
extern eC_Class * class_RhombicIcosahedral4R;
extern eC_Class * class_RhombicIcosahedral7H;
extern eC_Class * class_RhombicIcosahedral9R;
extern eC_Class * class_SliceAndDiceGreatCircleIcosahedralProjection;
extern eC_Class * class_VGCRadialVertex;
extern eC_Class * class_Vector3D;
extern eC_Class * class_Z7Zone;
extern eC_Class * class_rHEALPix;
extern eC_Class * class_rHEALPixProjection;

extern eC_Module dggal_init(eC_Module fromModule);

