# DGGAL High Vibes

## A set of high-level vibe-coded tools demonstrating DGGAL usage in Python

### Example usage

#### dgg-import

Create a Scalable UBJSON DGGS Data Store from a raster (e.g., GeoTIFF) or GeoJSON (quantizing to a DGGRS)

```
dgg-import gebco.tiff --dggrs IVEA4R --fields Elevation
dgg-import countries.geojson --dggrs IVEA3H --level 19 --depth 14
```

#### dgg-fetch

Create a Scalable UBJSON DGGS Data Store from an OGC API - DGGS deployment (implementing an OGC API - DGGS client):

```
dgg-fetch http://localhost:8080/collections/gebco/dggs/IVEA4R
```

#### dgg-export

Export a GeoTIFF or GeoJSON from a Scalable UBJSON DGGS Data Store:

```
dgg-export data out.tif --collection gebco --level 10
dgg-export data out.geojson --collection countries --level 19
```

#### dgg-serve

Deploy an OGC API - DGGS interface to DGGS-quantized collections in Scalable UBJSON DGGS Data Stores through OGC API - DGGS with DGGAL High Vibes (implementation of an OGC API - DGGS server):

```
dgg-serve --data-root data --port 8080
```

### Limitations

This code should all work with the latest [0.0.7 DGGAL release](https://github.com/ecere/dggal/releases/tag/v0.0.7).

This is an early version with some limitations:
- the tools do not currently support additional dimensions (time, pressure...) but this is conceptually supported by DGGS-UBJSON and this DGGS Data Store layout,
- Vector stores are currently limited to icosahedral equal-area projections (IVEA, ISEA, RTEA) DGGRSs and have only been tested with IVEA3H and polygons so far,
- a future version will likely introduce a DGGS-JSON (field value) quantization extension significantly reducing data size.
