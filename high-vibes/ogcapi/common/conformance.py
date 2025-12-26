# ogcapi_conformance.py
# /conformance endpoint: returns OGC API conformance classes (JSON + HTML).

from flask import *
from ..utils import negotiate_format, html_response, pretty_json
from typing import List

bp = Blueprint("conformance", __name__)

CONFORMANCE_HTML = """
{% block content %}
  <div class="small top-links">
    {% for l in top_links %}
      <a href="{{ l.href }}">{{ l.title }}</a>{% if not loop.last %} · {% endif %}
    {% endfor %}
  </div>

  <div class="card">
    <p class="small">Conformance classes implemented by this server</p>

    <ul>
      {% for c in conformsTo %}
        <li><a href="{{ c }}">{{ c }}</a></li>
      {% endfor %}
    </ul>
  </div>
{% endblock %}
"""

DEFAULT_CONFORMANCE: List[str] = [
    "https://www.opengis.net/spec/ogcapi-dggs-1/1.0/conf/core",
    "https://www.opengis.net/spec/ogcapi-dggs-1/1.0/conf/data-retrieval",
    "https://www.opengis.net/spec/ogcapi-dggs-1/1.0/conf/data-custom-depths",
    "https://www.opengis.net/spec/ogcapi-dggs-1/1.0/conf/collection-dggs",
    "https://www.opengis.net/spec/ogcapi-dggs-1/1.0/conf/data-json",
    "https://www.opengis.net/spec/ogcapi-dggs-1/1.0/conf/data-ubjson"
]

@bp.route("/conformance", methods=["GET"])
def conformance():
    app = current_app._get_current_object() if hasattr(current_app, "_get_current_object") else current_app

    conforms = app.config.get("CONFORMANCE_CLASSES") or DEFAULT_CONFORMANCE

    html_self = url_for("conformance.conformance", _external=False)
    json_self = html_self + "?f=json"
    up_href = url_for("landing.root_index", _external=False)

    # HTML top bar
    top_links = [
        {"href": up_href, "title": "Back to landing page"},
        {"href": json_self, "title": "JSON"}
    ]

    # JSON links (NO landing page as alternate)
    links = [
        {"rel": "self", "type": "application/json", "href": json_self},
        {"rel": "alternate", "type": "text/html", "href": html_self},
        {"rel": "up", "href": up_href}
    ]

    payload = {
        "title": "DGGS API Conformance",
        "description": "Conformance declarations for this server",
        "links": links,
        "conformsTo": conforms
    }

    fmt, _ = negotiate_format(request, request.path)

    if fmt == "html":
        return html_response(
            CONFORMANCE_HTML,
            title="Conformance",
            conformsTo=conforms,
            top_links=top_links
        )

    return Response(pretty_json(payload) + "\n", mimetype="application/json; charset=utf-8")
