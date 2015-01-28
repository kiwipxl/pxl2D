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

struct PXL_UVCoord {

	unsigned short x = 0, y = 0;
};

struct PXL_RGBA {

	unsigned char r = 255, g = 255, b = 255, a = 255;
};

struct PXL_VertexPoint {

	PXL_Vec2 pos;
	PXL_UVCoord uv;
	PXL_RGBA colour;
};

#endif