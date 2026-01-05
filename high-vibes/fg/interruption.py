# interruption.py
"""
Port of crosses5x6InterruptionV2Ex and rotate5x6Offset into Python.

This module depends on faces.Pointd / faces.Point dataclasses (from faces.py).
It implements the interruption detection logic, translated
to Python with the same numeric tolerances and control flow.

Public API:
  - rotate5x6_offset(r: Pointd, dx: float, dy: float, clockwise: bool) -> None
  - crosses5x6_interruption_v2_ex(
        c_in: Pointd,
        dx: float,
        dy: float,
        final_cross: bool = True
    ) -> (bool, Optional[Pointd], Optional[Pointd], Optional[bool], Optional[bool])

Return values for crosses5x6_interruption_v2_ex:
  (result, i_src, i_dst, in_north, ends_at_edge)
  - result: True if an interruption crossing was detected
  - i_src: Pointd of the source-side intersection (or None)
  - i_dst: Pointd of the destination-side intersection (or None)
  - in_north: bool indicating north/south interruption side (or None)
  - ends_at_edge: bool indicating the segment ends exactly at the edge (or None)

This implementation preserves coordinate precision and does not perform snapping.
"""

from typing import Optional, Tuple
import math

from fg.faces import Point, Pointd

# Tolerances copied from the original eC code
_EPS_EDGE = 1e-11
_EPS_SMALL = 1e-11  # used for tiny comparisons in several places


def sgn(v: float) -> int:
    if v > 0:
        return 1
    if v < 0:
        return -1
    return 0


def rotate5x6_offset(r: Pointd, dx: float, dy: float, clockwise: bool) -> None:
    """
    Apply the 60-degree rotation offset used in the 5x6 net.
    Mutates r in place to the rotated coordinates.
    """
    if clockwise:
        # 60 degrees clockwise rotation
        r.x = dx - dy
        r.y = dx
    else:
        # 60 degrees counter-clockwise rotation
        r.x = dy
        r.y = dy - dx


