#ifndef LIGHTS_H
#define LIGHTS_H

#include <iostream>
#include <vector>
#include "PXL_ShaderUtils.h"

using namespace std;

struct PXL_PointLight {

	int x;
	int y;
	float radius;
	float intensity;
	float r;
	float g;
	float b;
};

extern void PXL_lights_init();
extern PXL_PointLight* PXL_create_point_light(int x, int y, float radius, float intensity, float r, float g, float b);
extern void PXL_render_point_lights(PXL_Batch* batch);
extern void PXL_remove_point_light(PXL_PointLight* light, bool delete_pointer = true);

#endif