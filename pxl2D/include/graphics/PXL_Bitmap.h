#ifndef BITMAP_H
#define BITMAP_H

#include <string>
#include "PXL_ImageUtils.h"

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
		PXL_Bitmap(std::string path);
		/**
		\*brief: creates the bitmap with specified values
		\*param [width]: the width of the image
		\*param [height]: the height of the image
		\*param [buffer]: an array of pixels for the image
		**/
		PXL_Bitmap(int width, int height, unsigned char* buffer);
		/**
		\*brief: bitmap deconstructor
		**/
		~PXL_Bitmap();

		//bitmap data
		int w;
		int h;
		unsigned char* pixels;

		/**
		\*brief: loads a bitmap from the specified path
		\*param [path]: the path and file name for the bitmap to load
		**/
		void load_bitmap(std::string path);

		/**
		\*brief: frees all data from the bitmap
		**/
		void free();

	private:
		//buffer info
		bool buffer_loaded;
};

/**
\*brief: loads and creates a bitmap from the specified path
\*param [path]: the path and file name for the bitmap to load
**/
extern PXL_Bitmap* PXL_create_bitmap(std::string path);

#endif