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
        \*brief: adds another matrix4 (b) by this matrix and returns a temporary new result
        \*param [b]: constant non-pointer matrix4 reference
        **/
        PXL_Matrix4& add(const PXL_Matrix4& b);

        /**
        \*brief: adds this matrix by a float and returns a temporary new result
        \*param [b]: float to be added by
        **/
        PXL_Matrix4& add(float b);

        /**
        \*brief: subs another matrix4 (b) by this matrix and returns a temporary new result
        \*param [b]: constant non-pointer matrix4 reference
        **/
        PXL_Matrix4& sub(const PXL_Matrix4& b);

        /**
        \*brief: subs this matrix by a float and returns a temporary new result
        \*param [b]: float to be subtracted by
        **/
        PXL_Matrix4& sub(float b);

		/**
		\*brief: multiplies another matrix4 (b) by this matrix and returns a temporary new result
		\*param [b]: constant non-pointer matrix4 reference
		**/
        PXL_Matrix4& mul(const PXL_Matrix4& b);

        /**
        \*brief: multiplies this matrix by a float and returns a temporary new result
        \*param [b]: float to be multiplied by
        **/
        PXL_Matrix4& mul(float b);

        /**
        \*brief: clones this matrix4 and returns a new temporary result
        **/
        PXL_Matrix4 clone();

		/**
		\*brief: returns the raw internal matrix data
		**/
		PXL_float* get_raw_matrix() { return mat; }

		/**
		\*brief: sets the internal matrix data to the new specified raw matrix
		\*param [raw_matrix]: raw 4x4 matrix data array
		**/
        void set_raw_matrix(PXL_float* raw_matrix);

        /**
        \*brief: overrides the equal operator, sets this matrix to the operand and return this matrix
        \*param [b]: matrix to set equal to
        **/
        PXL_Matrix4& operator=(PXL_Matrix4& b);

        /**
        \*brief: overrides the addition operator and returns a temporary added matrix result
        \*param [b]: matrix to be added by
        **/
        PXL_Matrix4& operator+(const PXL_Matrix4& b) { return add(b); }

        /**
        \*brief: overrides the addition operator and returns a temporary added matrix result by a float
        \*param [b]: float value to be added by
        **/
        PXL_Matrix4& operator+(float b) { return add(b); }

        /**
        \*brief: overrides the subtraction operator and returns a temporary added matrix result
        \*param [b]: matrix to be subtracted by
        **/
        PXL_Matrix4& operator-(const PXL_Matrix4& b) { return sub(b); }

        /**
        \*brief: overrides the addition operator and returns a temporary added matrix result by a float
        \*param [b]: float value to be subtracted by
        **/
        PXL_Matrix4& operator-(float b) { return sub(b); }

		/**
		\*brief: overrides the multiplication operator and returns a temporary multiplied matrix result
		\*param [b]: matrix to be multiplied by
		**/
        PXL_Matrix4& operator*(const PXL_Matrix4& b) { return mul(b); }

        /**
        \*brief: overrides the multiplication operator and returns a temporary multiplied matrix result by a float
        \*param [b]: float value to be multiplied by
        **/
        PXL_Matrix4& operator*(float b) { return mul(b); }

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

        /**
        \*brief: updates private position, rotation and scaled vectors according to the matrix raw data
        **/
        void update_transform_vectors();
};

#endif