#include "PXL_TextureSheet.h"
#include <iostream>
#include <system/PXL_Window.h>

PXL_FrameBuffer* sheet_frame_buffer;

void PXL_TextureSheet::init() {
	sheet_frame_buffer = new PXL_FrameBuffer(1, 1);
}

PXL_TextureSheet::PXL_TextureSheet() {
	//initiate sheet
	texture_created = false;
	width = 0;
	height = 0;
	bg_colour.r = bg_colour.g = bg_colour.b = bg_colour.a = 0;
	bg_colour.b = bg_colour.a = 1;

	batch = new PXL_Batch(PXL_BATCH_SMALL);
}

void PXL_TextureSheet::create(bool dispose_all) {
	if (width == 0 || height == 0) { return; }

	//if the texture is already created then delete the sheet texture
	if (texture_created) {
		glDeleteTextures(1, &gl_id);
		texture_created = false;
	}

	glViewport(0, 0, width, height);
	batch->perspective_mat.identity();
	batch->perspective_mat.scale(1.0f / (width / 2), -1.0f / (height / 2));
	batch->perspective_mat.translate(-1.0f, 1.0f);

	//create texture from resulting pixels
	glGenTextures(1, &gl_id);
	glBindTexture(GL_TEXTURE_2D, gl_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	set_filters();

	sheet_frame_buffer->get_texture()->create(width, height, 0);

	sheet_frame_buffer->clear(bg_colour.r, bg_colour.g, bg_colour.b, bg_colour.a);
	batch->set_target(sheet_frame_buffer);
	batch->render_all();

	batch->set_target();

	//copytexsubimage copy method
	sheet_frame_buffer->bind(PXL_GL_FRAMEBUFFER_READ);
	//glBindTexture(GL_TEXTURE_2D, gl_id);
	//glReadBuffer(GL_COLOR_ATTACHMENT0);
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);

	unsigned char* temp = sheet_frame_buffer->get_pixels();
	unsigned char* pixels = new unsigned char[(width * height) * 4];
	int row_size = width * 4;
	for (int n = 0; n < height; ++n) {
		memcpy(pixels + (n * row_size), temp + ((height - 1 - n) * row_size), row_size);
	}

	glBindTexture(GL_TEXTURE_2D, gl_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glViewport(0, 0, PXL_window_width, PXL_window_height);
	glReadBuffer(GL_BACK);
	glBindFramebuffer(PXL_GL_FRAMEBUFFER_READ, 0);

	//todo dispose all from batch list
	//todo bind frame buffer texture
	//todo bitmap pixel modes
	//todo use texsubimagestorage for textures (can specify to turn off though)
	//todo create texture only once and use a texture object
	//todo remove unique ids from textures
	//todo texture's hold pixel blocks on multiple getpixel calls
	//todo texture bitmap information (buffer size, channels, ect), optional: texture's inherit bitmaps and protected methods

	texture_created = true;
}

void PXL_TextureSheet::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin, 
						   PXL_Flip flip, int z_depth, float r, float g, float b, float a, 
						   PXL_ShaderProgram* shader, PXL_BlendMode blend_mode) {
	batch->add(texture, rect, src_rect, rotation, origin, flip, z_depth, r, g, b, a, shader, blend_mode);
	int w = rect->x + rect->w;
	int h = rect->y + rect->h;
	if (w > width) { width = w; }
	if (h > height) { height = h; }
}

void PXL_TextureSheet::free() {
	if (texture_created) {
		glDeleteTextures(1, &gl_id);
		texture_created = false;
	}
}

PXL_TextureSheet::~PXL_TextureSheet() {
	free();
}