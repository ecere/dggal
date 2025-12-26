#!/usr/bin/env python3
# dgg-fetch.py
from dggal import *
from typing import List, Any, Dict, Iterator, Iterable, Optional, Tuple
import os, logging, argparse, json, sys
from itertools import islice

from ogcapi.dggs import client as dggs_client
from dggsStore.store import DGGSDataStore, read_package_root_ids_from_sqlite

app = Application(appGlobals=globals()); pydggal_setup(app)

LOG_FMT = "%(asctime)s %(levelname)s %(message)s"
logging.basicConfig(level=logging.INFO, format=LOG_FMT)
logger = logging.getLogger("raster")


def parse_args():
   p = argparse.ArgumentParser(prog="raster")
   p.add_argument("resource_url", help="Full DGGS resource URL e.g. https://host/.../collections/{collection}/dggs/{dggrsId}")
   p.add_argument("--outdir", default="data", help="Output directory for packages (default data/)")
   p.add_argument("--groupSize", type=int, default=5, help="Levels per package (written to collection.json and passed to store)")
   p.add_argument("--depth", type=int, help="data depth (zone-depth) to request (default from DGGRS defaultDepth)")
   p.add_argument("--max-level", type=int, default=None, help="Maximum refinement level (default from server maxRefinementLevel)")
   p.add_argument("--batch-size", type=int, default=32, help="Number of root zones to request per HTTP call (default 32)")
   p.add_argument("--no-resume", action="store_true")
   p.add_argument("--resume-verbose", action="store_true", help="Show resume-related info for skipped roots")
   p.add_argument("--dry-run", action="store_true")
   p.add_argument("--debug", action="store_true", help="Enable debug logging")
   return p.parse_args()


def process_batch(
   store: DGGSDataStore,
   zones: List[int],
   pkg_index: int,
   batch_num: int,
   args,
   existing_ids: set,
   pkg_path: str,
   bases_stack: List[int],
   per_package_workers: int,
   depth: int,
   landing: str,
   collection: str,
   dggrs_id: str
) -> int:
   dggrs = store.dggrs

   zone_by_text: Dict[str, int] = {}
   to_fetch_texts: List[str] = []

   for zone in zones:
      ztext = dggrs.getZoneTextID(zone)
      zone_by_text[ztext] = zone
      if ztext not in existing_ids:
         to_fetch_texts.append(ztext)

   if not to_fetch_texts:
      logger.info("PACKAGE #%d BATCH %d: all roots present, skipping", pkg_index, batch_num)
      return 0

   if not landing or not collection or not dggrs_id:
      logger.error("Invalid fetch context; skipping batch")
      return 0

   if args.dry_run:
      for ztext in to_fetch_texts:
         url = f"{landing.rstrip('/')}/collections/{collection}/dggs/{dggrs_id}/zones/{ztext}/data"
         params = f"?zone-depth={depth}"
         logger.info("DRY %s <- %s%s", pkg_path, url, params)
      return 0

   fetched = dggs_client.fetch_zone_data_parallel(landing, collection, dggrs_id, to_fetch_texts, workers=per_package_workers, depth=depth)

   entries: Dict[int, Any] = {}
   for ztext in to_fetch_texts:
      if ztext in fetched:
         zone = zone_by_text[ztext]
         zone_id = int(zone)
         entries[zone_id] = fetched[ztext]

   if not entries:
      logger.info("PACKAGE #%d BATCH %d: no fetched entries", pkg_index, batch_num)
      return 0

   store.write_zone_batch(base_zone=zones[0], entries=entries, base_ancestor_list=bases_stack, precompressed=False)
   written = len(entries)
   logger.info("PACKAGE #%d: wrote %d roots (batch %d) to %s", pkg_index, written, batch_num, pkg_path)
   return written


