from typing import List, Tuple, Dict, Any, Optional

# Sutherland-Hodgman helpers (top-level)
def _intersect_edge(p: Tuple[float,float], q: Tuple[float,float], edge: str,
                    xmin: float, ymin: float, xmax: float, ymax: float) -> Tuple[float,float]:
   x1, y1 = p
   x2, y2 = q
   dx = x2 - x1
   dy = y2 - y1
   if edge == 'left':
      x = xmin
      if abs(dx) < 1e-18:
         return (x, y1)
      t = (xmin - x1) / dx
      return (x, y1 + t * dy)
   if edge == 'right':
      x = xmax
      if abs(dx) < 1e-18:
         return (x, y1)
      t = (xmax - x1) / dx
      return (x, y1 + t * dy)
   if edge == 'bottom':
      y = ymin
      if abs(dy) < 1e-18:
         return (x1, y)
      t = (ymin - y1) / dy
      return (x1 + t * dx, y)
   y = ymax
   if abs(dy) < 1e-18:
      return (x1, y)
   t = (ymax - y1) / dy
   return (x1 + t * dx, y)

def _inside_for_edge(pt: Tuple[float,float], edge: str,
                     xmin: float, ymin: float, xmax: float, ymax: float) -> bool:
   x, y = pt
   if edge == 'left':
      return x >= xmin
   if edge == 'right':
      return x <= xmax
   if edge == 'bottom':
      return y >= ymin
   return y <= ymax

def rect_clip_polygon(ring_coords: List[Tuple[float,float]],
                      xmin: float, ymin: float, xmax: float, ymax: float) -> List[Tuple[float,float]]:
   def clip_edge(coords: List[Tuple[float,float]], edge: str) -> List[Tuple[float,float]]:
      out: List[Tuple[float,float]] = []
      if not coords:
         return out
      prev = coords[-1]
      for curr in coords:
         inside_curr = _inside_for_edge(curr, edge, xmin, ymin, xmax, ymax)
         inside_prev = _inside_for_edge(prev, edge, xmin, ymin, xmax, ymax)
         if inside_curr:
            if inside_prev:
               out.append(curr)
            else:
               out.append(_intersect_edge(prev, curr, edge, xmin, ymin, xmax, ymax))
               out.append(curr)
         else:
            if inside_prev:
               out.append(_intersect_edge(prev, curr, edge, xmin, ymin, xmax, ymax))
         prev = curr
      return out

   # make a working copy
   coords = list(ring_coords)

   # require at least 3 input points to form a polygon
   if len(coords) < 3:
      return []

   # ensure the ring is closed for clipping (first == last)
   if coords[0] != coords[-1]:
      coords.append(coords[0])

   # clip against each edge; if any clip empties the polygon, return empty
   for edge in ('left', 'right', 'bottom', 'top'):
      coords = clip_edge(coords, edge)
      if not coords:
         return []

   # if the result is closed (first == last), drop the duplicate closing point
   if coords and coords[0] == coords[-1]:
      coords = coords[:-1]

   # require at least 3 points (open ring) to form a polygon
   if len(coords) < 3:
      return []

   return coords
