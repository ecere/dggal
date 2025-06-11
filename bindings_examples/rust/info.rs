extern crate ecrt;

#[cfg(feature = "info_cmd")]
use ecrt::Application;

extern crate dggal;

#[cfg(feature = "info_cmd")]
use dggal::DGGAL;

#[cfg(feature = "info_cmd")]
use std::env;

#[cfg(feature = "info_cmd")]
use std::process::exit;

use dggal::DGGRS;
use dggal::DGGRSZone;
use dggal::GeoExtent;
use dggal::GeoPoint;
use dggal::nullZone;

use std::collections::HashMap;
use std::f64::consts::PI;

pub fn zone_info(dggrs: DGGRS, zone: DGGRSZone, options: HashMap<&str, &str>) -> i32
{
   let level = dggrs.getZoneLevel(zone);
   let n_edges = dggrs.countZoneEdges(zone);
   let centroid = dggrs.getZoneWGS84Centroid(zone);
   let extent: GeoExtent = dggrs.getZoneWGS84Extent(zone);
   let vertices: Vec<GeoPoint> = dggrs.getZoneWGS84Vertices(zone);
   let zone_id: String = dggrs.getZoneTextID(zone);
   let area: f64 = dggrs.getZoneArea(zone);
   let area_km2: f64 = area / 1000000.0;
   let mut depth: i32 = dggrs.get64KDepth();
   let parents = dggrs.getZoneParents(zone);
   let mut nb_types: [i32; 6] = [0; 6];
   let neighbors = dggrs.getZoneNeighbors(zone, &mut nb_types);
   let children = dggrs.getZoneChildren(zone);
   let centroid_parent: DGGRSZone = dggrs.getZoneCentroidParent(zone);
   let centroid_child: DGGRSZone = dggrs.getZoneCentroidChild(zone);
   let is_centroid_child = dggrs.isZoneCentroidChild(zone);
   let crs = &"EPSG:4326";
   let depth_option = options.get(&"depth");

   if depth_option != None
   {
      let max_depth: i32 = dggrs.getMaxDepth();
      depth = depth_option.unwrap().parse::<i32>().unwrap();
      if depth > max_depth
      {
         println!("Invalid depth (maximum: {max_depth})");
         return 1;
      }
   }

   let n_sub_zones: u64 = dggrs.countSubZones(zone, depth);

   println!("Textual Zone ID: {zone_id}");
   println!("64-bit integer ID: {zone} (0x{zone:X})");
   println!("");
   println!("Level {level} zone ({n_edges} edges{0})", if is_centroid_child { ", centroid child" } else { "" });
   println!("{area} m² ({area_km2} km²)");
   println!("{n_sub_zones} sub-zones at depth {depth}");

   println!("WGS84 Centroid (lat, lon): {0}, {1}",
      centroid.lat * 180.0 / PI, centroid.lon * 180.0 / PI);
   println!("WGS84 Extent (lat, lon): {{ {0}, {1} }}, {{ {2}, {3} }}",
      extent.ll.lat * 180.0 / PI, extent.ll.lon * 180.0 / PI,
      extent.ur.lat * 180.0 / PI, extent.ur.lon * 180.0 / PI);
   println!("");

   let n_parents = parents.len();
   if n_parents != 0
   {
      println!("Parent{0} ({n_parents}):", if n_parents > 1 { "s" } else { "" });
      for p in parents
      {
         let p_id = dggrs.getZoneTextID(p);
         print!("   {p_id}");
         if centroid_parent == p {
            print!(" (centroid child)");
         }
         println!("");
      }
   }
   else {
      println!("No parent");
   }
   println!("");

   println!("Children ({0}):", children.len());
   for c in children
   {
      let c_id = dggrs.getZoneTextID(c);
      print!("   {c_id}");
      if centroid_child == c {
         print!(" (centroid)");
      }
      println!("");
   }
   println!("");

   println!("Neighbors ({0}):", neighbors.len());

   let mut i: usize = 0;
   for n in neighbors
   {
      let n_id = dggrs.getZoneTextID(n);
      let nt = nb_types[i];
      println!("   (direction {nt}): {n_id}");
      i += 1;
   }
   println!("");
   println!("[{crs}] Vertices ({0}):", vertices.len());

   for v in vertices
   {
      let lat = v.lat * 180.0 / PI;
      let lon = v.lon * 180.0 / PI;
      println!("   {lat}, {lon}");
   }

   0 // No error
}

