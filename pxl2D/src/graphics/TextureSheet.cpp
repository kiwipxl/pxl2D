#include "graphics/TextureSheet.h"
#include "system/Debug.h"

namespace pxl { namespace graphics {

    FrameBuffer* sheet_frame_buffer = NULL;

    TextureSheet::TextureSheet() {
	    //initiate sheet
	    texture_created = false;
	    width = 0;
	    height = 0;
        bg_colour = COLOUR_TRANSPARENT_BLACK;
        batch = new Batch(NULL);
    }

    void TextureSheet::create_sheet(Channel sheet_channel, bool dispose_batch, bool dispose_list, bool clear_list) {
	    if (!batch->is_created()) {
            sys::show_exception("Could not create texture sheet, batch has been disposed", ERROR_TEXTURE_SHEET_CREATION_FAILED);
            return;
	    }

	    if (width == 0 || height == 0) {
            sys::show_exception("Could not create texture sheet, width/height are less than 0", ERROR_TEXTURE_SHEET_CREATION_FAILED);
		    return;
	    }

	    //initiate frame buffer if it has never been created before
	    if (sheet_frame_buffer == NULL) {
		    sheet_frame_buffer = new FrameBuffer(1, 1);
	    }
        
        sys::print << "width: " << width << ", height: " << height << "\n";

	    int viewport_size[4];
	    glGetIntegerv(GL_VIEWPORT, viewport_size);

	    glViewport(0, 0, width, height);
	    batch->perspective_mat.identity();
	    batch->perspective_mat.scale(1.0f / (width / 2), 1.0f / (height / 2));
	    batch->perspective_mat.translate(-1.0f, -1.0f);

	    create_texture(width, height, NULL, sheet_channel);
	    sheet_frame_buffer->get_texture()->create_texture(width, height, NULL, CHANNEL_RGBA);

	    sheet_frame_buffer->clear(bg_colour.r, bg_colour.g, bg_colour.b, bg_colour.a);
	    batch->set_render_target(sheet_frame_buffer);
	    batch->render_all();

	    batch->set_render_target();

	    Rect rect;
	    rect.x = 0; rect.y = 0; rect.w = width; rect.h = height;
	    sheet_frame_buffer->blit(*this, &rect);

	    glViewport(viewport_size[0], viewport_size[1], viewport_size[2], viewport_size[3]);

	    //todo: glreadbuffer not supported in gles2
	    //glReadBuffer(GL_BACK);

	    texture_created = true;

	    if (dispose_batch) {
		    batch->free();
	    }

	    if (dispose_list) {
		    for (size_t n = 0; n < texture_list.size(); ++n) {
			    delete texture_list[n];
		    }
	    }

        if (clear_list) clear();

        check_has_transparency();
    }

    void TextureSheet::clear() {
	    texture_list.clear();
    }

    void TextureSheet::add(Texture* texture, Rect* rect, Rect* src_rect, 
						       float rotation, Vec2* rotation_origin, Vec2* scale_origin, 
						       int z_depth, Colour colour,
						       ShaderProgram* shader, BlendMode blend_mode) {
	    if (!batch->is_created()) {
            sys::show_exception("Could not add texture to texture sheet as the batch has been disposed", ERROR_TEXTURE_SHEET_ADD_FAILED);
		    return;
	    }

	    batch->add(*texture, rect, src_rect, rotation, rotation_origin, scale_origin, z_depth, colour, shader, blend_mode);
	    int w = rect->x + rect->w;
	    int h = rect->y + rect->h;
	    if (w > width) { width = w; }
	    if (h > height) { height = h; }
	    texture_list.push_back(texture);
    }

    void TextureSheet::free() {
	    if (texture_created) {
		    glDeleteTextures(1, &id);

		    batch->free();
		    for (size_t n = 0; n < texture_list.size(); ++n) {
			    delete texture_list[n];
		    }
		    clear();

		    texture_created = false;
	    }
    }

    TextureSheet::~TextureSheet() {
	    free();
    }
}};