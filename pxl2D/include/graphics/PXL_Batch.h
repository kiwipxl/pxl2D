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

	std::vector<PXL_VertexPoint> vertices;
	unsigned int texture_id;
	PXL_ShaderProgram* shader;
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

		/** Sets the shader to use when the batch render is called
		@param shader the PXL_ShaderProgram to use
		@see render_all()
		**/
		void set_shader(PXL_ShaderProgram* shader = PXL_default_shader);

		/** Adds the specified texture to the render queue
		@param texture The texture to add to the batch
		@param rect Specifies where on the screen the texture will be rendered to
		@param src_rect Specifies which part of the texture to use, NULL to use the whole texture
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect);

		/** Adds the specified texture with it's transformations to the render queue
		@param texture The texture to add to the batch
		@param rect Specifies where on the screen the texture will be rendered to
		@param src_rect Specifies which part of the texture to use, NULL to use the whole texture
		@param rotation the rotation transformation of the texture
		@param origin the origin point of which the texture rotates around, NULL to use top-left (0, 0)
		@param flip defines the flip transformation for the texture
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin, PXL_Flip flip);

		/** Adds the specified texture with it's transformations to the render queue
		@param texture The texture to add to the batch
		@param rect Specifies where on the screen the texture will be rendered to
		@param src_rect Specifies which part of the texture to use, NULL to use the whole texture
		@param flip defines the flip transformation for the texture
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, PXL_Flip flip);

		/** Adds the specified texture with a colour modification
		@param texture The texture to add to the batch
		@param rect Specifies where on the screen the texture will be rendered to
		@param src_rect Specifies which part of the texture to use, NULL to use the whole texture
		@param r, g, b, a colour ranges from 0 to 255 which set the texture colour
		@param flip defines the flip transformation for the texture
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, int r, int g, int b, int a, PXL_Flip flip);

		/** Adds the specified texture with a colour modification
		@param texture The texture to add to the batch
		@param rect Specifies where on the screen the texture will be rendered to
		@param src_rect Specifies which part of the texture to use, NULL to use the whole texture
		@param r, g, b, a colour ranges from 0 to 255 which set the texture colour
		@param rotation the rotation transformation of the texture
		@param origin the origin point of which the texture rotates around, NULL to use top-left (0, 0)
		@param flip defines the flip transformation for the texture
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, int r, int g, int b, int a, 
				 float rotation, PXL_Vec2* origin, PXL_Flip flip);

		/** Deletes everything made in this batch
		**/
		void free();

		/** Gets the max render size of the batch
		\return the max renders amount this batch has
		**/
		int get_max_quads() { return max_quads_amount; }
		int get_num_added() { return num_added; }

	private:
		//batch info
		int max_quads_amount;
		int num_added;
		PXL_FrameBuffer* target_frame_buffer = NULL;

		//vbo
		bool vbo_created;
		unsigned int size;
		GLuint vertex_buffer_id;
		std::vector<PXL_VertexBatch> vertex_batches;

		/** Verifies whether the texture should be added to the batch and returns the result
		@param rect used to check the texture position on the screen
		**/
		bool verify_texture_add(PXL_Texture* texture, PXL_Rect* rect);

		/** Adds 4 new vertex points to the vertex data and calculates the new vertex pos, uvs and colour
		@param texture The texture to add to the batch
		@param rect Specifies where on the screen the texture will be rendered to
		@param src_rect Specifies which part of the texture to use, NULL to use the whole texture
		@param rotation the rotation transformation of the texture
		@param origin the origin point of which the texture rotates around, NULL to use top-left (0, 0)
		@param flip defines the flip transformation for the texture
		@param r, g, b, a colour ranges from 0 to 255 which set the texture colour
		**/
		void add_vertices(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect,
						  float rotation = 0, PXL_Vec2* origin = NULL, PXL_Flip flip = PXL_FLIP_NONE, 
						  int r = 255, int g = 255, int b = 255, int a = 255);

		/** Calculates the vertex position for 4 different vertex points
		@param index the index for the starting point in the vertex data
		@param texture The texture to add to the batch
		@param rect Specifies where on the screen the texture will be rendered to
		@param rotation the rotation transformation of the texture
		@param origin the origin point of which the texture rotates around, NULL to use top-left (0, 0)
		@param flip defines the flip transformation for the texture
		**/
		void set_vertex_pos(int index, PXL_Texture* texture, PXL_Rect* rect, float rotation, PXL_Vec2* origin, PXL_Flip flip);

		/** Calculates the vertex uvs for 4 different vertex points
		@param index the index for the starting point in the vertex data
		@param texture The texture to add to the batch
		@param src_rect Specifies which part of the texture to use, NULL to use the whole texture
		**/
		void set_vertex_uvs(int index, PXL_Texture* texture, PXL_Rect* src_rect);

		/** Calculates the vertex colours for 4 different vertex points
		@param index the index for the starting point in the vertex data
		@param r, g, b, a colour ranges from 0 to 255 which set the texture colour
		**/
		void set_vertex_colours(int index, int r, int g, int b, int a);

		/** Sorts the vertex data texture ids and uploads vertex data in sections depending on texture id, uploads and draws it
		**/
		void draw_vbo();
};

/** Creates a batch and returns it
@param size the max amount of adds this batch can have
**/
extern PXL_Batch* PXL_create_batch(PXL_MaxQuads max_quads = PXL_SMALL_BATCH);

#endif