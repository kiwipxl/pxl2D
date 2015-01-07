#ifndef MATRIX_4_H
#define MATRIX_4_H

#include <SDL.h>
#include <glew.h>
#include "../../tools/Point2DF.h"

class Matrix4 {

	public:
		Matrix4();
		~Matrix4();

		void identity();

		void translate(float x, float y);
		void set_position(float x, float y);
		Point2DF get_position() { return position; }

		void rotate(float degrees);
		void set_rotation(float angle);
		float get_rotation() { return rotation; }

		void scale(float scale_x, float scale_y);
		void set_scale(float scale_x, float scale_y);
		Point2DF get_scale() { return scaled; }

		GLfloat* get_mat() { return mat; }
		GLfloat*& get_mat_reference() { return mat; }
		
		Matrix4* operator*(const Matrix4* m);
		Matrix4& operator*(const Matrix4& m);
		GLfloat& operator[](int index);

	private:
		GLfloat* mat;
		Point2DF position;
		float rotation;
		Point2DF scaled;
};

#endif