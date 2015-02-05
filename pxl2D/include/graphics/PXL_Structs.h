#ifndef STRUCTS_H
#define STRUCTS_H

struct PXL_Rect {

	float x = 0, y = 0, w = 0, h = 0;
};

struct PXL_Vec2 {

	float x = 0, y = 0;
};

struct PXL_Vec3 {

	float x = 0, y = 0, z = 0;
};

struct PXL_Vec4 {

	float x = 0, y = 0, z = 0, w = 0;
};

struct PXL_RGBA {

	float r = 1, g = 1, b = 1, a = 1;
};

#endif