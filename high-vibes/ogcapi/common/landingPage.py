# ogcapi_landingPage.py
# Landing page for the DGGS API.

from flask import *
from ..utils import data_root, negotiate_format, html_response, pretty_json
import os
from typing import List, Dict, Any

bp = Blueprint("landing", __name__)

PAGE_TITLE = "DGGAL High Vibes DGGS Data Store"
PAGE_SUBTITLE = "A vibe-coded OGC API - DGGS implementation based on DGGAL"

ROOT_HTML = """
{% block content %}
  <div class="small top-links">
    {% for l in top_links %}
      <a href="{{ l.href }}">{{ l.title }}</a>{% if not loop.last %} · {% endif %}
    {% endfor %}
  </div>

  <div class="card">
    <p class="small">{{ subtitle }}</p>

    <div class="big-link" style="margin:0.5em 0; font-weight:600;">
      <a href="{{ data_collections_href }}">Browse Data Collections</a>
    </div>

    <div class="small mid-links">
      <a href="{{ conformance_href }}">Conformance declaration</a> ·
      <a href="{{ api_doc_href }}">API documentation</a> ·
      <a href="{{ openapi_href }}">OpenAPI definition</a>
    </div>
  </div>
{% endblock %}
"""

@bp.route("/", methods=["GET"])
def root_index():
   ROOT = data_root()

   # collections list removed per request (not enumerating directories)
   collections: List[Dict[str, Any]] = []

   base = None
   if ROOT and os.path.isdir(ROOT):
      # Prefer a "collections" subfolder if present
      coll_dir = os.path.join(ROOT, "collections")
      if os.path.isdir(coll_dir):
         base = coll_dir
      else:
         # Fall back to ROOT (not used for listing here)
         base = ROOT

   html_self = url_for("landing.root_index", _external=False)
   json_self = html_self + "?f=json"

   top_links = [
      {"href": json_self, "title": "JSON"}
   ]

   # Data Collections promoted separately
   data_collections_href = url_for("collections.list_collections")
   conformance_href = url_for("conformance.conformance")
   api_doc_href = "https://developer.ogc.org/api/dggs/index.html"
   openapi_href = "https://schemas.opengis.net/ogcapi/dggs/1.0/openapi/ogcapi-dggs-1.bundled.json"

   links = [
      {"rel": "self", "type": "application/json", "href": json_self},
      {"rel": "alternate", "type": "text/html", "href": html_self},
      {"rel": "data", "href": data_collections_href},
      {"rel": "conformance", "href": conformance_href},
      {"rel": "service-doc", "type" : "text/html", "href": api_doc_href},
      {"rel": "service-desc", "type": "application/vnd.oai.openapi+json;version=3.0", "href": openapi_href}
   ]

   fmt, _ = negotiate_format(request, request.path)

   if fmt == "json":
      payload = {
         "title": PAGE_TITLE,
         "description": PAGE_SUBTITLE,
         "links": links
      }
      return Response(pretty_json(payload) + "\n", mimetype="application/json; charset=utf-8")

   return html_response(
      ROOT_HTML,
      title=PAGE_TITLE,
      subtitle=PAGE_SUBTITLE,
      # collections intentionally left empty / unused in template
      top_links=top_links,
      data_collections_href=data_collections_href,
      conformance_href=conformance_href,
      api_doc_href=api_doc_href,
      openapi_href=openapi_href
   )
