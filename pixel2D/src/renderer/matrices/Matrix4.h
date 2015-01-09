#ifndef MATRIX_4_H
#define MATRIX_4_H

#include <SDL.h>
#include <glew.h>
#include "../../tools/Point3Df.h"

class Matrix4 {

	public:
		Matrix4();
		~Matrix4();

		Matrix4& identity();

		Matrix4& translate(float x = 0, float y = 0, float z = 0);
		Matrix4& set_position(float x = 0, float y = 0, float z = 0);
		Point3DF get_position() {
			position.x = mat[3]; position.y = mat[7]; position.z = mat[11];
			return position;
		}

		Matrix4& rotate_x(float angle);
		Matrix4& rotate_y(float angle);
		Matrix4& rotate_z(float angle);
		Matrix4& set_rotation_x(float angle);
		Matrix4& set_rotation_y(float angle);
		Matrix4& set_rotation_z(float angle);
		Point3DF get_rotation() { return rotation; }

		Matrix4& scale(float scale_x = 0, float scale_y = 0, float scale_z = 0);
		Matrix4& set_scale(float scale_x = 1, float scale_y = 1, float scale_z = 1);
		Point3DF get_scale() {
			scaled.x = mat[0]; scaled.y = mat[5]; scaled.z = mat[10];
			return scaled;
		}

		Matrix4& multiply(const Matrix4& m);

		GLfloat* get_mat() { return mat; }
		GLfloat*& get_mat_reference() { return mat; }
		void set_mat(GLfloat* new_mat) { mat = new_mat; }

		Matrix4* operator*(const Matrix4* m);
		Matrix4& operator*(const Matrix4& m);
		GLfloat& operator[](int index);

	private:
		GLfloat* mat;
		Point3DF position;
		Point3DF rotation;
		Point3DF scaled;
};

#endif