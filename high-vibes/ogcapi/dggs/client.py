# ogcapi/dggs/client.py
from typing import List, Dict, Any, Optional, Tuple
from concurrent.futures import ThreadPoolExecutor, as_completed
from urllib.parse import urlparse
import requests

JSON_MIME = "application/json"
# UBJSON_MIME = "application/ub+json"
# GZIP_HEADER = "gzip"


def parse_resource_url(resource_url: str) -> Tuple[str, str, str]:
   """
   Parse a resource URL like:
     http://host/.../ogcapi/collections/{collectionId}/dggs/{dggrsId}
   Returns (landing_base, collectionId, dggrsId).
   """
   p = urlparse(resource_url)
   parts = p.path.strip("/").split("/")
   ci = parts.index("collections")
   collectionId = parts[ci + 1]
   di = parts.index("dggs")
   dggrsId = parts[di + 1]
   landing_base = f"{p.scheme}://{p.netloc}"
   if "ogcapi" in parts:
      oi = parts.index("ogcapi")
      landing_base = f"{landing_base}/{'/'.join(parts[:oi+1])}"
   return landing_base, collectionId, dggrsId


def get_collection_info(landing: str, collectionId: str, timeout: int = 30) -> Dict[str, Any]:
   """
   GET /collections/{collectionId} and return parsed JSON.
   """
   url = f"{landing.rstrip('/')}/collections/{collectionId}"
   r = requests.get(url, headers={"Accept": JSON_MIME}, timeout=timeout)
   r.raise_for_status()
   return r.json()


def get_dggrs_description(landing: str, collectionId: str, dggrsId: str, timeout: int = 30) -> Dict[str, Any]:
   """
   GET /collections/{collectionId}/dggs/{dggrsId} and return parsed JSON.
   """
   url = f"{landing.rstrip('/')}/collections/{collectionId}/dggs/{dggrsId}"
   r = requests.get(url, headers={"Accept": JSON_MIME}, timeout=timeout)
   r.raise_for_status()
   return r.json()


def _zone_data_url(landing: str, collectionId: str, dggrsId: str, zoneId: str) -> str:
   return f"{landing.rstrip('/')}/collections/{collectionId}/dggs/{dggrsId}/zones/{zoneId}/data"


def fetch_zone_data_parallel(landing: str,
                             collectionId: str,
                             dggrsId: str,
                             zone_texts: List[str],
                             workers: int = 8,
                             timeout: int = 30,
                             depth: Optional[int] = None,
                             use_ubjson: bool = False) -> Dict[str, Any]:
   """
   Fetch zone data objects in parallel.
   Returns a dict mapping zone_text -> parsed object.
   By default requests application/json. To request UBJSON+gzip set use_ubjson=True
   and ensure server supports it (UBJSON handling is commented below).
   """
   headers = {
      "Accept": JSON_MIME,
      "User-Agent": "dgg-fetch/1.0"
   }

   # If you want UBJSON + gzip from servers that support it, uncomment:
   # headers = {
   #    "Accept": UBJSON_MIME,
   #    "Accept-Encoding": GZIP_HEADER,
   #    "User-Agent": "dgg-fetch/1.0"
   # }

   params = {}
   if depth is not None:
      params["zone-depth"] = depth

   def _fetch(ztext: str):
      url = _zone_data_url(landing, collectionId, dggrsId, ztext)
      r = requests.get(url, params=params, headers=headers, timeout=timeout)
      r.raise_for_status()
      # If UBJSON response handling is desired and server returns UBJSON bytes:
      # if use_ubjson:
      #    import gzip, ubjson
      #    data = r.content
      #    # if server sent gzip, decompress:
      #    # data = gzip.decompress(data)
      #    return ztext, ubjson.loadb(data)
      return ztext, r.json()

   out: Dict[str, Any] = {}
   with ThreadPoolExecutor(max_workers=workers) as ex:
      futures = [ex.submit(_fetch, z) for z in zone_texts]
      for fut in as_completed(futures):
         ztext, obj = fut.result()
         out[ztext] = obj
   return out
