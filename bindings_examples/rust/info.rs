#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
//#![allow(unused_variables)]

// *********** DGGAL rust bindings *********************
use std::env;
use std::ffi::CString;
use std::ffi::CStr;
use std::ffi::c_void;
use std::f64::consts::PI;
use std::slice;
use std::collections::HashMap;

#[allow(warnings)] mod dggal;

// *** This code should be moved to / generated inside the DGGAL rust bindings
const nullZone : dggal::DGGRSZone = 0xFFFFFFFFFFFFFFFFu64;
const nullInst : dggal::Instance = 0 as dggal::Instance;
const nullPtr : *mut *mut c_void = 0 as *mut *mut c_void;

struct DGGRS {
   imp: dggal::DGGRS
}

struct Application {
   app: dggal::Application,
   _mEcere: dggal::Module
}
impl Drop for Application {
   fn drop(&mut self) {
      unsafe {
         dggal::__ecereNameSpace__ecere__com__eInstance_DecRef(self.app);
      }
   }
}

impl Application {
   fn new(_args: &Vec<String>) -> Application
   {
      unsafe {
         let app = dggal::eC_init(nullInst, true as u32, false as u32, 0,
               0 /*ptr::null_mut::<* mut * mut i8>()*/ as * mut * mut i8); // TODO: argc, args);
         Application { app: app, _mEcere: dggal::ecere_init(app) }
      }
   }
}

struct DGGAL {
   mDGGAL: dggal::Module
}

impl DGGAL {
   fn new(app: &Application) -> DGGAL
   {
      unsafe {
         DGGAL { mDGGAL: dggal::dggal_init(app.app) }
      }
   }

   fn newDGGRS(&self, name: &str) -> Option<DGGRS>
   {
      let dggrsName = CString::new(name).unwrap();
      unsafe {
         let c = dggal::__ecereNameSpace__ecere__com__eSystem_FindClass(self.mDGGAL, dggrsName.as_ptr());

         if c != nullPtr as * mut dggal::Class {
            Some(DGGRS { imp: dggal::__ecereNameSpace__ecere__com__eInstance_New(c) as dggal::DGGRS })
         }
         else {
            None
         }
      }
   }

}

impl DGGRS {
   // TODO: Could we use rust function-generating macros?

