#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <string>

using namespace std;

class PXL_PixelBuffer {

	public:
		PXL_PixelBuffer() { }
		PXL_PixelBuffer(int pos_x, int pos_y, unsigned int w, unsigned int h, 
						unsigned int max_w, unsigned int max_h, char* pixels, unsigned int size) :
						x(pos_x), y(pos_y), width(w), height(h), max_width(max_w), max_height(max_h), 
						buffer(pixels), buffer_size(size) { }
		~PXL_PixelBuffer() { free(); }

		char* buffer;
		unsigned int buffer_size;
		int x;
		int y;
		unsigned int width;
		unsigned int height;
		unsigned int max_width;
		unsigned int max_height;

		void free() {
			delete[] buffer;
		}
};

/**
\*brief: loads the contents of a png image into a PXL_PixelBuffer
\*param [file_name]: the path and file name of the png to load
**/
extern PXL_PixelBuffer* PXL_load_png(string file_name);

/**
\*brief: clamps a value from min to max
\*param [v]: float value
\*param [min]: min float value
\*param [max]: max float value
**/
extern float PXL_clamp(float v, float min, float max);

#endif