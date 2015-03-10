#ifndef _PXL_TIMER_H
#define _PXL_TIMER_H

#include <vector>
#include "system/PXL_API.h"

#if defined(PXL_PLATFORM_WIN32)
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
#elif defined(PXL_PLATFORM_ANDROID)
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

//todo: replace with timer_start and timer_stop
extern void PXL_start_timer();
extern long PXL_stop_timer();
extern void PXL_sleep(int ms);

#endif