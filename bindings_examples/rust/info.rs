#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
#![allow(unused_variables)]

// *********** DGGAL rust bindings *********************
use std::env;
use std::ffi::CString;
use std::ffi::CStr;
use std::ffi::c_void;
use std::f64::consts::PI;
use std::slice;

#[allow(warnings)] mod dggal;

// *** This code should be moved to / generated inside the DGGAL rust bindings
const nullZone : dggal::DGGRSZone = 0xFFFFFFFFFFFFFFFFu64;
const nullCStr : * const i8 = 0 as * const i8;
const nullInst : dggal::Instance = 0 as dggal::Instance;
const nullPtr : *mut *mut c_void = 0 as *mut *mut c_void;

// TODO: Could we use rust function-generating macros?

// DGGRS::getZoneFromTextID()
fn DGGRS_getZoneFromTextID(dggrs: dggal::DGGRS, zoneID: * const i8) -> dggal::DGGRSZone
{
   let mut zone = nullZone;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneFromTextID_vTblID as usize));
      if cMethod != 0usize {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zoneID: * const i8) -> dggal::DGGRSZone = std::mem::transmute(cMethod);
         zone = method(dggrs, zoneID);
      }
   }
   zone
}

// DGGRS::getZoneLevel()
fn DGGRS_getZoneLevel(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> i32
{
   let mut level = -1;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneLevel_vTblID as usize));
      if cMethod != 0usize {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
         level = method(dggrs, zone);
      }
   }
   level
}

// DGGRS::countZoneEdges()
fn DGGRS_countZoneEdges(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> i32
{
   let mut level = -1;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_countZoneEdges_vTblID as usize));
      if cMethod != 0usize {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
         level = method(dggrs, zone);
      }
   }
   level
}

// DGGRS::get64KDepth() -- (not currently a virtual method)
fn DGGRS_get64KDepth(dggrs: dggal::DGGRS) -> i32
{
   let mut depth = -1;
   unsafe
   {
      if dggrs != nullInst {
         depth = dggal::DGGRS_get64KDepth.unwrap()(dggrs);
      }
   }
   depth
}

// DGGRS::getRefinementRatio()
fn DGGRS_getRefinementRatio(dggrs: dggal::DGGRS) -> i32
{
   let mut depth = -1;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getRefinementRatio_vTblID as usize));
      if cMethod != 0usize {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS) -> i32 = std::mem::transmute(cMethod);
         depth = method(dggrs);
      }
   }
   depth
}

// DGGRS::getMaxDGGRSZoneLevel()
fn DGGRS_getMaxDGGRSZoneLevel(dggrs: dggal::DGGRS) -> i32
{
   let mut depth = -1;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getMaxDGGRSZoneLevel_vTblID as usize));
      if cMethod != 0usize {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS) -> i32 = std::mem::transmute(cMethod);
         depth = method(dggrs);
      }
   }
   depth
}

// DGGRS::getZoneWGS84Centroid()
fn DGGRS_getZoneWGS84Centroid(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> dggal::GeoPoint
{
   let mut centroid = dggal::GeoPoint { lat: 0.0, lon: 0.0 };
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneWGS84Centroid_vTblID as usize));
      if cMethod != std::mem::transmute(0usize) {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, centroid: *mut dggal::GeoPoint) = std::mem::transmute(cMethod);
         method(dggrs, zone, std::ptr::from_mut(&mut centroid));
      }
   }
   centroid
}

// DGGRS::getZoneWGS84Vertices()
fn DGGRS_getZoneWGS84Vertices(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> Vec<dggal::GeoPoint>
{
   let mut n: i32 = 0;
   let vertices: Vec<dggal::GeoPoint>;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let mut v: [dggal::GeoPoint; 6] = [dggal::GeoPoint { lat: 0.0, lon: 0.0 }; 6]; // REVIEW: Anyway to avoid this initialization?
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneWGS84Vertices_vTblID as usize));
      if cMethod != std::mem::transmute(0usize) {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, vertices: *mut dggal::GeoPoint) -> i32 = std::mem::transmute(cMethod);
         n = method(dggrs, zone, std::ptr::from_mut(&mut v[0]));
      }
      vertices = slice::from_raw_parts(&v[0], n as usize).to_vec();
   }
   vertices
}

