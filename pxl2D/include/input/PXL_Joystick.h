#ifndef _PXL_JOYSTICK_H
#define _PXL_JOYSTICK_H

#include <string>
#include "system/PXL_API.h"

struct PXL_Joystick {

	uint32 device_id;
	std::string name;
	uint32 num_buttons;
	uint32 num_axes;

	bool activate();
	void deactivate();
};

extern void PXL_joystick_init();

extern uint32 PXL_num_joysticks();
extern PXL_Joystick* PXL_get_joystick(uint32 joystick_index);

#endif