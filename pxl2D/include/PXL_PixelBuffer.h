#ifndef PIXEL_BUFFER_H
#define PIXEL_BUFFER_H

class PXL_PixelBuffer {

	public:
		PXL_PixelBuffer();
		~PXL_PixelBuffer();

		char* buffer;
		unsigned int buffer_size;
		unsigned int width;
		unsigned int height;

		void free();
};

#endif