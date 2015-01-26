#include "PXL_Bitmap.h"
#include <iostream>

PXL_Bitmap::PXL_Bitmap() {
	buffer_loaded = false;
}

PXL_Bitmap::PXL_Bitmap(std::string path) {
	buffer_loaded = false;
	load_bitmap(path);
}

PXL_Bitmap::PXL_Bitmap(int width, int height, unsigned char* buffer) {
	buffer_loaded = true;
	w = width;
	h = height;
	pixels = &buffer[0];
}

PXL_Bitmap* PXL_create_bitmap(std::string path) {
	return new PXL_Bitmap(path);
}

void PXL_Bitmap::load_bitmap(std::string path) {
	if (buffer_loaded) {
		buffer_loaded = false;
		delete pixels;
	}
	
	PXL_PixelBuffer* pix_buf = PXL_load_png(path);
	w = pix_buf->width;
	h = pix_buf->height;
	pixels = new unsigned char[pix_buf->buffer_size];
	memcpy(pixels, pix_buf->buffer, pix_buf->buffer_size);
	delete pix_buf;

	buffer_loaded = true;
}

void PXL_Bitmap::free() {
	if (buffer_loaded) {
		buffer_loaded = false;
		delete pixels;
	}
}

PXL_Bitmap::~PXL_Bitmap() {
	free();
}