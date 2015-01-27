#include "PXL.h"
#include "system/PXL_Exception.h"

void PXL_init() {
	GLenum error;
	if ((error = glewInit()) != GLEW_OK) {
		PXL_show_exception("Could not initialise PXL: " + (std::string)(char*)glewGetErrorString(error));
		return;
	}
	std::cout << "glew initiated\n";
	std::cout << "gl version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "glsl version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

	PXL_shader_init();
	PXL_font_init();
	PXL_lights_init();
}