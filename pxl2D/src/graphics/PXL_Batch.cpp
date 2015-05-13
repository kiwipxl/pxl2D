#include "graphics/PXL_Batch.h"
#include <algorithm>
#include "system/PXL_Window.h"
#include "system/PXL_Exception.h"
#include "system/PXL_Debug.h"

PXL_Batch::PXL_Batch(PXL_Window* window) {
	batch_created = false;
	create_batch(window);
}

void PXL_Batch::create_batch(PXL_Window* window) {
	free();

	{
		//create the vbo
        glGenBuffers(1, &vbo_id);

		batch_created = true;
	}

	clear_all();

	if (window != NULL) {
		set_window_target(window);
		render_bounds.x = 0;					render_bounds.y = 0;
		render_bounds.w = window->get_width();	render_bounds.h = window->get_height();
	}else {
		render_bounds.x = 0;					render_bounds.y = 0;
		render_bounds.w = 1024;					render_bounds.h = 768;
	}

	//set perspective matrix to window coordinates and translate to 0,0 top left
	view_mat.identity();
	perspective_mat.identity();

	perspective_mat.scale(1.0f / (render_bounds.w / 2), -1.0f / (render_bounds.h / 2));
    perspective_mat.translate(-1.0f, 1.0f);

    //view_mat = view_mat + 8 * 2 - 4 + perspective_mat;
    //view_mat -= 4;

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    //flat identifiers in shaders (non interpolated) use the first inputted vertex to pass to the fragment shader
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
}

void PXL_Batch::use_shader(PXL_ShaderProgram* shader) {
	if (shader == NULL) { shader = PXL_default_shader; }

	if (current_shader != shader) {
		current_shader = shader;

		//use specified program id
		glUseProgram(current_shader->get_program_id());

		//set matrix uniform in the vertex shader for the program
        glUniformMatrix4fv(current_shader->get_matrix_loc(), 1, false, proj_view_mat.get_raw_matrix());
	}
}

void PXL_Batch::use_blend_mode(PXL_BlendMode blend_mode) {
	if (current_blend_mode != blend_mode) {
        current_blend_mode = blend_mode;
		if (current_blend_mode == PXL_BLEND) {
            glEnable(GL_BLEND);
            glDepthMask(GL_FALSE);
            glDepthFunc(GL_LESS);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}else if (current_blend_mode == PXL_NO_BLEND) {
            glDisable(GL_BLEND);
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LESS);
		}
	}
}

