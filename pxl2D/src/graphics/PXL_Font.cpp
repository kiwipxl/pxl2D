#include "PXL_Font.h"
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

PXL_Font::PXL_Font(std::string path, int c_max_font_size) {
	max_font_size = c_max_font_size;
	if (!FT_New_Face(PXL_FT_lib, path.c_str(), 0, &f)) {
		FT_Set_Pixel_Sizes(f, max_font_size, 0);
		std::cout << "charmaps: " << f->num_charmaps << "\n";
		std::cout << "num glyphs: " << f->num_glyphs << "\n";

		name = f->family_name;
		num_glyphs = f->num_glyphs;
		width = f->max_advance_width;
		height = f->max_advance_height;

		glyph_sheet = new PXL_TextureSheet();
		PXL_Rect rect;
		glyph_rects = new PXL_Rect[f->num_glyphs];
		f->style_flags = FT_STYLE_FLAG_BOLD;
		for (int n = 0; n < f->num_glyphs; ++n) {
			FT_Load_Glyph(f, n, FT_LOAD_RENDER);
			PXL_Bitmap* bitmap = new PXL_Bitmap();
			bitmap->width = f->glyph->bitmap.width;
			bitmap->height = f->glyph->bitmap.rows;

			max_char_width = PXL_max(max_char_width, bitmap->width);
			max_char_height = PXL_max(max_char_height, bitmap->height);

			bitmap->num_channels = 1;
			bitmap->size = bitmap->width * bitmap->height;
			bitmap->pixels = new unsigned char[bitmap->size];
			memcpy(bitmap->pixels, f->glyph->bitmap.buffer, bitmap->size);

			rect.w = bitmap->width;
			rect.h = bitmap->height;
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glyph_sheet->add(new PXL_Texture(bitmap, GL_ALPHA), &rect);
			glyph_rects[n] = rect;

			rect.x += bitmap->width;
			if (rect.x >= 1024) {
				rect.x = 0;
				rect.y = glyph_sheet->get_height();
			}
		}
		glyph_sheet->create_sheet(true);
	}
}

int PXL_Font::get_glyph_index(int char_code) {
	return FT_Get_Char_Index(f, char_code);
}

PXL_Font* PXL_create_font(std::string path, int c_max_font_size) {
	return new PXL_Font(path, c_max_font_size);
}

void PXL_Font::free() {
	if (font_loaded) {
		font_loaded = false;
	}
}

PXL_Font::~PXL_Font() {
	free();
}