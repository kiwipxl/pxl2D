#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <string>
#include <iostream>
#include "PXL_ShaderProgram.h"

using namespace std;

extern PXL_ShaderProgram* PXL_load_shader(string vertex_file, string fragment_file);
extern PXL_ShaderProgram* PXL_load_glsl_shader(string glsl_file);
extern string PXL_load_file(string file_name);

//premade pxl glsl shaders
extern PXL_ShaderProgram* PXL_default_shader;
extern PXL_ShaderProgram* PXL_bloom_shader;
extern PXL_ShaderProgram* PXL_repeat_shader;;

extern void PXL_shader_init();

#endif