#include "PXL_Font.h"
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

PXL_Font::PXL_Font(string path, int c_max_font_size) {
	max_font_size = c_max_font_size;
	if (!FT_New_Face(PXL_FT_lib, path.c_str(), 0, &f)) {
		FT_Set_Pixel_Sizes(f, max_font_size, 0);
		cout << "charmaps: " << f->num_charmaps << "\n";
		cout << "num glyphs: " << f->num_glyphs << "\n";

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
			PXL_PixelBuffer* pix_buf = new PXL_PixelBuffer();
			pix_buf->width = f->glyph->bitmap.width;
			pix_buf->height = f->glyph->bitmap.rows;
			pix_buf->max_width = pix_buf->width;
			pix_buf->max_height = pix_buf->height;

			max_char_width = MAX(max_char_width, pix_buf->max_width);
			max_char_height = MAX(max_char_height, pix_buf->max_height);

			pix_buf->channels = 1;
			pix_buf->buffer_size = pix_buf->width * pix_buf->height;
			pix_buf->buffer = new unsigned char[pix_buf->buffer_size];
			memcpy(pix_buf->buffer, f->glyph->bitmap.buffer, pix_buf->buffer_size);

			rect.w = pix_buf->width;
			rect.h = pix_buf->height;
			glyph_sheet->add(pix_buf, &rect);
			glyph_rects[n] = rect;

			rect.x += pix_buf->width;
			if (rect.x >= 1024) {
				rect.x = 0;
				rect.y = glyph_sheet->get_height();
			}
		}
		glyph_sheet->create();
		glyph_sheet->delete_pixel_vec();
	}
}

int PXL_Font::get_glyph_index(int char_code) {
	return FT_Get_Char_Index(f, char_code);
}

PXL_Font* PXL_create_font(string path, int c_max_font_size) {
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