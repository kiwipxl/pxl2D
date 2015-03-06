#include "PXL_GraphicsAPI.h"
#include <iostream>
#include "system/PXL_API.h"
#include "system/PXL_Exception.h"

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

extern void PXL_set_clear_colour(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

extern void PXL_set_clear_depth(float d) {
	glClearDepthf(d);
}

extern void PXL_clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}
