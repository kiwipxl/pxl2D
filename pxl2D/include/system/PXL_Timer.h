#ifndef _PXL_TIMER_H
#define _PXL_TIMER_H

#include <vector>
#include "PXL_API.h"
#include <cstdlib>

#if defined(PXL_WINDOWS)
#include <Windows.h>

struct PXL_Timer {

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
#elif defined(PXL_ANDROID)
struct PXL_Timer {

	public:
		long elapsed = 0;

		void start() {

		}

		long end() {
			return elapsed;
		}
};
#endif

extern void PXL_start_timer();
extern long PXL_stop_timer();

#endif