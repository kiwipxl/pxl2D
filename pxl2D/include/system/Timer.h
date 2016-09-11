#ifndef _TIMER_H
#define _TIMER_H

#include <vector>
#include "API.h"

#include <Windows.h>

namespace pxl { namespace sys {

	struct Timer {

		public:
			long elapsed = 0;

			void start();
			long end();

		private:
            LARGE_INTEGER start_time, end_time, elapsed_ms, freq;
	};

    //todo: replace with timer_start and timer_stop
    extern void start_timer();
    extern long stop_timer();
    extern void sleep(int ms);
}};

#endif