def main():
   args = parse_args()
   logger.setLevel(logging.DEBUG if args.debug else logging.INFO)

   landing, collection, dggrs_id = dggs_client.parse_resource_url(args.resource_url)
   if not landing or not collection or not dggrs_id:
      logger.error("Invalid resource_url; must include collection and dggrs id")
      return

   landing = landing.rstrip('/')

   dggrs_desc = dggs_client.get_dggrs_description(landing, collection, dggrs_id)
   depth = args.depth if args.depth is not None else int(dggrs_desc.get("defaultDepth", 8))

   # Use explicit --max-level if provided; otherwise prefer server's maxRefinementLevel; fallback to 24
   server_max = dggrs_desc.get("maxRefinementLevel")
   if args.max_level is not None:
      max_refinement = int(args.max_level)
   elif server_max is not None:
      max_refinement = int(server_max)
   else:
      logger.error("Server did not provide maxRefinementLevel and --max-level not specified; aborting")
      return

   coll_meta = dggs_client.get_collection_info(landing, collection)
   title = coll_meta.get("title") or coll_meta.get("name") or collection
   description = coll_meta.get("description") or coll_meta.get("abstract") or coll_meta.get("summary") or title

   coll_info = {
      "dggrs": dggrs_id,
      "maxRefinementLevel": max_refinement,
      "depth": depth,
      "groupSize": args.groupSize,
      "title": title,
      "description": description,
      "version": coll_meta.get("version", "1.0")
   }

   base = os.path.join(args.outdir, collection)
   os.makedirs(base, exist_ok=True)
   coll_json_path = os.path.join(base, "collection.json")
   with open(coll_json_path, "w", encoding="utf-8") as fh:
      json.dump(coll_info, fh, indent=2)
   logger.info("Wrote collection config to %s", coll_json_path)

   logger.info("Instantiating DGGSDataStore groupSize=%d dggrs=%s depth=%s", coll_info["groupSize"], coll_info["dggrs"], coll_info["depth"])
   store = DGGSDataStore(args.outdir, collection, config=coll_info)
   dggrs = store.dggrs

   total_written = 0
   fetch_batch_size = int(args.batch_size)
   per_package_workers = min(fetch_batch_size, 32)

   deepest_root_level = max(0, max_refinement - depth)
   max_base_level = store._base_level_for_root(deepest_root_level)
   logger.info("Computed levels: depth=%d max_base_level=%d groupSize=%d fetch_batch_size=%d",
               depth, max_base_level, store.groupSize, fetch_batch_size)

   logger.info("Beginning package iteration (max_base_level=%d)", max_base_level)

   pkg_index = 0

   for base_zone, base_ancestors in store.iter_bases(max_base_level, up_to=True):
      pkg_index += 1

      base_text = dggrs.getZoneTextID(base_zone)
      pkg_path = store.compute_package_path_for_root_zone(base_zone, base_ancestors)
      logger.info("PACKAGE #%d: base_zone=%s pkg_path=%s", pkg_index, base_text, pkg_path)

      base_level = dggrs.getZoneLevel(base_zone)
      package_group_levels = store.group0Size if base_level == 0 else store.groupSize
      max_root_level = base_level + package_group_levels - 1
      roots_iter = store.iter_roots_for_base(base_zone, max_root_level, up_to=True)


      existing_ids = set()
      if not args.no_resume:
         existing_ids = set(read_package_root_ids_from_sqlite(pkg_path))
         if args.resume_verbose:
            logger.info("PACKAGE #%d: resume existing_ids_count=%d", pkg_index, len(existing_ids))

      batch_num = 0
      batch_zones: List[int] = []
      for zone in roots_iter:
         ztext = dggrs.getZoneTextID(zone)

         if ztext in existing_ids:
            if args.resume_verbose:
               logger.debug("PACKAGE #%d: skipping already-present root %s", pkg_index, ztext)
            continue

         batch_zones.append(zone)
         if len(batch_zones) >= fetch_batch_size:
            batch_num += 1
            written = process_batch(store, batch_zones, pkg_index, batch_num, args, existing_ids, pkg_path, base_ancestors, per_package_workers, depth, landing, collection, dggrs_id)
            total_written += written
            batch_zones = []

      if batch_zones:
         batch_num += 1
         written = process_batch(store, batch_zones, pkg_index, batch_num, args, existing_ids, pkg_path, base_ancestors, per_package_workers, depth, landing, collection, dggrs_id)
         total_written += written

   logger.info("all processing complete; total written=%d; output=%s", total_written, args.outdir)


if __name__ == "__main__":
   main()
