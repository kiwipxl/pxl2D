#include "PXL_ShaderUtils.h"
#include <fstream>
#include "graphics/PXL_Batch.h"
#include "graphics/PXL_PrebuiltShaders.h"

//defines
const char* start_v_header = "//[START_VERTEX]";
const char* end_v_header = "//[END_VERTEX]";
const char* start_f_header = "//[START_FRAGMENT]";
const char* end_f_header = "//[END_FRAGMENT]";

PXL_ShaderProgram* PXL_default_shader;
PXL_ShaderProgram* PXL_bloom_shader;
PXL_ShaderProgram* PXL_repeat_shader;
PXL_ShaderProgram* PXL_grayscale_shader;
PXL_ShaderProgram* PXL_blur_shader;
PXL_ShaderProgram* PXL_outline_shader;
PXL_ShaderProgram* PXL_glow_shader;
PXL_ShaderProgram* PXL_text_shader;
PXL_ShaderProgram* PXL_point_light_shader;

const void PXL_shader_init() {
	//setup premade pxl glsl shaders
	PXL_default_shader = PXL_create_shader(PXL_basic_vertex_shader_str, PXL_default_shader_str, "default_vert", "default_frag");
	PXL_bloom_shader = PXL_create_shader(PXL_basic_vertex_shader_str, PXL_bloom_shader_str, "bloom_vert", "bloom_frag");
	PXL_repeat_shader = PXL_create_shader(PXL_basic_vertex_shader_str, PXL_repeat_shader_str, "repeat_vert", "repeat_frag");
	PXL_grayscale_shader = PXL_create_shader(PXL_basic_vertex_shader_str, PXL_grayscale_shader_str, "grayscale_vert", "grayscale_frag");
	PXL_blur_shader = PXL_create_shader(PXL_basic_vertex_shader_str, PXL_blur_shader_str, "blur_vert", "blur_frag");
	PXL_outline_shader = PXL_create_shader(PXL_basic_vertex_shader_str, PXL_outline_shader_str, "outline_vert", "outline_frag");
	PXL_glow_shader = PXL_create_shader(PXL_basic_vertex_shader_str, PXL_glow_shader_str, "glow_vert", "glow_frag");
	PXL_text_shader = PXL_create_shader(PXL_basic_vertex_shader_str, PXL_text_shader_str, "text_vert", "text_frag");
	PXL_point_light_shader = PXL_create_shader(PXL_basic_vertex_shader_str, PXL_point_light_shader_str, "point_light_vert", "point_light_frag");
}

const void PXL_set_default_shader(PXL_Batch* batch) {
	glUseProgram(PXL_default_shader->get_program_id());
}

const void PXL_set_bloom_shader(PXL_Batch* batch, float spread, float intensity) {
	glUseProgram(PXL_bloom_shader->get_program_id());
	glUniform1f(glGetUniformLocation(PXL_bloom_shader->get_program_id(), "outline_spread"), spread);
	glUniform1f(glGetUniformLocation(PXL_bloom_shader->get_program_id(), "outline_intensity"), intensity);
}

const void PXL_set_repeat_shader(PXL_Batch* batch, float repeat_x, float repeat_y) {
	glUseProgram(PXL_repeat_shader->get_program_id());
	glUniform2f(glGetUniformLocation(PXL_repeat_shader->get_program_id(), "repeat"), repeat_x, repeat_y);
}

const void PXL_set_grayscale_shader(PXL_Batch* batch) {
	glUseProgram(PXL_grayscale_shader->get_program_id());
}

const void PXL_set_blur_shader(PXL_Batch* batch, float spread_x, float spread_y) {
	glUseProgram(PXL_blur_shader->get_program_id());
	glUniform2f(glGetUniformLocation(PXL_blur_shader->get_program_id(), "blur_size"), spread_x, spread_y);
}

