#include "PXL_Texture.h"
#include <iostream>

PXL_Texture::PXL_Texture() {
	texture_created = false;
}

PXL_Texture::PXL_Texture(PXL_Bitmap* bitmap) {
	texture_created = false;
	create_texture(bitmap);
}

PXL_Texture* PXL_create_texture(PXL_Bitmap* bitmap, int buffer_size) {
	return new PXL_Texture(bitmap);
}

void PXL_Texture::create_texture(PXL_Bitmap* bitmap, int buffer_size) {
	if (bitmap != NULL) {
		//if the texture is already texture_created then delete the texture but not the buffer
		if (texture_created) {
			glDeleteTextures(1, &id);
			texture_created = false;
		}else {
			//free the buffer if it's already being used and create a new buffer object
			if (buffer_object != NULL) { buffer_object->free(); }
			buffer_object = new PXL_VBO(buffer_size);
		}

		width = bitmap->w;
		height = bitmap->h;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->pixels);
		glBindTexture(GL_TEXTURE_2D, NULL);
		set_filters();
		texture_created = true;
		set_colour(255, 255, 255, 255);
	}
}

void PXL_Texture::set_colour(float r, float g, float b, float a) {
	PXL_VertexPoint* v = buffer_object->vertex_data;

	for (int n = 0; n < buffer_object->buffer_size; ++n) {
		v[n].colour.r = r;
		v[n].colour.g = g;
		v[n].colour.b = b;
		v[n].colour.a = a;
	}
}

void PXL_Texture::set_filters(PXL_MinTextureFilter min_filter, PXL_MaxTextureFilter max_filter) {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, max_filter);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void PXL_Texture::free() {
	if (texture_created) {
		glDeleteTextures(1, &id);
		delete buffer_object;
		buffer_object = NULL;
		texture_created = false;
	}
}

PXL_Texture::~PXL_Texture() {
	free();
}