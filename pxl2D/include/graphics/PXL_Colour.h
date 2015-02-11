#ifndef _PXL_COLOUR_H
#define _PXL_COLOUR_H

#define PXL_Color PXL_Colour

struct PXL_Colour {

	PXL_Colour() { }
	PXL_Colour(float red, float green, float blue, float alpha = 1);
	PXL_Colour(unsigned int colour);

	float r = 1, g = 1, b = 1, a = 1;

	static const PXL_Colour WHITE; /**> Supports blending when rendering **/
	static const PXL_Colour BLACK; /**> Supports blending when rendering **/
	static const PXL_Colour RED; /**> Supports blending when rendering **/
	static const PXL_Colour GREEN; /**> Supports blending when rendering **/
	static const PXL_Colour BLUE; /**> Supports blending when rendering **/
	static const PXL_Colour YELLOW; /**> Supports blending when rendering **/
	static const PXL_Colour ORANGE; /**> Supports blending when rendering **/
	static const PXL_Colour PINK; /**> Supports blending when rendering **/
	static const PXL_Colour PURPLE; /**> Supports blending when rendering **/
	static const PXL_Colour LIGHT_RED; /**> Supports blending when rendering **/
	static const PXL_Colour LIGHT_GREEN; /**> Supports blending when rendering **/
	static const PXL_Colour LIGHT_BLUE; /**> Supports blending when rendering **/
	static const PXL_Colour DARK_RED; /**> Supports blending when rendering **/
	static const PXL_Colour DARK_GREEN; /**> Supports blending when rendering **/
	static const PXL_Colour DARK_BLUE; /**> Supports blending when rendering **/
	static const PXL_Colour TRANSPARENT_WHITE; /**> Supports blending when rendering **/
	static const PXL_Colour TRANSPARENT_BLACK; /**> Supports blending when rendering **/

	void set_colour(float red, float green, float blue, float alpha = 1);
	void set_colour(unsigned int colour);

	//todo: add colour operators
};

#endif