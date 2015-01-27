#include "PXL_FontUtils.h"
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "PXL_Bitmap.h"
#include "PXL_Texture.h"
#include "PXL_Batch.h"
#include "PXL_Font.h"
#include "system/PXL_Exception.h"

FT_Library PXL_FT_lib;

void PXL_font_init() {
	if (FT_Init_FreeType(&PXL_FT_lib)) {
		PXL_show_exception("An error occurred while loading the freetype library");
	}else {
		std::cout << "freetype library loaded successfully\n";
	}
}