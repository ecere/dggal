# ogcapi_dggs_zoneInfo.py
# Zone info endpoint (relies only on DGGRS implementation; no DGGS store access)
from dggal import *
from dggsStore.store import *
from ..utils import *
from flask import Blueprint, request, url_for, Response, current_app
from typing import Any, Dict, List
import logging
import ubjson

logger = logging.getLogger("dggs-serve.zoneInfo")

bp = Blueprint("dggs_zoneinfo", __name__, url_prefix="/collections/<collectionId>/dggs/<dggrsId>")

# HTML fragment: top links, card with zone info, then download links at the bottom
ZONE_HTML = """
{% block content %}
  <div class="small top-links">
    {% for l in top_links %}
      <a href="{{ l.href }}">{{ l.title }}</a>{% if not loop.last %} · {% endif %}
    {% endfor %}
  </div>

  <div class="card">
    <h2>Zone information for DGGRS {{ dggrsTitle }} and collection {{ collectionTitle }}</h2>

    {% if zone.level is defined %}
      <div class="small">Level: {{ zone.level }}</div>
    {% endif %}

    {% if zone.shapeType is defined %}
      <div class="small">Shape: {{ zone.shapeType }}</div>
    {% endif %}

    {% if zone.areaMetersSquare is defined %}
      <div class="small">Area (m²): {{ zone.areaMetersSquare }}</div>
    {% endif %}

    {% if zone.centroid is defined %}
      <div class="small">Centroid:</div>
      <div class="small" style="margin-left:20px">Latitude: {{ zone.centroid[1] }}</div>
      <div class="small" style="margin-left:20px">Longitude: {{ zone.centroid[0] }}</div>
    {% endif %}

    {% if zone.bbox is defined %}
      <div class="small">Extent:</div>
      <div class="small" style="margin-left:20px">Latitude: from {{ zone.bbox[1] }} to {{ zone.bbox[3] }}</div>
      <div class="small" style="margin-left:20px">Longitude: from {{ zone.bbox[0] }} to {{ zone.bbox[2] }}</div>
    {% endif %}

    {% if zone.geometry is defined %}
      <div class="small">Geometry (Polygon):</div>
      <pre style="background:#f8f8f8;padding:8px;border-radius:4px;overflow:auto;max-height:300px">{{ zone.geometry | tojson(indent=2) }}</pre>
    {% endif %}

    <div style="height:8px"></div>

    <div class="small">Download data for this zone:
      <a href="{{ data_json }}">JSON</a> · <a href="{{ data_ubjson }}">UBJSON</a>
    </div>
  </div>
{% endblock %}
"""

# ---------------- Helpers ----------------

def polygon_geometry_from_zone(dggrs, zone):
   """
   Return a GeoJSON Polygon geometry for zone using DGGRS.getZoneRefinedWGS84Vertices.
   - Calls dggrs.getZoneRefinedWGS84Vertices(zone, 0).
   - Converts vertex objects to [lon, lat] float pairs.
   - Ensures the ring is closed (last == first).
   - Returns None if the vertex list is empty or None.
   Note: returns the Polygon geometry directly (no Feature wrapper, no properties).
   """
   if zone is None:
      return None
   verts = dggrs.getZoneRefinedWGS84Vertices(zone, 0)
   if not verts:
      return None
   coords = [[float(v.lon), float(v.lat)] for v in verts]
   if not coords:
      return None
   if coords[0] != coords[-1]:
      coords.append(coords[0])
   return {"type": "Polygon", "coordinates": [coords]}

# ---------------- Route: zone info ----------------

