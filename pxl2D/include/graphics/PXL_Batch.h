#ifndef BATCH_H
#define BATCH_H

#include <vector>
#include "PXL_Bitmap.h"
#include "PXL_Texture.h"
#include "PXL_Matrix4.h"
#include "PXL_Structs.h"
#include "PXL_ShaderUtils.h"
#include "PXL_TextureSheet.h"
#include "PXL_ShaderProgram.h"
#include "PXL_FrameBuffer.h"

struct PXL_VertexBatch {

	int num_vertices;
	unsigned int texture_id;
	PXL_ShaderProgram* shader = NULL;
};

typedef int PXL_Flip;

#define PXL_FLIP_NONE 0
#define PXL_FLIP_HORIZONTAL 1
#define PXL_FLIP_VERTICAL 2

typedef int PXL_MaxQuads;

#define PXL_TINY_BATCH 100
#define PXL_SMALL_BATCH 2000
#define PXL_MEDIUM_BATCH 10000
#define PXL_LARGE_BATCH 50000

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
		PXL_Batch(PXL_MaxQuads max_quads = PXL_SMALL_BATCH);
		~PXL_Batch();

		//batch matrices
		PXL_Matrix4 view_mat;
		PXL_Matrix4 perspective_mat;

		/** Creates the batch with the specified max render size
		@param size the max amount of adds this batch can have and the size of the vbo uploaded
		**/
		void create_batch(PXL_MaxQuads max_quads = PXL_SMALL_BATCH);

		/** Renders everything that was added to the batch and clears all data when finished. You
		can set where the target will render to using set_target with a PXL_FrameBuffer.
		@see clear_all(), add()
		**/
		void render_all(bool depth_test = true);

		/** Clears everything in the render queue
		@see render_all(), add()
		**/
		void clear_all();

		/** Sets the target framebuffer to render to. When render_all is called, everything will be 
		rendered to the target. Specifying a target of NULL will render to the default buffer.
		Read more about frame buffers here: https://www.opengl.org/wiki/Framebuffer_Object
		@see render_all(), clear_all(), add()
		**/
		void set_target(PXL_FrameBuffer* buffer = NULL);

		/** Adds the specified texture to the render queue
		@param texture The texture to add to the batch
		@param rect Specifies where on the screen the texture will be rendered to
		@param src_rect Specifies which part of the texture to use, NULL to use the whole texture
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect = NULL);

		/** Adds the specified texture with it's transformations to the render queue
		@param texture The texture to add to the batch
		@param rect Specifies where on the screen the texture will be rendered to
		@param src_rect Specifies which part of the texture to use, NULL to use the whole texture
		@param flip defines the flip transformation for the texture
		@param shader The shader to use when rendering this texture. Use NULL to use the default shader
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, PXL_ShaderProgram* shader);

		/** Adds the specified texture with it's transformations to the render queue
		@param texture The texture to add to the batch
		@param rect Specifies where on the screen the texture will be rendered to
		@param src_rect Specifies which part of the texture to use, NULL to use the whole texture
		@param rotation the rotation transformation of the texture
		@param origin the origin point of which the texture rotates around, NULL to use top-left (0, 0)
		@param flip defines the flip transformation for the texture
		@param shader The shader to use when rendering this texture. Use NULL to use the default shader
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin);

		/** Adds the specified texture with it's transformations to the render queue
		@param texture The texture to add to the batch
		@param rect Specifies where on the screen the texture will be rendered to
		@param src_rect Specifies which part of the texture to use, NULL to use the whole texture
		@param rotation the rotation transformation of the texture
		@param origin the origin point of which the texture rotates around, NULL to use top-left (0, 0)
		@param flip defines the flip transformation for the texture
		@param shader The shader to use when rendering this texture. Use NULL to use the default shader
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin, 
				 PXL_Flip flip, PXL_ShaderProgram* shader);

		/** Adds the specified texture with a colour modification
		@param texture The texture to add to the batch
		@param rect Specifies where on the screen the texture will be rendered to
		@param src_rect Specifies which part of the texture to use. Use NULL to use the whole texture
		@param r, g, b, a colour ranges from 0 to 1 which set the texture colour
		@param flip defines the flip transformation for the texture
		@param shader The shader to use when rendering this texture. Use NULL to use the default shader
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, PXL_Flip flip, PXL_ShaderProgram* shader,
				 float r = 1, float g = 1, float b = 1, float a = 1);

		/** Adds the specified texture with a colour modification
		@param texture The texture to add to the batch
		@param rect Specifies where on the screen the texture will be rendered to
		@param src_rect Specifies which part of the texture to use. Use NULL to use the whole texture
		@param r, g, b, a colour ranges from 0 to 1 which set the texture colour
		@param rotation the rotation transformation of the texture
		@param origin the origin point of which the texture rotates around. Use NULL for top-left (0, 0)
		@param flip defines the flip transformation for the texture
		@param shader The shader to use when rendering this texture. Use NULL to use the default shader
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin,
			PXL_Flip flip, PXL_ShaderProgram* shader, float r = 1, float g = 1, float b = 1, float a = 1);

		/** Deletes everything made in this batch
		**/
		void free();

		/** Gets the max amount of quads this batch can add
		\return Max quad amount number
		**/
		int get_max_quads() { return max_quads_amount; }
		/** Gets the amount of items added to the batch
		@return The number of added items
		**/
		int get_num_added() { return num_added; }

	private:
		//batch info
		int max_quads_amount; /**> The max amount of quads this batch can add **/
		int num_added; /**> The current number of added items in this batch **/
		PXL_FrameBuffer* target_frame_buffer = NULL; /**> The target frame buffer object to use when rendering **/

		//vbo
		bool vbo_created; /**> Defines whether or not the vertex buffer object has been created **/
		GLuint vertex_buffer_id; /**> The id associated with the vertex buffer object **/
		std::vector<PXL_VertexBatch> vertex_batches; /**> List that contains vertex batches used for rendering **/
		std::vector<PXL_VertexPoint> vertex_data;
		int vertex_data_size;

		/** Starts using specified shader
		@param shader A PXL_ShaderProgram shader object
		**/
		void set_shader(PXL_ShaderProgram* shader = PXL_default_shader);

		/** Verifies whether the texture should be added to the batch and returns the result
		@param rect Used to check the texture position on the screen
		**/
		bool verify_texture_add(PXL_Texture* texture, PXL_Rect* rect);

		/** Calculates the new vertex pos, uvs and colour based on the specified parameters and stores it in the vertex batches list
		@param texture The texture to add to the batch
		@param rect Specifies where on the screen the texture will be rendered to
		@param src_rect Specifies which part of the texture to use, NULL to use the whole texture
		@param rotation The rotation transformation of the texture
		@param origin The origin point of which the texture rotates around, NULL to use top-left (0, 0)
		@param flip Defines the flip transformation for the texture
		@param r, g, b, a Colour ranges from 0 to 1 which set the texture colour
		**/
		void add_quad(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect,
						  float rotation = 0, PXL_Vec2* origin = NULL, PXL_Flip flip = PXL_FLIP_NONE, 
						  float r = 1, float g = 1, float b = 1, float a = 1, PXL_ShaderProgram* shader = NULL);

		/** Calculates the vertex position for a quad based on the parameters
		@param texture The texture to add to the batch
		@param rect Specifies where on the screen the texture will be rendered to
		@param rotation The rotation transformation of the texture
		@param origin The origin point of which the texture rotates around. Use NULL to use the top-left (0, 0)
		@param flip Defines the flip transformation for the texture
		**/
		void set_quad_pos(PXL_Texture* texture, PXL_Rect* rect, float rotation, PXL_Vec2* origin, PXL_Flip flip);

		/** Calculates the vertex uvs for 4 different vertex points
		@param texture The texture to add to the batch
		@param src_rect Specifies which part of the texture to use, NULL to use the whole texture
		**/
		void set_quad_uvs(PXL_Texture* texture, PXL_Rect* src_rect);

		/** Calculates the vertex colours for 4 different vertex points
		@param r, g, b, a Colour ranges from 0 to 1 which set the texture colour
		**/
		void set_quad_colours(float r, float g, float b, float a);

		/** Draws each item in the vertex batches list
		**/
		void draw_vbo();
};

/** Creates a batch and returns it
@param size the max amount of adds this batch can have
**/
extern PXL_Batch* PXL_create_batch(PXL_MaxQuads max_quads = PXL_SMALL_BATCH);

#endif