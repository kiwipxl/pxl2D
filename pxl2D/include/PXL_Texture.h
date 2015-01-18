#ifndef TEXTURE_H
#define TEXTURE_H

#include <glew.h>
#include "PXL_Structs.h"
#include "PXL_Bitmap.h"

enum PXL_TextureFilter {
	PXL_GL_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	PXL_GL_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
	PXL_GL_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	PXL_GL_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
	PXL_GL_NEAREST = GL_NEAREST,
	PXL_GL_LINEAR = GL_LINEAR
};

class PXL_Texture {

	public:
		/**
		\*brief: default constructor
		**/
		PXL_Texture();
		/**
		\*brief: creates the texture from specified bitmap
		\*param [bitmap]: holds all pixel information for an image
		**/
		PXL_Texture(PXL_Bitmap* bitmap);
		/**
		\*brief: batch deconstructor
		**/
		~PXL_Texture();

		//texture info
		bool texture_created;

		/**
		\*brief: creates the texture from specified bitmap
		\*param [bitmap]: holds all pixel information for an image
		**/
		void create_texture(PXL_Bitmap* bitmap);

		/**
		\*brief: sets filter parameters for uploaded texture
		\*param [min_filter]: the filter type for when the texture is minimised
		\*param [max_filter]: the filter type for when the texture is maximised
		**/
		void set_filters(PXL_TextureFilter min_filter = PXL_GL_LINEAR, PXL_TextureFilter max_filter = PXL_GL_LINEAR);

		/**
		\*brief: collects all pixel information from opengl
		**/
		char* get_pixels();

		/**
		\*brief: deletes all texture information
		**/
		void free();

		/**
		\*brief: gets the texture width
		**/
		int get_width() { return width; }
		/**
		\*brief: gets the texture height
		**/
		int get_height() { return height; }
		/**
		\*brief: gets the id associated with this texture
		**/
		GLint get_id() { return id; }

	protected:
		//texture info
		int width;
		int height;
		GLuint id;
};

/**
\*brief: creates a texture from specified bitmap
\*param [bitmap]: holds all pixel information for an image
**/
extern PXL_Texture* PXL_create_texture(PXL_Bitmap* bitmap);

#endif