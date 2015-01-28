#include "PXL_Graphics.h"
#include "system/PXL_Exception.h"

void PXL_graphics_init() {
	GLenum error;
	if ((error = glewInit()) != GLEW_OK) {
		PXL_show_exception("Could not initialise PXL: " + (std::string)(char*)glewGetErrorString(error));
		return;
	}
	std::cout << "glew initiated\n";
	std::cout << "gl version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "glsl version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

	PXL_set_clear_colour(1, 1, 1, 1);
	PXL_set_clear_depth(1);
	PXL_clear();

	PXL_shader_init();
	PXL_font_init();
	PXL_lights_init();
}

extern void PXL_set_clear_colour(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

extern void PXL_set_clear_depth(float d) {
	glClearDepth(d);
}

extern void PXL_clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}
