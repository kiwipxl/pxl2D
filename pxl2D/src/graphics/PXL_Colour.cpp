#include "PXL_Colour.h"
#include <iostream>

const PXL_Colour PXL_Colour::WHITE(0xffffffff);
const PXL_Colour PXL_Colour::BLACK(0x000000ff);
const PXL_Colour PXL_Colour::RED(0xff0000ff);
const PXL_Colour PXL_Colour::GREEN(0x00ff00ff);
const PXL_Colour PXL_Colour::BLUE(0x0000ffff);
const PXL_Colour PXL_Colour::YELLOW(0xffff00ff);
const PXL_Colour PXL_Colour::ORANGE(0xff9900ff);
const PXL_Colour PXL_Colour::PINK(0xff66ffff);
const PXL_Colour PXL_Colour::PURPLE(0xcc00ccff);
const PXL_Colour PXL_Colour::LIGHT_RED(0xff9999ff);
const PXL_Colour PXL_Colour::LIGHT_GREEN(0x99ff99ff);
const PXL_Colour PXL_Colour::LIGHT_BLUE(0x00ccffff);
const PXL_Colour PXL_Colour::DARK_RED(0x751919ff);
const PXL_Colour PXL_Colour::DARK_GREEN(0x19a347ff);
const PXL_Colour PXL_Colour::DARK_BLUE(0x000099ff);
const PXL_Colour PXL_Colour::TRANSPARENT_WHITE(0xffffff00);
const PXL_Colour PXL_Colour::TRANSPARENT_BLACK(0x00000000);

PXL_Colour::PXL_Colour(float red, float green, float blue, float alpha) {
	set_colour(red, green, blue, alpha);
}

PXL_Colour::PXL_Colour(unsigned int colour) {
	set_colour(colour);
}

void PXL_Colour::set_colour(float red, float green, float blue, float alpha) {
	r = red; g = green; b = blue; a = alpha;
}

void PXL_Colour::set_colour(unsigned int colour) {
	//mask r, g, b, a and move bits to the right to get value and then normalise
	r = ((colour & 0xff000000) >> 24) / 255.0f;
	g = ((colour & 0x00ff0000) >> 16) / 255.0f;
	b = ((colour & 0x0000ff00) >> 8) / 255.0f;
	a = (colour & 0x000000ff) / 255.0f;
}