#include "PXL_Bitmap.h"
#include <iostream>
#include "PXL_ImageUtils.h"

PXL_Bitmap::PXL_Bitmap() {
	buffer_loaded = false;
}

PXL_Bitmap::PXL_Bitmap(std::string path) {
	buffer_loaded = false;
	load_bitmap(path);
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