from flask import Blueprint, request, url_for, Response, current_app
import logging
from dggsStore.store import get_store
from ..utils import negotiate_format, html_response, pretty_json

logger = logging.getLogger("dgg-serve.dggrs")

bp = Blueprint("dggrs", __name__, url_prefix="/collections/<collectionId>/dggs")


def build_dggrs_description(store, collectionId, dggrsId):
    cfg = store.config
    collection_title = cfg.get("title", collectionId)

    desc_href = f"/collections/{collectionId}/dggs/{dggrsId}"
    def_href = f"https://www.opengis.net/def/dggrs/OGC/1.0/{dggrsId}"
    collection_href = f"/collections/{collectionId}"
    zones_href = f"/collections/{collectionId}/dggs/{dggrsId}/zones"

    top_links = [
        {"rel": "self", "title": "Description", "href": desc_href},
        {"rel": "[ogc:geodata]", "title": "Collection", "href": collection_href},
    ]

    mid_links = [
        {"rel": "[ogc:dggrs-definition]", "title": "DGGRS Definition", "href": def_href},
        {"rel": "[ogc:dggrs-zone-query]", "title": "Zones", "href": zones_href},
    ]

    json_links = top_links + mid_links

    return {
        "id": dggrsId,
        "title": f"{dggrsId} DGGRS for {collection_title}",
        "uri": def_href,
        "defaultDepth": store.depth,
        "maxRefinementLevel": store.maxRefinementLevel,
        "links": json_links,
        "top_links": top_links,
        "mid_links": mid_links,
        "linkTemplates": [
            {
                "rel": "[ogc:dggrs-zone-data]",
                "title": f"{dggrsId} data for {collectionId}",
                "uriTemplate": f"/collections/{collectionId}/dggs/{dggrsId}/zones/{{zoneId}}/data"
            }
        ]
    }


DGGRS_LIST_HTML = """
{% block content %}
  <div class="small top-links">
    {% for l in top_links %}
      <a href="{{ l.href }}">{{ l.title }}</a>{% if not loop.last %} · {% endif %}
    {% endfor %}
  </div>

  <div class="card"><h2>Available DGGRSs</h2></div>

  {% for g in dggrs %}
    <div class="card">
      <h3><a href="{{ g.links[0].href }}">{{ g.title }}</a></h3>

      <div class="small">
        {% for l in g.mid_links %}
          <a href="{{ l.href }}">{{ l.title }}</a>{% if not loop.last %} · {% endif %}
        {% endfor %}
      </div>

      <div class="small">
        <strong>Zone Data Retrieval Link Template:</strong><br>
        {% for t in g.linkTemplates %}
          <code>{{ t.uriTemplate }}</code>{% if not loop.last %} · {% endif %}
        {% endfor %}
      </div>
    </div>
  {% endfor %}
{% endblock %}
"""

DGGRS_HTML = """
{% block content %}
  <div class="small top-links">
    {% for l in top_links %}
      <a href="{{ l.href }}">{{ l.title }}</a>{% if not loop.last %} · {% endif %}
    {% endfor %}
  </div>

  <div class="card">
    <h2>{{ dggrs.title }}</h2>

    <div class="small">
      {% for l in mid_links %}
        <a href="{{ l.href }}">{{ l.title }}</a>{% if not loop.last %} · {% endif %}
      {% endfor %}
    </div>

    <div class="small">
      <strong>Zone Data Retrieval Link Template:</strong><br>
      {% for t in dggrs.linkTemplates %}
        <code>{{ t.uriTemplate }}</code>{% if not loop.last %} · {% endif %}
      {% endfor %}
    </div>
  </div>
{% endblock %}
"""


