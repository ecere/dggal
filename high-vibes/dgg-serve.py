#!/usr/bin/env python3
# dgg-serve.py
# Main entrypoint that composes the smaller ogcapi-* modules into a working Flask app.

import os
import logging
import argparse
import atexit
from flask import Flask
from dggal import *
import signal
import traceback

app = Application(appGlobals=globals()); pydggal_setup(app)

# Configure logging
logging.basicConfig(level=logging.INFO, format="%(asctime)s %(levelname)s %(message)s")
logger = logging.getLogger("dgg-serve")

# Import the modular blueprints
from ogcapi.common.landingPage import bp as landing_bp
from ogcapi.common.conformance import bp as conformance_bp
from ogcapi.common.collections import bp as collections_bp
from ogcapi.dggs.dggrs import bp as dggrs_bp
from ogcapi.dggs.zones import bp as dggs_zones_bp
from ogcapi.dggs.zoneInfo import bp as dggs_zoneinfo_bp
from ogcapi.dggs.zoneData import bp as dggs_zoneData_bp

# Import store lifecycle helpers so we can close stores on shutdown
from dggsStore.store import close_all_stores

# Create Flask app and register blueprints
def create_app(data_root: str, dggrs_schema_uri: str = None) -> Flask:
    app = Flask(__name__)
    # Configuration
    app.config["DATA_ROOT"] = data_root
    if dggrs_schema_uri:
        app.config["DGGS_JSON_SCHEMA_URI"] = dggrs_schema_uri
    else:
        app.config["DGGS_JSON_SCHEMA_URI"] = "https://schemas.opengis.net/ogcapi/dggs/part1/1.0/openapi/schemas/dggrs-json/schema"

    # Register blueprints (they include their own url_prefix where appropriate)
    app.register_blueprint(landing_bp)            # root "/"
    app.register_blueprint(conformance_bp)       # "/conformance"
    app.register_blueprint(collections_bp)       # "/collections"
    app.register_blueprint(dggrs_bp)             # "/collections/<collectionId>/dggs"
    app.register_blueprint(dggs_zones_bp)        # "/collections/<collectionId>/dggs/<dggrsId>"
    app.register_blueprint(dggs_zoneinfo_bp)     # "/collections/<collectionId>/dggs/<dggrsId>"
    app.register_blueprint(dggs_zoneData_bp)     # "/collections/<collectionId>/dggs/<dggrsId>"

    # Simple CORS and Vary headers for all responses
    @app.after_request
    def _add_cors_and_vary(resp):
        resp.headers.setdefault("Access-Control-Allow-Origin", "*")
        resp.headers.setdefault("Access-Control-Allow-Methods", "GET,OPTIONS,POST")
        resp.headers.setdefault("Access-Control-Allow-Headers", "Content-Type,Accept,Accept-Encoding")
        vary = resp.headers.get("Vary", "")
        vary_set = set([v.strip() for v in (vary.split(",") if vary else []) if v.strip()])
        vary_set.update(["Accept", "Accept-Encoding"])
        resp.headers["Vary"] = ", ".join(sorted(vary_set))
        return resp

    return app

def parse_args():
    p = argparse.ArgumentParser(prog="dgg-serve", description="OGC API - DGGS server (modular)")
    p.add_argument("--data-root", help="Data root directory (default: ./data)", default=os.path.join(os.getcwd(), "data"))
    p.add_argument("--host", help="Host to bind", default="0.0.0.0")
    p.add_argument("--port", help="Port to listen on", type=int, default=int(os.environ.get("PORT", "8080")))
    p.add_argument("--debug", help="Enable debug mode", action="store_true")
    p.add_argument("--schema-uri", help="DGGS JSON schema URI override", default=None)
    return p.parse_args()

def dump_traces_and_exit(signum, frame):
    for tid, fr in sys._current_frames().items():
        print(f"\n--- Thread {tid} ---", file=sys.stderr)
        traceback.print_stack(fr, file=sys.stderr)
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    raise KeyboardInterrupt

def main():
    args = parse_args()
    DATA_ROOT = args.data_root
    if not os.path.isdir(DATA_ROOT):
        print(f"ERROR: DATA_ROOT directory does not exist: {DATA_ROOT!r}")
        return
    logger.info("Starting dgg-serve on %s:%d with data root: %s", args.host, args.port, DATA_ROOT)
    app = create_app(DATA_ROOT, dggrs_schema_uri=args.schema_uri)
    atexit.register(lambda: (logger.info("Closing all stores..."), close_all_stores()))
    signal.signal(signal.SIGINT, dump_traces_and_exit)
    app.run(host=args.host, port=args.port, debug=args.debug, use_reloader=False, threaded=True)

if __name__ == "__main__":
    main()
