#include "PXL_VBO.h"
#include <iostream>

PXL_VBO::PXL_VBO(int data_points) {
	size = data_points;

	glGenBuffers(1, &vertex_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(PXL_VertexPoint), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	/**
	glGenBuffers(1, &index_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLubyte), index_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	**/
}

void PXL_VBO::free() {
	glDeleteBuffers(1, &vertex_id);
}

PXL_VBO::~PXL_VBO() {
	free();
}