#include "PXL.h"
#include <android/log.h>

#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

void PXL_init() {
	LOGW("initiating pxl2d");

	PXL_graphics_init();
	PXL_system_init();
	PXL_physics_init();
	PXL_input_init();

	LOGW("successfully initiated pxl2d222222");
}