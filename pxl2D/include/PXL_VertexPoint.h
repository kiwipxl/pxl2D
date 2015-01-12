#ifndef VERTEX_POINT_H
#define VERTEX_POINT_H

#include "PXL_RGBA.h"
#include "PXL_Vec2.h"

struct PXL_VertexPos {
	
	signed short x = 0;
	signed short y = 0;
};

struct PXL_UVCoord {

	unsigned short x = 0;
	unsigned short y = 0;
};

struct PXL_VertexPoint {
	
	PXL_VertexPos pos;
	PXL_UVCoord uv;
	PXL_RGBA colour;
};

#endif