#include "PXL_Text.h"
#include <iostream>

PXL_Text::PXL_Text(PXL_Font* f_font, string f_text, short f_size, short f_kerning) {
	text_loaded = false;
	font = f_font;
	text = f_text;
	font_size = f_size;
	kerning = f_kerning;
}

PXL_Text* PXL_create_text(PXL_Font* f_font, string f_text, short f_size, short f_kerning) {
	return new PXL_Text(f_font, f_text, f_size, f_kerning);
}

void PXL_Text::render(PXL_Batch* batch) {
	float font_scale = font_size / float(font->max_font_size);
	rect.x = 0; rect.y = 0;
	for (int n = 0; n < text.length(); ++n) {
		if (text[n] == ' ') {
			rect.x += (font->max_char_width * font_scale) + space_kerning;
			continue;
		}else if (text[n] == '\n') {
			rect.x = 0;
			rect.y += (font->max_char_height * font_scale) + vertical_kerning;
			continue;
		}
		src_rect = font->get_glyph_rects()[font->get_glyph_index(text[n])];
		rect.w = src_rect.w * font_scale;
		rect.h = src_rect.h * font_scale;
		batch->add(font->glyph_sheet, &rect, &src_rect, 0, 255, 0, 120, PXL_FLIP_NONE);
		rect.x += rect.w + kerning;
	}
}

void PXL_Text::free() {
	if (text_loaded) {
		text_loaded = false;
	}
}

PXL_Text::~PXL_Text() {
	free();
}