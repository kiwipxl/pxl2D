#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "graphics/GraphicsAPI.h"
#include "graphics/Structs.h"
#include "graphics/Bitmap.h"
#include "API.h"
#include "system/Exception.h"

namespace pxl { namespace graphics {

    typedef int TextureFilter;

    /** The Texture class handles uploading of pixel data to the GPU to create a texture that can be
    used with a Batch to render images to the screen
    A texture can be created with a Bitmap, a PixelBuffer or a raw pixel array.
    **/
    class Texture {

	    public:
		    Texture();
		    ~Texture();

		    bool texture_created; /**< Defines whether the texture has been created or not **/
		    bool has_transparency = false;

		    bool create_texture(std::string file_path);
		    /** Creates the texture from specified bitmap
		    @param bitmap Holds all pixel information for an image
		    @param pixel_mode The pixel type of the pixel data (default is R, G, B, A)
		    **/
		    bool create_texture(Bitmap* bitmap);
		    bool create_texture(int w, int h, uint8* pixels, Channel pixel_channel = CHANNEL_RGBA);

		    void bind();

		    /** Sets filter parameters for uploaded texture
		    @param min_filter The filter value for when the texture is scaled and is smaller than its original size
		    @param max_filter The filter value for when the texture is scaled and is larger than its original size
		    **/
            void set_filters(TextureFilter min_filter = GL_LINEAR, TextureFilter max_filter = GL_LINEAR);
        
		    bool check_has_transparency();
		
		    void update_data(uint8* pixels);

		    /** Deletes all texture information
		    **/
		    void free();

		    /** Gets the raw pixel array contents stored in the GPU VRAM
		    \return A raw pixel byte array
		    **/
		    uint8* get_vram_pixels();
		    /** Gets the texture width
		    \return The width of the texture
		    **/
		    int get_width() const { return width; }
		    /** Gets the texture height
		    \return The height of the texture
		    **/
		    int get_height() const { return height; }
		    /** Gets the opengl id associated with this texture
		    \return The id
		    **/
		    GLint get_id() const { return id; }
		    Channel get_channel() const { return channel; }

	    protected:
		    int width; /**< The width of the texture **/
		    int height; /**< The height of the texture **/
		    GLuint id; /**< The opengl id associated with the texture **/
		    Channel channel;
    };

    /**
    \*brief: Creates a texture from specified bitmap
    @param bitmap Holds all pixel information for an image
    @param pixel_mode The pixel type of the pixel data (default is R, G, B, A)
    **/
    extern Texture* create_texture(Bitmap* bitmap, Channel channel);
}};

#endif