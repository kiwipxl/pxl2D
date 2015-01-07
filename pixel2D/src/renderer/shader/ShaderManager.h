#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <glew.h>
#include <string>
#include <iostream>
#include "ShaderProgram.h"

using namespace std;

class ShaderManager {

	public:
		ShaderManager();

		ShaderProgram* load_shader(string vertex_file, string fragment_file);
		ShaderProgram* load_glsl_shader(string glsl_file);
		string ShaderManager::load_file(string file_name);

	private:
		const char* start_v_header = "#START_VERTEX";
		const char* end_v_header = "#END_VERTEX";
		const char* start_f_header = "#START_FRAGMENT";
		const char* end_f_header = "#END_FRAGMENT";

		void log_error(string e);
};

#endif