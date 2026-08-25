import sys
import json
import wkbc

def run_wkbc_test():
   # Ensure the user provided both the input and output GeoJSON paths as arguments
   if len(sys.argv) < 3:
      print("Usage: python test_wkbc.py <input.geojson> <output.geojson>")
      return

   input_path = sys.argv[1]
   output_geojson_path = sys.argv[2]
   temp_wkbc_path = output_geojson_path + ".wkbc"

   # 1. Read input GeoJSON file
   with open(input_path, "r", encoding="utf-8") as f:
      input_fc = json.load(f)

   # Ensure features have IDs populated for compliance with the binary lookup table index
   for idx, feat in enumerate(input_fc.get("features", []) or []):
      if "id" not in feat or feat["id"] is None:
         feat["id"] = idx + 1

   # 2. Serialize to binary WKBC format using your cleaned function
   wkbc.write_wkb_collection_file(
      fc=input_fc,
      path=temp_wkbc_path,
      include_fc_bbox=True,
      include_feature_bbox=True
   )

   # 3. Read binary structures back from the temporary file
   parsed_fc = wkbc.read_wkb_collection_file(temp_wkbc_path)

   # Print structural validation stats directly to the console
   print(f"\n--- WKBC PIPELINE ROUND-TRIP TEST FOR {input_path} ---")
   print(f"Collection-level BBOX: {parsed_fc.get('bbox')}")

   features = parsed_fc.get("features", []) or []
   print(f"Total features recovered: {len(features)}")
   if features:
      print(f"Feature #0 BBOX: {features[0].get('bbox')}")
   print("--------------------------------------------------\n")

   # 4. Save round-tripped validation dataset to the output GeoJSON
   with open(output_geojson_path, "w", encoding="utf-8") as f:
      json.dump(parsed_fc, f, indent=2, ensure_ascii=False)

if __name__ == "__main__":
   run_wkbc_test()
