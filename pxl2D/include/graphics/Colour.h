#ifndef _COLOUR_H
#define _COLOUR_H

#include "API.h"

#define Color Colour

namespace pxl { namespace graphics {

    struct Colour {

	    Colour() { }
	    Colour(float red, float green, float blue, float alpha = 1);
	    Colour(uint32 colour);

	    float r = 1, g = 1, b = 1, a = 1;

	    void set_colour(float red, float green, float blue, float alpha = 1);
	    void set_colour(uint32 colour);

	    void clamp(float min = 0, float max = 1, bool clamp_alpha = true);

	    Colour operator*(const Colour& op) {
		    Colour n;
		    n.r = r * op.r; n.g = g * op.g; n.b = b * op.g; n.a = a * op.a;
		    return Colour(n);
	    }

	    Colour operator*(const int op) {
		    Colour n;
		    n.r = r * op; n.g = g * op; n.b = b * op; n.a = a * op;
		    return Colour(n);
	    }

	    const Colour& operator*=(const Colour& op) {
		    r *= op.r; g *= op.g; b *= op.g; a *= op.a;
		    return *this;
	    }
	
	    const Colour& operator*=(const int op) {
		    r *= op; g *= op; b *= op; a *= op;
		    return *this;
	    }

	    Colour operator+(const Colour& op) {
		    Colour n;
		    n.r = r + op.r; n.g = g + op.g; n.b = b + op.g; n.a = a + op.a;
		    return Colour(n);
	    }

	    Colour operator+(const int op) {
		    Colour n;
		    n.r = r + op; n.g = g + op; n.b = b + op; n.a = a + op;
		    return Colour(n);
	    }

	    const Colour& operator+=(const Colour& op) {
		    r += op.r; g += op.g; b += op.g; a += op.a;
		    return *this;
	    }

	    const Colour& operator+=(const int op) {
		    r += op; g += op; b += op; a += op;
		    return *this;
	    }

	    Colour operator-(const Colour& op) {
		    Colour n;
		    n.r = r - op.r; n.g = g - op.g; n.b = b - op.g; n.a = a - op.a;
		    return Colour(n);
	    }

	    Colour operator-(const int op) {
		    Colour n;
		    n.r = r - op; n.g = g - op; n.b = b - op; n.a = a - op;
		    return Colour(n);
	    }

	    const Colour& operator-=(const Colour& op) {
		    r -= op.r; g -= op.g; b -= op.g; a -= op.a;
		    return *this;
	    }

	    const Colour& operator-=(const int op) {
		    r -= op; g -= op; b -= op; a -= op;
		    return *this;
	    }
    };

    struct Gradient {

	    Gradient(uint32 gr1, uint32 gr2) {
		    g1 = gr1;
		    g2 = gr2;
	    }

	    Colour g1;
	    Colour g2;
    };

    #define COLOR_WHITE						COLOUR_WHITE					/**> Pre-defined white colour **/
    #define COLOR_BLACK						COLOUR_BLACK					/**> Pre-defined black colour **/
    #define COLOR_RED						COLOUR_RED						/**> Pre-defined red colour **/
    #define COLOR_GREEN						COLOUR_GREEN					/**> Pre-defined green colour **/
    #define COLOR_BLUE						COLOUR_BLUE						/**> Pre-defined blue colour **/
    #define COLOR_YELLOW					COLOUR_YELLOW					/**> Pre-defined yellow colour **/
    #define COLOR_ORANGE					COLOUR_ORANGE					/**> Pre-defined orange colour **/
    #define COLOR_PINK						COLOUR_PINK						/**> Pre-defined pink colour **/
    #define COLOR_PURPLE					COLOUR_PURPLE					/**> Pre-defined purple colour **/
    #define COLOR_LIGHT_RED					COLOUR_LIGHT_RED				/**> Pre-defined light red colour **/
    #define COLOR_LIGHT_GREEN				COLOUR_LIGHT_GREEN				/**> Pre-defined light green colour **/
    #define COLOR_LIGHT_BLUE				COLOUR_LIGHT_BLUE				/**> Pre-defined light blue colour **/
    #define COLOR_DARK_RED					COLOUR_DARK_RED					/**> Pre-defined dark red colour **/
    #define COLOR_DARK_GREEN				COLOUR_DARK_GREEN				/**> Pre-defined dark green colour **/
    #define COLOR_DARK_BLUE					COLOUR_DARK_BLUE				/**> Pre-defined dark blue colour **/
    #define COLOR_TRANSPARENT_WHITE			COLOUR_TRANSPARENT_WHITE		/**> Pre-defined fully transparent white colour **/
    #define COLOR_TRANSPARENT_BLACK			COLOUR_TRANSPARENT_BLACK		/**> Pre-defined fully transparent black colour **/

    const extern Colour						COLOUR_WHITE;					/**> Pre-defined white colour **/
    const extern Colour						COLOUR_BLACK;					/**> Pre-defined black colour **/
    const extern Colour						COLOUR_RED;						/**> Pre-defined red colour **/
    const extern Colour						COLOUR_GREEN;					/**> Pre-defined green colour **/
    const extern Colour						COLOUR_BLUE;					/**> Pre-defined blue colour **/
    const extern Colour						COLOUR_YELLOW;					/**> Pre-defined yellow colour **/
    const extern Colour						COLOUR_ORANGE;					/**> Pre-defined orange colour **/
    const extern Colour						COLOUR_PINK;					/**> Pre-defined pink colour **/
    const extern Colour						COLOUR_PURPLE;					/**> Pre-defined purple colour **/
    const extern Colour						COLOUR_LIGHT_RED;				/**> Pre-defined light red colour **/
    const extern Colour						COLOUR_LIGHT_GREEN;				/**> Pre-defined light green colour **/
    const extern Colour						COLOUR_LIGHT_BLUE;				/**> Pre-defined light blue colour **/
    const extern Colour						COLOUR_DARK_RED;				/**> Pre-defined dark red colour **/
    const extern Colour						COLOUR_DARK_GREEN;				/**> Pre-defined dark green colour **/
    const extern Colour						COLOUR_DARK_BLUE;				/**> Pre-defined dark blue colour **/
    const extern Colour						COLOUR_TRANSPARENT_WHITE;		/**> Pre-defined fully transparent white colour **/
    const extern Colour						COLOUR_TRANSPARENT_BLACK;		/**> Pre-defined fully transparent black colour **/
    const extern Gradient					GRADIENT_WHITE_BLACK;			/**> Pre-defined white to black gradient colour **/
    const extern Gradient					GRADIENT_RED_BLACK;				/**> Pre-defined red to black gradient colour **/

}};

#endif