#ifndef _TIMER_H
#define _TIMER_H

#include <vector>
#include "API.h"

namespace pxl { namespace sys {

    #if defined(PLATFORM_WIN32)
	    #include <Windows.h>

	    struct Timer {

		    public:
			    long elapsed = 0;

			    void start() {
				    QueryPerformanceFrequency(&freq);
				    QueryPerformanceCounter(&start_time);
			    }

			    long end() {
				    QueryPerformanceCounter(&end_time);
				    elapsed_ms.QuadPart = end_time.QuadPart - start_time.QuadPart;
				    elapsed_ms.QuadPart *= 1000000;
				    elapsed_ms.QuadPart /= freq.QuadPart;
				    elapsed = (long)elapsed_ms.QuadPart;
				    return elapsed;
			    }

		    private:
			    LARGE_INTEGER start_time, end_time, elapsed_ms, freq;
	    };
    #elif defined(PLATFORM_ANDROID)
	    struct Timer {

		    public:
			    long elapsed = 0;

			    void start() {

			    }

			    long end() {
				    return elapsed;
			    }
	    };
    #endif

    //todo: replace with timer_start and timer_stop
    extern void start_timer();
    extern long stop_timer();
    extern void sleep(int ms);
}};

#endif