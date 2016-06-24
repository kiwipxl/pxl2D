#ifndef _FONT_H
#define _FONT_H

#include "graphics/FontUtils.h"
#include "graphics/TextureSheet.h"
#include "system/Math.h"

typedef struct FT_FaceRec_* FT_Face;

namespace pxl { namespace graphics {

    class Font {

	    public:
		    /**
		    \*brief: loads the font
		    \*param [path]: the path and file name for the font to load
		    **/
		    Font(std::string path, int c_max_font_size = 72);
		    /**
		    \*brief: font deconstructor
		    **/
		    ~Font();

		    std::string name;
		    int num_glyphs;
		    int width;
		    int height;

		    Texture* get_glyph_sheet() { return glyph_sheet; }

		    const Rect* get_glyph_rects() { return &glyph_rects[0]; }

		    int get_glyph_index(int char_code);
		    int get_max_font_size() { return max_font_size; }
		    int get_max_char_width() { return max_char_width; }
		    int get_max_char_height() { return max_char_height; }

		    /**
		    \*brief: frees all data from the font
		    **/
		    void free();

	    private:
		    //font info
		    bool font_loaded;
		    FT_Face f;
		    Rect* glyph_rects;
		    uint32 max_font_size;
		    uint32 max_char_width = 0;
		    uint32 max_char_height = 0;

		    TextureSheet* glyph_sheet; /**> Texture sheet containing all glyphs in this font **/
    };

    /**
    \*brief: loads and creates a font from the specified path
    \*param [path]: the path and file name for the font to load
    **/
    extern Font* create_font(std::string path, int c_max_font_size = 72);

}};

#endif