import dggalFactory from './libdggal.js';

export class DGGRS {
  constructor(module, modulePtr, name) {
    this.module = module;
    const stack = this.module.stackSave();
    const namePtr = this.stackAllocString(name);
    this.dggrsPtr = this.module._DGGAL_DGGRS_new(modulePtr, namePtr);
    this.module.stackRestore(stack);
  }

  delete() {
    if (this.dggrsPtr) {
      this.module._DGGAL_DGGRS_delete(this.dggrsPtr);
      this.dggrsPtr = 0;
    }
  }

  stackAllocString(str) {
    const maxBytes = str.length * 4 + 1;
    const ptr = this.module.stackAlloc(maxBytes);
    this.module.stringToUTF8(str, ptr, maxBytes);
    return ptr;
  }

  // Basic zone lookup / text id
  getZoneFromTextID(zoneID) {
    const stack = this.module.stackSave();
    const zonePtr = this.stackAllocString(zoneID);
    const result = BigInt.asUintN(64, this.module._DGGAL_DGGRS_getZoneFromTextID(this.dggrsPtr, zonePtr));
    this.module.stackRestore(stack);
    return result;
  }

  getZoneTextID(zone) {
    const stack = this.module.stackSave();
    const buffer = this.module.stackAlloc(256);
    this.module._DGGAL_DGGRS_getZoneTextID(this.dggrsPtr, zone, buffer);
    const result = this.module.UTF8ToString(buffer);
    this.module.stackRestore(stack);
    return result;
  }

  // Zone metadata
  getZoneLevel(zone) {
    return this.module._DGGAL_DGGRS_getZoneLevel(this.dggrsPtr, zone);
  }

  getZoneArea(zone) {
    return this.module._DGGAL_DGGRS_getZoneArea(this.dggrsPtr, zone);
  }

  countZoneEdges(zone) {
    return this.module._DGGAL_DGGRS_countZoneEdges(this.dggrsPtr, zone);
  }

  countSubZones(zone, depth) {
    return BigInt.asUintN(64, this.module._DGGAL_DGGRS_countSubZones(this.dggrsPtr, zone, depth));
  }

  // Parents / children / neighbors
  getZoneParents(zone) {
    const stack = this.module.stackSave();
    const buffer = this.module.stackAlloc(3 * 8);
    this.module._DGGAL_DGGRS_getZoneParents(this.dggrsPtr, zone, buffer);
    const result = Array.from({ length: 3 }, (_, i) =>
      BigInt.asUintN(64, this.module.HEAP64[(buffer + i * 8) >> 3])
    );
    this.module.stackRestore(stack);
    return result;
  }

  getZoneChildren(zone) {
    const stack = this.module.stackSave();
    const buffer = this.module.stackAlloc(13 * 8);
    this.module._DGGAL_DGGRS_getZoneChildren(this.dggrsPtr, zone, buffer);
    const result = Array.from({ length: 13 }, (_, i) =>
      BigInt.asUintN(64, this.module.HEAP64[(buffer + i * 8) >> 3])
    );
    this.module.stackRestore(stack);
    return result;
  }

  getZoneNeighbors(zone) {
    const stack = this.module.stackSave();
    const zonesBuf = this.module.stackAlloc(6 * 8);
    const typesBuf = this.module.stackAlloc(6 * 4);
    this.module._DGGAL_DGGRS_getZoneNeighbors(this.dggrsPtr, zone, zonesBuf, typesBuf);
    const result = Array.from({ length: 6 }, (_, i) => ({
      zone: BigInt.asUintN(64, this.module.HEAP64[(zonesBuf + i * 8) >> 3]),
      type: this.module.HEAP32[(typesBuf + i * 4) >> 2],
    }));
    this.module.stackRestore(stack);
    return result;
  }

  // Centroid related
  getZoneWGS84Centroid(zone) {
    const stack = this.module.stackSave();
    const buf = this.module.stackAlloc(16); // GeoPoint { double lat; double lon; }
    this.module._DGGAL_DGGRS_getZoneWGS84Centroid(this.dggrsPtr, zone, buf);
    const lat = this.module.HEAPF64[buf >> 3];
    const lon = this.module.HEAPF64[(buf + 8) >> 3];
    this.module.stackRestore(stack);
    return { lat, lon };
  }

  getZoneCentroidParent(zone) {
    return BigInt.asUintN(64, this.module._DGGAL_DGGRS_getZoneCentroidParent(this.dggrsPtr, zone));
  }

  getZoneCentroidChild(zone) {
    return BigInt.asUintN(64, this.module._DGGAL_DGGRS_getZoneCentroidChild(this.dggrsPtr, zone));
  }

