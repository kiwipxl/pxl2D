#include "graphics/PXL_Texture.h"
#include "system/PXL_Debug.h"

PXL_Texture::PXL_Texture() {
	texture_created = false;
}

bool PXL_Texture::create_texture(std::string file_path) {
	PXL_Bitmap bitmap;
	if (bitmap.create_bitmap(file_path)) {
		return create_texture(&bitmap);
	}
	return false;
}

bool PXL_Texture::create_texture(PXL_Bitmap* bitmap) {
	texture_created = false;
	if (bitmap != NULL) {
		create_texture(bitmap->get_width(), bitmap->get_height(), bitmap->get_pixels(), bitmap->get_channel());
		texture_created = true;
	}else {
		PXL_show_exception("Could not create texture, specified bitmap is NULL", PXL_ERROR_TEXTURE_CREATION_FAILED);
	}
	return texture_created;
}

bool PXL_Texture::create_texture(int w, int h, PXL_ubyte* pixels, PXL_Channel pixel_channel) {
	if (w <= 0 || h <= 0) {
		PXL_show_exception("Could not create texture, width/height are less than 0", PXL_ERROR_TEXTURE_CREATION_FAILED);
		return false;
	}

	width = w;
	height = h;
	channel = pixel_channel;

	glPixelStorei(GL_UNPACK_ALIGNMENT, channel.num_channels);

	if (!texture_created) { glGenTextures(1, &id); }

	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, channel.gl_pixel_mode, width, height, 0, channel.gl_pixel_mode, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (!texture_created) { set_filters(); }

	texture_created = true;

	return true;
}

void PXL_Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, id);
}

PXL_ubyte* PXL_Texture::get_pixels() {
	if (texture_created) {
		bind();
		PXL_ubyte* pixels = new PXL_ubyte[(width * height) * channel.num_channels];
		glReadPixels(0, 0, width, height, channel.gl_pixel_mode, GL_UNSIGNED_BYTE, pixels);
		//todo: glgetteximage not supported by gles2
		//glGetTexImage(GL_TEXTURE_2D, 0, channel.gl_pixel_mode, GL_UNSIGNED_BYTE, pixels);
		return pixels;
	}
	return NULL;
}

void PXL_Texture::set_filters(PXL_TextureFilter min_filter, PXL_TextureFilter max_filter) {
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max_filter);
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