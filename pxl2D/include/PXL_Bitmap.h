#ifndef BITMAP_H
#define BITMAP_H

#include <string>
#include "PXL_BufferObject.h"
#include "PXL_Rect.h"

using namespace std;

class PXL_Bitmap {

	public:
		PXL_Bitmap(string path);
		~PXL_Bitmap();

		int w;
		int h;
		char* pixels;

		void create_bitmap(string path);
		void free();

	private:
		bool created;
};

extern PXL_Bitmap* PXL_create_bitmap(string path);

#endif