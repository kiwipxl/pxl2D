#ifndef _GRAPHICS_API_H
#define _GRAPHICS_API_H

#include "API.h"

#if defined(PLATFORM_WIN32) || defined(PLATFORM_LINUX) || defined(PLATFORM_MAC_OS)

#define GLEW_STATIC
#include <glew.h>

#elif defined(PLATFORM_ANDROID)

#include <EGL/egl.h>

//#include <GLES/gl.h>
//#include <GLES/glext.h>
//#include <GLES/glplatform.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
	
//#include <GLES3/gl3.h>
//#include <GLES3/gl3ext.h>
//#include <GLES3/gl3platform.h>

#endif

#include <GLFW/glfw3.h>

namespace pxl { namespace graphics {

    extern void glew_init();

    extern void set_clear_colour(float r, float g, float b, float a);
    extern void set_clear_depth(float d);
    extern void clear();
}};

#endif