@bp.route("", methods=["GET"])
def list_collection_dggrs(collectionId: str):
    try:
        DATA_ROOT = current_app.config.get("DATA_ROOT")
        store = get_store(DATA_ROOT, collectionId) if DATA_ROOT else None
        if not store:
            return Response(
                pretty_json({"error": "Collection not found"}) + "\n",
                status=404,
                mimetype="application/json; charset=utf-8",
            )

        cfg = store.config
        dggrs_id = cfg.get("dggrs")
        entries = []

        if dggrs_id:
            entries.append(build_dggrs_description(store, collectionId, dggrs_id))

        html_self = f"/collections/{collectionId}/dggs"
        json_self = html_self + "?f=json"
        html_alt = html_self + "?f=html"
        up_href = f"/collections/{collectionId}"

        fmt, _ = negotiate_format(request, request.path)

        if fmt == "json":
            payload = {
                "title": f"DGGRSs for collection {collectionId}",
                "links": [
                    {"rel": "self", "type": "application/json", "href": json_self},
                    {"rel": "alternate", "type": "text/html", "href": html_alt},
                    {"rel": "up", "href": up_href},
                ],
                "dggrs": [
                    {
                        "id": g["id"],
                        "title": g["title"],
                        "uri": g["uri"],
                        "defaultDepth": g["defaultDepth"],
                        "maxRefinementLevel": g["maxRefinementLevel"],
                        "links": g["links"],
                        "linkTemplates": g["linkTemplates"],
                    }
                    for g in entries
                ],
            }
            return Response(
                pretty_json(payload) + "\n",
                mimetype="application/json; charset=utf-8",
            )

        html_entries = []
        for g in entries:
            html_entries.append(g)

        top_links = [
            {"href": up_href, "title": "Back to collection"},
            {"href": json_self, "title": "JSON"},
        ]

        return html_response(
            DGGRS_LIST_HTML,
            title=f"DGGRSs for collection {collectionId}",
            collectionId=collectionId,
            dggrs=html_entries,
            top_links=top_links,
        )

    except Exception:
        logger.exception("list_collection_dggrs failed for %s", collectionId)
        return Response(
            pretty_json({"error": "Internal server error"}) + "\n",
            status=500,
            mimetype="application/json; charset=utf-8",
        )

@bp.route("/<dggrsId>", methods=["GET"])
def get_dggrs(collectionId: str, dggrsId: str):
    try:
        DATA_ROOT = current_app.config.get("DATA_ROOT")
        store = get_store(DATA_ROOT, collectionId) if DATA_ROOT else None
        if not store:
            return Response(
                pretty_json({"error": "Not found"}) + "\n",
                status=404,
                mimetype="application/json; charset=utf-8",
            )

        cfg = store.config
        if cfg.get("dggrs") != dggrsId:
            return Response(
                pretty_json({"error": "Not found"}) + "\n",
                status=404,
                mimetype="application/json; charset=utf-8",
            )

        payload = build_dggrs_description(store, collectionId, dggrsId)
        desc_href = f"/collections/{collectionId}/dggs/{dggrsId}"

        fmt, _ = negotiate_format(request, request.path)

        if fmt == "json":
            json_payload = {
                "id": payload["id"],
                "title": payload["title"],
                "uri": payload["uri"],
                "defaultDepth": payload["defaultDepth"],
                "maxRefinementLevel": payload["maxRefinementLevel"],
                "links": payload["links"],
                "linkTemplates": payload["linkTemplates"],
            }
            return Response(
                pretty_json(json_payload) + "\n",
                mimetype="application/json; charset=utf-8",
            )

        top_links = [
            {"href": f"/collections/{collectionId}/dggs", "title": "Back to DGGRSs"},
            {"href": f"/collections/{collectionId}", "title": "Back to collection"},
            {"href": desc_href + "?f=json", "title": "JSON"},
        ]

        return html_response(
            DGGRS_HTML,
            title=f"DGGRS {dggrsId}",
            dggrs=payload,
            collectionId=collectionId,
            top_links=top_links,
            mid_links=payload["mid_links"],
        )

    except Exception:
        logger.exception("get_dggrs failed for %s/%s", collectionId, dggrsId)
        return Response(
            pretty_json({"error": "Internal server error"}) + "\n",
            status=500,
            mimetype="application/json; charset=utf-8",
        )
