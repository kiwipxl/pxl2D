#include "input/PXL_Keyboard.h"
#include <iostream>

KeyEvent* PXL_keys = new KeyEvent[255];

bool PXL_key_down(PXL_KeyCode key_code) {
	return PXL_keys[key_code].key_down;
}

bool PXL_key_up(PXL_KeyCode key_code) {
	return PXL_keys[key_code].key_down;
}
