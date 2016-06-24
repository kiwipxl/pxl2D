#include "Graphics.h"

namespace pxl {

    using namespace graphics;

    void graphics_init() {
	    set_clear_colour(1, 1, 1, 1);
	    set_clear_depth(1);
	    clear();

	    init_shader();
	    init_font();
    }
};