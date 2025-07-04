#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
//#![allow(unused_variables)]

extern crate ecrt_sys;

extern crate ecrt;

use ecrt::define_bitclass;
use ecrt::Application;
use ecrt::File;
use ecrt::nullVTbl;
use ecrt::nullInst;
use ecrt::nullPtr;
use ecrt::Array;
use ecrt::Map;
use ecrt::FieldValue;
use ecrt::ConstString;
use ecrt::Instance;
use ecrt::TTAU64;
use ecrt::delegate_ttau64_and_default;

extern crate dggal_sys;

use std::ffi::CString;
use std::ffi::CStr;
use std::ffi::c_void;
use std::slice;
use std::mem;
use std::f64::consts::PI;
use std::ops::Deref;
/////

pub type GeoPoint = dggal_sys::GeoPoint;
pub type GeoExtent = dggal_sys::GeoExtent;
pub type DGGRSZone = dggal_sys::DGGRSZone;

pub const nullZone : DGGRSZone = 0xFFFFFFFFFFFFFFFFu64;

pub const wholeWorld: GeoExtent = GeoExtent {
   ll: GeoPoint { lat: -90.0 * PI / 180.0, lon : -180.0 * PI / 180.0 },
   ur: GeoPoint { lat:  90.0 * PI / 180.0, lon :  180.0 * PI / 180.0 }
};

define_bitclass! { CRS, dggal_sys::CRS,
    registry => { set: set_registry, is_bool: false, type: dggal_sys::CRSRegistry, prim_type: u32, mask: dggal_sys::CRS_registry_MASK, shift: dggal_sys::CRS_registry_SHIFT },
    crsID =>    { set: set_crsID,    is_bool: false, type: u32,                    prim_type: u32, mask: dggal_sys::CRS_crsID_MASK,    shift: dggal_sys::CRS_crsID_SHIFT },
    h =>        { set: set_h,        is_bool: true,  type: bool,                   prim_type: u32, mask: dggal_sys::CRS_h_MASK,        shift: dggal_sys::CRS_h_SHIFT }
}

#[macro_export] macro_rules! CRS {
   ($registry:expr, $crsID:expr $(, $h:expr)?) => {
      {
         let mut instance = CRS(0);
         instance.set_registry($registry);
         instance.set_crsID($crsID);
         $(instance.set_h($h);)?
         instance
      }
   };
}

pub const epsg : dggal_sys::CRSRegistry = dggal_sys::CRSRegistry_CRSRegistry_epsg;
pub const ogc  : dggal_sys::CRSRegistry = dggal_sys::CRSRegistry_CRSRegistry_ogc;

pub struct DGGRS {
   imp: dggal_sys::DGGRS,
   mDGGAL: ecrt_sys::Module
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
}

impl DGGRS {
   pub fn new(dggal: &DGGAL, name: &str) -> Result<Self, String>
   {
      let dggrsName = CString::new(name).unwrap();
      unsafe {
         let c = ecrt_sys::__eCNameSpace__eC__types__eSystem_FindClass(dggal.mDGGAL, dggrsName.as_ptr());
         if c != nullVTbl as * mut ecrt_sys::Class {
            Ok(DGGRS { imp: ecrt_sys::__eCNameSpace__eC__types__eInstance_New(c) as dggal_sys::DGGRS, mDGGAL: dggal.mDGGAL })
         }
         else {
            Err(format!("Failure to instantiate DGGRS {name}"))
         }
      }
   }

   // TODO: Could we use rust function-generating macros?

