#!/usr/bin/env python3
# dgg-import.py
# Import a raster or vector file into a DGGS Data Store by
# sampling the raster at, or quantizing the vector coordinates to, DGGS sub-zone centroids
# and writing results into the store via DGGSDataStore.write_zone_batch.
#
# Usage:
#  python dgg-import.py input.(tif|json) --dggrs IVEA4R --data-root data --collection mycol
#
# Notes:
# - Default import level is computed from the raster native resolution via DGGRS.getLevelFromPixelsAndExtent.
# - The finest root level = data_level - depth (depth defaults to dggrs.get64KDepth()).
# - Batching mirrors dgg-fetch: roots are grouped and written with write_zone_batch.

from dggal import *
import argparse
import os
import rasterio
import sys

from dggsImport.rasterImport import *
from dggsImport.vectorImport import import_vector

# initialize dggal runtime
app = Application(appGlobals=globals()); pydggal_setup(app)

# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------
def _parse_bands_and_fields_arg(args, ds_count: int):
   # Returns (bands: Optional[List[int]], fields: Optional[List[str]], err: Optional[str]).
   if args.bands is None:
      bands = None
   else:
      parts = [p.strip() for p in args.bands.split(",") if p.strip()]
      bands = []
      for p in parts:
         if not p.isdigit():
            return None, None, "Invalid --bands: must be comma-separated positive integers (1-based)."
         b = int(p)
         if b < 1 or b > ds_count:
            return None, None, f"Invalid band index {b}: must be 1..{ds_count}."
         bands.append(b)

   fields = [f.strip() for f in args.fields.split(",") if f.strip()] if args.fields else None

   if fields is not None:
      expected = ds_count if bands is None else len(bands)
      if len(fields) != expected:
         return None, None, f"Field count {len(fields)} does not match expected {expected}."

   return bands, fields, None

def main():
   p = argparse.ArgumentParser(prog="dgg-import")
   p.add_argument("input_file", help="input raster file or vector GeoJSON (.json/.geojson)")
   p.add_argument("--dggrs", default="IVEA4R", help="DGGRS name (class name or string)")
   p.add_argument("--collection", default=None, help="collection id (defaults to filename without ext)")
   p.add_argument("--data-root", default="data", help="data root directory (default data)")
   p.add_argument("--level", type=int, default=None, help="DGGS data level (defaults to raster native resolution)")
   p.add_argument("--depth", type=int, default=None, help="depth (default dggrs.get64KDepth())")
   p.add_argument("--fields", default=None, help="comma-separated field names (defaults to field1[,field2...])")
   p.add_argument("--bands", default=None, help="comma-separated 1-based band indices to sample (defaults to all bands)")
   p.add_argument("--batch-size", type=int, default=32, help="Number of root zones per write batch")
   p.add_argument("--groupSize", type=int, default=5, help="Levels per package (default 5)")
   p.add_argument("--max-workers", type=int, default=16, help="Max worker processes")
   p.add_argument("--skip-reproj", action="store_true", help="Skip reprojection step for vector import")
   p.add_argument("--skip-fix", action="store_true", help="Skip topology fix step for vector import")
   args = p.parse_args()

   input_path = args.input_file
   ext = os.path.splitext(input_path)[1].lower()

   collection_id = args.collection if args.collection else os.path.splitext(os.path.basename(input_path))[0]

   # Vector import branch for GeoJSON files
   if ext in (".json", ".geojson"):
      print(f"[IMPORT] Detected vector input {input_path}; using vector import", flush=True)

      if args.level is None:
         print("Error: --level is required for vector import (absolute quantize level).", flush=True)
         return 1

      rc = import_vector(
         input_geojson_path=input_path,
         collection_id=collection_id,
         dggrs_name=args.dggrs,
         data_root=args.data_root,
         level=args.level,
         depth=args.depth,
         batch_size=args.batch_size,
         groupSize=args.groupSize,
         max_workers=args.max_workers,
         skip_reproj=args.skip_reproj,
         skip_fix=args.skip_fix
      )
      return rc

   # Raster import branch (default)
   try:
      ds = rasterio.open(input_path)
   except Exception as e:
      print(f"Error: failed to open raster {input_path}: {e}", flush=True)
      return 1

   ds_count = ds.count

   bands, fields, err = _parse_bands_and_fields_arg(args, ds_count)
   if err:
      print("Error:", err, flush=True)
      ds.close()
      return 1

   rc = import_raster(
      ds=ds,
      collection_id=collection_id,
      dggrs_name=args.dggrs,
      data_root=args.data_root,
      level=args.level,
      depth=args.depth,
      fields=fields,
      bands=bands,
      batch_size=args.batch_size,
      groupSize=args.groupSize,
      max_workers=max_workers
   )

   return rc

if __name__ == "__main__":
   sys.exit(main())
