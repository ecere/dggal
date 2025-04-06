#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
//#![allow(unused_variables)]

use std::ffi::CString;
use std::ffi::CStr;
use std::ffi::c_void;
use std::slice;

#[allow(warnings)] mod dggal_cffi;

// *** This code should be moved to / generated inside the DGGAL rust bindings
pub const nullZone : dggal_cffi::DGGRSZone = 0xFFFFFFFFFFFFFFFFu64;
pub const nullInst : dggal_cffi::Instance = 0 as dggal_cffi::Instance;
pub const nullPtr : *mut *mut c_void = 0 as *mut *mut c_void;

pub type GeoPoint = dggal_cffi::GeoPoint;
pub type GeoExtent = dggal_cffi::GeoExtent;
pub type DGGRSZone = dggal_cffi::DGGRSZone;

pub struct DGGRS {
   imp: dggal_cffi::DGGRS
}

pub struct Application {
   app: dggal_cffi::Application
}
impl Drop for Application {
   fn drop(&mut self) {
      unsafe {
         dggal_cffi::__eCNameSpace__eC__types__eInstance_DecRef(self.app);
      }
   }
}

impl Application {
   pub fn new(_args: &Vec<String>) -> Application
   {
      unsafe {
         let app = dggal_cffi::ecrt_init(nullInst, true as u32, false as u32, 0,
               0 /*ptr::null_mut::<* mut * mut i8>()*/ as * mut * mut i8); // TODO: argc, args);
         Application { app: app }
      }
   }
}

pub struct DGGAL {
   mDGGAL: dggal_cffi::Module
}

impl DGGAL {
   pub fn new(app: &Application) -> DGGAL
   {
      unsafe {
         DGGAL { mDGGAL: dggal_cffi::dggal_init(app.app) }
      }
   }

   pub fn newDGGRS(&self, name: &str) -> Option<DGGRS>
   {
      let dggrsName = CString::new(name).unwrap();
      unsafe {
         let c = dggal_cffi::__eCNameSpace__eC__types__eSystem_FindClass(self.mDGGAL, dggrsName.as_ptr());

         if c != nullPtr as * mut dggal_cffi::Class {
            Some(DGGRS { imp: dggal_cffi::__eCNameSpace__eC__types__eInstance_New(c) as dggal_cffi::DGGRS })
         }
         else {
            None
         }
      }
   }

}

impl DGGRS {
   // TODO: Could we use rust function-generating macros?

