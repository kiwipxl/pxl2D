#include "PXL_Renderer.h"
#include "PXL_ShaderUtils.h"

void PXL_initiate(int screen_width, int screen_height) {
	s_width = screen_width;
	s_height = screen_height;

	PXL_load_glsl_shader("assets/bloom.glsl");

	PXL_perspective_mat.scale(1.0f / (screen_width / 2), -1.0f / (screen_height / 2));
	PXL_perspective_mat.translate(-1.0f, 1.0f);
}

void PXL_render(PXL_Texture* texture, PXL_Rect* src_rect, PXL_Rect* rect, bool c_set_buffer) {
	if (texture->created) {
		if (rect->x + rect->w < 0 || rect->y + rect->h < 0 || 
			rect->x > s_width || rect->y > s_height) {
			return;
		}

		PXL_model_view_mat.identity();

		PXL_model_view_mat.translate(rect->x, rect->y);
		PXL_model_view_mat.scale(rect->w / texture->width, rect->h / texture->height);

		bool upload_buffer = false;
		if (c_set_buffer) { upload_buffer = PXL_set_buffer(texture, src_rect); }
		PXL_draw_buffer(texture, upload_buffer);

		++PXL_render_calls;
	}
}

void PXL_render_transform(PXL_Texture* texture, PXL_Rect* src_rect, PXL_Rect* rect,
								float angle, PXL_Vec2* origin, PXL_Flip flip, bool c_set_buffer) {
	if (texture->created) {
		if (rect->x + rect->w < 0 || rect->y + rect->h < 0 ||
			rect->x > s_width || rect->y > s_height) {
			return;
		}

		PXL_model_view_mat.identity();

		float scale_x = rect->w / texture->width; float scale_y = rect->h / texture->height;
		switch (flip) {
			case PXL_FLIP_NONE:
				PXL_model_view_mat.scale(scale_x, scale_y);
				break;
			case PXL_FLIP_HORIZONTAL:
				PXL_model_view_mat.translate(origin->x * 2, 0);
				PXL_model_view_mat.scale(-scale_x, scale_y);
				break;
			case PXL_FLIP_VERTICAL:
				PXL_model_view_mat.translate(0, origin->y * 2);
				PXL_model_view_mat.scale(scale_x, -scale_y);
				break;
		}

		origin->x /= scale_x;
		origin->y /= scale_y;

		PXL_model_view_mat.translate(-origin->x, -origin->y);
		PXL_model_view_mat.rotate_z(angle);
		PXL_model_view_mat.translate(rect->x + origin->x, rect->y + origin->y);

		bool upload_buffer = false;
		if (c_set_buffer) { upload_buffer = PXL_set_buffer(texture, src_rect); }
		PXL_draw_buffer(texture, upload_buffer);

		++PXL_transform_render_calls;
	}
}

bool PXL_set_buffer(PXL_Texture* texture, PXL_Rect* src_rect) {
	PXL_VertexPoint* v = texture->buffer_object->vertex_data;

	PXL_Rect* last_s_r = &texture->last_src_rect;

	float uv_x = 0; float uv_y = 0; float uv_w = 1; float uv_h = 1;
	if (src_rect != NULL) {
		uv_x = src_rect->x / texture->width; uv_y = src_rect->y / texture->height;
		uv_w = src_rect->w / texture->width; uv_h = src_rect->h / texture->height;
	}

	bool has_changed = false;
	if (src_rect == NULL || 
		last_s_r->x != src_rect->x || last_s_r->y != src_rect->y ||
		last_s_r->w != src_rect->w || last_s_r->h != src_rect->h) {
		v[0].uv.x = uv_x;						v[0].uv.y = uv_y;
		v[1].uv.x = uv_x + uv_w;				v[1].uv.y = uv_y;
		v[2].uv.x = uv_x + uv_w;				v[2].uv.y = uv_y + uv_h;
		v[3].uv.x = uv_x;						v[3].uv.y = uv_y + uv_h;

		has_changed = true;

		if (src_rect != NULL) { texture->last_src_rect = *src_rect; }
	}
	
	if (v[1].pos.x != texture->width || v[2].pos.x != texture->width || 
		v[2].pos.y != texture->height || v[3].pos.y != texture->height) {
		v[0].pos.x = 0;							v[0].pos.y = 0;
		v[1].pos.x = texture->width;			v[1].pos.y = 0;
		v[2].pos.x = texture->width;			v[2].pos.y = texture->height;
		v[3].pos.x = 0;							v[3].pos.y = texture->height;

		has_changed = true;
	}

	if (v[0].colour.x != texture->last_colour.x ||
		v[0].colour.y != texture->last_colour.y ||
		v[0].colour.z != texture->last_colour.z ||
		v[0].colour.w != texture->last_colour.w) {
		has_changed = true;
		texture->last_colour.x = v[0].colour.x;
		texture->last_colour.y = v[0].colour.y;
		texture->last_colour.z = v[0].colour.z;
		texture->last_colour.w = v[0].colour.w;
	}

	return has_changed;
}

void PXL_draw_buffer(PXL_Texture* texture, bool upload_buffer) {
	glUseProgram(3);

	glUniformMatrix4fv(glGetUniformLocation(3, "matrix"), 1, true, (PXL_model_view_mat * PXL_view_mat * PXL_perspective_mat).get_mat());

	glBindTexture(GL_TEXTURE_2D, texture->id);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	{
		glBindBuffer(GL_ARRAY_BUFFER, texture->buffer_object->vertex_id);
		if (upload_buffer) {
			glBufferSubData(GL_ARRAY_BUFFER, 0, texture->buffer_object->buffer_size * sizeof(PXL_VertexPoint), texture->buffer_object->vertex_data);
			++PXL_vertices_uploaded;
		}

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, sizeof(PXL_VertexPoint), 0);								//pos
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(PXL_VertexPoint), (void*)sizeof(PXL_Vec2));		//uv
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(PXL_VertexPoint), (void*)sizeof(PXL_Vec4));		//colour

		glBindBuffer(GL_ARRAY_BUFFER, texture->buffer_object->index_id);
		glDrawElements(GL_QUADS, texture->buffer_object->buffer_size, GL_UNSIGNED_INT, texture->buffer_object->index_data);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	++PXL_total_render_calls;

	glUseProgram(0);
}