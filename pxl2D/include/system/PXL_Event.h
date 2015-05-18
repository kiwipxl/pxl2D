#ifndef _PXL_EVENT_H
#define _PXL_EVENT_H

#include <vector>
#include "system/PXL_API.h"
#include "input/PXL_Joystick.h"
#include "graphics/PXL_Structs.h"

#if defined(PXL_PLATFORM_WIN32)
    #define NOMINMAX //macro to not have the windows header define min/max so it doesn't interfere
    #include <Windows.h>
    #undef ABSOLUTE
    #undef RELATIVE
    
	#define PXL_EVENT_NULL			0
	#define PXL_EVENT_CLOSE			WM_CLOSE /**> Event message that occurs when a window sends a termination message **/
	#define PXL_EVENT_QUIT			WM_QUIT

	#define PXL_EVENT_KEYDOWN		WM_KEYDOWN
	#define PXL_EVENT_KEYUP			WM_KEYUP

	#define PXL_EVENT_MOVE			WM_MOVE
	#define PXL_EVENT_SIZE			WM_SIZE
	#define PXL_EVENT_ACTIVATE		WM_ACTIVATE
	#define PXL_EVENT_SETFOCUS		WM_SETFOCUS

	#define PXL_EVENT_TOUCH 0
	
#elif defined(PXL_PLATFORM_ANDROID)
	#define PXL_EVENT_UNKNOWN 0
	#define PXL_EVENT_CLOSE 1
	#define PXL_EVENT_QUIT 2

	#define PXL_EVENT_KEYDOWN 0
	#define PXL_EVENT_KEYUP 0

	#define PXL_EVENT_MOVE 0
	#define PXL_EVENT_SIZE 0
	#define PXL_EVENT_ACTIVATE 0
	#define PXL_EVENT_SETFOCUS 0

	#define PXL_EVENT_TOUCH 4
	
#elif defined(PXL_PLATFORM_LINUX)
	
#elif defined(PXL_PLATFORM_MAC_OS)
	
#endif

enum PXL_TouchEventState {

	PXL_TOUCH_IDLE, 
	PXL_TOUCH_DOWN,
	PXL_TOUCH_UP,
	PXL_TOUCH_MOVE
};

struct PXL_TouchInfo {

	int x;
	int y;
	int id = -1;
	PXL_TouchEventState state;
};

struct PXL_TouchEvent {

	std::vector<PXL_TouchInfo> touches;
	int num_touching = 0;
};

class PXL_Event {

	public:
		PXL_Event() { }

		PXL_TouchEvent touch_event;
		uint64 jbuttons;
		uint64 jnum_buttons;
		uint64 mouse_x;
		uint64 mouse_y;
		uint32 key_code;
		uint32 type; /**> The message value **/
		uint64 time; /**> The time value for when this event was posted **/
};

#endif