#include "PXL_FontUtils.h"
#include <iostream>
#include "PXL.h"
#include "PXL_Bitmap.h"
#include "PXL_Texture.h"
#include "PXL_Batch.h"
#include "PXL_Font.h"
#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library PXL_FT_lib;

void log_font_error(string e);

void PXL_font_init() {
	if (FT_Init_FreeType(&PXL_FT_lib)) {
		log_font_error("an error occurred while loading the freetype library");
	}else {
		cout << "freetype library loaded successfully\n";
	}
}

void log_font_error(string e) {
	cout << "[FontUtils log error]: " << e << "\n";
}