// DGGRS::getZoneArea()
fn DGGRS_getZoneArea(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> f64
{
   let mut area = 0.0;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneArea_vTblID as usize));
      if cMethod != std::mem::transmute(0usize) {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> f64 = std::mem::transmute(cMethod);
         area = method(dggrs, zone);
      }
   }
   area
}

// DGGRS::countSubZones()
fn DGGRS_countSubZones(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, depth: i32) -> u64
{
   let mut count = 0;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_countSubZones_vTblID as usize));
      if cMethod != std::mem::transmute(0usize) {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, depth: i32) -> u64 = std::mem::transmute(cMethod);
         count = method(dggrs, zone, depth);
      }
   }
   count
}

// DGGRS::getZoneTextID()
fn DGGRS_getZoneTextID(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> String
{
   let id: String;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let mut zoneID = [0i8; 256]; // REVIEW: Anyway to avoid this initialization?
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneTextID_vTblID as usize));
      if cMethod != std::mem::transmute(0usize) {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, zoneID: *mut i8) = std::mem::transmute(cMethod);
         method(dggrs, zone, std::ptr::from_mut(&mut zoneID[0]));
      }
      id = CStr::from_ptr(zoneID.as_ptr()).to_str().unwrap().to_string();
   }
   id
}

// DGGRS::getZoneParents()
fn DGGRS_getZoneParents(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> Vec<dggal::DGGRSZone>
{
   let mut n: i32 = 0;
   let parents: Vec<dggal::DGGRSZone>;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let mut p = [nullZone; 3]; // REVIEW: Anyway to avoid this initialization?
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneParents_vTblID as usize));
      if cMethod != std::mem::transmute(0usize) {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, parents: *mut dggal::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
         n = method(dggrs, zone, std::ptr::from_mut(&mut p[0]));
      }
      parents = slice::from_raw_parts(&p[0], n as usize).to_vec();
   }
   parents
}

// DGGRS::getZoneChildren()
fn DGGRS_getZoneChildren(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> Vec<dggal::DGGRSZone>
{
   let mut n: i32 = 0;
   let children: Vec<dggal::DGGRSZone>;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let mut ch = [nullZone; 9]; // REVIEW: Anyway to avoid this initialization?
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneChildren_vTblID as usize));
      if cMethod != std::mem::transmute(0usize) {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, children: *mut dggal::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
         n = method(dggrs, zone, std::ptr::from_mut(&mut ch[0]));
      }
      children = slice::from_raw_parts(&ch[0], n as usize).to_vec();
   }
   children
}

// DGGRS::getZoneNeighbors()
fn DGGRS_getZoneNeighbors(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, nbTypes: &mut [i32; 6]) -> Vec<dggal::DGGRSZone>
{
   let mut n: i32 = 0;
   let neighbors: Vec<dggal::DGGRSZone>;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let mut nb = [nullZone; 6]; // REVIEW: Anyway to avoid this initialization?
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneNeighbors_vTblID as usize));
      if cMethod != std::mem::transmute(0usize) {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, neighbors: *mut dggal::DGGRSZone, nbTypes: *mut i32) -> i32 = std::mem::transmute(cMethod);
         n = method(dggrs, zone, std::ptr::from_mut(&mut nb[0]), std::ptr::from_mut(&mut nbTypes[0]));
      }
      neighbors = slice::from_raw_parts(&nb[0], n as usize).to_vec();
   }
   neighbors
}

// DGGRS::getZoneCentroidParent()
fn DGGRS_getZoneCentroidParent(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> dggal::DGGRSZone
{
   let mut centroidParent: dggal::DGGRSZone = nullZone;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneCentroidParent_vTblID as usize));
      if cMethod != std::mem::transmute(0usize) {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> dggal::DGGRSZone = std::mem::transmute(cMethod);
         centroidParent = method(dggrs, zone);
      }
   }
   centroidParent
}

