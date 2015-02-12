#include "PXL_Text.h"
#include <iostream>

PXL_Text::PXL_Text(PXL_Font* f_font, std::string f_text, int f_pos_x, int f_pos_y, short f_size) {
	text_loaded = false;
	font = f_font;
	text = f_text;
	size = f_size;
	x = f_pos_x;
	y = f_pos_y;
	colour.r = colour.g = colour.b = 0;
	colour.a = 1;
	set_scale(1, 1);
}

PXL_Text* PXL_create_text(PXL_Font* f_font, std::string f_text, int f_pos_x, int f_pos_y, short f_size) {
	return new PXL_Text(f_font, f_text, f_pos_x, f_pos_y, f_size);
}

bool PXL_Text::set_char_pos(PXL_byte symbol, int start_x) {
	src_rect = font->get_glyph_rects()[font->get_glyph_index(symbol)];
	rect.w = src_rect.w * font_scale.x;
	rect.h = src_rect.h * font_scale.y;
	if (symbol == ' ') {
		rect.x += ((font->get_max_char_width() * font_scale.x) / 2) + spacing_kerning;
		return true;
	}else if (symbol == '\n') {
		rect.x = start_x;
		rect.y += (font->get_max_char_height() * font_scale.y) + vertical_kerning;
		return true;
	}else if (src_rect.w <= 1 && src_rect.h <= 1) {
		return true;
	}

	return false;
}

void PXL_Text::set_origin(float x, float y) {
	origin_type = PXL_CUSTOM_ORIGIN;
	origin.x = x; origin.y = y;
}

void PXL_Text::set_origin(const PXL_TextOrigin origin_point) {
	origin_type = origin_point;

	rect.x = 0; rect.y = 0;
	width = 0; height = 0;
	for (int n = 0; n < text.length(); ++n) {
		bool special_symbol_found = set_char_pos(text[n], 0);
		width = PXL_max(width, rect.x);
		height = PXL_max(height, rect.y);
		if (special_symbol_found) { continue; }
		rect.x += rect.w + kerning;
	}

	height += font->get_max_char_height() * font_scale.y;
	if (clamp_max_size) { width = PXL_min(width, max_width); height = PXL_min(height, max_height); }

	switch (origin_point) {
		case PXL_TOP_LEFT_ORIGIN:
			origin.x = 0; origin.y = 0;
			break;
		case PXL_TOP_CENTER_ORIGIN:
			origin.x = width / 2; origin.y = 0;
			break;
		case PXL_TOP_RIGHT_ORIGIN:
			origin.x = width; origin.y = 0;
			break;
		case PXL_MID_LEFT_ORIGIN:
			origin.x = 0; origin.y = height / 2;
			break;
		case PXL_CENTER_ORIGIN:
			origin.x = width / 2; origin.y = height / 2;
			break;
		case PXL_MID_RIGHT_ORIGIN:
			origin.x = width; origin.y = height / 2;
			break;
		case PXL_BOT_LEFT_ORIGIN:
			origin.x = 0; origin.y = height;
			break;
		case PXL_BOT_CENTER_ORIGIN:
			origin.x = width / 2; origin.y = height;
			break;
		case PXL_BOT_RIGHT_ORIGIN:
			origin.x = width; origin.y = height;
			break;
	}
}

void PXL_Text::render(PXL_Batch* batch) {
	scaled_max.x = max_width; scaled_max.y = max_height;
	if (scale_max_size) { scaled_max.x = max_width * font_scale.x; scaled_max.y = max_height * font_scale.y; }

	rect.x = x; rect.y = y;
	float pos_x = 0; float pos_y = 0;
	for (int n = 0; n < text.length(); ++n) {
		if (set_char_pos(text[n], x)) { continue; }

		float offset_x = rect.w + kerning;
		if (max_width != INT_MAX || max_height != INT_MAX) {
			pos_x = (rect.x - x) + rect.w;
			pos_y = (rect.y - y) + rect.h;
			if (pos_x - rect.w >= scaled_max.x) { continue; }
			if (pos_y - rect.h >= scaled_max.y) { continue; }

			//cut off texture width if it goes over the max width
			if (pos_x >= scaled_max.x) {
				src_rect.w = src_rect.w - fabs(scaled_max.x - pos_x);
				src_rect.w = PXL_min(src_rect.w, scaled_max.x);
				rect.w = src_rect.w * font_scale.x;
			}
			//cut off texture height if it goes over the max height
			if (pos_y >= scaled_max.y) {
				src_rect.h = src_rect.h - fabs(scaled_max.y - pos_y);
				src_rect.h = PXL_min(src_rect.h, scaled_max.y);
				rect.h = src_rect.h * font_scale.y;
			}
		}

		temp_origin.x = (x + origin.x) - rect.x; temp_origin.y = (y + origin.y) - rect.y;
		rect.x += temp_origin.x; rect.y += temp_origin.y;
		batch->add(font->glyph_sheet, &rect, &src_rect, rotation, &temp_origin, PXL_FLIP_NONE, 
				   z_depth, colour.r, colour.g, colour.g, colour.a, PXL_text_shader);
		rect.x += offset_x;
		rect.x -= temp_origin.x; rect.y -= temp_origin.y;
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