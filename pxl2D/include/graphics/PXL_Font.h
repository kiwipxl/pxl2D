#ifndef _PXL_FONT_H
#define _PXL_FONT_H

#include "PXL_FontUtils.h"
#include "PXL_TextureSheet.h"
#include "system/PXL_Math.h"

typedef struct FT_FaceRec_* FT_Face;

class PXL_Font {

	public:
		/**
		\*brief: loads the font
		\*param [path]: the path and file name for the font to load
		**/
		PXL_Font(std::string path, int c_max_font_size = 72);
		/**
		\*brief: font deconstructor
		**/
		~PXL_Font();

		std::string name;
		int num_glyphs;
		int width;
		int height;

		PXL_Texture* get_glyph_sheet() { return glyph_sheet; }

		const PXL_Rect* get_glyph_rects() { return &glyph_rects[0]; }

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
		PXL_Rect* glyph_rects;
		PXL_uint max_font_size;
		PXL_uint max_char_width = 0;
		PXL_uint max_char_height = 0;

		PXL_TextureSheet* glyph_sheet; /**> Texture sheet containing all glyphs in this font **/
};

/**
\*brief: loads and creates a font from the specified path
\*param [path]: the path and file name for the font to load
**/
extern PXL_Font* PXL_create_font(std::string path, int c_max_font_size = 72);

#endif