#include "graphics/PXL_FontUtils.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "graphics/PXL_Bitmap.h"
#include "graphics/PXL_Texture.h"
#include "graphics/PXL_Batch.h"
#include "graphics/PXL_Font.h"
#include "system/PXL_Exception.h"

FT_Library PXL_FT_lib;

void PXL_init_font() {
	if (FT_Init_FreeType(&PXL_FT_lib)) {
		PXL_show_exception("An error occurred while loading the freetype library");
	}else {
		std::cout << "freetype library loaded successfully\n";
	}
}