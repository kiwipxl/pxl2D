#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include <glew.h>
#include "PXL_VertexPoint.h"

class PXL_VBO {

	public:
		PXL_VBO(int c_buffer_size = 4);
		~PXL_VBO();

		int buffer_size;
		GLuint vertex_id;
		PXL_VertexPoint* vertex_data;
		GLuint index_id;
		GLuint* index_data;

		void free();
};

#endif