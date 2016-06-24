#include "input/Joystick.h"
#include <vector>
#include "input/Keyboard.h"
#include "system/Exception.h"

namespace pxl { namespace input {

    std::vector<Joystick*> joysticks;

    uint32 num_joysticks() {
	    return joysticks.size();
    }

    Joystick* get_joystick(uint32 joystick_index) {
	    if (joystick_index >= joysticks.size()) {
            system::show_exception("Joystick id specified is out of bounds of joysticks list");
		    return NULL;
	    }else {
		    return joysticks[joystick_index];
	    }
    }

    #if defined(PLATFORM_WIN32)
	    #include <Windows.h>
	    #include <RegStr.h>

	    int8* get_joystick_name(uint32 device_id, JOYCAPS joy_caps) {
		    HKEY key_handle;
		    HKEY sub_key_handle;
		    DWORD reg_size;

		    int8 reg_path[256];
		    int8 reg_value[256];
		    int8 reg_name[256];
		    int8* name;

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

	    void joystick_init() {
		    JOYINFOEX joy_info;
		    JOYCAPS joy_caps;
		    uint32 num_devices = joyGetNumDevs();
		    uint32 device_id;

		    for (size_t n = 0; n < num_devices; ++n) {
			    if (joyGetPosEx(n, &joy_info) == JOYERR_NOERROR) {
				    device_id = n;

				    joyGetDevCaps(device_id, &joy_caps, sizeof(joy_caps));

				    Joystick* joystick = new Joystick();

				    joystick->name = get_joystick_name(device_id, joy_caps);
				    joystick->device_id = device_id;
				    joystick->num_buttons = joy_caps.wNumButtons;
				    joystick->num_axes = joy_caps.wNumAxes;

				    joysticks.push_back(joystick);
			    }
		    }
	    }

	    bool Joystick::activate() {
		    return joySetCapture(0, device_id, 0, false) == 0;
	    }

	    void Joystick::deactivate() {
		    joyReleaseCapture(device_id);
	    }

    #elif defined(PLATFORM_ANDROID)
	    void joystick_init() {

	    }

	    bool Joystick::activate() {
		    return false;
	    }

	    void Joystick::deactivate() {

	    }

    #elif defined(PLATFORM_LINUX)

    #elif defined(PLATFORM_MAC_OS)

    #endif
}};
