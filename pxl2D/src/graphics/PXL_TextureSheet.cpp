#include "PXL_TextureSheet.h"
#include <iostream>
#include <system/PXL_Window.h>

PXL_FrameBuffer* sheet_frame_buffer = NULL;

PXL_TextureSheet::PXL_TextureSheet() {
	//initiate sheet
	texture_created = false;
	width = 0;
	height = 0;
	bg_colour = PXL_COLOUR_TRANSPARENT_BLACK;

	batch = new PXL_Batch(PXL_BATCH_SMALL);

	if (sheet_frame_buffer == NULL) {
		sheet_frame_buffer = new PXL_FrameBuffer(1, 1);
	}
}

void PXL_TextureSheet::create_sheet(bool clear_list, bool dispose_all) {
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

	create_texture(width, height, NULL, PXL_CHANNEL_RGBA);
	sheet_frame_buffer->get_texture()->create_texture(width, height, NULL, PXL_CHANNEL_RGBA);

	sheet_frame_buffer->clear(bg_colour.r, bg_colour.g, bg_colour.b, bg_colour.a);
	batch->set_target(sheet_frame_buffer);
	batch->render_all();

	batch->set_target();

	PXL_Rect rect;
	rect.x = 0; rect.y = 0; rect.w = width; rect.h = height;
	sheet_frame_buffer->blit(this, &rect);

	glViewport(0, 0, PXL_window_width, PXL_window_height);
	glReadBuffer(GL_BACK);

	texture_created = true;

	if (dispose_all) {
		for (int n = 0; n < texture_list.size(); ++n) {
			delete texture_list[n];
		}
	}

	if (clear_list) clear();
}

void PXL_TextureSheet::clear() {
	texture_list.clear();
}

void PXL_TextureSheet::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin, 
						   PXL_Flip flip, int z_depth, float r, float g, float b, float a, 
						   PXL_ShaderProgram* shader, PXL_BlendMode blend_mode) {
	batch->add(texture, rect, src_rect, rotation, origin, flip, z_depth, r, g, b, a, shader, blend_mode);
	int w = rect->x + rect->w;
	int h = rect->y + rect->h;
	if (w > width) { width = w; }
	if (h > height) { height = h; }
	texture_list.push_back(texture);
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