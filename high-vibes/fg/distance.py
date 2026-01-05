# distance5x6.py
"""
Compute true 5x6-space distance between two 5x6 coordinates, delegating
seam/interruption detection to crosses5x6_interruption_v2_ex from interruption.py.

Public API:
  distance5x6(a: Pointd, b: Pointd) -> (
      distance: Pointd,
      b_in_a_frame: Optional[Pointd],
      mod_a: Optional[Pointd],
      i_src: Optional[Pointd],
      i_dst: Optional[Pointd],
      in_north: Optional[bool],
      ends_at_edge: Optional[bool]
  )

Notes:
 - This module depends on faces.get_face and interruption.crosses5x6_interruption_v2_ex
 - It preserves the original control flow and wrap-around normalization logic.
 - No snapping or precision-reducing operations are performed here.
"""

from typing import Optional, Tuple, Sequence, Union
import math

from fg.faces import Point, Pointd, get_face
from fg.interruption import rotate5x6_offset, crosses5x6_interruption_v2_ex

def sgn(v: float) -> int:
    if v > 0: return 1
    if v < 0: return -1
    return 0

_EPS = 1e-11

def distance5x6(
    a_in: Pointd,
    b_in: Pointd,
) -> Tuple[
    Pointd,
    Optional[Pointd],
    Optional[Pointd],
    Optional[Pointd],
    Optional[Pointd],
    Optional[bool],
    Optional[bool],
]:
    """
    Compute the true 5x6-space distance between points a_in and b_in,
    taking interruptions into account.

    Returns:
      (distance, b_in_a_frame, mod_a, i_src, i_dst, in_north, ends_at_edge)

    - distance: Pointd (dx, dy) or (nan, nan) if faces non-adjacent
    - b_in_a_frame: b expressed in a's frame (or None)
    - mod_a: possibly-modified a after face rotations (or None)
    - i_src, i_dst: interruption source/destination intersection points (or None)
    - in_north: bool indicating north/south interruption side (or None)
    - ends_at_edge: bool indicating the segment ends exactly at the edge (or None)
    """
    # local mutable copies
    a = Pointd(a_in.x, a_in.y)
    b = Pointd(b_in.x, b_in.y)

    # get faces and fractional coords
    f1, d1 = get_face(a)
    f2, d2 = get_face(b)

    dx = f2.x - f1.x
    dy = f2.y - f1.y

    # normalize wrap-around differences
    if dx < -3 or dy < -3:
        dx += 5
        dy += 5
    if dx > 3 or dy > 3:
        dx -= 5
        dy -= 5

    # attempt pole rotations to make faces adjacent when possible
    if abs(dx) > 0 or abs(dy) > 0:
        # a is north-pole-like vertex and can be rotated
        if (f1.x - f1.y) == 0 and d1.x > 1.0 - _EPS and d1.y < _EPS:
            for i in range(5):
                nx = (f1.x + i) % 5
                ny = nx
                tdx = f2.x - nx
                tdy = f2.y - ny
                if tdx < -3 or tdy < -3:
                    tdx += 5; tdy += 5
                if tdx > 3 or tdy > 3:
                    tdx -= 5; tdy -= 5
                if abs(tdx) <= 1 and abs(tdy) <= 1:
                    if abs(dx) > 1 or abs(dy) > 1 or (abs(tdx) < abs(dx) and abs(tdy) < abs(dy)):
                        a.x += (nx - f1.x)
                        a.y += (ny - f1.y)
                        f1 = Point(nx, ny)
                        dx = tdx; dy = tdy
                        if dx == 0 and dy == 0:
                            break
        # b is north-pole-like vertex and can be rotated
        elif (f2.x - f2.y) == 0 and d2.x > 1.0 - _EPS and d2.y < _EPS:
            for i in range(5):
                nx = (f2.x + i) % 5
                ny = nx
                tdx = nx - f1.x
                tdy = ny - f1.y
                if tdx < -3 or tdy < -3:
                    tdx += 5; tdy += 5
                if tdx > 3 or tdy > 3:
                    tdx -= 5; tdy -= 5
                if abs(tdx) <= 1 and abs(tdy) <= 1:
                    if abs(dx) > 1 or abs(dy) > 1 or (abs(tdx) < abs(dx) and abs(tdy) < abs(dy)):
                        b.x += (nx - f2.x)
                        b.y += (ny - f2.y)
                        f2 = Point(nx, ny)
                        dx = tdx; dy = tdy
                        if dx == 0 and dy == 0:
                            break
        # a is south-pole-like vertex and can be rotated
        elif (f1.y - f1.x) == 1 and d1.x < _EPS and d1.y > 1.0 - _EPS:
            for i in range(5):
                nx = (f1.x + i) % 5
                ny = nx + 1
                tdx = f2.x - nx
                tdy = f2.y - ny
                if tdx < -3 or tdy < -3:
                    tdx += 5; tdy += 5
                if tdx > 3 or tdy > 3:
                    tdx -= 5; tdy -= 5
                if abs(tdx) <= 1 and abs(tdy) <= 1:
                    if abs(dx) > 1 or abs(dy) > 1 or (abs(tdx) < abs(dx) and abs(tdy) < abs(dy)):
                        a.x += (nx - f1.x)
                        a.y += (ny - f1.y)
                        f1 = Point(nx, ny)
                        dx = tdx; dy = tdy
                        if dx == 0 and dy == 0:
                            break
        # b is south-pole-like vertex and can be rotated
        elif (f2.y - f2.x) == 1 and d2.x < _EPS and d2.y > 1.0 - _EPS:
            for i in range(5):
                nx = (f2.x + i) % 5
                ny = nx + 1
                tdx = nx - f1.x
                tdy = ny - f1.y
                if tdx < -3 or tdy < -3:
                    tdx += 5; tdy += 5
                if tdx > 3 or tdy > 3:
                    tdx -= 5; tdy -= 5
                if abs(tdx) <= 1 and abs(tdy) <= 1:
                    if abs(dx) > 1 or abs(dy) > 1 or (abs(tdx) < abs(dx) and abs(tdy) < abs(dy)):
                        b.x += (nx - f2.x)
                        b.y += (ny - f2.y)
                        f2 = Point(nx, ny)
                        dx = tdx; dy = tdy
                        if dx == 0 and dy == 0:
                            break

    # If faces still non-adjacent, return NaN distance
    if abs(dx) > 1 or abs(dy) > 1:
        nanp = float("nan")
        mod_a = Pointd(a.x, a.y)
        return Pointd(nanp, nanp), None, mod_a, None, None, None, None

    # same-face case
    if dx == 0 and dy == 0:
        distance = Pointd(b.x - a.x, b.y - a.y)
        # normalize wrap-around
        if distance.x < -3 or distance.y < -3:
            distance.x += 5; distance.y += 5
            b.x += 5; b.y += 5
        elif distance.x > 3 or distance.y > 3:
            distance.x -= 5; distance.y -= 5
            b.x -= 5; b.y -= 5
        b_in_a_frame = Pointd(b.x, b.y)
        mod_a = Pointd(a.x, a.y)
        return distance, b_in_a_frame, mod_a, None, None, None, None

    # adjacent-face case: prepare pivot and rotated coords
    rotation = 0
    pivot = Point(f2.x if (dx > 0 or dy > 0) else f1.x,
                  f2.y if (dx > 0 or dy > 0) else f1.y)

    # compute rb = b - pivot
    rb = Pointd(b.x - pivot.x, b.y - pivot.y)

    # determine rotation amount
    north = pivot.x >= pivot.y
    top = (f1.x == f1.y)
    no_interruptions = (dx <= 0 and dy >= 0) if top else (dx >= 0 and dy <= 0)
    a_is_vertex = (abs(a.y - a.x) < _EPS) if top else (abs(a.y - a.x - 1.0) < _EPS)

    if (not no_interruptions) and (not a_is_vertex):
        if dx == -1:
            if dy == -1:
                rotation = -1 if north else 1
            elif dy == 0:
                rotation = 1 if north else -1
            elif dy == 1:
                rotation = 1
        elif dx == 0:
            if dy == -1:
                rotation = -1
            elif dy == 0:
                rotation = 1
            elif dy == 1:
                rotation = 1
        elif dx == 1:
            if dy == -1:
                rotation = 1
            elif dy == 0:
                rotation = 1
            elif dy == 1:
                rotation = 1 if north else -1

    if pivot.x - b.x < -3:
        pivot = Point(pivot.x + 5, pivot.y + 5)

    # apply rotation offset rotation times
    tmp_rb = Pointd(rb.x, rb.y)
    for _ in range(abs(rotation)):
        rotate5x6_offset(tmp_rb, tmp_rb.x, tmp_rb.y, rotation < 0)
    rb = Pointd(tmp_rb.x + pivot.x, tmp_rb.y + pivot.y)

    ddx = rb.x - a.x
    ddy = rb.y - a.y
    if ddx > 3 or ddy > 3:
        ddx -= 5; ddy -= 5
    elif ddx < -3 or ddy < -3:
        ddx += 5; ddy += 5

    # Delegate seam crossing detection and intersection computation to external function.
    crossing, i_src, i_dst, in_north, ends_at_edge = crosses5x6_interruption_v2_ex(a, ddx, ddy, final_cross=True)

    if not crossing:
        # No seam interruption detected
        if no_interruptions or a_is_vertex:
            distance = Pointd(b.x - a.x, b.y - a.y)
        else:
            distance = Pointd(ddx, ddy)

        # normalize wrap-around
        if distance.x < -3 or distance.y < -3:
            distance.x += 5; distance.y += 5
            b.x += 5; b.y += 5
        elif distance.x > 3 or distance.y > 3:
            distance.x -= 5; distance.y -= 5
            b.x -= 5; b.y -= 5

        b_in_a_frame = Pointd(b.x, b.y)
        mod_a = Pointd(a.x, a.y)
        return distance, b_in_a_frame, mod_a, i_src, i_dst, in_north, ends_at_edge
    else:
        # There is a seam crossing. Return ddx/ddy as nominal vector and rb as bInAFrame
        distance = Pointd(ddx, ddy)
        b_in_a_frame = Pointd(rb.x, rb.y)
        mod_a = Pointd(a.x, a.y)
        return distance, b_in_a_frame, mod_a, i_src, i_dst, in_north, ends_at_edge

