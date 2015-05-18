#ifndef _PXL_COLOUR_H
#define _PXL_COLOUR_H

#include "system/PXL_API.h"

#define PXL_Color PXL_Colour

struct PXL_Colour {

	PXL_Colour() { }
	PXL_Colour(float red, float green, float blue, float alpha = 1);
	PXL_Colour(uint32 colour);

	float r = 1, g = 1, b = 1, a = 1;

	void set_colour(float red, float green, float blue, float alpha = 1);
	void set_colour(uint32 colour);

	void clamp(float min = 0, float max = 1, bool clamp_alpha = true);

	PXL_Colour operator*(const PXL_Colour& op) {
		PXL_Colour n;
		n.r = r * op.r;
		n.g = g * op.g;
		n.b = b * op.g;
		n.a = a * op.a;
		return PXL_Colour(n);
	}
};

#define PXL_COLOR_WHITE					PXL_COLOUR_WHITE				/**> Pre-defined white colour **/
#define PXL_COLOR_BLACK					PXL_COLOUR_BLACK				/**> Pre-defined black colour **/
#define PXL_COLOR_RED					PXL_COLOUR_RED					/**> Pre-defined red colour **/
#define PXL_COLOR_GREEN					PXL_COLOUR_GREEN				/**> Pre-defined green colour **/
#define PXL_COLOR_BLUE					PXL_COLOUR_BLUE					/**> Pre-defined blue colour **/
#define PXL_COLOR_YELLOW				PXL_COLOUR_YELLOW				/**> Pre-defined yellow colour **/
#define PXL_COLOR_ORANGE				PXL_COLOUR_ORANGE				/**> Pre-defined orange colour **/
#define PXL_COLOR_PINK					PXL_COLOUR_PINK					/**> Pre-defined pink colour **/
#define PXL_COLOR_PURPLE				PXL_COLOUR_PURPLE				/**> Pre-defined purple colour **/
#define PXL_COLOR_LIGHT_RED				PXL_COLOUR_LIGHT_RED			/**> Pre-defined light red colour **/
#define PXL_COLOR_LIGHT_GREEN			PXL_COLOUR_LIGHT_GREEN			/**> Pre-defined light green colour **/
#define PXL_COLOR_LIGHT_BLUE			PXL_COLOUR_LIGHT_BLUE			/**> Pre-defined light blue colour **/
#define PXL_COLOR_DARK_RED				PXL_COLOUR_DARK_RED				/**> Pre-defined dark red colour **/
#define PXL_COLOR_DARK_GREEN			PXL_COLOUR_DARK_GREEN			/**> Pre-defined dark green colour **/
#define PXL_COLOR_DARK_BLUE				PXL_COLOUR_DARK_BLUE			/**> Pre-defined dark blue colour **/
#define PXL_COLOR_TRANSPARENT_WHITE		PXL_COLOUR_TRANSPARENT_WHITE	/**> Pre-defined fully transparent white colour **/
#define PXL_COLOR_TRANSPARENT_BLACK		PXL_COLOUR_TRANSPARENT_BLACK	/**> Pre-defined fully transparent black colour **/

const extern PXL_Colour PXL_COLOUR_WHITE;				/**> Pre-defined white colour **/
const extern PXL_Colour PXL_COLOUR_BLACK;				/**> Pre-defined black colour **/
const extern PXL_Colour PXL_COLOUR_RED;					/**> Pre-defined red colour **/
const extern PXL_Colour PXL_COLOUR_GREEN;				/**> Pre-defined green colour **/
const extern PXL_Colour PXL_COLOUR_BLUE;				/**> Pre-defined blue colour **/
const extern PXL_Colour PXL_COLOUR_YELLOW;				/**> Pre-defined yellow colour **/
const extern PXL_Colour PXL_COLOUR_ORANGE;				/**> Pre-defined orange colour **/
const extern PXL_Colour PXL_COLOUR_PINK;				/**> Pre-defined pink colour **/
const extern PXL_Colour PXL_COLOUR_PURPLE;				/**> Pre-defined purple colour **/
const extern PXL_Colour PXL_COLOUR_LIGHT_RED;			/**> Pre-defined light red colour **/
const extern PXL_Colour PXL_COLOUR_LIGHT_GREEN;			/**> Pre-defined light green colour **/
const extern PXL_Colour PXL_COLOUR_LIGHT_BLUE;			/**> Pre-defined light blue colour **/
const extern PXL_Colour PXL_COLOUR_DARK_RED;			/**> Pre-defined dark red colour **/
const extern PXL_Colour PXL_COLOUR_DARK_GREEN;			/**> Pre-defined dark green colour **/
const extern PXL_Colour PXL_COLOUR_DARK_BLUE;			/**> Pre-defined dark blue colour **/
const extern PXL_Colour PXL_COLOUR_TRANSPARENT_WHITE;	/**> Pre-defined fully transparent white colour **/
const extern PXL_Colour PXL_COLOUR_TRANSPARENT_BLACK;	/**> Pre-defined fully transparent black colour **/

#endif