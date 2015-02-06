#ifndef _PXL_LIGHTS_H
#define _PXL_LIGHTS_H

#include <iostream>
#include <vector>
#include "PXL_ShaderUtils.h"

struct PXL_PointLight {

	float x, y;
	float radius, intensity;
	float r, g, b;
};

extern std::vector<PXL_PointLight*> PXL_point_lights;
static int PXL_max_point_lights = 72;

extern void PXL_lights_init();
extern PXL_PointLight* PXL_create_point_light(int x, int y, float radius, float intensity, float r, float g, float b);
extern void PXL_render_point_lights(PXL_Batch* batch, int z_depth = 0);
extern void PXL_remove_point_light(PXL_PointLight* light, bool delete_pointer = true);
extern void PXL_set_point_light_config(float max_alpha = 1);

#endif