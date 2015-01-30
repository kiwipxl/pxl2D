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

		for (int n = 0; n < max_quads_amount; ++n) {
			vertex_batches.push_back(PXL_VertexBatch());
		}
		for (int n = 0; n < max_quads_amount * 4; ++n) {
			vertex_data.push_back(PXL_VertexPoint());
		}
		vbo_created = true;
	}

	set_shader(PXL_default_shader);

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
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target_frame_buffer->get_id());
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
}

void PXL_Batch::set_shader(PXL_ShaderProgram* shader) {
	PXL_ShaderProgram* sh = shader;
	if (sh == NULL) { sh = PXL_default_shader; }

	//use specified program id
	glUseProgram(sh->get_program_id());

	//set matrix uniform in the vertex shader for the program
	view_mat.identity();
	glUniformMatrix4fv(sh->get_matrix_loc(), 1, true, (view_mat * perspective_mat).get_mat());
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect) {
	if (verify_texture_add(texture, rect)) {
		add_quad(texture, rect, src_rect);
		++num_added;
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, PXL_ShaderProgram* shader) {
	if (verify_texture_add(texture, rect)) {
		add_quad(texture, rect, src_rect, 0, NULL, 0, 1, 1, 1, 1, shader);
		++num_added;
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin) {
	if (verify_texture_add(texture, rect)) {
		add_quad(texture, rect, src_rect, rotation, origin, 0, 1, 1, 1, 1, NULL);
		++num_added;
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin, 
					PXL_Flip flip, PXL_ShaderProgram* shader) {
	if (verify_texture_add(texture, rect)) {
		add_quad(texture, rect, src_rect, rotation, origin, flip, 1, 1, 1, 1, shader);
		++num_added;
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, PXL_Flip flip, PXL_ShaderProgram* shader, 
					float r, float g, float b, float a) {
	if (verify_texture_add(texture, rect)) {
		add_quad(texture, rect, src_rect, 0, NULL, flip, r, g, b, a, shader);
		++num_added;
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin, 
					PXL_Flip flip, PXL_ShaderProgram* shader, float r, float g, float b, float a) {
	if (verify_texture_add(texture, rect)) {
		add_quad(texture, rect, src_rect, rotation, origin, flip, r, g, b, a, shader);
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
							 float r, float g, float b, float a, PXL_ShaderProgram* shader) {
	//set the texture id and shader program for the vertex batch
	vertex_batches[num_added].texture_id = texture->get_id();
	vertex_batches[num_added].shader = shader;
	vertex_batches[num_added].num_vertices = 4;

	if (num_added >= max_quads_amount) {
		PXL_show_exception("Hit max batch quad size at " + std::to_string(max_quads_amount) + " max quads.");
	}

	//set vertex pos, uvs and colours
	set_quad_pos(texture, rect, rotation, origin, flip);
	set_quad_uvs(texture, src_rect);
	set_quad_colours(r, g, b, a);

	vertex_data_size += 4;
}

void PXL_Batch::set_quad_pos(PXL_Texture* texture, PXL_Rect* rect, float rotation, PXL_Vec2* origin, PXL_Flip flip) {
	PXL_VertexPoint* v = &vertex_data[vertex_data_size];

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
		float c = cos(rotation); float s = sin(rotation);

		//apply rotation transformation
		for (int n = 0; n < 4; ++n) {
			int v_x = (v[n].pos.x - origin_x) - x; int v_y = (v[n].pos.y - origin_y) - y;
			v[n].pos.x = ((c * v_x) - (s * v_y)) + x + origin_x;
			v[n].pos.y = ((s * v_x) + (c * v_y)) + y + origin_y;
		}
	}
}

void PXL_Batch::set_quad_uvs(PXL_Texture* texture, PXL_Rect* src_rect) {
	PXL_VertexPoint* v = &vertex_data[vertex_data_size];

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
	PXL_VertexPoint* v = &vertex_data[vertex_data_size];

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
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_data_size * sizeof(PXL_VertexPoint), &vertex_data[0]);

	//loops through each texture and draws the vertex data with that texture id
	int size = 0;
	int offset = 0;
	int prev_id = vertex_batches[0].texture_id;
	PXL_ShaderProgram* prev_shader = vertex_batches[0].shader;
	set_shader(prev_shader);
	for (int i = 0; i < num_added; ++i) {
		if (i >= num_added - 1) { size += vertex_batches[i].num_vertices; }
		if (vertex_batches[i].texture_id != prev_id || 
			(vertex_batches[i].shader != NULL && vertex_batches[i].shader != prev_shader) || i >= num_added - 1) {
			if (vertex_batches[i].shader != prev_shader || i >= num_added - 1) {
				set_shader(prev_shader);
			}
			glBindTexture(GL_TEXTURE_2D, prev_id);
			prev_id = vertex_batches[i].texture_id;
			prev_shader = vertex_batches[i].shader;

			//draw vertex data from vertex data in buffer
			glDrawArrays(GL_QUADS, offset, size);

			offset += size;
			size = 0;
		}
		size += vertex_batches[i].num_vertices;
	}
}

void PXL_Batch::free() {
	glDeleteBuffers(1, &vertex_buffer_id);
	vbo_created = false;
}

PXL_Batch::~PXL_Batch() {
	free();
}