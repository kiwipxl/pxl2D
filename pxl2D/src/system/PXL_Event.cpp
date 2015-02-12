#include "PXL_Event.h"
#include <iostream>

MSG msg;

extern bool PXL_poll_event(PXL_Event& e) {
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
		e.key.code = msg.wParam;
		e.type = msg.message;
		e.time = msg.time;

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		return true;
	}
	return false;
}