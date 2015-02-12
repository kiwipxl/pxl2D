#include "PXL.h"

void PXL_init() {
	PXL_graphics_init();
	PXL_system_init();
	PXL_physics_init();
	PXL_input_init();
}