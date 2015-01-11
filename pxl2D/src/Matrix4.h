#ifndef MATRIX_4_H
#define MATRIX_4_H

#include <SDL.h>
#include <glew.h>
#include "PXL_Vec3.h"

class Matrix4 {

	public:
		Matrix4();
		~Matrix4();

		Matrix4& identity();

		Matrix4& translate(float x = 0, float y = 0, float z = 0);
		Matrix4& translate_x(float x);
		Matrix4& translate_y(float y);
		Matrix4& translate_z(float z);
		PXL_Vec3 get_position() {
			position.x = mat[3]; position.y = mat[7]; position.z = mat[11];
			return position;
		}

		Matrix4& rotate(float x = 0, float y = 0, float z = 0);
		Matrix4& rotate_x(float angle);
		Matrix4& rotate_y(float angle);
		Matrix4& rotate_z(float angle);
		PXL_Vec3 get_rotation() { return rotation; }

		Matrix4& scale(float x = 1, float y = 1, float z = 1);
		Matrix4& scale_x(float scale);
		Matrix4& scale_y(float scale);
		Matrix4& scale_z(float scale);
		PXL_Vec3 get_scale() {
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
		PXL_Vec3 position;
		PXL_Vec3 rotation;
		PXL_Vec3 scaled;
};

#endif