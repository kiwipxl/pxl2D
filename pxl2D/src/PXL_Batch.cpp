#include "PXL_Batch.h"
#include <iostream>
#include <algorithm>
#include "PXL.h"

PXL_Batch::PXL_Batch(PXL_MaxRenders size) {
	vbo_created = false;
	create_batch(size);
}

PXL_Batch* PXL_create_batch(PXL_MaxRenders size) {
	return new PXL_Batch(size);
}

void PXL_Batch::create_batch(PXL_MaxRenders size) {
	batch_size = size * 4;

	//if the batch is already created then delete the vbo
	if (vbo_created) {
		free();
	}else {
		//free the vbo if it's already being used and create a new vbo object
		if (vbo != NULL) { vbo->free(); }
		vbo = new PXL_VBO(size);
		vbo->vertex_data.clear();
		textures.assign(size, NULL);
	}
	vbo_created = true;

	use_shader(PXL_default_shader);

	//enable vertex attrib pointers when rendering
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void PXL_Batch::start() {

}

void PXL_Batch::end() {
	//flush vbo and reset texture index and vertex data
	draw_vbo();
	texture_index = 0;
	vbo->vertex_data.clear();
}

void PXL_Batch::use_shader(GLint program_id) {
	//set perspective matrix to screen coordinates and translate to 0,0 top left
	view_mat.identity();
	perspective_mat.identity();

	perspective_mat.scale(1.0f / PXL_center_screen_x, -1.0f / PXL_center_screen_y);
	perspective_mat.translate(-1.0f, 1.0f);

	//use specified program id
	glUseProgram(program_id);

	//set matrix uniform in the specified shader
	glUniformMatrix4fv(glGetUniformLocation(3, "matrix"), 1, true, (view_mat * perspective_mat).get_mat());
}

void PXL_Batch::use_shader(PXL_ShaderProgram* program) {
	use_shader(program->get_program_id());
}

void PXL_Batch::render(PXL_Texture* texture, PXL_Rect* src_rect, PXL_Rect* rect) {
	if (texture->texture_created) {
		if (rect->x + rect->w < 0 || rect->y + rect->h < 0 || 
			rect->x > PXL_screen_width || rect->y > PXL_screen_height) {
			return;
		}
		if (vbo->vertex_data.size() >= batch_size) {
			throw exception("hit max batch render size");
		}

		set_vbo(vbo->vertex_data.size(), texture, rect, src_rect);

		textures[texture_index] = texture; ++texture_index;
	}
}

void PXL_Batch::render_transformed(PXL_Texture* texture, PXL_Rect* src_rect, PXL_Rect* rect,
								float angle, PXL_Vec2* origin, PXL_Flip flip) {
	if (texture->texture_created) {
		if (rect->x + rect->w < 0 || rect->y + rect->h < 0 ||
			rect->x > PXL_screen_width || rect->y > PXL_screen_height) {
			return;
		}
		if (vbo->vertex_data.size() >= batch_size) {
			throw exception("hit max batch render size");
		}

		float scale_x = rect->w / texture->get_width(); float scale_y = rect->h / texture->get_height();
		switch (flip) {
			case PXL_FLIP_NONE:
				break;
			case PXL_FLIP_HORIZONTAL:
				break;
			case PXL_FLIP_VERTICAL:
				break;
		}

		set_vbo(vbo->vertex_data.size(), texture, rect, src_rect);

		PXL_VertexPoint* v = &vbo->vertex_data[0];

		if (angle != 0) {
			angle = angle / (180.0f / 3.14f);
			float c = cos(angle); float s = sin(angle);
			int index = vbo->vertex_data.size() - 4;

			for (int n = index; n < index + 4; ++n) {
				int x = v[n].pos.x - origin->x - rect->x; int y = v[n].pos.y - origin->y - rect->y;
				v[n].pos.x = ((c * x) - (s * y)) + rect->x + origin->x;
				v[n].pos.y = ((s * x) + (c * y)) + rect->y + origin->y;
			}
		}

		textures[texture_index] = texture; ++texture_index;
	}
}

bool PXL_Batch::set_vbo(int index, PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect) {
	for (int n = 0; n < 4; ++n) {
		vbo->vertex_data.push_back(PXL_VertexPoint());
	}

	if (index >= vbo->vertex_data.size()) {
		throw exception("index argument is out of bounds from vertex data");
	}

	PXL_VertexPoint* v = &vbo->vertex_data[0];

	unsigned short uv_x = 0; unsigned short uv_y = 0; unsigned short uv_w = USHRT_MAX; unsigned short uv_h = USHRT_MAX;
	if (src_rect != NULL) {
		uv_x = (src_rect->x / texture->get_width()) * USHRT_MAX; uv_y = (src_rect->y / texture->get_height()) * USHRT_MAX;
		uv_w = (src_rect->w / texture->get_width()) * USHRT_MAX; uv_h = (src_rect->h / texture->get_height()) * USHRT_MAX;
	}

	if (src_rect == NULL) {
		v[index].uv.x = uv_x;											v[index].uv.y = uv_y;
		v[index + 1].uv.x = uv_x + uv_w;								v[index + 1].uv.y = uv_y;
		v[index + 2].uv.x = uv_x + uv_w;								v[index + 2].uv.y = uv_y + uv_h;
		v[index + 3].uv.x = uv_x;										v[index + 3].uv.y = uv_y + uv_h;
	}

	float scale_x = rect->w / texture->get_width(); float scale_y = rect->h / texture->get_height();

	v[index].pos.x = rect->x;											v[index].pos.y = rect->y;
	v[index + 1].pos.x = rect->x + (texture->get_width() * scale_x);	v[index + 1].pos.y = rect->y;
	v[index + 2].pos.x = rect->x + (texture->get_width() * scale_x);	v[index + 2].pos.y = rect->y + (texture->get_height() * scale_y);
	v[index + 3].pos.x = rect->x;										v[index + 3].pos.y = rect->y + (texture->get_height() * scale_y);

	return true;
}

void PXL_Batch::draw_vbo() {
	if (texture_index == 0 || vbo->vertex_data.size() == 0) { return; }

	PXL_Texture* texture = textures[0];
	if (texture == NULL) { return; }

	glBindTexture(GL_TEXTURE_2D, texture->get_id());

	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo->vertex_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vbo->vertex_data.size() * sizeof(PXL_VertexPoint), &vbo->vertex_data[0]);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(PXL_VertexPoint), (void*)offsetof(PXL_VertexPoint, pos));
		glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(PXL_VertexPoint), (void*)offsetof(PXL_VertexPoint, uv));
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(PXL_VertexPoint), (void*)offsetof(PXL_VertexPoint, colour));

		//glBindBuffer(GL_ARRAY_BUFFER, vbo->index_id);
		//glDrawElements(GL_QUADS, vbo->vertex_data.size(), GL_UNSIGNED_BYTE, vbo->index_data);

		glDrawArrays(GL_QUADS, 0, vbo->vertex_data.size());

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}
}

void PXL_Batch::set_colour(float r, float g, float b, float a) {
	if (vbo->vertex_data.size() == 0) { return; }

	PXL_VertexPoint* v = &vbo->vertex_data[0];

	for (int n = 0; n < vbo->vertex_data.size(); ++n) {
		v[n].colour.r = r;
		v[n].colour.g = g;
		v[n].colour.b = b;
		v[n].colour.a = a;
	}
}

void PXL_Batch::set_filters(PXL_TextureFilter min_filter, PXL_TextureFilter max_filter) {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, max_filter);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void PXL_Batch::free() {
	delete vbo;
	vbo = NULL;
	vbo_created = false;
}

PXL_Batch::~PXL_Batch() {
	free();
}