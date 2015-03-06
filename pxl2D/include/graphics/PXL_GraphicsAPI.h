#ifndef _PXL_GRAPHICS_API_H
#define _PXL_GRAPHICS_API_H

#include <system/PXL_API.h>

#if defined(PXL_WINDOWS) || defined(PXL_LINUX) || defined(PXL_MAC_OS)
	#define GLEW_STATIC
	#include <glew.h>

	extern void PXL_glew_init() {
		glewExperimental = true;
		GLenum error;
		if ((error = glewInit()) != GLEW_OK) {
			PXL_force_show_exception("Could not initialise GLEW. Error: " + (std::string)(char*)glewGetErrorString(error));
			return;
		}
		std::cout << "glew initiated\n";
		int min;
		glGetIntegerv(GL_MINOR_VERSION, &min);
		int target;
		glGetIntegerv(GL_MAJOR_VERSION, &target);
		std::cout << "gl version: " << glGetString(GL_VERSION) << ", min: " << min << ", target: " << target << "\n";
		std::cout << "glsl version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
		std::cout << "vendor: " << glGetString(GL_VENDOR) << "\n";
		std::cout << "renderer: " << glGetString(GL_RENDERER) << "\n";
	}
#elif defined(PXL_ANDROID)
	#include <EGL/egl.h>

	//#include <GLES/gl.h>
	//#include <GLES/glext.h>
	//#include <GLES/glplatform.h>

	//#include <GLES2/gl2.h>
	//#include <GLES2/gl2ext.h>
	//#include <GLES2/gl2platform.h>

	#include <GLES3/gl3.h>
	#include <GLES3/gl3ext.h>
	#include <GLES3/gl3platform.h>
#endif

extern void PXL_set_clear_colour(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

extern void PXL_set_clear_depth(float d) {
	glClearDepthf(d);
}

extern void PXL_clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

#endif