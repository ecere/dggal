extern crate ecrt;

use ecrt::Application;
use ecrt::tokenizeWith;

extern crate dggal;

use dggal::DGGAL;
use dggal::DGGRS;
// use dggal::DGGRSZone;
//use dggal::nullZone;
use dggal::GeoExtent;
use dggal::GeoPoint;
use dggal::wholeWorld;

use std::collections::HashMap;
use std::env;
use std::process::exit;
use std::f64::consts::PI;

fn parse_bbox(options: &HashMap<&str, &str>, bbox: &mut GeoExtent) -> bool
{
   let mut result = true;
   let bbox_option = options.get(&"bbox");
   if bbox_option != None {
      let s = bbox_option.unwrap();
      // NOTE: tokenizeWith() will eventually be moved to ecrt crate
      let tokens: Vec<String> = tokenizeWith::<4>(s, ",", false);
      result = false;
      if tokens.len() == 4 {
         let a = tokens[0].parse::<f64>();
         let b = tokens[1].parse::<f64>();
         let c = tokens[2].parse::<f64>();
         let d = tokens[3].parse::<f64>();
         if a.is_ok() && b.is_ok() && c.is_ok() && d.is_ok() {
            let af = a.unwrap();
            let bf = b.unwrap();
            let cf = c.unwrap();
            let df = d.unwrap();
            if af < 90.0 && af > -90.0 {
               bbox.ll = GeoPoint { lat: af * PI / 180.0, lon: bf * PI / 180.0 };
               bbox.ur = GeoPoint { lat: cf * PI / 180.0, lon: df * PI / 180.0 };
               result = true;
            }
            else {
               result = false;
            }
         }
         else {
            result = false;
         }
      }
      if result == false {
         println!("Invalid bounding box specified");
      }
   }
   return result;
}

fn list_zones(dggrs: DGGRS, mut level: i32, options: &HashMap<&str, &str>) -> i32
{
   let mut exit_code = 0;
   let centroids = options.get(&"centroids");
   let compact = options.get(&"compact");
   let mut bbox = wholeWorld;

   if !parse_bbox(options, &mut bbox) {
      exit_code = 1
   }

   if compact != None && centroids != None {
      exit_code = 1;
      println!("Cannot return compact list of zones as centroids")
   }

   if level == -1 {
      level = 0;
   }

   if exit_code == 0 {
      let mut i = 0;
      let mut zones = dggrs.listZones(level, &bbox);

      if compact != None {
         dggrs.compactZones(&mut zones);
      }

      print!("[");
      for z in zones {
         print!("{}", if i > 0 { ", " } else { " " });
         if centroids != None {
            let centroid = dggrs.getZoneWGS84Centroid(z);
            print!("[ {}, {} ]", centroid.lat * 180.0 / PI, centroid.lon * 180.0 / PI);
         }
         else {
            let zone_id = dggrs.getZoneTextID(z);
            print!("\"{zone_id}\"");
         }
         i += 1;
      }
      println!(" ]");
   }
   return exit_code;
}

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
   let mut level: i32 = -1;
   let arg0: &str = &args[0];
   let mut options = HashMap::<&str, &str>::new();

        if arg0.eq_ignore_ascii_case("i3h") { dggrs_name = &"ISEA3H"; }
   else if arg0.eq_ignore_ascii_case("v3h") { dggrs_name = &"IVEA3H"; }
   else if arg0.eq_ignore_ascii_case("r3h") { dggrs_name = &"RTEA3H"; }
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
      else if arg1.eq_ignore_ascii_case("rHEALPix") { dggrs_name = &"rHEALPix"; }
      else if arg1.eq_ignore_ascii_case("gnosis") { dggrs_name = &"GNOSISGlobalGrid"; }
      a += 1;
   }

   if argc > a {
      level = args[a].parse::<i32>().unwrap();
      a+=1;
   }

   while a < argc {
      let key = &args[a];
      a+=1;
      if key.as_bytes()[0] == '-' as u8 {
         let k = &key[1..];
         if k == "bbox" {
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

      println!("DGGRS: https://maps.gnosis.earth/ogcapi/dggrs/{dggrs_name}");

      if exit_code == 0 {
         list_zones(dggrs, level, &options);
      }
   }
   else {
      show_syntax = true;
      exit_code = 1;
   }

   if show_syntax {
      println!("Syntax:");
      println!("   list <dggrs> [level [options]]");
      println!("where dggrs is one of gnosis, isea3h, isea9r, ivea3h, ivea9r, rtea3h, rtea9r or rHEALPix");
   }

   exit(exit_code)
}
