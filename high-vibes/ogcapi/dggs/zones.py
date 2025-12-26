# ogcapi-dggs-zones.py
# DGGRS zones listing endpoint:
#   GET /collections/<collectionId>/dggs/<dggrsId>/zones

from flask import Blueprint, request, Response, current_app
import logging

from dggsStore.store import get_store
from ..utils import pretty_json, html_response

ZONE_QUERY_LEVEL = 2

logger = logging.getLogger("dgg-serve.zones")

bp = Blueprint(
    "dggs_zones",
    __name__,
    url_prefix="/collections/<collectionId>/dggs/<dggrsId>"
)

ZONES_HTML = """
{% block content %}

  <div class="small top-links">
    {% for l in top_links %}
      <a href="{{ l.href }}">{{ l.title }}</a>{% if not loop.last %} · {% endif %}
    {% endfor %}
  </div>

  <div class="card">
    <div class="small">
      {% for l in mid_links %}
        <a href="{{ l.href }}">{{ l.title }}</a>{% if not loop.last %} · {% endif %}
      {% endfor %}
    </div>

    <div class="small">
      <strong>Zone Data Retrieval Link Template:</strong><br>
      {% for t in linkTemplates %}
        <code>{{ t.uriTemplate }}</code>{% if not loop.last %} · {% endif %}
      {% endfor %}
    </div>
  </div>

  <div class="card">
    <h2>Zones ({{ zones|length }} items)</h2>
    <ul>
      {% for z in zones %}
        <li>
          <a href="/collections/{{ collectionId }}/dggs/{{ dggrsId }}/zones/{{ z }}">
            {{ z }}
          </a>
        </li>
      {% endfor %}
    </ul>
  </div>

{% endblock %}
"""

@bp.route("/zones")
def list_zones(collectionId, dggrsId):
   DATA_ROOT = current_app.config.get("DATA_ROOT")

   store = get_store(DATA_ROOT, collectionId)
   if store is not None:
      dggrs = store.dggrs

      # DGGS level is fixed for now
      level = ZONE_QUERY_LEVEL

      # Get zones as TEXT IDs
      zones = store.list_zones_with_data_at_level(level, as_textIDs=True)

      # Base href (no ?f=)
      zones_href = f"/collections/{collectionId}/dggs/{dggrsId}/zones"

      # Typed links (explicit representations)
      zones_href_json = zones_href + "?f=json"
      zones_href_html = zones_href + "?f=html"

      #
      # JSON REPRESENTATION
      #
      if request.args.get("f") == "json":
         payload = {
            "title": f"{dggrsId} DGGRS Zones for {store.config.get('title', collectionId)}",

            # Typed links → MUST include type + ?f=
            "links": [
               {
                  "rel": "self",
                  "title": "Zones (JSON)",
                  "href": zones_href_json,
                  "type": "application/json"
               },
               {
                  "rel": "alternate",
                  "title": "Zones (HTML)",
                  "href": zones_href_html,
                  "type": "text/html"
               }
            ],

            # URI templates (NOT real links)
            "linkTemplates": [
               {
                  "rel": "[ogc-rel:dggrs-zone-data]",
                  "title": "Zone Data Retrieval Link Template",
                  "uriTemplate": f"/collections/{collectionId}/dggs/{dggrsId}/zones/{{zoneId}}/data"
               }
            ],

            "zones": zones
         }

         return Response(
            pretty_json(payload) + "\n",
            mimetype="application/json; charset=utf-8"
         )

      #
      # HTML REPRESENTATION
      #
      return html_response(
         ZONES_HTML,

         title=f"{dggrsId} DGGRS Zones for {store.config.get('title', collectionId)}",
         collectionId=collectionId,
         dggrsId=dggrsId,
         zones=zones,

         #
         # Untyped navigation links → MUST NOT use ?f=
         #
         top_links=[
            {"href": f"/collections/{collectionId}/dggs/{dggrsId}", "title": "Back to DGGRS"},
            {"href": f"/collections/{collectionId}", "title": "Back to collection"},
         ],

         #
         # Typed links for explicit representations
         #
         links=[
            {
               "rel": "self",
               "title": "Zones (HTML)",
               "href": zones_href_html,
               "type": "text/html"
            },
            {
               "rel": "alternate",
               "title": "Zones (JSON)",
               "href": zones_href_json,
               "type": "application/json"
            }
         ],

         #
         # URI templates (NOT real links)
         #
         linkTemplates=[
            {
               "rel": "[ogc-rel:dggrs-zone-data]",
               "title": "Zone Data Retrieval Link Template",
               "uriTemplate": f"/collections/{collectionId}/dggs/{dggrsId}/zones/{{zoneId}}/data"
            }
         ]
      )
   else:
      return Response(b"", status=404, mimetype=None)
