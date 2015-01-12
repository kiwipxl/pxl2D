#ifndef TEXTURE_H
#define TEXTURE_H

#include "PXL_VBO.h"
#include "PXL_Rect.h"
#include "PXL_Bitmap.h"

enum PXL_MinTextureFilter {
	PXL_MIN_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	PXL_MIN_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
	PXL_MIN_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	PXL_MIN_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
	PXL_MIN_NEAREST = GL_NEAREST,
	PXL_MIN_LINEAR = GL_LINEAR
};

enum PXL_MaxTextureFilter {
	PXL_MAX_LINEAR = GL_LINEAR, 
	PXL_MAX_NEAREST = GL_NEAREST
};

class PXL_Texture {

	public:
		PXL_Texture();
		PXL_Texture(PXL_Bitmap* bitmap);
		~PXL_Texture();

		bool texture_created;
		PXL_VBO* buffer_object = NULL;
		PXL_Rect last_src_rect;
		PXL_RGBA last_colour;

		void create_texture(PXL_Bitmap* bitmap, int buffer_size = 4);
		void set_colour(float r, float g, float b, float a);
		void set_filters(PXL_MinTextureFilter min_filter = PXL_MIN_LINEAR, PXL_MaxTextureFilter max_filter = PXL_MAX_LINEAR);

		void free();

		int get_width() { return width; }
		int get_height() { return height; }
		GLint get_id() { return id; }

	private:
		float width;
		float height;
		GLuint id;
};

extern PXL_Texture* PXL_create_texture(PXL_Bitmap* bitmap, int buffer_size = 4);

#endif