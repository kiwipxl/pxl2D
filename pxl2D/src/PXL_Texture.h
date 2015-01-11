#ifndef PXL_TEXTURE_H
#define PXL_TEXTURE_H

#include <SDL.h>
#include <glew.h>
#include "BufferObject.h"

class PXL_Texture {

	public:
		PXL_Texture() { created = false; }
		~PXL_Texture();
		
		float width;
		float height;
		GLuint id;
		SDL_Surface* s;
		bool created;
		BufferObject* buffer_object = NULL;
		SDL_Rect last_src_rect;
		PXL_Vec4 last_colour;

		void create_texture(SDL_Surface* surface, int buffer_size = 4);
		void set_colour(float r, float g, float b, float a);
		void free();
};

#endif