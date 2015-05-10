#include "graphics/PXL_Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "system/PXL_Debug.h"

PXL_Font::PXL_Font(std::string path, int c_max_font_size) {
	max_font_size = c_max_font_size;
	PXL_print << "attempting to load font...\n";
	if (!FT_New_Face(PXL_FT_lib, path.c_str(), 0, &f)) {
		FT_Set_Pixel_Sizes(f, max_font_size, 0);

		name = f->family_name;
		num_glyphs = f->num_glyphs;
		width = f->max_advance_width;
		height = f->max_advance_height;

		glyph_sheet = new PXL_TextureSheet();
		PXL_Rect rect;
		glyph_rects = new PXL_Rect[f->num_glyphs];
		f->style_flags = FT_STYLE_FLAG_BOLD;
		int glyphs_loaded = 0;
		for (int n = 0; n < f->num_glyphs; ++n) {
			FT_Load_Glyph(f, n, FT_LOAD_RENDER);
			if (f->glyph->bitmap.width == 0 || f->glyph->bitmap.rows == 0) { continue; }

			int size = f->glyph->bitmap.width * f->glyph->bitmap.rows;
			PXL_ubyte* pixels = new PXL_ubyte[size];
			memcpy(pixels, f->glyph->bitmap.buffer, size);
			PXL_Bitmap bitmap;
			bitmap.create_bitmap(f->glyph->bitmap.width, f->glyph->bitmap.rows, pixels, PXL_CHANNEL_ALPHA);

			max_char_width = PXL_max(max_char_width, bitmap.get_width());
			max_char_height = PXL_max(max_char_height, bitmap.get_height());

			rect.w = bitmap.get_width();
			rect.h = bitmap.get_height();
			PXL_Texture* t = new PXL_Texture();
			t->create_texture(&bitmap);
			glyph_sheet->add(t, &rect);
			glyph_rects[n] = rect;

			rect.x += bitmap.get_width();
			if (rect.x >= 1024) {
				rect.x = 0;
				rect.y = glyph_sheet->get_height();
			}
			++glyphs_loaded;
		}
		//create sheet from all textures added and then clear and dispose the textures
        glyph_sheet->create_sheet(PXL_CHANNEL_ALPHA, true, true, true);

		PXL_print << "loaded font (" << path << ") with " << glyphs_loaded << " glyphs loaded\n";
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
		delete[] glyph_rects;
		delete glyph_sheet;
	}
}

PXL_Font::~PXL_Font() {
	free();
}