   fn getZoneFromTextID(&self, zoneID: &str) -> dggal::DGGRSZone
   {
      let mut zone = nullZone;
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneFromTextID_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zoneID: * const i8) -> dggal::DGGRSZone = std::mem::transmute(cMethod);
            let csZoneID = CString::new(zoneID).unwrap();
            zone = method(self.imp, csZoneID.as_ptr());
         }
      }
      zone
   }

   fn getZoneLevel(&self, zone: dggal::DGGRSZone) -> i32
   {
      let mut level = -1;
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneLevel_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
            level = method(self.imp, zone);
         }
      }
      level
   }

   fn countZoneEdges(&self, zone: dggal::DGGRSZone) -> i32
   {
      let mut level = -1;
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_countZoneEdges_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
            level = method(self.imp, zone);
         }
      }
      level
   }

   // (not currently a virtual method)
   fn get64KDepth(&self) -> i32
   {
      let mut depth = -1;
      unsafe
      {
         if self.imp != nullInst {
            depth = dggal::DGGRS_get64KDepth.unwrap()(self.imp);
         }
      }
      depth
   }

   // (not currently a virtual method)
   fn getMaxDepth(&self) -> i32
   {
      let mut depth = -1;
      unsafe
      {
         if self.imp != nullInst {
            depth = dggal::DGGRS_getMaxDepth.unwrap()(self.imp);
         }
      }
      depth
   }

   fn getRefinementRatio(&self) -> i32
   {
      let mut depth = -1;
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getRefinementRatio_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS) -> i32 = std::mem::transmute(cMethod);
            depth = method(self.imp,);
         }
      }
      depth
   }

   fn getMaxDGGRSZoneLevel(&self) -> i32
   {
      let mut depth = -1;
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getMaxDGGRSZoneLevel_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS) -> i32 = std::mem::transmute(cMethod);
            depth = method(self.imp,);
         }
      }
      depth
   }

   fn getZoneWGS84Centroid(&self, zone: dggal::DGGRSZone) -> dggal::GeoPoint
   {
      let mut centroid = dggal::GeoPoint { lat: 0.0, lon: 0.0 };
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneWGS84Centroid_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, centroid: *mut dggal::GeoPoint) = std::mem::transmute(cMethod);
            method(self.imp, zone, std::ptr::from_mut(&mut centroid));
         }
      }
      centroid
   }

   fn getZoneWGS84Vertices(&self, zone: dggal::DGGRSZone) -> Vec<dggal::GeoPoint>
   {
      let mut n: i32 = 0;
      let vertices: Vec<dggal::GeoPoint>;
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut v: [dggal::GeoPoint; 6] = [dggal::GeoPoint { lat: 0.0, lon: 0.0 }; 6]; // REVIEW: Anyway to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneWGS84Vertices_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, vertices: *mut dggal::GeoPoint) -> i32 = std::mem::transmute(cMethod);
            n = method(self.imp, zone, std::ptr::from_mut(&mut v[0]));
         }
         vertices = slice::from_raw_parts(&v[0], n as usize).to_vec();
      }
      vertices
   }

   fn getZoneArea(&self, zone: dggal::DGGRSZone) -> f64
   {
      let mut area = 0.0;
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneArea_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> f64 = std::mem::transmute(cMethod);
            area = method(self.imp, zone);
         }
      }
      area
   }

   fn countSubZones(&self, zone: dggal::DGGRSZone, depth: i32) -> u64
   {
      let mut count = 0;
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_countSubZones_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, depth: i32) -> u64 = std::mem::transmute(cMethod);
            count = method(self.imp, zone, depth);
         }
      }
      count
   }

   fn getZoneTextID(&self, zone: dggal::DGGRSZone) -> String
   {
      let id: String;
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut zoneID = [0i8; 256]; // REVIEW: Anyway to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneTextID_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, zoneID: *mut i8) = std::mem::transmute(cMethod);
            method(self.imp, zone, std::ptr::from_mut(&mut zoneID[0]));
         }
         id = CStr::from_ptr(zoneID.as_ptr()).to_str().unwrap().to_string();
      }
      id
   }

   fn getZoneParents(&self, zone: dggal::DGGRSZone) -> Vec<dggal::DGGRSZone>
   {
      let mut n: i32 = 0;
      let parents: Vec<dggal::DGGRSZone>;
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut p = [nullZone; 3]; // REVIEW: Anyway to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneParents_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, parents: *mut dggal::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
            n = method(self.imp, zone, std::ptr::from_mut(&mut p[0]));
         }
         parents = slice::from_raw_parts(&p[0], n as usize).to_vec();
      }
      parents
   }

   fn getZoneChildren(&self, zone: dggal::DGGRSZone) -> Vec<dggal::DGGRSZone>
   {
      let mut n: i32 = 0;
      let children: Vec<dggal::DGGRSZone>;
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut ch = [nullZone; 9]; // REVIEW: Anyway to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneChildren_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, children: *mut dggal::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
            n = method(self.imp, zone, std::ptr::from_mut(&mut ch[0]));
         }
         children = slice::from_raw_parts(&ch[0], n as usize).to_vec();
      }
      children
   }

   fn getZoneNeighbors(&self, zone: dggal::DGGRSZone, nbTypes: &mut [i32; 6]) -> Vec<dggal::DGGRSZone>
   {
      let mut n: i32 = 0;
      let neighbors: Vec<dggal::DGGRSZone>;
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut nb = [nullZone; 6]; // REVIEW: Anyway to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneNeighbors_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, neighbors: *mut dggal::DGGRSZone, nbTypes: *mut i32) -> i32 = std::mem::transmute(cMethod);
            n = method(self.imp, zone, std::ptr::from_mut(&mut nb[0]), std::ptr::from_mut(&mut nbTypes[0]));
         }
         neighbors = slice::from_raw_parts(&nb[0], n as usize).to_vec();
      }
      neighbors
   }

   fn getZoneCentroidParent(&self, zone: dggal::DGGRSZone) -> dggal::DGGRSZone
   {
      let mut centroidParent: dggal::DGGRSZone = nullZone;
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneCentroidParent_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> dggal::DGGRSZone = std::mem::transmute(cMethod);
            centroidParent = method(self.imp, zone);
         }
      }
      centroidParent
   }

   fn getZoneCentroidChild(&self, zone: dggal::DGGRSZone) -> dggal::DGGRSZone
   {
      let mut centroidChild: dggal::DGGRSZone = nullZone;
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneCentroidChild_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> dggal::DGGRSZone = std::mem::transmute(cMethod);
            centroidChild = method(self.imp, zone);
         }
      }
      centroidChild
   }

   fn isZoneCentroidChild(&self, zone: dggal::DGGRSZone) -> bool
   {
      let mut isZoneCentroidChild: bool = false;
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_isZoneCentroidChild_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> u32 = std::mem::transmute(cMethod);
            isZoneCentroidChild = method(self.imp, zone) != 0;
         }
      }
      isZoneCentroidChild
   }

   fn getZoneWGS84Extent(&self, zone: dggal::DGGRSZone) -> dggal::GeoExtent
   {
      let mut extent: dggal::GeoExtent = dggal::GeoExtent {    // REVIEW: Anyway to avoid this initialization?
         ll: dggal::GeoPoint { lat: 0.0, lon: 0.0 },
         ur: dggal::GeoPoint { lat: 0.0, lon: 0.0 } };
      unsafe
      {
         let c = dggal::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneWGS84Extent_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, vertices: *mut dggal::GeoExtent) = std::mem::transmute(cMethod);
            method(self.imp, zone, std::ptr::from_mut(&mut extent));
         }
      }
      extent
   }
}
impl Drop for DGGRS {
   fn drop(&mut self)
   {
      unsafe {
         dggal::__ecereNameSpace__ecere__com__eInstance_DecRef(self.imp as dggal::Instance);
      }
   }
}

