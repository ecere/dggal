# DGGAL High Vibes

## A set of high-level vibe-coded tools demonstrating DGGAL usage in Python

### Example usage

#### dgg-import

Create a Scalable UBJSON DGGS Data Store from a GeoTIFF (quantizing to a DGGRS)

```
python dgg-import.py gebco.tiff --dggrs IVEA4R --fields Elevation
```

#### dgg-fetch

Create a Scalable UBJSON DGGS Data Store from an OGC API - DGGS deployment (implementing an OGC API - DGGS client):

```
python dgg-fetch.py http://localhost:8080/collections/gebco/dggs/IVEA4R
```

#### dgg-export

Export a GeoTIFF from a Scalable UBJSON DGGS Data Store (relatively well optimized):

```
python dgg-export.py data out.tif --collection gebco --level 10
```

#### dgg-serve

Deploy an OGC API - DGGS interface to DGGS-quantized collections in Scalable UBJSON DGGS Data Stores through OGC API - DGGS with DGGAL High Vibes (implementation of an OGC API - DGGS server):

```
python dgg-serve.py --data-root data --port 8080
```

### Limitations

This code should all work with the last [0.0.6 DGGAL release](https://github.com/ecere/dggal/releases/tag/v0.0.6).

This is an early version with some limitations:
- the tools do not currently support additional dimensions (time, pressure...) but this is conceptually supported by DGGS-UBJSON and this DGGS Data Store layout,
- this is currently limited to raster (DGGS-JSON), but a future version should support DGGS-JSON-FG for a proper DGGS-quantized vector data store (not rasterising the vector data!) -- also supported by this DGGS Data Store layout,
- a future version will likely introduce a DGGS-JSON (field value) quantization extension significantly reducing data size.
