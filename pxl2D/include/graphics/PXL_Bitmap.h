#ifndef _PXL_BITMAP_H
#define _PXL_BITMAP_H

#include <glew.h>
#include <string>
#include "PXL_Colour.h"
#include "system/PXL_API.h"

struct PXL_Channel {

	PXL_uint num_channels;
	PXL_uint gl_pixel_mode;
};

static const PXL_Channel PXL_CHANNEL_RGB		{ 3, GL_RGB };
static const PXL_Channel PXL_CHANNEL_RGBA		{ 4, GL_RGBA };
static const PXL_Channel PXL_CHANNEL_BGRA		{ 4, GL_BGRA };
static const PXL_Channel PXL_CHANNEL_RED		{ 1, GL_RED };
static const PXL_Channel PXL_CHANNEL_GREEN		{ 1, GL_GREEN };
static const PXL_Channel PXL_CHANNEL_BLUE		{ 1, GL_BLUE };
static const PXL_Channel PXL_CHANNEL_ALPHA		{ 1, GL_ALPHA };

class PXL_Bitmap {

	public:
		PXL_Bitmap();
		/**
		\*brief: bitmap deconstructor
		**/
		~PXL_Bitmap();

		//bitmap data
		bool has_transparency = false;

		/**
		\*brief: loads the bitmap
		\*param [path]: the path and file name for the bitmap to load
		**/
		bool create_bitmap(std::string path);
		/**
		\*brief: constructs the bitmap with specified values
		\*param [width]: the width of the image
		\*param [height]: the height of the image
		\*param [buffer]: an array of pixels for the image
		**/
		void create_bitmap(int width, int height, PXL_Colour fill_colour, PXL_Channel pixel_channel);
		/**
		\*brief: constructs the bitmap with specified values
		\*param [width]: the width of the image
		\*param [height]: the height of the image
		\*param [buffer]: an array of pixels for the image
		**/
		void create_bitmap(int width, int height, PXL_ubyte* pixel_buffer, PXL_Channel pixel_channel);

		void fill(PXL_Colour colour);

		/**
		\*brief: frees all data from the bitmap
		**/
		void free();

		PXL_uint get_width() { return width; }
		PXL_uint get_height() { return height; }
		PXL_Channel get_channel() { return channel; }
		PXL_uint get_num_channels() { return channel.num_channels; }
		PXL_ubyte* get_pixels() { return pixels; }

	private:
		//buffer info
		bool buffer_loaded;
		int width;
		int height;
		PXL_ubyte* pixels;
		PXL_uint buffer_size;
		PXL_uint row_size;
		PXL_Channel channel;
		int bit_depth;

		bool check_has_transparency();
};

/**
\*brief: loads and creates a bitmap from the specified path
\*param [path]: the path and file name for the bitmap to load
**/
extern PXL_Bitmap* PXL_create_bitmap(std::string path);

#endif