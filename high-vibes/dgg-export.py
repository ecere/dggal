#!/usr/bin/env python3
import argparse
import logging
from dggal import *
from dggsStore.store import DGGSDataStore
from dggsExport.raster import rasterize_to_geotiff

app = Application(appGlobals=globals()); pydggal_setup(app)

def parse_args():
   p = argparse.ArgumentParser(prog="dgg-export")
   p.add_argument("datastore", help="base datastore directory")
   p.add_argument("outfile", help="output GeoTIFF path")
   p.add_argument("--collection", required=True, help="collection name")
   p.add_argument("--dggrs", required=False, help="(ignored) store provides dggrs instance")
   p.add_argument("--level", type=int, required=True, help="DGGS level to sample")
   p.add_argument("--workers", type=int, default=8)
   p.add_argument("--debug", action="store_true")
   p.add_argument("--nodata", type=float, default=3.4028235e+38)
   p.add_argument("--compress", default="lzw")
   return p.parse_args()

def main():
   args = parse_args()
   if args.debug:
      logging.getLogger().setLevel(logging.DEBUG)

   store: DGGSDataStore = DGGSDataStore(args.datastore, args.collection)

   result = rasterize_to_geotiff(
      store,
      args.level,
      args.outfile,
      workers=args.workers,
      nodata=args.nodata,
      compress=args.compress,
      debug=args.debug
   )
   logging.info("done: elapsed=%.1f s", result["elapsed_seconds"])

if __name__ == "__main__":
   main()
