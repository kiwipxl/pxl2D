#ifndef _GRAPHICS_API_H
#define _GRAPHICS_API_H

#include "API.h"

namespace pxl { namespace graphics {

    extern void glew_init();

    extern void set_clear_colour(float r, float g, float b, float a);
    extern void set_clear_depth(float d);
    extern void clear();
}};

#endif