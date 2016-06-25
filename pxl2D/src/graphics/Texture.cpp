#include "graphics/Texture.h"
#include "system/Debug.h"

namespace pxl { namespace graphics {

    Texture::Texture() {
	    texture_created = false;
    }

    bool Texture::create_texture(std::string file_path) {
        bool success = false;

	    //creates a bitmap from the specified file path and checks if it is valid
        Bitmap* bitmap = new Bitmap();
	    if (bitmap->create_bitmap(file_path)) {
		    success = create_texture(bitmap);
	    }
        delete bitmap;

        return success;
    }

    bool Texture::create_texture(Bitmap* bitmap) {
	    texture_created = false;
	    if (bitmap != NULL) {
            has_transparency = bitmap->has_transparency;
		    create_texture(bitmap->get_width(), bitmap->get_height(), bitmap->get_pixels(), bitmap->get_channel());
		    texture_created = true;
	    }else {
            sys::show_exception("Could not create texture, specified bitmap is NULL", ERROR_TEXTURE_CREATION_FAILED);
	    }
	    return texture_created;
    }

    bool Texture::create_texture(int w, int h, uint8* pixels, Channel pixel_channel) {
	    if (w <= 0 || h <= 0) {
            sys::show_exception("Could not create texture, width/height are less than 0", ERROR_TEXTURE_CREATION_FAILED);
		    return false;
	    }

	    width = w;
	    height = h;
	    channel = pixel_channel;

	    glPixelStorei(GL_UNPACK_ALIGNMENT, channel.num_channels);

	    if (!texture_created) { glGenTextures(1, &id); }

	    bind();
	    glTexImage2D(GL_TEXTURE_2D, 0, channel.gl_pixel_mode, width, height, 0, channel.gl_pixel_mode, GL_UNSIGNED_BYTE, pixels);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	    if (!texture_created) { set_filters(); }

	    texture_created = true;

	    return true;
    }

    void Texture::update_data(uint8* pixels) {
	    bind();
	    glPixelStorei(GL_UNPACK_ALIGNMENT, channel.num_channels);
	    glTexImage2D(GL_TEXTURE_2D, 0, channel.gl_pixel_mode, width, height, 0, channel.gl_pixel_mode, GL_UNSIGNED_BYTE, pixels);
    }

    void Texture::bind() {
	    glBindTexture(GL_TEXTURE_2D, id);
    }

    uint8* Texture::get_vram_pixels() {
	    if (texture_created) {
            bind();
            //todo: potential memory leak, must fix
		    uint8* pixels = new uint8[(width * height) * channel.num_channels];
		    glReadPixels(0, 0, width, height, channel.gl_pixel_mode, GL_UNSIGNED_BYTE, pixels);
		    //todo: glgetteximage not supported by gles2
		    //glGetTexImage(GL_TEXTURE_2D, 0, channel.gl_pixel_mode, GL_UNSIGNED_BYTE, pixels);
		    return pixels;
	    }
	    return NULL;
    }

    void Texture::set_filters(TextureFilter min_filter, TextureFilter max_filter) {
	    bind();
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max_filter);
    }

    bool Texture::check_has_transparency() {
        //checks whether the specified pixels contain any transparency
        /*int8* pixels = get_pixels();
        if (pixels == NULL || channel.channel_index.a == -1) {
            delete[] pixels;
            return false;
        }else {
            for (size_t n = 0; n < (width * height) * channel.num_channels; n += channel.num_channels) {
                if (pixels[n + channel.channel_index.a] == 0) {
                    has_transparency = true;
                    delete[] pixels;
                    return true;
                }
            }
        }*/
        return true;
    }

    void Texture::free() {
	    if (texture_created) {
		    glDeleteTextures(1, &id);
		    texture_created = false;
	    }
    }

    Texture::~Texture() {
	    free();
    }
}};