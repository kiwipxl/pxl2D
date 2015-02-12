#ifndef _PXL_EVENT_H
#define _PXL_EVENT_H

#include <string>
#include <vector>
#include "PXL_Window.h"

typedef uint PXL_Message;

class PXL_Event {

	public:
		PXL_Event() { }

		MSG msg;
		HWND window_handle;
		PXL_Message message; /**> The message value **/
		ulong time; /**> The time value for when this event was posted **/
};

extern bool PXL_poll_event(PXL_Event& e);

#endif