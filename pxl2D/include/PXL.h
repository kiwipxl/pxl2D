#ifndef _PXL_H
#define _PXL_H

#include "PXL_Graphics.h"
#include "PXL_System.h"
#include "PXL_Physics.h"
#include "PXL_Input.h"

/** Initialises all PXL sub components
**/
extern void PXL_init();

extern int main(int argc, char* args[]);

#if defined(PXL_PLATFORM_ANDROID)
	#include <android_native_app_glue.h>
#endif

#endif