#ifndef _FRAME_BUFFER_H
#define _FRAME_BUFFER_H

#include "graphics/Structs.h"
#include "graphics/Texture.h"
#include "PXLAPI.h"

namespace pxl { namespace graphics {

    typedef int FrameBufferAction;

    #if defined(GL_READ_FRAMEBUFFER)
	    #define GL_FRAMEBUFFER_READ GL_READ_FRAMEBUFFER
    #elif defined(GL_FRAMEBUFFER)
	    #define GL_FRAMEBUFFER_READ GL_FRAMEBUFFER
    #else
	    #define GL_FRAMEBUFFER_READ 0
    #endif

    #if defined(GL_DRAW_FRAMEBUFFER)
	    #define GL_FRAMEBUFFER_WRITE GL_DRAW_FRAMEBUFFER
    #elif defined(GL_FRAMEBUFFER)
	    #define GL_FRAMEBUFFER_WRITE GL_FRAMEBUFFER
    #else
	    #define GL_FRAMEBUFFER_WRITE 0
    #endif

    /** The FrameBuffer class handles uploading of pixel data to the GPU to create a frame_buffer that can be
    used with a Batch to render images to the screen
    A frame_buffer can be created with a Bitmap, a PixelBuffer or a raw pixel array.
    **/
    class FrameBuffer {

	    public:
		    /** Creates the frame_buffer from specified bitmap
		    @param bitmap Holds all pixel information for an image
		    @param pixel_mode The pixel type of the pixel data (default is R, G, B, A)
		    **/
		    FrameBuffer(int width, int height, bool create_depth_buffer = false);
		    ~FrameBuffer();

		    bool frame_buffer_created; /**< Defines whether the frame buffer has been created or not **/

		    /** Creates the frame_buffer from specified bitmap
		    @param bitmap Holds all pixel information for an image
		    @param pixel_mode The pixel type of the pixel data (default is R, G, B, A)
		    **/
		    void create_frame_buffer(int w, int h, bool create_depth_buffer = false);

		    void clear(float r, float g, float b, float a);
		    void blit(FrameBuffer* dest_frame_buffer, Rect* rect = NULL, Rect* src_rect = NULL, 
				      TextureFilter filter = GL_NEAREST);
		    void blit(Texture& dest_texture, Rect* rect);
		    void bind(FrameBufferAction action = GL_FRAMEBUFFER_WRITE);
		    void bind_texture();

		    /** Deletes all frame_buffer information
		    **/
		    void free();

		    /** Gets the raw pixel array contents stored in the GPU
		    \return A raw pixel byte array
		    **/
		    uint8* get_pixels();
		    /** Gets the frame_buffer width
		    \return The width of the frame_buffer
		    **/
		    const int get_width() { return width; }
		    /** Gets the frame_buffer height
		    \return The height of the frame_buffer
		    **/
		    const int get_height() { return height; }
		    /** Gets the id associated with this frame_buffer
		    \return The id
		    **/
		    const GLint get_id() { return id; }
		    /** Gets the id associated with this frame_buffer
		    \return The id
		    **/
		    Texture* get_texture() { return texture; }
		    /** Gets the id associated with this frame_buffer
		    \return The id
		    **/
		    const GLint get_texture_id() { return texture->get_id(); }
		    /** Gets the depth id associated with this frame buffer. The depth id represents the depth buffer created when initialising the frame buffer
		    \return -1 if the depth buffer was not created, if it was then returns the depth id
		    **/
		    const GLint get_depth_id() { return depth_id; }

	    protected:
		    int width; /**< The width of the frame_buffer **/
		    int height; /**< The height of the frame_buffer **/
		    GLuint id; /**< The id associated with the frame_buffer **/
		    Texture* texture;
		    GLuint depth_id;

	    private:
		    Rect frame_rect;
		    Rect frame_src_rect;
    };

    /**
    \*brief: Creates a frame_buffer from specified bitmap
    @param bitmap Holds all pixel information for an image
    @param pixel_mode The pixel type of the pixel data (default is R, G, B, A)
    **/
    extern FrameBuffer* create_frame_buffer(int w, int h, bool create_depth_buffer = false);

}};

#endif