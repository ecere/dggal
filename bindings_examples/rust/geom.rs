#[cfg(feature = "geom_cmd")]
use std::env;

#[cfg(feature = "geom_cmd")]
use std::process::exit;

use std::collections::HashMap;
use std::f64::consts::PI;
use std::ffi::CStr;

extern crate ecrt;

#[cfg(feature = "geom_cmd")]
use ecrt::Application;

use ecrt::FieldValue;
use ecrt::FieldTypeEx;
use ecrt::FieldType;

extern crate dggal;

#[cfg(feature = "geom_cmd")]
use dggal::DGGAL;

#[cfg(feature = "geom_cmd")]
use dggal::nullZone;

use dggal::DGGRS;
use dggal::DGGRSZone;

use dggal::CRS;
use dggal::ogc;
use dggal::epsg;

pub fn resolve_crs_string(crs_option: Option<&&str>) -> CRS
{
   let mut crs = CRS(0);
   if crs_option.is_some() {
      let str = crs_option.unwrap();
      // NOTE: Currently re-using the same CRS identifiers regardless of actual projection for 5x6 and icosahedron net space
           if str.eq_ignore_ascii_case("5x6" )       { crs = CRS!(ogc, 153456); }
      else if str.eq_ignore_ascii_case("ico") ||
              str.eq_ignore_ascii_case("isea") ||
              str.eq_ignore_ascii_case("ivea") ||
              str.eq_ignore_ascii_case("rtea")       { crs = CRS!(ogc, 1534); }
      else if str.eq_ignore_ascii_case("OGC:CRS84")  { crs = CRS!(ogc, 84); }
      else if str.eq_ignore_ascii_case("EPSG:4326")  { crs = CRS!(epsg, 4326); }
      else if str.eq_ignore_ascii_case("rhp")        { crs = CRS!(ogc, 99999); }
   }
   return crs;
}

pub fn generate_zone_feature(dggrs: &DGGRS, zone : DGGRSZone, crs: CRS, id: u64, centroids: bool, fc: bool, properties: Option<&HashMap<String, FieldValue>>)
{
   let t = if fc { "   " } else { "" };
   let zone_id = dggrs.getZoneTextID(zone);

   println!("{{");
   println!("{t}   \"type\" : \"Feature\",");
   print!("{t}   \"id\" : ");
   if id != 0 {
      print!("{id}");
   }
   else {
      print!("\"{zone_id}\"");
   }
   println!(",");

   generate_zone_geometry(dggrs, zone, crs, centroids, fc);

   println!(",");

   println!("{t}   \"properties\" : {{");
   print!("{t}     \"zone_id\" : \"");
   print!("{zone_id}");
   print!("\"");
   if properties.is_some() {
      for (key, v) in properties.unwrap() {
         print!(",\n{t}     \"{key}\" : ");
         unsafe
         {
            match FieldTypeEx(v.type_).type_() {
               FieldType::Real    => print!("{}", v.__bindgen_anon_1.r),
               FieldType::Text    => print!("{}", CStr::from_ptr(v.__bindgen_anon_1.s).to_str().unwrap()),
               FieldType::Integer => print!("{}", v.__bindgen_anon_1.i),
               _ => todo!()
            }
         }
      }
   }

   println!("");
   println!("{t}   }}");
   print!("{t}}}");
}

pub fn generate_zone_geometry(dggrs: &DGGRS, zone: DGGRSZone, crs: CRS, centroids: bool, fc: bool)
{
   let t = if fc { "   " } else { "" };

   println!("{t}   \"geometry\" : {{");
   println!("{t}      \"type\" : \"{}\",", if centroids { "Point" } else { "Polygon" });
   print!("{t}      \"coordinates\" : [");

   if crs == 0 || crs == CRS!(ogc, 84) || crs == CRS!(epsg, 4326) {
      if centroids {
         let centroid = dggrs.getZoneWGS84Centroid(zone);
         print!(" {}, {}", centroid.lon * 180.0 / PI, centroid.lat * 180.0 / PI);
      }
      else {
         let vertices = dggrs.getZoneRefinedWGS84Vertices(zone, 0);
         let count = vertices.len();
         if count != 0 {
            println!("");
            print!("{t}         [ ");
            for i in 0..count {
               print!("{}[{}, {}]", if i != 0 { ", " } else { "" }, vertices[i].lon * 180.0 / PI, vertices[i].lat * 180.0 / PI);
            }
            print!("{}[{}, {}]", if count != 0  { ", " } else { "" }, vertices[0].lon * 180.0 / PI, vertices[0].lat * 180.0 / PI);
            println!(" ]");
         }
         print!("{t}     ");
      }
   }
   else {
      if centroids {
         let centroid = dggrs.getZoneCRSCentroid(zone, crs);
         print!(" {}, {}", centroid.x, centroid.y);
      }
      else {
         let vertices = dggrs.getZoneRefinedCRSVertices(zone, crs, 0);
         let count = vertices.len();
         if count != 0 {

            println!("");
            println!("{t}         [ ");

            for i in 0..count {
               print!("{}[{}, {}]", if i != 0  { ", " } else { "" }, vertices[i].x, vertices[i].y);
            }
            print!("{}[{}, {}]", if count != 0  { ", " } else { "" }, vertices[0].x, vertices[0].y);
            println!(" ]");
         }
         print!("{t}     ");
      }
   }
   println!(" ]");
   print!("{t}   }}");
}

