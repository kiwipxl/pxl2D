#include "PXL_Lights.h"
#include <fstream>
#include <algorithm>
#include "PXL_Batch.h"
#include "PXL_FrameBuffer.h"
#include "system/PXL_Exception.h"
#include "system/PXL_Window.h"

std::vector<PXL_PointLight*> PXL_point_lights;
int PXL_max_point_lights = 72;
std::vector<GLfloat> point_lights_arr;
PXL_ShaderProgram* point_light_shader;
PXL_FrameBuffer* frame_buffer;

void PXL_lights_init() {
	point_light_shader = PXL_load_glsl_shader("assets/point_light.glsl");
	frame_buffer = new PXL_FrameBuffer();
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

	batch->set_shader(point_light_shader->get_program_id());
	glUniform1fv(glGetUniformLocation(point_light_shader->get_program_id(), "points"), PXL_point_lights.size() * 7, &point_lights_arr[0]);
	glUniform1i(glGetUniformLocation(point_light_shader->get_program_id(), "points_length"), PXL_point_lights.size() * 7);

	PXL_Rect rect;
	rect.x = 0; rect.y = 0; rect.w = PXL_window_width; rect.h = PXL_window_height;
	batch->add(frame_buffer->get_texture(), &rect, NULL);
	batch->render_all();
}

void PXL_remove_point_light(PXL_PointLight* light, bool delete_pointer) {
	PXL_point_lights.erase(remove(PXL_point_lights.begin(), PXL_point_lights.end(), light), PXL_point_lights.end());
	if (delete_pointer) { delete light; }
}