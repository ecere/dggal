#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
//#![allow(unused_variables)]

extern crate ecrt_sys;
extern crate dggal_sys;

use std::ffi::CString;
use std::ffi::CStr;
use std::ffi::c_void;
use std::slice;

// *** This code should be moved to / generated inside the DGGAL rust bindings
pub const nullZone : dggal_sys::DGGRSZone = 0xFFFFFFFFFFFFFFFFu64;
pub const nullInst : ecrt_sys::Instance = 0 as ecrt_sys::Instance;
pub const nullPtr : *mut *mut c_void = 0 as *mut *mut c_void;

pub type GeoPoint = dggal_sys::GeoPoint;
pub type GeoExtent = dggal_sys::GeoExtent;
pub type DGGRSZone = dggal_sys::DGGRSZone;

pub struct DGGRS {
   imp: dggal_sys::DGGRS
}

pub struct Application {
   app: ecrt_sys::Application
}
impl Drop for Application {
   fn drop(&mut self) {
      unsafe {
         ecrt_sys::__eCNameSpace__eC__types__eInstance_DecRef(self.app);
      }
   }
}

impl Application {
   pub fn new(_args: &Vec<String>) -> Application
   {
      unsafe {
         let app = ecrt_sys::ecrt_init(nullInst, true as u32, false as u32, 0,
               0 /*ptr::null_mut::<* mut * mut i8>()*/ as * mut * mut i8); // TODO: argc, args);
         Application { app: app }
      }
   }
}

pub struct DGGAL {
   mDGGAL: ecrt_sys::Module
}

impl DGGAL {
   pub fn new(app: &Application) -> DGGAL
   {
      unsafe {
         DGGAL { mDGGAL: dggal_sys::dggal_init(app.app) }
      }
   }

   pub fn newDGGRS(&self, name: &str) -> Option<DGGRS>
   {
      let dggrsName = CString::new(name).unwrap();
      unsafe {
         let c = ecrt_sys::__eCNameSpace__eC__types__eSystem_FindClass(self.mDGGAL, dggrsName.as_ptr());

         if c != nullPtr as * mut ecrt_sys::Class {
            Some(DGGRS { imp: ecrt_sys::__eCNameSpace__eC__types__eInstance_New(c) as dggal_sys::DGGRS })
         }
         else {
            None
         }
      }
   }

}

impl DGGRS {
   // TODO: Could we use rust function-generating macros?