   pub fn getZoneFromTextID(&self, zoneID: &str) -> dggal_cffi::DGGRSZone
   {
      let mut zone = nullZone;
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_getZoneFromTextID_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS, zoneID: * const i8) -> dggal_cffi::DGGRSZone = std::mem::transmute(cMethod);
            let csZoneID = CString::new(zoneID).unwrap();
            zone = method(self.imp, csZoneID.as_ptr());
         }
      }
      zone
   }

   pub fn getZoneLevel(&self, zone: dggal_cffi::DGGRSZone) -> i32
   {
      let mut level = -1;
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_getZoneLevel_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS, zone: dggal_cffi::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
            level = method(self.imp, zone);
         }
      }
      level
   }

   pub fn countZoneEdges(&self, zone: dggal_cffi::DGGRSZone) -> i32
   {
      let mut level = -1;
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_countZoneEdges_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS, zone: dggal_cffi::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
            level = method(self.imp, zone);
         }
      }
      level
   }

   // (not currently a virtual method)
   pub fn get64KDepth(&self) -> i32
   {
      let mut depth = -1;
      unsafe
      {
         if self.imp != nullInst {
            depth = dggal_cffi::DGGRS_get64KDepth.unwrap()(self.imp);
         }
      }
      depth
   }

   // (not currently a virtual method)
   pub fn getMaxDepth(&self) -> i32
   {
      let mut depth = -1;
      unsafe
      {
         if self.imp != nullInst {
            depth = dggal_cffi::DGGRS_getMaxDepth.unwrap()(self.imp);
         }
      }
      depth
   }

   pub fn getRefinementRatio(&self) -> i32
   {
      let mut depth = -1;
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_getRefinementRatio_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS) -> i32 = std::mem::transmute(cMethod);
            depth = method(self.imp,);
         }
      }
      depth
   }

   pub fn getMaxDGGRSZoneLevel(&self) -> i32
   {
      let mut depth = -1;
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_getMaxDGGRSZoneLevel_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS) -> i32 = std::mem::transmute(cMethod);
            depth = method(self.imp,);
         }
      }
      depth
   }

   pub fn getZoneWGS84Centroid(&self, zone: dggal_cffi::DGGRSZone) -> dggal_cffi::GeoPoint
   {
      let mut centroid = dggal_cffi::GeoPoint { lat: 0.0, lon: 0.0 };
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_getZoneWGS84Centroid_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS, zone: dggal_cffi::DGGRSZone, centroid: *mut dggal_cffi::GeoPoint) = std::mem::transmute(cMethod);
            method(self.imp, zone, std::ptr::from_mut(&mut centroid));
         }
      }
      centroid
   }

   pub fn getZoneWGS84Vertices(&self, zone: dggal_cffi::DGGRSZone) -> Vec<dggal_cffi::GeoPoint>
   {
      let mut n: i32 = 0;
      let vertices: Vec<dggal_cffi::GeoPoint>;
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut v: [dggal_cffi::GeoPoint; 6] = [dggal_cffi::GeoPoint { lat: 0.0, lon: 0.0 }; 6]; // REVIEW: Anyway to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_getZoneWGS84Vertices_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS, zone: dggal_cffi::DGGRSZone, vertices: *mut dggal_cffi::GeoPoint) -> i32 = std::mem::transmute(cMethod);
            n = method(self.imp, zone, std::ptr::from_mut(&mut v[0]));
         }
         vertices = slice::from_raw_parts(&v[0], n as usize).to_vec();
      }
      vertices
   }

   pub fn getZoneArea(&self, zone: dggal_cffi::DGGRSZone) -> f64
   {
      let mut area = 0.0;
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_getZoneArea_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS, zone: dggal_cffi::DGGRSZone) -> f64 = std::mem::transmute(cMethod);
            area = method(self.imp, zone);
         }
      }
      area
   }

   pub fn countSubZones(&self, zone: dggal_cffi::DGGRSZone, depth: i32) -> u64
   {
      let mut count = 0;
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_countSubZones_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS, zone: dggal_cffi::DGGRSZone, depth: i32) -> u64 = std::mem::transmute(cMethod);
            count = method(self.imp, zone, depth);
         }
      }
      count
   }

   pub fn getZoneTextID(&self, zone: dggal_cffi::DGGRSZone) -> String
   {
      let id: String;
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut zoneID = [0i8; 256]; // REVIEW: Anyway to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_getZoneTextID_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS, zone: dggal_cffi::DGGRSZone, zoneID: *mut i8) = std::mem::transmute(cMethod);
            method(self.imp, zone, std::ptr::from_mut(&mut zoneID[0]));
         }
         id = CStr::from_ptr(zoneID.as_ptr()).to_str().unwrap().to_string();
      }
      id
   }

   pub fn getZoneParents(&self, zone: dggal_cffi::DGGRSZone) -> Vec<dggal_cffi::DGGRSZone>
   {
      let mut n: i32 = 0;
      let parents: Vec<dggal_cffi::DGGRSZone>;
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut p = [nullZone; 3]; // REVIEW: Anyway to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_getZoneParents_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS, zone: dggal_cffi::DGGRSZone, parents: *mut dggal_cffi::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
            n = method(self.imp, zone, std::ptr::from_mut(&mut p[0]));
         }
         parents = slice::from_raw_parts(&p[0], n as usize).to_vec();
      }
      parents
   }

   pub fn getZoneChildren(&self, zone: dggal_cffi::DGGRSZone) -> Vec<dggal_cffi::DGGRSZone>
   {
      let mut n: i32 = 0;
      let children: Vec<dggal_cffi::DGGRSZone>;
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut ch = [nullZone; 9]; // REVIEW: Anyway to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_getZoneChildren_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS, zone: dggal_cffi::DGGRSZone, children: *mut dggal_cffi::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
            n = method(self.imp, zone, std::ptr::from_mut(&mut ch[0]));
         }
         children = slice::from_raw_parts(&ch[0], n as usize).to_vec();
      }
      children
   }

   pub fn getZoneNeighbors(&self, zone: dggal_cffi::DGGRSZone, nbTypes: &mut [i32; 6]) -> Vec<dggal_cffi::DGGRSZone>
   {
      let mut n: i32 = 0;
      let neighbors: Vec<dggal_cffi::DGGRSZone>;
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut nb = [nullZone; 6]; // REVIEW: Anyway to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_getZoneNeighbors_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS, zone: dggal_cffi::DGGRSZone, neighbors: *mut dggal_cffi::DGGRSZone, nbTypes: *mut i32) -> i32 = std::mem::transmute(cMethod);
            n = method(self.imp, zone, std::ptr::from_mut(&mut nb[0]), std::ptr::from_mut(&mut nbTypes[0]));
         }
         neighbors = slice::from_raw_parts(&nb[0], n as usize).to_vec();
      }
      neighbors
   }

   pub fn getZoneCentroidParent(&self, zone: dggal_cffi::DGGRSZone) -> dggal_cffi::DGGRSZone
   {
      let mut centroidParent: dggal_cffi::DGGRSZone = nullZone;
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_getZoneCentroidParent_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS, zone: dggal_cffi::DGGRSZone) -> dggal_cffi::DGGRSZone = std::mem::transmute(cMethod);
            centroidParent = method(self.imp, zone);
         }
      }
      centroidParent
   }

   pub fn getZoneCentroidChild(&self, zone: dggal_cffi::DGGRSZone) -> dggal_cffi::DGGRSZone
   {
      let mut centroidChild: dggal_cffi::DGGRSZone = nullZone;
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_getZoneCentroidChild_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS, zone: dggal_cffi::DGGRSZone) -> dggal_cffi::DGGRSZone = std::mem::transmute(cMethod);
            centroidChild = method(self.imp, zone);
         }
      }
      centroidChild
   }

   pub fn isZoneCentroidChild(&self, zone: dggal_cffi::DGGRSZone) -> bool
   {
      let mut isZoneCentroidChild: bool = false;
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_isZoneCentroidChild_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS, zone: dggal_cffi::DGGRSZone) -> u32 = std::mem::transmute(cMethod);
            isZoneCentroidChild = method(self.imp, zone) != 0;
         }
      }
      isZoneCentroidChild
   }

   pub fn getZoneWGS84Extent(&self, zone: dggal_cffi::DGGRSZone) -> dggal_cffi::GeoExtent
   {
      let mut extent: dggal_cffi::GeoExtent = dggal_cffi::GeoExtent {    // REVIEW: Anyway to avoid this initialization?
         ll: dggal_cffi::GeoPoint { lat: 0.0, lon: 0.0 },
         ur: dggal_cffi::GeoPoint { lat: 0.0, lon: 0.0 } };
      unsafe
      {
         let c = dggal_cffi::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_cffi::DGGRS_getZoneWGS84Extent_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_cffi::DGGRS, zone: dggal_cffi::DGGRSZone, vertices: *mut dggal_cffi::GeoExtent) = std::mem::transmute(cMethod);
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
         dggal_cffi::__eCNameSpace__eC__types__eInstance_DecRef(self.imp as dggal_cffi::Instance);
      }
   }
}
