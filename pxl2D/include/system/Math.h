#ifndef _MATH_H
#define _MATH_H

#include "PXLAPI.h"

namespace pxl { namespace math {

    #define PXL_PI 3.14159265359f
    #define PXL_PI_2 6.28318530718f
    #define PXL_RADIANS 180.0f / PXL_PI
    #define PXL_RADIANS_D double(180 / PXL_PI)

    extern float clamp(float x, float min, float max);
    extern float min(float x1, float x2);
    extern float max(float x1, float x2);
    extern int wrap(int x, int min, int max);

    extern void init();
    extern double fast_cos(double degrees);
    extern double fast_sin(double degrees);
}};

#endif