#ifndef RENDERER_H
#define RENDERER_H

#include <glew.h>
#include <vector>
#include "PXL_Texture.h"
#include "PXL_Matrix4.h"
#include "PXL_Rect.h"

enum PXL_Flip {
	PXL_FLIP_NONE,
	PXL_FLIP_HORIZONTAL,
	PXL_FLIP_VERTICAL
};

static int PXL_render_calls = 0;
static int PXL_transform_render_calls = 0;
static int PXL_vertices_uploaded = 0;
static int PXL_total_render_calls = 0;

static PXL_Matrix4 PXL_model_view_mat;
static PXL_Matrix4 PXL_view_mat;
static PXL_Matrix4 PXL_perspective_mat;

static int s_width;
static int s_height;

extern void PXL_initiate(int screen_width, int screen_height);

extern void PXL_render(PXL_Texture* texture, PXL_Rect* src_rect, PXL_Rect* rect, bool c_set_buffer = true);
extern void PXL_render_transform(PXL_Texture* texture, PXL_Rect* src_rect, PXL_Rect* rect,
	float angle, PXL_Vec2* origin, PXL_Flip flip, bool c_set_buffer = true);

extern bool PXL_set_buffer(PXL_Texture* texture, PXL_Rect* src_rect);
extern void PXL_draw_buffer(PXL_Texture* texture, bool upload_buffer = false);

#endif