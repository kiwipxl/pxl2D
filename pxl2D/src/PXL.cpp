#include "PXL.h"

void PXL_init() {
	PXL_print << "PXL is initialising...\n";
	PXL_system_init();
	PXL_graphics_init();
	PXL_physics_init();
	PXL_input_init();
	PXL_print << "PXL initialised successfully!\n";
}

#if defined(PXL_PLATFORM_ANDROID)
	void android_main(struct android_app* state) {
		android_state = state;
		main(NULL, NULL);
		this_thing();
	}
#endif