// DGGRS::getZoneCentroidChild()
fn DGGRS_getZoneCentroidChild(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> dggal::DGGRSZone
{
   let mut centroidChild: dggal::DGGRSZone = nullZone;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneCentroidChild_vTblID as usize));
      if cMethod != std::mem::transmute(0usize) {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> dggal::DGGRSZone = std::mem::transmute(cMethod);
         centroidChild = method(dggrs, zone);
      }
   }
   centroidChild
}

// DGGRS::isZoneCentroidChild()
fn DGGRS_isZoneCentroidChild(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> bool
{
   let mut isZoneCentroidChild: bool = false;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_isZoneCentroidChild_vTblID as usize));
      if cMethod != std::mem::transmute(0usize) {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> u32 = std::mem::transmute(cMethod);
         isZoneCentroidChild = method(dggrs, zone) != 0;
      }
   }
   isZoneCentroidChild
}

// DGGRS::getZoneWGS84Extent()
fn DGGRS_getZoneWGS84Extent(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> dggal::GeoExtent
{
   let mut extent: dggal::GeoExtent = dggal::GeoExtent {    // REVIEW: Anyway to avoid this initialization?
      ll: dggal::GeoPoint { lat: 0.0, lon: 0.0 },
      ur: dggal::GeoPoint { lat: 0.0, lon: 0.0 } };
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneWGS84Extent_vTblID as usize));
      if cMethod != std::mem::transmute(0usize) {
         let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, vertices: *mut dggal::GeoExtent) = std::mem::transmute(cMethod);
         method(dggrs, zone, std::ptr::from_mut(&mut extent));
      }
   }
   extent
}

// *********** (end of) DGGAL rust bindings *********************

/*
// parameterized templates in C! (for options map)
typedef Map T(Map, CString, constString);
typedef MapIterator T(MapIterator, CString, constString);

// For looking up internationalized strings
#define MODULE_NAME "info"
*/

