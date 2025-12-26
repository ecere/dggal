# ogcapi_collections.py
# OGC API - DGGS Collections

from flask import Blueprint, request, url_for, Response
import os
from dggsStore.store import get_store
from ..utils import (
   data_root,
   negotiate_format,
   make_link,
   label_for_link,
   html_response,
)
import json

bp = Blueprint("collections", __name__, url_prefix="/collections")

# ----------------------------------------------------------------------
# Templates (minimal presentation tweaks only)
# - Single modest "Explore list of DGGRSs" action placed once below spatial extent
# - Indent lat/lon lines and include degree symbol
# - Keep small spacing above description
# ----------------------------------------------------------------------

COLLECTIONS_HTML = """
{% block content %}
  <div class="small top-links">
    {% for l in top_links %}
      <a href="{{ l.href }}">{{ l.label }}</a>{% if not loop.last %} · {% endif %}
    {% endfor %}
  </div>

  <div class="grid">
  {% for c in collections %}
    <div class="card">
      <h2 class="collection-title"><a href="{{ c.self_href }}">{{ c.title }}</a></h2>
      <p class="description" style="margin-top:0.25rem;">{{ c.description }}</p>

      <div class="small">
        {% for l in c.sub_links %}
          <a href="{{ l.href }}">{{ l.label }}</a>{% if not loop.last %} · {% endif %}
        {% endfor %}
      </div>
    </div>
  {% endfor %}
  </div>
{% endblock %}
"""

COLLECTION_HTML = """
{% block content %}
  <div class="small top-links">
    {% for l in top_links %}
      <a href="{{ l.href }}">{{ l.label }}</a>{% if not loop.last %} · {% endif %}
    {% endfor %}
  </div>

  {# small spacing above description retained #}
  {% if collection.description %}
    <p style="margin-top:0.25rem;margin-bottom:0.5rem;color:#444;">{{ collection.description }}</p>
  {% endif %}

  {% if extent_lines %}
    <div style="margin:0.75rem 0;">
      <div style="font-weight:600;margin-bottom:0.25rem;">Spatial extent</div>
      <div style="margin-left:1rem;color:#333;">
        {% for line in extent_lines %}
          <div style="margin:0.15rem 0;">{{ line }}</div>
        {% endfor %}
      </div>
    </div>
  {% endif %}

  {# Single modest action placed only here (below spatial extent) #}
  {% if sub_links %}
    <div style="margin:0.5rem 0 0.75rem 0;">
      <a href="{{ sub_links[0].href }}" style="font-size:1.05rem;color:#0b74de;text-decoration:none;font-weight:600;">
        Explore list of DGGRSs
      </a>
    </div>
  {% endif %}
{% endblock %}
"""

# ----------------------------------------------------------------------
# Helpers
# ----------------------------------------------------------------------

def collections_base(root: str) -> str:
   path = os.path.join(root, "collections")
   return path if os.path.isdir(path) else root

def format_extent(extent):
   try:
      minx, miny, maxx, maxy = map(float, extent)
   except Exception:
      return []
   # include degree symbol
   return [
      f"Latitude: from {miny}° to {maxy}°",
      f"Longitude: from {minx}° to {maxx}°",
   ]

# ----------------------------------------------------------------------
# Routes
# ----------------------------------------------------------------------

@bp.route("", methods=["GET"])
def list_collections():
   root = data_root()
   base = collections_base(root)
   ids = sorted(d for d in os.listdir(base) if os.path.isdir(os.path.join(base, d)))

   fmt, _ = negotiate_format(request)

   collections = []
   for cid in ids:
      store = get_store(root, cid)
      cfg = getattr(store, "config", {}) or {}

      self_href = url_for("collections.get_collection", collectionId=cid)

      # JSON links (self + alternate + dggrs)
      links = [
         make_link(f"{self_href}?f=json", rel="self", type="application/json"),
         make_link(f"{self_href}?f=html", rel="alternate", type="text/html"),
         make_link(
            url_for("dggrs.list_collection_dggrs", collectionId=cid),
            rel="[ogc-rel:dggrs]",
            title="DGGRS list"
         )
      ]

      # HTML-only subresource links (kept as small text links)
      sub_links = [
         {"href": l["href"], "label": label_for_link(l)}
         for l in links if l.get("rel") == "[ogc-rel:dggrs]"
      ]

      collections.append({
         "id": cid,
         "title": cfg.get("title", cid),
         "description": cfg.get("description", ""),
         "self_href": self_href,
         "sub_links": sub_links,   # HTML only
         "links": links            # JSON only
      })

   if fmt == "html":
      top_links = [
         {"href": "/", "label": "Back to landing page"},
         {"href": f"{request.path}?f=json", "label": "JSON"},
      ]
      return html_response(
         COLLECTIONS_HTML,
         title="Collections",
         collections=collections,
         top_links=top_links,
      )

   # JSON output (NO sub_links)
   body = json.dumps({"collections": collections}, indent=2) + "\n"
   return Response(body, mimetype="application/json; charset=utf-8")


@bp.route("/<collectionId>", methods=["GET"])
def get_collection(collectionId: str):
   root = data_root()
   store = get_store(root, collectionId)
   if store is not None:
      cfg = getattr(store, "config", {}) or {}

      fmt, _ = negotiate_format(request)

      self_href = url_for("collections.get_collection", collectionId=collectionId)

      # JSON links (self + alternate + up + dggrs)
      links = [
         make_link(f"{self_href}?f=json", rel="self", type="application/json"),
         make_link(f"{self_href}?f=html", rel="alternate", type="text/html"),
         make_link(url_for("collections.list_collections"), rel="up"),
         make_link(
            url_for("dggrs.list_collection_dggrs", collectionId=collectionId),
            rel="[ogc-rel:dggrs]",
            title="DGGRS list"
         )
      ]

      resp = {
         "id": collectionId,
         "title": cfg.get("title", collectionId),
         "description": cfg.get("description", ""),
         "extent": {"spatial": {"bbox": [[-180, -90, 180, 90]]}},
         "links": links,
      }

      extent = resp["extent"]["spatial"]["bbox"][0]
      extent_lines = format_extent(extent)

      if fmt == "html":
         top_links = [
            {"href": url_for("collections.list_collections"), "label": "Back to Collections"},
            {"href": f"{request.path}?f=json", "label": "JSON"},
         ]

         sub_links = [
            {"href": l["href"], "label": label_for_link(l)}
            for l in links if l.get("rel") == "[ogc-rel:dggrs]"
         ]

         return html_response(
            COLLECTION_HTML,
            title=resp["title"],
            collection=resp,
            top_links=top_links,
            sub_links=sub_links,
            extent_lines=extent_lines,
         )

      body = json.dumps(resp, indent=2) + "\n"
      return Response(body, mimetype="application/json; charset=utf-8")
   else:
      return Response(b"", status=404, mimetype=None)
