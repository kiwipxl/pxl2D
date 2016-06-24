#include "graphics/FrameBuffer.h"
#include "system/Window.h"
#include "system/Debug.h"

namespace pxl { namespace graphics {

    FrameBuffer::FrameBuffer(int w, int h, bool create_depth_buffer) {
	    frame_buffer_created = false;
	    create_frame_buffer(w, h, create_depth_buffer);
    }

    FrameBuffer* create_frame_buffer(int w, int h, bool create_depth_buffer) {
	    return new FrameBuffer(w, h, create_depth_buffer);
    }

    void FrameBuffer::create_frame_buffer(int w, int h, bool create_depth_buffer) {
	    //if the frame_buffer is already created then delete the frame_buffer but not the buffer
	    if (frame_buffer_created) {
		    free();
		    frame_buffer_created = false;
	    }
	    width = w;
	    height = h;

	    //creates the frame buffer container object
	    glGenFramebuffers(1, &id);
	    bind(GL_FRAMEBUFFER_WRITE);

	    if (create_depth_buffer) {
		    depth_id = -1;
		    //glGenRenderbuffers(1, &depth_id);
		    //glBindRenderbuffer(GL_FRAMEBUFFER_WRITE, depth_id);
		    //todo: compatible depth component for gles2 and opengl
		    //glRenderbufferStorage(GL_FRAMEBUFFER_WRITE, NULL, width, height);
		    //glFramebufferRenderbuffer(GL_FRAMEBUFFER_WRITE, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_id);
	    }else {
		    depth_id = -1;
	    }

	    texture = new Texture();
	    texture->create_texture(width, height, NULL, CHANNEL_RGBA);
	    glFramebufferTexture2D(GL_FRAMEBUFFER_WRITE, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, get_texture_id(), 0);

	    clear(1, 1, 1, 0);

	    if (glCheckFramebufferStatus(GL_FRAMEBUFFER_WRITE) != GL_FRAMEBUFFER_COMPLETE) {
		    std::cout << "an error occurred when creating a frame buffer\n";
	    }

	    frame_buffer_created = true;

	    //bind to default frame buffer
	    glBindFramebuffer(GL_FRAMEBUFFER_WRITE, 0);
    }

    void FrameBuffer::clear(float r, float g, float b, float a) {
	    glBindFramebuffer(GL_FRAMEBUFFER_WRITE, id);
	    glClearColor(r, g, b, a);
	    glClear(GL_COLOR_BUFFER_BIT);
	    //todo: error code 1282 occurs here
	    if (depth_id != -1) {
		    glClear(GL_DEPTH_BUFFER_BIT);
	    }
    }

    void FrameBuffer::blit(FrameBuffer* dest_frame_buffer, Rect* rect, Rect* src_rect, TextureFilter filter) {
	    int draw_id = 0;
	    if (dest_frame_buffer != NULL) {
		    draw_id = dest_frame_buffer->get_id();
		    if (rect == NULL) {
			    //todo replace magic variables with window width/height somehow
			    rect->x = 0; rect->y = 0; rect->w = 1024; rect->h = 768;
		    }else {
			    frame_rect = *rect;
		    }
	    }else if (rect == NULL) {
		    rect->x = 0; rect->y = 0; rect->w = dest_frame_buffer->get_width(); rect->h = dest_frame_buffer->get_height();
	    }else {
		    frame_rect = *rect;
	    }

	    if (src_rect == NULL) {
		    frame_src_rect.x = 0; frame_src_rect.y = 0; frame_src_rect.w = width; frame_src_rect.h = height;
	    }else {
		    frame_src_rect = *src_rect;
	    }

	    glBindTexture(GL_TEXTURE_2D, dest_frame_buffer->get_texture_id());
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dest_frame_buffer->get_pixels());
	    print << "pixels: " << dest_frame_buffer->get_pixels() << "\n";

	    //glBindFramebuffer(GL_FRAMEBUFFER_READ, id);
	    //glBindFramebuffer(GL_FRAMEBUFFER_WRITE, draw_id);

	    //int8* pixels = new int8[size];
	    //glReadPixels(frame_rect.x, frame_rect.y, frame_rect.w, frame_rect.h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	    //todo: blit frame buffer in gles2
	    /*glBlitFramebuffer(frame_src_rect.x, frame_src_rect.y, frame_src_rect.w, frame_src_rect.h, 
					      frame_rect.x, frame_rect.y, frame_rect.w, frame_rect.h, 
					      GL_COLOR_BUFFER_BIT, filter);*/
    }

    void FrameBuffer::blit(Texture& dest_texture, Rect* rect) {
	    bind(GL_FRAMEBUFFER_READ);
	    dest_texture.bind();
	    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rect->x, rect->y, rect->w, rect->h);
    }

    void FrameBuffer::bind(FrameBufferAction action) {
	    glBindFramebuffer(action, id);
    }

    void FrameBuffer::bind_texture() {
	    texture->bind();
    }

    uint8* FrameBuffer::get_pixels() {
	    return texture->get_vram_pixels();
    }

    void FrameBuffer::free() {
	    if (frame_buffer_created) {
		    glDeleteFramebuffers(1, &id);
		    if (depth_id != -1) { glDeleteRenderbuffers(1, &depth_id); }
		    delete texture;
		    frame_buffer_created = false;
	    }
    }

    FrameBuffer::~FrameBuffer() {
	    free();
    }
}};