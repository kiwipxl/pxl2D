#include "graphics/PXL_Bitmap.h"
#include "system/PXL_ImageIO.h"
#include "system/PXL_Debug.h"

PXL_Bitmap::PXL_Bitmap() {
	buffer_loaded = false;
}

bool PXL_Bitmap::create_bitmap(std::string path) {
	free();

	buffer_loaded = PXL_load_png(path, this);

	check_has_transparency();

	//fill(PXL_COLOUR_BLACK);

	return buffer_loaded;
}

void PXL_Bitmap::create_bitmap(int bitmap_width, int bitmap_height, PXL_Colour fill_colour, PXL_Channel pixel_channel) {
	free();
	set_attribs(bitmap_width, bitmap_height, pixel_channel);
	pixels = new uint8[buffer_size];

	fill(fill_colour);
	check_has_transparency();
}

void PXL_Bitmap::create_bitmap(int bitmap_width, int bitmap_height, PXL_Gradient gradient_fill, PXL_Channel pixel_channel) {
	free();
	set_attribs(bitmap_width, bitmap_height, pixel_channel);
	pixels = new uint8[buffer_size];

	fill(gradient_fill);
	check_has_transparency();
}

void PXL_Bitmap::create_bitmap(int bitmap_width, int bitmap_height, uint8* pixel_buffer, PXL_Channel pixel_channel) {
	free();
	set_attribs(bitmap_width, bitmap_height, pixel_channel);
	pixels = pixel_buffer;

	check_has_transparency();
}

void PXL_Bitmap::set_attribs(int bitmap_width, int bitmap_height, PXL_Channel pixel_channel) {
	buffer_loaded = true;
	width = bitmap_width;
	height = bitmap_height;

	channel = pixel_channel;
	row_size = width * channel.num_channels;
	buffer_size = row_size * height;
}

void PXL_Bitmap::fill(PXL_Colour colour) {
	if (pixels == NULL) return;

	//multiplies the input colour by 255 (as it's a 0-1 range float)
	PXL_Colour rgba = colour * 255;

	for (size_t y = 0; y < height; ++y) {
		int row_y = y * row_size;
		for (size_t x = 0; x < row_size; x += channel.num_channels) {
			if (channel.channel_index.r != -1) pixels[x + row_y + channel.channel_index.r] = rgba.r;
			if (channel.channel_index.g != -1) pixels[x + row_y + channel.channel_index.g] = rgba.g;
			if (channel.channel_index.b != -1) pixels[x + row_y + channel.channel_index.b] = rgba.b;
			if (channel.channel_index.a != -1) pixels[x + row_y + channel.channel_index.a] = rgba.a;
			//todo: check for pixel transparency here to remove the need for 2 pixel loops
		}
	}
}

void PXL_Bitmap::fill(PXL_Gradient gradient) {
	if (pixels == NULL) return;

	//multiplies the input colour by 255 (as it's a 0-1 range float)
	PXL_Colour c1 = gradient.g1 * 255;
	PXL_Colour c2 = gradient.g2 * 255;

	//calculate the rate it takes to interpolate between c1 and c2 for r, g, b a
	uint32 row_width = row_size / channel.num_channels;
	float r_rate = (c1.r - c2.r) / row_width;
	float g_rate = (c1.g - c2.g) / row_width;
	float b_rate = (c1.b - c2.b) / row_width;
	float a_rate = (c1.a - c2.a) / row_width;

	for (size_t y = 0; y < height; ++y) {
		int row_y = y * row_size;
		for (size_t x = 0; x < row_size; x += channel.num_channels) {
			//decrease/increase r, g, b, a depending on the difference between c1 and c2
			c1.r -= r_rate;
			c1.g -= g_rate;
			c1.b -= b_rate;
			c1.a -= a_rate;
			if (channel.channel_index.r != -1) pixels[x + row_y + channel.channel_index.r] = c1.r;
			if (channel.channel_index.g != -1) pixels[x + row_y + channel.channel_index.g] = c1.g;
			if (channel.channel_index.b != -1) pixels[x + row_y + channel.channel_index.b] = c1.b;
			if (channel.channel_index.a != -1) pixels[x + row_y + channel.channel_index.a] = c1.a;
			//todo: check for pixel transparency here to remove the need for 2 pixel loops
		}
		//calculate back to the original c1 values
		c1.r += r_rate * row_width;
		c1.g += g_rate * row_width;
		c1.b += b_rate * row_width;
		c1.a += a_rate * row_width;
	}
}

bool PXL_Bitmap::check_has_transparency() {
	//checks whether the specified pixels contain any transparency
	if (pixels == NULL || channel.channel_index.a == -1) {
		return false;
	}else {
		for (size_t n = 0; n < buffer_size; n += channel.num_channels) {
			if (pixels[n + channel.channel_index.a] != 255) {
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