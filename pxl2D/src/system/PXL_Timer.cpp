#include "PXL_Timer.h"
#include <iostream>

PXL_Timer timer;

void PXL_start_timer() {
	timer.start();
}

long PXL_stop_timer() {
	return timer.end();
}