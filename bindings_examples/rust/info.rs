// *********** DGGAL rust bindings *********************
use std::env;
use std::ffi::CString;
use std::ffi::CStr;
use std::ffi::c_void;
use std::f64::consts::PI;
use std::slice;

mod dggal;

// *** This code should be moved to / generated inside the DGGAL rust bindings
const nullZone : dggal::DGGRSZone = 0xFFFFFFFFFFFFFFFFu64;
const nullCStr : * const i8 = 0 as * const i8;
const nullInst : dggal::Instance = 0 as dggal::Instance;
const nullPtr : *mut *mut c_void = 0 as *mut *mut c_void;

// TODO: Could we use rust function generating macros?

// DGGRS::getZoneFromTextID()
fn DGGRS_getZoneFromTextID(dggrs: dggal::DGGRS, zoneID: * const i8) -> dggal::DGGRSZone
{
   let mut zone = nullZone;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zoneID: * const i8) -> dggal::DGGRSZone =
         std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneFromTextID_vTblID as usize));
      if method != std::mem::transmute(0usize) {
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
      let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> i32 =
         std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneLevel_vTblID as usize));
      if method != std::mem::transmute(0usize) {
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
      let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> i32 =
         std::mem::transmute(*vTbl.add(dggal::DGGRS_countZoneEdges_vTblID as usize));
      if method != std::mem::transmute(0usize) {
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
      let method : unsafe extern "C" fn(dggrs: dggal::DGGRS) -> i32 =
         std::mem::transmute(*vTbl.add(dggal::DGGRS_getRefinementRatio_vTblID as usize));
      if method != std::mem::transmute(0usize) {
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
      let method : unsafe extern "C" fn(dggrs: dggal::DGGRS) -> i32 =
         std::mem::transmute(*vTbl.add(dggal::DGGRS_getMaxDGGRSZoneLevel_vTblID as usize));
      if method != std::mem::transmute(0usize) {
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
      let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, centroid: *mut dggal::GeoPoint) =
         std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneWGS84Centroid_vTblID as usize));
      if method != std::mem::transmute(0usize) {
         method(dggrs, zone, std::ptr::from_mut(&mut centroid));
      }
   }
   centroid
}

