from dggal import *
from .distance import distance5x6, move5x6
from typing import Any, Dict, List, Tuple, Sequence
import math
from enum import IntEnum

def sgn(v: float) -> int:
    if v > 0: return 1
    if v < 0: return -1
    return 0

_DEFAULT_SUBDIV = 50 #300

check_eps = 1e-2

def _segment_near_pole_by_crs(p: Tuple[float, float], n: Tuple[float, float]) -> bool:
   # Proximity Checks
   # Geographic South Pole
   if n[0] < 2 + check_eps and n[1] > 3.5 - check_eps and n[1] < 3.5 + check_eps:
      return True
   if p[0] < 2 + check_eps and p[1] > 3.5 - check_eps and p[1] < 3.5 + check_eps:
      return True

   if n[1] > 3 - check_eps and n[0] > 1.5 - check_eps and n[0] < 1.5 + check_eps:
      return True
   if p[1] > 3 - check_eps and p[0] > 1.5 - check_eps and p[0] < 1.5 + check_eps:
      return True

   # Geographic North Pole
   if n[1] < check_eps and n[0] > 0.5 - check_eps and n[0] < 0.5 + check_eps:
      return True
   if p[1] < check_eps and p[0] > 0.5 - check_eps and p[0] < 0.5 + check_eps:
      return True

   if n[0] > 5 - check_eps and n[1] > 4.5 - check_eps and n[1] < 4.5 + check_eps:
      return True
   if p[0] > 5 - check_eps and p[1] > 4.5 - check_eps and p[1] < 4.5 + check_eps:
      return True

   # Straddle Checks (Catches crossing lines like (2,3) -> (2,4))
   eps = 1e-5
   min_x, max_x = min(p[0], n[0]), max(p[0], n[0])
   min_y, max_y = min(p[1], n[1]), max(p[1], n[1])

   # South Pole Line 1: x close to 2.0, y straddles 3.5
   if abs(p[0] - 2.0) < eps and abs(n[0] - 2.0) < eps:
      if min_y <= 3.5 <= max_y:
         return True

   # South Pole Line 2: y close to 3.0, x straddles 1.5
   if abs(p[1] - 3.0) < eps and abs(n[1] - 3.0) < eps:
      if min_x <= 1.5 <= max_x:
         return True

   # North Pole Line 1: y close to 0.0, x straddles 0.5
   if abs(p[1] - 0.0) < eps and abs(n[1] - 0.0) < eps:
      if min_x <= 0.5 <= max_x:
         return True

   # North Pole Line 2: x close to 5.0, y straddles 4.5
   if abs(p[0] - 5.0) < eps and abs(n[0] - 5.0) < eps:
      if min_y <= 4.5 <= max_y:
         return True

   return False

def _choose_twin_point_for_segment(p: Tuple[float, float], n: Tuple[float, float], eps: float = 0.05) -> Optional[Tuple[float, float]]:
   # Return the twin/pole point (in 5x6 CRS coords) relevant to segment p->n,
   # or None if no known twin is close enough.
   # eps is a loose proximity threshold in CRS units.

   # Known seam/twin coordinates (5x6 CRS)
   SOUTH_TWIN_A = (2.0, 3.5)   # main south seam
   SOUTH_TWIN_B = (1.5, 3.0)   # alternate south twin
   NORTH_TWIN_A = (0.5, 0.0)   # main north seam
   NORTH_TWIN_B = (5.0, 4.5)   # alternate north twin (wrap)

   candidates = [SOUTH_TWIN_A, SOUTH_TWIN_B, NORTH_TWIN_A, NORTH_TWIN_B]

   # Helper to compute distance squared between two points
   def _dist2(a, b):
      dx = float(a[0]) - float(b[0])
      dy = float(a[1]) - float(b[1])
      return dx*dx + dy*dy

   # Precompute bounding boxes and cross-tolerances for segment straddle check
   line_eps = 1e-5
   min_x, max_x = min(p[0], n[0]), max(p[0], n[0])
   min_y, max_y = min(p[1], n[1]), max(p[1], n[1])

   for cand in candidates:
      cx, cy = cand[0], cand[1]

      # 1. Structural Straddle Check: Check if candidate lies directly inside the segment line path
      # Vertical segment check (e.g. x is locked at 2.0, y spans across 3.5)
      if abs(p[0] - cx) < line_eps and abs(n[0] - cx) < line_eps:
         if min_y <= cy <= max_y:
            return cand

      # Horizontal segment check (e.g. y is locked at 3.0, x spans across 1.5)
      if abs(p[1] - cy) < line_eps and abs(n[1] - cy) < line_eps:
         if min_x <= cx <= max_x:
            return cand

      # 2. Proximity Fallback: Check if either endpoint itself is directly near the candidate point
      if _dist2(p, cand) <= eps*eps or _dist2(n, cand) <= eps*eps:
         return cand
   return None

