# ogcapi_dggs_zoneData.py
from flask import Blueprint, request, Response, current_app
import logging
import gzip
import json
import ubjson
from typing import Any, Dict, List

from dggsStore.store import *
from dggsStore.customDepths import assemble_zone_at_depth, parse_zone_depths, build_dggs_json_from_values
from fg.dggsJSONFG import read_dggs_json_fg

from ..utils import *

logger = logging.getLogger("dggs-serve.zoneData")

bp = Blueprint("dggs_zoneData", __name__, url_prefix="/collections/<collectionId>/dggs/<dggrsId>")

@bp.route("/zones/<zoneId>/data", methods=["GET"])
@bp.route("/zones/<zoneId>/data.json", methods=["GET"])
@bp.route("/zones/<zoneId>/data.ubjson", methods=["GET"])
@bp.route("/zones/<zoneId>/data.geojson", methods=["GET"])
def dggs_zone_data(collectionId: str, dggrsId: str, zoneId: str):

   payload_already_gzipped = False
   raw_blob: bytes | None = None
   dggs_json: object | None = None
   body_bytes: bytes | None = None

   DATA_ROOT = data_root()
   store = get_store(DATA_ROOT, collectionId)
   if store is not None:
      is_vector = store.is_vector
      dggrs_impl = store.dggrs
      zone = dggrs_impl.getZoneFromTextID(zoneId)

      fmt, gzip_ok = negotiate_format(request, request.path)
      if is_vector:
         if fmt == "geojson" or fmt == "geoubjson":
            profile = "rfc7946"
         else:
            if fmt == "json":      fmt = "geojson"
            elif fmt == "ubjson":  fmt = "geoubjson"
            profile = "jsonfg-dggs"
         profile = negotiate_profile(request, fmt, profile)
         return_dggs_fg = not profile_token_matches(profile, GEOJSON_PROFILES)
      else:
         profile = None

      requested_depths = parse_zone_depths(request.args.get("zone-depth", str(store.depth)))

      if len(requested_depths) == 1 and requested_depths[0] == store.depth:
         pkg = store.compute_package_path_for_root_zone(zone)
         if pkg is not None:
            blob = store.read_zone_blob(pkg, zone)
            if blob is not None:
               # We can't return vector directly because we need to add attributes
               if fmt == "ubjson":
                  if gzip_ok:
                     raw_blob = blob
                     payload_already_gzipped = True
                  else:
                     raw_blob = decompress_blob(blob)
               else:
                  dggs_json = decode_blob(blob)
      elif not is_vector:
         collected_by_depth: CollectedValues = {}
         for d in requested_depths:
            if d == store.depth:
               pkg = store.compute_package_path_for_root_zone(zone)
               blob = store.read_zone_blob(pkg, zone) if pkg is not None else None
               obj = decode_blob(blob) if blob is not None else None
               values_for_depth = obj["values"] if obj is not None else None
            else:
               values_for_depth = assemble_zone_at_depth(store, zone, d)
            if values_for_depth is None:
               collected_by_depth = None
               break
            collected_by_depth[d] = values_for_depth
         if collected_by_depth is not None:
            dggs_json = build_dggs_json_from_values(store, zone, collected_by_depth)
      else:
         body_bytes = pretty_json({"error": "Custom depth not yet supported for vector data"}) + "\n",
         response_status = 400
         payload_mimetype = "application/json"

      if is_vector and dggs_json:
         features = dggs_json.get("features", []) or []
         if features:
            ids = [f.get("id") for f in features if f.get("id") is not None]
            if ids:
               attrs_map = store.get_attributes_for_feature_ids(ids)
               for feat in features:
                  fid = feat.get("id")
                  feat["properties"] = attrs_map.get(fid, {}) or {}
            if not return_dggs_fg:
               dggs_json = read_dggs_json_fg(dggs_json, refine_wgs84=1e-2)

   response_headers = None

   if raw_blob or dggs_json:
      response_status = 200
      if fmt == "geojson":
         body_bytes = pretty_json(dggs_json, indent=3, indent_step=2).encode("utf-8")
         payload_mimetype = "application/geo+json"
      elif fmt == "geoubjson":
         body_bytes = raw_blob if raw_blob is not None else ubjson.dumpb(dggs_json)
         payload_mimetype = "application/geo+ubjson"
      elif fmt == "ubjson":
         body_bytes = raw_blob if raw_blob is not None else ubjson.dumpb(dggs_json)
         payload_mimetype = "application/ubjson"
      else:
         body_bytes = pretty_json(dggs_json, indent=3, indent_step=2).encode("utf-8")
         payload_mimetype = "application/json"

      if gzip_ok:
         if not payload_already_gzipped:
            body_bytes = gzip.compress(body_bytes)
         response_headers = { "Content-Encoding": "gzip" }

      if profile:
         if not response_headers: response_headers = { }
         response_headers["Link"] = f'<https://www.opengis.net/def/profile/ogc/0/{profile}>; rel="profile"'

   elif not body_bytes:
      body_bytes = b""
      response_status = 404
      payload_mimetype = None

   return Response(body_bytes, status=response_status, mimetype=payload_mimetype, headers=response_headers)
