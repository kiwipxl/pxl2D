#ifndef BATCH_H
#define BATCH_H

#include <vector>
#include "PXL_VertexPoint.h"
#include "PXL_Bitmap.h"
#include "PXL_Texture.h"
#include "PXL_Matrix4.h"
#include "PXL_Structs.h"
#include "PXL_ShaderUtils.h"
#include "PXL_TextureSheet.h"

enum PXL_Flip {
	PXL_FLIP_NONE,
	PXL_FLIP_HORIZONTAL,
	PXL_FLIP_VERTICAL
};

enum PXL_MaxRenders {
	PXL_TINY_BATCH = 100, 
	PXL_SMALL_BATCH = 2000, 
	PXL_MEDIUM_BATCH = 10000, 
	PXL_LARGE_BATCH = 50000
};

class PXL_Batch {

	public:
		/**
		\*brief: batch constructor
		\*param [size]: the max amount of adds this batch can have
		**/
		PXL_Batch(PXL_MaxRenders size = PXL_SMALL_BATCH);
		/**
		\*brief: batch deconstructor
		**/
		~PXL_Batch();

		/**
		\*brief: renders everything that was added and clears all data when finished
		**/
		void render_all();

		/**
		\*brief: clears everything in the render queue
		**/
		void clear_all();

		/**
		\*brief: sets the shader to use when render_all is called
		\*param [shader_program_id]: sets the shader from specified shader program id
		**/
		void set_shader(GLint shader_program_id = PXL_default_shader->get_program_id());

		/**
		\*brief: sets the shader to use when render_all is called
		\*param [shader]: sets the shader to the specified PXL_ShaderProgram
		**/
		void set_shader(PXL_ShaderProgram* shader = PXL_default_shader);

		/**
		\*brief: adds the specified texture to the render queue
		\*param [texture]: the texture to add to the batch
		\*param [rect]: specifies where on the screen the texture will be rendered to
		\*param [src_rect]: specifies which part of the texture to use, NULL to use the whole texture
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect);

		/**
		\*brief: adds the specified texture with it's transformations to the render queue
		\*param [texture]: the texture to add to the batch
		\*param [rect]: specifies where on the screen the texture will be rendered to
		\*param [src_rect]: specifies which part of the texture to use, NULL to use the whole texture
		\*param [rotation]: the rotation transformation of the texture
		\*param [origin]: the origin point of which the texture rotates around, NULL to use top-left (0, 0)
		\*param [flip]: defines the flip transformation for the texture
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin, PXL_Flip flip);

		/**
		\*brief: adds the specified texture with it's transformations to the render queue
		\*param [texture]: the texture to add to the batch
		\*param [rect]: specifies where on the screen the texture will be rendered to
		\*param [src_rect]: specifies which part of the texture to use, NULL to use the whole texture
		\*param [flip]: defines the flip transformation for the texture
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, PXL_Flip flip);

		/**
		\*brief: adds the specified texture with a colour modification
		\*param [texture]: the texture to add to the batch
		\*param [rect]: specifies where on the screen the texture will be rendered to
		\*param [src_rect]: specifies which part of the texture to use, NULL to use the whole texture
		\*param [r, g, b, a]: colour ranges from 0 to 255 which set the texture colour
		\*param [flip]: defines the flip transformation for the texture
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, int r, int g, int b, int a, PXL_Flip flip);

		/**
		\*brief: adds the specified texture with a colour modification
		\*param [texture]: the texture to add to the batch
		\*param [rect]: specifies where on the screen the texture will be rendered to
		\*param [src_rect]: specifies which part of the texture to use, NULL to use the whole texture
		\*param [r, g, b, a]: colour ranges from 0 to 255 which set the texture colour
		\*param [rotation]: the rotation transformation of the texture
		\*param [origin]: the origin point of which the texture rotates around, NULL to use top-left (0, 0)
		\*param [flip]: defines the flip transformation for the texture
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, int r, int g, int b, int a, 
				 float rotation, PXL_Vec2* origin, PXL_Flip flip);

		/**
		\*brief: creates the batch and vertex buffer objects
		\*param [size]: the max amount of adds this batch can have and the size of the vbo uploaded
		**/
		void create_batch(PXL_MaxRenders size = PXL_SMALL_BATCH);

