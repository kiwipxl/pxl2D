#include "system/Timer.h"
#include <iostream>

namespace pxl { namespace system {

    Timer timer;

    void start_timer() {
	    timer.start();
    }

    long stop_timer() {
	    return timer.end();
    }

    #if defined(PLATFORM_WIN32)
	    void sleep(int ms) {
		    timeBeginPeriod(1);
		    Sleep(ms);
	    }

    #elif defined(PLATFORM_ANDROID)
	    #include <unistd.h>

	    void sleep(int ms) {
		    sleep(ms);
	    }
    #endif
}};