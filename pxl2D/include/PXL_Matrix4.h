#ifndef MATRIX_4_H
#define MATRIX_4_H

#include <SDL.h>
#include <glew.h>
#include "PXL_Vec3.h"

class PXL_Matrix4 {

	public:
		PXL_Matrix4();
		~PXL_Matrix4();

		PXL_Matrix4& identity();

		PXL_Matrix4& translate(float x = 0, float y = 0, float z = 0);
		PXL_Matrix4& translate_x(float x);
		PXL_Matrix4& translate_y(float y);
		PXL_Matrix4& translate_z(float z);
		PXL_Vec3 get_position() {
			position.x = mat[3]; position.y = mat[7]; position.z = mat[11];
			return position;
		}

		PXL_Matrix4& rotate(float x = 0, float y = 0, float z = 0);
		PXL_Matrix4& rotate_x(float angle);
		PXL_Matrix4& rotate_y(float angle);
		PXL_Matrix4& rotate_z(float angle);
		PXL_Vec3 get_rotation() { return rotation; }

		PXL_Matrix4& scale(float x = 1, float y = 1, float z = 1);
		PXL_Matrix4& scale_x(float scale);
		PXL_Matrix4& scale_y(float scale);
		PXL_Matrix4& scale_z(float scale);
		PXL_Vec3 get_scale() {
			scaled.x = mat[0]; scaled.y = mat[5]; scaled.z = mat[10];
			return scaled;
		}

		PXL_Matrix4& multiply(const PXL_Matrix4& m);

		GLfloat* get_mat() { return mat; }
		GLfloat*& get_mat_reference() { return mat; }
		void set_mat(GLfloat* new_mat) { mat = new_mat; }

		PXL_Matrix4* operator*(const PXL_Matrix4* m);
		PXL_Matrix4& operator*(const PXL_Matrix4& m);
		GLfloat& operator[](int index);

	private:
		GLfloat* mat;
		PXL_Vec3 position;
		PXL_Vec3 rotation;
		PXL_Vec3 scaled;
};

#endif