#include "PXL_Keyboard.h"
#include <iostream>

bool* PXL_keys = new bool[255] {false};

bool PXL_key_down(PXL_KeyCode key_code) {
	return PXL_keys[key_code];
}

bool PXL_key_up(PXL_KeyCode key_code) {
	return PXL_keys[key_code];
}
