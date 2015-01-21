#ifndef TEXT_H
#define TEXT_H

#include <string>
#include "PXL_FontUtils.h"
#include "PXL_Batch.h"

using namespace std;

class PXL_Text {

	public:
		/**
		\*brief: text constructor
		**/
		PXL_Text(PXL_Font* f_font, string f_text, short f_size = 24, short f_kerning = 4);
		/**
		\*brief: text deconstructor
		**/
		~PXL_Text();

		PXL_Font* font;
		string text = "";
		short font_size;
		short kerning = 0;
		short space_kerning = 0;
		short vertical_kerning = 0;

		/**
		\*brief: frees all data from the text
		**/
		void render(PXL_Batch* batch);

		/**
		\*brief: frees all data from the text
		**/
		void free();

	private:
		//text info
		bool text_loaded;
		PXL_Rect rect;
		PXL_Rect src_rect;
};

/**
\*brief: loads and creates a text from the specified path
\*param [path]: the path and file name for the text to load
**/
extern PXL_Text* PXL_create_text(PXL_Font* f_font, string c_text, short f_size = 24, short f_kerning = 4);

#endif