#include "PXL_FrameBuffer.h"
#include <iostream>
#include "PXL_Window.h"

PXL_FrameBuffer::PXL_FrameBuffer(int w, int h) {
	frame_buffer_created = false;
	create_frame_buffer(w, h);
}

PXL_FrameBuffer* PXL_create_frame_buffer(int w, int h) {
	return new PXL_FrameBuffer(w, h);
}

void PXL_FrameBuffer::create_frame_buffer(int w, int h) {
	//if the frame_buffer is already created then delete the frame_buffer but not the buffer
	if (frame_buffer_created) {
		free();
		frame_buffer_created = false;
	}
	width = w;
	height = h;

	//creates the frame buffer container object
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, width);
	glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, height);
	glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_SAMPLES, 4);

	glGenRenderbuffers(1, &depth_id);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_id);

	texture = new PXL_Texture(width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, get_texture_id(), 0);

	clear(1, 1, 1, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "an error occurred when creating a frame buffer\n";
	}

	frame_buffer_created = true;

	//bind to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PXL_FrameBuffer::clear(float r, float g, float b, float a) {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
	glClearColor(r, g, b, a);
	//glClearBufferiv(GL_DRAW_FRAMEBUFFER, get_texture_id(), 0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PXL_FrameBuffer::blit(PXL_FrameBuffer* dest_frame_buffer, PXL_Rect* rect, PXL_Rect* src_rect, PXL_TextureFilter filter) {
	int draw_id = 0;
	if (dest_frame_buffer != NULL) {
		draw_id = dest_frame_buffer->get_id();
		if (rect == NULL) {
			rect->x = 0; rect->y = 0; rect->w = PXL_window_width; rect->h = PXL_window_height;
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
	glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw_id);
	glBlitFramebuffer(frame_src_rect.x, frame_src_rect.y, frame_src_rect.w, frame_src_rect.h, 
					  frame_rect.x, frame_rect.y, frame_rect.w, frame_rect.h, 
					  GL_COLOR_BUFFER_BIT, filter);
}

void PXL_FrameBuffer::bind(PXL_FrameBufferAction action) {
	glBindFramebuffer(action, id);
}

unsigned char* PXL_FrameBuffer::get_pixels() {
	if (frame_buffer_created) {
		glBindBuffer(GL_TEXTURE_2D, get_texture_id());
		unsigned char* pixels = new unsigned char[(width * height) * 4];
		glGetTexImage(GL_FRAMEBUFFER, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		return pixels;
	}
	return NULL;
}

void PXL_FrameBuffer::free() {
	if (frame_buffer_created) {
		glDeleteFramebuffers(1, &id);
		glDeleteRenderbuffers(1, &depth_id);
		delete texture;
		frame_buffer_created = false;
	}
}

PXL_FrameBuffer::~PXL_FrameBuffer() {
	free();
}