   // These are the virtual methods:
   pub fn getZoneFromTextID(&self, zoneID: &str) -> dggal_sys::DGGRSZone
   {
      let mut zone = nullZone;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
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
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
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
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_countZoneEdges_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone) -> i32 = std::mem::transmute(cMethod);
            level = method(self.imp, zone);
         }
      }
      level
   }

   pub fn getRefinementRatio(&self) -> i32
   {
      let mut depth = -1;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
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
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
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
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
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
      let vertices: Vec<dggal_sys::GeoPoint>;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut v: [dggal_sys::GeoPoint; 6] = [dggal_sys::GeoPoint { lat: 0.0, lon: 0.0 }; 6]; // REVIEW: Any way to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneWGS84Vertices_vTblID as usize));
         let mut n: i32 = 0;
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
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
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
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
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
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut zoneID = [0i8; 256]; // REVIEW: Any way to avoid this initialization?
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
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut p = [nullZone; 3]; // REVIEW: Any way to avoid this initialization?
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
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut ch = [nullZone; 9]; // REVIEW: Any way to avoid this initialization?
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
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut nb = [nullZone; 6]; // REVIEW: Any way to avoid this initialization?
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
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
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
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
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
      let mut result: bool = false;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_isZoneCentroidChild_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone) -> u32 = std::mem::transmute(cMethod);
            result = method(self.imp, zone) != 0;
         }
      }
      result
   }

   pub fn getZoneWGS84Extent(&self, zone: dggal_sys::DGGRSZone) -> dggal_sys::GeoExtent
   {
      let mut extent: dggal_sys::GeoExtent = dggal_sys::GeoExtent {    // REVIEW: Any way to avoid this initialization?
         ll: dggal_sys::GeoPoint { lat: 0.0, lon: 0.0 },
         ur: dggal_sys::GeoPoint { lat: 0.0, lon: 0.0 } };
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneWGS84Extent_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone, extent: *mut dggal_sys::GeoExtent) = std::mem::transmute(cMethod);
            method(self.imp, zone, std::ptr::from_mut(&mut extent));
         }
      }
      extent
   }

   pub fn listZones(&self, level: i32, bbox: &dggal_sys::GeoExtent) -> Vec<dggal_sys::DGGRSZone>
   {
      let zones: Vec<dggal_sys::DGGRSZone>;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_listZones_vTblID as usize));
         let mut n: usize = 0;
         let mut a: *const dggal_sys::DGGRSZone = nullPtr as *const dggal_sys::DGGRSZone;

         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, level: i32, bbox: *const dggal_sys::GeoExtent) -> dggal_sys::template_Array_DGGRSZone = std::mem::transmute(cMethod);
            let az: dggal_sys::template_Array_DGGRSZone = method(self.imp, level, bbox);
            if az != nullInst {
               let am: *const ecrt_sys::class_members_Array = ((az as *const i8).wrapping_add((*ecrt_sys::class_Array).offset as usize)) as *const ecrt_sys::class_members_Array;
               n = (*am).count as usize;
               a = (*am).array;
            }
         }
         zones = slice::from_raw_parts(a, n).to_vec();
      }
      zones
   }

   pub fn getZoneRefinedWGS84Vertices(&self, zone: dggal_sys::DGGRSZone, refinement: i32) -> Vec<dggal_sys::GeoPoint>
   {
      let vertices: Vec<dggal_sys::GeoPoint>;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut n: usize = 0;
         let mut a: *const dggal_sys::GeoPoint = nullPtr as *const dggal_sys::GeoPoint;

         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneRefinedWGS84Vertices_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone, refinement: i32) -> dggal_sys::template_Array_GeoPoint = std::mem::transmute(cMethod);
            let ap: dggal_sys::template_Array_GeoPoint = method(self.imp, zone, refinement);
            if ap != nullInst {
               let am: *const ecrt_sys::class_members_Array = ((ap as *const i8).wrapping_add((*ecrt_sys::class_Array).offset as usize)) as *const ecrt_sys::class_members_Array;
               n = (*am).count as usize;
               a = (*am).array as *const dggal_sys::GeoPoint;
            }
         }
         vertices = slice::from_raw_parts(a, n).to_vec();
      }
      vertices
   }

   pub fn getSubZones(&self, parent: dggal_sys::DGGRSZone, relativeDepth: i32) -> Vec<dggal_sys::DGGRSZone>
   {
      let zones: Vec<dggal_sys::DGGRSZone>;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getSubZones_vTblID as usize));
         let mut n: usize = 0;
         let mut a: *const dggal_sys::DGGRSZone = nullPtr as *const dggal_sys::DGGRSZone;

         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, parent: dggal_sys::DGGRSZone, relativeDepth: i32) -> dggal_sys::template_Array_DGGRSZone = std::mem::transmute(cMethod);
            let az: dggal_sys::template_Array_DGGRSZone = method(self.imp, parent, relativeDepth);
            if az != nullInst {
               let am: *const ecrt_sys::class_members_Array = ((az as *const i8).wrapping_add((*ecrt_sys::class_Array).offset as usize)) as *const ecrt_sys::class_members_Array;
               n = (*am).count as usize;
               a = (*am).array;
            }
         }
         zones = slice::from_raw_parts(a, n).to_vec();
      }
      zones
   }

   pub fn getZoneFromWGS84Centroid(&self, level: i32, centroid: &dggal_sys::GeoPoint) -> dggal_sys::DGGRSZone
   {
      let mut zone = nullZone;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneFromWGS84Centroid_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, level: i32, centroid: * const dggal_sys::GeoPoint) -> dggal_sys::DGGRSZone = std::mem::transmute(cMethod);
            zone = method(self.imp, level, centroid);
         }
      }
      zone
   }

   pub fn getZoneFromCRSCentroid(&self, level: i32, crs: CRS, centroid: &ecrt_sys::Pointd) -> dggal_sys::DGGRSZone
   {
      let mut zone = nullZone;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneFromCRSCentroid_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, level: i32, crs: dggal_sys::CRS, centroid: * const ecrt_sys::Pointd) -> dggal_sys::DGGRSZone = std::mem::transmute(cMethod);
            zone = method(self.imp, level, *crs, centroid);
         }
      }
      zone
   }

   pub fn countZones(&self, level: i32) -> u64
   {
      let mut count = 0;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_countZones_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, level: i32) -> u64 = std::mem::transmute(cMethod);
            count = method(self.imp, level);
         }
      }
      count
   }

   pub fn getFirstSubZone(&self, parent: dggal_sys::DGGRSZone, relativeDepth: i32) -> dggal_sys::DGGRSZone
   {
      let mut zone = nullZone;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getFirstSubZone_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, parent: dggal_sys::DGGRSZone, relativeDepth: i32) -> dggal_sys::DGGRSZone = std::mem::transmute(cMethod);
            zone = method(self.imp, parent, relativeDepth);
         }
      }
      zone
   }

   pub fn getIndexMaxDepth(&self) -> i32
   {
      let mut maxDepth = 0;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getIndexMaxDepth_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS) -> i32 = std::mem::transmute(cMethod);
            maxDepth = method(self.imp);
         }
      }
      maxDepth
   }

   pub fn getMaxChildren(&self) -> i32
   {
      let mut maxChildren = 0;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getMaxChildren_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS) -> i32 = std::mem::transmute(cMethod);
            maxChildren = method(self.imp);
         }
      }
      maxChildren
   }

   pub fn getMaxNeighbors(&self) -> i32
   {
      let mut maxNeighbors = 0;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getMaxNeighbors_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS) -> i32 = std::mem::transmute(cMethod);
            maxNeighbors = method(self.imp);
         }
      }
      maxNeighbors
   }

   pub fn getMaxParents(&self) -> i32
   {
      let mut maxParents = 0;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getMaxParents_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS) -> i32 = std::mem::transmute(cMethod);
            maxParents = method(self.imp);
         }
      }
      maxParents
   }

   pub fn getSubZoneAtIndex(&self, parent: dggal_sys::DGGRSZone, index: i64) -> dggal_sys::DGGRSZone
   {
      let mut zone = nullZone;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getSubZoneAtIndex_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, parent: dggal_sys::DGGRSZone, index: i64) -> dggal_sys::DGGRSZone = std::mem::transmute(cMethod);
            zone = method(self.imp, parent, index);
         }
      }
      zone
   }

   pub fn getSubZoneIndex(&self, parent: dggal_sys::DGGRSZone, subZone: dggal_sys::DGGRSZone) -> i64
   {
      let mut index = -1;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getSubZoneIndex_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, parent: dggal_sys::DGGRSZone, subZone: dggal_sys::DGGRSZone) -> i64 = std::mem::transmute(cMethod);
            index = method(self.imp, parent, subZone);
         }
      }
      index
   }

   pub fn getSubZoneCRSCentroids(&self, parent: dggal_sys::DGGRSZone, crs: CRS, relativeDepth: i32) -> Vec<ecrt_sys::Pointd>
   {
      let centroids: Vec<ecrt_sys::Pointd>;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut n: usize = 0;
         let mut a: *const ecrt_sys::Pointd = nullPtr as *const ecrt_sys::Pointd;

         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getSubZoneCRSCentroids_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, parent: dggal_sys::DGGRSZone, crs: dggal_sys::CRS, relativeDepth: i32) -> dggal_sys::template_Array_Pointd = std::mem::transmute(cMethod);
            let ap: dggal_sys::template_Array_Pointd = method(self.imp, parent, *crs, relativeDepth);
            if ap != nullInst {
               let am: *const ecrt_sys::class_members_Array = ((ap as *const i8).wrapping_add((*ecrt_sys::class_Array).offset as usize)) as *const ecrt_sys::class_members_Array;
               n = (*am).count as usize;
               a = (*am).array as *const ecrt_sys::Pointd;
            }
         }
         centroids = slice::from_raw_parts(a, n).to_vec();
      }
      centroids
   }

   pub fn getSubZoneWGS84Centroids(&self, parent: dggal_sys::DGGRSZone, relativeDepth: i32) -> Vec<dggal_sys::GeoPoint>
   {
      let centroids: Vec<dggal_sys::GeoPoint>;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut n: usize = 0;
         let mut a: *const dggal_sys::GeoPoint = nullPtr as *const dggal_sys::GeoPoint;

         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getSubZoneWGS84Centroids_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, parent: dggal_sys::DGGRSZone, relativeDepth: i32) -> dggal_sys::template_Array_GeoPoint = std::mem::transmute(cMethod);
            let ap: dggal_sys::template_Array_GeoPoint = method(self.imp, parent, relativeDepth);
            if ap != nullInst {
               let am: *const ecrt_sys::class_members_Array = ((ap as *const i8).wrapping_add((*ecrt_sys::class_Array).offset as usize)) as *const ecrt_sys::class_members_Array;
               n = (*am).count as usize;
               a = (*am).array as *const dggal_sys::GeoPoint;
            }
         }
         centroids = slice::from_raw_parts(a, n).to_vec();
      }
      centroids
   }

   pub fn getZoneCRSVertices(&self, zone: dggal_sys::DGGRSZone, crs: CRS) -> Vec<ecrt_sys::Pointd>
   {
      let vertices: Vec<ecrt_sys::Pointd>;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut v: [ecrt_sys::Pointd; 6] = [ecrt_sys::Pointd { x: 0.0, y: 0.0 }; 6]; // REVIEW: Any way to avoid this initialization?
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneCRSVertices_vTblID as usize));
         let mut n: i32 = 0;
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone, crs: dggal_sys::CRS, vertices: *mut ecrt_sys::Pointd) -> i32 = std::mem::transmute(cMethod);
            n = method(self.imp, zone, *crs, std::ptr::from_mut(&mut v[0]));
         }
         vertices = slice::from_raw_parts(&v[0], n as usize).to_vec();
      }
      vertices
   }

   pub fn getZoneRefinedCRSVertices(&self, zone: dggal_sys::DGGRSZone, crs: CRS, refinement: i32) -> Vec<ecrt_sys::Pointd>
   {
      let vertices: Vec<ecrt_sys::Pointd>;
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let mut n: usize = 0;
         let mut a: *const ecrt_sys::Pointd = nullPtr as *const ecrt_sys::Pointd;

         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneRefinedCRSVertices_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone, crs: dggal_sys::CRS, refinement: i32) -> dggal_sys::template_Array_GeoPoint = std::mem::transmute(cMethod);
            let ap: dggal_sys::template_Array_GeoPoint = method(self.imp, zone, *crs, refinement);
            if ap != nullInst {
               let am: *const ecrt_sys::class_members_Array = ((ap as *const i8).wrapping_add((*ecrt_sys::class_Array).offset as usize)) as *const ecrt_sys::class_members_Array;
               n = (*am).count as usize;
               a = (*am).array as *const ecrt_sys::Pointd;
            }
         }
         vertices = slice::from_raw_parts(a, n).to_vec();
      }
      vertices
   }

   pub fn getZoneCRSCentroid(&self, zone: dggal_sys::DGGRSZone, crs: CRS) -> ecrt_sys::Pointd
   {
      let mut centroid = ecrt_sys::Pointd { x: 0.0, y: 0.0 };
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneCRSCentroid_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone, crs: dggal_sys::CRS, centroid: *mut ecrt_sys::Pointd) = std::mem::transmute(cMethod);
            method(self.imp, zone, *crs, std::ptr::from_mut(&mut centroid));
         }
      }
      centroid
   }

   pub fn getZoneCRSExtent(&self, zone: dggal_sys::DGGRSZone, crs: CRS) -> dggal_sys::CRSExtent
   {
      let mut extent: dggal_sys::CRSExtent = dggal_sys::CRSExtent {    // REVIEW: Any way to avoid this initialization?
         tl: ecrt_sys::Pointd { x: 0.0, y: 0.0 },
         br: ecrt_sys::Pointd { x: 0.0, y: 0.0 },
         crs: 0 };
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_getZoneCRSExtent_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zone: dggal_sys::DGGRSZone, crs: dggal_sys::CRS, extent: *mut dggal_sys::CRSExtent) = std::mem::transmute(cMethod);
            method(self.imp, zone, *crs, std::ptr::from_mut(&mut extent));
         }
      }
      extent
   }

   pub fn compactZones(&self, zones: &mut Vec<dggal_sys::DGGRSZone>)
   {
      unsafe
      {
         let c = dggal_sys::class_DGGRS;
         let vTbl = if self.imp != nullInst && (*self.imp)._vTbl != nullVTbl { (*self.imp)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(dggal_sys::DGGRS_compactZones_vTblID as usize));
         if cMethod != std::mem::transmute(0usize) {
            let mut n: usize = zones.len();
            if n != 0 {
               let method : unsafe extern "C" fn(dggrs: dggal_sys::DGGRS, zones: dggal_sys::template_Array_DGGRSZone) = std::mem::transmute(cMethod);
               let ac = ecrt_sys::__eCNameSpace__eC__types__eSystem_FindClass(self.mDGGAL, "Array<DGGRSZone>\0".as_ptr() as *const i8);
               let az: dggal_sys::template_Array_DGGRSZone = ecrt_sys::__eCNameSpace__eC__types__eInstance_New(ac) as dggal_sys::template_Array_DGGRSZone;
               let am: *const ecrt_sys::class_members_Array;
               let mut a: *mut dggal_sys::DGGRSZone;

               ecrt_sys::Array_set_size.unwrap()(az, n as u32);
               am = ((az as *const i8).wrapping_add((*ecrt_sys::class_Array).offset as usize)) as *const ecrt_sys::class_members_Array;
               a = (*am).array as *mut dggal_sys::DGGRSZone;
               ecrt_sys::memcpy(a as *mut c_void, zones.as_ptr() as *const c_void, (mem::size_of::<dggal_sys::DGGRSZone>() * n) as std::os::raw::c_ulong);
               method(self.imp, az);

               n = (*am).count as usize;
               a = (*am).array;

               zones.reserve(n);
               zones.set_len(n);
               ecrt_sys::memcpy(zones.as_ptr() as *mut c_void, a as *const c_void, (mem::size_of::<dggal_sys::DGGRSZone>() * n) as std::os::raw::c_ulong);

               ecrt_sys::__eCNameSpace__eC__types__eInstance_DecRef(az);
            }
         }
      }
   }

   // These methods are NOT virtual:
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

   pub fn areZonesNeighbors(&self, a: dggal_sys::DGGRSZone, b: dggal_sys::DGGRSZone) -> bool
   {
      let mut result: bool = false;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_areZonesNeighbors.unwrap()(self.imp, a, b) != 0;
         }
      }
      result
   }

   pub fn areZonesSiblings(&self, a: dggal_sys::DGGRSZone, b: dggal_sys::DGGRSZone) -> bool
   {
      let mut result: bool = false;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_areZonesSiblings.unwrap()(self.imp, a, b) != 0;
         }
      }
      result
   }

   pub fn doZonesOverlap(&self, a: dggal_sys::DGGRSZone, b: dggal_sys::DGGRSZone) -> bool
   {
      let mut result: bool = false;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_doZonesOverlap.unwrap()(self.imp, a, b) != 0;
         }
      }
      result
   }

   pub fn doesZoneContain(&self, hayStack: dggal_sys::DGGRSZone, needle: dggal_sys::DGGRSZone) -> bool
   {
      let mut result: bool = false;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_doesZoneContain.unwrap()(self.imp, hayStack, needle) != 0;
         }
      }
      result
   }

   pub fn isZoneAncestorOf(&self, ancestor: dggal_sys::DGGRSZone, descendant: dggal_sys::DGGRSZone, maxDepth: i32) -> bool
   {
      let mut result: bool = false;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_isZoneAncestorOf.unwrap()(self.imp, ancestor, descendant, maxDepth) != 0;
         }
      }
      result
   }

   pub fn isZoneContainedIn(&self, needle: dggal_sys::DGGRSZone, hayStack: dggal_sys::DGGRSZone) -> bool
   {
      let mut result: bool = false;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_isZoneContainedIn.unwrap()(self.imp, needle, hayStack) != 0;
         }
      }
      result
   }

   pub fn isZoneDescendantOf(&self, descendant: dggal_sys::DGGRSZone, ancestor: dggal_sys::DGGRSZone, maxDepth: i32) -> bool
   {
      let mut result: bool = false;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_isZoneDescendantOf.unwrap()(self.imp, descendant, ancestor, maxDepth) != 0;
         }
      }
      result
   }

   pub fn isZoneImmediateChildOf(&self, child: dggal_sys::DGGRSZone, parent: dggal_sys::DGGRSZone) -> bool
   {
      let mut result: bool = false;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_isZoneImmediateChildOf.unwrap()(self.imp, child, parent) != 0;
         }
      }
      result
   }

   pub fn isZoneImmediateParentOf(&self, parent: dggal_sys::DGGRSZone, child: dggal_sys::DGGRSZone) -> bool
   {
      let mut result: bool = false;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_isZoneImmediateParentOf.unwrap()(self.imp, parent, child) != 0;
         }
      }
      result
   }

   pub fn zoneHasSubZone(&self, hayStack: dggal_sys::DGGRSZone, needle: dggal_sys::DGGRSZone) -> bool
   {
      let mut result: bool = false;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_zoneHasSubZone.unwrap()(self.imp, hayStack, needle) != 0;
         }
      }
      result
   }

   pub fn getLevelFromMetersPerSubZone(&self, physicalMetersPerSubZone: f64, relativeDepth: i32) -> i32
   {
      let mut result: i32 = 0;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_getLevelFromMetersPerSubZone.unwrap()(self.imp, physicalMetersPerSubZone, relativeDepth);
         }
      }
      result
   }

   pub fn getLevelFromPixelsAndExtent(&self, extent: &dggal_sys::GeoExtent, pixels: &ecrt_sys::Point, relativeDepth: i32) -> i32
   {
      let mut result: i32 = 0;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_getLevelFromPixelsAndExtent.unwrap()(self.imp, extent, pixels, relativeDepth);
         }
      }
      result
   }

   pub fn getLevelFromRefZoneArea(&self, metersSquared: f64) -> i32
   {
      let mut result: i32 = 0;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_getLevelFromRefZoneArea.unwrap()(self.imp, metersSquared);
         }
      }
      result
   }

   pub fn getLevelFromScaleDenominator(&self, scaleDenominator: f64, relativeDepth: i32, mmPerPixel: f64) -> i32
   {
      let mut result: i32 = 0;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_getLevelFromScaleDenominator.unwrap()(self.imp, scaleDenominator, relativeDepth, mmPerPixel);
         }
      }
      result
   }

   pub fn getMetersPerSubZoneFromLevel(&self, parentLevel: i32, relativeDepth: i32) -> f64
   {
      let mut result: f64 = 0.0;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_getMetersPerSubZoneFromLevel.unwrap()(self.imp, parentLevel, relativeDepth);
         }
      }
      result
   }

   pub fn getRefZoneArea(&self, level: i32) -> f64
   {
      let mut result: f64 = 0.0;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_getRefZoneArea.unwrap()(self.imp, level);
         }
      }
      result
   }

   pub fn getScaleDenominatorFromLevel(&self, parentLevel: i32, relativeDepth: i32, mmPerPixel: f64) -> f64
   {
      let mut result: f64 = 0.0;
      unsafe
      {
         if self.imp != nullInst {
            result = dggal_sys::DGGRS_getScaleDenominatorFromLevel.unwrap()(self.imp, parentLevel, relativeDepth, mmPerPixel);
         }
      }
      result
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

unsafe impl Sync for DGGRS {

}

#[repr(transparent)]
pub struct DGGSJSONDepth(pub Instance);
delegate_ttau64_and_default!(DGGSJSONDepth);

impl DGGSJSONDepth
{
   pub fn data(&self) -> Array<FieldValue>
   {
      let mut data = Array::<FieldValue>::new(nullInst);
      if self.0.0 != nullInst {
         unsafe {
            let members: *const dggal_sys::class_members_DGGSJSONDepth = ((self.0.0 as *const i8).wrapping_add((*dggal_sys::class_DGGSJSONDepth).offset as usize)) as *const dggal_sys::class_members_DGGSJSONDepth;
            data.array = (*members).data;
         }
      }
      data
   }
}

#[repr(transparent)]
pub struct DGGSJSON(pub Instance);
delegate_ttau64_and_default!(DGGSJSON);

impl DGGSJSON {
   pub fn dggrs(&self) -> ConstString
   {
      let mut dggrs: ConstString = Default::default();
      if self.0.0 != nullInst {
         unsafe {
            let members: *const dggal_sys::class_members_DGGSJSON = ((self.0.0 as *const i8).wrapping_add((*dggal_sys::class_DGGSJSON).offset as usize)) as *const dggal_sys::class_members_DGGSJSON;
            dggrs = ConstString((*members).dggrs);
         }
      }
      dggrs
   }

   pub fn zoneId(&self) -> ConstString
   {
      let mut zoneId: ConstString = Default::default();
      if self.0.0 != nullInst {
         unsafe {
            let members: *const dggal_sys::class_members_DGGSJSON = ((self.0.0 as *const i8).wrapping_add((*dggal_sys::class_DGGSJSON).offset as usize)) as *const dggal_sys::class_members_DGGSJSON;
            zoneId = ConstString((*members).zoneId);
         }
      }
      zoneId
   }

   pub fn depths(&self) -> Array<i32>
   {
      let mut depths = Array::<i32>::new(nullInst);
      if self.0.0 != nullInst {
         unsafe {
            let members: *const dggal_sys::class_members_DGGSJSON = ((self.0.0 as *const i8).wrapping_add((*dggal_sys::class_DGGSJSON).offset as usize)) as *const dggal_sys::class_members_DGGSJSON;
            depths.array = (*members).depths;
         }
      }
      depths
   }

   pub fn values(&self) -> Map<ecrt::String, ArrayOfDGGSJSONDepth>
   {
      let mut values = Map::<ecrt::String, ArrayOfDGGSJSONDepth>::new(nullInst);
      if self.0.0 != nullInst {
         unsafe {
            let members: *const dggal_sys::class_members_DGGSJSON = ((self.0.0 as *const i8).wrapping_add((*dggal_sys::class_DGGSJSON).offset as usize)) as *const dggal_sys::class_members_DGGSJSON;
            values.map = (*members).values;
         }
      }
      values
   }
}

pub type ArrayOfDGGSJSONDepth = Instance;

pub fn readDGGSJSON(f: &File) -> Result<DGGSJSON, &str>
{
   let mut result: Result<DGGSJSON, &str> = Err("Failure to load DGGS-JSON");
   unsafe
   {
      if f.file != nullInst {
         let r: dggal_sys::DGGSJSON = dggal_sys::fnptr_readDGGSJSON.unwrap()(f.file);
         if r != nullInst {
            result = Ok(DGGSJSON(Instance(r)))
         }
      }
   }
   result
}
