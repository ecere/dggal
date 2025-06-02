use std::collections::HashMap;
use std::env;
use std::process::exit;

extern crate ecrt;

use ecrt::Application;
use ecrt::FieldValue;
use ecrt::File;
use ecrt::FileOpenMode;
use ecrt::getLastDirectory;
use ecrt::MapIterator;
use ecrt::Array;
use ecrt::nullInst;

extern crate dggal;

use dggal::DGGAL;
use dggal::DGGRS;
use dggal::DGGRSZone;
use dggal::nullZone;
use dggal::CRS;
use dggal::DGGSJSON;
use dggal::DGGSJSONDepth;
use dggal::ArrayOfDGGSJSONDepth;
use dggal::readDGGSJSON;

mod geom;

use geom::resolve_crs_string;
use geom::generate_zone_feature;

fn convert_to_geojson(dggal: &DGGAL, input_file: &str, options: &HashMap::<&str, &str>) -> i32
{
   let mut exit_code = 1;
   let fr = File::open(input_file, FileOpenMode::Read);
   if fr.is_ok() {
      let f = fr.unwrap();
      let dggs_json_option = readDGGSJSON(&f);
      if dggs_json_option.is_ok() {
         let dggs_json: DGGSJSON = dggs_json_option.unwrap();

         let dggrs_uri = dggs_json.dggrs().string();

         let mut dggrs_id = getLastDirectory(&dggrs_uri);

         dggrs_id.truncate(dggrs_id.len().max(1) - 1);

         let dggrs_result = DGGRS::new(&dggal, &dggrs_id);

         if dggrs_result.is_ok() {
            let dggrs = dggrs_result.unwrap();
            let zone_id = dggs_json.zoneId().string();
            let zone = dggrs.getZoneFromTextID(&zone_id);

            if zone != nullZone {
               let depths = dggs_json.depths();
               if depths.array != nullInst {
                  let mut max_depth = -1;
                  let mut d_index: i32 = -1;

                  for d in 0..depths.count() {
                     // NOTE: Unfortunately we can't implement the Index trait which would support [ ]
                     //       due to internal representations not always matching type of array and Index trait
                     //       requiring references (not an issue in Python with __setitem__ and __getitem__)
                     let depth = depths.getElement(d);
                     if depth > max_depth {
                        max_depth = depth;
                        d_index = d as i32;
                        break;
                     }
                  }
                  if max_depth != -1 {
                     let depth = max_depth;
                     let sub_zones: Vec<DGGRSZone> = dggrs.getSubZones(zone, depth);
                     let centroids = options.get("centroids");
                     let crs: CRS = resolve_crs_string(options.get("crs"));

                     if sub_zones.len() != 0 {
                        let mut i: i32 = 0;

                        println!("{{");
                        println!("   \"type\": \"FeatureCollection\",");
                        print! ("   \"features\": [ ");

                        for z in sub_zones {
                           let mut props = HashMap::<String, FieldValue>::new();

                           print!("{}", if i != 0 { ", " } else { "   " });

                           let mut it = MapIterator!(<ecrt::String, ArrayOfDGGSJSONDepth> dggs_json.values());

                           while it.next() {
                              let key: ecrt::String = it.key();
                              let v_depths = Array::<DGGSJSONDepth>::new(*it.value());
                              if !key.is_null() && v_depths.array != nullInst && v_depths.count() > d_index {
                                 let dj_depth = v_depths.getElement(d_index);
                                 let data: Array<FieldValue> = dj_depth.data();
                                 let v: FieldValue = data.getElement(i);
                                 props.insert(key.string(), v);
                              }
                           }
                           generate_zone_feature(&dggrs, z, crs, (i + 1) as u64, centroids.is_some(), true, Some(&props));
                           i += 1;
                        }
                        println!(" ]");
                        println!("}}");

                        exit_code = 0;
                     }
                  }
               }
            } else {
               println!("Invalid zone ID: {zone_id}")
            }
         } else {
            println!("Failure to recognize DGGRS {dggrs_id}")
         }
      } else {
         println!("Failure to parse DGGS-JSON file {input_file}")
      }
   } else {
      println!("Failure to open file {input_file}")
   }
   exit_code
}

fn main()
{
   let args: Vec<String> = env::args().collect();
   let argc = args.len();
   let my_app = Application::new(&args);
   let dggal = DGGAL::new(&my_app);
   let mut exit_code: i32 = 0;
   let mut show_syntax = false;
   let mut a : usize = 1;
   let mut options = HashMap::<&str, &str>::new();
   let mut input: &str = "";

   if argc > a {
      input = &args[a];
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

   if exit_code == 0 {
      if input == "" {
         println!("Missing input DGGS-JSON file");
         exit_code = 1;
      }

      if exit_code == 0 {
         convert_to_geojson(&dggal, input, &options);
      }
   }
   else {
      show_syntax = true;
      exit_code = 1;
   }

   if show_syntax {
      println!("Syntax:");
      println!("   togeo <DGGS-JSON file> [options]");
   }

   exit(exit_code)
}
