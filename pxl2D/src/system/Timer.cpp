#include "system/Timer.h"

#include <iostream>

namespace pxl { namespace sys {

    Timer timer;

    void start_timer() {
	    timer.start();
    }

    long stop_timer() {
	    return timer.end();
    }

    void Timer::start() {
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start_time);
    }

    long Timer::end() {
        QueryPerformanceCounter(&end_time);
        elapsed_ms.QuadPart = end_time.QuadPart - start_time.QuadPart;
        elapsed_ms.QuadPart *= 1000000;
        elapsed_ms.QuadPart /= freq.QuadPart;
        elapsed = (long)elapsed_ms.QuadPart;
        return elapsed;
    }

	void sleep(int ms) {

	}
}};