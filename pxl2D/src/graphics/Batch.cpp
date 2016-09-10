#include "graphics/Batch.h"
#include <algorithm>
#include "system/Exception.h"
#include "system/Debug.h"

namespace pxl { namespace graphics {

    //cpp constants (hidden from public)
    #define MIN_DEPTH_CHANGE (1.0f / PXL_24U_MAX);                          //the minimum depth value that can be added/subbed in a float

    Batch::Batch(sys::Window* window) {
        batch_created = false;
        create_batch(window);
    }

    void Batch::create_batch(sys::Window* window) {
        free();

        {
            //create the vbo
            glGenBuffers(1, &vbo_id);
            glGenBuffers(1, &ibo_id);

            batch_created = true;
        }

        clear_all();

        if (window != NULL) {
            set_window_target(window);
            render_bounds.x = 0;					render_bounds.y = 0;
            render_bounds.w = window->getWidth();	render_bounds.h = window->getHeight();
        }
        else {
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

    void Batch::use_shader(ShaderProgram* shader) {
        if (shader == NULL) { shader = default_shader; }

        if (current_shader != shader) {
            current_shader = shader;

            //use specified program id
            glUseProgram(current_shader->get_program_id());

            //set matrix uniform in the vertex shader for the program
            glUniformMatrix4fv(current_shader->get_matrix_loc(), 1, false, proj_view_mat.get_raw_matrix());
        }
    }

    void Batch::use_blend_mode(BlendMode blend_mode) {
        //if (current_blend_mode != blend_mode) {
            current_blend_mode = blend_mode;
            if (current_blend_mode == BLEND) {
			    glEnable(GL_BLEND);
                glDepthMask(GL_FALSE);
                glDepthFunc(GL_LESS);
			    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }else if (current_blend_mode == NO_BLEND) {
			    glDisable(GL_BLEND);
                glDepthMask(GL_TRUE);
                glDepthFunc(GL_LESS);
            }
        //}
    }

    void Batch::add(const Texture& texture, Rect* rect, Rect* src_rect, 
	    float rotation, Vec2* rotation_origin, Vec2* scale_origin, 
	    int z_depth, Colour colour, ShaderProgram* shader, BlendMode blend_mode) {
        if (verify_texture_add(texture, rect)) {
            if (total_vertices >= vertices.size()) {
                int prev_size = vertices.size();

                vertices.resize(prev_size + CONFIG_BATCH_VERTEX_RESIZE);

                VertexBatch* last_batch;
                for (int n = 0; n < CONFIG_BATCH_VERTEX_RESIZE; ++n) {
                    if (n % 4 == 0) {
                        last_batch = new VertexBatch();
                        last_batch->num_vertices = 4;
                    }
                    vertices[n + prev_size].batch = last_batch;
                }
            }
            if (total_indices >= indices.size()) {
                indices.resize(indices.size() + CONFIG_BATCH_INDICES_RESIZE);
            }

            VertexPoint* v = &vertices[total_vertices];
            VertexBatch& batch = *v->batch;
            batch.num_vertices = 4;
            batch.num_indices = 6;
            batch.texture_id = texture.get_id();
            batch.shader = shader;
            batch.z_depth = z_depth;
            batch.blend_mode = blend_mode;
		    batch.add_id = num_added;
            if (texture.has_transparency || colour.a != 1.0f) {
                batch.uses_transparency = true;
                batch.blend_mode = BLEND;
            }else {
                batch.uses_transparency = false;
                batch.blend_mode = NO_BLEND;
                total_opq_vertices += 4;
            }

            uint32 i = indices_count;
            indices[total_indices] = i;			indices[total_indices + 1] = i + 1;		indices[total_indices + 2] = i + 2;
            indices[total_indices + 3] = i;		indices[total_indices + 4] = i + 3;		indices[total_indices + 5] = i + 2;
            total_indices += 6;
            indices_count += 4;

            total_vertices += 4;
		    ++num_added;

            /**
            ==================================================================================
                                           Set vertex positions
            ==================================================================================
            **/
            //set vertex pos, uvs and colours
		    //copy rect contents into temp rect
		    Rect r = *rect;

            //set rotation origin
		    Vec2 r_origin;
		    if (rotation_origin != NULL) r_origin = *rotation_origin;
		    //set scale origin
		    Vec2 s_origin;
		    if (scale_origin != NULL) {
			    s_origin = *scale_origin;
			    //apply scale origin offset
			    s_origin.x -= rect->x;
			    s_origin.y -= rect->y;
			    if (s_origin.x != 0 && s_origin.y != 0) {
				    r.x += ((texture.get_width() - rect->w) / (texture.get_width() / s_origin.x));
				    r.y += ((texture.get_height() - rect->h) / (texture.get_height() / s_origin.y));
			    }
		    }

            //apply rotation
            if (rotation != 0) {
                //set rotation to degrees rather than radians
                rotation = rotation / PXL_RADIANS;
			    float c = math::fast_cos(rotation); float s = math::fast_sin(rotation);

			    r_origin.x -= r.x; r_origin.y -= r.y;
			    r.x += r_origin.x; r.y += r_origin.y;
			    r.w -= r_origin.x; r.h -= r_origin.y;

                //set vertex position including scale and rotation
                v[0].pos.x = r.x + ((c * -r_origin.x) - (s * -r_origin.y));
                v[0].pos.y = r.y + ((s * -r_origin.x) + (c * -r_origin.y));

                v[1].pos.x = r.x + ((c * r.w) - (s * -r_origin.y));
                v[1].pos.y = r.y + ((s * r.w) + (c * -r_origin.y));

                v[2].pos.x = r.x + ((c * r.w) - (s * r.h));
                v[2].pos.y = r.y + ((s * r.w) + (c * r.h));

                v[3].pos.x = r.x + ((c * -r_origin.x) - (s * r.h));
                v[3].pos.y = r.y + ((s * -r_origin.x) + (c * r.h));
            }else {
                //set vertex position including scale
			    v[0].pos.x = r.x;			v[0].pos.y = r.y;
			    v[1].pos.x = r.x + r.w;		v[1].pos.y = r.y;
			    v[2].pos.x = r.x + r.w;		v[2].pos.y = r.y + r.h;
			    v[3].pos.x = r.x;			v[3].pos.y = r.y + r.h;
            }
		    v[0].order = total_vertices + 3;
		    v[1].order = total_vertices + 2;
		    v[2].order = total_vertices + 1;
		    v[3].order = total_vertices;

            /**
            ==================================================================================
                                           Set UV vertex coords
            ==================================================================================
            **/
            //default un-normalised uv coords
            uint16 uv_x = 0; uint16 uv_y = 0; uint16 uv_w = USHRT_MAX; uint16 uv_h = USHRT_MAX;
            if (src_rect != NULL) {
                //calculate uv x, y, w, h by the src rect
                uv_x = (src_rect->x / texture.get_width()) * USHRT_MAX; uv_y = (src_rect->y / texture.get_height()) * USHRT_MAX;
                uv_w = (src_rect->w / texture.get_width()) * USHRT_MAX; uv_h = (src_rect->h / texture.get_height()) * USHRT_MAX;
            }

            //set uv coordinates
            v[0].uv.x = uv_x;										v[0].uv.y = uv_y;
            v[1].uv.x = uv_x + uv_w;								v[1].uv.y = uv_y;
            v[2].uv.x = uv_x + uv_w;								v[2].uv.y = uv_y + uv_h;
            v[3].uv.x = uv_x;										v[3].uv.y = uv_y + uv_h;

            /**
            ==================================================================================
                                           Set vertex colours
            ==================================================================================
            **/
            int i_r = colour.r * 255; int i_g = colour.g * 255; int i_b = colour.b * 255; int i_a = colour.a * 255;

            //set vertex colours
            for (int n = 0; n < 4; ++n) {
                v[n].colour.r = i_r;
                v[n].colour.g = i_g;
                v[n].colour.b = i_b;
                v[n].colour.a = i_a;
            }
        }
    }

    inline bool Batch::verify_texture_add(const Texture& texture, Rect* rect) {
	    return true;
        if (texture.texture_created) {
            if (rect->x + rect->w > render_bounds.x && rect->y + rect->h > render_bounds.y && rect->x < render_bounds.w && rect->y < render_bounds.h) {
                return true;
            }
        }
        return false;
    }

    void Batch::set_render_target(FrameBuffer* f) {
        //sets the target frame buffer to be used for rendering
        target_frame_buffer = f;
    }

    void Batch::set_window_target(sys::Window* window) {
        target_window = window;
    }


    void Batch::clear_all() {
        total_vertices = 0;
        total_opq_vertices = 0;
        total_indices = 0;
        indices_count = 0;
        num_added = 0;
    }

    void Batch::render_all() {
        //if there are no textures to draw or no vertex data then return
        if (num_added != 0) {
            //if a framebuffer is specified, bind to it, if not bind to the default framebuffer
            if (target_frame_buffer != NULL) {
                target_frame_buffer->bind(GL_FRAMEBUFFER_WRITE);
            }else {
                glBindFramebuffer(GL_FRAMEBUFFER_WRITE, 0);
            }

            proj_view_mat = (perspective_mat * view_mat).transpose();

            //clear depth buffer
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_ALWAYS);
            glClearDepthf(1.0f);
            glClear(GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);

            draw_vbo();

            glBindFramebuffer(GL_FRAMEBUFFER_WRITE, 0);
        }
        clear_all();
    }

    void Batch::draw_vbo() {
        //loops through each texture and draws the vertex data with that texture id
        int vertex_offset = 0;
        int indices_offset = 0;
	    int vertex_index = 0;
        int num_vertices = 0;
        int num_indices = 0;
        bool changed = false;
	    VertexBatch* v;

        std::stable_sort(vertices.begin(), vertices.begin() + total_vertices,
        [](const VertexPoint& a, const VertexPoint& b) {
            if (a.batch->uses_transparency < b.batch->uses_transparency) return true;
            if (a.batch->uses_transparency > b.batch->uses_transparency) return false;

            if (a.batch->z_depth < b.batch->z_depth) return false;
            if (a.batch->z_depth > b.batch->z_depth) return true;

            if (!a.batch->uses_transparency) {
                if (a.batch->add_id < b.batch->add_id) return false;
                if (a.batch->add_id > b.batch->add_id) return true;
            }else {
                if (a.batch->add_id < b.batch->add_id) return true;
                if (a.batch->add_id > b.batch->add_id) return false;
            }

            return false;
	    });

        //algorithm that calculates the depth buffer value for each vertex batch.
        //primarily used for z depths. basically, the order in which the vertex is in, the higher/lower the
        //depth buffer value will be (which is why z depth is sorted in order above)
	    VertexPoint* vi1 = nullptr;
	    if (total_opq_vertices > 0) {
		    vi1 = &vertices[total_opq_vertices - 1];
		    vi1 -= vi1->batch->num_vertices - 1;
	    }
	    VertexPoint* vi2 = nullptr;
	    if (total_vertices > 0) {
		    vi2 = &vertices[total_vertices - 1];
		    vi2 -= vi2->batch->num_vertices - 1;
	    }
	    VertexPoint* vih = vi1;

        float depth = 1.0f - MIN_DEPTH_CHANGE;
        for (int n = 0; n < num_added; ++n) {
		    bool set_vi1_depth = false;
		    if (vi2 == nullptr) {
			    set_vi1_depth = true;
		    }else if (vi1 == nullptr) {
			    set_vi1_depth = false;
		    }else if (vi2 <= vih) {
                set_vi1_depth = true;
            }else if (vi1 < &vertices[0]) {
                set_vi1_depth = false;
            }else if (vi1->batch->z_depth == vi2->batch->z_depth) {
                set_vi1_depth = vi1->batch->add_id < vi2->batch->add_id;
            }else {
                set_vi1_depth = vi1->batch->z_depth < vi2->batch->z_depth;
            }

            if (set_vi1_depth) {
                vi1->pos.z = depth;
                //move vertex index 1 up to the next vertex batch. if the index is out of bounds, only move by 1
                vi1 -= (vi1 <= &vertices[0]) ? 1 : (vi1 - 1)->batch->num_vertices;
            }else {
                vi2->pos.z = depth;
                //move vertex index 2 up to the next vertex batch. if the index is out of bounds, only move by 1
                vi2 -= (vih == nullptr || vi2 <= vih) ? 1 : (vi2 - 1)->batch->num_vertices;
            }
            depth -= MIN_DEPTH_CHANGE;
        }

        //binds vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

        //enable vertex attrib pointers when rendering
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        //set vertex shader attrib pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPoint), (void*)0);
        glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(VertexPoint), (void*)12);
        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexPoint), (void*)16);

        glBufferData(GL_ARRAY_BUFFER, total_vertices * sizeof(VertexPoint), &vertices[0], GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, total_indices * sizeof(uint32), &indices[0], GL_DYNAMIC_DRAW);

        v = vertices[0].batch;
	    vertex_index = 0;

        GLuint prev_id = v->texture_id;
        glBindTexture(GL_TEXTURE_2D, prev_id);
        BlendMode prev_blend_mode = v->blend_mode;
        use_blend_mode(prev_blend_mode);
        ShaderProgram* prev_shader = v->shader;
        use_shader(prev_shader);

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
                glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (void*)(indices_offset * sizeof(uint32)));

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

    void Batch::free() {
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
            std::vector<VertexPoint>().swap(vertices);

            batch_created = false;
            vbo_id = 0;
        }
    }

    Batch::~Batch() {
        free();
    }
}};