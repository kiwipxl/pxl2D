#ifndef VERTEX_POINT_H
#define VERTEX_POINT_H

struct PXL_VertexPos {

	float x = 0;
	float y = 0;
};

struct PXL_UVCoord {

	unsigned short x = 0;
	unsigned short y = 0;
};

struct PXL_RGBA {

	unsigned char r = 255;
	unsigned char g = 255;
	unsigned char b = 255;
	unsigned char a = 255;
};

struct PXL_VertexPoint {

	PXL_VertexPos pos;
	PXL_UVCoord uv;
	PXL_RGBA colour;
	unsigned int texture_id;
};

#endif