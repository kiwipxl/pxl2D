#ifndef _JOYSTICK_H
#define _JOYSTICK_H

#include <string>
#include "API.h"

namespace pxl { namespace input {

    struct Joystick {

	    uint32 device_id;
	    std::string name;
	    uint32 num_buttons;
	    uint32 num_axes;

	    bool activate();
	    void deactivate();
    };

    extern void joystick_init();

    extern uint32 num_joysticks();
    extern Joystick* get_joystick(uint32 joystick_index);
}};

#endif