  isZoneCentroidChild(zone) {
    return this.module._DGGAL_DGGRS_isZoneCentroidChild(this.dggrsPtr, zone) !== 0;
  }

  // Relationship queries
  areZonesNeighbors(a, b) {
    return this.module._DGGAL_DGGRS_areZonesNeighbors(this.dggrsPtr, a, b) !== 0;
  }

  areZonesSiblings(a, b) {
    return this.module._DGGAL_DGGRS_areZonesSiblings(this.dggrsPtr, a, b) !== 0;
  }

  doZonesOverlap(a, b) {
    return this.module._DGGAL_DGGRS_doZonesOverlap(this.dggrsPtr, a, b) !== 0;
  }

  doesZoneContain(haystack, needle) {
    return this.module._DGGAL_DGGRS_doesZoneContain(this.dggrsPtr, haystack, needle) !== 0;
  }

  isZoneAncestorOf(ancestor, descendant, maxDepth) {
    return this.module._DGGAL_DGGRS_isZoneAncestorOf(this.dggrsPtr, ancestor, descendant, maxDepth) !== 0;
  }

  isZoneContainedIn(needle, haystack) {
    return this.module._DGGAL_DGGRS_isZoneContainedIn(this.dggrsPtr, needle, haystack) !== 0;
  }

  isZoneDescendantOf(descendant, ancestor, maxDepth) {
    return this.module._DGGAL_DGGRS_isZoneDescendantOf(this.dggrsPtr, descendant, ancestor, maxDepth) !== 0;
  }

  isZoneImmediateChildOf(child, parent) {
    return this.module._DGGAL_DGGRS_isZoneImmediateChildOf(this.dggrsPtr, child, parent) !== 0;
  }

  isZoneImmediateParentOf(parent, child) {
    return this.module._DGGAL_DGGRS_isZoneImmediateParentOf(this.dggrsPtr, parent, child) !== 0;
  }

  zoneHasSubZone(haystack, needle) {
    return this.module._DGGAL_DGGRS_zoneHasSubZone(this.dggrsPtr, haystack, needle) !== 0;
  }

  // Limits / metadata
  getMaxDGGRSZoneLevel() {
    return this.module._DGGAL_DGGRS_getMaxDGGRSZoneLevel(this.dggrsPtr);
  }

  getMaxDepth() {
    return this.module._DGGAL_DGGRS_getMaxDepth(this.dggrsPtr);
  }

  get64KDepth() {
    return this.module._DGGAL_DGGRS_get64KDepth(this.dggrsPtr);
  }

  getMaxChildren() {
    return this.module._DGGAL_DGGRS_getMaxChildren(this.dggrsPtr);
  }

  getMaxParents() {
    return this.module._DGGAL_DGGRS_getMaxParents(this.dggrsPtr);
  }

  getMaxNeighbors() {
    return this.module._DGGAL_DGGRS_getMaxNeighbors(this.dggrsPtr);
  }

  getZoneCRSVertices(zone, crs) {
    const stack = this.module.stackSave();
    const buf = this.module.stackAlloc(6 * 16); // reserve space for up to 6 Pointd structs (16 bytes each)
    const count = this.module._DGGAL_DGGRS_getZoneCRSVertices(this.dggrsPtr, zone, crs, buf);
    const verts = new Array(count);
    for (let i = 0; i < count; i++) {
      const ptr = buf + i * 16;
      verts[i] = {
        x: this.module.HEAPF64[ptr >> 3],
        y: this.module.HEAPF64[(ptr + 8) >> 3]
      };
    }
    this.module.stackRestore(stack);
    return verts;
  }

  getZoneWGS84Vertices(zone) {
    const stack = this.module.stackSave();
    const buf = this.module.stackAlloc(6 * 16);
    const count = this.module._DGGAL_DGGRS_getZoneWGS84Vertices(this.dggrsPtr, zone, buf);
    const verts = new Array(count);
    for (let i = 0; i < count; i++) {
      const ptr = buf + i * 16;
      verts[i] = {
        lat: this.module.HEAPF64[ptr >> 3],
        lon: this.module.HEAPF64[(ptr + 8) >> 3],
      };
    }
    this.module.stackRestore(stack);
    return verts;
  }

  /* ---- DGGAL-managed array wrappers ----
     These map the header's Array_* helpers to JS arrays.
     Array_GeoPoint => GeoPoint { double lat; double lon; }
     Array_Pointd   => Pointd { double x; double y; }
     Array_DGGRSZone=> uint64_t zone identifiers
  */

