#ifndef VERTEX_POINT_H
#define VERTEX_POINT_H

#include "PXL_Vec2.h"
#include "PXL_Vec4.h"

struct PXL_VertexPoint {

	PXL_Vec2 pos;
	PXL_Vec2 uv;
	PXL_Vec4 colour;
};

#endif