// *********** (end of) DGGAL rust bindings *********************

fn zoneInfo(dggrs: DGGRS, zone: dggal::DGGRSZone, options: HashMap<&str, &str>) -> i32
{
   let level = dggrs.getZoneLevel(zone);
   let nEdges = dggrs.countZoneEdges(zone);
   let centroid = dggrs.getZoneWGS84Centroid(zone);
   let extent: dggal::GeoExtent = dggrs.getZoneWGS84Extent(zone);
   let vertices: Vec<dggal::GeoPoint> = dggrs.getZoneWGS84Vertices(zone);
   let zoneID: String = dggrs.getZoneTextID(zone);
   let area: f64 = dggrs.getZoneArea(zone);
   let areaKM2: f64 = area / 1000000.0;
   let mut depth: i32 = dggrs.get64KDepth();
   let parents = dggrs.getZoneParents(zone);
   let mut nbTypes: [i32; 6] = [0; 6];
   let neighbors = dggrs.getZoneNeighbors(zone, &mut nbTypes);
   let children = dggrs.getZoneChildren(zone);
   let centroidParent: dggal::DGGRSZone = dggrs.getZoneCentroidParent(zone);
   let centroidChild: dggal::DGGRSZone = dggrs.getZoneCentroidChild(zone);
   let isCentroidChild = dggrs.isZoneCentroidChild(zone);
   let crs = &"EPSG:4326";
   let depthOption = options.get(&"depth");

   if depthOption != None
   {
      let maxDepth: i32 = dggrs.getMaxDepth();
      depth = depthOption.unwrap().parse::<i32>().unwrap();
      if depth > maxDepth
      {
         println!("Invalid depth (maximum: {maxDepth})");
         return 1;
      }
   }

   let nSubZones: u64 = dggrs.countSubZones(zone, depth);

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
         let pID = dggrs.getZoneTextID(p);
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
      let cID = dggrs.getZoneTextID(c);
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
      let nID = dggrs.getZoneTextID(n);
      let nt = nbTypes[i];
      println!("   (direction {nt}): {nID}");
      i += 1;
   }
   println!("");
   let nVertices = vertices.len();
   println!("[{crs}] Vertices ({nVertices}):");

   for v in vertices
   {
      let lat = v.lat * 180.0 / PI;
      let lon = v.lon * 180.0 / PI;
      println!("   {lat}, {lon}");
   }

   0 // No error
}