		/**
		\*brief: deletes everything made in this batch
		**/
		void free();

		/**
		\*brief: gets the max render size of the batch
		**/
		int get_max_renders() { return max_renders; }

	private:
		//batch info
		int max_renders;

		//vbo
		bool vbo_created;
		unsigned int size;
		GLuint vertex_id;
		vector<PXL_VertexPoint> vertex_data;

		//batch matrices
		PXL_Matrix4 view_mat;
		PXL_Matrix4 perspective_mat;

		//batch textures
		vector<int> texture_ids;
		vector<int> texture_offsets;

		/**
		\*brief: adds a texture id to a vector if it is not already in it
		\*param [texture_id]: the id binded to the texture
		**/
		void PXL_Batch::add_texture(int texture_id);

		/**
		\*brief: verifies whether the texture should be added to the batch and returns the result
		\*param [rect]: used to check the texture position on the screen
		**/
		bool verify_texture_add(PXL_Texture* texture, PXL_Rect* rect);

		/**
		\*brief: adds 4 new vertex points to the vertex data and calculates the new vertex pos, uvs and colour
		\*param [texture]: the texture to add to the batch
		\*param [rect]: specifies where on the screen the texture will be rendered to
		\*param [src_rect]: specifies which part of the texture to use, NULL to use the whole texture
		\*param [rotation]: the rotation transformation of the texture
		\*param [origin]: the origin point of which the texture rotates around, NULL to use top-left (0, 0)
		\*param [flip]: defines the flip transformation for the texture
		\*param [r, g, b, a]: colour ranges from 0 to 255 which set the texture colour
		**/
		void add_vertices(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect,
						  float rotation = 0, PXL_Vec2* origin = NULL, PXL_Flip flip = PXL_FLIP_NONE, 
						  int r = 255, int g = 255, int b = 255, int a = 255);

		/**
		\*brief: calculates the vertex position for 4 different vertex points
		\*param [index]: the index for the starting point in the vertex data
		\*param [texture]: the texture to add to the batch
		\*param [rect]: specifies where on the screen the texture will be rendered to
		\*param [rotation]: the rotation transformation of the texture
		\*param [origin]: the origin point of which the texture rotates around, NULL to use top-left (0, 0)
		\*param [flip]: defines the flip transformation for the texture
		**/
		void set_vertex_pos(int index, PXL_Texture* texture, PXL_Rect* rect, float rotation, PXL_Vec2* origin, PXL_Flip flip);

		/**
		\*brief: calculates the vertex uvs for 4 different vertex points
		\*param [index]: the index for the starting point in the vertex data
		\*param [texture]: the texture to add to the batch
		\*param [src_rect]: specifies which part of the texture to use, NULL to use the whole texture
		**/
		void set_vertex_uvs(int index, PXL_Texture* texture, PXL_Rect* src_rect);

		/**
		\*brief: calculates the vertex colours for 4 different vertex points
		\*param [index]: the index for the starting point in the vertex data
		\*param [r, g, b, a]: colour ranges from 0 to 255 which set the texture colour
		**/
		void set_vertex_colours(int index, int r, int g, int b, int a);

		/**
		\*brief: sorts the vertex data texture ids and uploads vertex data in sections depending on texture id, uploads and draws it
		**/
		void draw_vbo();
};

/**
\*brief: creates a batch and returns it
\*param [size]: the max amount of adds this batch can have
**/
extern PXL_Batch* PXL_create_batch(PXL_MaxRenders size = PXL_SMALL_BATCH);

#endif