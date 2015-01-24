#include "PXL_Lights.h"
#include <fstream>
#include <algorithm>
#include "PXL_Batch.h"

std::vector<PXL_PointLight*> point_lights_vec;
GLfloat* point_lights_arr;
PXL_ShaderProgram* point_light_shader;
const int max_point_lights = 72;

void PXL_lights_init() {
	point_light_shader = PXL_load_glsl_shader("assets/point_light.glsl");
	point_lights_arr = new GLfloat[max_point_lights * 7];
}

PXL_PointLight* PXL_create_point_light(int x, int y, float radius, float intensity, float r, float g, float b) {
	if (point_lights_vec.size() < max_point_lights) {
		PXL_PointLight* light = new PXL_PointLight();
		light->x = x; light->y = y;
		light->radius = radius; light->intensity = intensity;
		light->r = r; light->g = g; light->b = b;
		point_lights_vec.push_back(light);

		int index = (point_lights_vec.size() * 7) - 7;
		point_lights_arr[index] = light->x;
		point_lights_arr[index + 1] = light->y;
		point_lights_arr[index + 2] = light->radius;
		point_lights_arr[index + 3] = light->intensity;
		point_lights_arr[index + 4] = light->r;
		point_lights_arr[index + 5] = light->g;
		point_lights_arr[index + 6] = light->b;

		return light;
	}
	return NULL;
}

void PXL_render_point_lights(PXL_Batch* batch) {
	batch->set_shader(point_light_shader->get_program_id());
	glUniform1fv(glGetUniformLocation(point_light_shader->get_program_id(), "points"), point_lights_vec.size() * 7, point_lights_arr);
	glUniform1i(glGetUniformLocation(point_light_shader->get_program_id(), "points_length"), point_lights_vec.size() * 7);
}

void PXL_remove_point_light(PXL_PointLight* light, bool delete_pointer) {
	point_lights_vec.erase(remove(point_lights_vec.begin(), point_lights_vec.end(), light), point_lights_vec.end());
	if (delete_pointer) { delete light; }
}