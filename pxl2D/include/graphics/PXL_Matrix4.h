#ifndef _PXL_MATRIX_4_H
#define _PXL_MATRIX_4_H

#include "graphics/PXL_Structs.h"
#include "system/PXL_Math.h"
#include "system/PXL_API.h"

class PXL_Matrix4 {

	public:
		PXL_Matrix4();
		~PXL_Matrix4();

		/**
		\*brief: sets the matrix to it's identity
		**/
		PXL_Matrix4& identity();

		/**
		\*brief: sets the matrix to it's identity
		**/
		PXL_Matrix4& transpose();

		/**
		\*brief: translates the matrix position
		\*param [x, y, z]: values to translate on axis
		**/
		PXL_Matrix4& translate(float x = 0, float y = 0, float z = 0);
		/**
		\*brief: translates the matrix position on the x axis
		\*param [x]: translate value
		**/
		PXL_Matrix4& translate_x(float x);
		/**
		\*brief: translates the matrix position on the y axis
		\*param [y]: translate value
		**/
		PXL_Matrix4& translate_y(float y);
		/**
		\*brief: translates the matrix position on the z axis
		\*param [z]: translate value
		**/
		PXL_Matrix4& translate_z(float z);
		/**
		\*brief: returns a vec3 of the position in the matrix
		**/
		PXL_Vec3 get_position() {
			position.x = mat[3]; position.y = mat[7]; position.z = mat[11];
			return position;
		}

		/**
		\*brief: rotates the matrix on the x, y, z axis
		\*param [x, y, z]: values to rotate
		**/
		PXL_Matrix4& rotate(float x = 0, float y = 0, float z = 0);
		/**
		\*brief: rotates the matrix on the x axis
		\*param [angle]: value to rotate in degrees
		**/
		PXL_Matrix4& rotate_x(float angle);
		/**
		\*brief: rotates the matrix on the y axis
		\*param [angle]: value to rotate in degrees
		**/
		PXL_Matrix4& rotate_y(float angle);
		/**
		\*brief: rotates the matrix on the z axis
		\*param [angle]: value to rotate in degrees
		**/
		PXL_Matrix4& rotate_z(float angle);
		/**
		\*brief: returns a vec3 rotation in the matrix
		**/
		PXL_Vec3 get_rotation() { return rotation; }

		/**
		\*brief: scales the matrix on the x, y, z axis
		\*param [x, y, z]: values to scale
		**/
		PXL_Matrix4& scale(float x = 1, float y = 1, float z = 1);
		/**
		\*brief: scales the matrix on the x axis
		\*param [scale]: value to be multiplied on x axis
		**/
		PXL_Matrix4& scale_x(float scale);
		/**
		\*brief: scales the matrix on the y axis
		\*param [scale]: value to be multiplied on y axis
		**/
		PXL_Matrix4& scale_y(float scale);
		/**
		\*brief: scales the matrix on the z axis
		\*param [scale]: value to be multiplied on z axis
		**/
		PXL_Matrix4& scale_z(float scale);
		/**
		\*brief: returns a vec3 of the scale in the matrix
		**/
		PXL_Vec3 get_scale() {
			scaled.x = mat[0]; scaled.y = mat[5]; scaled.z = mat[10];
			return scaled;
		}

		/**
		\*brief: multiplies another matrix4 by this matrix and return the result
		\*param [m]: constant non-pointer matrix4 reference
		**/
		PXL_Matrix4& multiply(const PXL_Matrix4& b);

		/**
		\*brief: returns a copy of the matrix
		**/
		PXL_float* get_mat() { return mat; }
		/**
		\*brief: sets the matrix data to another matrix data reference
		\*param [new_mat]: the new mat to be replaced
		**/
		//void set_mat(PXL_float* new_mat) { mat = new_mat; }

		/**
		\*brief: overrides the multiplication operator to multiply matrix pointer
		\*param [m]: matrix to be multiplied by
		**/
		PXL_Matrix4* operator*(const PXL_Matrix4* m);
		/**
		\*brief: overrides the multiplication operator to multiply matrix reference
		\*param [m]: matrix to be multiplied by
		**/
		PXL_Matrix4& operator*(const PXL_Matrix4& m);
		/**
		\*brief: overrides the [] operator that returns a reference to a matrix value
		\*param [index]: the index value of the matrix array
		**/
		PXL_float& operator[](int index);

	private:
		//matrix data
		PXL_float mat[16];

		//matrix info
		PXL_Vec3 position;
		PXL_Vec3 rotation;
		PXL_Vec3 scaled;
};

#endif