use std::collections::HashMap;
use std::env;
use std::process::exit;

extern crate ecrt;

use ecrt::Application;
use ecrt::Pointd;
use ecrt::tokenizeWith;

extern crate dggal;

use dggal::DGGAL;
use dggal::DGGRS;
use dggal::DGGRSZone;
use dggal::nullZone;
use dggal::CRS;
use dggal::epsg;

mod info;

use info::display_info;

fn query_zone(dggrs: DGGRS, coordinates: &str, mut level: i32, options: HashMap<&str, &str>) -> i32
{
   let mut exit_code: i32 = 1;
   let coords: Vec<String> = tokenizeWith::<2>(coordinates, ",", false);
   let n = coords.len();
   if n == 2 {
      let lat = coords[0].parse::<f64>();
      let lon = coords[1].parse::<f64>();
      if lat.is_ok() && lon.is_ok() {
         let f_lat = lat.unwrap();
         let f_lon = lon.unwrap();
         if f_lat < 90.0 && f_lat > -90.0 {
            if level == -1 {
               level = 0;
            }
            let zone: DGGRSZone = dggrs.getZoneFromCRSCentroid(level,
               CRS!(epsg, 4326), &Pointd { x: f_lat, y: f_lon });
            if zone != nullZone {
               display_info(dggrs, zone, options);
               exit_code = 0;
            }
            else {
               println!("Could not identify zone from coordinates");
            }
         }
      }
   }

   if exit_code != 0 {
      if coordinates != "" {
         println!("Invalid coordinates for zone query");
      } else {
         println!("Missing coordinates for zone query");
      }
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
   let arg0: &str = &args[0];
   let mut options = HashMap::<&str, &str>::new();
   let mut coordinates: &str = "";
   let mut level: i32 = -1;

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
      coordinates = &args[a];
      a+=1;
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
         if k == "depth" {
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
         exit_code = query_zone(dggrs, coordinates, level, options);
      }
   }
   else {
      show_syntax = true;
      exit_code = 1;
   }

   if show_syntax {
      println!("Syntax:");
      println!("   zone <dggrs> <lat,lon> <level> [options]");
      println!("where dggrs is one of gnosis, isea3h, isea9r, ivea3h, ivea9r, rtea3h, rtea9r or rHEALPix");
   }

   exit(exit_code)
}
