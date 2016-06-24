#ifndef _H
#define _H

#include "Graphics.h"
#include "System.h"
#include "Physics.h"
#include "Input.h"

namespace pxl {

    /** Initialises all PXL sub components
    **/
    extern void init();

    extern int main(int argc, char* args[]);

    #if defined(PLATFORM_ANDROID)
	    #include <android_native_app_glue.h>
    #endif
};

#endif