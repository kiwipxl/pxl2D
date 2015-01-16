#ifndef BITMAP_H
#define BITMAP_H

#include <string>
#include "PXL_ImageUtils.h"

using namespace std;

class PXL_Bitmap {

	public:
		/**
		\*brief: bitmap constructor
		**/
		PXL_Bitmap();
		/**
		\*brief: loads the bitmap
		\*param [path]: the path and file name for the bitmap to load
		**/
		PXL_Bitmap(string path);
		/**
		\*brief: bitmap deconstructor
		**/
		~PXL_Bitmap();

		//bitmap data
		int w;
		int h;
		char* pixels;

		/**
		\*brief: loads a bitmap from the specified path
		\*param [path]: the path and file name for the bitmap to load
		**/
		void load_bitmap(string path);

		/**
		\*brief: frees all data from the bitmap
		**/
		void free();

	private:
		//buffer info
		bool buffer_loaded;
		PXL_PixelBuffer* buffer;
};

/**
\*brief: loads and creates a bitmap from the specified path
\*param [path]: the path and file name for the bitmap to load
**/
extern PXL_Bitmap* PXL_create_bitmap(string path);

#endif