#include "PXL_ShaderUtils.h"
#include <fstream>
#include "PXL_Batch.h"

//defines
const char* start_v_header = "#START_VERTEX";
const char* end_v_header = "#END_VERTEX";
const char* start_f_header = "#START_FRAGMENT";
const char* end_f_header = "#END_FRAGMENT";

void log_shader_error(string e);

PXL_ShaderProgram* PXL_default_shader;
PXL_ShaderProgram* PXL_bloom_shader;
PXL_ShaderProgram* PXL_repeat_shader;
PXL_ShaderProgram* PXL_grayscale_shader;
PXL_ShaderProgram* PXL_blur_shader;
PXL_ShaderProgram* PXL_outline_shader;
PXL_ShaderProgram* PXL_outer_glow_shader;

void PXL_shader_init() {
	//setup premade pxl glsl shaders
	PXL_default_shader = PXL_load_glsl_shader("assets/default.glsl");
	PXL_bloom_shader = PXL_load_glsl_shader("assets/bloom.glsl");
	PXL_repeat_shader = PXL_load_glsl_shader("assets/repeat.glsl");
	PXL_grayscale_shader = PXL_load_glsl_shader("assets/grayscale.glsl");
	PXL_blur_shader = PXL_load_glsl_shader("assets/blur.glsl");
	PXL_outline_shader = PXL_load_glsl_shader("assets/outline.glsl");
	PXL_outer_glow_shader = PXL_load_glsl_shader("assets/outer_glow.glsl");
}

void PXL_use_default_shader(PXL_Batch* batch) {
	batch->set_shader(PXL_default_shader);
}

void PXL_use_bloom_shader(PXL_Batch* batch, float spread, float intensity) {
	batch->set_shader(PXL_bloom_shader);
	glUniform1f(glGetUniformLocation(PXL_bloom_shader->get_program_id(), "outline_spread"), spread);
	glUniform1f(glGetUniformLocation(PXL_bloom_shader->get_program_id(), "outline_intensity"), intensity);
}

void PXL_use_repeat_shader(PXL_Batch* batch, float repeat_x, float repeat_y) {
	batch->set_shader(PXL_repeat_shader);
	glUniform2f(glGetUniformLocation(PXL_repeat_shader->get_program_id(), "repeat"), repeat_x, repeat_y);
}

void PXL_use_grayscale_shader(PXL_Batch* batch) {
	batch->set_shader(PXL_grayscale_shader);
}

void PXL_use_blur_shader(PXL_Batch* batch, float spread_x, float spread_y) {
	batch->set_shader(PXL_blur_shader);
	glUniform2f(glGetUniformLocation(PXL_blur_shader->get_program_id(), "outline_size"), spread_x, spread_y);
}

void PXL_use_outline_shader(PXL_Batch* batch, float thickness, float r, float g, float b, float a, float threshold) {
	batch->set_shader(PXL_outline_shader);
	glUniform1f(glGetUniformLocation(PXL_outline_shader->get_program_id(), "outline_thickness"), thickness);
	glUniform4f(glGetUniformLocation(PXL_outline_shader->get_program_id(), "outline_colour"), r, g, b, a);
	glUniform1f(glGetUniformLocation(PXL_outline_shader->get_program_id(), "outline_threshold"), threshold);
}

void PXL_use_outer_glow_shader(PXL_Batch* batch, float size, float r, float g, float b, float intensity, float threshold) {
	batch->set_shader(PXL_outer_glow_shader);
	glUniform1f(glGetUniformLocation(PXL_outline_shader->get_program_id(), "outline_size"), size);
	glUniform3f(glGetUniformLocation(PXL_outline_shader->get_program_id(), "outline_colour"), r, g, b);
	glUniform1f(glGetUniformLocation(PXL_outline_shader->get_program_id(), "outline_threshold"), threshold);
	glUniform1f(glGetUniformLocation(PXL_outline_shader->get_program_id(), "outline_intensity"), intensity);
}

PXL_ShaderProgram* PXL_load_shader(string vertex_file, string fragment_file) {
	return new PXL_ShaderProgram(PXL_load_file(vertex_file), PXL_load_file(fragment_file), vertex_file, fragment_file);
}

PXL_ShaderProgram* PXL_load_glsl_shader(string glsl_file) {
	string s = PXL_load_file(glsl_file);

	int start_v = s.find(start_v_header);
	int end_v = s.find(end_v_header);
	int start_f = s.find(start_f_header);
	int end_f = s.find(end_f_header);

	if (start_v != -1 && end_v != -1 && start_f != -1 && end_f != -1) {
		string vertex_file = s.substr(start_v + strlen(start_v_header) + 1, end_v - (start_v + strlen(start_v_header) + 1));
		string fragment_file = s.substr(start_f + strlen(start_f_header) + 1, end_f - (start_f + strlen(start_f_header) + 1));
		return new PXL_ShaderProgram(vertex_file, fragment_file, glsl_file + " - vertex", glsl_file + " - fragment");
	}else {
		log_shader_error("headers not found. start the vertex shader with #START_VERTEX and end with #END_VERTEX and for fragment too");
	}
	return new PXL_ShaderProgram("", "");
}

string PXL_load_file(string file_name) {
	ifstream file(file_name, ifstream::in);
	if (file) {
		file.ignore(numeric_limits<streamsize>::max());
		streamsize size = file.gcount();
		file.seekg(0, ifstream::beg);

		if (size >= 0) {
			char* buffer = new char[size];
			file.read(buffer, size);

			file.close();
			if (buffer) {
				buffer[size] = '\0';
				return buffer;
			}else {
				log_shader_error("(" + file_name + ") could not be read successfully");
				delete[] buffer;
			}
		}else {
			log_shader_error("size of (" + file_name + ") is less than zero");
		}
	}else {
		log_shader_error("couldn't load shader file (" + file_name + "), may not exist");
	}
	return "";
}

void log_shader_error(string e) {
	cout << "[shader log error]: " << e << "\n";
}