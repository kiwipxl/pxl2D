#ifndef PXL_VEC_4_H
#define PXL_VEC_4_H

class PXL_Vec4 {

	public:
		PXL_Vec4() {
			x = y = z = w = 0;
		}

		float x;
		float y;
		float z;
		float w;
};

#endif