fn zoneInfo(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, options: dggal::Instance /*Map<String, const String>*/) -> i32
{
   let level = DGGRS_getZoneLevel(dggrs, zone);
   let nEdges = DGGRS_countZoneEdges(dggrs, zone);
   let centroid = DGGRS_getZoneWGS84Centroid(dggrs, zone);
   let extent: dggal::GeoExtent = DGGRS_getZoneWGS84Extent(dggrs, zone);
   let vertices: Vec<dggal::GeoPoint> = DGGRS_getZoneWGS84Vertices(dggrs, zone);
   let zoneID: String = DGGRS_getZoneTextID(dggrs, zone);
   let area: f64 = DGGRS_getZoneArea(dggrs, zone);
   let areaKM2: f64 = area / 1000000.0;
   let depth: i32 = DGGRS_get64KDepth(dggrs);
   let parents = DGGRS_getZoneParents(dggrs, zone);
   let mut nbTypes: [i32; 6] = [0; 6];
   let neighbors = DGGRS_getZoneNeighbors(dggrs, zone, &mut nbTypes);
   let children = DGGRS_getZoneChildren(dggrs, zone);
   let centroidParent: dggal::DGGRSZone = DGGRS_getZoneCentroidParent(dggrs, zone);
   let centroidChild: dggal::DGGRSZone = DGGRS_getZoneCentroidChild(dggrs, zone);
   let isCentroidChild = DGGRS_isZoneCentroidChild(dggrs, zone);
   let crs = (c"EPSG:4326").as_ptr();

   /*
   constString depthOption = ptr::null();
   if(options)
   {
      T(MapIterator, CString, constString) it = { options };
      if(Iterator_index((Iterator *)&it, TAp((void *)"depth"), false))
         depthOption = pTA(const char, Iterator_getData((Iterator *)&it));
   }

   if(depthOption)
   {
      i32 maxDepth = DGGRS_getMaxDepth(dggrs);
      _onGetDataFromString(CO(i32), &depth, depthOption);
      if(depth > maxDepth)
      {
         printLn(CO(CString), $("Invalid depth (maximum: "), CO(i32), maxDepth, ")", ptr::null());
         return 1;
      }
   }
   */

   let nSubZones: u64 = DGGRS_countSubZones(dggrs, zone, depth);

   println!("Textual Zone ID: {zoneID}");
   println!("64-bit integer ID: {zone} (0x{zone:X})");
   println!("");

   let sCChild = if isCentroidChild { ", centroid child" } else { "" };
   println!("Level {level} zone ({nEdges} edges{sCChild})");

   println!("{area} m² ({areaKM2} km²)");
   println!("{nSubZones} sub-zones at depth {depth}");

   let cLat = centroid.lat * 180.0 / PI;
   let cLon = centroid.lon * 180.0 / PI;
   println!("WGS84 Centroid (lat, lon): {cLat}, {cLon}");

   let llLat = extent.ll.lat * 180.0 / PI;
   let llLon = extent.ll.lon * 180.0 / PI;
   let urLat = extent.ur.lat * 180.0 / PI;
   let urLon = extent.ur.lon * 180.0 / PI;
   println!("WGS84 Extent (lat, lon): {{ {llLat}, {llLon} }}, {{ {urLat}, {urLon} }}");

   println!("");

   let nParents = parents.len();
   if nParents != 0
   {
      let pPlural = if nParents > 1 { "s" } else { "" };
      println!("Parent{pPlural} ({nParents}):");
      for p in parents
      {
         let pID = DGGRS_getZoneTextID(dggrs, p);
         print!("   {pID}");
         if centroidParent == p {
            print!(" (centroid child)");
         }
         println!("");
      }
   }
   else {
      println!("No parent");
   }
   println!("");

   let nChildren = children.len();
   println!("Children ({nChildren}):");
   for c in children
   {
      let cID = DGGRS_getZoneTextID(dggrs, c);
      print!("   {cID}");
      if centroidChild == c {
         print!(" (centroid)");
      }
      println!("");
   }
   println!("");

   let nNeighbors = neighbors.len();
   println!("Neighbors ({nNeighbors}):");

   let mut i: usize = 0;
   for n in neighbors
   {
      let nID = DGGRS_getZoneTextID(dggrs, n);
      let nt = nbTypes[i];
      println!("   (direction {nt}): {nID}");
      i += 1;
   }
   println!("");
   let sCRS = unsafe { CStr::from_ptr(crs).to_str().unwrap() };
   let nVertices = vertices.len();
   println!("[{sCRS}] Vertices ({nVertices}):");

   for v in vertices
   {
      let lat = v.lat * 180.0 / PI;
      let lon = v.lon * 180.0 / PI;
      println!("   {lat}, {lon}");
   }

   0 // No error
}

fn dggrsInfo(dggrs: dggal::DGGRS, options: dggal::Instance /*Map<String, const String>*/) -> i32
{
   let depth64k = DGGRS_get64KDepth(dggrs);
   let ratio = DGGRS_getRefinementRatio(dggrs);
   let maxLevel = DGGRS_getMaxDGGRSZoneLevel(dggrs);

   println!("Refinement Ratio: {ratio}");
   println!("Maximum level for 64-bit global identifiers (DGGAL DGGRSZone): {maxLevel}");
   println!("Default ~64K sub-zones relative depth: {depth64k}");
   0 // No error
}

fn displayInfo(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, options: dggal::Instance /*Map<String, const String>*/) -> i32
{
   if zone != nullZone {
      zoneInfo(dggrs, zone, options)
   } else {
      dggrsInfo(dggrs, options)
   }
}

// Class * class_Map_String_constString;

