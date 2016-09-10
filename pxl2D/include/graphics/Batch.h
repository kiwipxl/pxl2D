#ifndef _BATCH_H
#define _BATCH_H

#include <vector>
#include <memory>
#include "graphics/Bitmap.h"
#include "graphics/Texture.h"
#include "graphics/Matrix4.h"
#include "graphics/Structs.h"
#include "graphics/ShaderUtils.h"
#include "graphics/ShaderProgram.h"
#include "graphics/FrameBuffer.h"
#include "system/Window.h"
#include "API.h"

namespace pxl { namespace graphics {

    enum BlendMode {
        BLEND, /**> Applies blending when rendering **/
        NO_BLEND, /**> Doesn't blend when rendering **/
    };

    struct VertexBatch {

        //vertex values
        int z_depth = 0;
        bool uses_transparency = false;
        GLuint texture_id = 0;
        ShaderProgram* shader = NULL;
        BlendMode blend_mode;
        uint16 num_vertices;
        uint16 num_indices;
	    uint32 add_id = 0;

        //transform cache values
        Colour colour;
        Rect rect;
        Rect src_rect;
        Vec2 origin;
        float rotation;
    };

    struct VertexPoint {

	    struct VertexPos {
            float x = 0, y = 0, z = 0;
	    } pos;
	    struct Vertex_UVCoord {
            uint16 x = 0, y = 0;
	    } uv;
	    struct Vertex_RGBA {
            uint8 r = 255, g = 255, b = 255, a = 255;
        } colour;

	    VertexBatch* batch;
	    uint32 order = 0;
    };

    /** The Batch class handles batch rendering of textures, texture sheets and sprites with transformations.
    The batch works by sorting each texture to limit binding calls and by chunking data to speed up render times.\n
    Use add() to add a texture to the render queue and render_all() once you've finished adding all your items to render.
    You can call render_all() as many times as you want, however, it's best to keep it limited.\n
    You can also set the shader when rendering with set_shader().
    **/
    class Batch {

    public:
	    /** Creates a batch with a specified max render size
	    @param size the max amount of adds this batch can have
	    **/
	    Batch() { }
	    Batch(sys::Window* window);
	    ~Batch();

	    //batch matrices
	    Matrix4 view_mat;
	    Matrix4 perspective_mat;

	    /** Creates the batch with the specified max render size
	    @param size the max amount of adds this batch can have and the size of the vbo uploaded
	    **/
	    void create_batch(sys::Window* window);

	    /** Renders everything that was added to the batch and clears all data when finished. You
	    can set where the target will render to using set_target with a FrameBuffer.
	    @see clear_all(), add()
	    **/
	    void render_all();

	    /** Clears everything in the render queue
	    @see render_all(), add()
	    **/
	    void clear_all();

	    /** Starts using specified shader
	    @param shader A ShaderProgram shader object
	    **/
	    void use_shader(ShaderProgram* shader = default_shader);

	    void use_blend_mode(BlendMode blend_mode);

	    /** Sets the target framebuffer to render to. When render_all is called, everything will be
	    rendered to the target. Specifying a target of NULL will render to the default buffer.
	    Read more about frame buffers here: https://www.opengl.org/wiki/Framebuffer_Object
	    @see render_all(), clear_all(), add()
	    **/
	    void set_render_target(FrameBuffer* buffer = NULL);

	    void set_window_target(sys::Window* window);

	    /** Adds the specified texture to the batch render queue and transforms it with all specified parameters
	    @param texture The texture to add to the batch
	    @param rect Specifies where on the screen the texture will be rendered to
	    @param src_rect Specifies which part of the texture to use. Use NULL to use the whole texture
	    @param r, g, b, a colour ranges from 0 to 1 which set the texture colour
	    @param rotation the rotation transformation of the texture
	    @param origin the origin point of which the texture rotates around. Use NULL for top-left (0, 0)
	    @param flip defines the flip transformation for the texture
	    @param shader The shader to use when rendering this texture. Use NULL to use the default shader
	    **/
	    void add(const Texture& texture, Rect* rect, Rect* src_rect = NULL, 
		    float rotation = 0, Vec2* rotation_origin = NULL, Vec2* scale_origin = NULL, int z_depth = 0,
		    Colour colour = COLOUR_WHITE, ShaderProgram* shader = NULL, BlendMode blend_mode = BLEND);

	    /** Deletes everything made in this batch
	    **/
	    void free();

	    /** Gets the amount of items added to the batch
	    @return The number of added items
	    **/
	    int get_num_added() { return num_added; }

	    bool is_created() { return batch_created; }

    private:
	    //batch info
	    bool batch_created = false;                     /**> Defines whether or not the vertex buffer object has been created **/
	    int num_added;                                  /**> The current number of added items in this batch **/
	    FrameBuffer* target_frame_buffer = NULL;    /**> The target frame buffer object to use when rendering **/
	    ShaderProgram* current_shader = NULL;
	    BlendMode current_blend_mode;
	    Matrix4 proj_view_mat;
        sys::Window* target_window;
        Rect render_bounds;

	    //vertex data
        GLuint vbo_id; /**> The id associated with the vertex buffer object **/
        GLuint vao_id;
        GLuint ibo_id;

        uint32 total_vertices = 0;
        uint32 total_opq_vertices = 0;
        uint32 total_indices = 0;
        uint32 indices_count = 0;

        std::vector<VertexPoint> vertices;
        std::vector<uint32> indices;

	    /** Verifies whether the texture should be added to the batch and returns the result
	    @param rect Used to check the texture position on the screen
	    **/
	    inline bool verify_texture_add(const Texture& texture, Rect* rect);

	    /** Draws each item in the vertex batches list
	    **/
	    void draw_vbo();
    };
}};

#endif