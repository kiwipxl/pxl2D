#ifndef _TEXT_H
#define _TEXT_H

#include <string>
#include "graphics/FontUtils.h"
#include "graphics/Batch.h"
#include "graphics/Sprite.h"
#include "PXLAPI.h"

namespace pxl { namespace graphics {

    class TextOrigin;

    class Text : public Sprite {

	    public:
            Text();
		    /**
		    \*brief: text constructor
		    **/
		    Text(Font* f_font, std::string f_text, 
				     int f_pos_x = 0, int f_pos_y = 0, uint16 f_size = 24);
		    /**
		    \*brief: text deconstructor
		    **/
		    ~Text();

		    bool text_loaded;				/*> Defines whether or not the text has been loaded or not */
		    Font* font;
		    int max_width = INT_MAX;
		    int max_height = INT_MAX;
		    bool scale_max_size = true;
		    bool clamp_max_size = true;
		    Vec2 scale_origin;

            void init();

		    void scale(float scale_x, float scale_y) {
			    text_scale.x += scale_x; text_scale.y += scale_y; update_scale_origin(); set_font_scale();
		    }
		    void set_scale(float scale_x, float scale_y) {
			    text_scale.x = scale_x; text_scale.y = scale_y; update_scale_origin(); set_font_scale();
		    }
		    Vec2 get_scale() {
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
		    void render(Batch* batch) override;

		    /**
		    \*brief: frees all data from the text
		    **/
		    void free() override;

	    protected:
		    std::string text = "";			/*> The text to be rendered */
		    short size;						/*> The size of the text to be rendered */
		    float width = 0;				/*> The width boundaries of the text */
		    float height = 0;				/*> The height boundaries of the text */
		    Rect rect;					/*> The rendering boundaries */
		    Rect src_rect;				/*> The texture source rendering boundaries */
		    Vec2 origin;				/*> The origin point of the text to perform rotation and scaling transformations */
		    Vec2 temp_origin;			/*> The origin used to calculate when rendering */
		    Vec2 font_scale;			/*> The scale of the font texture */
		    Vec2 text_scale;			/*> The scale of the text */
		    Vec2 scaled_max;			/*> The max scale */
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
				    width = math::max(width, rect.x);
				    height = math::max(height, rect.y);
				    if (special_symbol_found) { continue; }
				    rect.x += rect.w + kerning;
			    }

			    height += font->get_max_char_height() * font_scale.y;
			    if (clamp_max_size) { width = math::min(width, max_width); height = math::min(height, max_height); }
		    }
    };
}};

#endif