# ogcapi_utils.py
# Shared helpers: base template, media negotiation, link helpers, html renderer

import os
import json
from typing import Iterable
from flask import current_app, request, render_template_string, Response

BASE_HTML = """<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>{{ title or "DGGS API" }}</title>
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <style>
    body{font-family:system-ui,Segoe UI,Roboto,Arial;margin:18px;color:#222;background:#f6f7fb}
    header{margin-bottom:12px}
    .card{border:1px solid #e1e4ea;padding:12px;margin:8px 0;border-radius:6px;background:#fff}
    .meta{color:#666;font-size:0.9em}
    pre.json{background:#f7f7f9;padding:8px;border-radius:4px;overflow:auto}
    a{color:#0366d6;text-decoration:none}
    a:hover{text-decoration:underline}
    .small{font-size:0.9em;color:#555}
    .grid{display:grid;grid-template-columns:1fr;gap:10px}
    @media(min-width:800px){ .grid{grid-template-columns:repeat(2,1fr)} }
    .format-links{margin-top:8px}
    .format-links a{margin-right:10px}
  </style>
</head>
<body>
  <header><h1>{{ title or "DGGS API" }}</h1></header>
  <main>{% block content %}{% endblock %}</main>
</body>
</html>
"""

def data_root():
    return current_app.config.get("DATA_ROOT", os.getcwd())

def negotiate_format(req, path=None):
    # Decide whether the client wants HTML or JSON.
    p = (path or req.path or "").lower()

    if p.endswith(".json"):      return "json",       False
    if p.endswith(".ubjson"):    return "ubjson",     False
    if p.endswith(".geojson"):   return "geojson",    False
    if p.endswith(".geoubjson"): return "geoubjson",  False

    f = (req.args.get("f") or "").lower()
    if f in ("json", "ubjson", "geojson", "geoubjson", "html"):
        fmt = f
    else:
        accept = (req.headers.get("Accept") or "").lower()
        if "text/html" in accept: fmt = "html"
        elif "application/json"        in accept: fmt = "json"
        elif "application/ubjson"      in accept: fmt = "ubjson"
        elif "application/geo+json"    in accept: fmt = "geojson"
        elif "application/geo+ubjson"  in accept: fmt = "geoubjson"
        else:                                     fmt = "json"

    ae = (req.headers.get("Accept-Encoding") or "").lower()
    gzip_ok = "gzip" in ae or "*" in ae

    return fmt, gzip_ok

GEOJSON_PROFILES = {"rfc7946", "jsonfg", "jsonfg-plus"}
DGGSFG_PROFILES = {"jsonfg-dggs", "jsonfg-dggs-plus", "jsonfg-dggs-zoneids", "jsonfg-dggs-zoneids-plus"}

def negotiate_profile(request, fmt, default_profile=None):
   # collect profile tokens from Accept-Profile header and profile query param
   profile_tokens = []
   accept_profile_hdr = request.headers.get("Accept-Profile", "")
   if accept_profile_hdr:
      for part in accept_profile_hdr.split(","):
         token = part.split(";", 1)[0].strip()
         if token:
            profile_tokens.append(token)
   profile_q = request.args.get("profile", "")
   if profile_q:
      for part in profile_q.split(","):
         token = part.split(";", 1)[0].strip()
         if token:
            profile_tokens.append(token)

   profile = default_profile
   if profile_tokens:
      if fmt == "geojson" or fmt == "geoubjson":
         if any(profile_token_matches(tok, DGGSFG_PROFILES) for tok in profile_tokens):
            profile = "jsonfg-dggs"
         elif any(profile_token_matches(tok, GEOJSON_PROFILES) for tok in profile_tokens):
            profile = "rfc7946"

   return profile

def make_link(href, rel=None, title=None, type=None):
    # Create a link object. Nothing more.
    # You decide where it goes (JSON, HTML top, HTML bottom).
    link = {"href": href}
    if rel:
        link["rel"] = rel
    if title:
        link["title"] = title
    if type:
        link["type"] = type
    return link


def label_for_link(link):
    return link.get("title") or link.get("rel") or "link"


def html_response(body_template: str, **ctx):
    # Render HTML using the base template.
    full = BASE_HTML.replace("{% block content %}{% endblock %}", body_template)
    return Response(
        render_template_string(full, base=BASE_HTML, **ctx),
        mimetype="text/html; charset=utf-8"
    )

# ----------------------------------------------------------------------
# JSON pretty printer with compact arrays
# ----------------------------------------------------------------------

def _is_primitive(v):
    return v is None or isinstance(v, (str, bool, int, float))

def _serialize_primitive(v):
    return json.dumps(v, ensure_ascii=False)

def pretty_json(obj, indent=0, indent_step=3):
    #Pretty-print JSON with:
    #- indentation
    #- arrays of primitives on one line
    #- nested arrays/objects expanded
    sp = " " * indent
    next_sp = " " * (indent + indent_step)

    if _is_primitive(obj):
        return _serialize_primitive(obj)

    if isinstance(obj, list):
        if not obj:
            return "[]"
        if all(_is_primitive(x) for x in obj):
            inner = ", ".join(_serialize_primitive(x) for x in obj)
            return f"[{inner}]"
        parts = [
            f"{next_sp}{pretty_json(x, indent + indent_step, indent_step)}"
            for x in obj
        ]
        return "[\n" + ",\n".join(parts) + "\n" + sp + "]"

    if isinstance(obj, dict):
        if not obj:
            return "{}"
        parts = []
        for k, v in obj.items():
            key = json.dumps(k, ensure_ascii=False)
            val = pretty_json(v, indent + indent_step, indent_step)
            parts.append(f"{next_sp}{key}: {val}")
        return "{\n" + ",\n".join(parts) + "\n" + sp + "}"

    return json.dumps(obj, ensure_ascii=False)

def profile_token_matches(token: str, short_names: Iterable[str]) -> bool:
   # Return True when the profile token matches any of the provided short_names.
   # Supported token forms:
   # - short name (e.g., "jsonfg-dggs")
   # - CURIE form "ogc-profile:NAME"
   # - full URI forms "https://www.opengis.net/def/profile/ogc/0/NAME"
   #  and "http://www.opengis.net/def/profile/ogc/0/NAME"
   if not token:
      return False
   tok = token.strip()
   if not tok:
      return False

   # direct short-name match
   if tok in short_names:
      return True

   # CURIE form ogc-profile:NAME
   if tok.startswith("ogc-profile:"):
      name = tok.split(":", 1)[1]
      if name in short_names:
         return True

   # full URI forms
   for prefix in ("https://www.opengis.net/def/profile/ogc/0/",
                  "http://www.opengis.net/def/profile/ogc/0/"):
      if tok.startswith(prefix):
         name = tok[len(prefix):]
         if name in short_names:
            return True

   return False
