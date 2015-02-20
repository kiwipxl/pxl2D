#include "PXL_Bitmap.h"
#include <iostream>
#include "PXL_ImageUtils.h"

PXL_Bitmap::PXL_Bitmap() {
	buffer_loaded = false;
}

bool PXL_Bitmap::create_bitmap(std::string path) {
	free();

	buffer_loaded = PXL_load_png(path, this);

	check_has_transparency();

	return buffer_loaded;
}

void PXL_Bitmap::create_bitmap(int bitmap_width, int bitmap_height, PXL_Colour fill_colour, PXL_Channel pixel_channel) {
	free();

	buffer_loaded = true;
	width = bitmap_width;
	height = bitmap_height;

	channel = pixel_channel;
	row_size = width * channel.num_channels;
	buffer_size = row_size * height;
	pixels = new PXL_ubyte[buffer_size];

	fill(fill_colour);
	check_has_transparency();
}

void PXL_Bitmap::create_bitmap(int bitmap_width, int bitmap_height, PXL_ubyte* pixel_buffer, PXL_Channel pixel_channel) {
	free();

	buffer_loaded = true;
	width = bitmap_width;
	height = bitmap_height;

	channel = pixel_channel;
	row_size = width * channel.num_channels;
	buffer_size = row_size * height;
	pixels = pixel_buffer;

	check_has_transparency();
}

void PXL_Bitmap::fill(PXL_Colour colour) {
	if (pixels == NULL) return;

	PXL_ubyte r = colour.r * 255;
	PXL_ubyte g = colour.g * 255;
	PXL_ubyte b = colour.b * 255;
	PXL_ubyte a = colour.a * 255;

	for (int y = 0; y < height; ++y) {
		int row_y = y * row_size;
		for (int x = 0; x < row_size; x += channel.num_channels) {
			if (channel.channel_index.r != -1) pixels[x + row_y + channel.channel_index.r] = r;
			if (channel.channel_index.g != -1) pixels[x + row_y + channel.channel_index.g] = g;
			if (channel.channel_index.b != -1) pixels[x + row_y + channel.channel_index.b] = b;
			if (channel.channel_index.a != -1) pixels[x + row_y + channel.channel_index.a] = a;
		}
	}
}

bool PXL_Bitmap::check_has_transparency() {
	//checks whether the specified pixels contain any transparency
	if (pixels == NULL || channel.channel_index.a != -1) {
		return false;
	}else {
		for (int n = 0; n < buffer_size; n += channel.num_channels) {
			if (pixels[n + channel.channel_index.a] == 0) {
				has_transparency = true;
				return true;
			}
		}
	}
}

void PXL_Bitmap::free() {
	if (buffer_loaded) {
		buffer_loaded = false;
		if (pixels != NULL) {
			delete[] pixels;
			pixels = NULL;
		}
	}
}

PXL_Bitmap::~PXL_Bitmap() {
	free();
}