  // GeoPoint array (used by getZoneRefinedWGS84Vertices)
  _readArrayGeoPoint(arrPtr) {
    const count = Number(
      this.module._DGGAL_Array_GeoPoint_getCount(arrPtr)
    );
    const basePtr = this.module._DGGAL_Array_GeoPoint_getPointer(arrPtr);
    if (!basePtr || count === 0) {
      this.module._DGGAL_Array_GeoPoint_delete(arrPtr);
      return [];
    }
    const out = new Array(count);
    const bytesPerPoint = 16; // two doubles: lat, lon
    for (let i = 0; i < count; i++) {
      const gpPtr = basePtr + i * bytesPerPoint;
      const lat = this.module.HEAPF64[gpPtr >> 3];
      const lon = this.module.HEAPF64[(gpPtr + 8) >> 3];
      out[i] = { lat, lon };
    }
    this.module._DGGAL_Array_GeoPoint_delete(arrPtr);
    return out;
  }

  // Pointd array (used by getZoneRefinedCRSVertices)
  _readArrayPointd(arrPtr) {
    const count = Number(this.module._DGGAL_Array_Pointd_getCount(arrPtr));
    const basePtr = this.module._DGGAL_Array_Pointd_getPointer(arrPtr);
    if (!basePtr || count === 0) {
      this.module._DGGAL_Array_Pointd_delete(arrPtr);
      return [];
    }
    const out = new Array(count);
    const bytesPerPoint = 16; // two doubles: x, y
    for (let i = 0; i < count; i++) {
      const pPtr = basePtr + i * bytesPerPoint;
      const x = this.module.HEAPF64[pPtr >> 3];
      const y = this.module.HEAPF64[(pPtr + 8) >> 3];
      out[i] = { x, y };
    }
    this.module._DGGAL_Array_Pointd_delete(arrPtr);
    return out;
  }

  // DGGRSZone array (used by getSubZones, listZones, compactZones)
  _readArrayDGGRSZone(arrPtr) {
    const count = Number(
      this.module._DGGAL_Array_DGGRSZone_getCount(arrPtr)
    );
    const basePtr = this.module._DGGAL_Array_DGGRSZone_getPointer(arrPtr);
    if (!basePtr || count === 0) {
      this.module._DGGAL_Array_DGGRSZone_delete(arrPtr);
      return [];
    }
    const out = new Array(count);
    const bytesPerZone = 8; // DGGRSZone is 64-bit
    for (let i = 0; i < count; i++) {
      const zPtr = basePtr + i * bytesPerZone;
      const low = BigInt.asUintN(64, this.module.HEAPU32[zPtr >> 2]);
      const high = BigInt.asUintN(64, this.module.HEAPU32[(zPtr + 4) >> 2]);
      out[i] = (high << 32n) | low;
    }
    this.module._DGGAL_Array_DGGRSZone_delete(arrPtr);
    return out;
  }

  // ---- Methods that return DGGAL-managed arrays ----

  getZoneRefinedWGS84Vertices(zone, edgeRefinement) {
    return this._readArrayGeoPoint(this.module._DGGAL_DGGRS_getZoneRefinedWGS84Vertices(this.dggrsPtr, zone, edgeRefinement));
  }

  getZoneRefinedCRSVertices(zone, edgeRefinement) {
    return this._readArrayPointd(this.module._DGGAL_DGGRS_getZoneRefinedCRSVertices(this.dggrsPtr, zone, edgeRefinement));
  }

  getSubZones(zone, depth) {
    return this._readArrayDGGRSZone(this.module._DGGAL_DGGRS_getSubZones(this.dggrsPtr, zone, depth));
  }

  // Refinement
  getRefinementRatio() {
    return this.module._DGGAL_DGGRS_getRefinementRatio(this.dggrsPtr);
  }

  // WGS84 extent
  getZoneWGS84Extent(zone) {
    const stack = this.module.stackSave();
    const buf = this.module.stackAlloc(32); // GeoExtent { ll, ur }
    this.module._DGGAL_DGGRS_getZoneWGS84Extent(this.dggrsPtr, zone, buf);
    const ll_lat = this.module.HEAPF64[buf >> 3];
    const ll_lon = this.module.HEAPF64[(buf + 8) >> 3];
    const ur_lat = this.module.HEAPF64[(buf + 16) >> 3];
    const ur_lon = this.module.HEAPF64[(buf + 24) >> 3];
    this.module.stackRestore(stack);
    return {
      ll: { lat: ll_lat, lon: ll_lon },
      ur: { lat: ur_lat, lon: ur_lon },
    };
  }

