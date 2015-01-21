#ifndef TEXT_H
#define TEXT_H

#include <string>
#include "PXL_FontUtils.h"
#include "PXL_Batch.h"

using namespace std;

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
		PXL_Text(PXL_Font* f_font, string f_text, 
				 int f_pos_x = 0, int f_pos_y = 0, short f_size = 24);
		/**
		\*brief: text deconstructor
		**/
		~PXL_Text();

		PXL_Font* font;
		string text = "";
		int x;
		int y;
		short font_size;
		float rotation = 0;
		PXL_RGBA colour;
		short kerning = 4;
		short space_kerning = 0;
		short vertical_kerning = 0;
		PXL_TextOrigin origin_point_type = PXL_TOP_LEFT_ORIGIN;

		void rotate(float degrees);

		void set_rotation(float degrees);

		void set_origin(float x = 0, float y = 0);
		void set_origin(const PXL_TextOrigin origin_point = PXL_TOP_LEFT_ORIGIN);

		void set_text(string new_text = "");

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
		PXL_Vec2 origin;
		PXL_Vec2 temp_origin;
		float font_scale;

		void calculate_char_pos(char symbol);
};

/**
\*brief: loads and creates a text from the specified path
\*param [path]: the path and file name for the text to load
**/
extern PXL_Text* PXL_create_text(PXL_Font* f_font, string c_text, 
								 int f_pos_x = 0, int f_pos_y = 0, short f_size = 24, short f_kerning = 4);

#endif