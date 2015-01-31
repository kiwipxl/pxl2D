#include "PXL_Batch.h"
#include <iostream>
#include <algorithm>
#include "PXL_Window.h"
#include "system/PXL_Exception.h"

PXL_Batch::PXL_Batch(PXL_MaxQuads max_quads) {
	vbo_created = false;
	create_batch(max_quads);
}

PXL_Batch* PXL_create_batch(PXL_MaxQuads max_quads) {
	return new PXL_Batch(max_quads);
}

void PXL_Batch::create_batch(PXL_MaxQuads max_quads) {
	max_quads_amount = max_quads;

	//if the batch is already created then delete the vbo
	if (vbo_created) { free(); }

	clear_all();

	{
		//create the vbo
		glGenBuffers(1, &vertex_buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, max_quads_amount * 4 * sizeof(PXL_VertexPoint), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);

		current_texture_ids = new SparseTextureId[max_quads_amount];
		next_texture_ids = new SparseTextureId[max_quads_amount];
		vertex_batches = new PXL_VertexBatch[max_quads_amount];
		vertex_data = new PXL_VertexPoint[max_quads_amount * 4];
		vbo_created = true;
	}

	//set perspective matrix to window coordinates and translate to 0,0 top left
	view_mat.identity();
	perspective_mat.identity();

	perspective_mat.scale(1.0f / PXL_center_window_x, -1.0f / PXL_center_window_y);
	perspective_mat.translate(-1.0f, 1.0f);

	//enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//binds vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

	//enable vertex attrib pointers when rendering
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//set vertex shader attrib pointers
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(PXL_VertexPoint), (void*)offsetof(PXL_VertexPoint, pos));
	glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(PXL_VertexPoint), (void*)offsetof(PXL_VertexPoint, uv));
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(PXL_VertexPoint), (void*)offsetof(PXL_VertexPoint, colour));
}