fn dggrsInfo(dggrs: DGGRS, _options: HashMap<&str, &str>) -> i32
{
   let depth64k = dggrs.get64KDepth();
   let ratio = dggrs.getRefinementRatio();
   let maxLevel = dggrs.getMaxDGGRSZoneLevel();

   println!("Refinement Ratio: {ratio}");
   println!("Maximum level for 64-bit global identifiers (DGGAL DGGRSZone): {maxLevel}");
   println!("Default ~64K sub-zones relative depth: {depth64k}");
   0 // No error
}

fn displayInfo(dggrs: DGGRS, zone: dggal::DGGRSZone, options: HashMap<&str, &str>) -> i32
{
   if zone != nullZone {
      zoneInfo(dggrs, zone, options)
   } else {
      dggrsInfo(dggrs, options)
   }
}

fn main()
{
   let args: Vec<String> = env::args().collect();
   let argc = args.len();
   let myApp = Application::new(&args);
   let dggal = DGGAL::new(&myApp);
   let mut exitCode: i32 = 0;
   let mut show_syntax = false;
   let mut dggrsName = "";
   let mut a : usize = 1;
   let mut zoneID: &str = "";
   let arg0: &str = &args[0];
   let mut options = HashMap::<&str, &str>::new();

        if arg0.eq_ignore_ascii_case("i3h") { dggrsName = &"ISEA3H"; }
   else if arg0.eq_ignore_ascii_case("i9r") { dggrsName = &"ISEA9R"; }
   else if arg0.eq_ignore_ascii_case("ggg") { dggrsName = &"GNOSISGlobalGrid"; }

   if dggrsName == "" && argc > 1
   {
      let arg1: &str = &args[1];
           if arg1.eq_ignore_ascii_case("isea3h") { dggrsName = &"ISEA3H"; }
      else if arg1.eq_ignore_ascii_case("isea9r") { dggrsName = &"ISEA9R"; }
      else if arg1.eq_ignore_ascii_case("gnosis") { dggrsName = &"GNOSISGlobalGrid"; }
      a += 1;
   }

   if argc > a {
      zoneID = &args[a];
      a+=1;
   }

   while a < argc
   {
      let key = &args[a];
      a+=1;
      if key.as_bytes()[0] == '-' as u8 && a < argc
      {
         options.insert(&key[1..], &args[a]);
         a+=1;
      }
      else
      {
         exitCode = 1;
         show_syntax = true;
      }
   }

   if dggrsName != "" && exitCode == 0
   {
      let dggrs: DGGRS = dggal.newDGGRS(dggrsName).expect("Unknown DGGRS");
      let mut zone = nullZone;

      println!("DGGRS: https://maps.gnosis.earth/ogcapi/dggrs/{dggrsName}");

      if zoneID != "" {
         zone = dggrs.getZoneFromTextID(zoneID);
         if zone == nullZone
         {
            println!("Invalid {dggrsName} zone identifier: {zoneID}");
            exitCode = 1;
         }
      }

      if exitCode == 0 {
         displayInfo(dggrs, zone, options);
      }
   }
   else
   {
      show_syntax = true;
      exitCode = 1;
   }

   if show_syntax {
      println!("Syntax:");
      println!("   info <dggrs> [zone] [options]");
      println!("where dggrs is one of gnosis, isea3h or isea9r");
   }

   std::process::exit(exitCode)
}
