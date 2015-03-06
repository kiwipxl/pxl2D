#ifndef _PXL_BITMAP_H
#define _PXL_BITMAP_H

#include "PXL_GraphicsAPI.h"
#include <string>
#include "PXL_Colour.h"
#include "PXL_Structs.h"
#include "system/PXL_API.h"

struct PXL_Channel {

	PXL_uint num_channels;
	PXL_uint gl_pixel_mode;

	struct PXL_ChannelIndex {

		PXL_ChannelIndex() { }
		PXL_ChannelIndex(PXL_short c_r, PXL_short c_g, PXL_short c_b, PXL_short c_a) : r(c_r), g(c_g), b(c_b), a(c_a) { }

		PXL_short r = 0, g = 0, b = 0, a = 0;

	} channel_index;
};

static const PXL_Channel PXL_CHANNEL_RGB			{ 3, GL_RGB,					{ 0, 1, 2, -1 }		};
static const PXL_Channel PXL_CHANNEL_RGBA			{ 4, GL_RGBA,					{ 0, 1, 2, 3 }		};

//android GLES does not support GL_BGRA, so leave out temporarily and maybe add back later
//static const PXL_Channel PXL_CHANNEL_BGRA			{ 4, GL_BGRA,					{ 2, 1, 0, 3 }		};

static const PXL_Channel PXL_CHANNEL_GRAY_ALPHA		{ 2, GL_LUMINANCE_ALPHA,		{ 0, 0, 0, 1 }		};
static const PXL_Channel PXL_CHANNEL_ALPHA			{ 1, GL_ALPHA,					{ -1, -1, -1, 0 }	};

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
		PXL_uint width;
		PXL_uint height;
		PXL_ubyte* pixels;
		PXL_uint buffer_size;
		PXL_uint row_size;
		PXL_Channel channel;

		bool check_has_transparency();
};

/**
\*brief: loads and creates a bitmap from the specified path
\*param [path]: the path and file name for the bitmap to load
**/
extern PXL_Bitmap* PXL_create_bitmap(std::string path);

#endif