@bp.route("/zones/<zoneId>", methods=["GET"])
def get_zone_info(collectionId: str, dggrsId: str, zoneId: str):
   """
   Zone info endpoint that relies only on the DGGRS implementation for zone metadata.
   The incoming path parameter is a text identifier; resolve it to a DGGRSZone using
   dggrs.getZoneFromTextID(zoneId). All subsequent DGGRS calls use that DGGRSZone value.
   Produces JSON / UBJSON / HTML.
   """
   app = current_app._get_current_object() if hasattr(current_app, "_get_current_object") else current_app

   # Obtain DGGRS implementation instance by name
   dggrs = get_or_create_dggrs(dggrsId)
   if dggrs is None:
      payload = {"error": "DGGRS implementation not available", "dggrs": dggrsId}
      fmt, _ = negotiate_format(request, request.path)
      if fmt == "html":
         top_links = [
            {"href": f"/collections/{collectionId}/dggs/{dggrsId}/zones", "title": "Back to zones"},
            {"href": request.path + "?f=json", "title": "JSON"},
            {"href": request.path + "?f=ubjson", "title": "UBJSON"}
         ]
         return html_response("<div class='card'><h2>DGGRS not available</h2></div>", {"title": "DGGRS not available", "top_links": top_links}, status=404)
      return Response(pretty_json(payload) + "\n", status=404, mimetype="application/json; charset=utf-8")

   # Resolve the incoming text identifier to the DGGRSZone (internal 64-bit identifier)
   zone = dggrs.getZoneFromTextID(zoneId)
   if zone is None:
      payload = {"error": "Zone not found", "zone": zoneId}
      fmt, _ = negotiate_format(request, request.path)
      if fmt == "html":
         top_links = [
            {"href": f"/collections/{collectionId}/dggs/{dggrsId}/zones", "title": "Back to zones"},
            {"href": request.path + "?f=json", "title": "JSON"},
            {"href": request.path + "?f=ubjson", "title": "UBJSON"}
         ]
         return html_response("<div class='card'><h2>Zone not found</h2></div>", {"title": "Zone not found", "top_links": top_links}, status=404)
      return Response(pretty_json(payload) + "\n", status=404, mimetype="application/json; charset=utf-8")

   # canonical text id for presentation / links (DGGRS provides text id)
   zid_text = dggrs.getZoneTextID(zone) or zoneId

   # Use store to obtain collection title
   root = data_root()
   store = get_store(root, collectionId)

   # Titles for H2
   dggrs_title = dggrsId
   collection_title = store.config.get("title", collectionId)

   # Build fixed canonical links for this resource and its data subresource
   up_href = f"/collections/{collectionId}/dggs/{dggrsId}/zones"
   json_self = request.path + "?f=json"
   ubjson_self = request.path + "?f=ubjson"
   data_json = f"/collections/{collectionId}/dggs/{dggrsId}/zones/{zid_text}/data?f=json"
   data_ubjson = f"/collections/{collectionId}/dggs/{dggrsId}/zones/{zid_text}/data?f=ubjson"

   # Links presented in JSON responses: include UBJSON alternate for the resource and bracketed OGC rels for data
   json_links: List[Dict[str, Any]] = [
      {"rel": "self", "type": "application/json", "href": json_self},
      {"rel": "alternate", "type": "text/html", "href": request.path + "?f=html"},
      {"rel": "alternate", "type": "application/ub+json", "href": ubjson_self},
      {"rel": "[ogc-rel:dggrs-zone-data]", "type": "application/json", "href": data_json},
      {"rel": "[ogc-rel:dggrs-zone-data]", "type": "application/ub+json", "href": data_ubjson}
   ]

   # Top links for HTML: Back to zones then JSON and UBJSON alternates for this resource
   top_links = [
      {"href": up_href, "title": "Back to zones"},
      {"href": json_self, "title": "JSON"},
      {"href": ubjson_self, "title": "UBJSON"}
   ]

   # Assemble zone dict using DGGRS methods only (no defensive probing)
   zone_payload: Dict[str, Any] = {"id": zid_text, "links": json_links, "crs": "[OGC:CRS84]"}

   # level
   lvl = dggrs.getZoneLevel(zone)
   if lvl is not None:
      zone_payload["level"] = int(lvl)

   # shape type derived from edge count (only 3-6 handled)
   edges = dggrs.countZoneEdges(zone)
   if edges == 3:
      zone_payload["shapeType"] = "triangle"
   elif edges == 4:
      zone_payload["shapeType"] = "quadrilateral"
   elif edges == 5:
      zone_payload["shapeType"] = "pentagon"
   elif edges == 6:
      zone_payload["shapeType"] = "hexagon"

   # geometry included for response only (build Polygon geometry from refined WGS84 vertices)
   polygon_geom = polygon_geometry_from_zone(dggrs, zone)
   if polygon_geom:
      zone_payload["geometry"] = polygon_geom

   # centroid: use getZoneWGS84Centroid() which returns a GeoPoint with lat and lon
   gp = dggrs.getZoneWGS84Centroid(zone)
   if gp:
      # present as [lon, lat] internally; HTML template formats as Latitude/Longitude lines
      zone_payload["centroid"] = [float(gp.lon), float(gp.lat)]

   # area (present before extent)
   a = dggrs.getZoneArea(zone)
   if a is not None:
      zone_payload["areaMetersSquare"] = float(a)

   # bbox: use getZoneWGS84Extent(extent) which fills a GeoExtent object with ll and ur members
   extent = GeoExtent()
   dggrs.getZoneWGS84Extent(zone, extent)
   # canonical order [minx, miny, maxx, maxy]
   zone_payload["bbox"] = [float(extent.ll.lon), float(extent.ll.lat), float(extent.ur.lon), float(extent.ur.lat)]

   # Format negotiation and response
   fmt, _ = negotiate_format(request, request.path)

   if fmt == "html":
      # Render HTML with top_links and download links at the bottom; pass ubjson_self as well
      return html_response(
         ZONE_HTML,
         title=f"Zone {zid_text}",
         zone=zone_payload,
         collectionId=collectionId,
         dggrsId=dggrsId,
         dggrsTitle=dggrs_title,
         collectionTitle=collection_title,
         top_links=top_links,
         data_json=data_json,
         data_ubjson=data_ubjson,
         ubjson_self=ubjson_self
      )

   if fmt == "ubjson":
      payload = ubjson.dumpb(zone_payload)
      return Response(payload, status=200, mimetype="application/ub+json")

   # default JSON response
   return Response(pretty_json(zone_payload) + "\n", status=200, mimetype="application/json; charset=utf-8")
