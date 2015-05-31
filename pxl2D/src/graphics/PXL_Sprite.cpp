#include "graphics/PXL_Sprite.h"
#include <iostream>

PXL_Sprite::PXL_Sprite() : rotation_origin(this), scale_origin(this) {
	scale.x = scale.y = 1;
}

PXL_Sprite::PXL_Sprite(const PXL_Texture& source) : rotation_origin(this), scale_origin(this) {
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

void PXL_Sprite::render(PXL_Batch* batch) {
	if (scale_origin.get_type() != PXL_ORIGIN_CUSTOM_GLOBAL) {
		scale_origin.x = scale_origin.get_relative_point().x + x;
		scale_origin.y = scale_origin.get_relative_point().y + y;
	}
    //todo: check if texture is null before rendering, otherwise will crash
	rect.x = x;
	rect.y = y;
	rect.w = width; rect.h = height;
	batch->add(*texture_source, &rect, &src_rect, rotation, &rotation_origin, &scale_origin, z_depth, colour);
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