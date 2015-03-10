#include "PXL.h"

void PXL_init() {
	PXL_print << "PXL is initialising...\n";
	PXL_graphics_init();
	PXL_system_init();
	PXL_physics_init();
	PXL_input_init();
	PXL_print << "PXL initialised successfully!\n";
}