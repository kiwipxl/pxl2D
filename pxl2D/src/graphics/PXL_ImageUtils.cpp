#include "PXL_ImageUtils.h"
#include <iostream>
#include <fstream>
#include <png.h>
#include "system/PXL_Exception.h"

#define PNG_SIG_SIZE 8

bool png_validate(std::ifstream& file);
void read_png(png_structp read, png_bytep data, png_size_t length);

std::ifstream file;

PXL_Bitmap* PXL_load_png(std::string file_name, PXL_Bitmap* bitmap) {
	file.open(file_name, std::ios::binary);

	if (!png_validate(file)) {
		PXL_show_exception("(" + file_name + ") is not a valid png", PXL_EXCEPTION_CONSOLE, false);
		return NULL;
	}

	png_structp png_pointer = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_pointer) {
		PXL_show_exception("Could not initialise png read struct for (" + file_name + ")", PXL_EXCEPTION_CONSOLE, false);
	}

	png_infop info_pointer = png_create_info_struct(png_pointer);
	if (!info_pointer) {
		PXL_show_exception("Could not initialise png info struct for (" + file_name + ")", PXL_EXCEPTION_CONSOLE, false);
		png_destroy_read_struct(&png_pointer, (png_infopp)0, (png_infopp)0);
	}

	png_set_read_fn(png_pointer, (png_voidp)&file, read_png);

	//set amount of bytes already read
	png_set_sig_bytes(png_pointer, PNG_SIG_SIZE);

	//read png header info into libpng
	png_read_info(png_pointer, info_pointer);

	png_uint_32 png_width = png_get_image_width(png_pointer, info_pointer);
	png_uint_32 png_height = png_get_image_height(png_pointer, info_pointer);

	png_uint_32 bit_depth = png_get_bit_depth(png_pointer, info_pointer);
	png_uint_32 channels = png_get_channels(png_pointer, info_pointer);
	png_uint_32 colour_type = png_get_color_type(png_pointer, info_pointer);

	if (bitmap == NULL) {
		bitmap = new PXL_Bitmap(png_width, png_height, 0);
	}
	bitmap->width = png_width;
	bitmap->height = png_height;

	const unsigned int row_length = bitmap->width * ((bit_depth * channels) / 8);
	bitmap->size = row_length * bitmap->height;
	bitmap->pixels = new unsigned char[bitmap->size];

	png_bytep* row_pointers = new png_bytep[bitmap->height];
	for (int y = 0; y < bitmap->height; ++y) {
		row_pointers[y] = (png_bytep)(bitmap->pixels + (y * row_length));
	}

	png_read_image(png_pointer, row_pointers);

	delete[] row_pointers;
	png_destroy_read_struct(&png_pointer, &info_pointer, (png_infopp)0);

	file.close();

	return bitmap;
}

void read_png(png_structp png_pointer, png_bytep data, png_size_t length) {
	file.read((char*)data, length);
}

bool png_validate(std::ifstream& file) {
	//allocate a buffer of 8 constant bytes
	png_byte png_sig[PNG_SIG_SIZE];

	//read 8 byte sig from stream into buffer
	file.read((char*)png_sig, PNG_SIG_SIZE);

	//return valid/invalid sig
	return (png_sig_cmp(png_sig, 0, PNG_SIG_SIZE) == 0);
}