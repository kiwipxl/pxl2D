#include "graphics/Colour.h"
#include "system/Math.h"

namespace pxl { namespace graphics {

    const Colour COLOUR_WHITE					= Colour(0xffffffff);					/**> Pre-defined white colour **/
    const Colour COLOUR_BLACK					= Colour(0x000000ff);					/**> Pre-defined black colour **/
    const Colour COLOUR_RED						= Colour(0xff0000ff);					/**> Pre-defined red colour **/
    const Colour COLOUR_GREEN					= Colour(0x00ff00ff);					/**> Pre-defined green colour **/
    const Colour COLOUR_BLUE					= Colour(0x0000ffff);					/**> Pre-defined blue colour **/
    const Colour COLOUR_YELLOW					= Colour(0xffff00ff);					/**> Pre-defined yellow colour **/
    const Colour COLOUR_ORANGE					= Colour(0xff9900ff);					/**> Pre-defined orange colour **/
    const Colour COLOUR_PINK					= Colour(0xff66ffff);					/**> Pre-defined pink colour **/
    const Colour COLOUR_PURPLE					= Colour(0xcc00ccff);					/**> Pre-defined purple colour **/
    const Colour COLOUR_LIGHT_RED				= Colour(0xff9999ff);					/**> Pre-defined light red colour **/
    const Colour COLOUR_LIGHT_GREEN				= Colour(0x99ff99ff);					/**> Pre-defined light green colour **/
    const Colour COLOUR_LIGHT_BLUE				= Colour(0x00ccffff);					/**> Pre-defined light blue colour **/
    const Colour COLOUR_DARK_RED				= Colour(0x751919ff);					/**> Pre-defined dark red colour **/
    const Colour COLOUR_DARK_GREEN				= Colour(0x19a347ff);					/**> Pre-defined dark green colour **/
    const Colour COLOUR_DARK_BLUE				= Colour(0x000099ff);					/**> Pre-defined dark blue colour **/
    const Colour COLOUR_TRANSPARENT_WHITE		= Colour(0xffffff00);					/**> Pre-defined fully transparent white colour **/
    const Colour COLOUR_TRANSPARENT_BLACK		= Colour(0x00000000);					/**> Pre-defined fully transparent black colour **/
    const Gradient GRADIENT_WHITE_BLACK			= Gradient(0xffffff00, 0x000000ff);		/**> Pre-defined white to black gradient colour **/
    const Gradient GRADIENT_RED_BLACK			= Gradient(0xff0000ff, 0x000000ff);		/**> Pre-defined red to black gradient colour **/

    Colour::Colour(float red, float green, float blue, float alpha) {
	    set_colour(red, green, blue, alpha);
    }

    Colour::Colour(uint32 colour) {
	    set_colour(colour);
    }

    void Colour::set_colour(float red, float green, float blue, float alpha) {
	    r = red; g = green; b = blue; a = alpha;
    }

    void Colour::set_colour(uint32 colour) {
	    //mask r, g, b, a and move bits to the right to get value and then normalise
	    r = ((colour & 0xff000000) >> 24) / 255.0f;
	    g = ((colour & 0x00ff0000) >> 16) / 255.0f;
	    b = ((colour & 0x0000ff00) >> 8) / 255.0f;
	    a = (colour & 0x000000ff) / 255.0f;
    }

    void Colour::clamp(float min, float max, bool clamp_alpha) {
	    r = math::clamp(r, min, max);
	    g = math::clamp(g, min, max);
	    b = math::clamp(b, min, max);
	    if (clamp_alpha) a = math::clamp(a, min, max);
    }
}};