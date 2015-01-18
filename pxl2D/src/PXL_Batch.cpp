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
	max_renders = size * 4;

	//if the batch is already created then delete the vbo
	if (vbo_created) { free(); }

	{
		//create the vbo
		glGenBuffers(1, &vertex_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(PXL_VertexPoint), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);

		vertex_data.clear();
		vbo_created = true;
	}

	set_shader(PXL_default_shader);

	//set perspective matrix to screen coordinates and translate to 0,0 top left
	view_mat.identity();
	perspective_mat.identity();

	perspective_mat.scale(1.0f / PXL_center_screen_x, -1.0f / PXL_center_screen_y);
	perspective_mat.translate(-1.0f, 1.0f);

	//enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//enable vertex attrib pointers when rendering
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void PXL_Batch::render_all() {
	//draw vbo and clear all data
	draw_vbo();
	clear_all();
}

void PXL_Batch::clear_all() {
	//reset texture index and vertex data
	texture_ids.clear();
	vertex_data.clear();
}

void PXL_Batch::set_shader(GLint shader_program_id) {
	//use specified program id
	glUseProgram(shader_program_id);

	//set matrix uniform in the vertex shader for the program
	glUniformMatrix4fv(glGetUniformLocation(3, "matrix"), 1, true, (view_mat * perspective_mat).get_mat());
}

void PXL_Batch::set_shader(PXL_ShaderProgram* shader) {
	set_shader(shader->get_program_id());
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect) {
	if (verify_texture_add(texture, rect)) {
		add_texture(texture->get_id());
		add_vertices(texture, rect, src_rect);
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, PXL_Flip flip) {
	if (verify_texture_add(texture, rect)) {
		add_texture(texture->get_id());
		add_vertices(texture, rect, src_rect);
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin, PXL_Flip flip) {
	if (verify_texture_add(texture, rect)) {
		add_texture(texture->get_id());
		add_vertices(texture, rect, src_rect, rotation, origin, flip);
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, int r, int g, int b, int a, PXL_Flip flip) {
	if (verify_texture_add(texture, rect)) {
		add_texture(texture->get_id());
		add_vertices(texture, rect, src_rect, 0, NULL, flip, r, g, b, a);
	}
}

void PXL_Batch::add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, int r, int g, int b, int a, 
					float rotation, PXL_Vec2* origin, PXL_Flip flip) {
	if (verify_texture_add(texture, rect)) {
		add_texture(texture->get_id());
		add_vertices(texture, rect, src_rect, rotation, origin, flip, r, g, b, a);
	}
}

void PXL_Batch::add_texture(int texture_id) {
	//if texture id is not already in texture ids vector then add it
	if (find(texture_ids.begin(), texture_ids.end(), texture_id) == texture_ids.end()) {
		texture_ids.push_back(texture_id);
	}
}

bool PXL_Batch::verify_texture_add(PXL_Texture* texture, PXL_Rect* rect) {
	if (texture->texture_created) {
		if (rect->x + rect->w > 0 && rect->y + rect->h > 0 && rect->x < PXL_screen_width && rect->y < PXL_screen_height) {
			if (vertex_data.size() >= max_renders) {
				throw exception("hit max batch render size");
			}

			return true;
		}
	}
	return false;
}

void PXL_Batch::add_vertices(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, 
							 float rotation, PXL_Vec2* origin, PXL_Flip flip, 
							 int r, int g, int b, int a) {
	//creates 4 new vertex points and adds them to the vertex data
	int index = vertex_data.size();

	for (int n = 0; n < 4; ++n) {
		vertex_data.push_back(PXL_VertexPoint());
		//set the texture id for the vertex
		vertex_data[index + n].texture_id = texture->get_id();
	}

	if (index >= vertex_data.size()) {
		throw exception("index argument is out of bounds from vertex data");
	}

	//set vertex pos, uvs and colours
	set_vertex_pos(index, texture, rect, rotation, origin, flip);
	set_vertex_uvs(index, texture, src_rect);
	set_vertex_colours(index, r, g, b, a);
}

void PXL_Batch::set_vertex_pos(int index, PXL_Texture* texture, PXL_Rect* rect, float rotation, PXL_Vec2* origin, PXL_Flip flip) {
	PXL_VertexPoint* v = &vertex_data[index];

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
		rotation = rotation / (180.0f / 3.14f);
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
	PXL_VertexPoint* v = &vertex_data[index];

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
	PXL_VertexPoint* v = &vertex_data[index];

	//set vertex colours
	for (int n = 0; n < 4; ++n) {
		v[n].colour.r = r;
		v[n].colour.g = g;
		v[n].colour.b = b;
		v[n].colour.a = a;
	}
}

void PXL_Batch::draw_vbo() {
	//if there are no textures to draw or no vertex data then return
	if (texture_ids.size() == 0 || vertex_data.size() == 0) { return; }

	//sort vertex data based on texture ids to minimise binding
	//stable sort keeps the order the same
	stable_sort(vertex_data.begin(), vertex_data.end(), 
	[](const PXL_VertexPoint& a, const PXL_VertexPoint& b) -> bool {
		return a.texture_id < b.texture_id;
	});

	//sort texture ids to lowest to highest
	sort(texture_ids.begin(), texture_ids.end(), [](int a, int b) -> bool { return a < b; });

	//searches through the vertex data to find the offsets of each texture id
	texture_offsets.clear();
	int prev_id = INT_MAX;
	for (int n = 0; n < vertex_data.size(); ++n) {
		if (vertex_data[n].texture_id != prev_id) {
			texture_offsets.push_back(n * sizeof(PXL_VertexPoint));
			prev_id = vertex_data[n].texture_id;
		}
	}

	//binds vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertex_id);

	//set vertex shader attrib pointers
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(PXL_VertexPoint), (void*)offsetof(PXL_VertexPoint, pos));
	glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(PXL_VertexPoint), (void*)offsetof(PXL_VertexPoint, uv));
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(PXL_VertexPoint), (void*)offsetof(PXL_VertexPoint, colour));

	//loops through each texture and draws the vertex data with that texture id
	for (int i = 0; i < texture_ids.size(); ++i) {
		glBindTexture(GL_TEXTURE_2D, texture_ids[i]);

		//gets the offset and calculates the vertex data size for the texture
		int offset = texture_offsets[i];
		int size;
		if (i < texture_offsets.size() - 1) {
			size = texture_offsets[i + 1] - texture_offsets[i];
		}else {
			size = (vertex_data.size() * sizeof(PXL_VertexPoint)) - offset;
		}

		//upload sub data with offset and region size
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, &vertex_data[offset / sizeof(PXL_VertexPoint)]);

		//draw vertex data from binded buffer
		glDrawArrays(GL_QUADS, offset / sizeof(PXL_VertexPoint), size / sizeof(PXL_VertexPoint));
	}
}

void PXL_Batch::free() {
	glDeleteBuffers(1, &vertex_id);
	vbo_created = false;
}

PXL_Batch::~PXL_Batch() {
	free();
}