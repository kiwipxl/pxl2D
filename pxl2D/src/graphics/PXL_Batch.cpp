#include "graphics/PXL_Batch.h"
#include <algorithm>
#include "system/PXL_Window.h"
#include "system/PXL_Exception.h"
#include "system/PXL_Debug.h"

//cpp constants (hidden from public)
#define MIN_DEPTH_CHANGE (1.0f / PXL_24U_MAX);                          //the minimum depth value that can be added/subbed in a float
#define BUFFER_INDEX_OFFSET(offset) (static_cast<char*>(0) + offset)    //somehow calculates a valid buffer indices offset. godamnit opengl

PXL_Batch::PXL_Batch(PXL_Window* window) {
    batch_created = false;
    create_batch(window);
}

void PXL_Batch::create_batch(PXL_Window* window) {
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
        render_bounds.w = window->get_width();	render_bounds.h = window->get_height();
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
            glEnable(GL_BLEND);
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

            vertices.resize(prev_size + PXL_CONFIG_BATCH_VERTEX_RESIZE);

            PXL_VertexBatch* last_batch;
            for (int n = 0; n < PXL_CONFIG_BATCH_VERTEX_RESIZE; ++n) {
                if (n % 4 == 0) {
                    last_batch = new PXL_VertexBatch();
                    last_batch->num_vertices = 4;
                }
                vertices[n + prev_size].batch = last_batch;
            }
        }
        if (total_indices >= indices.size()) {
            indices.resize(indices.size() + PXL_CONFIG_BATCH_INDICES_RESIZE);
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
        if (texture.has_transparency || colour.a != 1.0f) {
            batch.uses_transparency = true;
            batch.blend_mode = PXL_BLEND;
            batch.add_id = num_added;
        }else {
            batch.uses_transparency = false;
            batch.blend_mode = PXL_NO_BLEND;
            batch.add_id = num_added;
            total_opq_vertices += 4;
        }

        total_vertices += 4;
        ++num_added;

        int i = indices_count;
        indices[total_indices] = i;			indices[total_indices + 1] = i + 1;		indices[total_indices + 2] = i + 2;
        indices[total_indices + 3] = i;		indices[total_indices + 4] = i + 3;		indices[total_indices + 5] = i + 2;
        total_indices += 6;
        indices_count += 4;

        /**
        ==================================================================================
                                       Set vertex positions
        ==================================================================================
        **/
        //set vertex pos, uvs and colours
        //set origin
        float origin_x = 0; float origin_y = 0;
        if (origin != NULL) { origin_x = origin->x; origin_y = origin->y; }

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
        }else {
            //set vertex position including scale
            v[0].pos.x = x;											v[0].pos.y = y;
            v[1].pos.x = x + scaled_width;							v[1].pos.y = y;
            v[2].pos.x = x + scaled_width;							v[2].pos.y = y + scaled_height;
            v[3].pos.x = x;											v[3].pos.y = y + scaled_height;
        }

        /**
        ==================================================================================
                                       Set UV vertex coords
        ==================================================================================
        **/
        //default un-normalised uv coords
        uint16 uv_x = 0; uint16 uv_y = 0; uint16 uv_w = PXL_USHRT_MAX; uint16 uv_h = PXL_USHRT_MAX;
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
    total_opq_vertices = 0;
    total_indices = 0;
    indices_count = 0;
    num_added = 0;
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
    //loops through each texture and draws the vertex data with that texture id
    int vertex_offset = 0;
    int indices_offset = 0;
    int vertex_index = total_vertices - vertices[total_vertices - 1].batch->num_vertices;
    int num_vertices = 0;
    int num_indices = 0;
    bool changed = false;
    PXL_VertexBatch* v;

    //todo: replace this sort with tim sort or merge sort
    std::stable_sort(vertices.begin(), vertices.begin() + total_vertices,
        [](const PXL_VertexPoint& a, const PXL_VertexPoint& b) {
            if (a.batch->uses_transparency < b.batch->uses_transparency) return true;
            if (b.batch->uses_transparency < a.batch->uses_transparency) return false;

            if (a.batch->z_depth < b.batch->z_depth) return false;
            if (b.batch->z_depth < a.batch->z_depth) return true;

            if (!a.batch->uses_transparency) {
                if (a.batch->add_id < b.batch->add_id) return false;
                if (b.batch->add_id < a.batch->add_id) return true;
            }else {
                if (a.batch->add_id < b.batch->add_id) return true;
                if (b.batch->add_id < a.batch->add_id) return false;
            }

            return false;
        }
    );

    //algorithm that calculates the depth buffer value for each vertex batch.
    //primarily used for z depths. basically, the order in which the vertex is in, the higher/lower the
    //depth buffer value will be (which is why z depth is sorted in order above)
    PXL_VertexPoint* vi1 = &vertices[total_opq_vertices - 1];
    vi1 -= vi1->batch->num_vertices - 1;
    PXL_VertexPoint* vi2 = &vertices[total_vertices - 1];
    vi2 -= vi2->batch->num_vertices - 1;
    PXL_VertexPoint* vih = &vertices[total_opq_vertices - 1];
    vih -= vih->batch->num_vertices - 1;

    float depth = 1.0f - MIN_DEPTH_CHANGE;
    for (int n = 0; n < num_added; ++n) {
        bool set_vi1_depth = false;
        if (vi2 <= vih) {
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
            vi2 -= (vi2 <= vih) ? 1 : (vi2 - 1)->batch->num_vertices;
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PXL_VertexPoint), (void*)0);
    glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(PXL_VertexPoint), (void*)12);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(PXL_VertexPoint), (void*)16);

    glBufferData(GL_ARRAY_BUFFER, total_vertices * sizeof(PXL_VertexPoint), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, total_indices * sizeof(uint32), &indices[0], GL_DYNAMIC_DRAW);

    v = vertices[0].batch;
    vertex_index = 0;

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
            glDrawElements(GL_TRIANGLES, num_vertices * 2, GL_UNSIGNED_INT, BUFFER_INDEX_OFFSET(indices_offset * sizeof(uint32)));

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