def _interpolate_between_5x6(a: Tuple[float, float], b: Tuple[float, float], divs: int) -> List[Tuple[float, float]]:
   # Return intermediate points between a and b in 5x6 space using distance5x6/move5x6.
   # Excludes the start 'a' and excludes the end 'b'.
   # subdivision logic: divs parts -> returns divs-1 intermediate points).
   if divs <= 1:
      return []
   # compute displacement in 5x6 units from a to b
   d, *unused = distance5x6(Pointd(a[0], a[1]), Pointd(b[0], b[1]))
   out: List[Tuple[float, float]] = []
   for j in range(1, divs):
      factor = j / float(divs)
      dx = d.x * factor
      dy = d.y * factor
      moved = move5x6((a[0], a[1]), dx, dy, 1)
      out.append((float(moved.x), float(moved.y)))
   # note: this returns points that lie strictly between a and b; callers decide whether to include endpoints
   return out

# REVIEW: This function was only intended for 5x6 space
def _insert_intermediate_points_crs_segment(p: Tuple[float, float], n: Tuple[float, float],
   refine_wgs84 = None) -> List[Tuple[float, float]]:

   # always start with p
   out: List[Tuple[float, float]] = [(float(p[0]), float(p[1]))]

   near_pole = _segment_near_pole_by_crs(p, n)

   if not near_pole:
      # REVIEW: Improve on automatic refinement
      if True: #refine_wgs84:
         max_5x6_distance = 1e-3 #refine_wgs84
         d, *unused = distance5x6(Pointd(p[0], p[1]), Pointd(n[0], n[1]))
         md = abs(d.x) + abs(d.y)
         if md > max_5x6_distance:
            divs = int(floor(md / max_5x6_distance))
            #print("Interpolating between", p, "and", n, "distance:", d.x, ",", d.y, ",", md, "with divisions:", divs)
            inter = _interpolate_between_5x6(p, n, divs)
            out.extend(inter)
      return out

   divs = _DEFAULT_SUBDIV

   # choose the twin/pole point relevant to this segment
   twin = _choose_twin_point_for_segment(p, n)
   if twin is None:
      # fallback: if we couldn't pick a twin, fall back to the original single interpolation
      # (interpolate from p toward n, excluding n)
      inter = _interpolate_between_5x6(p, n, divs)
      out.extend(inter)
      return out

   # If twin equals p or is extremely close, skip first interpolation
   eps_close = 0.1 # 1e-12 # was causing small gaps near south pole with IVEA4R
   if not (abs(twin[0] - p[0]) <= eps_close and abs(twin[1] - p[1]) <= eps_close):
      # interpolate from p toward twin (exclude endpoints)
      seg1 = _interpolate_between_5x6(p, twin, divs)
      # seg1 excludes p and twin; append them in order: seg1 then twin
      out.extend(seg1)

   # explicitly add the twin point (the seam/twin)
   out.append((float(twin[0]), float(twin[1])))

   # If twin equals n or extremely close, we're done (do not add n)
   if abs(twin[0] - n[0]) <= eps_close and abs(twin[1] - n[1]) <= eps_close:
      return out

   # interpolate from twin toward n (exclude endpoints)
   seg2 = _interpolate_between_5x6(twin, n, divs)
   out.extend(seg2)

   return out