   pub fn getZoneFromTextID(&self, zoneID: &str) -> dggal_sys::DGGRSZone
   {
      let mut zone = nullZone;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneFromTextID_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zoneID: * const i8) -> dggal_sys::DGGRSZone = std::mem::transmute(cMethod);
            let csZoneID = CString::new(zoneID).unwrap();
            zone = method(self.imp, csZoneID.as_ptr());
         }
      }
      zone
   }

   pub fn getZoneLevel(&self, zone: dggal_sys::DGGRSZone) -> i32
   {
      let mut level = -1;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneLevel_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
            level = method(self.imp, zone);
         }
      }
      level
   }

   pub fn countZoneEdges(&self, zone: dggal_sys::DGGRSZone) -> i32
   {
      let mut level = -1;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_countZoneEdges_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
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
            depth = dggal_sys::DGGRS_get64KDepth.unwrap()(self.imp);
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
            depth = dggal_sys::DGGRS_getMaxDepth.unwrap()(self.imp);
         }
      }
      depth
   }

   pub fn getRefinementRatio(&self) -> i32
   {
      let mut depth = -1;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getRefinementRatio_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS) -> i32 = std::mem::transmute(cMethod);
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
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getMaxDGGRSZoneLevel_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS) -> i32 = std::mem::transmute(cMethod);
            depth = method(self.imp,);
         }
      }
      depth
   }

   pub fn getZoneWGS84Centroid(&self, zone: dggal_sys::DGGRSZone) -> dggal_sys::GeoPoint
   {
      let mut centroid = dggal_sys::GeoPoint { lat: 0.0, lon: 0.0 };
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneWGS84Centroid_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone, centroid: *mut dggal_sys::GeoPoint) = std::mem::transmute(cMethod);
            method(self.imp, zone, std::ptr::from_mut(&mut centroid));
         }
      }
      centroid
   }

   pub fn getZoneWGS84Vertices(&self, zone: dggal_sys::DGGRSZone) -> Vec<dggal_sys::GeoPoint>
   {
      let mut n: i32 = 0;
      let vertices: Vec<dggal_sys::GeoPoint>;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut v: [dggal_sys::GeoPoint; 6] = [dggal_sys::GeoPoint { lat: 0.0, lon: 0.0 }; 6]; // REVIEW: Anyway to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneWGS84Vertices_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone, vertices: *mut dggal_sys::GeoPoint) -> i32 = std::mem::transmute(cMethod);
            n = method(self.imp, zone, std::ptr::from_mut(&mut v[0]));
         }
         vertices = slice::from_raw_parts(&v[0], n as usize).to_vec();
      }
      vertices
   }

   pub fn getZoneArea(&self, zone: dggal_sys::DGGRSZone) -> f64
   {
      let mut area = 0.0;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneArea_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone) -> f64 = std::mem::transmute(cMethod);
            area = method(self.imp, zone);
         }
      }
      area
   }

   pub fn countSubZones(&self, zone: dggal_sys::DGGRSZone, depth: i32) -> u64
   {
      let mut count = 0;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_countSubZones_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone, depth: i32) -> u64 = std::mem::transmute(cMethod);
            count = method(self.imp, zone, depth);
         }
      }
      count
   }

   pub fn getZoneTextID(&self, zone: dggal_sys::DGGRSZone) -> String
   {
      let id: String;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut zoneID = [0i8; 256]; // REVIEW: Anyway to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneTextID_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone, zoneID: *mut i8) = std::mem::transmute(cMethod);
            method(self.imp, zone, std::ptr::from_mut(&mut zoneID[0]));
         }
         id = CStr::from_ptr(zoneID.as_ptr()).to_str().unwrap().to_string();
      }
      id
   }

   pub fn getZoneParents(&self, zone: dggal_sys::DGGRSZone) -> Vec<dggal_sys::DGGRSZone>
   {
      let mut n: i32 = 0;
      let parents: Vec<dggal_sys::DGGRSZone>;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut p = [nullZone; 3]; // REVIEW: Anyway to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneParents_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone, parents: *mut dggal_sys::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
            n = method(self.imp, zone, std::ptr::from_mut(&mut p[0]));
         }
         parents = slice::from_raw_parts(&p[0], n as usize).to_vec();
      }
      parents
   }

   pub fn getZoneChildren(&self, zone: dggal_sys::DGGRSZone) -> Vec<dggal_sys::DGGRSZone>
   {
      let mut n: i32 = 0;
      let children: Vec<dggal_sys::DGGRSZone>;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut ch = [nullZone; 9]; // REVIEW: Anyway to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneChildren_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone, children: *mut dggal_sys::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
            n = method(self.imp, zone, std::ptr::from_mut(&mut ch[0]));
         }
         children = slice::from_raw_parts(&ch[0], n as usize).to_vec();
      }
      children
   }

   pub fn getZoneNeighbors(&self, zone: dggal_sys::DGGRSZone, nbTypes: &mut [i32; 6]) -> Vec<dggal_sys::DGGRSZone>
   {
      let mut n: i32 = 0;
      let neighbors: Vec<dggal_sys::DGGRSZone>;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut nb = [nullZone; 6]; // REVIEW: Anyway to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneNeighbors_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone, neighbors: *mut dggal_sys::DGGRSZone, nbTypes: *mut i32) -> i32 = std::mem::transmute(cMethod);
            n = method(self.imp, zone, std::ptr::from_mut(&mut nb[0]), std::ptr::from_mut(&mut nbTypes[0]));
         }
         neighbors = slice::from_raw_parts(&nb[0], n as usize).to_vec();
      }
      neighbors
   }

   pub fn getZoneCentroidParent(&self, zone: dggal_sys::DGGRSZone) -> dggal_sys::DGGRSZone
   {
      let mut centroidParent: dggal_sys::DGGRSZone = nullZone;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneCentroidParent_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone) -> dggal_sys::DGGRSZone = std::mem::transmute(cMethod);
            centroidParent = method(self.imp, zone);
         }
      }
      centroidParent
   }

   pub fn getZoneCentroidChild(&self, zone: dggal_sys::DGGRSZone) -> dggal_sys::DGGRSZone
   {
      let mut centroidChild: dggal_sys::DGGRSZone = nullZone;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneCentroidChild_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone) -> dggal_sys::DGGRSZone = std::mem::transmute(cMethod);
            centroidChild = method(self.imp, zone);
         }
      }
      centroidChild
   }

   pub fn isZoneCentroidChild(&self, zone: dggal_sys::DGGRSZone) -> bool
   {
      let mut isZoneCentroidChild: bool = false;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_isZoneCentroidChild_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone) -> u32 = std::mem::transmute(cMethod);
            isZoneCentroidChild = method(self.imp, zone) != 0;
         }
      }
      isZoneCentroidChild
   }

   pub fn getZoneWGS84Extent(&self, zone: dggal_sys::DGGRSZone) -> dggal_sys::GeoExtent
   {
      let mut extent: dggal_sys::GeoExtent = dggal_sys::GeoExtent {    // REVIEW: Anyway to avoid this initialization?
         ll: dggal_sys::GeoPoint { lat: 0.0, lon: 0.0 },
         ur: dggal_sys::GeoPoint { lat: 0.0, lon: 0.0 } };
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullPtr { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneWGS84Extent_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone, vertices: *mut dggal_sys::GeoExtent) = std::mem::transmute(cMethod);
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
         ecrt_sys::__eCNameSpace__eC__types__eInstance_DecRef(self.imp as ecrt_sys::Instance);
      }
   }
}
