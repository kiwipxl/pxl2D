#ifndef _PXL_TIMER_H
#define _PXL_TIMER_H

#include <vector>

#if defined(WIN32)
#include <Windows.h>

struct PXL_Timer {

	public:
		void start() {
			QueryPerformanceFrequency(&freq);
			QueryPerformanceCounter(&start_time);
		}

		long end() {
			QueryPerformanceCounter(&end_time);
			elapsed.QuadPart = end_time.QuadPart - start_time.QuadPart;
			elapsed.QuadPart *= 1000000;
			elapsed.QuadPart /= freq.QuadPart;
			return elapsed.QuadPart;
		}

	private:
		LARGE_INTEGER start_time, end_time, elapsed, freq;
};
#endif

extern void PXL_start_timer();
extern long PXL_stop_timer();

#endif