#ifndef _PXL_BATCH_H
#define _PXL_BATCH_H

#include <vector>
#include "graphics/PXL_Bitmap.h"
#include "graphics/PXL_Texture.h"
#include "graphics/PXL_Matrix4.h"
#include "graphics/PXL_Structs.h"
#include "graphics/PXL_ShaderUtils.h"
#include "graphics/PXL_ShaderProgram.h"
#include "graphics/PXL_FrameBuffer.h"
#include "system/PXL_API.h"
#include "system/PXL_Window.h"

enum PXL_Flip {
	PXL_FLIP_NONE,
	PXL_FLIP_HORIZONTAL,
	PXL_FLIP_VERTICAL
};

enum PXL_BlendMode {
	PXL_BLEND, /**> Applies blending when rendering **/
	PXL_NO_BLEND, /**> Doesn't blend when rendering **/
};

struct PXL_VertexPoint {

	struct PXL_VertexPos {
		float x = 0, y = 0;
	} pos;
	struct PXL_Vertex_UVCoord {
		PXL_ushort x = 0, y = 0;
	} uv;
	struct PXL_Vertex_RGBA {
		PXL_ubyte r = 255, g = 255, b = 255, a = 255;
	} colour;
};

struct PXL_VertexBatch {

	//vertex values
	GLuint texture_id = 0;
	int z_depth = 0;
	PXL_ShaderProgram* shader = NULL;
	PXL_BlendMode blend_mode;
	PXL_ushort num_vertices;
	PXL_ushort num_indices;

	//transform cache values
	PXL_Colour colour;
	PXL_Rect rect;
	PXL_Rect src_rect;
	PXL_Vec2 origin;
	float rotation;
	PXL_Flip flip;
};

/** The PXL_Batch class handles batch rendering of textures, texture sheets and sprites with transformations.
The batch works by sorting each texture to limit binding calls and by chunking data to speed up render times.\n
Use add() to add a texture to the render queue and render_all() once you've finished adding all your items to render.
You can call render_all() as many times as you want, however, it's best to keep it limited.\n
You can also set the shader when rendering with set_shader().
**/
class PXL_Batch {

public:
	/** Creates a batch with a specified max render size
	@param size the max amount of adds this batch can have
	**/
	PXL_Batch() { }
	PXL_Batch(PXL_Window* window);
	~PXL_Batch();

	//batch matrices
	PXL_Matrix4 view_mat;
	PXL_Matrix4 perspective_mat;

	/** Creates the batch with the specified max render size
	@param size the max amount of adds this batch can have and the size of the vbo uploaded
	**/
	void create_batch(PXL_Window* window);

	/** Renders everything that was added to the batch and clears all data when finished. You
	can set where the target will render to using set_target with a PXL_FrameBuffer.
	@see clear_all(), add()
	**/
	void render_all();

	/** Clears everything in the render queue
	@see render_all(), add()
	**/
	void clear_all();

	/** Starts using specified shader
	@param shader A PXL_ShaderProgram shader object
	**/
	void use_shader(PXL_ShaderProgram* shader = PXL_default_shader);

	void use_blend_mode(PXL_BlendMode blend_mode);

	/** Sets the target framebuffer to render to. When render_all is called, everything will be
	rendered to the target. Specifying a target of NULL will render to the default buffer.
	Read more about frame buffers here: https://www.opengl.org/wiki/Framebuffer_Object
	@see render_all(), clear_all(), add()
	**/
	void set_render_target(PXL_FrameBuffer* buffer = NULL);

	void set_window_target(PXL_Window* window);

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
	void add(const PXL_Texture& texture, PXL_Rect* rect, PXL_Rect* src_rect = NULL, float rotation = 0, PXL_Vec2* origin = NULL,
		PXL_Flip flip = PXL_FLIP_NONE, int z_depth = 0, PXL_Colour colour = PXL_COLOUR_WHITE,
		PXL_ShaderProgram* shader = NULL, PXL_BlendMode blend_mode = PXL_BLEND);

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
	PXL_FrameBuffer* target_frame_buffer = NULL;    /**> The target frame buffer object to use when rendering **/
	PXL_ShaderProgram* current_shader = NULL;
	PXL_BlendMode current_blend_mode;
	PXL_Matrix4 proj_view_mat;
	PXL_Window* target_window;
	PXL_Rect render_bounds;

	//vertex data
	GLuint vbo_id; /**> The id associated with the vertex buffer object **/
	GLuint vao_id;
	PXL_uint min_vertex_index = 0;
	PXL_uint max_vertex_index = 0;
	PXL_uint total_vertices = 0;
	PXL_uint min_vertices_count;

	struct VertexContainer {
		PXL_uint batch_index = 0;
		PXL_uint data_index = 0;
		PXL_uint indices_index = 0;
		PXL_uint indices_count = 0;
		std::vector<PXL_VertexBatch> batches;
		std::vector<PXL_VertexPoint> data;
		std::vector<PXL_ushort> indices;
    };

    struct PXL_VertexBatchPoint {
        struct PXL_VertexPos {
            float x = 0, y = 0;
        } pos[4];
        struct PXL_Vertex_UVCoord {
            PXL_ushort x = 0, y = 0;
        } uv[4];
        struct PXL_Vertex_RGBA {
            PXL_ubyte r = 255, g = 255, b = 255, a = 255;
        } colour[4];

        //vertex values
        int z_depth = 0;
        bool uses_transparency = false;
        GLuint texture_id = 0;
        PXL_ShaderProgram* shader = NULL;
        PXL_BlendMode blend_mode;
        PXL_ushort num_vertices;
        PXL_ushort num_indices;
    };

    std::vector<PXL_VertexBatchPoint> vertices;

	/** Verifies whether the texture should be added to the batch and returns the result
	@param rect Used to check the texture position on the screen
	**/
	bool verify_texture_add(const PXL_Texture& texture, PXL_Rect* rect);

	/** Draws each item in the vertex batches list
	**/
	void draw_vbo();
};

#endif