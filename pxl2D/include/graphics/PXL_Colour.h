#ifndef _PXL_COLOUR_H
#define _PXL_COLOUR_H

#define PXL_Color PXL_Colour

struct PXL_Colour {

	PXL_Colour() { }
	PXL_Colour(float red, float green, float blue, float alpha = 1);
	PXL_Colour(unsigned int colour);

	float r = 1, g = 1, b = 1, a = 1;

	static const PXL_Colour WHITE; /**> Pre-defined white colour **/
	static const PXL_Colour BLACK; /**> Pre-defined black colour **/
	static const PXL_Colour RED; /**> Pre-defined red colour **/
	static const PXL_Colour GREEN; /**> Pre-defined green colour **/
	static const PXL_Colour BLUE; /**> Pre-defined blue colour **/
	static const PXL_Colour YELLOW; /**> Pre-defined yellow colour **/
	static const PXL_Colour ORANGE; /**> Pre-defined orange colour **/
	static const PXL_Colour PINK; /**> Pre-defined pink colour **/
	static const PXL_Colour PURPLE; /**> Pre-defined purple colour **/
	static const PXL_Colour LIGHT_RED; /**> Pre-defined light red colour **/
	static const PXL_Colour LIGHT_GREEN; /**> Pre-defined light green colour **/
	static const PXL_Colour LIGHT_BLUE; /**> Pre-defined light blue colour **/
	static const PXL_Colour DARK_RED; /**> Pre-defined dark red colour **/
	static const PXL_Colour DARK_GREEN; /**> Pre-defined dark green colour **/
	static const PXL_Colour DARK_BLUE; /**> Pre-defined dark blue colour **/
	static const PXL_Colour TRANSPARENT_WHITE; /**> Pre-defined fully transparent white colour **/
	static const PXL_Colour TRANSPARENT_BLACK; /**> Pre-defined fully transparent black colour **/

	void set_colour(float red, float green, float blue, float alpha = 1);
	void set_colour(unsigned int colour);

	//todo: add colour operators
};

#endif