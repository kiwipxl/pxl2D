#include "PXL.h"

void PXL_init() {
	PXL_log("PXL is initialising...");
	PXL_graphics_init();
	PXL_system_init();
	PXL_physics_init();
	PXL_input_init();
	PXL_log("PXL initialised successfully");
}