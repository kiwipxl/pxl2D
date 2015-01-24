#include "PXL.h"

void PXL_init() {
	GLenum error;
	if ((error = glewInit()) != GLEW_OK) {
		MessageBox(NULL, (char*)glewGetErrorString(error), "An error occurred!", MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	std::cout << "glew initiated\n";
	std::cout << "gl version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "glsl version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

	PXL_shader_init();
	PXL_font_init();
	PXL_lights_init();
}