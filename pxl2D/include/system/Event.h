#ifndef _EVENT_H
#define _EVENT_H

#include <vector>
#include "API.h"
#include "input/Joystick.h"
#include "graphics/Structs.h"

namespace pxl { namespace sys {

    #if defined(PLATFORM_WIN32)
	    #define EVENT_NULL			0
	    #define EVENT_CLOSE			0x0010 /**> Event message that occurs when a window sends a termination message **/
	    #define EVENT_QUIT			0x0012

	    #define EVENT_KEYDOWN		0x100
	    #define EVENT_KEYUP			0x101

	    #define EVENT_MOVE			0x0003
	    #define EVENT_SIZE			0x0005
	    #define EVENT_ACTIVATE		0x0006
	    #define EVENT_SETFOCUS		0x0007
        
	    #define EVENT_TOUCH 0
	
    #elif defined(PLATFORM_ANDROID)
	    #define EVENT_UNKNOWN 0
	    #define EVENT_CLOSE 1
	    #define EVENT_QUIT 2

	    #define EVENT_KEYDOWN 0
	    #define EVENT_KEYUP 0

	    #define EVENT_MOVE 0
	    #define EVENT_SIZE 0
	    #define EVENT_ACTIVATE 0
	    #define EVENT_SETFOCUS 0

	    #define EVENT_TOUCH 4
	
    #elif defined(PLATFORM_LINUX)
	
    #elif defined(PLATFORM_MAC_OS)
	
    #endif

    enum TouchEventState {

	    TOUCH_IDLE, 
	    TOUCH_DOWN,
	    TOUCH_UP,
	    TOUCH_MOVE
    };

    struct TouchInfo {

	    int x;
	    int y;
	    int id = -1;
	    TouchEventState state;
    };

    struct TouchEvent {

	    std::vector<TouchInfo> touches;
	    int num_touching = 0;
    };

    class Event {

	    public:
		    Event() { }

		    TouchEvent touch_event;
		    uint64 jbuttons;
		    uint64 jnum_buttons;
		    uint64 mouse_x;
		    uint64 mouse_y;
		    uint32 key_code;
		    uint32 type; /**> The message value **/
		    uint64 time; /**> The time value for when this event was posted **/
    };
}};

#endif