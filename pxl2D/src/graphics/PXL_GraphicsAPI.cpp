#include "graphics/PXL_GraphicsAPI.h"
#include "system/PXL_API.h"
#include "system/PXL_Exception.h"
#include "system/PXL_Debug.h"

extern void PXL_glew_init() {
	#if defined(PXL_PLATFORM_WIN32)
		glewExperimental = true;
        GLenum error;
        if ((error = glewInit()) != GL_NO_ERROR) {
            PXL_force_show_exception("Could not initialise GLEW. Error: " + (std::string)(char*)glewGetErrorString(error));
        }
		PXL_print << "glew initiated\n";
		int min;
		glGetIntegerv(GL_MINOR_VERSION, &min);
		int target;
		glGetIntegerv(GL_MAJOR_VERSION, &target);
		PXL_print << "gl version: " << glGetString(GL_VERSION) << ", min: " << min << ", target: " << target << "\n";
		PXL_print << "glsl version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
		PXL_print << "vendor: " << glGetString(GL_VENDOR) << "\n";
		PXL_print << "renderer: " << glGetString(GL_RENDERER) << "\n";
	#endif
}

extern void PXL_set_clear_colour(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

extern void PXL_set_clear_depth(float d) {
	glClearDepthf(d);
}

extern void PXL_clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}
