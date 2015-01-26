#include "PXL_Matrix4.h"
#include <iostream>

PXL_Matrix4::PXL_Matrix4() {
	mat = new GLfloat[16];
	identity();
}

PXL_Matrix4& PXL_Matrix4::identity() {
	for (int i = 0; i < 16; ++i) {
		mat[i] = 0;
		if (i % 5 == 0) {
			mat[i] = 1;
		}
	}
	position.x = position.y = position.z = 0;
	rotation.x = rotation.y = rotation.z = 0;
	scaled.x = scaled.y = scaled.z = 1;

	return *this;
}

PXL_Matrix4& PXL_Matrix4::translate(float x, float y, float z) {
	if (x != 0) { translate_x(x); }
	if (y != 0) { translate_y(y); }
	if (z != 0) { translate_z(z); }
	return *this;
}

PXL_Matrix4& PXL_Matrix4::translate_x(float x) {
	mat[3] += x;
	//mat[12] += x;
	//mat[13] += x;
	//mat[14] += x;
	position.x += x;

	return *this;
}

PXL_Matrix4& PXL_Matrix4::translate_y(float y) {
	mat[7] += y;
	//mat[12] += mat[4] * y;
	//mat[13] += mat[5] * y;
	//mat[14] += mat[6] * y;
	position.y += y;

	return *this;
}

PXL_Matrix4& PXL_Matrix4::translate_z(float z) {
	mat[11] += z;
	position.z += z;

	return *this;
}

PXL_Matrix4& PXL_Matrix4::rotate(float x, float y, float z) {
	if (x != 0) { rotate_x(x); }
	if (y != 0) { rotate_y(y); }
	if (z != 0) { rotate_z(z); }

	return *this;
}

PXL_Matrix4& PXL_Matrix4::rotate_x(float angle) {
	//convert angle to radians
	angle = angle / (180.0f / 3.14f);
	rotation.x += angle;

	float cos_a = cos(rotation.x) * scaled.x; float sin_a = sin(rotation.x) * scaled.y;
	float x = mat[3]; float y = mat[7];

	mat[5] = cos_a;
	mat[6] = -sin_a;
	mat[9] = 0;
	mat[10] = cos_a;

	mat[7] = cos_a * position.y - sin_a * position.z;
	//mat[11] = sin_a * position.y + cos_a * position.z;

	return *this;
}

PXL_Matrix4& PXL_Matrix4::rotate_y(float angle) {
	//convert angle to radians
	angle = angle / (180.0f / 3.14f);
	rotation.y += angle;

	float cos_a = cos(rotation.y) * scaled.x; float sin_a = sin(rotation.y) * scaled.y;
	float x = mat[3]; float y = mat[7];

	mat[0] = cos_a;
	mat[2] = sin_a;
	mat[8] = 0;
	mat[10] = cos_a;

	mat[3] = cos_a * position.x + sin_a * position.z;
	//mat[11] = -sin_a * position.x + cos_a * position.z;

	return *this;
}

PXL_Matrix4& PXL_Matrix4::rotate_z(float angle) {
	//convert angle to radians
	angle = angle / (180.0f / 3.14f);
	rotation.z += angle;

	float cos_a = cos(rotation.z) * scaled.x; float sin_a = sin(rotation.z) * scaled.y;
	float x = mat[3]; float y = mat[7];

	mat[0] = cos_a;
	mat[1] = -sin_a;
	mat[4] = sin_a;
	mat[5] = cos_a;

	mat[3] = cos_a * position.x - sin_a * position.y;
	mat[7] = sin_a * position.x + cos_a * position.y;

	return *this;
}

PXL_Matrix4& PXL_Matrix4::scale(float x, float y, float z) {
	if (x != 1) { scale_x(x); }
	if (y != 1) { scale_y(y); }
	if (z != 1) { scale_z(z); }

	return *this;
}

PXL_Matrix4& PXL_Matrix4::scale_x(float scale) {
	mat[0] *= scale;
	mat[1] *= scale;
	mat[2] *= scale;
	mat[3] *= scale;
	scaled.x *= scale;

	return *this;
}

PXL_Matrix4& PXL_Matrix4::scale_y(float scale) {
	mat[4] *= scale;
	mat[5] *= scale;
	mat[6] *= scale;
	mat[7] *= scale;
	scaled.y *= scale;

	return *this;
}

PXL_Matrix4& PXL_Matrix4::scale_z(float scale) {
	mat[8] *= scale;
	mat[9] *= scale;
	mat[10] *= scale;
	mat[11] *= scale;
	scaled.z *= scale;

	return *this;
}

PXL_Matrix4& PXL_Matrix4::multiply(const PXL_Matrix4& m) {
	PXL_Matrix4* t = new PXL_Matrix4();

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

PXL_Matrix4* PXL_Matrix4::operator*(const PXL_Matrix4* m) {
	return &multiply(*m);
}

PXL_Matrix4& PXL_Matrix4::operator*(const PXL_Matrix4& m) {
	return multiply(m);
}

GLfloat& PXL_Matrix4::operator[](const int index) {
	return mat[index];
}

PXL_Matrix4::~PXL_Matrix4() {
	delete[] mat;
}