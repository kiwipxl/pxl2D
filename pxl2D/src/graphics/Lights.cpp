#include "graphics/Lights.h"
#include <fstream>
#include <algorithm>
#include "graphics/Batch.h"
#include "system/Exception.h"
#include "system/Window.h"

namespace pxl { namespace graphics {

    std::vector<PointLight*> point_lights;
    std::vector<GLfloat> point_lights_arr;
    Texture* screen_texture = new Texture();

    const void lights_init() {
	    screen_texture->create_texture(1024, 768, NULL, CHANNEL_RGBA);

	    glUseProgram(point_light_shader->get_program_id());
	    point_light_shader->add_uniform_location("points");
	    point_light_shader->add_uniform_location("points_length");
	    point_light_shader->add_uniform_location("max_alpha");

	    set_point_light_config(1);
    }

    PointLight* create_point_light(int x, int y, float radius, float intensity, float r, float g, float b) {
	    if (point_lights.size() < max_point_lights) {
		    PointLight* light = new PointLight();
		    light->x = x; light->y = y;
		    light->radius = radius; light->intensity = intensity;
		    light->r = r; light->g = g; light->b = b;
		    point_lights.push_back(light);

		    point_lights_arr.push_back(light->x);
		    point_lights_arr.push_back(light->y);
		    point_lights_arr.push_back(light->radius);
		    point_lights_arr.push_back(light->intensity);
		    point_lights_arr.push_back(light->r);
		    point_lights_arr.push_back(light->g);
		    point_lights_arr.push_back(light->b);

		    glUseProgram(point_light_shader->get_program_id());
		    glUniform1i(point_light_shader->get_uniform_location(1), point_lights.size() * 7);
		    glUseProgram(0);

            int a = math::wrap(40, 0, 10);
		    int b = math::wrap(-20, 0, 100);
		    int c = math::wrap(40, 20, 25);

		    return light;
	    }else {
		    //todo: std::to_string not supported by android
		    //show_exception("Cannot create more than " + std::to_string(max_point_lights) + " lights");
	    }
	    return NULL;
    }

    const void render_point_lights(Batch* batch, int z_depth) {
	    int index = 0;
	    for (int n = 0; n < point_lights.size(); ++n) {
		    PointLight* light = point_lights[n];
		    point_lights_arr[index] = light->x;
		    point_lights_arr[index + 1] = light->y;
		    point_lights_arr[index + 2] = light->radius;
		    point_lights_arr[index + 3] = light->intensity;
		    point_lights_arr[index + 4] = light->r;
		    point_lights_arr[index + 5] = light->g;
		    point_lights_arr[index + 6] = light->b;
		    index += 7;
	    }

	    glUseProgram(point_light_shader->get_program_id());
	    glUniform1fv(point_light_shader->get_uniform_location(0), point_lights.size() * 7, &point_lights_arr[0]);

	    Rect rect;
	    rect.x = 0; rect.y = 0; rect.w = 1024; rect.h = 768;
	    Rect src_rect;
	    src_rect.x = 0; src_rect.y = 0; src_rect.w = 1024; src_rect.h = 768;
	    batch->add(*screen_texture, &rect, &src_rect, NULL, NULL, NULL, z_depth, COLOUR_WHITE, point_light_shader);
    }

    const void remove_point_light(PointLight* light, bool delete_pointer) {
	    //todo: vector erasing maybe not supported by android port
	    //point_lights.erase(remove(point_lights.begin(), point_lights.end(), light), point_lights.end());

	    glUseProgram(point_light_shader->get_program_id());
	    glUniform1i(point_light_shader->get_uniform_location(1), point_lights.size() * 7);
	    glUseProgram(0);

	    if (delete_pointer) { delete light; }
    }

    const void set_point_light_config(float max_alpha) {
	    glUseProgram(point_light_shader->get_program_id());
	    glUniform1f(point_light_shader->get_uniform_location(2), max_alpha);
	    glUseProgram(0);
    }

}};