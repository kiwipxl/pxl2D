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
			for (int i = 0; i < 4; ++i) {
				vertex_batches[n].vertices.push_back(PXL_VertexPoint());
			}
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

	//enable vertex attrib pointers when rendering
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void PXL_Batch::render_all(bool depth_test) {
	//enable/disable depth testing depending on depth_test input
	if (depth_test) {
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}else {
		glDisable(GL_DEPTH_TEST);
	}

	//draw vbo and clear all data
	draw_vbo();
	clear_all();
}

void PXL_Batch::clear_all() {
	//reset texture index and vertex data
	texture_ids.clear();
	num_added = 0;
	num_added = 0;
}

void PXL_Batch::set_shader(PXL_ShaderProgram* shader) {
	//use specified program id
	glUseProgram(shader->get_program_id());

	//set matrix uniform in the vertex shader for the program
	view_mat.identity();
	glUniformMatrix4fv(shader->get_matrix_loc(), 1, true, (view_mat * perspective_mat).get_mat());
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect) {
	if (verify_texture_add(texture, rect)) {
		add_vertices(texture, rect, src_rect);
		++num_added;
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, PXL_Flip flip) {
	if (verify_texture_add(texture, rect)) {
		add_vertices(texture, rect, src_rect, 0, NULL, flip);
		++num_added;
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin, PXL_Flip flip) {
	if (verify_texture_add(texture, rect)) {
		add_vertices(texture, rect, src_rect, rotation, origin, flip);
		++num_added;
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, int r, int g, int b, int a, PXL_Flip flip) {
	if (verify_texture_add(texture, rect)) {
		add_vertices(texture, rect, src_rect, 0, NULL, flip, r, g, b, a);
		++num_added;
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, int r, int g, int b, int a, 
					float rotation, PXL_Vec2* origin, PXL_Flip flip) {
	if (verify_texture_add(texture, rect)) {
		add_vertices(texture, rect, src_rect, rotation, origin, flip, r, g, b, a);
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

void PXL_Batch::add_vertices(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, 
							 float rotation, PXL_Vec2* origin, PXL_Flip flip, 
							 int r, int g, int b, int a) {
	//set the texture id for the vertex batch
	vertex_batches[num_added].texture_id = texture->get_id();

	if (num_added >= max_quads_amount) {
		PXL_show_exception("Hit max batch quad size at " + std::to_string(max_quads_amount) + " max quads.");
	}

	//set vertex pos, uvs and colours
	set_vertex_pos(num_added, texture, rect, rotation, origin, flip);
	set_vertex_uvs(num_added, texture, src_rect);
	set_vertex_colours(num_added, r, g, b, a);
}

void PXL_Batch::set_vertex_pos(int index, PXL_Texture* texture, PXL_Rect* rect, float rotation, PXL_Vec2* origin, PXL_Flip flip) {
	PXL_VertexPoint* v = &vertex_batches[index].vertices[0];

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

void PXL_Batch::set_vertex_uvs(int index, PXL_Texture* texture, PXL_Rect* src_rect) {
	PXL_VertexPoint* v = &vertex_batches[index].vertices[0];

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

void PXL_Batch::set_vertex_colours(int index, int r, int g, int b, int a) {
	PXL_VertexPoint* v = &vertex_batches[index].vertices[0];

	//set vertex colours
	if (r != 1 && g != 1 && b != 1 || a != 1) {
		for (int n = 0; n < 4; ++n) {
			v[n].colour.r = r;
			v[n].colour.g = g;
			v[n].colour.b = b;
			v[n].colour.a = a;
		}
	}
}

void PXL_Batch::set_target(PXL_FrameBuffer* f) {
	//sets the target frame buffer to be used for rendering
	target_frame_buffer = f;
}

void PXL_Batch::draw_vbo() {
	//if there are no textures to draw or no vertex data then return
	if (num_added == 0) { return; }

	//sort vertex data based on texture ids to minimise binding
	//stable sort keeps the order the same
	/*std::sort(vertex_batches.begin(), vertex_batches.begin() + num_added, 
	[](const PXL_VertexBatch& a, const PXL_VertexBatch& b) -> bool {
		return a.texture_id < b.texture_id;
	});*/

	//if a framebuffer is specified, bind to it, if not bind to the default framebuffer
	if (target_frame_buffer != NULL) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target_frame_buffer->get_id());
	}
	//binds vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

	//set vertex shader attrib pointers
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(PXL_VertexPoint), (void*)offsetof(PXL_VertexPoint, pos));
	glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(PXL_VertexPoint), (void*)offsetof(PXL_VertexPoint, uv));
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(PXL_VertexPoint), (void*)offsetof(PXL_VertexPoint, colour));

	//loops through each texture and draws the vertex data with that texture id
	int v_index = 0;
	int offset_bytes = 0;
	int prev_id = vertex_batches[0].texture_id;
	for (int i = 0; i < num_added; ++i) {
		if (vertex_batches[i].texture_id != prev_id) {
			prev_id = vertex_batches[i].texture_id;
			glBindTexture(GL_TEXTURE_2D, prev_id);

			//gets the offset and calculates the vertex data size for the texture
			int offset = offset_bytes / sizeof(PXL_VertexPoint);
			int v_size = 0;
			int size = i - offset;

			//upload sub data with offset and region size
			for (int n = 0; n < size; ++n) {
				int v_bytes = vertex_batches[v_index].vertices.size() * sizeof(PXL_VertexPoint);
				glBufferSubData(GL_ARRAY_BUFFER, offset_bytes, v_bytes, &vertex_batches[v_index].vertices[0]);
				++v_index;
				v_size += vertex_batches[v_index].vertices.size();
				offset_bytes += v_bytes;
			}

			//draw vertex data from binded buffer
			glDrawArrays(GL_QUADS, offset, v_size);
		}
	}
	//glDrawArrays(GL_QUADS, 0, num_added);

	//set back to default framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void PXL_Batch::free() {
	glDeleteBuffers(1, &vertex_buffer_id);
	vbo_created = false;
}

PXL_Batch::~PXL_Batch() {
	free();
}