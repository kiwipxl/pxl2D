#include "PXL_Bitmap.h"
#include <iostream>

PXL_Bitmap::PXL_Bitmap() {
	buffer_loaded = false;
}

PXL_Bitmap::PXL_Bitmap(string path) {
	buffer_loaded = false;
	load_bitmap(path);
}

PXL_Bitmap* PXL_create_bitmap(string path) {
	return new PXL_Bitmap(path);
}

void PXL_Bitmap::load_bitmap(string path) {
	if (buffer_loaded) {
		buffer_loaded = false;
		delete buffer;
	}
	
	buffer = PXL_load_png(path);
	w = buffer->width;
	h = buffer->height;
	pixels = &buffer->buffer[0];

	buffer_loaded = true;
}

void PXL_Bitmap::free() {
	if (buffer_loaded) {
		buffer_loaded = false;
		delete buffer;
	}
}

PXL_Bitmap::~PXL_Bitmap() {
	free();
}