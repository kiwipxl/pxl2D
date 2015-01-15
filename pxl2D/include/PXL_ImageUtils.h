#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <string>

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

using namespace std;

extern PXL_PixelBuffer* PXL_load_png(string file_name);

#endif