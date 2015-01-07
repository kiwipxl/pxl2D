#include "Matrix4.h"
#include <iostream>

Matrix4::Matrix4() {
	mat = new GLfloat[16];
}

void Matrix4::identity() {
	for (int i = 0; i < 16; ++i) {
		mat[i] = 0;
		if (i % 5 == 0) {
			mat[i] = 1;
		}
	}
}

void Matrix4::translate(float x, float y) {
	mat[12] += x;
	mat[13] += y;
}

void Matrix4::set_position(float x, float y) {
	mat[12] = x;
	mat[13] = y;
}

void Matrix4::rotate(float degrees) {

}

void Matrix4::set_rotation(float angle) {

}

void Matrix4::scale(float scale_x, float scale_y) {

}

void Matrix4::set_scale(float scale_x, float scale_y) {
	mat[0] = scale_x;
	mat[5] = scale_y;
}

Matrix4* Matrix4::operator*(const Matrix4* m) {
	GLfloat* t = new GLfloat[16];
	float sum = 0;
	for (int y = 0; y < 16; ++y) {
		for (int x = 0; x < 4; ++x) {
			sum += mat[x + (int(y / 4) * 4)] * m->mat[(4 * x) + int(y / 4)];
		}
		t[y] = sum;
		sum = 0;
	}

	delete[] mat;
	mat = t;

	return this;
}

Matrix4& Matrix4::operator*(const Matrix4& m) {
	GLfloat* t = new GLfloat[16];
	float sum = 0;
	for (int y = 0; y < 16; ++y) {
		for (int x = 0; x < 4; ++x) {
			sum += mat[x + (int(y / 4) * 4)] * m.mat[(4 * x) + (y % 4)];
		}
		t[y] = sum;
		sum = 0;
	}

	delete[] mat;
	mat = t;

	return *this;
}

GLfloat& Matrix4::operator[](const int index) {
	return mat[index];
}

Matrix4::~Matrix4() {
	delete[] mat;
}