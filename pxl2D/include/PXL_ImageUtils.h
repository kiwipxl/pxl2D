#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <string>

using namespace std;

class PXL_PixelBuffer {

	public:
		PXL_PixelBuffer() { }
		~PXL_PixelBuffer() { free(); }

		char* buffer;
		unsigned int buffer_size;
		unsigned int width;
		unsigned int height;

		void free() {
			delete[] buffer;
		}
};

/**
\*brief: loads the contents of a png image into a PXL_PixelBuffer
\*param [file_name]: the path and file name of the png to load
**/
extern PXL_PixelBuffer* PXL_load_png(string file_name);

#endif