  // listZones(level, bbox) → Array<DGGRSZone>
  listZones(level, bbox) {
    const stack = this.module.stackSave();
    const extentPtr = this.module.stackAlloc(32);
    this.module.HEAPF64[extentPtr >> 3] = bbox.ll.lat;
    this.module.HEAPF64[(extentPtr + 8) >> 3] = bbox.ll.lon;
    this.module.HEAPF64[(extentPtr + 16) >> 3] = bbox.ur.lat;
    this.module.HEAPF64[(extentPtr + 24) >> 3] = bbox.ur.lon;
    const arrPtr = this.module._DGGAL_DGGRS_listZones(this.dggrsPtr, level, extentPtr);
    const result = this._readArrayDGGRSZone(arrPtr);
    this.module.stackRestore(stack);
    return result;
  }

  getZoneFromWGS84Centroid(level, geoPoint) {
    const stack = this.module.stackSave();
    const buf = this.module.stackAlloc(16);        // GeoPoint { double lat; double lon; }
    this.module.HEAPF64[buf >> 3]       = geoPoint.lat;
    this.module.HEAPF64[(buf + 8) >> 3] = geoPoint.lon;
    const zone = BigInt.asUintN(64, this.module._DGGAL_DGGRS_getZoneFromWGS84Centroid(this.dggrsPtr, level, buf ));
    this.module.stackRestore(stack);
    return zone;
  }

  getZoneFromCRSCentroid(crs, pointd) {
    const stack = this.module.stackSave();
    const buf = this.module.stackAlloc(16);        // Pointd { double x; double y; }
    this.module.HEAPF64[buf >> 3]       = pointd.x;
    this.module.HEAPF64[(buf + 8) >> 3] = pointd.y;
    const zone = BigInt.asUintN(64, this.module._DGGAL_DGGRS_getZoneFromCRSCentroid(this.dggrsPtr, crs, buf));
    this.module.stackRestore(stack);
    return zone;
  }

  countZones(level) {
    return BigInt.asUintN(64, this.module._DGGAL_DGGRS_countZones(this.dggrsPtr, level));
  }

  // First / indexed sub-zones
  getFirstSubZone(parent, relativeDepth) {
    return BigInt.asUintN(64, this.module._DGGAL_DGGRS_getFirstSubZone(this.dggrsPtr, parent, relativeDepth)
    );
  }

  getIndexMaxDepth() {
    return this.module._DGGAL_DGGRS_getIndexMaxDepth(this.dggrsPtr);
  }

  getSubZoneAtIndex(parent, relativeDepth, index) {
    return BigInt.asUintN(64, this.module._DGGAL_DGGRS_getSubZoneAtIndex(this.dggrsPtr, parent, relativeDepth, index));
  }

  getSubZoneIndex(parent, subZone) {
    return BigInt.asUintN(64, this.module._DGGAL_DGGRS_getSubZoneIndex(this.dggrsPtr, parent, subZone));
  }

  // Sub-zone centroid arrays
  getSubZoneCRSCentroids(parent, crs, relativeDepth) {
    return this._readArrayPointd(this.module._DGGAL_DGGRS_getSubZoneCRSCentroids(this.dggrsPtr, parent, crs, relativeDepth));
  }

  getSubZoneWGS84Centroids(parent, relativeDepth) {
    return this._readArrayGeoPoint(this.module._DGGAL_DGGRS_getSubZoneWGS84Centroids(this.dggrsPtr, parent, relativeDepth));
  }

  // CRS-based centroid & extent
  getZoneCRSCentroid(zone, crs) {
    const stack = this.module.stackSave();
    const buf = this.module.stackAlloc(16);
    this.module._DGGAL_DGGRS_getZoneCRSCentroid(this.dggrsPtr, zone, crs, buf);
    const x = this.module.HEAPF64[buf >> 3];
    const y = this.module.HEAPF64[(buf + 8) >> 3];
    this.module.stackRestore(stack);
    return { x, y };
  }

  getZoneCRSExtent(zone, crs) {
    const stack = this.module.stackSave();
    const buf = this.module.stackAlloc(40); // CRS + Pointd tl + Pointd br
    this.module._DGGAL_DGGRS_getZoneCRSExtent(this.dggrsPtr, zone, crs, buf);
    const crsOut = BigInt.asUintN(64, this.module.HEAP64[buf >> 3]);
    const tlx = this.module.HEAPF64[(buf + 8) >> 3];
    const tly = this.module.HEAPF64[(buf + 16) >> 3];
    const brx = this.module.HEAPF64[(buf + 24) >> 3];
    const bry = this.module.HEAPF64[(buf + 32) >> 3];
    this.module.stackRestore(stack);
    return {
      crs: crsOut,
      tl: { x: tlx, y: tly },
      br: { x: brx, y: bry },
    };
  }

