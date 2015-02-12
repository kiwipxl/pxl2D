#ifndef _PXL_TEXT_H
#define _PXL_TEXT_H

#include <string>
#include "PXL_FontUtils.h"
#include "PXL_Batch.h"
#include "system/PXL_API.h"

enum PXL_TextOrigin {
	PXL_TOP_LEFT_ORIGIN,
	PXL_TOP_CENTER_ORIGIN,
	PXL_TOP_RIGHT_ORIGIN,
	PXL_MID_LEFT_ORIGIN,
	PXL_CENTER_ORIGIN,
	PXL_MID_RIGHT_ORIGIN,
	PXL_BOT_LEFT_ORIGIN,
	PXL_BOT_CENTER_ORIGIN,
	PXL_BOT_RIGHT_ORIGIN, 
	PXL_CUSTOM_ORIGIN
};

class PXL_Text {

	public:
		/**
		\*brief: text constructor
		**/
		PXL_Text(PXL_Font* f_font, std::string f_text, 
				 int f_pos_x = 0, int f_pos_y = 0, short f_size = 24);
		/**
		\*brief: text deconstructor
		**/
		~PXL_Text();

		PXL_Font* font;
		int x;
		int y;
		int max_width = INT_MAX;
		int max_height = INT_MAX;
		bool scale_max_size = true;
		bool clamp_max_size = true;
		float rotation = 0;
		int z_depth = 0;
		PXL_Colour colour;
		PXL_TextOrigin origin_type = PXL_TOP_LEFT_ORIGIN;

		void scale(float scale_x, float scale_y) {
			text_scale.x += scale_x; text_scale.y += scale_y; set_origin(origin_type); set_font_scale();
		}
		void set_scale(float scale_x, float scale_y) {
			text_scale.x = scale_x; text_scale.y = scale_y; set_origin(origin_type); set_font_scale();
		}
		void set_font_scale() {
			font_scale.x = (size / float(font->get_max_font_size())) * text_scale.x;
			font_scale.y = (size / float(font->get_max_font_size())) * text_scale.y;
		}
		PXL_Vec2 get_scale() {
			return text_scale;
		}

		void set_colour(float r, float g, float b, float a) {
			colour.r = r;
			colour.g = g;
			colour.b = b;
			colour.a = a;
		}

		void set_origin(float x = 0, float y = 0);
		void set_origin(const PXL_TextOrigin origin_point = PXL_TOP_LEFT_ORIGIN);

		void set_text(std::string new_text = "") { text = new_text; set_origin(origin_type); }
		std::string get_text() { return text; }

		void set_size(short c_size) { size = c_size; set_origin(origin_type); set_font_scale(); }
		short get_size() { return size; }

		float get_width() { return width; }
		float get_height() { return height; }

		void set_kerning(short c_kerning) { kerning = c_kerning; set_origin(origin_type); }
		short get_kerning() { return kerning; }

		void set_spacing_kerning(short c_spacing_kerning) { spacing_kerning = c_spacing_kerning; set_origin(origin_type); }
		short get_spacing_kerning() { return spacing_kerning; }

		void set_vertical_kerning(short c_vertical_kerning) { vertical_kerning = c_vertical_kerning; set_origin(origin_type); }
		short get_vertical_kerning() { return vertical_kerning; }

		/**
		\*brief: frees all data from the text
		**/
		void render(PXL_Batch* batch);

		/**
		\*brief: frees all data from the text
		**/
		void free();

	private:
		bool text_loaded; /*> Defines whether or not the text has been loaded or not*/
		std::string text = ""; /*> The text to be rendered*/
		short size; /*> The size of the text to be rendered*/
		float width = 0; /*> The width boundaries of the text*/
		float height = 0; /*> The height boundaries of the text*/
		PXL_Rect rect; /*> The rendering boundaries*/
		PXL_Rect src_rect; /*> The texture source rendering boundaries*/
		PXL_Vec2 origin; /*> The origin point of the text to perform rotation and scaling transformations*/
		PXL_Vec2 temp_origin; /*> The origin used to calculate when rendering*/
		PXL_Vec2 font_scale; /*> The scale of the font texture*/
		PXL_Vec2 text_scale; /*> The scale of the text*/
		PXL_Vec2 scaled_max; /*> The max scale*/
		short kerning = 4; /*> The number that specifies the spacing between each character*/
		short spacing_kerning = 0; /*> The number that specifies the spacing between spaces*/
		short vertical_kerning = 4; /*> The number that specifies the spacing for new lines*/

		bool set_char_pos(PXL_byte symbol, int start_x);
};

/**
\*brief: loads and creates a text from the specified path
\*param [path]: the path and file name for the text to load
**/
extern PXL_Text* PXL_create_text(PXL_Font* f_font, std::string c_text, 
								 int f_pos_x = 0, int f_pos_y = 0, short f_size = 24, short f_kerning = 4);

#endif