void PXL_Batch::add(const PXL_Texture& texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin,
	PXL_Flip flip, int z_depth, PXL_Colour colour, PXL_ShaderProgram* shader, PXL_BlendMode blend_mode) {
	if (verify_texture_add(texture, rect)) {
        if (total_vertices >= vertices.size()) {
            int prev_size = vertices.size();

            vertices.resize(prev_size + PXL_CONFIG_INC_BATCH_RESIZE);

            PXL_VertexBatch* last_batch;
            for (int n = 0; n < PXL_CONFIG_INC_BATCH_RESIZE; ++n) {
                if (n % 4 == 0) last_batch = new PXL_VertexBatch();
                vertices[n + prev_size].batch = last_batch;
                last_batch->num_vertices = 4;
            }
        }

        /*z_depth = 0;
		VertexContainer& c = vertices[z_depth];

		if (c.batch_index >= c.batches.size()) {
			c.batches.push_back(PXL_VertexBatch());
		}
		PXL_VertexBatch& v_batch = c.batches[c.batch_index];*/

		/*v_batch.num_vertices = 4;
		v_batch.num_indices = 6;
		v_batch.texture_id = texture.get_id();
		v_batch.shader = shader;
		v_batch.z_depth = z_depth;
		v_batch.blend_mode = blend_mode;

		min_vertex_index = PXL_min(min_vertex_index, (PXL_uint)z_depth);
		max_vertex_index = PXL_max(max_vertex_index, (PXL_uint)z_depth);

		if (c.data_index >= c.data.size()) {
			c.data.push_back(PXL_VertexPoint());
			c.data.push_back(PXL_VertexPoint());
			c.data.push_back(PXL_VertexPoint());
			c.data.push_back(PXL_VertexPoint());
		}
		PXL_VertexPoint* v = &c.data[c.data_index];

		if (c.indices_index >= c.indices.size()) {
			for (int n = 0; n < 6; ++n) {
				c.indices.push_back(0);
			}
		}
		int i = c.indices_count;
		c.indices[c.indices_index] = i;			c.indices[c.indices_index + 1] = i + 1;		c.indices[c.indices_index + 2] = i + 2;
		c.indices[c.indices_index + 3] = i;		c.indices[c.indices_index + 4] = i + 3;		c.indices[c.indices_index + 5] = i + 2;
		c.indices_index += 6;
		c.indices_count += 4;

		total_vertices += 4;
		++c.batch_index;
		c.data_index += 4;*/

        PXL_VertexPoint* v = &vertices[total_vertices];
        PXL_VertexBatch& batch = *vertices[total_vertices].batch;
        batch.num_vertices = 4;
        batch.num_indices = 6;
        batch.texture_id = texture.get_id();
        batch.shader = shader;
        batch.z_depth = z_depth;
        batch.blend_mode = blend_mode;
        if (texture.has_transparency) {
            batch.uses_transparency = true;
            batch.blend_mode = PXL_BLEND;
            batch.add_id = num_added;
        }else {
            batch.uses_transparency = false;
            batch.blend_mode = PXL_NO_BLEND;
            batch.add_id = num_added;
        }

        total_vertices += 4;
        ++num_added;

		/**
		==================================================================================
		                            Set vertex positions
		==================================================================================
		**/
		//set vertex pos, uvs and colours
		//set origin
		float origin_x = 0; float origin_y = 0;
		if (origin != NULL) { origin_x = origin->x; origin_y = origin->y; }

		/*bool modified = false;
		if (v_batch.rect.x != rect->x || v_batch.rect.y != rect->y ||
			v_batch.rect.w != rect->w || v_batch.rect.h != rect->h) {
			modified = true;

			memcpy(&v_batch.rect, rect, sizeof(PXL_Rect));
		}
		if (v_batch.rotation != rotation || v_batch.flip != flip ||
			v_batch.origin.x != origin_x || v_batch.origin.y != origin_y) {
			modified = true;

			v_batch.rotation = rotation;
			v_batch.flip = flip;
			v_batch.origin.x = origin_x; v_batch.origin.y = origin_y;
		}*/
		//if (modified) {
			//get positions from rect
			int x = rect->x; int y = rect->y;

			//set scale
			float scale_x = rect->w / texture.get_width(); float scale_y = rect->h / texture.get_height();
			if (flip == PXL_FLIP_HORIZONTAL) {
				scale_x = -scale_x;
				x += rect->w;
				origin_x -= rect->w;
			}else if (flip == PXL_FLIP_VERTICAL) {
				scale_y = -scale_y;
				y += rect->h;
				origin_y -= rect->h;
			}
			int scaled_width = texture.get_width() * scale_x;
			int scaled_height = texture.get_height() * scale_y;

			//apply rotation
			if (rotation != 0) {
				//set rotation to degrees rather than radians
				rotation = rotation / PXL_radian;
				float c = PXL_fast_cos(rotation); float s = PXL_fast_sin(rotation);

				x += origin_x; y += origin_y;
				scaled_width -= origin_x; scaled_height -= origin_y;

				//set vertex position including scale and rotation
                v[0].pos.x = x + ((c * -origin_x) - (s * -origin_y));
                v[0].pos.y = y + ((s * -origin_x) + (c * -origin_y));

                v[1].pos.x = x + ((c * scaled_width) - (s * -origin_y));
                v[1].pos.y = y + ((s * scaled_width) + (c * -origin_y));

                v[2].pos.x = x + ((c * scaled_width) - (s * scaled_height));
                v[2].pos.y = y + ((s * scaled_width) + (c * scaled_height));

                v[3].pos.x = x + ((c * -origin_x) - (s * scaled_height));
                v[3].pos.y = y + ((s * -origin_x) + (c * scaled_height));

				//v_batch.rotation = rotation;
			}else {
				//set vertex position including scale
                v[0].pos.x = x;											v[0].pos.y = y;
                v[1].pos.x = x + scaled_width;							v[1].pos.y = y;
                v[2].pos.x = x + scaled_width;							v[2].pos.y = y + scaled_height;
                v[3].pos.x = x;											v[3].pos.y = y + scaled_height;
			}
            unsigned int c = 10000000;
            unsigned int t = (500) * (c / 1000.0f);
            float depth = (float(-num_added + t) / c);
            v[0].pos.z = depth;
            v[1].pos.z = depth;
            v[2].pos.z = depth;
            v[3].pos.z = depth;
		//}

		/**
		==================================================================================
		                            Set UV vertex coords
		==================================================================================
		**/
		//attempt to optimise by not setting uv values if they have the same value in the vertex batch as the new values
		/*modified = false;
		if (src_rect == NULL) {
			if (v_batch.src_rect.x != 0 || v_batch.src_rect.y != 0 ||
				v_batch.src_rect.w != texture.get_width() || v_batch.src_rect.h != texture.get_height()) {
				modified = true;

				v_batch.src_rect.x = 0; v_batch.src_rect.y = 0;
				v_batch.src_rect.w = texture.get_width(); v_batch.src_rect.h = texture.get_height();
			}
		}else {
			if (v_batch.src_rect.x != src_rect->x || v_batch.src_rect.y != src_rect->y ||
				v_batch.src_rect.w != src_rect->w || v_batch.src_rect.h != src_rect->h) {
				modified = true;

				memcpy(&v_batch.src_rect, src_rect, sizeof(PXL_Rect));
			}
		}*/

		//if (modified) {
			//default un-normalised uv coords
			PXL_ushort uv_x = 0; PXL_ushort uv_y = 0; PXL_ushort uv_w = PXL_USHRT_MAX; PXL_ushort uv_h = PXL_USHRT_MAX;
			if (src_rect != NULL) {
				//calculate uv x, y, w, h by the src rect
				uv_x = (src_rect->x / texture.get_width()) * PXL_USHRT_MAX; uv_y = (src_rect->y / texture.get_height()) * PXL_USHRT_MAX;
				uv_w = (src_rect->w / texture.get_width()) * PXL_USHRT_MAX; uv_h = (src_rect->h / texture.get_height()) * PXL_USHRT_MAX;
			}

			//set uv coordinates
            v[0].uv.x = uv_x;										v[0].uv.y = uv_y;
            v[1].uv.x = uv_x + uv_w;								v[1].uv.y = uv_y;
            v[2].uv.x = uv_x + uv_w;								v[2].uv.y = uv_y + uv_h;
            v[3].uv.x = uv_x;										v[3].uv.y = uv_y + uv_h;
		//}

		/**
		==================================================================================
		                            Set vertex colours
		==================================================================================
		**/
		int i_r = colour.r * 255; int i_g = colour.g * 255; int i_b = colour.b * 255; int i_a = colour.a * 255;

		//if (v_batch.colour.r != i_r || v_batch.colour.g != i_g || v_batch.colour.b != i_b || v_batch.colour.a != i_a) {
			//set vertex colours
			for (int n = 0; n < 4; ++n) {
                v[n].colour.r = i_r;
                v[n].colour.g = i_g;
                v[n].colour.b = i_b;
                v[n].colour.a = i_a;
			}
			/*v_batch.colour.r = i_r;
			v_batch.colour.g = i_g;
			v_batch.colour.b = i_b;
			v_batch.colour.a = i_a;*/
		//}
	}
}

