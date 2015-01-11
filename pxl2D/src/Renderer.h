#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <glew.h>
#include <vector>
#include "PXL_Texture.h"
#include "Matrix4.h"

static int render_calls = 0;
static int transform_render_calls = 0;
static int vertices_uploaded = 0;
static int total_render_calls = 0;

static Matrix4 model_view_mat;
static Matrix4 view_mat;
static Matrix4 perspective_mat;

static int s_width;
static int s_height;

extern void initiate(int screen_width, int screen_height);

extern void render(PXL_Texture* texture, SDL_Rect* src_rect, SDL_Rect* rect, bool c_set_buffer = true);
extern void render_transform(PXL_Texture* texture, SDL_Rect* src_rect, SDL_Rect* rect,
	float angle, SDL_Point* origin, SDL_RendererFlip flip, bool c_set_buffer = true);

extern bool set_buffer(PXL_Texture* texture, SDL_Rect* src_rect);
extern void draw_buffer(PXL_Texture* texture, bool upload_buffer = false);

#endif