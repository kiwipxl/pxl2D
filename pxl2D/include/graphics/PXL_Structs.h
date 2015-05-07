#ifndef _PXL_STRUCTS_H
#define _PXL_STRUCTS_H

struct PXL_Rect {

	PXL_Rect() { }
	PXL_Rect(float pos_x, float pos_y, float width, float height) : x(pos_x), y(pos_y), w(width), h(height) { }

	float x = 0, y = 0, w = 0, h = 0;
};

struct PXL_Vec2 {

	PXL_Vec2() { }
	PXL_Vec2(float pos_x, float pos_y) : x(pos_x), y(pos_y) { }

	float x = 0, y = 0;
};

struct PXL_Vec3 {

	PXL_Vec3() { }
	PXL_Vec3(float pos_x, float pos_y, float pos_z) : x(pos_x), y(pos_y), z(pos_z) { }

	float x = 0, y = 0, z = 0;
};

#endif