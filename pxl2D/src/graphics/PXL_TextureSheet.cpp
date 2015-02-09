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

	batch = new PXL_Batch(PXL_BATCH_SMALL);
}

void PXL_TextureSheet::create_sheet(bool dispose_all) {
	if (width == 0 || height == 0) { return; }

	//if the texture is already created then delete the sheet texture
	if (texture_created) {
		glDeleteTextures(1, &id);
		texture_created = false;
	}

	glViewport(0, 0, width, height);
	batch->perspective_mat.identity();
	batch->perspective_mat.scale(1.0f / (width / 2), 1.0f / (height / 2));
	batch->perspective_mat.translate(-1.0f, -1.0f);

	create_texture(width, height, NULL);
	sheet_frame_buffer->get_texture()->create_texture(width, height, 0);

	sheet_frame_buffer->clear(bg_colour.r, bg_colour.g, bg_colour.b, bg_colour.a);
	batch->set_target(sheet_frame_buffer);
	batch->render_all();

	batch->set_target();

	PXL_Rect rect;
	rect.x = 0; rect.y = 0; rect.w = width; rect.h = height;
	sheet_frame_buffer->blit(this, &rect);

	glViewport(0, 0, PXL_window_width, PXL_window_height);
	glReadBuffer(GL_BACK);

	//todo dispose all from batch list
	//todo bind frame buffer texture
	//todo bitmap pixel modes
	//todo use texsubimagestorage for textures (can specify to turn off though)
	//todo create texture only once and use a texture object
	//todo remove unique ids from textures
	//todo texture's hold pixel blocks on multiple getpixel calls
	//todo texture bitmap information (buffer size, channels, ect), optional: texture's inherit bitmaps and protected methods
	//todo min x, y positions using glviewport

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
		glDeleteTextures(1, &id);
		texture_created = false;
	}
}

PXL_TextureSheet::~PXL_TextureSheet() {
	free();
}