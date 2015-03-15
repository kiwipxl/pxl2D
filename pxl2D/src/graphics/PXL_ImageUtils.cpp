#include "graphics/PXL_ImageUtils.h"
#include <iostream>
#include <fstream>
#include <png.h>
#include "system/PXL_Exception.h"
#include "system/PXL_Debug.h"

#define PNG_SIG_SIZE 8

bool png_validate(std::ifstream& file);
void read_png(png_structp read, png_bytep data, png_size_t length);

std::ifstream file;

std::string load_file(std::string file_name) {
	std::ifstream file(file_name.c_str(), std::ifstream::in);
	if (file) {
		file.ignore(std::numeric_limits<std::streamsize>::max());
		std::streamsize size = file.gcount();
		file.seekg(0, std::ifstream::beg);

		PXL_print << "file size: " << size << "\n";

		if (size >= 0) {
			char* buffer = new char[size];
			file.read(buffer, size);

			file.close();
			if (buffer) {
				buffer[size] = '\0';
				return buffer;
			}else {
				PXL_show_exception("(" + file_name + ") could not be read successfully", PXL_ERROR_INVALID_FILE, PXL_EXCEPTION_CONSOLE, false);
				delete[] buffer;
			}
		}else {
			file.close();
			PXL_show_exception("(" + file_name + ") does not contain any content when read", PXL_ERROR_EMPTY_FILE, PXL_EXCEPTION_CONSOLE, false);
		}
	}else {
		file.close();
		PXL_show_exception("Couldn't load file (" + file_name + "). It may not exist", PXL_ERROR_INVALID_FILE, PXL_EXCEPTION_CONSOLE, false);
	}
	return "";
}

PXL_Bitmap* PXL_load_png(std::string file_name, PXL_Bitmap* bitmap) {
	if (bitmap == NULL) return NULL;

	file_name = "/sdcard/Download/cutecat.png";

	file.open(file_name.c_str(), std::ios::binary);

	if (!png_validate(file)) {
		PXL_show_exception("(" + file_name + ") is not a valid png (or it may not exist)", PXL_ERROR_INVALID_PNG);
		return NULL;
	}

	png_structp png_pointer = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_pointer) {
		PXL_show_exception("Could not initialise png read struct for (" + file_name + ")", PXL_ERROR_INVALID_PNG);
	}

	png_infop info_pointer = png_create_info_struct(png_pointer);
	if (!info_pointer) {
		PXL_show_exception("Could not initialise png info struct for (" + file_name + ")", PXL_ERROR_INVALID_PNG);
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

	PXL_Channel channel;
	if (colour_type == 0) channel = PXL_CHANNEL_ALPHA;
	if (colour_type == 2) channel = PXL_CHANNEL_RGB;
	if (colour_type == 3) PXL_show_exception("This colour type is not yet supported (palette index)", PXL_ERROR_INVALID_PNG);
	if (colour_type == 4) channel = PXL_CHANNEL_GRAY_ALPHA;
	if (colour_type == 6) channel = PXL_CHANNEL_RGBA;

	bitmap->create_bitmap(png_width, png_height, PXL_COLOR_BLACK, channel);

	const PXL_uint row_length = bitmap->get_width() * ((bit_depth * channels) / 8);
	png_bytep* row_pointers = new png_bytep[bitmap->get_height()];
	for (size_t y = 0; y < bitmap->get_height(); ++y) {
		row_pointers[y] = (png_bytep)(bitmap->get_pixels() + (y * row_length));
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