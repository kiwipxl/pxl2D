#ifndef _PXL_BITMAP_H
#define _PXL_BITMAP_H

#include <string>
#include "graphics/PXL_GraphicsAPI.h"
#include "graphics/PXL_Colour.h"
#include "graphics/PXL_Structs.h"
#include "system/PXL_API.h"

struct PXL_Channel {

	uint32 num_channels;
	uint32 gl_pixel_mode;

	struct PXL_ChannelIndex {

		PXL_ChannelIndex() { }
		PXL_ChannelIndex(short c_r, short c_g, short c_b, short c_a) : r(c_r), g(c_g), b(c_b), a(c_a) { }

		short r = 0, g = 0, b = 0, a = 0;

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
		void create_bitmap(int width, int height, uint8* pixel_buffer, PXL_Channel pixel_channel);

		void fill(PXL_Colour colour);

		/**
		\*brief: frees all data from the bitmap
		**/
		void free();

		uint32 get_width() const { return width; }
		uint32 get_height() const { return height; }
		PXL_Channel get_channel() const { return channel; }
		uint32 get_num_channels() const { return channel.num_channels; }
		uint8* get_pixels() const { return pixels; }

	private:
		//buffer info
		bool buffer_loaded;
		uint32 width;
		uint32 height;
		uint8* pixels;
		uint32 buffer_size;
		uint32 row_size;
		PXL_Channel channel;

		bool check_has_transparency();
};

/**
\*brief: loads and creates a bitmap from the specified path
\*param [path]: the path and file name for the bitmap to load
**/
extern PXL_Bitmap* PXL_create_bitmap(std::string path);

#endif