// DGGRS::getZoneWGS84Vertices()
fn DGGRS_getZoneWGS84Vertices(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone) -> Vec<dggal::GeoPoint>
{
   let mut n: i32 = 0;
   let mut vertices: Vec<dggal::GeoPoint>;
   unsafe
   {
      let c = dggal::class_DGGRS;
      let vTbl = if dggrs != nullInst && (*dggrs)._vTbl != nullPtr { (*dggrs)._vTbl } else { (*c)._vTbl };
      let mut v: [dggal::GeoPoint; 6] = [dggal::GeoPoint { lat: 0.0, lon: 0.0 }; 6]; // REVIEW: Anyway to avoid this initialization?
      let method : unsafe extern "C" fn(dggrs: dggal::DGGRS, zone: dggal::DGGRSZone, vertices: *mut dggal::GeoPoint) -> i32 =
         std::mem::transmute(*vTbl.add(dggal::DGGRS_getZoneWGS84Vertices_vTblID as usize));
      if method != std::mem::transmute(0usize) {
         n = method(dggrs, zone, std::ptr::from_mut(&mut v[0]));
      }
      vertices = slice::from_raw_parts(&v[0], n as usize).to_vec();
   }
   vertices
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
   let extent: dggal::GeoExtent;
   let vertices: Vec<dggal::GeoPoint> = DGGRS_getZoneWGS84Vertices(dggrs, zone);
   let mut zoneID: [i8; 256];
   /*
   let area: f64 = DGGRS_getZoneArea(dggrs, zone);
   let areaKM2: f64 = area / 1000000.0;
   let depth: i32 = DGGRS_get64KDepth(dggrs);
   */
   let mut parents: [dggal::DGGRSZone; 3];
   let mut neighbors: [dggal::DGGRSZone; 6];
   let mut children: [dggal::DGGRSZone; 9];
   /*
   let nParents: i32 = DGGRS_getZoneParents(dggrs, zone, parents);
   let nbTypes: [i32; 6];
   let nNeighbors: i32 = DGGRS_getZoneNeighbors(dggrs, zone, neighbors, nbTypes);
   let nChildren: i32 = DGGRS_getZoneChildren(dggrs, zone, children);
   let centroidParent: dggal::DGGRSZone = DGGRS_getZoneCentroidParent(dggrs, zone);
   let centroidChild: dggal::DGGRSZone = DGGRS_getZoneCentroidChild(dggrs, zone);
   */
   let isCentroidChild = false; //DGGRS_isZoneCentroidChild(dggrs, zone);
   let i: i32;
   let crs = (c"EPSG:4326").as_ptr();
   let nSubZones: i64;

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

   //nSubZones = DGGRS_countSubZones(dggrs, zone, depth);

   /*
   DGGRS_getZoneWGS84Extent(dggrs, zone, &extent);
   DGGRS_getZoneTextID(dggrs, zone, zoneID);

   printLn(CO(CString), $("Textual Zone ID: "), CO(CString), zoneID, ptr::null());
   printx(CO(CString), $("64-bit integer ID: "), CO(uint64), &zone, CO(CString), " (", ptr::null());

   printf(FORMAT64HEX, zone);
   printLn(CO(CString), ")", ptr::null());
   printLn(CO(CString), "", ptr::null());
   */
   let sCChild = if isCentroidChild { ", centroid child" } else { "" };
   println!("Level {level} zone ({nEdges} edges{sCChild})");


   // printLn(CO(double), &area, CO(CString), " m² (", CO(double), &areaKM2, CO(CString), " km²)", ptr::null());
   // printLn(CO(int64), &nSubZones, CO(CString), $(" sub-zones at depth "), CO(i32), &depth, ptr::null());

   let cLat = centroid.lat * 180.0 / PI;
   let cLon = centroid.lon * 180.0 / PI;
   println!("WGS84 Centroid (lat, lon): {cLat}, {cLon}");
   /*
   printLn(
      CO(CString), $("WGS84 Extent (lat, lon): { "),
      CO(Degrees), &extent.ll.lat, CO(CString), ", ",
      CO(Degrees), &extent.ll.lon, CO(CString), " }, { ",
      CO(Degrees), &extent.ur.lat, CO(CString), ", ",
      CO(Degrees), &extent.ur.lon, CO(CString), " }", ptr::null());

   printLn(CO(CString), "", ptr::null());
   if(nParents)
   {
      printLn(CO(CString), $("Parent"), CO(CString), nParents > 1 ? "s" : "", CO(CString), " (", CO(i32), &nParents, CO(CString), "):", ptr::null());
      for(i = 0; i < nParents; i+=1)
      {
         char pID[256];
         DGGRS_getZoneTextID(dggrs, parents[i], pID);
         printx(CO(CString), "   ", CO(CString), pID, ptr::null());
         if(centroidParent == parents[i])
            printx(CO(CString), $(" (centroid child)"), ptr::null());
         printLn(CO(CString), "", ptr::null());
      }
   }
   else
      printLn(CO(CString), $("No parent"), ptr::null());

   printLn(CO(CString), "", ptr::null());
   printLn(CO(CString), $("Children ("), CO(i32), &nChildren, CO(CString), "):", ptr::null());
   for(i = 0; i < nChildren; i+=1)
   {
      char cID[256];
      DGGRS_getZoneTextID(dggrs, children[i], cID);
      printx(CO(CString), "   ", CO(CString), cID, ptr::null());
      if(centroidChild == children[i])
         printx(CO(CString), $(" (centroid)"), ptr::null());
      printLn(CO(CString), "", ptr::null());
   }

   printLn(CO(CString), "", ptr::null());
   printLn(CO(CString), $("Neighbors ("), CO(i32), &nNeighbors, CO(CString), "):", ptr::null());
   for(i = 0; i < nNeighbors; i+=1)
   {
      char nID[256];
      DGGRS_getZoneTextID(dggrs, neighbors[i], nID);
      printLn(CO(CString), $("   (direction "), CO(i32), &nbTypes[i], CO(CString), "): ", CO(CString), nID, ptr::null());
   }
*/
   println!("");
   let sCRS = unsafe { CStr::from_ptr(crs).to_str().unwrap() };
   let nVertices = vertices.len();
   println!("[{sCRS} Vertices ({nVertices}):");

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
      let arg0 : *const i8 = CString::new(args[0].clone()).unwrap().as_ptr();
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
            let value = CString::new(args[a].clone()).unwrap().as_ptr();
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
