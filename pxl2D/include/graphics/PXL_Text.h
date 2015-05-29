#ifndef _PXL_TEXT_H
#define _PXL_TEXT_H

#include <string>
#include "graphics/PXL_FontUtils.h"
#include "graphics/PXL_Batch.h"
#include "graphics/PXL_Sprite.h"
#include "system/PXL_API.h"

class PXL_TextOrigin;

class PXL_Text : public PXL_Sprite {

	public:
		PXL_Text() : scale_origin(this) { }
		/**
		\*brief: text constructor
		**/
		PXL_Text(PXL_Font* f_font, std::string f_text, 
				 int f_pos_x = 0, int f_pos_y = 0, uint16 f_size = 24);
		/**
		\*brief: text deconstructor
		**/
		~PXL_Text();

		bool text_loaded;				/*> Defines whether or not the text has been loaded or not */
		PXL_Font* font;
		int max_width = INT_MAX;
		int max_height = INT_MAX;
		bool scale_max_size = true;
		bool clamp_max_size = true;
		PXL_Origin scale_origin;

		void scale(float scale_x, float scale_y) {
			text_scale.x += scale_x; text_scale.y += scale_y; update_scale_origin(); set_font_scale();
		}
		void set_scale(float scale_x, float scale_y) {
			text_scale.x = scale_x; text_scale.y = scale_y; update_scale_origin(); set_font_scale();
		}
		PXL_Vec2 get_scale() {
			return text_scale;
		}

		void set_text(std::string new_text = "") { text = new_text; update_scale_origin(); }
		std::string get_text() { return text; }

		void set_size(short c_size) { size = c_size; update_scale_origin(); set_font_scale(); }
		short get_size() { return size; }

		float get_width() { return width; }
		float get_height() { return height; }

		void set_kerning(short c_kerning) { kerning = c_kerning; update_scale_origin(); }
		short get_kerning() { return kerning; }

		void set_spacing_kerning(short c_spacing_kerning) { spacing_kerning = c_spacing_kerning; update_scale_origin(); }
		short get_spacing_kerning() { return spacing_kerning; }

		void set_vertical_kerning(short c_vertical_kerning) { vertical_kerning = c_vertical_kerning; update_scale_origin(); }
		short get_vertical_kerning() { return vertical_kerning; }

		/**
		\*brief: frees all data from the text
		**/
		void render(PXL_Batch* batch) override;

		/**
		\*brief: frees all data from the text
		**/
		void free() override;

	protected:
		std::string text = "";			/*> The text to be rendered */
		short size;						/*> The size of the text to be rendered */
		float width = 0;				/*> The width boundaries of the text */
		float height = 0;				/*> The height boundaries of the text */
		PXL_Rect rect;					/*> The rendering boundaries */
		PXL_Rect src_rect;				/*> The texture source rendering boundaries */
		PXL_Vec2 origin;				/*> The origin point of the text to perform rotation and scaling transformations */
		PXL_Vec2 temp_origin;			/*> The origin used to calculate when rendering */
		PXL_Vec2 font_scale;			/*> The scale of the font texture */
		PXL_Vec2 text_scale;			/*> The scale of the text */
		PXL_Vec2 scaled_max;			/*> The max scale */
		short kerning = 4;				/*> The number that specifies the spacing between each character */
		short spacing_kerning = 0;		/*> The number that specifies the spacing between spaces */
		short vertical_kerning = 4;		/*> The number that specifies the spacing for new lines */

		bool set_char_pos(int8 symbol, int start_x);

		void set_font_scale() {
			font_scale.x = (size / float(font->get_max_font_size())) * text_scale.x;
			font_scale.y = (size / float(font->get_max_font_size())) * text_scale.y;
		}

		void update_scale_origin() {
			//calculate width and height of the whole text
			rect.x = 0; rect.y = 0;
			width = 0; height = 0;
			for (size_t n = 0; n < text.length(); ++n) {
				bool special_symbol_found = set_char_pos(text[n], 0);
				width = PXL_max(width, rect.x);
				height = PXL_max(height, rect.y);
				if (special_symbol_found) { continue; }
				rect.x += rect.w + kerning;
			}

			height += font->get_max_char_height() * font_scale.y;
			if (clamp_max_size) { width = PXL_min(width, max_width); height = PXL_min(height, max_height); }

			//set scale origin based on the current scale origin type
			scale_origin.set(scale_origin.get_type());
		}
};

#endif