pub fn dggrs_info(dggrs: DGGRS, _options: HashMap<&str, &str>) -> i32
{
   let depth64k = dggrs.get64KDepth();
   let ratio = dggrs.getRefinementRatio();
   let max_level = dggrs.getMaxDGGRSZoneLevel();

   println!("Refinement Ratio: {ratio}");
   println!("Maximum level for 64-bit global identifiers (DGGAL DGGRSZone): {max_level}");
   println!("Default ~64K sub-zones relative depth: {depth64k}");
   0 // No error
}

pub fn display_info(dggrs: DGGRS, zone: DGGRSZone, options: HashMap<&str, &str>) -> i32
{
   if zone != nullZone {
      zone_info(dggrs, zone, options)
   } else {
      dggrs_info(dggrs, options)
   }
}

#[cfg(feature = "info_cmd")]
fn main()
{
   let args: Vec<String> = env::args().collect();
   let argc = args.len();
   let my_app = Application::new(&args);
   let dggal = DGGAL::new(&my_app);
   let mut exit_code: i32 = 0;
   let mut show_syntax = false;
   let mut dggrs_name = "";
   let mut a : usize = 1;
   let mut zone_id: &str = "";
   let arg0: &str = &args[0];
   let mut options = HashMap::<&str, &str>::new();

        if arg0.eq_ignore_ascii_case("i3h") { dggrs_name = &"ISEA3H"; }
   else if arg0.eq_ignore_ascii_case("v3h") { dggrs_name = &"IVEA3H"; }
   else if arg0.eq_ignore_ascii_case("r3h") { dggrs_name = &"RTSEA3H"; }
   else if arg0.eq_ignore_ascii_case("i9r") { dggrs_name = &"ISEA9R"; }
   else if arg0.eq_ignore_ascii_case("v9r") { dggrs_name = &"IVEA9R"; }
   else if arg0.eq_ignore_ascii_case("r9r") { dggrs_name = &"RTEA9R"; }
   else if arg0.eq_ignore_ascii_case("rhp") { dggrs_name = &"rHEALPix"; }
   else if arg0.eq_ignore_ascii_case("ggg") { dggrs_name = &"GNOSISGlobalGrid"; }

   if dggrs_name == "" && argc > 1
   {
      let arg1: &str = &args[1];
           if arg1.eq_ignore_ascii_case("isea3h") { dggrs_name = &"ISEA3H"; }
      else if arg1.eq_ignore_ascii_case("isea9r") { dggrs_name = &"ISEA9R"; }
      else if arg1.eq_ignore_ascii_case("ivea3h") { dggrs_name = &"IVEA3H"; }
      else if arg1.eq_ignore_ascii_case("ivea9r") { dggrs_name = &"IVEA9R"; }
      else if arg1.eq_ignore_ascii_case("rtea3h") { dggrs_name = &"RTEA3H"; }
      else if arg1.eq_ignore_ascii_case("rtea9r") { dggrs_name = &"RTVEA9R"; }
      else if arg1.eq_ignore_ascii_case("rHEALPix") { dggrs_name = &"rHEALPix"; }
      else if arg1.eq_ignore_ascii_case("gnosis") { dggrs_name = &"GNOSISGlobalGrid"; }
      a += 1;
   }

   if argc > a {
      zone_id = &args[a];
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
         exit_code = 1;
         show_syntax = true;
      }
   }

   if dggrs_name != "" && exit_code == 0
   {
      let dggrs: DGGRS = DGGRS::new(&dggal, dggrs_name).expect("Unknown DGGRS");
      let mut zone = nullZone;

      println!("DGGRS: https://maps.gnosis.earth/ogcapi/dggrs/{dggrs_name}");

      if zone_id != "" {
         zone = dggrs.getZoneFromTextID(zone_id);
         if zone == nullZone
         {
            println!("Invalid {dggrs_name} zone identifier: {zone_id}");
            exit_code = 1;
         }
      }

      if exit_code == 0 {
         display_info(dggrs, zone, options);
      }
   }
   else
   {
      show_syntax = true;
      exit_code = 1;
   }

   if show_syntax {
      println!("Syntax:");
      println!("   info <dggrs> [zone] [options]");
      println!("where dggrs is one of gnosis, isea3h, isea9r, ivea3h, ivea9r, rtea3h, rtea9r or rHEALPix");
   }

   exit(exit_code)
}
