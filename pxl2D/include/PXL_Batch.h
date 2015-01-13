#ifndef BATCH_H
#define BATCH_H

#include "PXL_VBO.h"
#include "PXL_Rect.h"
#include "PXL_Bitmap.h"
#include "PXL_Texture.h"
#include "PXL_Matrix4.h"
#include "PXL_Rect.h"

enum PXL_Flip {
	PXL_FLIP_NONE,
	PXL_FLIP_HORIZONTAL,
	PXL_FLIP_VERTICAL
};

enum PXL_BatchSize {
	PXL_TINY_BATCH = 100, 
	PXL_SMALL_BATCH = 2000, 
	PXL_MEDIUM_BATCH = 10000, 
	PXL_LARGE_BATCH = 25000
};

class PXL_Batch {

	public:
		PXL_Batch(PXL_BatchSize size = PXL_SMALL_BATCH);
		~PXL_Batch();

		void start();
		void end();

		void render(PXL_Texture* texture, PXL_Rect* src_rect, PXL_Rect* rect);
		void render_transformed(PXL_Texture* texture, PXL_Rect* src_rect, PXL_Rect* rect,
			float angle, PXL_Vec2* origin, PXL_Flip flip = PXL_FLIP_NONE);

		void create_batch(PXL_BatchSize size = PXL_SMALL_BATCH);
		void set_colour(float r, float g, float b, float a);
		void set_filters(PXL_TextureFilter min_filter = PXL_GL_LINEAR, PXL_TextureFilter max_filter = PXL_GL_LINEAR);

		void free();

		int get_width() { return width; }
		int get_height() { return height; }
		GLint get_id() { return id; }

	private:
		float width;
		float height;
		GLuint id;

		bool vbo_created;
		PXL_VBO* vbo = NULL;

		PXL_Matrix4 view_mat;
		PXL_Matrix4 perspective_mat;

		vector<PXL_Texture*> textures;
		unsigned int texture_index = 0;

		bool set_vertex_data(int index, PXL_Texture* texture, PXL_Rect* rect, PXL_Rect* src_rect);
		void draw_vbo();
};

extern PXL_Batch* PXL_create_batch(PXL_BatchSize size = PXL_SMALL_BATCH);

#endif