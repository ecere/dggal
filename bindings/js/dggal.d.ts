// dggal.d.ts

// A 64-bit unsigned zone identifier
export type DGGRSZone = bigint

// A Coordinate Reference Systme identifier
export type CRS = bigint;

// A geographic point in radians
export interface GeoPoint {
  lat: number
  lon: number
}

// Neighbor relationship with zone ID and adjacency type
export interface Neighbor {
  zone: DGGRSZone
  type: number
}

// A 2D point in projected CRS units
export interface Pointd {
  x: number
  y: number
}

// A geographic bounding box (WGS84)
export interface GeoExtent {
  ll: GeoPoint
  ur: GeoPoint
}

// A projected CRS bounding box
export interface CRSExtent
{
  crs: CRS
  tl: Pointd
  br: Pointd
}

export const CRSRegistry: {
  readonly epsg: 0
  readonly ogc:  1
}

export type CRSRegistry = typeof CRSRegistry[keyof typeof CRSRegistry]

// A DGGAL Discrete Global Grid Reference System
// Constructed via `const rs = dggal.createDGGRS(name)`
export class DGGRS {
  // Convert a text ID to a 64-bit zone identifier
  getZoneFromTextID(zoneID: string): DGGRSZone

  // Convert a 64-bit zone identifier back to its text ID
  getZoneTextID(zone: DGGRSZone): string

  // DGGRSZone metadata
  getZoneLevel(zone: DGGRSZone): number
  getZoneArea(zone: DGGRSZone): number
  countZoneEdges(zone: DGGRSZone): number
  countSubZones(zone: DGGRSZone, depth: number): bigint

  // Hierarchy
  getZoneParents(zone: DGGRSZone): DGGRSZone[]
  getZoneChildren(zone: DGGRSZone): DGGRSZone[]
  getZoneNeighbors(zone: DGGRSZone): Neighbor[]

  // Centroid operations
  getZoneWGS84Centroid(zone: DGGRSZone): GeoPoint
  getZoneCentroidParent(zone: DGGRSZone): DGGRSZone
  getZoneCentroidChild(zone: DGGRSZone): DGGRSZone
  isZoneCentroidChild(zone: DGGRSZone): boolean

  // Relationship queries
  areZonesNeighbors(a: DGGRSZone, b: DGGRSZone): boolean
  areZonesSiblings(a: DGGRSZone, b: DGGRSZone): boolean
  doZonesOverlap(a: DGGRSZone, b: DGGRSZone): boolean
  doesZoneContain(haystack: DGGRSZone, needle: DGGRSZone): boolean
  isZoneAncestorOf(ancestor: DGGRSZone, descendant: DGGRSZone, maxDepth: number): boolean
  isZoneContainedIn(needle: DGGRSZone, haystack: DGGRSZone): boolean
  isZoneDescendantOf(descendant: DGGRSZone, ancestor: DGGRSZone, maxDepth: number): boolean
  isZoneImmediateChildOf(child: DGGRSZone, parent: DGGRSZone): boolean
  isZoneImmediateParentOf(parent: DGGRSZone, child: DGGRSZone): boolean
  zoneHasSubZone(haystack: DGGRSZone, needle: DGGRSZone): boolean

  // Limits / metadata
  getMaxDGGRSZoneLevel(): number
  getMaxDepth(): number
  get64KDepth(): number
  getMaxChildren(): number
  getMaxParents(): number
  getMaxNeighbors(): number

  // Vertices
  getZoneCRSVertices(zone: DGGRSZone, crs: CRS): Pointd[]
  getZoneWGS84Vertices(zone: DGGRSZone): GeoPoint[]
  getZoneRefinedWGS84Vertices(zone: DGGRSZone, edgeRefinement: number): GeoPoint[]
  getZoneRefinedCRSVertices(zone: DGGRSZone, edgeRefinement: number): Pointd[]

  // Sub-zone queries
  getSubZones(zone: DGGRSZone, depth: number): DGGRSZone[]

  // Refinement
  getRefinementRatio(): number

  // WGS84 extent
  getZoneWGS84Extent(zone: DGGRSZone): GeoExtent

  // Listing zones within a box
  listZones(level: number, bbox: GeoExtent): DGGRSZone[]

  // Centroid → zone
  getZoneFromWGS84Centroid(level: number, geoPoint: GeoPoint): DGGRSZone
  getZoneFromCRSCentroid(crs: CRS, pointd: Pointd): DGGRSZone

  // Global zone counts
  countZones(level: number): DGGRSZone

  // Indexed sub-zones
  getFirstSubZone(parent: DGGRSZone, relativeDepth: number): DGGRSZone
  getIndexMaxDepth(): number
  getSubZoneAtIndex(parent: DGGRSZone, relativeDepth: number, index: number): DGGRSZone
  getSubZoneIndex(parent: DGGRSZone, subZone: DGGRSZone): DGGRSZone

  // Sub-zone centroid arrays
  getSubZoneCRSCentroids(parent: DGGRSZone, crs: CRS, relativeDepth: number): Pointd[]
  getSubZoneWGS84Centroids(parent: DGGRSZone, relativeDepth: number): GeoPoint[]

  // CRS-based centroid & extent
  getZoneCRSCentroid(zone: DGGRSZone, crs: CRS): Pointd
  getZoneCRSExtent(zone: DGGRSZone, crs: CRS): CRSExtent

  // Compact & rebuild zone arrays
  compactZones(zones: DGGRSZone[]): DGGRSZone[]

  // Level / metrics conversions
  getLevelFromMetersPerSubZone(physicalMetersPerSubZone: number, relativeDepth: number): number
  getLevelFromPixelsAndExtent(extent: GeoExtent, width: number, height: number, relativeDepth: number): number
  getLevelFromRefZoneArea(refArea: number, relativeDepth: number): number
  getLevelFromScaleDenominator(scaleDenominator: number, relativeDepth: number): number
  getMetersPerSubZoneFromLevel(level: number, relativeDepth: number): number
  getRefZoneArea(relativeDepth: number): number
  getScaleDenominatorFromLevel(level: number, relativeDepth: number): number

  // Clean up native resources
  delete(): void
}

// Main entry point
// Use `const dggal = await DGGAL.init()`, then `dggal.createDGGRS()`
export class DGGAL {
  // A sentinel value indicating "no zone"
  static nullZone: DGGRSZone

  static readonly CRSRegistry: {
    readonly epsg: 0
    readonly ogc:  1
  }

  // Construct a CRS identifier
  static CRS(registry: CRSRegistry, code: number, h: boolean): CRS

  // Initialize the WASM module and return the API handle
  static init(): Promise<DGGAL>

  // Create a DGGRS instance by name
  createDGGRS(name: string): DGGRS

  // Tear down the WASM runtime
  terminate(): void
}

export default DGGAL
