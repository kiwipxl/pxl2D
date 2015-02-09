#ifndef _PXL_TEXTURE_H
#define _PXL_TEXTURE_H

#include <glew.h>
#include "PXL_Structs.h"
#include "PXL_Bitmap.h"

typedef int PXL_TextureFilter;

#define PXL_GL_NEAREST_MIPMAP_NEAREST GL_NEAREST_MIPMAP_NEAREST
#define PXL_GL_LINEAR_MIPMAP_NEAREST GL_LINEAR_MIPMAP_NEAREST
#define PXL_GL_NEAREST_MIPMAP_LINEAR GL_NEAREST_MIPMAP_LINEAR
#define PXL_GL_LINEAR_MIPMAP_LINEAR GL_LINEAR_MIPMAP_LINEAR
#define PXL_GL_NEAREST GL_NEAREST
#define PXL_GL_LINEAR GL_LINEAR

/** The PXL_Texture class handles uploading of pixel data to the GPU to create a texture that can be
used with a PXL_Batch to render images to the screen
A texture can be created with a PXL_Bitmap, a PXL_PixelBuffer or a raw pixel array.
**/
class PXL_Texture {

	public:
		PXL_Texture();
		/** Creates the texture from specified bitmap
		@param bitmap Holds all pixel information for an image
		@param pixel_mode The pixel type of the pixel data (default is R, G, B, A)
		**/
		PXL_Texture(PXL_Bitmap* bitmap, int pixel_mode = GL_RGBA);
		PXL_Texture(int w, int h, unsigned char* pixels = NULL, int pixel_mode = GL_RGBA);
		~PXL_Texture();

		bool texture_created; /**< Defines whether the texture has been created or not **/
		bool has_transparency = false;

		/** Creates the texture from specified bitmap
		@param bitmap Holds all pixel information for an image
		@param pixel_mode The pixel type of the pixel data (default is R, G, B, A)
		**/
		void create(PXL_Bitmap* bitmap, int pixel_mode = GL_RGBA);
		void create(int w, int h, unsigned char* pixels, int pixel_mode = GL_RGBA);

		void bind();

		/** Sets filter parameters for uploaded texture
		@param min_filter The filter value for when the texture is scaled and is smaller than its original size
		@param max_filter The filter value for when the texture is scaled and is larger than its original size
		**/
		void set_filters(PXL_TextureFilter min_filter = PXL_GL_LINEAR, PXL_TextureFilter max_filter = PXL_GL_LINEAR);

		/** Deletes all texture information
		**/
		void free();

		/** Gets the raw pixel array contents stored in the GPU
		\return A raw pixel byte array
		**/
		unsigned char* get_pixels();
		/** Gets the texture width
		\return The width of the texture
		**/
		const int get_width() { return width; }
		/** Gets the texture height
		\return The height of the texture
		**/
		const int get_height() { return height; }
		/** Gets the opengl id associated with this texture
		\return The id
		**/
		const GLint get_id() { return id; }

	protected:
		int width; /**< The width of the texture **/
		int height; /**< The height of the texture **/
		GLuint id; /**< The opengl id associated with the texture **/
};

/**
\*brief: Creates a texture from specified bitmap
@param bitmap Holds all pixel information for an image
@param pixel_mode The pixel type of the pixel data (default is R, G, B, A)
**/
extern PXL_Texture* PXL_create_texture(PXL_Bitmap* bitmap, int pixel_mode = GL_RGBA);

#endif