def move5x6(o: Union[Pointd, Sequence[float]],
            dx: float,
            dy: float,
            nRotations: int,
            adjX: Optional[Sequence[float]] = None,
            adjY: Optional[Sequence[float]] = None,
            finalCross: bool = False) -> Pointd:
    """
    Port of the DGGAL move5x6 function.

    Parameters
    ----------
    o : Pointd or (x,y)
        Origin point.
    dx, dy : float
        Movement vector components.
    nRotations : int
        Number of 60-degree rotations to apply when interruption occurs.
    adjX, adjY : optional mutable sequences (e.g., [value]) or None
        If provided, they will be updated in-place to mirror pointer behavior.
    finalCross : bool
        If True, allow crossing at the final step.

    Returns
    -------
    Pointd
        The resulting point after moving.
    """
    # normalize origin input
    if isinstance(o, Pointd):
        c = Pointd(o.x, o.y)
    else:
        c = Pointd(float(o[0]), float(o[1]))

    # small constants
    EPS = 1e-11
    TINY = 1e-11

    if c.x < 0 and c.y < 1 + 1e-11:
        c.x += 5.0
        c.y += 5.0

    if abs(dx) < 1e-11 and abs(dy) < 1e-11:
        return c

    while True:
        cdx = c.x
        cdy = c.y
        rotation = 0
        north = not (cdy - cdx > 1)
        iy = int(math.floor(cdy + EPS))
        ix = int(math.floor(cdx + EPS))
        doNudge = True

        # Nudge logic for y-aligned grid lines
        if (abs(c.y - iy) < EPS and 0 <= iy <= 5
            and c.x > float(iy - 1) - EPS and c.x < float(iy) + EPS):
            cdy += 2 * sgn(dy) * EPS
            if c.x > float(iy - 1) + EPS and c.x < float(iy) - EPS:
                doNudge = False

        # Nudge logic for x-aligned grid lines
        if (abs(c.x - ix) < EPS and 0 <= ix <= 5
            and c.y > float(ix) - EPS and c.y < float(ix + 1) + EPS):
            cdx += 2 * sgn(dx) * EPS
            if c.y > float(ix) + EPS and c.y < float(ix + 1) - EPS:
                doNudge = False

        if doNudge:
            if not north:
                cdx += TINY
                cdy -= TINY
            else:
                cdx -= TINY
                cdy += TINY

        # Wrap-around adjustments (5x6 tiling)
        if cdx < 0 and cdy < 1 + 1e-11:
            cdx += 5.0; cdy += 5.0
            c.x += 5.0; c.y += 5.0
        if cdx > 5 and cdy > 5 - 1e-11:
            cdx -= 5.0; cdy -= 5.0
            c.x -= 5.0; c.y -= 5.0
        if cdy < 0 and cdx < 1e-11:
            cdx += 5.0; cdy += 5.0
            c.x += 5.0; c.y += 5.0

        # clamp
        if cdx < 0: cdx = 0.0
        if cdy < 0: cdy = 0.0
        if cdx > 5: cdx = 5.0
        if cdy > 6: cdy = 6.0

        cx = int(math.floor(cdx))
        cy = int(math.floor(cdy))

        # compute px, py (how far we can move within current cell)
        if dx < 0:
            px = max(cx - c.x, dx)
        else:
            px = min(cx + 1 - c.x, dx)

        if dy < 0:
            py = max(cy - c.y, dy)
        else:
            py = min(cy + 1 - c.y, dy)

        # if both dx and dy non-zero, synchronize to the earliest crossing
        if dx != 0.0 and dy != 0.0:
            pkx = px / dx
            pky = py / dy
            if pkx < pky:
                py = pkx * dy
            elif pky < pkx:
                px = pky * dx

        c.x += px
        c.y += py

        # if not finalCross and we've consumed the entire dx/dy, break
        if not finalCross:
            if abs(dx - px) < 1e-11 and abs(dy - py) < 1e-11:
                break

        # determine next cell indices (nx, ny)
        atVertex = (abs(c.x - int(c.x + 0.5)) < EPS and
                    abs(c.y - int(c.y + 0.5)) < EPS)

        nx = int(math.floor(c.x + EPS * sgn(dx)))
        ny = int(math.floor(c.y + EPS * sgn(dy)))

        if atVertex and abs(dx) > EPS and abs(dy) <= EPS:
            ny = cy
            nx = cx + sgn(dx)
        elif atVertex and abs(dy) > EPS and abs(dx) <= EPS:
            nx = cx
            ny = cy + sgn(dy)
        elif nx > cx and ny < cy:
            nx = cx
        elif nx < cx and ny > cy:
            ny = cy

        # interruption handling
        if nx != cx or ny != cy:
            nNorth = not ((cx + cy) & 1)

            if nNorth:
                # North
                if (ny == cy and nx == cx + 1):
                    # Crossing interruption to the right
                    iy2 = int(c.x - 1 + 1e-11)
                    # c = { iy + 2 - (c.y - iy), c.x };
                    c = Pointd(iy2 + 2 - (c.y - iy2), c.x)
                    rotation = -1
                elif nx == cx and ny == cy - 1:
                    # Crossing interruption to the left
                    ix2 = int(c.y + 1e-11)
                    c = Pointd(c.y, ix2 - (c.x - ix2))
                    rotation = 1
            else:
                # South
                if nx == cx and ny == cy + 1:
                    ix2 = int(c.y - 2 + 1e-11)
                    c = Pointd(c.y - 1, ix2 + 3 - (c.x - ix2))
                    rotation = 1
                elif ny == cy and nx == cx - 1:
                    iy2 = int(c.x + 1 + 1e-11)
                    c = Pointd(iy2 - 1 - (c.y - iy2), c.x + 1)
                    rotation = -1

        # consume the portion we moved
        dx -= px
        dy -= py

        # more wrap-around corrections
        if c.x < -1e-11 and c.y < 1 + 1e-11:
            c.x += 5.0; c.y += 5.0
        elif c.y < 0 and c.x < 1e-11:
            c.x += 5.0; c.y += 5.0
        elif c.x > 5 - 1e-11 and c.y > 6 + 1e-11:
            c.x -= 5.0; c.y -= 5.0

        # Apply rotation(s) if interruption required one
        if rotation:
            for _ in range(nRotations):
                if rotation == -1:
                    # 60 degrees clockwise rotation
                    ndx = dx - dy
                    dy = dx
                    dx = ndx
                    if adjX is not None and adjY is not None:
                        nax = adjX[0] - adjY[0]
                        adjY[0] = adjX[0]
                        adjX[0] = nax
                else:
                    # 60 degrees counter-clockwise rotation
                    ndy = dy - dx
                    dx = dy
                    dy = ndy
                    if adjX is not None and adjY is not None:
                        nay = adjY[0] - adjX[0]
                        adjX[0] = adjY[0]
                        adjY[0] = nay

        if abs(dx) < 1e-11:
            dx = 0.0
        if abs(dy) < 1e-11:
            dy = 0.0

        if dx == 0.0 and dy == 0.0:
            break

    return c
