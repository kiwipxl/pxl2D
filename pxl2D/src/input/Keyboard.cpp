#include "input/Keyboard.h"
#include <iostream>

namespace pxl { namespace input {

    KeyEvent* keys = new KeyEvent[255];

    bool key_down(KeyCode key_code) {
	    return keys[key_code].key_down;
    }

    bool key_up(KeyCode key_code) {
	    return keys[key_code].key_down;
    }

}};