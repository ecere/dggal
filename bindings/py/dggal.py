from EcereSDK.ecere import *
from _pydggal import *

@ffi.callback("void(eC_DGGRS, eC_Array)")
def cb_DGGRS_compactZones(__e, zones):
   dggrs = pyOrNewObject(DGGRS, __e)
   dggrs.fn_DGGRS_compactZones(dggrs, Array("", impl = zones))

@ffi.callback("uint64(eC_DGGRS, eC_DGGRSZone, int)")
def cb_DGGRS_countSubZones(__e, zone, depth):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_countSubZones(dggrs, DGGRSZone(impl = zone), depth)

@ffi.callback("int(eC_DGGRS, eC_DGGRSZone)")
def cb_DGGRS_countZoneEdges(__e, zone):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_countZoneEdges(dggrs, DGGRSZone(impl = zone))

@ffi.callback("uint64(eC_DGGRS, int)")
def cb_DGGRS_countZones(__e, level):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_countZones(dggrs, level)

@ffi.callback("eC_DGGRSZone(eC_DGGRS, eC_DGGRSZone, int)")
def cb_DGGRS_getFirstSubZone(__e, zone, relativeDepth):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getFirstSubZone(dggrs, DGGRSZone(impl = zone), relativeDepth)

@ffi.callback("int(eC_DGGRS)")
def cb_DGGRS_getIndexMaxDepth(__e):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getIndexMaxDepth(dggrs)

@ffi.callback("int(eC_DGGRS)")
def cb_DGGRS_getMaxChildren(__e):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getMaxChildren(dggrs)

@ffi.callback("int(eC_DGGRS)")
def cb_DGGRS_getMaxDGGRSZoneLevel(__e):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getMaxDGGRSZoneLevel(dggrs)

@ffi.callback("int(eC_DGGRS)")
def cb_DGGRS_getMaxNeighbors(__e):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getMaxNeighbors(dggrs)

@ffi.callback("int(eC_DGGRS)")
def cb_DGGRS_getMaxParents(__e):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getMaxParents(dggrs)

@ffi.callback("int(eC_DGGRS)")
def cb_DGGRS_getRefinementRatio(__e):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getRefinementRatio(dggrs)

@ffi.callback("eC_DGGRSZone(eC_DGGRS, eC_DGGRSZone, int, int64)")
def cb_DGGRS_getSubZoneAtIndex(__e, parent, relativeDepth, index):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getSubZoneAtIndex(dggrs, DGGRSZone(impl = parent), relativeDepth, index)

@ffi.callback("template_Array_Pointd(eC_DGGRS, eC_DGGRSZone, eC_CRS, int)")
def cb_DGGRS_getSubZoneCRSCentroids(__e, parent, crs, relativeDepth):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getSubZoneCRSCentroids(dggrs, DGGRSZone(impl = parent), CRS(impl = crs), relativeDepth)

@ffi.callback("int64(eC_DGGRS, eC_DGGRSZone, eC_DGGRSZone)")
def cb_DGGRS_getSubZoneIndex(__e, parent, subZone):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getSubZoneIndex(dggrs, DGGRSZone(impl = parent), DGGRSZone(impl = subZone))

@ffi.callback("template_Array_GeoPoint(eC_DGGRS, eC_DGGRSZone, int)")
def cb_DGGRS_getSubZoneWGS84Centroids(__e, parent, relativeDepth):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getSubZoneWGS84Centroids(dggrs, DGGRSZone(impl = parent), relativeDepth)

@ffi.callback("template_Array_DGGRSZone(eC_DGGRS, eC_DGGRSZone, int)")
def cb_DGGRS_getSubZones(__e, parent, relativeDepth):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getSubZones(dggrs, DGGRSZone(impl = parent), relativeDepth)

@ffi.callback("double(eC_DGGRS, eC_DGGRSZone)")
def cb_DGGRS_getZoneArea(__e, zone):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getZoneArea(dggrs, DGGRSZone(impl = zone))

@ffi.callback("void(eC_DGGRS, eC_DGGRSZone, eC_CRS, eC_Pointd *)")
def cb_DGGRS_getZoneCRSCentroid(__e, zone, crs, centroid):
   dggrs = pyOrNewObject(DGGRS, __e)
   dggrs.fn_DGGRS_getZoneCRSCentroid(dggrs, DGGRSZone(impl = zone), CRS(impl = crs), Pointd(impl = centroid))

@ffi.callback("void(eC_DGGRS, eC_DGGRSZone, eC_CRS, eC_CRSExtent *)")
def cb_DGGRS_getZoneCRSExtent(__e, zone, crs, extent):
   dggrs = pyOrNewObject(DGGRS, __e)
   dggrs.fn_DGGRS_getZoneCRSExtent(dggrs, DGGRSZone(impl = zone), CRS(impl = crs), CRSExtent(impl = extent))

@ffi.callback("int(eC_DGGRS, eC_DGGRSZone, eC_CRS, eC_Pointd *)")
def cb_DGGRS_getZoneCRSVertices(__e, zone, crs, vertices):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getZoneCRSVertices(dggrs, DGGRSZone(impl = zone), CRS(impl = crs), vertices)

@ffi.callback("eC_DGGRSZone(eC_DGGRS, eC_DGGRSZone)")
def cb_DGGRS_getZoneCentroidChild(__e, zone):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getZoneCentroidChild(dggrs, DGGRSZone(impl = zone))

@ffi.callback("eC_DGGRSZone(eC_DGGRS, eC_DGGRSZone)")
def cb_DGGRS_getZoneCentroidParent(__e, zone):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getZoneCentroidParent(dggrs, DGGRSZone(impl = zone))

@ffi.callback("int(eC_DGGRS, eC_DGGRSZone, eC_DGGRSZone *)")
def cb_DGGRS_getZoneChildren(__e, zone, children):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getZoneChildren(dggrs, DGGRSZone(impl = zone), children)

@ffi.callback("eC_DGGRSZone(eC_DGGRS, int, eC_CRS, const eC_Pointd *)")
def cb_DGGRS_getZoneFromCRSCentroid(__e, level, crs, centroid):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getZoneFromCRSCentroid(dggrs, level, CRS(impl = crs), Pointd(impl = centroid))

@ffi.callback("eC_DGGRSZone(eC_DGGRS, constString)")
def cb_DGGRS_getZoneFromTextID(__e, zoneID):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getZoneFromTextID(dggrs, zoneID)

@ffi.callback("eC_DGGRSZone(eC_DGGRS, int, const eC_GeoPoint *)")
def cb_DGGRS_getZoneFromWGS84Centroid(__e, level, centroid):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getZoneFromWGS84Centroid(dggrs, level, GeoPoint(impl = centroid))

@ffi.callback("int(eC_DGGRS, eC_DGGRSZone)")
def cb_DGGRS_getZoneLevel(__e, zone):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getZoneLevel(dggrs, DGGRSZone(impl = zone))

@ffi.callback("int(eC_DGGRS, eC_DGGRSZone, eC_DGGRSZone *, int *)")
def cb_DGGRS_getZoneNeighbors(__e, zone, neighbors, nbType):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getZoneNeighbors(dggrs, DGGRSZone(impl = zone), neighbors, nbType)

@ffi.callback("int(eC_DGGRS, eC_DGGRSZone, eC_DGGRSZone *)")
def cb_DGGRS_getZoneParents(__e, zone, parents):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getZoneParents(dggrs, DGGRSZone(impl = zone), parents)

@ffi.callback("template_Array_Pointd(eC_DGGRS, eC_DGGRSZone, eC_CRS, int)")
def cb_DGGRS_getZoneRefinedCRSVertices(__e, zone, crs, edgeRefinement):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getZoneRefinedCRSVertices(dggrs, DGGRSZone(impl = zone), CRS(impl = crs), edgeRefinement)

@ffi.callback("template_Array_GeoPoint(eC_DGGRS, eC_DGGRSZone, int)")
def cb_DGGRS_getZoneRefinedWGS84Vertices(__e, zone, edgeRefinement):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getZoneRefinedWGS84Vertices(dggrs, DGGRSZone(impl = zone), edgeRefinement)

@ffi.callback("void(eC_DGGRS, eC_DGGRSZone, eC_String)")
def cb_DGGRS_getZoneTextID(__e, zone, zoneID):
   dggrs = pyOrNewObject(DGGRS, __e)
   dggrs.fn_DGGRS_getZoneTextID(dggrs, DGGRSZone(impl = zone), zoneID)

@ffi.callback("void(eC_DGGRS, eC_DGGRSZone, eC_GeoPoint *)")
def cb_DGGRS_getZoneWGS84Centroid(__e, zone, centroid):
   dggrs = pyOrNewObject(DGGRS, __e)
   dggrs.fn_DGGRS_getZoneWGS84Centroid(dggrs, DGGRSZone(impl = zone), GeoPoint(impl = centroid))

@ffi.callback("void(eC_DGGRS, eC_DGGRSZone, eC_GeoExtent *)")
def cb_DGGRS_getZoneWGS84Extent(__e, zone, extent):
   dggrs = pyOrNewObject(DGGRS, __e)
   dggrs.fn_DGGRS_getZoneWGS84Extent(dggrs, DGGRSZone(impl = zone), GeoExtent(impl = extent))

@ffi.callback("int(eC_DGGRS, eC_DGGRSZone, eC_GeoPoint *)")
def cb_DGGRS_getZoneWGS84Vertices(__e, zone, vertices):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_getZoneWGS84Vertices(dggrs, DGGRSZone(impl = zone), vertices)

@ffi.callback("eC_bool(eC_DGGRS, eC_DGGRSZone)")
def cb_DGGRS_isZoneCentroidChild(__e, zone):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_isZoneCentroidChild(dggrs, DGGRSZone(impl = zone))

@ffi.callback("template_Array_DGGRSZone(eC_DGGRS, int, const eC_GeoExtent *)")
def cb_DGGRS_listZones(__e, level, bbox):
   dggrs = pyOrNewObject(DGGRS, __e)
   return dggrs.fn_DGGRS_listZones(dggrs, level, GeoExtent(impl = bbox))

