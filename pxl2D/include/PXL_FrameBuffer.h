#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "PXL_Structs.h"
#include "PXL_Texture.h"

/** The PXL_FrameBuffer class handles uploading of pixel data to the GPU to create a frame_buffer that can be
used with a PXL_Batch to render images to the screen
A frame_buffer can be created with a PXL_Bitmap, a PXL_PixelBuffer or a raw pixel array.
**/
class PXL_FrameBuffer {

	public:
		/** Creates the frame_buffer from specified bitmap
		@param bitmap Holds all pixel information for an image
		@param pixel_mode The pixel type of the pixel data (default is R, G, B, A)
		**/
		PXL_FrameBuffer();
		~PXL_FrameBuffer();

		bool frame_buffer_created; /**< Defines whether the frame buffer has been created or not **/

		/** Creates the frame_buffer from specified bitmap
		@param bitmap Holds all pixel information for an image
		@param pixel_mode The pixel type of the pixel data (default is R, G, B, A)
		**/
		void create_frame_buffer();

		void clear(float r, float g, float b, float a);

		/** Deletes all frame_buffer information
		**/
		void free();

		/** Gets the raw pixel array contents stored in the GPU
		\return A raw pixel byte array
		**/
		unsigned char* get_pixels();
		/** Gets the frame_buffer width
		\return The width of the frame_buffer
		**/
		const int get_width() { return width; }
		/** Gets the frame_buffer height
		\return The height of the frame_buffer
		**/
		const int get_height() { return height; }
		/** Gets the id associated with this frame_buffer
		\return The id
		**/
		const GLint get_id() { return id; }
		PXL_Texture* get_texture() { return texture; }
		const GLint get_texture_id() { return texture->get_id(); }

	protected:
		int width; /**< The width of the frame_buffer **/
		int height; /**< The height of the frame_buffer **/
		GLuint id; /**< The id associated with the frame_buffer **/
		PXL_Texture* texture;
};

/**
\*brief: Creates a frame_buffer from specified bitmap
@param bitmap Holds all pixel information for an image
@param pixel_mode The pixel type of the pixel data (default is R, G, B, A)
**/
extern PXL_FrameBuffer* PXL_create_frame_buffer();

#endif