#ifndef _PXL_BITMAP_H
#define _PXL_BITMAP_H

#include <string>
#include "PXL_Structs.h"

class PXL_Bitmap {

	public:
		/**
		\*brief: loads the bitmap
		\*param [path]: the path and file name for the bitmap to load
		**/
		PXL_Bitmap(std::string path);
		/**
		\*brief: constructs the bitmap with specified values
		\*param [width]: the width of the image
		\*param [height]: the height of the image
		\*param [buffer]: an array of pixels for the image
		**/
		PXL_Bitmap(int width, int height, PXL_Colour fill_colour);
		/**
		\*brief: constructs the bitmap with specified values
		\*param [width]: the width of the image
		\*param [height]: the height of the image
		\*param [buffer]: an array of pixels for the image
		**/
		PXL_Bitmap(int width, int height, unsigned char* pixel_buffer);
		/**
		\*brief: bitmap deconstructor
		**/
		~PXL_Bitmap();

		//bitmap data
		int width;
		int height;
		unsigned char* pixels;
		unsigned int size;
		int num_channels;
		int bit_depth;

		/**
		\*brief: loads a bitmap from the specified path
		\*param [path]: the path and file name for the bitmap to load
		**/
		void load_bitmap(std::string path);

		void fill(PXL_Colour colour);

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