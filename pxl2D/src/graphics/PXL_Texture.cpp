#include "PXL_Texture.h"
#include <iostream>

PXL_Texture::PXL_Texture() {
	texture_created = false;
}

PXL_Texture::PXL_Texture(PXL_Bitmap* bitmap, int pixel_mode) {
	texture_created = false;
	create(bitmap, pixel_mode);
}

PXL_Texture::PXL_Texture(int w, int h, unsigned char* pixels, int pixel_mode) {
	texture_created = false;
	create(w, h, pixels, pixel_mode);
}

PXL_Texture* PXL_create_texture(PXL_Bitmap* bitmap, int pixel_mode) {
	return new PXL_Texture(bitmap, pixel_mode);
}

void PXL_Texture::create(PXL_Bitmap* bitmap, int pixel_mode) {
	if (bitmap != NULL) {
		create(bitmap->width, bitmap->height, bitmap->pixels, pixel_mode);
		texture_created = true;
	}
}

void PXL_Texture::create(int w, int h, unsigned char* pixels, int pixel_mode) {
	if (pixels == NULL) {
		has_transparency = true;
	}else {
		for (int n = 0; n < (w * h) * 4; n += 4) {
			/*if (pixels[n + 3] == 0) {
				pixels[n] = 255;
				pixels[n + 1] = 255;
				pixels[n + 2] = 255;
				pixels[n + 3] = 0;
				has_transparency = true;
			}*/
		}
	}

	width = w;
	height = h;
	if (!texture_created) { glGenTextures(1, &id); }

	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, pixel_mode, GL_UNSIGNED_BYTE, pixels);

	if (!texture_created) { set_filters(); }

	texture_created = true;
}

void PXL_Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, id);
}

unsigned char* PXL_Texture::get_pixels() {
	if (texture_created) {
		bind();
		unsigned char* pixels = new unsigned char[(width * height) * 4];
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		return pixels;
	}
	return NULL;
}

void PXL_Texture::set_filters(PXL_TextureFilter min_filter, PXL_TextureFilter max_filter) {
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, max_filter);
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