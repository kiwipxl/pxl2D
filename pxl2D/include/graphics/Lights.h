#ifndef _LIGHTS_H
#define _LIGHTS_H

#include <iostream>
#include <vector>
#include "graphics/ShaderUtils.h"
#include "API.h"

namespace pxl { namespace graphics {

    struct PointLight {

	    float x, y;
	    float radius, intensity;
	    float r, g, b;
    };

    extern std::vector<PointLight*> point_lights;
    static uint32 max_point_lights = 72;

    extern const void lights_init();
    extern PointLight* create_point_light(int x, int y, float radius, float intensity, float r, float g, float b);
    extern const void render_point_lights(Batch* batch, int z_depth = 0);
    extern const void remove_point_light(PointLight* light, bool delete_pointer = true);
    extern const void set_point_light_config(float max_alpha = 1);

}};

#endif