class DGGRS(Instance):
   class_members = [
                      'compactZones',
                      'countSubZones',
                      'countZoneEdges',
                      'countZones',
                      'getFirstSubZone',
                      'getIndexMaxDepth',
                      'getMaxChildren',
                      'getMaxDGGRSZoneLevel',
                      'getMaxNeighbors',
                      'getMaxParents',
                      'getRefinementRatio',
                      'getSubZoneAtIndex',
                      'getSubZoneCRSCentroids',
                      'getSubZoneIndex',
                      'getSubZoneWGS84Centroids',
                      'getSubZones',
                      'getZoneArea',
                      'getZoneCRSCentroid',
                      'getZoneCRSExtent',
                      'getZoneCRSVertices',
                      'getZoneCentroidChild',
                      'getZoneCentroidParent',
                      'getZoneChildren',
                      'getZoneFromCRSCentroid',
                      'getZoneFromTextID',
                      'getZoneFromWGS84Centroid',
                      'getZoneLevel',
                      'getZoneNeighbors',
                      'getZoneParents',
                      'getZoneRefinedCRSVertices',
                      'getZoneRefinedWGS84Vertices',
                      'getZoneTextID',
                      'getZoneWGS84Centroid',
                      'getZoneWGS84Extent',
                      'getZoneWGS84Vertices',
                      'isZoneCentroidChild',
                      'listZones',
                   ]

   def init_args(self, args, kwArgs): init_args(DGGRS, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   def areZonesNeighbors(self, a, b):
      if a is not None and not isinstance(a, DGGRSZone): a = DGGRSZone(a)
      if a is None: a = ffi.NULL
      if b is not None and not isinstance(b, DGGRSZone): b = DGGRSZone(b)
      if b is None: b = ffi.NULL
      return lib.DGGRS_areZonesNeighbors(self.impl, a, b)

   def areZonesSiblings(self, a, b):
      if a is not None and not isinstance(a, DGGRSZone): a = DGGRSZone(a)
      if a is None: a = ffi.NULL
      if b is not None and not isinstance(b, DGGRSZone): b = DGGRSZone(b)
      if b is None: b = ffi.NULL
      return lib.DGGRS_areZonesSiblings(self.impl, a, b)

   def fn_unset_DGGRS_compactZones(self, zones):
      return lib.DGGRS_compactZones(self.impl, ffi.NULL if zones is None else zones.impl)

   @property
   def compactZones(self):
      if hasattr(self, 'fn_DGGRS_compactZones'): return self.fn_DGGRS_compactZones
      else: return self.fn_unset_DGGRS_compactZones
   @compactZones.setter
   def compactZones(self, value):
      self.fn_DGGRS_compactZones = value
      lib.Instance_setMethod(self.impl, "compactZones".encode('u8'), cb_DGGRS_compactZones)

   def fn_unset_DGGRS_countSubZones(self, zone, depth):
      return lib.DGGRS_countSubZones(self.impl, zone, depth)

   @property
   def countSubZones(self):
      if hasattr(self, 'fn_DGGRS_countSubZones'): return self.fn_DGGRS_countSubZones
      else: return self.fn_unset_DGGRS_countSubZones
   @countSubZones.setter
   def countSubZones(self, value):
      self.fn_DGGRS_countSubZones = value
      lib.Instance_setMethod(self.impl, "countSubZones".encode('u8'), cb_DGGRS_countSubZones)

   def fn_unset_DGGRS_countZoneEdges(self, zone):
      return lib.DGGRS_countZoneEdges(self.impl, zone)

   @property
   def countZoneEdges(self):
      if hasattr(self, 'fn_DGGRS_countZoneEdges'): return self.fn_DGGRS_countZoneEdges
      else: return self.fn_unset_DGGRS_countZoneEdges
   @countZoneEdges.setter
   def countZoneEdges(self, value):
      self.fn_DGGRS_countZoneEdges = value
      lib.Instance_setMethod(self.impl, "countZoneEdges".encode('u8'), cb_DGGRS_countZoneEdges)

   def fn_unset_DGGRS_countZones(self, level):
      return lib.DGGRS_countZones(self.impl, level)

   @property
   def countZones(self):
      if hasattr(self, 'fn_DGGRS_countZones'): return self.fn_DGGRS_countZones
      else: return self.fn_unset_DGGRS_countZones
   @countZones.setter
   def countZones(self, value):
      self.fn_DGGRS_countZones = value
      lib.Instance_setMethod(self.impl, "countZones".encode('u8'), cb_DGGRS_countZones)

   def doZonesOverlap(self, a, b):
      if a is not None and not isinstance(a, DGGRSZone): a = DGGRSZone(a)
      if a is None: a = ffi.NULL
      if b is not None and not isinstance(b, DGGRSZone): b = DGGRSZone(b)
      if b is None: b = ffi.NULL
      return lib.DGGRS_doZonesOverlap(self.impl, a, b)

   def doesZoneContain(self, hayStack, needle):
      if hayStack is not None and not isinstance(hayStack, DGGRSZone): hayStack = DGGRSZone(hayStack)
      if hayStack is None: hayStack = ffi.NULL
      if needle is not None and not isinstance(needle, DGGRSZone): needle = DGGRSZone(needle)
      if needle is None: needle = ffi.NULL
      return lib.DGGRS_doesZoneContain(self.impl, hayStack, needle)

   def get64KDepth(self):
      return lib.DGGRS_get64KDepth(self.impl)

   def fn_unset_DGGRS_getFirstSubZone(self, zone, relativeDepth):
      return lib.DGGRS_getFirstSubZone(self.impl, zone, relativeDepth)

   @property
   def getFirstSubZone(self):
      if hasattr(self, 'fn_DGGRS_getFirstSubZone'): return self.fn_DGGRS_getFirstSubZone
      else: return self.fn_unset_DGGRS_getFirstSubZone
   @getFirstSubZone.setter
   def getFirstSubZone(self, value):
      self.fn_DGGRS_getFirstSubZone = value
      lib.Instance_setMethod(self.impl, "getFirstSubZone".encode('u8'), cb_DGGRS_getFirstSubZone)

   def fn_unset_DGGRS_getIndexMaxDepth(self):
      return lib.DGGRS_getIndexMaxDepth(self.impl)

   @property
   def getIndexMaxDepth(self):
      if hasattr(self, 'fn_DGGRS_getIndexMaxDepth'): return self.fn_DGGRS_getIndexMaxDepth
      else: return self.fn_unset_DGGRS_getIndexMaxDepth
   @getIndexMaxDepth.setter
   def getIndexMaxDepth(self, value):
      self.fn_DGGRS_getIndexMaxDepth = value
      lib.Instance_setMethod(self.impl, "getIndexMaxDepth".encode('u8'), cb_DGGRS_getIndexMaxDepth)

   def getLevelFromMetersPerSubZone(self, physicalMetersPerSubZone, relativeDepth):
      return lib.DGGRS_getLevelFromMetersPerSubZone(self.impl, physicalMetersPerSubZone, relativeDepth)

   def getLevelFromPixelsAndExtent(self, extent, pixels, relativeDepth):
      if extent is not None and not isinstance(extent, GeoExtent): extent = GeoExtent(extent)
      extent = ffi.NULL if extent is None else extent.impl
      if pixels is not None and not isinstance(pixels, Point): pixels = Point(pixels)
      pixels = ffi.NULL if pixels is None else pixels.impl
      return lib.DGGRS_getLevelFromPixelsAndExtent(self.impl, ffi.cast("eC_GeoExtent *", extent), ffi.cast("eC_Point *", pixels), relativeDepth)

   def getLevelFromRefZoneArea(self, metersSquared):
      return lib.DGGRS_getLevelFromRefZoneArea(self.impl, metersSquared)

   def getLevelFromScaleDenominator(self, scaleDenominator, relativeDepth, mmPerPixel):
      return lib.DGGRS_getLevelFromScaleDenominator(self.impl, scaleDenominator, relativeDepth, mmPerPixel)

   def fn_unset_DGGRS_getMaxChildren(self):
      return lib.DGGRS_getMaxChildren(self.impl)

   @property
   def getMaxChildren(self):
      if hasattr(self, 'fn_DGGRS_getMaxChildren'): return self.fn_DGGRS_getMaxChildren
      else: return self.fn_unset_DGGRS_getMaxChildren
   @getMaxChildren.setter
   def getMaxChildren(self, value):
      self.fn_DGGRS_getMaxChildren = value
      lib.Instance_setMethod(self.impl, "getMaxChildren".encode('u8'), cb_DGGRS_getMaxChildren)

   def fn_unset_DGGRS_getMaxDGGRSZoneLevel(self):
      return lib.DGGRS_getMaxDGGRSZoneLevel(self.impl)

   @property
   def getMaxDGGRSZoneLevel(self):
      if hasattr(self, 'fn_DGGRS_getMaxDGGRSZoneLevel'): return self.fn_DGGRS_getMaxDGGRSZoneLevel
      else: return self.fn_unset_DGGRS_getMaxDGGRSZoneLevel
   @getMaxDGGRSZoneLevel.setter
   def getMaxDGGRSZoneLevel(self, value):
      self.fn_DGGRS_getMaxDGGRSZoneLevel = value
      lib.Instance_setMethod(self.impl, "getMaxDGGRSZoneLevel".encode('u8'), cb_DGGRS_getMaxDGGRSZoneLevel)

   def getMaxDepth(self):
      return lib.DGGRS_getMaxDepth(self.impl)

   def fn_unset_DGGRS_getMaxNeighbors(self):
      return lib.DGGRS_getMaxNeighbors(self.impl)

   @property
   def getMaxNeighbors(self):
      if hasattr(self, 'fn_DGGRS_getMaxNeighbors'): return self.fn_DGGRS_getMaxNeighbors
      else: return self.fn_unset_DGGRS_getMaxNeighbors
   @getMaxNeighbors.setter
   def getMaxNeighbors(self, value):
      self.fn_DGGRS_getMaxNeighbors = value
      lib.Instance_setMethod(self.impl, "getMaxNeighbors".encode('u8'), cb_DGGRS_getMaxNeighbors)

   def fn_unset_DGGRS_getMaxParents(self):
      return lib.DGGRS_getMaxParents(self.impl)

   @property
   def getMaxParents(self):
      if hasattr(self, 'fn_DGGRS_getMaxParents'): return self.fn_DGGRS_getMaxParents
      else: return self.fn_unset_DGGRS_getMaxParents
   @getMaxParents.setter
   def getMaxParents(self, value):
      self.fn_DGGRS_getMaxParents = value
      lib.Instance_setMethod(self.impl, "getMaxParents".encode('u8'), cb_DGGRS_getMaxParents)

   def getMetersPerSubZoneFromLevel(self, parentLevel, relativeDepth):
      return lib.DGGRS_getMetersPerSubZoneFromLevel(self.impl, parentLevel, relativeDepth)

   def getRefZoneArea(self, level):
      return lib.DGGRS_getRefZoneArea(self.impl, level)

   def fn_unset_DGGRS_getRefinementRatio(self):
      return lib.DGGRS_getRefinementRatio(self.impl)

   @property
   def getRefinementRatio(self):
      if hasattr(self, 'fn_DGGRS_getRefinementRatio'): return self.fn_DGGRS_getRefinementRatio
      else: return self.fn_unset_DGGRS_getRefinementRatio
   @getRefinementRatio.setter
   def getRefinementRatio(self, value):
      self.fn_DGGRS_getRefinementRatio = value
      lib.Instance_setMethod(self.impl, "getRefinementRatio".encode('u8'), cb_DGGRS_getRefinementRatio)

   def getScaleDenominatorFromLevel(self, parentLevel, relativeDepth, mmPerPixel):
      return lib.DGGRS_getScaleDenominatorFromLevel(self.impl, parentLevel, relativeDepth, mmPerPixel)

   def fn_unset_DGGRS_getSubZoneAtIndex(self, parent, relativeDepth, index):
      return lib.DGGRS_getSubZoneAtIndex(self.impl, parent, relativeDepth, index)

   @property
   def getSubZoneAtIndex(self):
      if hasattr(self, 'fn_DGGRS_getSubZoneAtIndex'): return self.fn_DGGRS_getSubZoneAtIndex
      else: return self.fn_unset_DGGRS_getSubZoneAtIndex
   @getSubZoneAtIndex.setter
   def getSubZoneAtIndex(self, value):
      self.fn_DGGRS_getSubZoneAtIndex = value
      lib.Instance_setMethod(self.impl, "getSubZoneAtIndex".encode('u8'), cb_DGGRS_getSubZoneAtIndex)

   def fn_unset_DGGRS_getSubZoneCRSCentroids(self, parent, crs, relativeDepth):
      return pyOrNewObject(Array, lib.DGGRS_getSubZoneCRSCentroids(self.impl, parent, crs, relativeDepth))

   @property
   def getSubZoneCRSCentroids(self):
      if hasattr(self, 'fn_DGGRS_getSubZoneCRSCentroids'): return self.fn_DGGRS_getSubZoneCRSCentroids
      else: return self.fn_unset_DGGRS_getSubZoneCRSCentroids
   @getSubZoneCRSCentroids.setter
   def getSubZoneCRSCentroids(self, value):
      self.fn_DGGRS_getSubZoneCRSCentroids = value
      lib.Instance_setMethod(self.impl, "getSubZoneCRSCentroids".encode('u8'), cb_DGGRS_getSubZoneCRSCentroids)

   def fn_unset_DGGRS_getSubZoneIndex(self, parent, subZone):
      return lib.DGGRS_getSubZoneIndex(self.impl, parent, subZone)

   @property
   def getSubZoneIndex(self):
      if hasattr(self, 'fn_DGGRS_getSubZoneIndex'): return self.fn_DGGRS_getSubZoneIndex
      else: return self.fn_unset_DGGRS_getSubZoneIndex
   @getSubZoneIndex.setter
   def getSubZoneIndex(self, value):
      self.fn_DGGRS_getSubZoneIndex = value
      lib.Instance_setMethod(self.impl, "getSubZoneIndex".encode('u8'), cb_DGGRS_getSubZoneIndex)

   def fn_unset_DGGRS_getSubZoneWGS84Centroids(self, parent, relativeDepth):
      return pyOrNewObject(Array, lib.DGGRS_getSubZoneWGS84Centroids(self.impl, parent, relativeDepth))

   @property
   def getSubZoneWGS84Centroids(self):
      if hasattr(self, 'fn_DGGRS_getSubZoneWGS84Centroids'): return self.fn_DGGRS_getSubZoneWGS84Centroids
      else: return self.fn_unset_DGGRS_getSubZoneWGS84Centroids
   @getSubZoneWGS84Centroids.setter
   def getSubZoneWGS84Centroids(self, value):
      self.fn_DGGRS_getSubZoneWGS84Centroids = value
      lib.Instance_setMethod(self.impl, "getSubZoneWGS84Centroids".encode('u8'), cb_DGGRS_getSubZoneWGS84Centroids)

   def fn_unset_DGGRS_getSubZones(self, parent, relativeDepth):
      return pyOrNewObject(Array, lib.DGGRS_getSubZones(self.impl, parent, relativeDepth))

   @property
   def getSubZones(self):
      if hasattr(self, 'fn_DGGRS_getSubZones'): return self.fn_DGGRS_getSubZones
      else: return self.fn_unset_DGGRS_getSubZones
   @getSubZones.setter
   def getSubZones(self, value):
      self.fn_DGGRS_getSubZones = value
      lib.Instance_setMethod(self.impl, "getSubZones".encode('u8'), cb_DGGRS_getSubZones)

   def fn_unset_DGGRS_getZoneArea(self, zone):
      return lib.DGGRS_getZoneArea(self.impl, zone)

   @property
   def getZoneArea(self):
      if hasattr(self, 'fn_DGGRS_getZoneArea'): return self.fn_DGGRS_getZoneArea
      else: return self.fn_unset_DGGRS_getZoneArea
   @getZoneArea.setter
   def getZoneArea(self, value):
      self.fn_DGGRS_getZoneArea = value
      lib.Instance_setMethod(self.impl, "getZoneArea".encode('u8'), cb_DGGRS_getZoneArea)

   def fn_unset_DGGRS_getZoneCRSCentroid(self, zone, crs, centroid):
      return lib.DGGRS_getZoneCRSCentroid(self.impl, zone, crs, ffi.NULL if centroid is None else centroid.impl)

   @property
   def getZoneCRSCentroid(self):
      if hasattr(self, 'fn_DGGRS_getZoneCRSCentroid'): return self.fn_DGGRS_getZoneCRSCentroid
      else: return self.fn_unset_DGGRS_getZoneCRSCentroid
   @getZoneCRSCentroid.setter
   def getZoneCRSCentroid(self, value):
      self.fn_DGGRS_getZoneCRSCentroid = value
      lib.Instance_setMethod(self.impl, "getZoneCRSCentroid".encode('u8'), cb_DGGRS_getZoneCRSCentroid)

   def fn_unset_DGGRS_getZoneCRSExtent(self, zone, crs, extent):
      return lib.DGGRS_getZoneCRSExtent(self.impl, zone, crs, ffi.NULL if extent is None else extent.impl)

   @property
   def getZoneCRSExtent(self):
      if hasattr(self, 'fn_DGGRS_getZoneCRSExtent'): return self.fn_DGGRS_getZoneCRSExtent
      else: return self.fn_unset_DGGRS_getZoneCRSExtent
   @getZoneCRSExtent.setter
   def getZoneCRSExtent(self, value):
      self.fn_DGGRS_getZoneCRSExtent = value
      lib.Instance_setMethod(self.impl, "getZoneCRSExtent".encode('u8'), cb_DGGRS_getZoneCRSExtent)

   def fn_unset_DGGRS_getZoneCRSVertices(self, zone, crs, vertices):
      return lib.DGGRS_getZoneCRSVertices(self.impl, zone, crs, vertices)

   @property
   def getZoneCRSVertices(self):
      if hasattr(self, 'fn_DGGRS_getZoneCRSVertices'): return self.fn_DGGRS_getZoneCRSVertices
      else: return self.fn_unset_DGGRS_getZoneCRSVertices
   @getZoneCRSVertices.setter
   def getZoneCRSVertices(self, value):
      self.fn_DGGRS_getZoneCRSVertices = value
      lib.Instance_setMethod(self.impl, "getZoneCRSVertices".encode('u8'), cb_DGGRS_getZoneCRSVertices)

   def fn_unset_DGGRS_getZoneCentroidChild(self, zone):
      return lib.DGGRS_getZoneCentroidChild(self.impl, zone)

   @property
   def getZoneCentroidChild(self):
      if hasattr(self, 'fn_DGGRS_getZoneCentroidChild'): return self.fn_DGGRS_getZoneCentroidChild
      else: return self.fn_unset_DGGRS_getZoneCentroidChild
   @getZoneCentroidChild.setter
   def getZoneCentroidChild(self, value):
      self.fn_DGGRS_getZoneCentroidChild = value
      lib.Instance_setMethod(self.impl, "getZoneCentroidChild".encode('u8'), cb_DGGRS_getZoneCentroidChild)

   def fn_unset_DGGRS_getZoneCentroidParent(self, zone):
      return lib.DGGRS_getZoneCentroidParent(self.impl, zone)

   @property
   def getZoneCentroidParent(self):
      if hasattr(self, 'fn_DGGRS_getZoneCentroidParent'): return self.fn_DGGRS_getZoneCentroidParent
      else: return self.fn_unset_DGGRS_getZoneCentroidParent
   @getZoneCentroidParent.setter
   def getZoneCentroidParent(self, value):
      self.fn_DGGRS_getZoneCentroidParent = value
      lib.Instance_setMethod(self.impl, "getZoneCentroidParent".encode('u8'), cb_DGGRS_getZoneCentroidParent)

   def fn_unset_DGGRS_getZoneChildren(self, zone, children):
      if children is None: children = ffi.NULL
      return lib.DGGRS_getZoneChildren(self.impl, zone, children)

   @property
   def getZoneChildren(self):
      if hasattr(self, 'fn_DGGRS_getZoneChildren'): return self.fn_DGGRS_getZoneChildren
      else: return self.fn_unset_DGGRS_getZoneChildren
   @getZoneChildren.setter
   def getZoneChildren(self, value):
      self.fn_DGGRS_getZoneChildren = value
      lib.Instance_setMethod(self.impl, "getZoneChildren".encode('u8'), cb_DGGRS_getZoneChildren)

   def fn_unset_DGGRS_getZoneFromCRSCentroid(self, level, crs, centroid):
      return lib.DGGRS_getZoneFromCRSCentroid(self.impl, level, crs, ffi.NULL if centroid is None else centroid.impl)

   @property
   def getZoneFromCRSCentroid(self):
      if hasattr(self, 'fn_DGGRS_getZoneFromCRSCentroid'): return self.fn_DGGRS_getZoneFromCRSCentroid
      else: return self.fn_unset_DGGRS_getZoneFromCRSCentroid
   @getZoneFromCRSCentroid.setter
   def getZoneFromCRSCentroid(self, value):
      self.fn_DGGRS_getZoneFromCRSCentroid = value
      lib.Instance_setMethod(self.impl, "getZoneFromCRSCentroid".encode('u8'), cb_DGGRS_getZoneFromCRSCentroid)

   def fn_unset_DGGRS_getZoneFromTextID(self, zoneID):
      return lib.DGGRS_getZoneFromTextID(self.impl, ffi.NULL if zoneID is None else zoneID.encode('u8'))

   @property
   def getZoneFromTextID(self):
      if hasattr(self, 'fn_DGGRS_getZoneFromTextID'): return self.fn_DGGRS_getZoneFromTextID
      else: return self.fn_unset_DGGRS_getZoneFromTextID
   @getZoneFromTextID.setter
   def getZoneFromTextID(self, value):
      self.fn_DGGRS_getZoneFromTextID = value
      lib.Instance_setMethod(self.impl, "getZoneFromTextID".encode('u8'), cb_DGGRS_getZoneFromTextID)

   def fn_unset_DGGRS_getZoneFromWGS84Centroid(self, level, centroid):
      return lib.DGGRS_getZoneFromWGS84Centroid(self.impl, level, ffi.NULL if centroid is None else centroid.impl)

   @property
   def getZoneFromWGS84Centroid(self):
      if hasattr(self, 'fn_DGGRS_getZoneFromWGS84Centroid'): return self.fn_DGGRS_getZoneFromWGS84Centroid
      else: return self.fn_unset_DGGRS_getZoneFromWGS84Centroid
   @getZoneFromWGS84Centroid.setter
   def getZoneFromWGS84Centroid(self, value):
      self.fn_DGGRS_getZoneFromWGS84Centroid = value
      lib.Instance_setMethod(self.impl, "getZoneFromWGS84Centroid".encode('u8'), cb_DGGRS_getZoneFromWGS84Centroid)

   def fn_unset_DGGRS_getZoneLevel(self, zone):
      return lib.DGGRS_getZoneLevel(self.impl, zone)

   @property
   def getZoneLevel(self):
      if hasattr(self, 'fn_DGGRS_getZoneLevel'): return self.fn_DGGRS_getZoneLevel
      else: return self.fn_unset_DGGRS_getZoneLevel
   @getZoneLevel.setter
   def getZoneLevel(self, value):
      self.fn_DGGRS_getZoneLevel = value
      lib.Instance_setMethod(self.impl, "getZoneLevel".encode('u8'), cb_DGGRS_getZoneLevel)

   def fn_unset_DGGRS_getZoneNeighbors(self, zone, neighbors, nbType):
      if neighbors is None: neighbors = ffi.NULL
      if nbType is None: nbType = ffi.NULL
      return lib.DGGRS_getZoneNeighbors(self.impl, zone, neighbors, nbType)

   @property
   def getZoneNeighbors(self):
      if hasattr(self, 'fn_DGGRS_getZoneNeighbors'): return self.fn_DGGRS_getZoneNeighbors
      else: return self.fn_unset_DGGRS_getZoneNeighbors
   @getZoneNeighbors.setter
   def getZoneNeighbors(self, value):
      self.fn_DGGRS_getZoneNeighbors = value
      lib.Instance_setMethod(self.impl, "getZoneNeighbors".encode('u8'), cb_DGGRS_getZoneNeighbors)

   def fn_unset_DGGRS_getZoneParents(self, zone, parents):
      if parents is None: parents = ffi.NULL
      return lib.DGGRS_getZoneParents(self.impl, zone, parents)

   @property
   def getZoneParents(self):
      if hasattr(self, 'fn_DGGRS_getZoneParents'): return self.fn_DGGRS_getZoneParents
      else: return self.fn_unset_DGGRS_getZoneParents
   @getZoneParents.setter
   def getZoneParents(self, value):
      self.fn_DGGRS_getZoneParents = value
      lib.Instance_setMethod(self.impl, "getZoneParents".encode('u8'), cb_DGGRS_getZoneParents)

   def fn_unset_DGGRS_getZoneRefinedCRSVertices(self, zone, crs, edgeRefinement):
      return pyOrNewObject(Array, lib.DGGRS_getZoneRefinedCRSVertices(self.impl, zone, crs, edgeRefinement))

   @property
   def getZoneRefinedCRSVertices(self):
      if hasattr(self, 'fn_DGGRS_getZoneRefinedCRSVertices'): return self.fn_DGGRS_getZoneRefinedCRSVertices
      else: return self.fn_unset_DGGRS_getZoneRefinedCRSVertices
   @getZoneRefinedCRSVertices.setter
   def getZoneRefinedCRSVertices(self, value):
      self.fn_DGGRS_getZoneRefinedCRSVertices = value
      lib.Instance_setMethod(self.impl, "getZoneRefinedCRSVertices".encode('u8'), cb_DGGRS_getZoneRefinedCRSVertices)

   def fn_unset_DGGRS_getZoneRefinedWGS84Vertices(self, zone, edgeRefinement):
      return pyOrNewObject(Array, lib.DGGRS_getZoneRefinedWGS84Vertices(self.impl, zone, edgeRefinement))

   @property
   def getZoneRefinedWGS84Vertices(self):
      if hasattr(self, 'fn_DGGRS_getZoneRefinedWGS84Vertices'): return self.fn_DGGRS_getZoneRefinedWGS84Vertices
      else: return self.fn_unset_DGGRS_getZoneRefinedWGS84Vertices
   @getZoneRefinedWGS84Vertices.setter
   def getZoneRefinedWGS84Vertices(self, value):
      self.fn_DGGRS_getZoneRefinedWGS84Vertices = value
      lib.Instance_setMethod(self.impl, "getZoneRefinedWGS84Vertices".encode('u8'), cb_DGGRS_getZoneRefinedWGS84Vertices)

   def fn_unset_DGGRS_getZoneTextID(self, zone):
      # TODO: Review how to automate returning by character buffer
      zid = ffi.new('byte[]', 256)
      lib.DGGRS_getZoneTextID(self.impl, zone, zid)
      return ffi.string(zid).decode('u8')

   @property
   def getZoneTextID(self):
      if hasattr(self, 'fn_DGGRS_getZoneTextID'): return self.fn_DGGRS_getZoneTextID
      else: return self.fn_unset_DGGRS_getZoneTextID
   @getZoneTextID.setter
   def getZoneTextID(self, value):
      self.fn_DGGRS_getZoneTextID = value
      lib.Instance_setMethod(self.impl, "getZoneTextID".encode('u8'), cb_DGGRS_getZoneTextID)

   def fn_unset_DGGRS_getZoneWGS84Centroid(self, zone, centroid):
      return lib.DGGRS_getZoneWGS84Centroid(self.impl, zone, ffi.NULL if centroid is None else centroid.impl)

   @property
   def getZoneWGS84Centroid(self):
      if hasattr(self, 'fn_DGGRS_getZoneWGS84Centroid'): return self.fn_DGGRS_getZoneWGS84Centroid
      else: return self.fn_unset_DGGRS_getZoneWGS84Centroid
   @getZoneWGS84Centroid.setter
   def getZoneWGS84Centroid(self, value):
      self.fn_DGGRS_getZoneWGS84Centroid = value
      lib.Instance_setMethod(self.impl, "getZoneWGS84Centroid".encode('u8'), cb_DGGRS_getZoneWGS84Centroid)

   def fn_unset_DGGRS_getZoneWGS84Extent(self, zone, extent):
      return lib.DGGRS_getZoneWGS84Extent(self.impl, zone, ffi.NULL if extent is None else extent.impl)

   @property
   def getZoneWGS84Extent(self):
      if hasattr(self, 'fn_DGGRS_getZoneWGS84Extent'): return self.fn_DGGRS_getZoneWGS84Extent
      else: return self.fn_unset_DGGRS_getZoneWGS84Extent
   @getZoneWGS84Extent.setter
   def getZoneWGS84Extent(self, value):
      self.fn_DGGRS_getZoneWGS84Extent = value
      lib.Instance_setMethod(self.impl, "getZoneWGS84Extent".encode('u8'), cb_DGGRS_getZoneWGS84Extent)

   def fn_unset_DGGRS_getZoneWGS84Vertices(self, zone, vertices):
      return lib.DGGRS_getZoneWGS84Vertices(self.impl, zone, vertices)

   @property
   def getZoneWGS84Vertices(self):
      if hasattr(self, 'fn_DGGRS_getZoneWGS84Vertices'): return self.fn_DGGRS_getZoneWGS84Vertices
      else: return self.fn_unset_DGGRS_getZoneWGS84Vertices
   @getZoneWGS84Vertices.setter
   def getZoneWGS84Vertices(self, value):
      self.fn_DGGRS_getZoneWGS84Vertices = value
      lib.Instance_setMethod(self.impl, "getZoneWGS84Vertices".encode('u8'), cb_DGGRS_getZoneWGS84Vertices)

   def isZoneAncestorOf(self, ancestor, descendant, maxDepth):
      if ancestor is not None and not isinstance(ancestor, DGGRSZone): ancestor = DGGRSZone(ancestor)
      if ancestor is None: ancestor = ffi.NULL
      if descendant is not None and not isinstance(descendant, DGGRSZone): descendant = DGGRSZone(descendant)
      if descendant is None: descendant = ffi.NULL
      return lib.DGGRS_isZoneAncestorOf(self.impl, ancestor, descendant, maxDepth)

   def fn_unset_DGGRS_isZoneCentroidChild(self, zone):
      return lib.DGGRS_isZoneCentroidChild(self.impl, zone)

   @property
   def isZoneCentroidChild(self):
      if hasattr(self, 'fn_DGGRS_isZoneCentroidChild'): return self.fn_DGGRS_isZoneCentroidChild
      else: return self.fn_unset_DGGRS_isZoneCentroidChild
   @isZoneCentroidChild.setter
   def isZoneCentroidChild(self, value):
      self.fn_DGGRS_isZoneCentroidChild = value
      lib.Instance_setMethod(self.impl, "isZoneCentroidChild".encode('u8'), cb_DGGRS_isZoneCentroidChild)

   def isZoneContainedIn(self, needle, hayStack):
      if needle is not None and not isinstance(needle, DGGRSZone): needle = DGGRSZone(needle)
      if needle is None: needle = ffi.NULL
      if hayStack is not None and not isinstance(hayStack, DGGRSZone): hayStack = DGGRSZone(hayStack)
      if hayStack is None: hayStack = ffi.NULL
      return lib.DGGRS_isZoneContainedIn(self.impl, needle, hayStack)

   def isZoneDescendantOf(self, descendant, ancestor, maxDepth):
      if descendant is not None and not isinstance(descendant, DGGRSZone): descendant = DGGRSZone(descendant)
      if descendant is None: descendant = ffi.NULL
      if ancestor is not None and not isinstance(ancestor, DGGRSZone): ancestor = DGGRSZone(ancestor)
      if ancestor is None: ancestor = ffi.NULL
      return lib.DGGRS_isZoneDescendantOf(self.impl, descendant, ancestor, maxDepth)

   def isZoneImmediateChildOf(self, child, parent):
      if child is not None and not isinstance(child, DGGRSZone): child = DGGRSZone(child)
      if child is None: child = ffi.NULL
      if parent is not None and not isinstance(parent, DGGRSZone): parent = DGGRSZone(parent)
      if parent is None: parent = ffi.NULL
      return lib.DGGRS_isZoneImmediateChildOf(self.impl, child, parent)

   def isZoneImmediateParentOf(self, parent, child):
      if parent is not None and not isinstance(parent, DGGRSZone): parent = DGGRSZone(parent)
      if parent is None: parent = ffi.NULL
      if child is not None and not isinstance(child, DGGRSZone): child = DGGRSZone(child)
      if child is None: child = ffi.NULL
      return lib.DGGRS_isZoneImmediateParentOf(self.impl, parent, child)

   def fn_unset_DGGRS_listZones(self, level, bbox):
      return pyOrNewObject(Array, lib.DGGRS_listZones(self.impl, level, ffi.NULL if bbox is None else bbox.impl))

   @property
   def listZones(self):
      if hasattr(self, 'fn_DGGRS_listZones'): return self.fn_DGGRS_listZones
      else: return self.fn_unset_DGGRS_listZones
   @listZones.setter
   def listZones(self, value):
      self.fn_DGGRS_listZones = value
      lib.Instance_setMethod(self.impl, "listZones".encode('u8'), cb_DGGRS_listZones)

   def zoneHasSubZone(self, hayStack, needle):
      if hayStack is not None and not isinstance(hayStack, DGGRSZone): hayStack = DGGRSZone(hayStack)
      if hayStack is None: hayStack = ffi.NULL
      if needle is not None and not isinstance(needle, DGGRSZone): needle = DGGRSZone(needle)
      if needle is None: needle = ffi.NULL
      return lib.DGGRS_zoneHasSubZone(self.impl, hayStack, needle)

class DGGRSZone(pyBaseClass):pass

class GeoPoint(Struct):
   def __init__(self, lat = 0, lon = 0, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_GeoPoint *", impl)
      else:
         if isinstance(lat, tuple):
            __tuple = lat
            lat = 0
            if len(__tuple) > 0: lat = __tuple[0]
            if len(__tuple) > 1: lon = __tuple[1]
         if lat is not None:
            if not isinstance(lat, Angle): lat = Degrees(lat)
            lat = lat.impl
         else:
            lat = Degrees()
         if lon is not None:
            if not isinstance(lon, Angle): lon = Degrees(lon)
            lon = lon.impl
         else:
            lon = Degrees()
         self.impl = ffi.new("eC_GeoPoint *", { 'lat' : lat, 'lon' : lon })

   @property
   def lat(self): return Degrees(impl = self.impl.lat)
   @lat.setter
   def lat(self, value):
      if not isinstance(value, Angle): value = Degrees(value)
      self.impl.lat = value.impl

   @property
   def lon(self): return Degrees(impl = self.impl.lon)
   @lon.setter
   def lon(self, value):
      if not isinstance(value, Angle): value = Degrees(value)
      self.impl.lon = value.impl

class GeoExtent(Struct):
   def __init__(self, ll = None, ur = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_GeoExtent *", impl)
      else:
         if isinstance(ll, tuple):
            __tuple = ll
            ll = None
            if len(__tuple) > 0: ll = __tuple[0]
            if len(__tuple) > 1: ur = __tuple[1]
         if ll is not None:
            if not isinstance(ll, GeoPoint): ll = GeoPoint(ll)
            ll = ll.impl[0]
         else:
            ll = GeoPoint()
            ll = ll.impl[0]
         if ur is not None:
            if not isinstance(ur, GeoPoint): ur = GeoPoint(ur)
            ur = ur.impl[0]
         else:
            ur = GeoPoint()
            ur = ur.impl[0]
         self.impl = ffi.new("eC_GeoExtent *", { 'll' : ll, 'ur' : ur })

   @property
   def ll(self): return GeoPoint(impl = self.impl.ll)
   @ll.setter
   def ll(self, value):
      if not isinstance(value, GeoPoint): value = GeoPoint(value)
      self.impl.ll = value.impl[0]

   @property
   def ur(self): return GeoPoint(impl = self.impl.ur)
   @ur.setter
   def ur(self, value):
      if not isinstance(value, GeoPoint): value = GeoPoint(value)
      self.impl.ur = value.impl[0]

   @property
   def geodeticArea(self): return lib.GeoExtent_get_geodeticArea(self.impl)

   def clear(self):
      lib.GeoExtent_clear(ffi.cast("eC_GeoExtent *", self.impl))

   def intersects(self, b = None):
      if b is not None and not isinstance(b, GeoExtent): b = GeoExtent(b)
      b = ffi.NULL if b is None else b.impl
      return lib.GeoExtent_intersects(ffi.cast("eC_GeoExtent *", self.impl), ffi.cast("eC_GeoExtent *", b))

wgs84Major = Meters ( 6378137.0 )

wholeWorld = GeoExtent (  ( -90, -180 ),  ( 90, 180 ) )

class CRS(pyBaseClass):
   def __init__(self, registry = 0, crsID = 0, h = False, impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(registry, CRS):
         self.impl = registry.impl
      else:
         if isinstance(registry, tuple):
            __tuple = registry
            registry = 0
            if len(__tuple) > 0: registry = __tuple[0]
            if len(__tuple) > 1: crsID = __tuple[1]
            if len(__tuple) > 2: h = __tuple[2]
         self.impl = (
            (registry << lib.CRS_registry_SHIFT) |
            (crsID    << lib.CRS_crsID_SHIFT)    |
            (h        << lib.CRS_h_SHIFT)        )

   @property
   def registry(self): return ((((self.impl)) & lib.CRS_registry_MASK) >> lib.CRS_registry_SHIFT)
   @registry.setter
   def registry(self, value): self.impl = ((self.impl) & ~(lib.CRS_registry_MASK)) | (((value)) << lib.CRS_registry_SHIFT)

   @property
   def crsID(self): return ((((self.impl)) & lib.CRS_crsID_MASK) >> lib.CRS_crsID_SHIFT)
   @crsID.setter
   def crsID(self, value): self.impl = ((self.impl) & ~(lib.CRS_crsID_MASK)) | (((value)) << lib.CRS_crsID_SHIFT)

   @property
   def h(self): return ((((self.impl)) & lib.CRS_h_MASK) >> lib.CRS_h_SHIFT)
   @h.setter
   def h(self, value): self.impl = ((self.impl) & ~(lib.CRS_h_MASK)) | (((value)) << lib.CRS_h_SHIFT)

class CRSExtent(Struct):
   def __init__(self, crs = None, tl = None, br = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_CRSExtent *", impl)
      else:
         if isinstance(crs, tuple):
            __tuple = crs
            crs = 0
            if len(__tuple) > 0: crs = __tuple[0]
            if len(__tuple) > 1: tl  = __tuple[1]
            if len(__tuple) > 2: br  = __tuple[2]
         if crs is not None:
            if not isinstance(crs, CRS): crs = CRS(crs)
            crs = crs.impl
         else:
            crs = CRS()
         if tl is not None:
            if not isinstance(tl, Pointd): tl = Pointd(tl)
            tl = tl.impl[0]
         else:
            tl = Pointd()
            tl = tl.impl[0]
         if br is not None:
            if not isinstance(br, Pointd): br = Pointd(br)
            br = br.impl[0]
         else:
            br = Pointd()
            br = br.impl[0]
         self.impl = ffi.new("eC_CRSExtent *", { 'crs' : crs, 'tl' : tl, 'br' : br })

   @property
   def crs(self): return CRS(impl = self.impl.crs)
   @crs.setter
   def crs(self, value):
      if not isinstance(value, CRS): value = CRS(value)
      self.impl.crs = value.impl

   @property
   def tl(self): return Pointd(impl = self.impl.tl)
   @tl.setter
   def tl(self, value):
      if not isinstance(value, Pointd): value = Pointd(value)
      self.impl.tl = value.impl[0]

   @property
   def br(self): return Pointd(impl = self.impl.br)
   @br.setter
   def br(self, value):
      if not isinstance(value, Pointd): value = Pointd(value)
      self.impl.br = value.impl[0]

class CRSRegistry:
   epsg = lib.CRSRegistry_epsg
   ogc  = lib.CRSRegistry_ogc

class DGGSJSON(Instance):
   class_members = [
                      'dggrs',
                      'zoneId',
                      'depths',
                      'representedValue',
                      'schema',
                      'dimensions',
                      'values',
                   ]

   def init_args(self, args, kwArgs): init_args(DGGSJSON, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   @property
   def dggrs(self): return pyOrNewObject(String, IPTR(lib, ffi, self, DGGSJSON).dggrs)
   @dggrs.setter
   def dggrs(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, DGGSJSON).dggrs = value

   @property
   def zoneId(self): return pyOrNewObject(String, IPTR(lib, ffi, self, DGGSJSON).zoneId)
   @zoneId.setter
   def zoneId(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, DGGSJSON).zoneId = value

   @property
   def depths(self): return pyOrNewObject(Array, IPTR(lib, ffi, self, DGGSJSON).depths)
   @depths.setter
   def depths(self, value): IPTR(lib, ffi, self, DGGSJSON).depths = value.impl

   @property
   def representedValue(self): return pyOrNewObject(String, IPTR(lib, ffi, self, DGGSJSON).representedValue)
   @representedValue.setter
   def representedValue(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, DGGSJSON).representedValue = value

   @property
   def schema(self): return pyOrNewObject(JSONSchema, IPTR(lib, ffi, self, DGGSJSON).schema)
   @schema.setter
   def schema(self, value):
      if not isinstance(value, JSONSchema): value = JSONSchema(value)
      IPTR(lib, ffi, self, DGGSJSON).schema = value.impl

   @property
   def dimensions(self): return pyOrNewObject(Array, IPTR(lib, ffi, self, DGGSJSON).dimensions)
   @dimensions.setter
   def dimensions(self, value): IPTR(lib, ffi, self, DGGSJSON).dimensions = value.impl

   @property
   def values(self): return pyOrNewObject(Map, IPTR(lib, ffi, self, DGGSJSON).values)
   @values.setter
   def values(self, value): IPTR(lib, ffi, self, DGGSJSON).values = value.impl

class DGGSJSONDepth(Instance):
   class_members = [
                      'depth',
                      'shape',
                      'data',
                   ]

   def init_args(self, args, kwArgs): init_args(DGGSJSONDepth, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   @property
   def depth(self): return IPTR(lib, ffi, self, DGGSJSONDepth).depth
   @depth.setter
   def depth(self, value): IPTR(lib, ffi, self, DGGSJSONDepth).depth = value

   @property
   def shape(self): return pyOrNewObject(DGGSJSONShape, IPTR(lib, ffi, self, DGGSJSONDepth).shape)
   @shape.setter
   def shape(self, value):
      if not isinstance(value, DGGSJSONShape): value = DGGSJSONShape(value)
      IPTR(lib, ffi, self, DGGSJSONDepth).shape = value.impl

   @property
   def data(self): return pyOrNewObject(Array, IPTR(lib, ffi, self, DGGSJSONDepth).data)
   @data.setter
   def data(self, value): IPTR(lib, ffi, self, DGGSJSONDepth).data = value.impl

class DGGSJSONGrid(Instance):
   class_members = [
                      'cellsCount',
                      'resolution',
                      'coordinates',
                      'boundsCoordinates',
                      'relativeBounds',
                      'firstCoordinate',
                   ]

   def init_args(self, args, kwArgs): init_args(DGGSJSONGrid, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   @property
   def cellsCount(self): return IPTR(lib, ffi, self, DGGSJSONGrid).cellsCount
   @cellsCount.setter
   def cellsCount(self, value): IPTR(lib, ffi, self, DGGSJSONGrid).cellsCount = value

   @property
   def resolution(self): return IPTR(lib, ffi, self, DGGSJSONGrid).resolution
   @resolution.setter
   def resolution(self, value): IPTR(lib, ffi, self, DGGSJSONGrid).resolution = value

   @property
   def coordinates(self): return pyOrNewObject(Array, IPTR(lib, ffi, self, DGGSJSONGrid).coordinates)
   @coordinates.setter
   def coordinates(self, value): IPTR(lib, ffi, self, DGGSJSONGrid).coordinates = value.impl

   @property
   def boundsCoordinates(self): return pyOrNewObject(Array, IPTR(lib, ffi, self, DGGSJSONGrid).boundsCoordinates)
   @boundsCoordinates.setter
   def boundsCoordinates(self, value): IPTR(lib, ffi, self, DGGSJSONGrid).boundsCoordinates = value.impl

   @property
   def relativeBounds(self): return pyOrNewObject(Array, IPTR(lib, ffi, self, DGGSJSONGrid).relativeBounds)
   @relativeBounds.setter
   def relativeBounds(self, value): IPTR(lib, ffi, self, DGGSJSONGrid).relativeBounds = value.impl

   @property
   def firstCoordinate(self): return FieldValue(impl = IPTR(lib, ffi, self, DGGSJSONGrid).firstCoordinate)
   @firstCoordinate.setter
   def firstCoordinate(self, value):
      if not isinstance(value, FieldValue): value = FieldValue(value)
      IPTR(lib, ffi, self, DGGSJSONGrid).firstCoordinate = value.impl

class DGGSJSONShape(Instance):
   class_members = [
                      'count',
                      'subZones',
                      'dimensions',
                   ]

   def init_args(self, args, kwArgs): init_args(DGGSJSONShape, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   @property
   def count(self): return IPTR(lib, ffi, self, DGGSJSONShape).count
   @count.setter
   def count(self, value): IPTR(lib, ffi, self, DGGSJSONShape).count = value

   @property
   def subZones(self): return IPTR(lib, ffi, self, DGGSJSONShape).subZones
   @subZones.setter
   def subZones(self, value): IPTR(lib, ffi, self, DGGSJSONShape).subZones = value

   @property
   def dimensions(self): return pyOrNewObject(Map, IPTR(lib, ffi, self, DGGSJSONShape).dimensions)
   @dimensions.setter
   def dimensions(self, value): IPTR(lib, ffi, self, DGGSJSONShape).dimensions = value.impl

class FieldType:
   integer = lib.FieldType_integer
   real    = lib.FieldType_real
   text    = lib.FieldType_text
   blob    = lib.FieldType_blob
   nil     = lib.FieldType_nil
   array   = lib.FieldType_array
   map     = lib.FieldType_map

class FieldTypeEx:
   def __init__(self, type = 0, mustFree = False, format = 0, isUnsigned = False, isDateTime = False, impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(type, FieldTypeEx):
         self.impl = type.impl
      else:
         if isinstance(type, tuple):
            __tuple = type
            type = 0
            if len(__tuple) > 0: type = __tuple[0]
            if len(__tuple) > 1: mustFree = __tuple[1]
            if len(__tuple) > 2: format = __tuple[2]
            if len(__tuple) > 3: isUnsigned = __tuple[3]
            if len(__tuple) > 4: isDateTime = __tuple[4]
         self.impl = (
            (type       << lib.FIELDTYPEEX_type_SHIFT)       |
            (mustFree   << lib.FIELDTYPEEX_mustFree_SHIFT)   |
            (format     << lib.FIELDTYPEEX_format_SHIFT)     |
            (isUnsigned << lib.FIELDTYPEEX_isUnsigned_SHIFT) |
            (isDateTime << lib.FIELDTYPEEX_isDateTime_SHIFT) )

   @property
   def type(self): return ((((self.impl)) & lib.FIELDTYPEEX_type_MASK) >> lib.FIELDTYPEEX_type_SHIFT)
   @type.setter
   def type(self, value): self.impl = ((self.impl) & ~(lib.FIELDTYPEEX_type_MASK)) | (((value)) << lib.FIELDTYPEEX_type_SHIFT)

   @property
   def mustFree(self): return ((((self.impl)) & lib.FIELDTYPEEX_mustFree_MASK) >> lib.FIELDTYPEEX_mustFree_SHIFT)
   @mustFree.setter
   def mustFree(self, value): self.impl = ((self.impl) & ~(lib.FIELDTYPEEX_mustFree_MASK)) | (((value)) << lib.FIELDTYPEEX_mustFree_SHIFT)

   @property
   def format(self): return ((((self.impl)) & lib.FIELDTYPEEX_format_MASK) >> lib.FIELDTYPEEX_format_SHIFT)
   @format.setter
   def format(self, value): self.impl = ((self.impl) & ~(lib.FIELDTYPEEX_format_MASK)) | (((value)) << lib.FIELDTYPEEX_format_SHIFT)

   @property
   def isUnsigned(self): return ((((self.impl)) & lib.FIELDTYPEEX_isUnsigned_MASK) >> lib.FIELDTYPEEX_isUnsigned_SHIFT)
   @isUnsigned.setter
   def isUnsigned(self, value): self.impl = ((self.impl) & ~(lib.FIELDTYPEEX_isUnsigned_MASK)) | (((value)) << lib.FIELDTYPEEX_isUnsigned_SHIFT)

   @property
   def isDateTime(self): return ((((self.impl)) & lib.FIELDTYPEEX_isDateTime_MASK) >> lib.FIELDTYPEEX_isDateTime_SHIFT)
   @isDateTime.setter
   def isDateTime(self, value): self.impl = ((self.impl) & ~(lib.FIELDTYPEEX_isDateTime_MASK)) | (((value)) << lib.FIELDTYPEEX_isDateTime_SHIFT)

class FieldValue(Struct):
   def __init__(self, type = None, i = None, r = None, s = None, b = None, a = None, m = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_FieldValue *", impl)
      else:
         if type is not None:
            if not isinstance(type, FieldTypeEx): type = FieldTypeEx(type)
         if s is not None:
            if not isinstance(s, String): s = String(s)
         __members = { }
         if type is not None: __members['type'] = type.impl
         if i is not None:    __members['i']    = i
         if r is not None:    __members['r']    = r
         if s is not None:    __members['s']    = s
         if b is not None:    __members['b']    = b
         if a is not None:    __members['a']    = a.impl
         if m is not None:    __members['m']    = m.impl
         self.impl = ffi.new("eC_FieldValue *", __members)

   @property
   def type(self): return FieldTypeEx(impl = self.impl.type)
   @type.setter
   def type(self, value):
      if not isinstance(value, FieldTypeEx): value = FieldTypeEx(value)
      self.impl.type = value.impl

   @property
   def i(self): return self.impl.i
   @i.setter
   def i(self, value): self.impl.i = value

   @property
   def r(self): return self.impl.r
   @r.setter
   def r(self, value): self.impl.r = value

   @property
   def s(self): return String(self.impl.s)
   @s.setter
   def s(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.s = value

   @property
   def b(self): return self.impl.b
   @b.setter
   def b(self, value): self.impl.b = value

   @property
   def a(self): return pyOrNewObject(Array, self.impl.a)
   @a.setter
   def a(self, value): self.impl.a = value.impl

   @property
   def m(self): return pyOrNewObject(Map, self.impl.m)
   @m.setter
   def m(self, value): self.impl.m = value.impl

   def compareInt(self, other = None):
      if other is not None and not isinstance(other, FieldValue): other = FieldValue(other)
      other = ffi.NULL if other is None else other.impl
      return lib.FieldValue_compareInt(ffi.cast("eC_FieldValue *", self.impl), ffi.cast("eC_FieldValue *", other))

   def compareReal(self, other = None):
      if other is not None and not isinstance(other, FieldValue): other = FieldValue(other)
      other = ffi.NULL if other is None else other.impl
      return lib.FieldValue_compareReal(ffi.cast("eC_FieldValue *", self.impl), ffi.cast("eC_FieldValue *", other))

   def compareText(self, other = None):
      if other is not None and not isinstance(other, FieldValue): other = FieldValue(other)
      other = ffi.NULL if other is None else other.impl
      return lib.FieldValue_compareText(ffi.cast("eC_FieldValue *", self.impl), ffi.cast("eC_FieldValue *", other))

   def formatArray(self, tempString, fieldData, onType):
      if isinstance(tempString, str): tempString = ffi.new("char[]", tempString.encode('u8'))
      elif tempString is None: tempString = ffi.NULL
      if hasattr(fieldData, 'impl'): fieldData = fieldData.impl
      if fieldData is None: fieldData = ffi.NULL
      return pyOrNewObject(String, lib.FieldValue_formatArray(self.impl, tempString, fieldData, onType))

   def formatFloat(self, stringOutput, fixDot):
      if isinstance(stringOutput, str): stringOutput = ffi.new("char[]", stringOutput.encode('u8'))
      elif stringOutput is None: stringOutput = ffi.NULL
      return pyOrNewObject(String, lib.FieldValue_formatFloat(self.impl, stringOutput, fixDot))

   def formatInteger(self, stringOutput):
      if isinstance(stringOutput, str): stringOutput = ffi.new("char[]", stringOutput.encode('u8'))
      elif stringOutput is None: stringOutput = ffi.NULL
      return pyOrNewObject(String, lib.FieldValue_formatInteger(self.impl, stringOutput))

   def formatMap(self, tempString, fieldData, onType):
      if isinstance(tempString, str): tempString = ffi.new("char[]", tempString.encode('u8'))
      elif tempString is None: tempString = ffi.NULL
      if hasattr(fieldData, 'impl'): fieldData = fieldData.impl
      if fieldData is None: fieldData = ffi.NULL
      return pyOrNewObject(String, lib.FieldValue_formatMap(self.impl, tempString, fieldData, onType))

   def getArrayOrMap(string, destClass = None):
      if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
      elif string is None: string = ffi.NULL
      if destClass is not None and not isinstance(destClass, Class): destClass = Class(destClass)
      destClass = ffi.NULL if destClass is None else destClass.impl
      destination = ffi.new("void * *")
      r = lib.FieldValue_getArrayOrMap(string, ffi.cast("struct eC_Class *", destClass), destination)
      if destination[0] == ffi.NULL: _destination = None
      else:
         if destClass.type == ClassType.normalClass:
            i = ffi.cast("eC_Instance", destination[0])
            n = ffi.string(i._class.name).decode('u8')
         else:
            n = ffi.string(destClass.name).decode('u8')
         t = pyTypeByName(n)
         ct = n + " * " if destClass.type == ClassType.noHeadClass else n
         _destination = t(impl = pyFFI().cast(ct, destination[0]))
      return r, _destination

   def stringify(self):
      return pyOrNewObject(String, lib.FieldValue_stringify(self.impl))

class FieldValueFormat:
   decimal     = lib.FieldValueFormat_decimal
   unset       = lib.FieldValueFormat_unset
   hex         = lib.FieldValueFormat_hex
   octal       = lib.FieldValueFormat_octal
   binary      = lib.FieldValueFormat_binary
   exponential = lib.FieldValueFormat_exponential
   boolean     = lib.FieldValueFormat_boolean
   textObj     = lib.FieldValueFormat_textObj
   color       = lib.FieldValueFormat_color

class GGGZone(DGGRSZone):
   def __init__(self, level = 0, row = 0, col = 0, impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(level, GGGZone):
         self.impl = level.impl
      else:
         if isinstance(level, tuple):
            __tuple = level
            level = 0
            if len(__tuple) > 0: level = __tuple[0]
            if len(__tuple) > 1: row = __tuple[1]
            if len(__tuple) > 2: col = __tuple[2]
         self.impl = (
            (level << lib.GGGZONE_level_SHIFT) |
            (row   << lib.GGGZONE_row_SHIFT)   |
            (col   << lib.GGGZONE_col_SHIFT)   )

   @property
   def level(self): return ((((self.impl)) & lib.GGGZONE_level_MASK) >> lib.GGGZONE_level_SHIFT)
   @level.setter
   def level(self, value): self.impl = ((self.impl) & ~(lib.GGGZONE_level_MASK)) | (((value)) << lib.GGGZONE_level_SHIFT)

   @property
   def row(self): return ((((self.impl)) & lib.GGGZONE_row_MASK) >> lib.GGGZONE_row_SHIFT)
   @row.setter
   def row(self, value): self.impl = ((self.impl) & ~(lib.GGGZONE_row_MASK)) | (((value)) << lib.GGGZONE_row_SHIFT)

   @property
   def col(self): return ((((self.impl)) & lib.GGGZONE_col_MASK) >> lib.GGGZONE_col_SHIFT)
   @col.setter
   def col(self, value): self.impl = ((self.impl) & ~(lib.GGGZONE_col_MASK)) | (((value)) << lib.GGGZONE_col_SHIFT)

class GNOSISGlobalGrid(DGGRS):
   class_members = []

   def init_args(self, args, kwArgs): init_args(GNOSISGlobalGrid, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

class ISEA3H(DGGRS):
   class_members = []

   def init_args(self, args, kwArgs): init_args(ISEA3H, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

class ISEA3HZone(DGGRSZone):
   def __init__(self, levelISEA9R = 0, rootRhombus = 0, rhombusIX = 0, subHex = 0, impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(levelISEA9R, ISEA3HZone):
         self.impl = levelISEA9R.impl
      else:
         if isinstance(levelISEA9R, tuple):
            __tuple = levelISEA9R
            levelISEA9R = 0
            if len(__tuple) > 0: levelISEA9R = __tuple[0]
            if len(__tuple) > 1: rootRhombus = __tuple[1]
            if len(__tuple) > 2: rhombusIX = __tuple[2]
            if len(__tuple) > 3: subHex = __tuple[3]
         self.impl = (
            (levelISEA9R << lib.ISEA3HZONE_levelISEA9R_SHIFT) |
            (rootRhombus << lib.ISEA3HZONE_rootRhombus_SHIFT) |
            (rhombusIX   << lib.ISEA3HZONE_rhombusIX_SHIFT)   |
            (subHex      << lib.ISEA3HZONE_subHex_SHIFT)      )

   @property
   def levelISEA9R(self): return ((((self.impl)) & lib.ISEA3HZONE_levelISEA9R_MASK) >> lib.ISEA3HZONE_levelISEA9R_SHIFT)
   @levelISEA9R.setter
   def levelISEA9R(self, value): self.impl = ((self.impl) & ~(lib.ISEA3HZONE_levelISEA9R_MASK)) | (((value)) << lib.ISEA3HZONE_levelISEA9R_SHIFT)

   @property
   def rootRhombus(self): return ((((self.impl)) & lib.ISEA3HZONE_rootRhombus_MASK) >> lib.ISEA3HZONE_rootRhombus_SHIFT)
   @rootRhombus.setter
   def rootRhombus(self, value): self.impl = ((self.impl) & ~(lib.ISEA3HZONE_rootRhombus_MASK)) | (((value)) << lib.ISEA3HZONE_rootRhombus_SHIFT)

   @property
   def rhombusIX(self): return ((((self.impl)) & lib.ISEA3HZONE_rhombusIX_MASK) >> lib.ISEA3HZONE_rhombusIX_SHIFT)
   @rhombusIX.setter
   def rhombusIX(self, value): self.impl = ((self.impl) & ~(lib.ISEA3HZONE_rhombusIX_MASK)) | (((value)) << lib.ISEA3HZONE_rhombusIX_SHIFT)

   @property
   def subHex(self): return ((((self.impl)) & lib.ISEA3HZONE_subHex_MASK) >> lib.ISEA3HZONE_subHex_SHIFT)
   @subHex.setter
   def subHex(self, value): self.impl = ((self.impl) & ~(lib.ISEA3HZONE_subHex_MASK)) | (((value)) << lib.ISEA3HZONE_subHex_SHIFT)

class ISEA9R(DGGRS):
   class_members = []

   def init_args(self, args, kwArgs): init_args(ISEA9R, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

class ISEA9RZone(DGGRSZone):
   def __init__(self, level = 0, row = 0, col = 0, impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(level, ISEA9RZone):
         self.impl = level.impl
      else:
         if isinstance(level, tuple):
            __tuple = level
            level = 0
            if len(__tuple) > 0: level = __tuple[0]
            if len(__tuple) > 1: row = __tuple[1]
            if len(__tuple) > 2: col = __tuple[2]
         self.impl = (
            (level << lib.ISEA9RZONE_level_SHIFT) |
            (row   << lib.ISEA9RZONE_row_SHIFT)   |
            (col   << lib.ISEA9RZONE_col_SHIFT)   )

   @property
   def level(self): return ((((self.impl)) & lib.ISEA9RZONE_level_MASK) >> lib.ISEA9RZONE_level_SHIFT)
   @level.setter
   def level(self, value): self.impl = ((self.impl) & ~(lib.ISEA9RZONE_level_MASK)) | (((value)) << lib.ISEA9RZONE_level_SHIFT)

   @property
   def row(self): return ((((self.impl)) & lib.ISEA9RZONE_row_MASK) >> lib.ISEA9RZONE_row_SHIFT)
   @row.setter
   def row(self, value): self.impl = ((self.impl) & ~(lib.ISEA9RZONE_row_MASK)) | (((value)) << lib.ISEA9RZONE_row_SHIFT)

   @property
   def col(self): return ((((self.impl)) & lib.ISEA9RZONE_col_MASK) >> lib.ISEA9RZONE_col_SHIFT)
   @col.setter
   def col(self, value): self.impl = ((self.impl) & ~(lib.ISEA9RZONE_col_MASK)) | (((value)) << lib.ISEA9RZONE_col_SHIFT)

class JSONSchema(Instance):
   class_members = [
                      'schema',
                      'id',
                      'title',
                      'comment',
                      'description',
                      'Default',
                      'readOnly',
                      'writeOnly',
                      'examples',
                      'multipleOf',
                      'type',
                      'Enum',
                      'format',
                      'contentMediaType',
                      'maximum',
                      'maximum',
                      'exclusiveMaximum',
                      'exclusiveMaximum',
                      'minimum',
                      'minimum',
                      'exclusiveMinimum',
                      'exclusiveMinimum',
                      'pattern',
                      'items',
                      'maxItems',
                      'maxItems',
                      'minItems',
                      'minItems',
                      'uniqueItems',
                      'contains',
                      'maxProperties',
                      'maxProperties',
                      'minProperties',
                      'minProperties',
                      'required',
                      'additionalProperties',
                      'definitions',
                      'properties',
                      'patternProperties',
                      'dependencies',
                      'propertyNames',
                      'contentEncoding',
                      'If',
                      'Then',
                      'Else',
                      'allOf',
                      'anyOf',
                      'oneOf',
                      'Not',
                      'xogcrole',
                      'xogcpropertySeq',
                      'xogcpropertySeq',
                      'Default',
                   ]

   def init_args(self, args, kwArgs): init_args(JSONSchema, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   @property
   def schema(self): return pyOrNewObject(String, IPTR(lib, ffi, self, JSONSchema).schema)
   @schema.setter
   def schema(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, JSONSchema).schema = value

   @property
   def id(self): return pyOrNewObject(String, IPTR(lib, ffi, self, JSONSchema).id)
   @id.setter
   def id(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, JSONSchema).id = value

   @property
   def title(self): return pyOrNewObject(String, IPTR(lib, ffi, self, JSONSchema).title)
   @title.setter
   def title(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, JSONSchema).title = value

   @property
   def comment(self): return pyOrNewObject(String, IPTR(lib, ffi, self, JSONSchema).comment)
   @comment.setter
   def comment(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, JSONSchema).comment = value

   @property
   def description(self): return pyOrNewObject(String, IPTR(lib, ffi, self, JSONSchema).description)
   @description.setter
   def description(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, JSONSchema).description = value

   @property
   def readOnly(self): return IPTR(lib, ffi, self, JSONSchema).readOnly
   @readOnly.setter
   def readOnly(self, value): IPTR(lib, ffi, self, JSONSchema).readOnly = value

   @property
   def writeOnly(self): return IPTR(lib, ffi, self, JSONSchema).writeOnly
   @writeOnly.setter
   def writeOnly(self, value): IPTR(lib, ffi, self, JSONSchema).writeOnly = value

   @property
   def examples(self): return pyOrNewObject(Array, IPTR(lib, ffi, self, JSONSchema).examples)
   @examples.setter
   def examples(self, value): IPTR(lib, ffi, self, JSONSchema).examples = value.impl

   @property
   def multipleOf(self): return pyOrNewObject(Array, IPTR(lib, ffi, self, JSONSchema).multipleOf)
   @multipleOf.setter
   def multipleOf(self, value): IPTR(lib, ffi, self, JSONSchema).multipleOf = value.impl

   @property
   def type(self): return JSONSchemaType(impl = IPTR(lib, ffi, self, JSONSchema).type)
   @type.setter
   def type(self, value): IPTR(lib, ffi, self, JSONSchema).type = value.impl

   @property
   def Enum(self): return pyOrNewObject(Array, IPTR(lib, ffi, self, JSONSchema).Enum)
   @Enum.setter
   def Enum(self, value): IPTR(lib, ffi, self, JSONSchema).Enum = value.impl

   @property
   def format(self): return pyOrNewObject(String, IPTR(lib, ffi, self, JSONSchema).format)
   @format.setter
   def format(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, JSONSchema).format = value

   @property
   def contentMediaType(self): return pyOrNewObject(String, IPTR(lib, ffi, self, JSONSchema).contentMediaType)
   @contentMediaType.setter
   def contentMediaType(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, JSONSchema).contentMediaType = value

   @property
   def maximum(self): return lib.JSONSchema_get_maximum(self.impl)
   # @maximum.isset # tofix: how do we get isset?
   # def maximum(self): lib.JSONSchema_isSet_maximum(self.impl)

   @property
   def exclusiveMaximum(self): return lib.JSONSchema_get_exclusiveMaximum(self.impl)
   # @exclusiveMaximum.isset # tofix: how do we get isset?
   # def exclusiveMaximum(self): lib.JSONSchema_isSet_exclusiveMaximum(self.impl)

   @property
   def minimum(self): return lib.JSONSchema_get_minimum(self.impl)
   # @minimum.isset # tofix: how do we get isset?
   # def minimum(self): lib.JSONSchema_isSet_minimum(self.impl)

   @property
   def exclusiveMinimum(self): return lib.JSONSchema_get_exclusiveMinimum(self.impl)
   # @exclusiveMinimum.isset # tofix: how do we get isset?
   # def exclusiveMinimum(self): lib.JSONSchema_isSet_exclusiveMinimum(self.impl)

   @property
   def pattern(self): return pyOrNewObject(String, IPTR(lib, ffi, self, JSONSchema).pattern)
   @pattern.setter
   def pattern(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, JSONSchema).pattern = value

   @property
   def items(self): return pyOrNewObject(JSONSchema, IPTR(lib, ffi, self, JSONSchema).items)
   @items.setter
   def items(self, value):
      if not isinstance(value, JSONSchema): value = JSONSchema(value)
      IPTR(lib, ffi, self, JSONSchema).items = value.impl

   @property
   def maxItems(self): return lib.JSONSchema_get_maxItems(self.impl)
   # @maxItems.isset # tofix: how do we get isset?
   # def maxItems(self): lib.JSONSchema_isSet_maxItems(self.impl)

   @property
   def minItems(self): return lib.JSONSchema_get_minItems(self.impl)
   # @minItems.isset # tofix: how do we get isset?
   # def minItems(self): lib.JSONSchema_isSet_minItems(self.impl)

   @property
   def uniqueItems(self): return IPTR(lib, ffi, self, JSONSchema).uniqueItems
   @uniqueItems.setter
   def uniqueItems(self, value): IPTR(lib, ffi, self, JSONSchema).uniqueItems = value

   @property
   def contains(self): return pyOrNewObject(String, IPTR(lib, ffi, self, JSONSchema).contains)
   @contains.setter
   def contains(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, JSONSchema).contains = value

   @property
   def maxProperties(self): return lib.JSONSchema_get_maxProperties(self.impl)
   # @maxProperties.isset # tofix: how do we get isset?
   # def maxProperties(self): lib.JSONSchema_isSet_maxProperties(self.impl)

   @property
   def minProperties(self): return lib.JSONSchema_get_minProperties(self.impl)
   # @minProperties.isset # tofix: how do we get isset?
   # def minProperties(self): lib.JSONSchema_isSet_minProperties(self.impl)

   @property
   def required(self): return pyOrNewObject(Array, IPTR(lib, ffi, self, JSONSchema).required)
   @required.setter
   def required(self, value): IPTR(lib, ffi, self, JSONSchema).required = value.impl

   @property
   def additionalProperties(self): return pyOrNewObject(JSONSchema, IPTR(lib, ffi, self, JSONSchema).additionalProperties)
   @additionalProperties.setter
   def additionalProperties(self, value):
      if not isinstance(value, JSONSchema): value = JSONSchema(value)
      IPTR(lib, ffi, self, JSONSchema).additionalProperties = value.impl

   @property
   def definitions(self): return pyOrNewObject(Map, IPTR(lib, ffi, self, JSONSchema).definitions)
   @definitions.setter
   def definitions(self, value): IPTR(lib, ffi, self, JSONSchema).definitions = value.impl

   @property
   def properties(self): return pyOrNewObject(Map, IPTR(lib, ffi, self, JSONSchema).properties)
   @properties.setter
   def properties(self, value): IPTR(lib, ffi, self, JSONSchema).properties = value.impl

   @property
   def patternProperties(self): return pyOrNewObject(Map, IPTR(lib, ffi, self, JSONSchema).patternProperties)
   @patternProperties.setter
   def patternProperties(self, value): IPTR(lib, ffi, self, JSONSchema).patternProperties = value.impl

   @property
   def dependencies(self): return pyOrNewObject(Map, IPTR(lib, ffi, self, JSONSchema).dependencies)
   @dependencies.setter
   def dependencies(self, value): IPTR(lib, ffi, self, JSONSchema).dependencies = value.impl

   @property
   def propertyNames(self): return pyOrNewObject(String, IPTR(lib, ffi, self, JSONSchema).propertyNames)
   @propertyNames.setter
   def propertyNames(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, JSONSchema).propertyNames = value

   @property
   def contentEncoding(self): return pyOrNewObject(String, IPTR(lib, ffi, self, JSONSchema).contentEncoding)
   @contentEncoding.setter
   def contentEncoding(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, JSONSchema).contentEncoding = value

   @property
   def If(self): return pyOrNewObject(JSONSchema, IPTR(lib, ffi, self, JSONSchema).If)
   @If.setter
   def If(self, value):
      if not isinstance(value, JSONSchema): value = JSONSchema(value)
      IPTR(lib, ffi, self, JSONSchema).If = value.impl

   @property
   def Then(self): return pyOrNewObject(JSONSchema, IPTR(lib, ffi, self, JSONSchema).Then)
   @Then.setter
   def Then(self, value):
      if not isinstance(value, JSONSchema): value = JSONSchema(value)
      IPTR(lib, ffi, self, JSONSchema).Then = value.impl

   @property
   def Else(self): return pyOrNewObject(JSONSchema, IPTR(lib, ffi, self, JSONSchema).Else)
   @Else.setter
   def Else(self, value):
      if not isinstance(value, JSONSchema): value = JSONSchema(value)
      IPTR(lib, ffi, self, JSONSchema).Else = value.impl

   @property
   def allOf(self): return pyOrNewObject(Array, IPTR(lib, ffi, self, JSONSchema).allOf)
   @allOf.setter
   def allOf(self, value): IPTR(lib, ffi, self, JSONSchema).allOf = value.impl

   @property
   def anyOf(self): return pyOrNewObject(Array, IPTR(lib, ffi, self, JSONSchema).anyOf)
   @anyOf.setter
   def anyOf(self, value): IPTR(lib, ffi, self, JSONSchema).anyOf = value.impl

   @property
   def oneOf(self): return pyOrNewObject(Array, IPTR(lib, ffi, self, JSONSchema).oneOf)
   @oneOf.setter
   def oneOf(self, value): IPTR(lib, ffi, self, JSONSchema).oneOf = value.impl

   @property
   def Not(self): return pyOrNewObject(JSONSchema, IPTR(lib, ffi, self, JSONSchema).Not)
   @Not.setter
   def Not(self, value):
      if not isinstance(value, JSONSchema): value = JSONSchema(value)
      IPTR(lib, ffi, self, JSONSchema).Not = value.impl

   @property
   def xogcrole(self): return pyOrNewObject(String, IPTR(lib, ffi, self, JSONSchema).xogcrole)
   @xogcrole.setter
   def xogcrole(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, JSONSchema).xogcrole = value

   @property
   def xogcpropertySeq(self): return None
   # @xogcpropertySeq.isset # tofix: how do we get isset?
   # def xogcpropertySeq(self): lib.JSONSchema_isSet_xogcpropertySeq(self.impl)

   @property
   def Default(self): return None
   # @Default.isset # tofix: how do we get isset?
   # def Default(self): lib.JSONSchema_isSet_Default(self.impl)

class JSONSchemaType:
   unset   = lib.JSONSchemaType_unset
   array   = lib.JSONSchemaType_array
   boolean = lib.JSONSchemaType_boolean
   integer = lib.JSONSchemaType_integer
   null    = lib.JSONSchemaType_null
   number  = lib.JSONSchemaType_number
   object  = lib.JSONSchemaType_object
   string  = lib.JSONSchemaType_string

def readDGGSJSON(f = None):
   if f is not None and not isinstance(f, File): f = File(f)
   f = ffi.NULL if f is None else f.impl
   return pyOrNewObject(DGGSJSON, lib.eC_readDGGSJSON(f))

def pydggal_setup(app):
   app.appGlobals.append(globals())
   if lib.dggal_init(app.impl) == ffi.NULL: raise Exception("Failed to load library")
   app.registerClass(DGGRS, True)
   app.registerClass(DGGSJSON, True)
   app.registerClass(DGGSJSONDepth, True)
   app.registerClass(DGGSJSONGrid, True)
   app.registerClass(DGGSJSONShape, True)
   app.registerClass(GNOSISGlobalGrid, True)
   app.registerClass(ISEA3H, True)
   app.registerClass(ISEA9R, True)
   app.registerClass(JSONSchema, True)
