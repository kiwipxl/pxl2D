#ifndef TEXTURE_H
#define TEXTURE_H

#include "PXL_BufferObject.h"
#include "PXL_Rect.h"
#include "PXL_Bitmap.h"

class PXL_Texture {

	public:
		PXL_Texture();
		PXL_Texture(PXL_Bitmap* bitmap);
		~PXL_Texture();
		
		float width;
		float height;
		GLuint id;
		PXL_Bitmap* s;
		bool texture_created;
		PXL_BufferObject* buffer_object = NULL;
		PXL_Rect last_src_rect;
		PXL_Vec4 last_colour;

		void create_texture(PXL_Bitmap* bitmap, int buffer_size = 4);
		void set_colour(float r, float g, float b, float a);
		void free();
};

extern PXL_Texture* PXL_create_texture(PXL_Bitmap* bitmap, int buffer_size = 4);

#endif