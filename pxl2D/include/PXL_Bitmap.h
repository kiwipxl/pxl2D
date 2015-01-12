#ifndef BITMAP_H
#define BITMAP_H

#include <string>
#include "PXL_ImageUtils.h"

using namespace std;

class PXL_Bitmap {

	public:
		PXL_Bitmap();
		PXL_Bitmap(string path);
		~PXL_Bitmap();

		int w;
		int h;
		char* pixels;

		void load_bitmap(string path);
		void free();

	private:
		bool buffer_loaded;
		PXL_PixelBuffer* buffer;
};

extern PXL_Bitmap* PXL_create_bitmap(string path);

#endif