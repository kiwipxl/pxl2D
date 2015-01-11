#ifndef RECT_H
#define RECT_H

class PXL_Rect {

	public:
		PXL_Rect() {
			x = y = w = h = 0;
		}

		float x;
		float y;
		float w;
		float h;
};

#endif