#[cfg(feature = "geom_cmd")]
pub fn generate_geometry(dggrs: &DGGRS, zone: DGGRSZone, options: &HashMap::<&str, &str>) -> i32
{
   if zone != nullZone {
      let centroids = options.get("centroids");
      let crs = resolve_crs_string(options.get("crs"));
      generate_zone_feature(dggrs, zone, crs, 0, centroids.is_some(), false, None);
      println!("");
      return 0;
   }
   else {
      println!("geom command requires a zone");
   }
   return 1;
}

#[cfg(feature = "geom_cmd")]
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
   let arg0: &str = &args[0];
   let mut options = HashMap::<&str, &str>::new();
   let mut zone_id: &str = "";
   let mut zone: DGGRSZone = nullZone;

        if arg0.eq_ignore_ascii_case("i3h") { dggrs_name = &"ISEA3H"; }
   else if arg0.eq_ignore_ascii_case("v3h") { dggrs_name = &"IVEA3H"; }
   else if arg0.eq_ignore_ascii_case("r3h") { dggrs_name = &"RTSEA3H"; }
   else if arg0.eq_ignore_ascii_case("i9r") { dggrs_name = &"ISEA9R"; }
   else if arg0.eq_ignore_ascii_case("v9r") { dggrs_name = &"IVEA9R"; }
   else if arg0.eq_ignore_ascii_case("r9r") { dggrs_name = &"RTEA9R"; }
   else if arg0.eq_ignore_ascii_case("rhp") { dggrs_name = &"rHEALPix"; }
   else if arg0.eq_ignore_ascii_case("ggg") { dggrs_name = &"GNOSISGlobalGrid"; }

   if dggrs_name == "" && argc > 1 {
      let arg1: &str = &args[1];
           if arg1.eq_ignore_ascii_case("isea3h") { dggrs_name = &"ISEA3H"; }
      else if arg1.eq_ignore_ascii_case("isea9r") { dggrs_name = &"ISEA9R"; }
      else if arg1.eq_ignore_ascii_case("ivea3h") { dggrs_name = &"IVEA3H"; }
      else if arg1.eq_ignore_ascii_case("ivea9r") { dggrs_name = &"IVEA9R"; }
      else if arg1.eq_ignore_ascii_case("rtea3h") { dggrs_name = &"RTEA3H"; }
      else if arg1.eq_ignore_ascii_case("rtea9r") { dggrs_name = &"RTVEA9R"; }
      else if arg1.eq_ignore_ascii_case("rHEALPIx") { dggrs_name = &"rHEALPix"; }
      else if arg1.eq_ignore_ascii_case("gnosis") { dggrs_name = &"GNOSISGlobalGrid"; }
      a += 1;
   }

   if argc > a {
      zone_id = &args[a];
      a+=1;
   }

   while a < argc {
      let key = &args[a];
      a+=1;
      if key.as_bytes()[0] == '-' as u8 {
         let k = &key[1..];
         if k == "crs" {
            if a < argc {
               options.insert(k, &args[a]);
               a+=1;
            }
            else {
               exit_code = 1;
               show_syntax = true;
            }
         }
         else {
            options.insert(k, "");
         }
      }
      else
      {
         exit_code = 1;
         show_syntax = true;
      }
   }

   if dggrs_name != "" && exit_code == 0 {
      let dggrs: DGGRS = DGGRS::new(&dggal, dggrs_name).expect("Unknown DGGRS");

      // println!("DGGRS: https://maps.gnosis.earth/ogcapi/dggrs/{dggrs_name}");

      if zone_id != "" {
         zone = dggrs.getZoneFromTextID(zone_id);
         if zone == nullZone
         {
            println!("Invalid {dggrs_name} zone identifier: {zone_id}");
            exit_code = 1;
         }
      }

      if exit_code == 0 {
         generate_geometry(&dggrs, zone, &options);
      }
   }
   else {
      show_syntax = true;
      exit_code = 1;
   }

   if show_syntax {
      println!("Syntax:");
      println!("   geom <dggrs> <zone> [options]");
      println!("where dggrs is one of gnosis, isea3h, isea9r, ivea3h, ivea9r, rtea3h, rtea9r or rHEALPix");
   }

   exit(exit_code)
}
