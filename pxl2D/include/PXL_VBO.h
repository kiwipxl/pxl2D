#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include <glew.h>
#include <vector>
#include "PXL_VertexPoint.h"

using namespace std;

class PXL_VBO {

	public:
		PXL_VBO(int data_points = 4);
		~PXL_VBO();

		unsigned int size;
		GLuint vertex_id;
		vector<PXL_VertexPoint> vertex_data;
		GLuint index_id;
		GLubyte* index_data;

		void free();
};

#endif