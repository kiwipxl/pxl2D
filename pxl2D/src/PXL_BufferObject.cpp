#include "PXL_BufferObject.h"
#include <iostream>

PXL_BufferObject::PXL_BufferObject(int c_buffer_size) {
	buffer_size = c_buffer_size;

	vertex_data = new PXL_VertexPoint[buffer_size];
	index_data = new GLuint[buffer_size] {0, 1, 2, 3};

	glGenBuffers(1, &vertex_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
	glBufferData(GL_ARRAY_BUFFER, buffer_size * sizeof(PXL_VertexPoint), vertex_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	glGenBuffers(1, &index_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_size * sizeof(GLuint), index_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}

void PXL_BufferObject::free() {
	glDeleteBuffers(1, &vertex_id);
	glDeleteBuffers(1, &index_id);
}

PXL_BufferObject::~PXL_BufferObject() {
	free();
}