def intersects_extent_deg(a: Sequence[float], b: Sequence[float], deg_epsilon: float = 1e-12) -> bool:
    # Test intersection of axis-aligned geographic extents in degrees.
    # Each extent is (xmin, ymin, xmax, ymax). Handles dateline-crossing extents
    # by splitting them into two normal extents.
    axmin, aymin, axmax, aymax = float(a[0]), float(a[1]), float(a[2]), float(a[3])
    bxmin, bymin, bxmax, bymax = float(b[0]), float(b[1]), float(b[2]), float(b[3])

    # this extent crosses the dateline (xmin > xmax)
    if axmin > axmax:
        a1 = (axmin, aymin, 180.0, aymax)
        a2 = (-180.0, aymin, axmax, aymax)
        return intersects_extent_deg(a1, b, deg_epsilon) or intersects_extent_deg(a2, b, deg_epsilon)

    # other extent crosses the dateline
    if bxmin > bxmax:
        b1 = (bxmin, bymin, 180.0, bymax)
        b2 = (-180.0, bymin, bxmax, bymax)
        return intersects_extent_deg(a, b1, deg_epsilon) or intersects_extent_deg(a, b2, deg_epsilon)

    # simple axis-aligned overlap test in degrees with tiny epsilon
    return (
        aymin < bymax - deg_epsilon
        and bymin < aymax - deg_epsilon
        and axmin < bxmax - deg_epsilon
        and bxmin < axmax - deg_epsilon
    )

