#ifndef BATCH_H
#define BATCH_H

#include "PXL_VBO.h"
#include "PXL_Bitmap.h"
#include "PXL_Texture.h"
#include "PXL_Matrix4.h"
#include "PXL_Structs.h"
#include "PXL_ShaderUtils.h"

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
		PXL_Batch(PXL_MaxRenders size = PXL_SMALL_BATCH);
		~PXL_Batch();

		/**
		\*brief - renders everything that was added and clears all data when finished
		**/
		void render_all();

		/**
		\*brief - clears everything in the render queue
		**/
		void clear_all();

		/**
		\*brief - sets the shader to use when render_all is called
		\*param [shader_program_id] - sets the shader from specified shader program id
		**/
		void set_shader(GLint shader_program_id = PXL_default_shader->get_program_id());

		/**
		\*brief - sets the shader to use when render_all is called
		\*param [shader] - sets the shader to the specified PXL_ShaderProgram
		**/
		void set_shader(PXL_ShaderProgram* shader = PXL_default_shader);

		/**
		\*brief - adds the specified texture to the render queue
		\*param [src_rect] - specifies which part of the texture to use, NULL to use the whole texture
		\*param [rect] - specifies where on the screen the texture will be rendered to
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect);

		/**
		\*brief - adds the specified texture with it's transformations to the render queue
		\*param [src_rect] - specifies which part of the texture to use, NULL to use the whole texture
		\*param [rect] - specifies where on the screen the texture will be rendered to
		\*param [rotation] - the rotation transformation of the texture
		\*param [origin] - the origin point of which the texture rotates around, NULL to use top-left (0, 0)
		\*param [flip] - defines the flip transformation for the texture
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, float rotation, PXL_Vec2* origin, PXL_Flip flip);

		/**
		\*brief - adds the specified texture with it's transformations to the render queue
		\*param [src_rect] - specifies which part of the texture to use, NULL to use the whole texture
		\*param [rect] - specifies where on the screen the texture will be rendered to
		\*param [flip] - defines the flip transformation for the texture
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, PXL_Flip flip);

		/**
		\*brief - adds the specified texture with a colour modification
		\*param [src_rect] - specifies which part of the texture to use, NULL to use the whole texture
		\*param [rect] - specifies where on the screen the texture will be rendered to
		\*param [r, g, b, a] - colour ranges from 0 to 255 which set the texture colour
		\*param [flip] - defines the flip transformation for the texture
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, int r, int g, int b, int a, PXL_Flip flip);

		/**
		\*brief - adds the specified texture with a colour modification
		\*param [src_rect] - specifies which part of the texture to use, NULL to use the whole texture
		\*param [rect] - specifies where on the screen the texture will be rendered to
		\*param [r, g, b, a] - colour ranges from 0 to 255 which set the texture colour
		\*param [rotation] - the rotation transformation of the texture
		\*param [origin] - the origin point of which the texture rotates around, NULL to use top-left (0, 0)
		\*param [flip] - defines the flip transformation for the texture
		**/
		void add(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect, int r, int g, int b, int a, 
				 float rotation, PXL_Vec2* origin, PXL_Flip flip);

		void create_batch(PXL_MaxRenders size = PXL_SMALL_BATCH);
		void set_colour(float r = 255, float g = 255, float b = 255, float a = 255);
		void set_filters(PXL_TextureFilter min_filter = PXL_GL_LINEAR, PXL_TextureFilter max_filter = PXL_GL_LINEAR);

		void free();

		int get_width() { return width; }
		int get_height() { return height; }
		GLint get_id() { return id; }
		int get_batch_size() { return batch_size; }

	private:
		float width;
		float height;
		GLuint id;

		bool vbo_created;
		PXL_VBO* vbo = NULL;

		PXL_Matrix4 view_mat;
		PXL_Matrix4 perspective_mat;

		int batch_size;

		vector<int> texture_ids;
		vector<int> texture_offsets;

		void PXL_Batch::add_texture(int texture_id);

		/**
		\*brief - verifies whether the texture should be added to the batch and returns the result
		\*param [rect] - used to check the texture position on the screen
		**/
		bool verify_texture_add(PXL_Texture* texture, PXL_Rect* rect);

		void add_vertices(PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect,
						  float rotation = 0, PXL_Vec2* origin = NULL, PXL_Flip flip = PXL_FLIP_NONE, 
						  int r = 255, int g = 255, int b = 255, int a = 255);
		void set_vertex_pos(int index, PXL_Texture* texture, PXL_Rect* rect, float rotation, PXL_Vec2* origin, PXL_Flip flip);
		void set_vertex_uvs(int index, PXL_Texture* texture, PXL_Rect* src_rect);
		void set_vertex_colours(int index, int r, int g, int b, int a);
		void draw_vbo();
};

extern PXL_Batch* PXL_create_batch(PXL_MaxRenders size = PXL_SMALL_BATCH);

#endif