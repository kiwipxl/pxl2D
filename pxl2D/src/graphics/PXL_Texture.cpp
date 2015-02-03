#include "PXL_Texture.h"
#include <iostream>

unsigned int unique_texture_id = 0;

PXL_Texture::PXL_Texture() {
	texture_created = false;
}

PXL_Texture::PXL_Texture(PXL_Bitmap* bitmap, int pixel_mode) {
	texture_created = false;
	create_texture(bitmap, pixel_mode);
}

PXL_Texture::PXL_Texture(int w, int h, unsigned char* pixels, int pixel_mode) {
	texture_created = false;
	create_texture(w, h, pixels, pixel_mode);
}

PXL_Texture* PXL_create_texture(PXL_Bitmap* bitmap, int pixel_mode) {
	return new PXL_Texture(bitmap, pixel_mode);
}

void PXL_Texture::create_texture(PXL_Bitmap* bitmap, int pixel_mode) {
	if (bitmap != NULL) {
		create_texture(bitmap->w, bitmap->h, bitmap->pixels, pixel_mode);
		texture_created = true;
	}
}

void PXL_Texture::create_texture(int w, int h, unsigned char* pixels, int pixel_mode) {
	//if the texture is already created then delete the texture but not the buffer
	if (texture_created) {
		glDeleteTextures(1, &gl_id);
		texture_created = false;
	}

	width = w;
	height = h;
	glGenTextures(1, &gl_id);
	glBindTexture(GL_TEXTURE_2D, gl_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, pixel_mode, GL_UNSIGNED_BYTE, pixels);
	set_filters();
	glBindTexture(GL_TEXTURE_2D, NULL);

	unique_id = unique_texture_id;
	++unique_texture_id;

	if (pixels == NULL) {
		has_transparency = true;
	}else {
		for (int n = 0; n < w * h; n += 4) {
			if (pixels[n + 3] == 0) {
				has_transparency = true;
				break;
			}
		}
	}

	texture_created = true;
}

unsigned char* PXL_Texture::get_pixels() {
	if (texture_created) {
		glBindTexture(GL_TEXTURE_2D, gl_id);
		unsigned char* pixels = new unsigned char[(width * height) * 4];
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		return pixels;
	}
	return NULL;
}

void PXL_Texture::set_filters(PXL_TextureFilter min_filter, PXL_TextureFilter max_filter) {
	glBindTexture(GL_TEXTURE_2D, gl_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, max_filter);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void PXL_Texture::free() {
	if (texture_created) {
		glDeleteTextures(1, &gl_id);
		texture_created = false;
	}
}

PXL_Texture::~PXL_Texture() {
	free();
}