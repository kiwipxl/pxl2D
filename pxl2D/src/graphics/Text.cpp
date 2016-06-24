#include "graphics/Text.h"
#include "system/Debug.h"

namespace pxl { namespace graphics {

    Text::Text() {
        init();
    }

    Text::Text(Font* f_font, std::string f_text, int f_pos_x, int f_pos_y, uint16 f_size) {
	    font = f_font;
	    text = f_text;
	    size = f_size;
	    x = f_pos_x;
	    y = f_pos_y;
	    colour.r = colour.g = colour.b = 0;
	    colour.a = 1;
	    set_scale(1, 1);

        init();
    }

    void Text::init() {
        scale_origin = Vec2(.5f, .5f);
    }

    bool Text::set_char_pos(int8 symbol, int start_x) {
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

    void Text::render(Batch* batch) {
	    scaled_max.x = max_width; scaled_max.y = max_height;
	    if (scale_max_size) { scaled_max.x = max_width * font_scale.x; scaled_max.y = max_height * font_scale.y; }

	    rect.x = x; rect.y = y;
	    float pos_x = 0; float pos_y = 0;
	    for (size_t n = 0; n < text.length(); ++n) {
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
				    src_rect.w = math::min(src_rect.w, scaled_max.x);
				    rect.w = src_rect.w * font_scale.x;
			    }
			    //cut off texture height if it goes over the max height
			    if (pos_y >= scaled_max.y) {
				    src_rect.h = src_rect.h - fabs(scaled_max.y - pos_y);
				    src_rect.h = math::min(src_rect.h, scaled_max.y);
				    rect.h = src_rect.h * font_scale.y;
			    }
		    }

		    temp_origin.x = (x + origin.x) - rect.x; temp_origin.y = (y + origin.y) - rect.y;
		    batch->add(*font->get_glyph_sheet(), &rect, &src_rect, rotation, &temp_origin, NULL, z_depth, colour, text_shader);
		    rect.x += offset_x;
	    }
    }

    void Text::free() {

    }

    Text::~Text() {
	    free();
    }

}};