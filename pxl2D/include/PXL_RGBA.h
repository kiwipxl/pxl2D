#ifndef RGBA_H
#define RGBA_H

class PXL_RGBA {

	public:
		PXL_RGBA() {
			r = g = b = 0;
			a = 255;
		}

		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
};

#endif