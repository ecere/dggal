# fg/wkbc.py
# Write a WKB Collection (WKBC) file using Shapely to produce WKB geometry bytes.
# Header (per spec): Endianness (1 byte), Coordinate Type (1 byte), Geometry Type (2 bytes), Feature Count (4 bytes)
# Followed by feature lookup table: (feature_id:uint64, offset:uint64) * N
# Followed by concatenated WKB geometry blobs (big-endian WKB produced by Shapely).

import struct
from typing import Dict, Any, List
from shapely.geometry import shape, mapping
from shapely import wkb

# Geometry-type codes per WKBC spec (1: point, 2: linestring, 3: polygon)
# Use the most specific code that represents the collection; if mixed, use 0.
_GEOM_CODE_POINT = 1
_GEOM_CODE_LINE = 2
_GEOM_CODE_POLY = 3

def _be_uint8(v: int) -> bytes:
   return struct.pack(">B", v)

def _be_uint16(v: int) -> bytes:
   return struct.pack(">H", v)

def _be_uint32(v: int) -> bytes:
   return struct.pack(">I", v)

def _be_uint64(v: int) -> bytes:
   return struct.pack(">Q", v)

def _detect_coord_type_and_geom_code(features: List[Dict[str, Any]]):
   any_z = False
   geom_kind = None  # one of 'point','line','poly' or None/mixed
   for f in features:
      g = f.get("geometry")
      if not g:
         continue
      shp = shape(g)
      # detect Z by inspecting geometry coordinates via shapely
      # shapely geometries expose has_z
      if getattr(shp, "has_z", False):
         any_z = True
      t = g.get("type")
      if t in ("Point", "MultiPoint"):
         kind = "point"
      elif t in ("LineString", "MultiLineString"):
         kind = "line"
      elif t in ("Polygon", "MultiPolygon"):
         kind = "poly"
      else:
         kind = "mixed"
      if geom_kind is None:
         geom_kind = kind
      elif geom_kind != kind:
         geom_kind = "mixed"
   coord_byte = 0
   if any_z:
      coord_byte |= 1  # Z bit
   # M not handled here (leave bit 0)
   if geom_kind == "point":
      geom_code = _GEOM_CODE_POINT
   elif geom_kind == "line":
      geom_code = _GEOM_CODE_LINE
   elif geom_kind == "poly":
      geom_code = _GEOM_CODE_POLY
   else:
      geom_code = 0  # mixed/unknown
   return coord_byte, geom_code

def write_wkb_collection_file(fc: Dict[str, Any], path: str) -> None:
   features = fc.get("features", []) or []
   count = len(features)

   coord_byte, geom_code = _detect_coord_type_and_geom_code(features)

   # Header: endianness (1 byte, 0 = big), coord type (1 byte), geom type (2 bytes), feature count (4 bytes)
   header = _be_uint8(0) + _be_uint8(coord_byte) + _be_uint16(geom_code) + _be_uint32(count)

   # Build WKB blobs using Shapely (big-endian). Use output_dimension=3 if geometry has Z.
   geom_blobs: List[bytes] = []
   for f in features:
      g = f.get("geometry")
      if not g:
         geom_blobs.append(b"")
         continue
      shp = shape(g)
      dim = 3 if getattr(shp, "has_z", False) else 2
      # shapely.wkb.dumps supports byte_order and output_dimension in modern versions
      # byte_order=0 -> big-endian
      geom_wkb = wkb.dumps(shp, output_dimension=dim, byte_order=0)
      geom_blobs.append(geom_wkb)

   # Compute lookup table offsets (offsets are from start of file)
   lookup_entry_size = 8 + 8  # feature_id:uint64 + offset:uint64
   lookup_table_size = count * lookup_entry_size
   offset_base = len(header) + lookup_table_size

   offsets: List[int] = []
   cur = offset_base
   for b in geom_blobs:
      offsets.append(cur)
      cur += len(b)

   # Write file: header, lookup table, geometries
   with open(path, "wb") as fh:
      fh.write(header)
      for i, f in enumerate(features):
         fid = int(f.get("id", 0))
         off = offsets[i]
         fh.write(_be_uint64(fid))
         fh.write(_be_uint64(off))
      for b in geom_blobs:
         fh.write(b)

# Read a WKBC file written by write_wkb_collection_file and return a GeoJSON-like FeatureCollection
def read_wkb_collection_file(path: str) -> Dict[str, Any]:
   fh = open(path, "rb")
   # header: endianness (1), coord type (1), geom type (2), feature count (4) -- big-endian per spec
   hdr = fh.read(8)
   if len(hdr) < 8:
      fh.close()
      return {"type": "FeatureCollection", "features": []}
   # unpack big-endian
   endianness = struct.unpack(">B", hdr[0:1])[0]
   coord_type = struct.unpack(">B", hdr[1:2])[0]
   geom_code = struct.unpack(">H", hdr[2:4])[0]
   feature_count = struct.unpack(">I", hdr[4:8])[0]

   # read lookup table: feature_id:uint64, offset:uint64
   lookup: List[tuple] = []
   for i in range(feature_count):
      entry = fh.read(16)
      if len(entry) < 16:
         break
      fid = struct.unpack(">Q", entry[0:8])[0]
      off = struct.unpack(">Q", entry[8:16])[0]
      lookup.append((int(fid), int(off)))

   # read geometry blobs using offsets
   geom_blobs: List[tuple] = []
   for i, (fid, off) in enumerate(lookup):
      fh.seek(off)
      if i + 1 < len(lookup):
         next_off = lookup[i + 1][1]
         length = next_off - off
         geom_bytes = fh.read(length)
      else:
         geom_bytes = fh.read()
      geom_blobs.append((fid, geom_bytes))

   fh.close()

   # convert WKB bytes to GeoJSON geometry using shapely
   features: List[Dict[str, Any]] = []
   for fid, gb in geom_blobs:
      if not gb:
         features.append({"type": "Feature", "id": int(fid), "properties": None, "geometry": None})
         continue
      shp = wkb.loads(bytes(gb))
      geojson_geom = mapping(shp)
      features.append({"type": "Feature", "id": int(fid), "properties": None, "geometry": geojson_geom})

   return {"type": "FeatureCollection", "features": features}
