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

		void start();
		void end();

		void render(PXL_Texture* texture, PXL_Rect* src_rect, PXL_Rect* rect);
		void render_transformed(PXL_Texture* texture, PXL_Rect* src_rect, PXL_Rect* rect,
			float angle, PXL_Vec2* origin, PXL_Flip flip = PXL_FLIP_NONE);

		void create_batch(PXL_MaxRenders size = PXL_SMALL_BATCH);
		void set_colour(float r = 255, float g = 255, float b = 255, float a = 255);
		void set_filters(PXL_TextureFilter min_filter = PXL_GL_LINEAR, PXL_TextureFilter max_filter = PXL_GL_LINEAR);
		void use_shader(GLint program_id = PXL_default_shader->get_program_id());
		void use_shader(PXL_ShaderProgram* program = PXL_default_shader);

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

		vector<PXL_Texture*> textures;
		unsigned int texture_index = 0;

		bool set_vbo(int index, PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect);
		void draw_vbo();
};

extern PXL_Batch* PXL_create_batch(PXL_MaxRenders size = PXL_SMALL_BATCH);

#endif