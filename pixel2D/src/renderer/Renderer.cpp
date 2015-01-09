#include "Renderer.h"
#include <iostream>
#include "../managers/WindowManager.h"
#include "shader/ShaderManager.h"

class Universe {

	public:
		static WindowManager* win_manager;
		static ShaderManager* shader;
};

void Renderer::initiate() {
	universe->shader->load_glsl_shader("assets/repeat.glsl");

	perspective_mat.set_scale(1.0f / universe->win_manager->center_x, -1.0f / universe->win_manager->center_y);
	perspective_mat.set_position(-1.0f, 1.0f);
}

void Renderer::render(Texture* texture, SDL_Rect* src_rect, SDL_Rect* rect, bool c_set_buffer) {
	if (texture->created) {
		if (rect->x + rect->w < 0 || rect->y + rect->h < 0 || 
			rect->x > universe->win_manager->screen_width || rect->y > universe->win_manager->screen_height) {
			return;
		}

		model_view_mat.identity();

		model_view_mat.translate(rect->x, rect->y);
		model_view_mat.scale(rect->w / texture->width, rect->h / texture->height);

		bool upload_buffer = false;
		if (c_set_buffer) { upload_buffer = set_buffer(texture, src_rect); }
		draw_buffer(texture, upload_buffer);

		++render_calls;
	}
}

void Renderer::render_transform(Texture* texture, SDL_Rect* src_rect, SDL_Rect* rect,
								float angle, SDL_Point* origin, SDL_RendererFlip flip, bool c_set_buffer) {
	if (texture->created) {
		if (rect->x + rect->w < 0 || rect->y + rect->h < 0 ||
			rect->x > universe->win_manager->screen_width || rect->y > universe->win_manager->screen_height) {
			return;
		}

		model_view_mat.identity();

		model_view_mat.translate(-origin->x, -origin->y);
		model_view_mat.rotate_z(angle);
		model_view_mat.translate(rect->x + origin->x, rect->y + origin->y);
		switch (flip) {
			case SDL_FLIP_NONE:
				model_view_mat.scale(-.5f, -.5f);
				//model_view_mat.scale((-rect->w / texture->width) - 1, (-rect->h / texture->height) - 1);
				break;
			case SDL_FLIP_HORIZONTAL:
				model_view_mat.translate(origin->x * 2, 0);
				model_view_mat.scale((-rect->w / texture->width) - 1, (rect->h / texture->height) - 1);
				break;
			case SDL_FLIP_VERTICAL:
				model_view_mat.translate(0, origin->y * 2);
				model_view_mat.scale((rect->w / texture->width) - 1, (-rect->h / texture->height) - 1);
				break;
		}

		bool upload_buffer = false;
		if (c_set_buffer) { upload_buffer = set_buffer(texture, src_rect); }
		draw_buffer(texture, upload_buffer);

		++transform_render_calls;
	}
}

bool Renderer::set_buffer(Texture* texture, SDL_Rect* src_rect) {
	VertexPoint* v = texture->buffer_object->vertex_data;

	SDL_Rect* last_s_r = &texture->last_src_rect;

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
		v[1].pos.x = texture->width;		v[1].pos.y = 0;
		v[2].pos.x = texture->width;		v[2].pos.y = texture->height;
		v[3].pos.x = 0;							v[3].pos.y = texture->height;

		has_changed = true;
	}

	if (v[0].colour.r != texture->last_colour.r ||
		v[0].colour.g != texture->last_colour.g ||
		v[0].colour.b != texture->last_colour.b ||
		v[0].colour.a != texture->last_colour.a) {
		has_changed = true;
		texture->last_colour.r = v[0].colour.r;
		texture->last_colour.g = v[0].colour.g;
		texture->last_colour.b = v[0].colour.b;
		texture->last_colour.a = v[0].colour.a;
	}

	return has_changed;
}

void Renderer::draw_buffer(Texture* texture, bool upload_buffer) {
	glUseProgram(3);

	glUniformMatrix4fv(glGetUniformLocation(3, "matrix"), 1, true, (model_view_mat * view_mat * perspective_mat).get_mat());

	glBindTexture(GL_TEXTURE_2D, texture->id);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	{
		glBindBuffer(GL_ARRAY_BUFFER, texture->buffer_object->vertex_id);
		if (upload_buffer) {
			glBufferSubData(GL_ARRAY_BUFFER, 0, texture->buffer_object->buffer_size * sizeof(VertexPoint), texture->buffer_object->vertex_data);
			++vertices_uploaded;
		}

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, sizeof(VertexPoint), 0);							//pos
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(VertexPoint), (void*)sizeof(Point2DF));	//uv
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(VertexPoint), (void*)sizeof(RGBA));		//colour

		glBindBuffer(GL_ARRAY_BUFFER, texture->buffer_object->index_id);
		glDrawElements(GL_QUADS, texture->buffer_object->buffer_size, GL_UNSIGNED_INT, texture->buffer_object->index_data);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	++total_render_calls;

	glUseProgram(0);
}