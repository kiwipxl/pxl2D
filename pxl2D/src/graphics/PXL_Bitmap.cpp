#include "PXL_Bitmap.h"
#include <iostream>
#include "PXL_ImageUtils.h"

PXL_Bitmap::PXL_Bitmap(std::string path) {
	buffer_loaded = false;
	load_bitmap(path);
}

PXL_Bitmap::PXL_Bitmap(int bitmap_width, int bitmap_height, int fill_colour) {
	buffer_loaded = true;
	width = bitmap_width;
	height = bitmap_height;
	pixels = new unsigned char[(width * height) * 4];
	fill(fill_colour);
}

PXL_Bitmap::PXL_Bitmap(int bitmap_width, int bitmap_height, unsigned char* pixel_buffer) {
	buffer_loaded = true;
	width = bitmap_width;
	height = bitmap_height;
	pixels = pixel_buffer;
}

PXL_Bitmap* PXL_create_bitmap(std::string path) {
	return new PXL_Bitmap(path);
}

void PXL_Bitmap::fill(unsigned int colour) {
	//mask r, g, b, a and move bits to the right to get value
	unsigned int r = (colour & 0xff000000) >> 24;
	unsigned int g = (colour & 0x00ff0000) >> 16;
	unsigned int b = (colour & 0x0000ff00) >> 8;
	unsigned int a = colour & 0x000000ff;

	int row_size = width * 4;
	for (int y = 0; y < height; ++y) {
		int row_y = y * row_size;
		for (int x = 0; x < row_size; x += 4) {
			pixels[x + row_y] = r;
			pixels[x + 1 + row_y] = g;
			pixels[x + 2 + row_y] = b;
			pixels[x + 3 + row_y] = a;
		}
	}
}

void PXL_Bitmap::load_bitmap(std::string path) {
	free();

	PXL_load_png(path, this);

	buffer_loaded = true;
}

void PXL_Bitmap::free() {
	if (buffer_loaded) {
		buffer_loaded = false;
		delete[] pixels;
	}
}

PXL_Bitmap::~PXL_Bitmap() {
	free();
}