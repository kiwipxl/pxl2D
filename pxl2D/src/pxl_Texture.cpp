#include "PXL_Texture.h"
#include <iostream>

PXL_Texture::PXL_Texture() {
	texture_created = false;
}

PXL_Texture::PXL_Texture(PXL_Bitmap* bitmap) {
	texture_created = false;
	create_texture(bitmap);
}

PXL_Texture* PXL_create_texture(PXL_Bitmap* bitmap) {
	return new PXL_Texture(bitmap);
}

void PXL_Texture::create_texture(PXL_Bitmap* bitmap) {
	if (bitmap != NULL) {
		//if the texture is already texture_created then delete the texture but not the buffer
		if (texture_created) {
			glDeleteTextures(1, &id);
			texture_created = false;
		}

		width = bitmap->w;
		height = bitmap->h;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->pixels);
		glBindTexture(GL_TEXTURE_2D, NULL);
		set_filters();

		texture_created = true;
	}
}

void PXL_Texture::set_filters(PXL_TextureFilter min_filter, PXL_TextureFilter max_filter) {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, max_filter);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void PXL_Texture::free() {
	if (texture_created) {
		glDeleteTextures(1, &id);
		texture_created = false;
	}
}

PXL_Texture::~PXL_Texture() {
	free();
}