fn main()
{
   unsafe
   {
      let args: Vec<String> = env::args().collect();
      let argc = args.len();
      let app = dggal::eC_init(nullInst, true as u32, false as u32, 0,
         0 /*ptr::null_mut::<* mut * mut i8>()*/ as * mut * mut i8); // TOOD: argc, args);
      let mEcere = dggal::ecere_init(app);
      let mDGGAL = dggal::dggal_init(app);
      let mut exitCode: i32 = 0;
      let mut show_syntax = false;
      let mut dggrsName: *const i8 = nullCStr;
      let mut a : usize = 1;
      let mut zoneID: *const i8 = nullCStr;
      let arg0CStr = CString::new(args[0].clone()).unwrap();
      let arg0 : *const i8 = arg0CStr.as_ptr();
      let options : dggal::Instance = nullInst; // Map<String, const String>

      /*
      class_Map_String_constString = eC_findClass(app, "Map<CString, const CString>");
      options = newi(Map, CString, constString);
      */

           if 0 == dggal::strcasecmp(arg0, (c"i3h").as_ptr()) { dggrsName = (c"ISEA3H").as_ptr(); }
      else if 0 == dggal::strcasecmp(arg0, (c"i9r").as_ptr()) { dggrsName = (c"ISEA9R").as_ptr(); }
      else if 0 == dggal::strcasecmp(arg0, (c"ggg").as_ptr()) { dggrsName = (c"GNOSISGlobalGrid").as_ptr(); }

      if dggrsName == nullCStr && argc > 1
      {
         let arg1cs = CString::new(args[1].clone()).unwrap(); // This variable needs to stay alive or the pointer will point to garbage
         let arg1 : *const i8 = arg1cs.as_ptr();
              if 0 == dggal::strcasecmp(arg1, (c"isea3h").as_ptr()) { dggrsName = (c"ISEA3H").as_ptr(); }
         else if 0 == dggal::strcasecmp(arg1, (c"isea9r").as_ptr()) { dggrsName = (c"ISEA9R").as_ptr(); }
         else if 0 == dggal::strcasecmp(arg1, (c"gnosis").as_ptr()) { dggrsName = (c"GNOSISGlobalGrid").as_ptr(); }
         a += 1;
      }

      let zoneArg;
      if argc > a {
         zoneArg = CString::new(args[a].clone()).unwrap();
         zoneID = zoneArg.as_ptr();
         a+=1;
      }

      while a < argc
      {
         let keycs = CString::new(args[a].clone()).unwrap(); // Needs to stay alive
         let key: *const i8 = keycs.as_ptr();
         a+=1;
         if *key == '-' as i8 && a < argc
         {
            let strValue = CString::new(args[a].clone()).unwrap();
            let value = strValue.as_ptr();
            a+=1;
            /*
            T(MapIterator, CString, constString) it = { options };
            Iterator_index((Iterator *)&it, TAp((void *)(key+1)), true);
            Iterator_setData((Iterator *)&it, TAp((void *)value));
            */
         }
         else
         {
            exitCode = 1;
            show_syntax = true;
         }
      }

      if dggrsName != nullCStr && exitCode == 0
      {
         let dggrs : dggal::DGGRS = dggal::__ecereNameSpace__ecere__com__eInstance_New(dggal::__ecereNameSpace__ecere__com__eSystem_FindClass(mDGGAL, dggrsName)) as dggal::DGGRS;
         let mut zone = nullZone;

         let dn = CStr::from_ptr(dggrsName).to_str().unwrap();
         println!("DGGRS: https://maps.gnosis.earth/ogcapi/dggrs/{dn}");

         if zoneID != nullCStr {
            zone = DGGRS_getZoneFromTextID(dggrs, zoneID);
            if zone == nullZone
            {
               let za = CStr::from_ptr(zoneID).to_str().unwrap();
               println!("Invalid {dn} zone identifier: {za}");
               exitCode = 1;
            }
         }

         if exitCode == 0 {
            displayInfo(dggrs, zone, options);
         }

         dggal::__ecereNameSpace__ecere__com__eInstance_DecRef(dggrs as dggal::Instance);
      }
      else
      {
         show_syntax = true;
         exitCode = 1;
      }

      dggal::__ecereNameSpace__ecere__com__eInstance_DecRef(options);

      if show_syntax {
         println!("Syntax:");
         println!("   info <dggrs> [zone] [options]");
         println!("where dggrs is one of gnosis, isea3h or isea9r");
      }
      dggal::__ecereNameSpace__ecere__com__eInstance_DecRef(app);

      std::process::exit(exitCode)
   }
}
