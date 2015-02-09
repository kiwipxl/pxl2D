#include "PXL_Graphics.h"
#include "system/PXL_Exception.h"
#include "system/PXL_Window.h"

void PXL_graphics_init() {
	PXL_set_clear_colour(1, 1, 1, 1);
	PXL_set_clear_depth(1);
	PXL_clear();

	PXL_shader_init();
	PXL_font_init();
	PXL_lights_init();
}

void PXL_glew_init() {
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

void PXL_set_clear_colour(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

void PXL_set_clear_depth(float d) {
	glClearDepth(d);
}

void PXL_clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}
