#include "PXL_Bitmap.h"
#include <iostream>
#include "PXL_ImageUtils.h"

PXL_Bitmap::PXL_Bitmap(string path) {
	created = false;
	create_bitmap(path);
}

PXL_Bitmap* PXL_create_bitmap(string path) {
	return new PXL_Bitmap(path);
}

void PXL_Bitmap::create_bitmap(string path) {
	if (created) {
		created = false;
	}
	
	PXL_PixelBuffer* pixel_buffer = PXL_load_png(path);
	w = pixel_buffer->width;
	h = pixel_buffer->height;
	pixels = pixel_buffer->buffer;

	created = true;
}

void PXL_Bitmap::free() {
	if (created) {
		created = false;
	}
}

PXL_Bitmap::~PXL_Bitmap() {
	free();
}