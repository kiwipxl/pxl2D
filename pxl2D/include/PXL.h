#ifndef _PXL_H
#define _PXL_H

#include "Graphics.h"
#include "System.h"
#include "Physics.h"
#include "Input.h"

namespace pxl {

    /** Initialises all PXL sub components
    **/
    extern void init();

    #if defined(PLATFORM_ANDROID)
	    #include <android_native_app_glue.h>
    #endif
};

#endif