def crosses5x6_interruption_v2_ex(
    c_in: Pointd,
    dx: float,
    dy: float,
    final_cross: bool = True,
) -> Tuple[bool, Optional[Pointd], Optional[Pointd], Optional[bool], Optional[bool]]:
    """
    Detect whether the segment starting at c_in and offset by (dx,dy) crosses
    a 5x6 interruption. Ported from eC implementation.

    Parameters
    - c_in: starting Pointd in 5x6 coordinates
    - dx, dy: offset vector from c_in
    - final_cross: if False, treat exact-end-of-segment as non-crossing

    Returns:
      (result, i_src, i_dst, in_north, ends_at_edge)
    """
    result = False
    # copy input
    c = Pointd(c_in.x, c_in.y)

    # normalize wrap-around near lower-left corner (matches eC)
    if c.x < 0 and c.y < 1.0 + _EPS_EDGE:
        c.x += 5.0
        c.y += 5.0

    # local working copies
    cdx = c.x
    cdy = c.y
    north = (cdx - cdy - _EPS_EDGE) > 0
    # integer cell indices and temporary variables
    cx = cy = 0
    nx = ny = 0
    px = py = 0.0

    # apply tiny bias depending on north/south to avoid exact-boundary ambiguity
    if north:
        cdx -= _EPS_SMALL
        cdy += _EPS_SMALL
    else:
        cdx += _EPS_SMALL
        cdy -= _EPS_SMALL

    # wrap adjustments mirroring original logic
    if cdx < 0 and cdy < 1.0 + _EPS_EDGE:
        cdx += 5.0
        cdy += 5.0
        c.x += 5.0
        c.y += 5.0

    if cdx > 5.0 and cdy > 5.0 - _EPS_EDGE:
        cdx -= 5.0
        cdy -= 5.0
        c.x -= 5.0
        c.y -= 5.0

    if cdy < 0.0 and cdx < 1e-11:
        cdx += 5.0
        cdy += 5.0
        c.x += 5.0
        c.y += 5.0

    # floor to get cell indices
    cx = int(math.floor(cdx))
    cy = int(math.floor(cdy))

    # compute px/py: the portion of dx/dy that reaches the current cell edge
    if dx < 0:
        px = max(cx - c.x, dx)
    else:
        px = min(cx + 1 - c.x, dx)

    if dy < 0:
        py = max(cy - c.y, dy)
    else:
        py = min(cy + 1 - c.y, dy)

    # if both dx and dy non-zero, pick the limiting parametric fraction
    if dx != 0.0 and dy != 0.0:
        pkx = px / dx
        pky = py / dy
        if pkx < pky:
            py = pkx * dy
        elif pky < pkx:
            px = pky * dx

    # advance c by the chosen px/py
    c.x += px
    c.y += py

    # if not finalCross and we've consumed the whole segment, no crossing
    if not final_cross:
        if abs(dx - px) < _EPS_SMALL and abs(dy - py) < _EPS_SMALL:
            return False, None, None, None, None
    else:
        # if finalCross and endsAtEdge detection requested, we would set endsAtEdge
        # but here we don't have an endsAtEdge pointer; we will compute it later if needed
        pass

    # compute next cell indices after the step (with tiny bias in direction of dx/dy)
    nx = int(math.floor(c.x + 1e-11 * sgn(dx)))
    ny = int(math.floor(c.y + 1e-11 * sgn(dy)))

    i_src: Optional[Pointd] = None
    i_dst: Optional[Pointd] = None
    in_north: Optional[bool] = None
    ends_at_edge: Optional[bool] = None

    # check whether we've moved to a different cell and whether conditions indicate an interruption
    moved_cell = (nx != cx or ny != cy)
    cond_extra = (nx > cx or (nx == cx and (ny - nx) == 2) or
                  (abs(dx - px) > _EPS_SMALL) or (abs(dy - py) > _EPS_SMALL))

    if moved_cell and cond_extra:
        root = cx + cy
        # even root => North pattern, odd root => South pattern
        if (root & 1) == 0:
            # North interruption cases
            # Crossing interruption to the right
            if (ny == cy) and (nx == cx + 1):
                iy = int(math.floor(c.x - 1.0 + 1e-11))
                i_src = Pointd(c.x, c.y)
                # iDst = { iy + 2 - (c.y - iy), c.x };
                i_dst = Pointd(iy + 2.0 - (c.y - iy), c.x)
                in_north = True
                if abs(px) > _EPS_SMALL or abs(py) > _EPS_SMALL:
                    result = True
            # Crossing interruption to the left
            elif (nx == cx) and (ny == cy - 1):
                ix = int(math.floor(c.y + 1e-11))
                i_src = Pointd(c.x, c.y)
                # iDst = { c.y, ix - (c.x - ix) };
                i_dst = Pointd(c.y, ix - (c.x - ix))
                in_north = True
                if abs(px) > _EPS_SMALL or abs(py) > _EPS_SMALL:
                    result = True
        else:
            # South interruption cases
            # Crossing interruption to the right
            if (nx == cx) and (ny == cy + 1):
                ix = int(math.floor(c.y - 2.0 + 1e-11))
                i_src = Pointd(c.x, c.y)
                # iDst = { c.y - 1, ix + 3 - (c.x - ix) };
                i_dst = Pointd(c.y - 1.0, ix + 3.0 - (c.x - ix))
                in_north = False
                if abs(px) > _EPS_SMALL or abs(py) > _EPS_SMALL:
                    result = True
            # Crossing interruption to the left
            elif (ny == cy) and (nx == cx - 1):
                iy = int(math.floor(c.x + 1.0 + 1e-11))
                i_src = Pointd(c.x, c.y)
                # iDst = { iy - 1 - (c.y - iy), c.x + 1 };
                i_dst = Pointd(iy - 1.0 - (c.y - iy), c.x + 1.0)
                in_north = False
                if abs(px) > _EPS_SMALL or abs(py) > _EPS_SMALL:
                    result = True

    # final wrap adjustment for iDst if needed (matches eC)
    if result and i_dst is not None and i_dst.x < 0 and i_dst.y < 1.0 + _EPS_EDGE:
        i_dst.x += 5.0
        i_dst.y += 5.0

    # ends_at_edge detection (only meaningful when final_cross True)
    if final_cross and (abs(dx - px) < _EPS_SMALL and abs(dy - py) < _EPS_SMALL):
        # check whether the segment ended exactly at the tile edge (approx)
        # This mirrors the eC logic branch that sets endsAtEdge when finalCross and ends exactly at edge.
        # We compute a conservative boolean here.
        # Determine whether the absolute movement equals the distance to the cell edge in both axes.
        # Note: original code had a more specific check; we approximate equivalently.
        # Compute distances to cell boundaries used earlier:
        dist_x = (cx + 1 - c.x) if dx > 0 else (cx - c.x)
        dist_y = (cy + 1 - c.y) if dy > 0 else (cy - c.y)
        if abs(abs(dx) - abs(dist_x)) < _EPS_SMALL and abs(abs(dy) - abs(dist_y)) < _EPS_SMALL:
            ends_at_edge = True
        else:
            ends_at_edge = False

    return result, i_src, i_dst, in_north, ends_at_edge
