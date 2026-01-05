# faces.py
"""
5x6 face helpers

Provides:
  - Point (integer face coordinates)
  - Pointd (floating 5x6 coordinates)
  - get_face(p) -> (face: Point)

get_face mirrors the behavior of the eC getFace function.
It normalizes wrap-around, applies tiny eps offsets, clamps to valid ranges,
and returns the integer face (cx,cy) and fractional coordinates inside that face.
"""

from dataclasses import dataclass
import math
from typing import Tuple

# Small tolerances copied from the original logic
_EPS_SMALL = 1e-13
_EPS_EDGE = 1e-11

@dataclass
class Point:
    x: int
    y: int

@dataclass
class Pointd:
    x: float
    y: float

def _clamp(v: float, lo: float, hi: float) -> float:
    if v < lo:
        return lo
    if v > hi:
        return hi
    return v

def get_face(a: Pointd) -> Tuple[Point, Pointd]:
    """
    Determine the 5x6 face and fractional coordinates for point `a`.

    Parameters
    - a: Pointd in 5x6 coordinates (may be outside canonical range)

    Returns
    - (face: Point, d: Pointd) where:
        face.x, face.y are integer tile indices (cx, cy)
        d.x, d.y are fractional coordinates inside that tile (cdx - cx, cdy - cy)
    """
    # Work on a copy
    c_x = float(a.x)
    c_y = float(a.y)

    # initial wrap adjustment from original code
    if c_x < 0.0 and c_y < 1.0 + _EPS_EDGE:
        c_x += 5.0
        c_y += 5.0

    # apply tiny epsilon offsets to avoid exact-boundary ambiguity
    cdx = c_x + _EPS_SMALL
    cdy = c_y + _EPS_SMALL

    # additional wrap adjustments mirroring original logic
    if cdx < 0.0 and cdy < 1.0 + _EPS_EDGE:
        cdx += 5.0
        cdy += 5.0
        c_x += 5.0
        c_y += 5.0

    if cdx > 5.0 and cdy > 5.0 - _EPS_EDGE:
        cdx -= 5.0
        cdy -= 5.0
        c_x -= 5.0
        c_y -= 5.0

    if cdy < 0.0 and cdx < 1e-11:
        cdx += 5.0
        cdy += 5.0
        c_x += 5.0
        c_y += 5.0

    # clamp to valid ranges used in original code
    cdx = _clamp(cdx, 0.0, 5.0)
    cdy = _clamp(cdy, 0.0, 6.0)

    cx = int(math.floor(cdx))
    cy = int(math.floor(cdy))

    # special-case adjustments for pole/edge conditions
    if (cx - cy == 1) and (cdx - cx < _EPS_EDGE):
        # North pole or right at left-side of northern interruption
        cx -= 1
    elif (cy - cx == 2) and (cdy - cy < _EPS_EDGE):
        # South pole or right at left-side of southern interruption
        cy -= 1

    face = Point(cx, cy)
    d = Pointd(cdx - cx, cdy - cy)
    return face, d
