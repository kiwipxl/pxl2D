#include "PXL_Texture.h"
#include <iostream>

void PXL_create_texture(PXL_Bitmap* bitmap, int buffer_size) {
	PXL_Texture* t = new PXL_Texture();
	t->create_texture(bitmap, buffer_size);
}

void PXL_Texture::create_texture(PXL_Bitmap* bitmap, int buffer_size) {
	if (bitmap != NULL) {
		//if the texture is already created then delete the texture but not the buffer
		if (created) {
			glDeleteTextures(1, &id);
			created = false;
		}else {
			//free the buffer if it's already being used and create a new buffer object
			if (buffer_object != NULL) { buffer_object->free(); }
			buffer_object = new PXL_BufferObject(buffer_size);
		}

		width = bitmap->w;
		height = bitmap->h;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, NULL);
		created = true;
		set_colour(1, 1, 1, 1);
	}
}

void PXL_Texture::set_colour(float r, float g, float b, float a) {
	PXL_VertexPoint* v = buffer_object->vertex_data;

	for (int n = 0; n < buffer_object->buffer_size; ++n) {
		v[n].colour.x = r;
		v[n].colour.y = g;
		v[n].colour.z = b;
		v[n].colour.w = a;
	}
}

void PXL_Texture::free() {
	if (created) {
		glDeleteTextures(1, &id);
		delete buffer_object;
		buffer_object = NULL;
		created = false;
	}
}

PXL_Texture::~PXL_Texture() {
	free();
}