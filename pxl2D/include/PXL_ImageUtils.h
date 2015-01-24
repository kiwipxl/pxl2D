#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <string>
#include <iostream>
#include "PXL_Structs.h"

struct PXL_PixelBuffer {

	PXL_PixelBuffer() { }
	PXL_PixelBuffer(int pos_x, int pos_y, unsigned int w, unsigned int h, 
					unsigned int max_w, unsigned int max_h, unsigned char* pixels, 
					unsigned int size, unsigned int channel_size) :
					x(pos_x), y(pos_y), width(w), height(h), max_width(max_w), max_height(max_h), 
					buffer(pixels), buffer_size(size), channels(channel_size) { }
	~PXL_PixelBuffer() { free(); }

	unsigned char* buffer;
	unsigned int buffer_size;
	unsigned int channels = 4;
	int x = 0;
	int y = 0;
	unsigned int width;
	unsigned int height;
	unsigned int max_width;
	unsigned int max_height;
	PXL_Rect src_rect;

	void free() {
		delete[] buffer;
		buffer = NULL;
	}
};

/**
\*brief: loads the contents of a png image into a PXL_PixelBuffer
\*param [file_name]: the path and file name of the png to load
**/
extern PXL_PixelBuffer* PXL_load_png(std::string file_name);

#endif