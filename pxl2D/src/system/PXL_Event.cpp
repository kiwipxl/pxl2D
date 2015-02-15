#include "PXL_Event.h"
#include <iostream>
#include "PXL_Window.h"

MSG msg;

extern bool PXL_poll_event(PXL_Event& e) {
	if (PeekMessage(&msg, PXL_primary_window->win_handle, 0, 0, PM_REMOVE) > 0) {
		PXL_get_joystick(0);
		JOYINFOEX joy_info;
		joy_info.dwFlags = JOY_RETURNALL;
		joyGetPosEx(1, &joy_info);
		e.jbuttons = joy_info.dwButtons;
		e.jnum_buttons = joy_info.dwButtonNumber;

		e.mouse_x = LOWORD(msg.lParam);
		e.mouse_y = HIWORD(msg.lParam);

		e.key_code = msg.wParam;
		e.type = msg.message;
		e.time = msg.time;

		if (e.jbuttons == 1) {
			SendMessage(msg.hwnd, 4, 1, 0);
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		return true;
	}
	return false;
}