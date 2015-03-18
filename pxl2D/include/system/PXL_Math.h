#ifndef _PXL_MATH_H
#define _PXL_MATH_H

#include <cmath>
#include "PXL_API.h"

#define PXL_clamp(v, min, max) v < min ? min : v && v > max ? max : v
#define PXL_min(v, v2) v < v2 ? v : v2
#define PXL_max(v, v2) v > v2 ? v : v2
#define PXL_wrap(v, min, max) int(((v + min) % max) + INT_MAX - (int(INT_MAX) % max)) % max

#define PXL_pi 3.14159265359f
#define PXL_two_pi 6.28318530718f
#define PXL_radian double(180 / PXL_pi)

extern void PXL_init_math();
extern double PXL_fast_cos(double degrees);
extern double PXL_fast_sin(double degrees);

#endif