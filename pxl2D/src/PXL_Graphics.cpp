#include "PXL_Graphics.h"
#include "system/PXL_Exception.h"
#include "system/PXL_Window.h"

void PXL_graphics_init() {
	PXL_set_clear_colour(1, 1, 1, 1);
	PXL_set_clear_depth(1);
	PXL_clear();

	PXL_shader_init();
	PXL_font_init();
}