#ifndef _PXL_GRAPHICS_API_H
#define _PXL_GRAPHICS_API_H

#include "system/PXL_API.h"

#if defined(PXL_PLATFORM_WIN32) || defined(PXL_PLATFORM_LINUX) || defined(PXL_PLATFORM_MAC_OS)
	#define GLEW_STATIC
	#include <glew.h>

	extern void PXL_glew_init();

#elif defined(PXL_PLATFORM_ANDROID)
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

extern void PXL_set_clear_colour(float r, float g, float b, float a);
extern void PXL_set_clear_depth(float d);
extern void PXL_clear();

#endif