const void PXL_set_outline_shader(PXL_Batch* batch, float thickness, float r, float g, float b, float a, float threshold) {
	glUseProgram(PXL_outline_shader->get_program_id());
	glUniform1f(glGetUniformLocation(PXL_outline_shader->get_program_id(), "outline_thickness"), thickness);
	glUniform4f(glGetUniformLocation(PXL_outline_shader->get_program_id(), "outline_colour"), r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	glUniform1f(glGetUniformLocation(PXL_outline_shader->get_program_id(), "outline_threshold"), threshold);
}

const void PXL_set_glow_shader(PXL_Batch* batch, float size, float r, float g, float b, float intensity, float threshold) {
	glUseProgram(PXL_glow_shader->get_program_id());
	glUniform1f(glGetUniformLocation(PXL_outline_shader->get_program_id(), "outline_size"), size);
	glUniform3f(glGetUniformLocation(PXL_outline_shader->get_program_id(), "outline_colour"), r / 255.0f, g / 255.0f, b / 255.0f);
	glUniform1f(glGetUniformLocation(PXL_outline_shader->get_program_id(), "outline_threshold"), threshold);
	glUniform1f(glGetUniformLocation(PXL_outline_shader->get_program_id(), "outline_intensity"), intensity);
}

const void PXL_set_text_shader(PXL_Batch* batch, float r, float g, float b, float a) {
	glUseProgram(PXL_text_shader->get_program_id());
	glUniform3f(glGetUniformLocation(PXL_text_shader->get_program_id(), "text_colour"), r / 255.0f, g / 255.0f, b / 255.0f);
}

PXL_ShaderProgram* PXL_create_shader(std::string vertex_file, std::string fragment_file) {
	return new PXL_ShaderProgram(PXL_load_file(vertex_file), PXL_load_file(fragment_file), vertex_file, fragment_file);
}

PXL_ShaderProgram* PXL_create_shader(std::string vertex_string, std::string fragment_string, 
								   std::string vertex_name, std::string fragment_name) {
	return new PXL_ShaderProgram(vertex_string, fragment_string, vertex_name, fragment_name);
}

PXL_ShaderProgram* verify_and_create_program_shader(std::string program_str, std::string program_name) {
	int start_v = program_str.find(start_v_header);
	int end_v = program_str.find(end_v_header);
	int start_f = program_str.find(start_f_header);
	int end_f = program_str.find(end_f_header);

	if (start_v != -1 && end_v != -1 && start_f != -1 && end_f != -1) {
		std::string vertex_file = program_str.substr(start_v + strlen(start_v_header) + 1, end_v - (start_v + strlen(start_v_header) + 1));
		std::string fragment_file = program_str.substr(start_f + strlen(start_f_header) + 1, end_f - (start_f + strlen(start_f_header) + 1));
		return new PXL_ShaderProgram(vertex_file, fragment_file, program_name + " - vertex", program_name + " - fragment");
	}else {
		PXL_show_exception("Headers not found when loading (" + program_name + "). Custom PXL_program shaders use " +
			start_v_header + " at the beginning of a vertex shader and " + end_v_header + " at the end. Fragment " +
			"shaders use " + start_f_header + " and " + end_f_header, PXL_ERROR_MISSING_SHADER_HEADERS, PXL_EXCEPTION_CONSOLE, false);
	}
	return NULL;
}

PXL_ShaderProgram* PXL_create_program_shader(std::string program_file) {
	return verify_and_create_program_shader(PXL_load_file(program_file), program_file);
}

PXL_ShaderProgram* PXL_create_program_shader(std::string program_str, std::string program_name) {
	return verify_and_create_program_shader(program_str, program_name);
}

std::string PXL_load_file(std::string file_name) {
	std::ifstream file(file_name.c_str(), std::ifstream::in);
	if (file) {
		file.ignore(std::numeric_limits<std::streamsize>::max());
		std::streamsize size = file.gcount();
		file.seekg(0, std::ifstream::beg);

		if (size >= 0) {
			char* buffer = new char[size];
			file.read(buffer, size);

			file.close();
			if (buffer) {
				buffer[size] = '\0';
				return buffer;
			}else {
				PXL_show_exception("(" + file_name + ") could not be read successfully", PXL_ERROR_INVALID_FILE, PXL_EXCEPTION_CONSOLE, false);
				delete[] buffer;
			}
		}else {
			PXL_show_exception("(" + file_name + ") does not contain any content when read", PXL_ERROR_EMPTY_FILE, PXL_EXCEPTION_CONSOLE, false);
		}
	}else {
		PXL_show_exception("Couldn't load file (" + file_name + "). It may not exist", PXL_ERROR_INVALID_FILE, PXL_EXCEPTION_CONSOLE, false);
	}
	return "";
}