#include "PXL_Joystick.h"
#include <iostream>
#include <RegStr.h>
#include "PXL_System.h"
#include "PXL_Keyboard.h"

PXL_byte* get_joystick_name(PXL_uint device_id, JOYCAPS joy_caps) {
	HKEY key_handle;
	HKEY sub_key_handle;
	DWORD reg_size;

	PXL_byte reg_path[256];
	PXL_byte reg_value[256];
	PXL_byte reg_name[256];
	PXL_byte* name;

	//set joy config reg path, reg key and the current joystick settings reg folder
	sprintf(reg_path, "%s\\%s\\%s", REGSTR_PATH_JOYCONFIG, joy_caps.szRegKey, REGSTR_KEY_JOYCURR);

	//attempt to open register path inside the local_machine key handle
	key_handle = HKEY_LOCAL_MACHINE;

	if (RegOpenKeyEx(key_handle, reg_path, 0, KEY_READ, &sub_key_handle) != ERROR_SUCCESS) {
		//if could not open local_machine path then attempt to open register path inside current_machine key handle
		key_handle = HKEY_CURRENT_USER;

		if (RegOpenKeyEx(key_handle, reg_path, 0, KEY_READ, &sub_key_handle) != ERROR_SUCCESS) {
			//could not open key handle so return empty string
			return "";
		}
	}

	//set register value for the joystick OEM name
	sprintf(reg_value, "Joystick%d%s", device_id + 1, REGSTR_VAL_JOYOEMNAME);

	//query value and return joystick reg name and then close key
	reg_size = sizeof(reg_name);
	if (RegQueryValueEx(sub_key_handle, reg_value, 0, 0, (LPBYTE)reg_name, &reg_size) != ERROR_SUCCESS) {
		RegCloseKey(sub_key_handle);
		return "";
	}
	RegCloseKey(sub_key_handle);

	//set reg path to the joystick OEM followed by the reg name value
	sprintf(reg_path, "%s\\%s", REGSTR_PATH_JOYOEM, reg_name);

	if (RegOpenKeyEx(key_handle, reg_path, 0, KEY_READ, &sub_key_handle) != ERROR_SUCCESS) {
		return "";
	}

	//get joystick OEM name from currently open key and check if no error occurred while getting the size for the OEM name
	reg_size = sizeof(reg_value);
	if (RegQueryValueEx(sub_key_handle, REGSTR_VAL_JOYOEMNAME, 0, 0, NULL, &reg_size) == ERROR_SUCCESS) {
		//allocate enough memory for the OEM name
		name = new char[reg_size];
		//read OEM value from registry
		RegQueryValueEx(sub_key_handle, REGSTR_VAL_JOYOEMNAME, 0, 0, (LPBYTE)name, &reg_size);

		RegCloseKey(sub_key_handle);
	}else {
		RegCloseKey(sub_key_handle);

		return "";
	}

	return name;
}

std::vector<PXL_Joystick*> joysticks;

void PXL_joystick_init() {
	JOYINFO joy_info;
	JOYCAPS joy_caps;
	PXL_uint num_devices = joyGetNumDevs();
	PXL_uint device_id;

	for (int n = 0; n < num_devices; ++n) {
		if (joyGetPos(n, &joy_info) == 0) {
			device_id = n;

			joyGetDevCaps(device_id, &joy_caps, sizeof(joy_caps));

			PXL_Joystick* joystick = new PXL_Joystick();

			joystick->name = get_joystick_name(device_id, joy_caps);
			joystick->device_id = device_id;
			joystick->num_buttons = joy_caps.wNumButtons;
			joystick->num_axes = joy_caps.wNumAxes;

			joysticks.push_back(joystick);
		}
	}
}

extern PXL_uint PXL_num_joysticks() {
	return joysticks.size();
}

extern PXL_Joystick* PXL_get_joystick(PXL_uint joystick_index) {
	if (joystick_index >= joysticks.size()) {
		PXL_show_exception("Joystick id specified is out of bounds of joysticks list");
		return NULL;
	}else {
		return joysticks[joystick_index];
	}
}