#include "graphics/Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "system/Debug.h"

namespace pxl { namespace graphics {

    Font::Font(std::string path, int c_max_font_size) {
        FT_Face a;
	    max_font_size = c_max_font_size;
        sys::print << "attempting to load font...\n";
	    if (!FT_New_Face(FT_lib, path.c_str(), 0, &a)) {
		    FT_Set_Pixel_Sizes(f, max_font_size, 0);

		    name = f->family_name;
		    num_glyphs = f->num_glyphs;
		    width = f->max_advance_width;
		    height = f->max_advance_height;

		    glyph_sheet = new TextureSheet();
		    Rect rect;
		    glyph_rects = new Rect[f->num_glyphs];
		    f->style_flags = FT_STYLE_FLAG_BOLD;
		    int glyphs_loaded = 0;
		    for (int n = 0; n < f->num_glyphs; ++n) {
			    FT_Load_Glyph(f, n, FT_LOAD_RENDER);
			    if (f->glyph->bitmap.width == 0 || f->glyph->bitmap.rows == 0) { continue; }

			    int size = f->glyph->bitmap.width * f->glyph->bitmap.rows;
			    uint8* pixels = new uint8[size];
			    memcpy(pixels, f->glyph->bitmap.buffer, size);
			    Bitmap bitmap;
			    bitmap.create_bitmap(f->glyph->bitmap.width, f->glyph->bitmap.rows, pixels, CHANNEL_ALPHA);

			    max_char_width = math::max(max_char_width, bitmap.get_width());
			    max_char_height = math::max(max_char_height, bitmap.get_height());

			    rect.w = bitmap.get_width();
			    rect.h = bitmap.get_height();
			    Texture* t = new Texture();
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
            glyph_sheet->create_sheet(CHANNEL_ALPHA, true, true, true);

		    sys::print << "loaded font (" << path << ") with " << glyphs_loaded << " glyphs loaded\n";
	    }
    }

    int Font::get_glyph_index(int char_code) {
	    return FT_Get_Char_Index(f, char_code);
    }

    Font* create_font(std::string path, int c_max_font_size) {
	    return new Font(path, c_max_font_size);
    }

    void Font::free() {
	    if (font_loaded) {
		    font_loaded = false;
		    delete[] glyph_rects;
		    delete glyph_sheet;
	    }
    }

    Font::~Font() {
	    free();
    }
}};