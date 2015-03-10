#include "system/PXL_Timer.h"
#include <iostream>

PXL_Timer timer;

void PXL_start_timer() {
	timer.start();
}

long PXL_stop_timer() {
	return timer.end();
}

#if defined(PXL_PLATFORM_WIN32)
	void PXL_sleep(int ms) {
		timeBeginPeriod(1);
		Sleep(ms);
	}

#elif defined(PXL_PLATFORM_ANDROID)
	#include <unistd.h>

	void PXL_sleep(int ms) {
		sleep(ms);
	}

#endif