#ifndef _PXL_JOYSTICK_H
#define _PXL_JOYSTICK_H

#include <string>
#include <Windows.h>
#include "system/PXL_API.h"

struct PXL_Joystick {

	PXL_uint device_id;
	std::string name;
	PXL_uint num_buttons;
	PXL_uint num_axes;
};

extern void PXL_joystick_init();

extern PXL_uint PXL_num_joysticks();
extern PXL_Joystick* PXL_get_joystick(PXL_uint joystick_index);

#endif