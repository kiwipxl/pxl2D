#include "Matrix4.h"
#include <iostream>

Matrix4::Matrix4() {
	mat = new GLfloat[16];
	identity();
}

Matrix4& Matrix4::identity() {
	for (int i = 0; i < 16; ++i) {
		mat[i] = 0;
		if (i % 5 == 0) {
			mat[i] = 1;
		}
	}
	set_position(0, 0, 0);
	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;
	set_scale(1, 1, 1);

	return *this;
}

Matrix4& Matrix4::translate(float x, float y, float z) {
	mat[3] += x;
	mat[7] += y;
	mat[11] += z;

	position.x += x;
	position.y += y;
	position.z += z;

	return *this;
}

Matrix4& Matrix4::set_position(float x, float y, float z) {
	mat[3] = x;
	mat[7] = y;
	mat[11] = z;

	position.x = x;
	position.y = y;
	position.z = z;

	return *this;
}

Matrix4& Matrix4::rotate_x(float angle) {
	rotation.x += angle;
	return set_rotation_x(rotation.x);
}

Matrix4& Matrix4::set_rotation_x(float angle) {
	rotation.x = angle;

	//convert angle to radians
	angle = angle / (180.0f / 3.14f);

	//calculate rotation on x axis
	mat[5] = cos(angle);
	mat[6] = -sin(angle);
	mat[9] = sin(angle);
	mat[10] = cos(angle);

	return *this;
}

Matrix4& Matrix4::rotate_y(float angle) {
	rotation.y += angle;
	return set_rotation_y(rotation.y);
}

Matrix4& Matrix4::set_rotation_y(float angle) {
	rotation.y = angle;

	//convert angle to radians
	angle = angle / (180.0f / 3.14f);

	//calculate rotation on y axis
	mat[0] = cos(angle);
	mat[2] = sin(angle);
	mat[8] = -sin(angle);
	mat[10] = cos(angle);

	return *this;
}

Matrix4& Matrix4::rotate_z(float angle) {
	rotation.z += angle;
	set_rotation_z(rotation.z);

	return *this;
}

Matrix4& Matrix4::set_rotation_z(float angle) {
	rotation.z = angle;

	//convert angle to radians
	angle = angle / (180.0f / 3.14f);

	//calculate rotation on z axis
	mat[0] = cos(angle) * scaled.x;
	mat[1] = -sin(angle) * scaled.y;
	mat[4] = sin(angle) * scaled.x;
	mat[5] = cos(angle) * scaled.y;

	//calculate origin from position
	mat[3] = (mat[0] * position.x) + (mat[1] * position.y);
	mat[7] = (mat[4] * position.x) + (mat[5] * position.y);

	return *this;
}

Matrix4& Matrix4::scale(float scale_x, float scale_y, float scale_z) {
	scaled.x += scale_x; scaled.y += scale_y; scaled.z += scale_z;

	mat[0] = mat[0] * scaled.x;
	mat[5] = mat[5] * scaled.y;
	mat[10] = mat[10] * scaled.z;

	mat[1] = mat[1] * scaled.y;
	mat[4] = mat[4] * scaled.x;

	return *this;
}

Matrix4& Matrix4::set_scale(float scale_x, float scale_y, float scale_z) {
	mat[0] = scale_x;
	mat[5] = scale_y;
	mat[10] = scale_z;

	scaled.x = scale_x; scaled.y = scale_y; scaled.z = scale_z;

	return *this;
}

Matrix4& Matrix4::multiply(const Matrix4& m) {
	Matrix4* t = new Matrix4();
	float sum = 0;
	for (int y = 0; y < 16; ++y) {
		for (int x = 0; x < 4; ++x) {
			sum += mat[(4 * x) + (y % 4)] * m.mat[x + (int(y / 4) * 4)];
		}
		(*t)[y] = sum;
		sum = 0;
	}
	t->position = t->get_position();
	t->rotation = t->get_rotation();
	t->scaled = t->get_scale();

	return *t;
}

Matrix4* Matrix4::operator*(const Matrix4* m) {
	return &multiply(*m);
}

Matrix4& Matrix4::operator*(const Matrix4& m) {
	return multiply(m);
}

GLfloat& Matrix4::operator[](const int index) {
	return mat[index];
}

Matrix4::~Matrix4() {
	delete[] mat;
}