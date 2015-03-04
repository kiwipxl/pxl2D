#include "PXL_Colour.h"
#include <iostream>
#include "system/PXL_Math.h"

PXL_Colour::PXL_Colour(float red, float green, float blue, float alpha) {
	set_colour(red, green, blue, alpha);
}

PXL_Colour::PXL_Colour(PXL_uint colour) {
	set_colour(colour);
}

void PXL_Colour::set_colour(float red, float green, float blue, float alpha) {
	r = red; g = green; b = blue; a = alpha;
}

void PXL_Colour::set_colour(PXL_uint colour) {
	//mask r, g, b, a and move bits to the right to get value and then normalise
	r = ((colour & 0xff000000) >> 24) / 255.0f;
	g = ((colour & 0x00ff0000) >> 16) / 255.0f;
	b = ((colour & 0x0000ff00) >> 8) / 255.0f;
	a = (colour & 0x000000ff) / 255.0f;
}

void PXL_Colour::clamp(float min, float max, bool clamp_alpha) {
	r = PXL_clamp(r, min, max);
	g = PXL_clamp(g, min, max);
	b = PXL_clamp(b, min, max);
	if (clamp_alpha) a = PXL_clamp(a, min, max);
}