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
		PXL_Texture();
		PXL_Texture(PXL_Bitmap* bitmap);
		~PXL_Texture();

		bool texture_created;

		void create_texture(PXL_Bitmap* bitmap);
		void set_filters(PXL_TextureFilter min_filter = PXL_GL_LINEAR, PXL_TextureFilter max_filter = PXL_GL_LINEAR);

		void free();

		int get_width() { return width; }
		int get_height() { return height; }
		GLint get_id() { return id; }

	private:
		float width;
		float height;
		GLuint id;
};

extern PXL_Texture* PXL_create_texture(PXL_Bitmap* bitmap);

#endif