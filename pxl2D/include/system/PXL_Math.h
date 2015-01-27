#ifndef _PXL_MATH_H
#define _PXL_MATH_H

#define PXL_clamp(v, min, max) v < min ? min : v && v > max ? max : v
#define PXL_min(v, v2) v < v2 ? v : v2
#define PXL_max(v, v2) v > v2 ? v : v2

#define PXL_pi 3.14159265359f
#define PXL_radian float(180 / PXL_pi)

#endif