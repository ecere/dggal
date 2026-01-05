# clipUtils.py
# Utility functions used by the clipper modules:
# - geometry predicates (point-in-polygon, segment intersections)
# - bbox helpers
# - zone boundary helpers (edges, boundary slice)
# - ring cleanup and small helpers

from typing import List, Dict, Optional, Tuple
EPS = 1e-12

def almost_equal_points(a: List[float], b: List[float], tol: float = 1e-9) -> bool:
   return abs(a[0] - b[0]) <= tol and abs(a[1] - b[1]) <= tol

def signed_area(verts: List[List[float]]) -> float:
    a = 0.0
    n = len(verts)
    for i in range(n):
        x0, y0 = verts[i]
        x1, y1 = verts[(i + 1) % n]
        a += (x0 * y1 - x1 * y0)
    return 0.5 * a

def point_in_polygon_old(pt: List[float], ring: List[List[float]]) -> bool:
    x, y = pt
    inside = False
    n = len(ring)
    for i in range(n):
        x0, y0 = ring[i]
        x1, y1 = ring[(i + 1) % n]
        cond = ((y0 > y) != (y1 > y))
        if cond:
            x_intersect = x0 + (x1 - x0) * (y - y0) / (y1 - y0)
            if x_intersect > x:
                inside = not inside
    return inside

def point_in_ring(pt, ring, eps=1e-12):
   # Winding-number point-in-polygon test adapted from libCartoSym's pointInsideContour2.
   # Returns True if point is inside or on the edge of the polygon, False otherwise.
   x, y = pt
   n = len(ring)
   if n < 3:
      return False

   # compute extent and quick reject
   xs = [p[0] for p in ring]
   ys = [p[1] for p in ring]
   minx, maxx = min(xs), max(xs)
   miny, maxy = min(ys), max(ys)
   if x < minx - abs(eps) or x > maxx + abs(eps) or y < miny - abs(eps) or y > maxy + abs(eps):
      return False

   # handle repeated final point equal to first
   def almost_equal(a, b, tol):
      return abs(a - b) <= tol

   if n > 2 and almost_equal(ring[-1][0], ring[0][0], eps) and almost_equal(ring[-1][1], ring[0][1], eps):
      count = n - 1
   else:
      count = n

   winding = 0

   def cross_from_line(px, py, ax, ay, bx, by):
      # cross product of (B-A) and (P-A): positive if P is left of AB
      return (bx - ax) * (py - ay) - (by - ay) * (px - ax)

   for i in range(count):
      ax, ay = ring[i]
      if i == count - 1:
         bx, by = ring[0]
      else:
         bx, by = ring[i + 1]

      d = float('inf')

      if ay <= y:
         if by > y:
            d = cross_from_line(x, y, ax, ay, bx, by)
            if d > eps:
               winding += 1
      else:
         if by <= y:
            d = cross_from_line(x, y, ax, ay, bx, by)
            if d < -eps:
               winding -= 1

      # on-edge detection: either d was not computed or is very small
      if d == float('inf') or abs(d) < eps:
         # bounding box check for the segment using x/y names
         minY = ay if ay < by else by
         maxY = by if ay < by else ay
         minX = ax if ax < bx else bx
         maxX = bx if ax < bx else ax
         if (y >= minY - eps and y <= maxY + eps and x >= minX - eps and x <= maxX + eps):
            # recompute d if needed
            if d == float('inf'):
               d = cross_from_line(x, y, ax, ay, bx, by)
               if abs(d) > eps:
                  continue
            return True

   return True if winding != 0 else False

def bbox_from_vertices(verts: List[List[float]]) -> List[float]:
    xs = [v[0] for v in verts]
    ys = [v[1] for v in verts]
    return [min(xs), min(ys), max(xs), max(ys)]

def segment_bbox(A: List[float], B: List[float]) -> List[float]:
    return [min(A[0], B[0]), min(A[1], B[1]), max(A[0], B[0]), max(A[1], B[1])]

def bbox_reject(seg_bbox: List[float], box: List[float]) -> bool:
    return (seg_bbox[2] < box[0] or seg_bbox[0] > box[2] or seg_bbox[3] < box[1] or seg_bbox[1] > box[3])

def segment_segment_intersections(A: List[float], B: List[float], C: List[float], D: List[float]) -> List[Dict]:
    x1, y1 = A
    x2, y2 = B
    x3, y3 = C
    x4, y4 = D
    den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)
    results = []
    if abs(den) > EPS:
        t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den
        u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) / den
        if -EPS <= t <= 1 + EPS and -EPS <= u <= 1 + EPS:
            ix = x1 + t * (x2 - x1)
            iy = y1 + t * (y2 - y1)
            results.append({"t": max(0.0, min(1.0, t)), "u": max(0.0, min(1.0, u)), "point": [float(ix), float(iy)], "type": "point"})
        return results
    # collinear / overlap
    cross = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1)
    if abs(cross) > EPS:
        return results
    if abs(x2 - x1) >= abs(y2 - y1):
        a0, a1 = x1, x2
        b0, b1 = x3, x4
    else:
        a0, a1 = y1, y2
        b0, b1 = y3, y4
    a_min, a_max = min(a0, a1), max(a0, a1)
    b_min, b_max = min(b0, b1), max(b0, b1)
    if a_max < b_min - EPS or b_max < a_min - EPS:
        return results
    def param_t_for_val(val, a0, a1):
        if abs(a1 - a0) < EPS:
            return 0.0
        return (val - a0) / (a1 - a0)
    ov_start = max(a_min, b_min)
    ov_end = min(a_max, b_max)
    if abs(x2 - x1) >= abs(y2 - y1):
        t0 = param_t_for_val(ov_start, x1, x2)
        t1 = param_t_for_val(ov_end, x1, x2)
    else:
        t0 = param_t_for_val(ov_start, y1, y2)
        t1 = param_t_for_val(ov_end, y1, y2)
    p0 = [x1 + t0 * (x2 - x1), y1 + t0 * (y2 - y1)]
    p1 = [x1 + t1 * (x2 - x1), y1 + t1 * (y2 - y1)]
    results.append({"t": max(0.0, min(1.0, t0)), "u": None, "point": [float(p0[0]), float(p0[1])], "type": "overlap_start"})
    results.append({"t": max(0.0, min(1.0, t1)), "u": None, "point": [float(p1[0]), float(p1[1])], "type": "overlap_end"})
    return results

def cleanup_ring(ring: List[List[float]]) -> Optional[List[List[float]]]:
    if not ring:
        return None
    out = [ring[0]]
    for p in ring[1:]:
        if not almost_equal_points(p, out[-1]):
            out.append(p)
    def collinear(a, b, c):
        return abs((b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0])) <= EPS
    i = 0
    while i < len(out) - 2:
        if collinear(out[i], out[i + 1], out[i + 2]):
            del out[i + 1]
        else:
            i += 1
    if len(out) < 4:
        return None
    if not almost_equal_points(out[0], out[-1]):
        out.append(out[0])
    return out
