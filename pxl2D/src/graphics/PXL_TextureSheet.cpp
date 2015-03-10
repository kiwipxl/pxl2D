#include "graphics/PXL_TextureSheet.h"
#include <iostream>
#include "system/PXL_Window.h"

PXL_FrameBuffer* sheet_frame_buffer = NULL;

PXL_TextureSheet::PXL_TextureSheet(PXL_BatchSize max_vertices) {
	//initiate sheet
	texture_created = false;
	width = 0;
	height = 0;
	bg_colour = PXL_COLOUR_TRANSPARENT_BLACK;

	batch = new PXL_Batch(max_vertices);
}

void PXL_TextureSheet::create_sheet(PXL_Channel sheet_channel, bool dispose_batch, bool dispose_list, bool clear_list) {
	if (!batch->is_created()) {
		PXL_show_exception("Could not create texture sheet, batch has been disposed", PXL_ERROR_TEXTURE_SHEET_CREATION_FAILED);
		return;
	}

	if (width == 0 || height == 0) {
		PXL_show_exception("Could not create texture sheet, width/height are less than 0", PXL_ERROR_TEXTURE_SHEET_CREATION_FAILED);
		return;
	}

	//initiate frame buffer if it has never been created before
	if (sheet_frame_buffer == NULL) {
		sheet_frame_buffer = new PXL_FrameBuffer(1, 1);
	}

	glViewport(0, 0, width, height);
	batch->perspective_mat.identity();
	batch->perspective_mat.scale(1.0f / (width / 2), 1.0f / (height / 2));
	batch->perspective_mat.translate(-1.0f, -1.0f);

	create_texture(width, height, NULL, sheet_channel);
	sheet_frame_buffer->get_texture()->create_texture(width, height, NULL, sheet_channel);

	sheet_frame_buffer->clear(bg_colour.r, bg_colour.g, bg_colour.b, bg_colour.a);
	batch->set_target(sheet_frame_buffer);
	batch->render_all();

	batch->set_target();

	PXL_Rect rect;
	rect.x = 0; rect.y = 0; rect.w = width; rect.h = height;
	sheet_frame_buffer->blit(*this, &rect);

	//todo: set viewport back to its previous value
	//todo: glreadbuffer not supported in gles2
	//glReadBuffer(GL_BACK);

	texture_created = true;

	if (dispose_batch) {
		batch->free();
	}

	if (dispose_list) {
		for (size_t n = 0; n < texture_list.size(); ++n) {
			delete texture_list[n];
		}
	}

	if (clear_list) clear();
}

void PXL_TextureSheet::clear() {
	texture_list.clear();
}

void PXL_TextureSheet::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin, 
						   PXL_Flip flip, int z_depth, PXL_Colour colour, 
						   PXL_ShaderProgram* shader, PXL_BlendMode blend_mode) {
	if (!batch->is_created()) {
		PXL_show_exception("Could not add texture to texture sheet as the batch has been disposed", PXL_ERROR_TEXTURE_SHEET_ADD_FAILED);
		return;
	}

	batch->add(*texture, rect, src_rect, rotation, origin, flip, z_depth, colour, shader, blend_mode);
	int w = rect->x + rect->w;
	int h = rect->y + rect->h;
	if (w > width) { width = w; }
	if (h > height) { height = h; }
	texture_list.push_back(texture);
}

void PXL_TextureSheet::free() {
	if (texture_created) {
		glDeleteTextures(1, &id);
		
		batch->free();
		for (size_t n = 0; n < texture_list.size(); ++n) {
			delete texture_list[n];
		}
		clear();

		texture_created = false;
	}
}

PXL_TextureSheet::~PXL_TextureSheet() {
	free();
}