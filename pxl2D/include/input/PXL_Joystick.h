#ifndef _PXL_JOYSTICK_H
#define _PXL_JOYSTICK_H

#include <Windows.h>
#include "system/PXL_API.h"

class PXL_Joystick {

	public:
		PXL_Joystick() { }
};

extern void PXL_joystick_init();

extern PXL_uint PXL_num_joysticks();
extern PXL_Joystick PXL_get_joystick(PXL_uint joystick_id);

#endif