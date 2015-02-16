#ifndef _PXL_EVENT_H
#define _PXL_EVENT_H

#include <Windows.h>
#include "PXL_API.h"
#include "input/PXL_Joystick.h"

#define PXL_EVENT_NULL			WM_NULL
#define PXL_EVENT_QUIT			WM_QUIT /**> Event message that occurs when a window sends a termination message **/

#define PXL_EVENT_KEYDOWN		WM_KEYDOWN
#define PXL_EVENT_KEYUP			WM_KEYUP

#define PXL_EVENT_MOVE			WM_MOVE
#define PXL_EVENT_SIZE			WM_SIZE
#define PXL_EVENT_ACTIVATE		WM_ACTIVATE
#define PXL_EVENT_SETFOCUS		WM_SETFOCUS

class PXL_Event {

	public:
		PXL_Event() { }

		HWND window_handle;
		PXL_ulong jbuttons;
		PXL_ulong jnum_buttons;
		PXL_ulong mouse_x;
		PXL_ulong mouse_y;
		PXL_uint key_code;
		PXL_uint type; /**> The message value **/
		PXL_ulong time; /**> The time value for when this event was posted **/
};

#endif