void PXL_Batch::render_all(bool depth_test) {
	if (num_added != 0) {
		//if a framebuffer is specified, bind to it, if not bind to the default framebuffer
		if (target_frame_buffer != NULL) {
			target_frame_buffer->bind(PXL_GL_FRAMEBUFFER_WRITE);
		}else {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}

		//enable/disable depth testing depending on depth_test input
		if (depth_test) {
			glClear(GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
		}else {
			glDisable(GL_DEPTH_TEST);
		}

		draw_vbo();
	}
	clear_all();
}

void PXL_Batch::clear_all() {
	num_added = 0;
	vertex_data_size = 0;
	vertex_batch_index = 0;
	min_texture_id = UINT_MAX;
	max_texture_id = 0;
}

void PXL_Batch::set_target_shader(PXL_ShaderProgram* shader) {
	target_shader = shader;
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect) {
	if (verify_texture_add(texture, rect)) {
		add_quad(texture, rect, src_rect);
		++num_added;
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin) {
	if (verify_texture_add(texture, rect)) {
		add_quad(texture, rect, src_rect, rotation, origin);
		++num_added;
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin, 
					PXL_Flip flip) {
	if (verify_texture_add(texture, rect)) {
		add_quad(texture, rect, src_rect, rotation, origin, flip);
		++num_added;
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, PXL_Flip flip, 
					float r, float g, float b, float a) {
	if (verify_texture_add(texture, rect)) {
		add_quad(texture, rect, src_rect, 0, NULL, flip, r, g, b, a);
		++num_added;
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin, 
					PXL_Flip flip, float r, float g, float b, float a) {
	if (verify_texture_add(texture, rect)) {
		add_quad(texture, rect, src_rect, rotation, origin, flip, r, g, b, a);
		++num_added;
	}
}

bool PXL_Batch::verify_texture_add(PXL_Texture* texture, PXL_Rect* rect) {
	if (texture->texture_created) {
		if (rect->x + rect->w > 0 && rect->y + rect->h > 0 && rect->x < PXL_window_width && rect->y < PXL_window_height) {
			if (num_added >= max_quads_amount) {
				PXL_show_exception("Hit the max batch quad size");
				return false;
			}

			return true;
		}
	}
	return false;
}

void PXL_Batch::add_quad(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, 
						 float rotation, PXL_Vec2* origin, PXL_Flip flip, 
						 float r, float g, float b, float a) {
	//set the texture id and shader program for the vertex batch
	int index = last_freq_index;
	GLuint texture_id = texture->get_id();
	if (next_texture_ids[texture_id].frequency >= 1) {
		index = next_texture_ids[texture_id].batch_index;
		++next_texture_ids[texture_id].batch_index;
	}else {
		++last_freq_index;
	}
	PXL_VertexBatch* v_batch = vertex_batches + index;
	v_batch->num_vertices = 4;

	++current_texture_ids[texture_id].frequency;
	min_texture_id = PXL_min(min_texture_id, texture_id);
	max_texture_id = PXL_max(max_texture_id, texture_id);

	if (num_added >= max_quads_amount) {
		PXL_show_exception("Hit max batch quad size at " + std::to_string(max_quads_amount) + " max quads.");
	}

	//set vertex pos, uvs and colours
	vertex_data_size = index * 4;
	set_quad_pos(texture, rect, rotation, origin, flip);
	set_quad_uvs(texture, src_rect);
	set_quad_colours(r, g, b, a);
}

void PXL_Batch::set_quad_pos(PXL_Texture* texture, PXL_Rect* rect, float rotation, PXL_Vec2* origin, PXL_Flip flip) {
	PXL_VertexPoint* v = vertex_data + vertex_data_size;

	//set origin
	float origin_x = 0; float origin_y = 0;
	if (origin != NULL) { origin_x = origin->x; origin_y = origin->y; }

	//get positions from rect
	int x = rect->x - origin_x; int y = rect->y - origin_y;

	//set scale
	float scale_x = rect->w / texture->get_width(); float scale_y = rect->h / texture->get_height();
	switch (flip) {
		case PXL_FLIP_NONE:
			break;
		case PXL_FLIP_HORIZONTAL:
			scale_x = -scale_x;
			x += rect->w;
			origin_x -= rect->w;
			break;
		case PXL_FLIP_VERTICAL:
			scale_y = -scale_y;
			y += rect->h;
			origin_y -= rect->h;
			break;
	}

	//set vertex position including scale
	v[0].pos.x = x;											v[0].pos.y = y;
	v[1].pos.x = x + (texture->get_width() * scale_x);		v[1].pos.y = y;
	v[2].pos.x = x + (texture->get_width() * scale_x);		v[2].pos.y = y + (texture->get_height() * scale_y);
	v[3].pos.x = x;											v[3].pos.y = y + (texture->get_height() * scale_y);

	//apply rotation
	if (rotation != 0) {
		//set rotation to degrees rather than radians
		rotation = rotation / PXL_radian;
		float c = PXL_fast_cos(rotation); float s = PXL_fast_sin(rotation);

		//apply rotation transformation
		for (int n = 0; n < 4; ++n) {
			int v_x = (v[n].pos.x - origin_x) - x; int v_y = (v[n].pos.y - origin_y) - y;
			v[n].pos.x = ((c * v_x) - (s * v_y)) + x + origin_x;
			v[n].pos.y = ((s * v_x) + (c * v_y)) + y + origin_y;
		}
	}
}

void PXL_Batch::set_quad_uvs(PXL_Texture* texture, PXL_Rect* src_rect) {
	PXL_VertexPoint* v = vertex_data + vertex_data_size;

	//default un-normalised uv coords
	unsigned short uv_x = 0; unsigned short uv_y = 0; unsigned short uv_w = USHRT_MAX; unsigned short uv_h = USHRT_MAX;
	if (src_rect != NULL) {
		//calculate uv x, y, w, h by the src rect
		uv_x = (src_rect->x / texture->get_width()) * USHRT_MAX; uv_y = (src_rect->y / texture->get_height()) * USHRT_MAX;
		uv_w = (src_rect->w / texture->get_width()) * USHRT_MAX; uv_h = (src_rect->h / texture->get_height()) * USHRT_MAX;
	}

	//set uv coordinates
	v[0].uv.x = uv_x;										v[0].uv.y = uv_y;
	v[1].uv.x = uv_x + uv_w;								v[1].uv.y = uv_y;
	v[2].uv.x = uv_x + uv_w;								v[2].uv.y = uv_y + uv_h;
	v[3].uv.x = uv_x;										v[3].uv.y = uv_y + uv_h;
}

void PXL_Batch::set_quad_colours(float r, float g, float b, float a) {
	PXL_VertexPoint* v = vertex_data + vertex_data_size;

	//set vertex colours
	for (int n = 0; n < 4; ++n) {
		v[n].colour.r = r * 255;
		v[n].colour.g = g * 255;
		v[n].colour.b = b * 255;
		v[n].colour.a = a * 255;
	}
}

void PXL_Batch::set_target(PXL_FrameBuffer* f) {
	//sets the target frame buffer to be used for rendering
	target_frame_buffer = f;
}

void PXL_Batch::draw_vbo() {
	//if there are no textures to draw or no vertex data then return
	if (num_added == 0) { return; }

	//binds vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, num_added * 4 * sizeof(PXL_VertexPoint), &vertex_data[0], GL_DYNAMIC_DRAW);

	if (target_shader == NULL) { target_shader = PXL_default_shader; }
	//use specified program id
	glUseProgram(target_shader->get_program_id());

	//set matrix uniform in the vertex shader for the program
	view_mat.identity();
	glUniformMatrix4fv(target_shader->get_matrix_loc(), 1, true, (view_mat * perspective_mat).get_mat());

	//loops through each texture and draws the vertex data with that texture id
	int size = 0;
	int offset = 0;
	int batch_index = 0;
	last_freq_index = 0;
	for (int n = min_texture_id; n <= max_texture_id; ++n) {
		if (current_texture_ids[n].frequency >= 1) {
			glBindTexture(GL_TEXTURE_2D, n);

			offset = batch_index * 4;
			size = current_texture_ids[n].frequency * 4;

			//draw vertex data from vertex data in buffer
			glDrawArrays(GL_QUADS, offset, size);

			next_texture_ids[n].frequency = current_texture_ids[n].frequency;
			next_texture_ids[n].batch_index = batch_index;
			batch_index += current_texture_ids[n].frequency;
			current_texture_ids[n].frequency = 0;
			current_texture_ids[n].batch_index = 0;
		}
	}
	last_freq_index = offset + size;
}

void PXL_Batch::free() {
	glDeleteBuffers(1, &vertex_buffer_id);
	vbo_created = false;
}

PXL_Batch::~PXL_Batch() {
	free();
}