bool PXL_Batch::verify_texture_add(const PXL_Texture& texture, PXL_Rect* rect) {
	return true;
	if (texture.texture_created) {
		if (rect->x + rect->w > render_bounds.x && rect->y + rect->h > render_bounds.y && rect->x < render_bounds.w && rect->y < render_bounds.h) {
			return true;
		}
	}
	return false;
}

void PXL_Batch::set_render_target(PXL_FrameBuffer* f) {
	//sets the target frame buffer to be used for rendering
	target_frame_buffer = f;
}

void PXL_Batch::set_window_target(PXL_Window* window) {
	target_window = window;
}


void PXL_Batch::clear_all() {
    total_vertices = 0;
    num_added = 0;
    min_vertex_index = UINT_MAX;
    max_vertex_index = 0;
    total_vertices = 0;
    min_vertices_count = 0;
}

void PXL_Batch::render_all() {
    //if there are no textures to draw or no vertex data then return
    if (num_added != 0) {
        //if a framebuffer is specified, bind to it, if not bind to the default framebuffer
        if (target_frame_buffer != NULL) {
            target_frame_buffer->bind(PXL_GL_FRAMEBUFFER_WRITE);
        }else {
            glBindFramebuffer(PXL_GL_FRAMEBUFFER_WRITE, 0);
        }

        proj_view_mat = (perspective_mat * view_mat).transpose();

        //clear depth buffer
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_ALWAYS);
        glClearDepthf(1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        draw_vbo();

        glBindFramebuffer(PXL_GL_FRAMEBUFFER_WRITE, 0);
    }
    clear_all();
}

