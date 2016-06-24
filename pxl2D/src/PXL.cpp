#include "PXL.h"

namespace pxl {

    void init() {
	    system::print << "PXL is initialising...\n";
	    system_init();
	    graphics_init();
	    physics_init();
	    input_init();
        system::print << "PXL initialised successfully!\n";
    }

    #if defined(PLATFORM_ANDROID)
	    void android_main(struct android_app* state) {
		    android_state = state;
		    main(NULL, NULL);
		    this_thing();
	    }
    #endif
};