  compactZones(zones) {
    const arrPtr = this.module._DGGAL_Array_DGGRSZone_new(zones.length);
    const basePtr = this.module._DGGAL_Array_DGGRSZone_getPointer(arrPtr);

    for (let i = 0; i < zones.length; i++) {
      const z = zones[i];
      this.module.HEAPU32[(basePtr + i * 8    ) >> 2] = Number(z & 0xFFFFFFFFn);
      this.module.HEAPU32[(basePtr + i * 8 + 4) >> 2] = Number(z >> 32n);
    }
    this.module._DGGAL_DGGRS_compactZones(this.dggrsPtr, arrPtr);
    return this._readArrayDGGRSZone(arrPtr);
  }

  // Level / metrics conversions
  getLevelFromMetersPerSubZone(physicalMetersPerSubZone, relativeDepth) {
    return this.module._DGGAL_DGGRS_getLevelFromMetersPerSubZone(this.dggrsPtr, physicalMetersPerSubZone, relativeDepth);
  }

  getLevelFromPixelsAndExtent(extent, width, height, relativeDepth) {
    const stack = this.module.stackSave();
    const buf = this.module.stackAlloc(32);
    // write WGS84 extent: ll.lat, ll.lon, ur.lat, ur.lon
    this.module.HEAPF64[buf >> 3] = extent.ll.lat;
    this.module.HEAPF64[(buf + 8) >> 3] = extent.ll.lon;
    this.module.HEAPF64[(buf + 16) >> 3] = extent.ur.lat;
    this.module.HEAPF64[(buf + 24) >> 3] = extent.ur.lon;
    const result = this.module._DGGAL_DGGRS_getLevelFromPixelsAndExtent(this.dggrsPtr, buf, width, height, relativeDepth );
    this.module.stackRestore(stack);
    return result;
  }

  getLevelFromRefZoneArea(refArea, relativeDepth) {
    return this.module._DGGAL_DGGRS_getLevelFromRefZoneArea(this.dggrsPtr, refArea, relativeDepth);
  }

  getLevelFromScaleDenominator(scaleDenominator, relativeDepth) {
    return this.module._DGGAL_DGGRS_getLevelFromScaleDenominator(this.dggrsPtr, scaleDenominator, relativeDepth);
  }

  getMetersPerSubZoneFromLevel(level, relativeDepth) {
    return this.module._DGGAL_DGGRS_getMetersPerSubZoneFromLevel(this.dggrsPtr, level, relativeDepth);
  }

  getRefZoneArea(relativeDepth) {
    return this.module._DGGAL_DGGRS_getRefZoneArea(this.dggrsPtr, relativeDepth);
  }

  getScaleDenominatorFromLevel(level, relativeDepth) {
    return this.module._DGGAL_DGGRS_getScaleDenominatorFromLevel(this.dggrsPtr, level, relativeDepth);
  }
}

// DGGAL module wrapper

export class DGGAL {
  static nullZone = 0xFFFFFFFFFFFFFFFFn;
  static CRSRegistry = { epsg: 0, ogc: 1 };

  static CRS(registry, code, h) {
    return BigInt.asUintN(64, this.module._DGGAL_CRS(registry, code, h));
  }

  constructor(module, modulePtr) {
    this.module = module;
    this.modulePtr = modulePtr;
  }

  static async init() {
    const module = await dggalFactory();
    const modulePtr = module._DGGAL_init();
    return new DGGAL(module, modulePtr);
  }

  terminate() {
    this.module._DGGAL_terminate(this.modulePtr);
  }

  createDGGRS(name) {
    return new DGGRS(this.module, this.modulePtr, name);
  }

  listDGGRS() {
    const PTR_SIZE = this.module.HEAPU32.BYTES_PER_ELEMENT;
    const PTR_TYPE = PTR_SIZE === 4 ? 'i32' : 'i64';
    const result = [];
    const listPtr = this.module._DGGAL_DGGRS_list(0);
    let offset = 0;
    while (true) {
      const strPtr = this.module.getValue(listPtr + offset, PTR_TYPE);
      if (strPtr === 0) break;
      const str = this.module.UTF8ToString(strPtr);
      result.push(str);
      offset += PTR_SIZE;
    }
    return result;
  }
}

export default DGGAL;