void PXL_Batch::draw_vbo() {
    int a = 5;

    std::sort(vertices.begin(), vertices.begin() + total_vertices, 
        [](PXL_VertexPoint& a, PXL_VertexPoint& b) {
            if (a.batch->z_depth < b.batch->z_depth) return true;
            if (b.batch->z_depth < a.batch->z_depth) return false;

            if (a.batch->uses_transparency < b.batch->uses_transparency) return true;
            if (b.batch->uses_transparency < a.batch->uses_transparency) return false;

            if (a.batch->add_id < b.batch->add_id) return true;
            if (b.batch->add_id < a.batch->add_id) return false;

            return false;
        }
    );

    //loops through each texture and draws the vertex data with that texture id
    PXL_VertexBatch* v = vertices[0].batch;
    int vertex_offset = 0;
    int indices_offset = 0;
    int vertex_index = 0;
    int num_vertices = 0;
    int num_indices = 0;
    bool changed = false;

    int prev_z_depth = v->z_depth;
    float z_depth_offset = 0;
    int num_adds_per_depth = 0;

    for (int n = 0; n < num_added; ++n) {
        v = vertices[vertex_index].batch;

        if (v->z_depth != prev_z_depth) {
            prev_z_depth = v->z_depth;
            z_depth_offset = float(n) / 10000000.0f;
        }
        
        vertices[vertex_index].pos.z -= z_depth_offset;

        vertex_index += v->num_vertices;
        ++num_adds_per_depth;
    }
    v = vertices[0].batch;
    vertex_index = 0;

    //binds vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

    //enable vertex attrib pointers when rendering
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //set vertex shader attrib pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PXL_VertexPoint), (void*)0);
    glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(PXL_VertexPoint), (void*)12);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(PXL_VertexPoint), (void*)16);

    glBufferData(GL_ARRAY_BUFFER, total_vertices * sizeof(PXL_VertexPoint), &vertices[0], GL_DYNAMIC_DRAW);

	GLuint prev_id = v->texture_id;
	//glBindTexture(GL_TEXTURE_2D, prev_id);
	PXL_BlendMode prev_blend_mode = v->blend_mode;
	//use_blend_mode(prev_blend_mode);
    PXL_ShaderProgram* prev_shader = v->shader;
    //use_shader(prev_shader);

    for (int n = 0; n <= num_added; ++n) {
		if (n >= num_added) changed = true;
        else v = vertices[vertex_index].batch;

		glBindTexture(GL_TEXTURE_2D, prev_id);
		if (v->texture_id != prev_id) {
			prev_id = v->texture_id;
			changed = true;
		}

		use_shader(prev_shader);
		if (v->shader != prev_shader) {
			prev_shader = v->shader;
			changed = true;
		}

        use_blend_mode(prev_blend_mode);
		if (v->blend_mode != prev_blend_mode) {
			prev_blend_mode = v->blend_mode;
			changed = true;
		}

		if (changed) {
            glDrawArrays(GL_QUADS, vertex_offset, num_vertices);

            vertex_offset += num_vertices;
			indices_offset += num_indices;
			num_vertices = 0;
			num_indices = 0;

			changed = false;
		}
        vertex_index += v->num_vertices;
        num_vertices += v->num_vertices;
        num_indices += v->num_indices;
	}

    glDisable(GL_DEPTH_TEST);
}

void PXL_Batch::free() {
	if (batch_created) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

        //enable vertex attrib pointers when rendering
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glDeleteBuffers(1, &vbo_id);

        total_vertices = 0;
        int num_vertices = 0;
        while (total_vertices < vertices.size()) {
            num_vertices = vertices[total_vertices].batch->num_vertices;
            delete vertices[total_vertices].batch;
            total_vertices += num_vertices;
        }
        //clear and set capacity to 0
        std::vector<PXL_VertexPoint>().swap(vertices);

		batch_created = false;
		vbo_id = 0;
	}
}

PXL_Batch::~PXL_Batch() {
	free();
}