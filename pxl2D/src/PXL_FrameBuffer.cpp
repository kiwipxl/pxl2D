#include "PXL_FrameBuffer.h"
#include <iostream>
#include "PXL_Window.h"

PXL_FrameBuffer::PXL_FrameBuffer() {
	frame_buffer_created = false;
	create_frame_buffer();
}

PXL_FrameBuffer* PXL_create_frame_buffer() {
	return new PXL_FrameBuffer();
}

void PXL_FrameBuffer::create_frame_buffer() {
	//if the frame_buffer is already created then delete the frame_buffer but not the buffer
	if (frame_buffer_created) {
		glDeleteFramebuffers(1, &id);
		frame_buffer_created = false;
	}

	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, PXL_window_width);
	glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, PXL_window_height);
	glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_SAMPLES, 4);
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);

	frame_buffer_created = true;
}

unsigned char* PXL_FrameBuffer::get_pixels() {
	if (frame_buffer_created) {
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		unsigned char* pixels = new unsigned char[(width * height) * 4];
		//glGetTexImage(GL_FRAMEBUFFER, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		return &pixels[0];
	}
	return NULL;
}

void PXL_FrameBuffer::free() {
	if (frame_buffer_created) {
		glDeleteFramebuffers(1, &id);
		frame_buffer_created = false;
	}
}

PXL_FrameBuffer::~PXL_FrameBuffer() {
	free();
}