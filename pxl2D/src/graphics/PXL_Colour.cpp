#include "graphics/PXL_Colour.h"
#include "system/PXL_Math.h"

const PXL_Colour PXL_COLOUR_WHITE				= PXL_Colour(0xffffffff);	/**> Pre-defined white colour **/
const PXL_Colour PXL_COLOUR_BLACK				= PXL_Colour(0x000000ff);	/**> Pre-defined black colour **/
const PXL_Colour PXL_COLOUR_RED					= PXL_Colour(0xff0000ff);	/**> Pre-defined red colour **/
const PXL_Colour PXL_COLOUR_GREEN				= PXL_Colour(0x00ff00ff);	/**> Pre-defined green colour **/
const PXL_Colour PXL_COLOUR_BLUE				= PXL_Colour(0x0000ffff);	/**> Pre-defined blue colour **/
const PXL_Colour PXL_COLOUR_YELLOW				= PXL_Colour(0xffff00ff);	/**> Pre-defined yellow colour **/
const PXL_Colour PXL_COLOUR_ORANGE				= PXL_Colour(0xff9900ff);	/**> Pre-defined orange colour **/
const PXL_Colour PXL_COLOUR_PINK				= PXL_Colour(0xff66ffff);	/**> Pre-defined pink colour **/
const PXL_Colour PXL_COLOUR_PURPLE				= PXL_Colour(0xcc00ccff);	/**> Pre-defined purple colour **/
const PXL_Colour PXL_COLOUR_LIGHT_RED			= PXL_Colour(0xff9999ff);	/**> Pre-defined light red colour **/
const PXL_Colour PXL_COLOUR_LIGHT_GREEN			= PXL_Colour(0x99ff99ff);	/**> Pre-defined light green colour **/
const PXL_Colour PXL_COLOUR_LIGHT_BLUE			= PXL_Colour(0x00ccffff);	/**> Pre-defined light blue colour **/
const PXL_Colour PXL_COLOUR_DARK_RED			= PXL_Colour(0x751919ff);	/**> Pre-defined dark red colour **/
const PXL_Colour PXL_COLOUR_DARK_GREEN			= PXL_Colour(0x19a347ff);	/**> Pre-defined dark green colour **/
const PXL_Colour PXL_COLOUR_DARK_BLUE			= PXL_Colour(0x000099ff);	/**> Pre-defined dark blue colour **/
const PXL_Colour PXL_COLOUR_TRANSPARENT_WHITE	= PXL_Colour(0xffffff00);	/**> Pre-defined fully transparent white colour **/
const PXL_Colour PXL_COLOUR_TRANSPARENT_BLACK	= PXL_Colour(0x00000000);	/**> Pre-defined fully transparent black colour **/

PXL_Colour::PXL_Colour(float red, float green, float blue, float alpha) {
	set_colour(red, green, blue, alpha);
}

PXL_Colour::PXL_Colour(uint32 colour) {
	set_colour(colour);
}

void PXL_Colour::set_colour(float red, float green, float blue, float alpha) {
	r = red; g = green; b = blue; a = alpha;
}

void PXL_Colour::set_colour(uint32 colour) {
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