#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <glew.h>
#include <string>
#include <iostream>
#include "PXL_ShaderProgram.h"

using namespace std;

extern PXL_ShaderProgram* PXL_load_shader(string vertex_file, string fragment_file);
extern PXL_ShaderProgram* PXL_load_glsl_shader(string glsl_file);
extern string PXL_load_file(string file_name);

static const char* start_v_header = "#START_VERTEX";
static const char* end_v_header = "#END_VERTEX";
static const char* start_f_header = "#START_FRAGMENT";
static const char* end_f_header = "#END_FRAGMENT";

extern void log_error(string e);

#endif