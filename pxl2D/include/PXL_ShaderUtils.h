#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <string>
#include <iostream>
#include "PXL_ShaderProgram.h"

struct PXL_Batch;

using namespace std;

extern PXL_ShaderProgram* PXL_load_shader(string vertex_file, string fragment_file);
extern PXL_ShaderProgram* PXL_load_glsl_shader(string glsl_file);
extern string PXL_load_file(string file_name);

//premade pxl glsl shaders
extern PXL_ShaderProgram* PXL_default_shader;
extern PXL_ShaderProgram* PXL_bloom_shader;
extern PXL_ShaderProgram* PXL_repeat_shader;
extern PXL_ShaderProgram* PXL_grayscale_shader;
extern PXL_ShaderProgram* PXL_blur_shader;
extern PXL_ShaderProgram* PXL_outline_shader;
extern PXL_ShaderProgram* PXL_outer_glow_shader;

extern void PXL_shader_init();

extern void PXL_use_default_shader(PXL_Batch* batch);
extern void PXL_use_bloom_shader(PXL_Batch* b, float spread = 2, float intensity = .5f);
extern void PXL_use_repeat_shader(PXL_Batch* batch, float repeat_x = 2, float repeat_y = 2);
extern void PXL_use_grayscale_shader(PXL_Batch* batch);
extern void PXL_use_blur_shader(PXL_Batch* batch, float spread_x = 2, float spread_y = 2);
extern void PXL_use_outline_shader(PXL_Batch* batch, 
								   float thickness = 1, float r = 0, float g = 0, float b = 0, float a = 1, float threshold = .5f);
extern void PXL_use_outer_glow_shader(PXL_Batch* batch, float size = 4, float r = 0, float g = 0, float b = 0, 
									  float intensity = 4, float threshold = .5f);

#endif