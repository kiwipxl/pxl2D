#ifndef _PXL_STRUCTS_H
#define _PXL_STRUCTS_H

struct PXL_Rect {

	float x = 0, y = 0, w = 0, h = 0;
};

struct PXL_Vec2 {

	float x = 0, y = 0;
};

struct PXL_Vec3 {

	float x = 0, y = 0, z = 0;
};

struct PXL_Vec4 {

	float x = 0, y = 0, z = 0, w = 0;
};

struct PXL_RGBA {

	float r = 1, g = 1, b = 1, a = 1;
};

#define PXL_Color	PXL_Colour

struct PXL_Colour {

	PXL_Colour() { }

	PXL_Colour(float red, float green, float blue, float alpha = 1) {
		set_colour(red, green, blue, alpha);
	}

	PXL_Colour(unsigned int colour) {
		set_colour(colour);
	}

	float r = 1, g = 1, b = 1, a = 1;

	void set_colour(float red, float green, float blue, float alpha = 1) {
		r = red; g = green; b = blue; a = alpha;
	}

	void set_colour(unsigned int colour) {
		//mask r, g, b, a and move bits to the right to get value
		r = ((colour & 0xff000000) >> 24) / 255.0f;
		g = ((colour & 0x00ff0000) >> 16) / 255.0f;
		b = ((colour & 0x0000ff00) >> 8) / 255.0f;
		a = (colour & 0x000000ff) / 255.0f;
	}
};

static const PXL_Colour PXL_colour_white(0xffffffff); /**> Supports blending when rendering **/
static const PXL_Colour PXL_colour_black(0x000000ff); /**> Supports blending when rendering **/
static const PXL_Colour PXL_colour_red(0xff0000ff); /**> Supports blending when rendering **/
static const PXL_Colour PXL_colour_green(0x00ff00ff); /**> Supports blending when rendering **/
static const PXL_Colour PXL_colour_blue(0x0000ffff); /**> Supports blending when rendering **/
static const PXL_Colour PXL_colour_yellow(0xffff00ff); /**> Supports blending when rendering **/
static const PXL_Colour PXL_colour_orange(0xff9900ff); /**> Supports blending when rendering **/
static const PXL_Colour PXL_colour_pink(0xff66ffff); /**> Supports blending when rendering **/
static const PXL_Colour PXL_colour_purple(0xcc00ccff); /**> Supports blending when rendering **/
static const PXL_Colour PXL_colour_light_red(0xff9999ff); /**> Supports blending when rendering **/
static const PXL_Colour PXL_colour_light_green(0x99ff99ff); /**> Supports blending when rendering **/
static const PXL_Colour PXL_colour_light_blue(0x00ccffff); /**> Supports blending when rendering **/
static const PXL_Colour PXL_colour_dark_red(0x751919ff); /**> Supports blending when rendering **/
static const PXL_Colour PXL_colour_dark_green(0x19a347ff); /**> Supports blending when rendering **/
static const PXL_Colour PXL_colour_dark_blue(0x000099ff); /**> Supports blending when rendering **/
static const PXL_Colour PXL_colour_transparent_white(0xffffff00); /**> Supports blending when rendering **/
static const PXL_Colour PXL_colour_transparent_black(0x00000000); /**> Supports blending when rendering **/

#endif