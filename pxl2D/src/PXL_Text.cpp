#include "PXL_Text.h"
#include <iostream>

PXL_Text::PXL_Text(PXL_Font* f_font, string f_text, int f_pos_x, int f_pos_y, short f_size) {
	text_loaded = false;
	font = f_font;
	text = f_text;
	font_size = f_size;
	x = f_pos_x;
	y = f_pos_y;
	colour.r = colour.g = colour.b = 0;
	colour.a = 255;
	font_size = 24;
	set_origin(PXL_TOP_LEFT_ORIGIN);
}

PXL_Text* PXL_create_text(PXL_Font* f_font, string f_text, int f_pos_x, int f_pos_y, short f_size) {
	return new PXL_Text(f_font, f_text, f_pos_x, f_pos_y, f_size);
}

bool PXL_Text::calculate_char_pos(char symbol) {
	font_scale = font_size / float(font->max_font_size);

	if (symbol == ' ') {
		rect.x += (font->max_char_width * font_scale) + space_kerning;
		return true;
	}else if (symbol == '\n') {
		rect.x = x;
		rect.y += (font->max_char_height * font_scale) + vertical_kerning;
		return true;
	}
	src_rect = font->get_glyph_rects()[font->get_glyph_index(symbol)];
	rect.w = src_rect.w * font_scale;
	rect.h = src_rect.h * font_scale;

	return false;
}

void PXL_Text::set_origin(float x, float y) {
	origin_point_type = PXL_CUSTOM_ORIGIN;
	origin.x = x; origin.y = y;
}

void PXL_Text::set_origin(const PXL_TextOrigin origin_point) {
	origin_point_type = origin_point;

	rect.x = 0; rect.y = 0;
	width = 0; height = 0;
	for (int n = 0; n < text.length(); ++n) {
		if (calculate_char_pos(text[n])) { continue; }
		width = MAX(width, rect.x);
		height = MAX(height, rect.y);
		rect.x += rect.w + kerning;
	}

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
	batch->render_all();
	PXL_use_text_shader(batch);

	rect.x = x; rect.y = y;
	for (int n = 0; n < text.length(); ++n) {
		if (calculate_char_pos(text[n])) { continue; }
		temp_origin.x = (x + origin.x) - rect.x; temp_origin.y = (y + origin.y) - rect.y;
		rect.x += temp_origin.x; rect.y += temp_origin.y;
		batch->add(font->glyph_sheet, &rect, &src_rect, colour.r, colour.g, colour.g, colour.a, rotation, &temp_origin, PXL_FLIP_NONE);
		rect.x += rect.w + kerning;
		rect.x -= temp_origin.x; rect.y -= temp_origin.y;
	}
	batch->render_all();
}

void PXL_Text::free() {
	if (text_loaded) {
		text_loaded = false;
	}
}

PXL_Text::~PXL_Text() {
	free();
}