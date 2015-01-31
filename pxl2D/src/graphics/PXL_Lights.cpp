#include "PXL_Lights.h"
#include <fstream>
#include <algorithm>
#include "PXL_Batch.h"
#include "system/PXL_Exception.h"
#include "system/PXL_Window.h"

std::vector<PXL_PointLight*> PXL_point_lights;
std::vector<GLfloat> point_lights_arr;
PXL_Texture screen_texture;

void PXL_lights_init() {
	screen_texture = PXL_Texture(PXL_window_width, PXL_window_height);

	glUseProgram(PXL_point_light_shader->get_program_id());
	PXL_point_light_shader->add_uniform_location("points");
	PXL_point_light_shader->add_uniform_location("points_length");
	PXL_point_light_shader->add_uniform_location("max_alpha");

	PXL_set_point_light_config(1);
}

PXL_PointLight* PXL_create_point_light(int x, int y, float radius, float intensity, float r, float g, float b) {
	if (PXL_point_lights.size() < PXL_max_point_lights) {
		PXL_PointLight* light = new PXL_PointLight();
		light->x = x; light->y = y;
		light->radius = radius; light->intensity = intensity;
		light->r = r; light->g = g; light->b = b;
		PXL_point_lights.push_back(light);

		point_lights_arr.push_back(light->x);
		point_lights_arr.push_back(light->y);
		point_lights_arr.push_back(light->radius);
		point_lights_arr.push_back(light->intensity);
		point_lights_arr.push_back(light->r);
		point_lights_arr.push_back(light->g);
		point_lights_arr.push_back(light->b);

		glUseProgram(PXL_point_light_shader->get_program_id());
		glUniform1i(PXL_point_light_shader->get_uniform_location(1), PXL_point_lights.size() * 7);
		glUseProgram(0);

		return light;
	}else {
		PXL_show_exception("Cannot create more than " + std::to_string(PXL_max_point_lights) + " lights");
	}
	return NULL;
}

void PXL_render_point_lights(PXL_Batch* batch) {
	batch->render_all();

	int index = 0;
	for (PXL_PointLight* light : PXL_point_lights) {
		point_lights_arr[index] = light->x;
		point_lights_arr[index + 1] = light->y;
		point_lights_arr[index + 2] = light->radius;
		point_lights_arr[index + 3] = light->intensity;
		point_lights_arr[index + 4] = light->r;
		point_lights_arr[index + 5] = light->g;
		point_lights_arr[index + 6] = light->b;
		index += 7;
	}

	glUseProgram(PXL_point_light_shader->get_program_id());
	glUniform1fv(PXL_point_light_shader->get_uniform_location(0), PXL_point_lights.size() * 7, &point_lights_arr[0]);
	batch->set_target_shader(PXL_point_light_shader);
	PXL_Rect rect;
	rect.x = 0; rect.y = 0; rect.w = PXL_window_width; rect.h = PXL_window_height;
	batch->add(&screen_texture, &rect, NULL, PXL_FLIP_NONE);

	batch->render_all();
}

void PXL_remove_point_light(PXL_PointLight* light, bool delete_pointer) {
	PXL_point_lights.erase(remove(PXL_point_lights.begin(), PXL_point_lights.end(), light), PXL_point_lights.end());

	glUseProgram(PXL_point_light_shader->get_program_id());
	glUniform1i(PXL_point_light_shader->get_uniform_location(1), PXL_point_lights.size() * 7);
	glUseProgram(0);

	if (delete_pointer) { delete light; }
}

void PXL_set_point_light_config(float max_alpha) {
	glUseProgram(PXL_point_light_shader->get_program_id());
	glUniform1f(PXL_point_light_shader->get_uniform_location(2), max_alpha);
	glUseProgram(0);
}