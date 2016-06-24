#include "graphics/Sprite.h"
#include <iostream>

namespace pxl { namespace graphics {

    Sprite::Sprite() {
        scale.x = scale.y = 1;
        init();
    }

    Sprite::Sprite(const Texture& source) {
	    set_texture(source);
        init();
    }

    void Sprite::init() {
        scale.x = scale.y = 1;
        rotation_origin = Vec2(.5f, .5f);
        scale_origin = Vec2(.5f, .5f);
    }

    void Sprite::set_texture(const Texture& source) {
	    texture_source = &source;

	    if (!texture_set) {
		    src_rect.x = src_rect.y = 0;
		    src_rect.w = rect.w = width = get_texture_width(); src_rect.h = rect.h = height = get_texture_height();

		    texture_set = true;
	    }
    }

    void Sprite::render(Batch* batch) {
	    //todo: check if texture is null before rendering, otherwise will crash
	    rect.x = x;
	    rect.y = y;
	    rect.w = width; rect.h = height;
	    batch->add(*texture_source, &rect, &src_rect, rotation, &rotation_origin, &scale_origin, z_depth, colour);
    }

    Sprite Sprite::clone() {
        return Sprite(*this);
    }

    void Sprite::free() {
        texture_set = false;
    }

    Sprite::~Sprite() {
	    free();
    }
}};