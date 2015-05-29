#include "graphics/PXL_Sprite.h"
#include <iostream>

PXL_Sprite::PXL_Sprite() {
	scale.x = scale.y = 1;
}

PXL_Sprite::PXL_Sprite(const PXL_Texture& source) {
	set_texture(source);
	scale.x = scale.y = 1;
}

void PXL_Sprite::set_texture(const PXL_Texture& source) {
	texture_source = &source;

	if (!texture_set) {
		src_rect.x = src_rect.y = 0;
		src_rect.w = rect.w = width = get_texture_width(); src_rect.h = rect.h = height = get_texture_height();

		texture_set = true;
	}
}

void PXL_Sprite::set_origin(float x, float y) {
	origin_type = PXL_ORIGIN_CUSTOM;
	origin.x = x; origin.y = y;
}

void PXL_Sprite::set_origin(const PXL_Origin origin_point) {
	origin_type = origin_point;

	switch (origin_point) {
		case PXL_ORIGIN_TOP_LEFT:
			origin.x = 0; origin.y = 0;
			break;
		case PXL_ORIGIN_TOP_CENTER:
			origin.x = width / 2; origin.y = 0;
			break;
		case PXL_ORIGIN_TOP_RIGHT:
			origin.x = width; origin.y = 0;
			break;
		case PXL_ORIGIN_MID_LEFT:
			origin.x = 0; origin.y = height / 2;
			break;
		case PXL_ORIGIN_CENTER:
			origin.x = width / 2; origin.y = height / 2;
			break;
		case PXL_ORIGIN_MID_RIGHT:
			origin.x = width; origin.y = height / 2;
			break;
		case PXL_ORIGIN_BOT_LEFT:
			origin.x = 0; origin.y = height;
			break;
		case PXL_ORIGIN_BOT_CENTER:
			origin.x = width / 2; origin.y = height;
			break;
		case PXL_ORIGIN_BOT_RIGHT:
			origin.x = width; origin.y = height;
			break;
	}
}

void PXL_Sprite::render(PXL_Batch* batch) {
    //todo: check if texture is null before rendering, otherwise will crash
	rect.x = x;
	rect.y = y;
	if (origin.x != 0) rect.x += ((texture_source->get_width() - width) / (texture_source->get_width() / origin.x));
	if (origin.y != 0) rect.y += ((texture_source->get_height() - height) / (texture_source->get_height() / origin.y));
	
	rect.w = width; rect.h = height;
	batch->add(*texture_source, &rect, &src_rect, rotation, &origin, NULL, PXL_FLIP_NONE, 
			   z_depth, colour);
}

PXL_Sprite PXL_Sprite::clone() {
    return PXL_Sprite(*this);
}

void PXL_Sprite::free() {
    texture_set = false;
}

PXL_Sprite::~PXL_Sprite() {
	free();
}