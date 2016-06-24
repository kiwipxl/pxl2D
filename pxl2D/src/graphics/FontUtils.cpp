#include "graphics/FontUtils.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "graphics/Bitmap.h"
#include "graphics/Texture.h"
#include "graphics/Batch.h"
#include "graphics/Font.h"
#include "system/Exception.h"

FT_Library FT_lib;

namespace pxl { namespace graphics {

    void init_font() {
	    if (FT_Init_FreeType(&FT_lib)) {
            system::show_exception("An error occurred while loading the freetype library");
	    }else {
		    std::cout << "freetype library loaded successfully\n";
	    }
    }
}};