def _snap_polar_vertex(lon: float, lat: float, sz_dlon: float, polar_cap_limit: float) -> Optional[Tuple[float, float]]:
   # Standalone top-level helper to evaluate and snap a vertex to polar boundaries.
   norm_lon = lon if lon <= 180.0 else lon - 360.0

   quad_idx = int((norm_lon + 180.0) // 90.0)
   if quad_idx < 0: quad_idx = 0
   if quad_idx > 3: quad_idx = 3

   quad_left = -180.0 + (quad_idx * 90.0)
   quad_right = quad_left + 90.0

   # 1. Absolute Polar Cap Baseline Expansion
   if lat < 0 and (90.0 + lat < polar_cap_limit):
      return quad_left, -90.0
   elif lat > 0 and (90.0 - lat < polar_cap_limit):
      return quad_left, 90.0

   # 2. Parent Root Zone Vertical Edge Snapping Pass (CLZ Bit-Length Optimization)
   rad_lat = lat * Pi / 180.0
   cos_factor = max(1e-10, math.cos(rad_lat))
   v = int(math.ceil(1.0 / cos_factor))

   # find next power of 2, applying required * 2 adjustment factor
   coalesce_mult = (1 << (v - 1).bit_length()) * 2 if v > 1 else 2

   row_dlon = sz_dlon * coalesce_mult
   snap_window = row_dlon * 0.51

   # Direct check against the absolute 180 antimeridian line boundary limits
   if abs(abs(norm_lon) - 180.0) <= snap_window or abs(abs(lon) - 180.0) <= snap_window:
      left_match = (abs(quad_left) == 180.0)
      right_match = (abs(quad_right) == 180.0)
   else:
      left_match = abs(norm_lon - quad_left) <= snap_window or abs(norm_lon - (quad_left + 360.0)) <= snap_window
      right_match = abs(norm_lon - quad_right) <= snap_window or abs(norm_lon - (quad_right - 360.0)) <= snap_window

   if left_match:
      return quad_left, lat
   elif right_match:
      return quad_right, lat

   return None


def _expand_polar_cap_vertices(out_coords: List[Tuple[float, float]], start_lon: float, snap_lat: float) -> None:
   # Generates a fixed set of intermediate vertices along the polar cap boundary.
   # Injects: Start, +4, +20, Midpoint (+45), End - 20, End - 4, and End.
   offsets = [0.0, 4.0, 20.0, 45.0, 70.0, 86.0, 90.0]

   for offset in offsets:
      out_coords.append((start_lon + offset, snap_lat))

class PolarRootMode(IntEnum):
   NONE = 0
   GGG = 1
   HEALPIX = 2
   RI5x6 = 3
   RHEALPIX = 4

def _unfold_healpix_pole_shelf(lon: float, lat: float, out_coords: List[Tuple[float, float]]) -> None:
   # Instantly unfolds a single polar vertex into two explicit horizontal
   # boundaries based on deterministic CCW polygon winding rules.
   is_north = (lat > 0)
   snap_lat = 90.0 if is_north else -90.0
   norm_lon = lon if lon <= 180.0 else lon - 360.0

   cell_base_idx = int(math.floor((norm_lon + 180.0) / 90.0))
   cell_base_idx = max(0, min(3, cell_base_idx))
   cell_left = -180.0 + (cell_base_idx * 90.0)
   cell_right = cell_left + 90.0

   if is_north:
      out_coords.append((cell_left, snap_lat))
      out_coords.append((cell_right, snap_lat))
   else:
      out_coords.append((cell_right, snap_lat))
      out_coords.append((cell_left, snap_lat))

def _unfold_rhealpix_pole_shelf(lon: float, lat: float, out_coords: List[Tuple[float, float]]) -> None:
   is_north = (lat > 0)
   snap_lat = 90.0 if is_north else -90.0
   west = -180
   east =  180
   if is_north:
      out_coords.append((west, snap_lat))
      out_coords.append((east, snap_lat))
   else:
      out_coords.append((east, snap_lat))
      out_coords.append((west, snap_lat))

def _calculate_healpix_cell_width(lat: float, subzone_level: int, polar_threshold: float) -> float:
    # Evaluates the Cotillon corner-to-corner cell width
    if abs(lat) >= polar_threshold:
        # At the absolute polar cap row, the border cell footprint width spans exactly 45.0 degrees.
        return 45.0

    row_height_segment = (math.pi / 8.0) / (2 ** subzone_level)

    sin_lat = math.sin(math.radians(abs(lat)))
    s_param = math.sqrt(max(0.0, 1.0 - sin_lat))
    width_param = math.sqrt(3.0) * (math.pi / 2.0) * s_param

    if width_param > 1e-11:
        ring_idx = int(round((width_param / 2.0) / row_height_segment))
        ring_idx = max(1, ring_idx)
        return 2.0 * (90.0 / ring_idx)

    return 90.0

def _calculate_rhealpix_cell_width(lat: float, subzone_level: int, polar_threshold: float) -> float:
    # Evaluates the Cotillon corner-to-corner cell width
    if abs(lat) >= polar_threshold:
        # At the absolute polar cap row, the border cell footprint width spans exactly 45.0 degrees.
        return 45.0

    row_height_segment = (math.pi / 2.0) / (3 ** subzone_level)

    sin_lat = math.sin(math.radians(abs(lat)))
    s_param = math.sqrt(max(0.0, 1.0 - sin_lat))
    width_param = math.sqrt(3.0) * (math.pi / 2.0) * s_param

    if width_param > 1e-11:
        ring_idx = int(round((width_param / 2.0) / row_height_segment))
        ring_idx = max(1, ring_idx)
        return 2.0 * (90.0 / ring_idx)

    return 90.0

def _snap_healpix_longitude(lon: float, lat: float, subzone_level: int, polar_threshold: float) -> float:
    norm_lon = lon if lon <= 180.0 else lon - 360.0

    # Determine proximity targets relative to the 90-degree quadrant cuts (0, \pm 90, \pm 180)
    cell_base_idx = int(math.floor((norm_lon + 180.0) / 90.0))
    cell_base_idx = max(0, min(3, cell_base_idx))
    cell_left = -180.0 + (cell_base_idx * 90.0)
    cell_right = cell_left + 90.0

    # Compute the active snapping width cushion from our pure latitude formula
    dLon = _calculate_healpix_cell_width(lat, subzone_level, polar_threshold)
    full_cell_width = dLon * 1.05

    # Execute snapping checks only along the boundary seams
    if abs(abs(norm_lon) - 180.0) <= full_cell_width:
        return -180.0 if norm_lon < 0 else 180.0

    if abs(norm_lon - cell_left) <= full_cell_width:
        return cell_left
    elif abs(norm_lon - cell_right) <= full_cell_width:
        return cell_right

    return lon

def _snap_rhealpix_longitude(lon: float, lat: float, subzone_level: int, polar_threshold: float) -> float:
    norm_lon = lon if lon <= 180.0 else lon - 360.0
    dLon = _calculate_rhealpix_cell_width(lat, subzone_level, polar_threshold)
    full_cell_width = dLon * 1.05
    if abs(abs(norm_lon) - 180.0) <= full_cell_width:
        return -180.0 if norm_lon < 0 else 180.0
    return lon

def _process_ring_crs_to_wgs84(ring_crs: List[Tuple[float, float]], proj: Any, zone_extent: list,
   pin: Pointd, gp: GeoPoint, root_level: int = 5, subzone_level: int = 16,
   polar_root_mode: PolarRootMode = PolarRootMode.NONE, refine_wgs84=None) -> List[Tuple[float, float]]:

   closed = list(ring_crs)
   if len(closed) > 0 and closed[0] != closed[-1]:
      closed.append(closed[0])
   out_coords: List[Tuple[float, float]] = []
   L = len(closed) - 1

   is5x6 = proj and (isinstance(proj, IVEAProjection) or isinstance(proj, ISEAProjection) or isinstance(proj, RTEAProjection))
   v_count = 0

   if polar_root_mode == PolarRootMode.GGG:
      s_level = subzone_level
      sDLat = 90.0 / (2 ** s_level)
      ggg_polar_cap_limit = sDLat * 1.01
      sz_dlon_ggg = 90.0 / (2 ** subzone_level)
   elif polar_root_mode == PolarRootMode.HEALPIX:
      hz_dlat = 45.0 / (2 ** subzone_level)
      hp_polar_threshold = 90.0 - (hz_dlat * 1.05)
   elif polar_root_mode == PolarRootMode.RHEALPIX:
      hz_dlat = 90.0 / (3 ** subzone_level)
      rhp_polar_threshold = 90.0 - (hz_dlat * 1.05)

   # --- ROLLING STATE TRACKERS FOR RI5x6 ---
   at_pole = False
   pole_lat_val = 0.0
   prev_lon = None
   prev_lat = None

   for i in range(L):
      p = closed[i]
      n = closed[i + 1]
      seg_pts = _insert_intermediate_points_crs_segment(p, n, refine_wgs84=refine_wgs84) if is5x6 else [p]

      for (x_crs, y_crs) in seg_pts:
         pin.x = x_crs
         pin.y = y_crs
         if proj:
            if not proj.inverse(pin, gp, False):
               print("WARNING: Inverse projection failed for:", pin.x, ",", pin.y, "->", gp.lat, ",", gp.lon)
               continue
            lat, lon = float(gp.lat), float(gp.lon)
         else:
            lat, lon = float(pin.x * 180 / Pi), float(pin.y * 180 / Pi)

         #if 90 - abs(lat) < 1e-10 and not intersects_extent_deg((lon, lat, lon, lat), zone_extent):
         #   continue

         if polar_root_mode == PolarRootMode.HEALPIX:
            lon = _snap_healpix_longitude(lon, lat, subzone_level, hp_polar_threshold)
            if abs(lat) >= hp_polar_threshold:
               _unfold_healpix_pole_shelf(lon, lat, out_coords)
               continue
         elif polar_root_mode == PolarRootMode.RHEALPIX:
            lon = _snap_rhealpix_longitude(lon, lat, subzone_level, rhp_polar_threshold)
            if prev_lon is not None and abs(lat) >= 84:
               if abs(lon - prev_lon) > 180.0 and (abs(lon - 180) < 0.001 or abs(lon - -180) < 0.001):
                  out_coords.append((sgn(prev_lon) * 180, prev_lat))
                  _unfold_rhealpix_pole_shelf(lon, lat, out_coords)
                  out_coords.append((lon, lat))
         elif polar_root_mode == PolarRootMode.GGG:
            snapped = _snap_polar_vertex(lon, lat, sz_dlon_ggg, ggg_polar_cap_limit)
            if snapped:
               lon, snap_lat = snapped
               if snap_lat in (-90.0, 90.0):
                  _expand_polar_cap_vertices(out_coords, lon, snap_lat)
                  continue
               lat = snap_lat

         # --- NATIVE STREAMING RI5x6 TRACKER ---
         elif polar_root_mode == PolarRootMode.RI5x6:
            is_curr_pole = (abs(lat) >= 90.0 - 1e-10)
            if is_curr_pole:
               lat = -90.0 if lat < 0 else 90.0
               # Normalize the active longitude so the trailing append doesn't insert a rogue value
               if prev_lon is not None:
                  lon = prev_lon

            if is_curr_pole and not at_pole:
               # Case A: Entering the pole singularity
               if prev_lon is not None and abs(lon - prev_lon) > 1e-7:
                  out_coords.append((prev_lon, lat))
                  v_count += 1
               at_pole = True
               pole_lat_val = lat

            elif not is_curr_pole and at_pole:
               # Case B: Leaving the pole singularity
               if abs(lon - prev_lon) > 1e-7:
                  out_coords.append((lon, pole_lat_val))
                  v_count += 1
               at_pole = False

         out_coords.append((lon, lat))
         v_count += 1
         prev_lon = lon
         prev_lat = lat

   # --- FINAL POINT CLOSURE WRAP ---
   if len(closed) > 0:
      last_x, last_y = closed[-1]
      pin.x, pin.y = float(last_x), float(last_y)
      if proj:
         if not proj.inverse(pin, gp, False):
            print("WARNING: Inverse projection failed for:", pin.x, ",", pin.y, "->", gp.lat, ",", gp.lon)
            return out_coords
         final_lon, final_lat = float(gp.lon), float(gp.lat)
      else:
         final_lon, final_lat = float(pin.y * 180 / Pi), float(pin.x * 180 / Pi)

      if polar_root_mode == PolarRootMode.HEALPIX:
         final_lon = _snap_healpix_longitude(final_lon, final_lat, subzone_level, hp_polar_threshold)
         if abs(final_lat) >= hp_polar_threshold:
            _unfold_healpix_pole_shelf(final_lon, final_lat, out_coords)
         else:
            out_coords.append((final_lon, final_lat))
      elif polar_root_mode == PolarRootMode.RHEALPIX:
         final_lon = _snap_rhealpix_longitude(final_lon, final_lat, subzone_level, rhp_polar_threshold)
         if abs(final_lat) >= 84 and prev_lon is not None and abs(final_lon - prev_lon) > 180.0 and (abs(final_lon - 180) < 0.001 or abs(final_lon - -180) < 0.001):
            out_coords.append((sgn(prev_lon) * 180, prev_lat))
            _unfold_rhealpix_pole_shelf(final_lon, final_lat, out_coords)
            out_coords.append((final_lon, final_lat))
         else:
            out_coords.append((final_lon, final_lat))
      elif polar_root_mode == PolarRootMode.GGG:
         snapped = _snap_polar_vertex(final_lon, final_lat, sz_dlon_ggg, ggg_polar_cap_limit)
         if snapped:
            final_lon, snap_lat = snapped
            if snap_lat in (-90.0, 90.0):
               _expand_polar_cap_vertices(out_coords, final_lon, snap_lat)
            else:
               final_lat = snap_lat
               out_coords.append((final_lon, final_lat))
         else:
            out_coords.append((final_lon, final_lat))
      elif polar_root_mode == PolarRootMode.RI5x6:
         is_final_pole = (abs(final_lat) >= 90.0 - 1e-10)
         if is_final_pole:
            final_lat = -90.0 if final_lat < 0 else 90.0
            if prev_lon is not None:
               final_lon = prev_lon

         if is_final_pole and not at_pole:
            if prev_lon is not None and abs(final_lon - prev_lon) > 1e-7:
               out_coords.append((prev_lon, final_lat))
         elif not is_final_pole and at_pole:
            if abs(final_lon - prev_lon) > 1e-7:
               out_coords.append((final_lon, pole_lat_val))
         out_coords.append((final_lon, final_lat))
      else:
         out_coords.append((final_lon, final_lat))

   return out_coords

def unproject_geojson_to_wgs84(obj: Dict[str, Any], proj: Any, zone_extent, refine_wgs84=None,
                               root_level: int = None, subzone_level: int = None, is_polar_root: PolarRootMode = PolarRootMode.NONE) -> Dict[str, Any]:
   pin = Pointd()
   gp = GeoPoint()

   is5x6 = proj and (isinstance(proj, IVEAProjection) or isinstance(proj, ISEAProjection) or isinstance(proj, RTEAProjection))

   def _process_geom(geom: Dict[str, Any], zone_extent, refine_wgs84 = None) -> Dict[str, Any]:
      gtype = geom["type"] if geom else None
      if gtype == "Polygon":
         if not geom["coordinates"]:
            raise InvalidPolygonGeometry
         exterior = geom["coordinates"][0]
         holes = geom["coordinates"][1:] if len(geom["coordinates"]) > 1 else []
         ext_wgs = _process_ring_crs_to_wgs84(exterior, proj, zone_extent, pin, gp,
                                              root_level=root_level, subzone_level=subzone_level, polar_root_mode=is_polar_root,
                                              refine_wgs84=refine_wgs84)

         # Prune if exterior ring lacks 3 valid vertices + closure point
         if not ext_wgs or len(ext_wgs) < 4:
            return None

         holes_wgs = []
         for h in holes:
            hw = _process_ring_crs_to_wgs84(h, proj, zone_extent, pin, gp,
                                            root_level=root_level, subzone_level=subzone_level, polar_root_mode=is_polar_root,
                                            refine_wgs84=refine_wgs84)
            # Prune invalid inner hole rings
            if hw and len(hw) >= 4:
               holes_wgs.append(hw)
         return {"type": "Polygon", "coordinates": [ext_wgs] + holes_wgs}
      if gtype == "MultiPolygon":
         new_polys = []
         for poly in geom["coordinates"]:
            ext = poly[0]
            holes = poly[1:] if len(poly) > 1 else []

            ext_wgs = _process_ring_crs_to_wgs84(ext, proj, zone_extent, pin, gp,
                                                 root_level=root_level, subzone_level=subzone_level, polar_root_mode=is_polar_root,
                                                 refine_wgs84=refine_wgs84)

            # Skip this sub-polygon component if its shell is invalid
            if not ext_wgs or len(ext_wgs) < 4:
               continue

            holes_wgs = []
            for h in holes:
               hw = _process_ring_crs_to_wgs84(h, proj, zone_extent, pin, gp,
                                               root_level=root_level, subzone_level=subzone_level, polar_root_mode=is_polar_root,
                                               refine_wgs84=refine_wgs84)
               # Prune invalid inner holes inside the multi-component
               if hw and len(hw) >= 4:
                  holes_wgs.append(hw)
            new_polys.append([ext_wgs] + holes_wgs)

         if not new_polys:
            return None
         return {"type": "MultiPolygon", "coordinates": new_polys}
      if gtype == "LineString":
         coords = geom["coordinates"]
         out_coords: List[Tuple[float, float]] = []
         for i in range(len(coords) - 1):
            p = coords[i]
            n = coords[i + 1]
            seg_pts = _insert_intermediate_points_crs_segment(p, n, refine_wgs84=refine_wgs84) if is5x6 else [p]
            for (x_crs, y_crs) in seg_pts:
               pin.x = x_crs
               pin.y = y_crs
               if proj:
                  if not proj.inverse(pin, gp, False):
                     print("WARNING: Inverse projection failed for:", pin.x, ",", pin.y, "->", gp.lat, ",", gp.lon)
                  out_coords.append((float(gp.lon), float(gp.lat)))
               else:
                  out_coords.append((float(pin.y * 180 / Pi), float(pin.x * 180 / Pi)))
         pin.x = coords[-1][0]
         pin.y = coords[-1][1]
         if proj:
            if not proj.inverse(pin, gp, False):
               print("WARNING: Inverse projection failed for:", pin.x, ",", pin.y, "->", gp.lat, ",", gp.lon)
            out_coords.append((float(gp.lon), float(gp.lat)))
         else:
            out_coords.append((float(pin.y * 180 / Pi), float(pin.x * 180 / Pi)))
         return {"type": "LineString", "coordinates": out_coords}
      if gtype == "MultiLineString":
         new_lines = []
         for line in geom["coordinates"]:
            out_coords = []
            for i in range(len(line) - 1):
               p = line[i]
               n = line[i + 1]
               seg_pts = _insert_intermediate_points_crs_segment(p, n, refine_wgs84=refine_wgs84) if is5x6 else [p]
               for (x_crs, y_crs) in seg_pts:
                  pin.x = x_crs
                  pin.y = y_crs
                  if proj:
                     if not proj.inverse(pin, gp, False):
                        print("WARNING: Inverse projection failed for:", pin.x, ",", pin.y, "->", gp.lat, ",", gp.lon)
                     out_coords.append((float(gp.lon), float(gp.lat)))
                  else:
                     out_coords.append((float(pin.y * 180 / Pi), float(pin.x * 180 / Pi)))
            pin.x = line[-1][0]
            pin.y = line[-1][1]
            if proj:
               if not proj.inverse(pin, gp, False):
                  print("WARNING: Inverse projection failed for:", pin.x, ",", pin.y, "->", gp.lat, ",", gp.lon)
               out_coords.append((float(gp.lon), float(gp.lat)))
            else:
               out_coords.append((float(pin.y * 180 / Pi), float(pin.x * 180 / Pi)))
            new_lines.append(out_coords)
         return {"type": "MultiLineString", "coordinates": new_lines}
      if gtype == "Point":
         pin.x = geom["coordinates"][0]
         pin.y = geom["coordinates"][1]
         if proj:
            if not proj.inverse(pin, gp, False):
               print("WARNING: Inverse projection failed for:", pin.x, ",", pin.y, "->", gp.lat, ",", gp.lon)
            coords = (float(gp.lon), float(gp.lat))
         else:
            coords = (float(pin.y * 180 / Pi), float(pin.x * 180 / Pi))
         return {"type": "Point", "coordinates": coords }
      if gtype == "MultiPoint":
         pts = []
         for (x_crs, y_crs) in geom["coordinates"]:
            pin.x = x_crs
            pin.y = y_crs
            if proj:
               if not proj.inverse(pin, gp, False):
                  print("WARNING: Inverse projection failed for:", pin.x, ",", pin.y, "->", gp.lat, ",", gp.lon)
               pts.append((float(gp.lon), float(gp.lat)))
            else:
               pts.append((float(pin.y * 180 / Pi), float(pin.x * 180 / Pi)))
         return {"type": "MultiPoint", "coordinates": pts}
      return geom

   typ = obj["type"]
   if typ == "FeatureCollection":
      out = {"type": "FeatureCollection", "features": []}
      for feat in obj["features"]:
         geom = feat.get("geometry")
         if geom is None:
            out["features"].append(dict(feat))
            continue
         new_geom = _process_geom(geom, zone_extent, refine_wgs84=refine_wgs84)

         # Skip feature entirely if the geometry collapsed to None
         if new_geom is None:
            continue

         new_feat = dict(feat)
         new_feat["geometry"] = new_geom
         out["features"].append(new_feat)
      return out
   if typ == "Feature":
      geom = obj["geometry"]
      new_geom = _process_geom(geom, zone_extent, refine_wgs84=refine_wgs84) if geom is not None else None

      # If single feature collapses to None, drop geometry payload cleanly
      out = dict(obj)
      out["geometry"] = new_geom
      return out
   return _process_geom(obj, zone_extent, refine_wgs84=refine_wgs84)
