#ifndef _SHADER_UTILS_H
#define _SHADER_UTILS_H

#include <string>
#include <iostream>
#include "graphics/ShaderProgram.h"

class Batch;

namespace pxl { namespace graphics {

    /**
    \*brief: creates a shader program from the specified vertex and fragment shader paths
    \*param [vertex_file]: the path to the vertex shader file
    \*param [fragment_file]: the path to the fragment shader file
    **/
    extern ShaderProgram* create_shader(std::string vertex_file, std::string fragment_file);

    extern ShaderProgram* create_shader(std::string vertex_string, std::string fragment_string,
										      std::string vertex_name, std::string fragment_name);

    /**
    \*brief: creates a shader program from the specified glsl custom shader path
    \*param [glsl_file]: the path to the glsl shader file
    **/
    extern ShaderProgram* create_program_shader(std::string program_file);

    extern ShaderProgram* create_program_shader(std::string program_str, std::string program_name);

    /**
    \*brief: reads the contents of a file
    \*param [file_name]: the path and file name to load
    **/
    extern std::string load_file(std::string file_name);

    //premade pxl glsl shaders
    extern ShaderProgram* default_shader;
    extern ShaderProgram* bloom_shader;
    extern ShaderProgram* repeat_shader;
    extern ShaderProgram* grayscale_shader;
    extern ShaderProgram* blur_shader;
    extern ShaderProgram* outline_shader;
    extern ShaderProgram* glow_shader;
    extern ShaderProgram* text_shader;
    extern ShaderProgram* point_light_shader;

    /**
    \*brief: initialises prebuilt shaders, note: this should only ever be called by PXL
    **/
    extern const void init_shader();

    /**
    \*brief: sets a prebuilt default shader onto the specified batch
    \*param [batch]: the batch object to set the shader to
    **/
    extern const void set_default_shader(Batch* batch);

    /**
    \*brief: sets a prebuilt bloom shader onto the specified batch
    \*param [batch]: the batch object to set the shader to
    \*param [spread]: the bloom spread amount
    \*param [intensity]: the bloom intensity
    **/
    extern const void set_bloom_shader(Batch* batch, float spread = 2, float intensity = .5f);

    /**
    \*brief: sets a prebuilt default shader onto the specified batch
    \*param [batch]: the batch object to set the shader to
    \*param [repeat_x]: the amount of times to repeat textures horizontally
    \*param [repeat_y]: the amount of times to repeat textures vertically
    **/
    extern const void set_repeat_shader(Batch* batch, float repeat_x = 2, float repeat_y = 2);

    /**
    \*brief: sets a prebuilt default shader onto the specified batch
    \*param [batch]: the batch object to set the shader to
    **/
    extern const void set_grayscale_shader(Batch* batch);

    /**
    \*brief: sets a prebuilt default shader onto the specified batch
    \*param [batch]: the batch object to set the shader to
    \*param [spread_x]: the value to blur horizontally
    \*param [spread_y]: the value to blur vertically
    **/
    extern const void set_blur_shader(Batch* batch, float spread_x = 2, float spread_y = 2);

    /**
    \*brief: sets a prebuilt default shader onto the specified batch
    \*param [batch]: the batch object to set the shader to
    \*param [thickness]: the outline thickness
    \*param [r]: red colour for the outline which ranges from 0 to 255
    \*param [g]: green colour for the outline which ranges from 0 to 255
    \*param [b]: blue colour for the outline which ranges from 0 to 255
    \*param [a]: alpha colour for the outline which ranges from 0 to 255
    \*param [threshold]: the value from 0 to 1 which defines in what alpha will the outline be filled in
    **/
    extern const void set_outline_shader(Batch* batch, 
										     float thickness = 1, float r = 0, float g = 0, float b = 0, float a = 255, float threshold = .5f);

    /**
    \*brief: sets a prebuilt default shader onto the specified batch
    \*param [batch]: the batch object to set the shader to
    \*param [size]: the size of the glow
    \*param [r]: red colour for the outline which ranges from 0 to 255
    \*param [g]: green colour for the outline which ranges from 0 to 255
    \*param [b]: blue colour for the outline which ranges from 0 to 255
    \*param [intensity]: the intensity of the glow
    \*param [threshold]: the value from 0 to 1 which defines in what alpha will the outline be filled in
    **/
    extern const void set_glow_shader(Batch* batch, float size = 4, float r = 0, float g = 0, float b = 0,
									      float intensity = 4, float threshold = .5f);

    /**
    \*brief: sets a prebuilt default shader onto the specified batch
    \*param [batch]: the batch object to set the shader to
    \*param [thickness]: the text thickness
    \*param [r]: red colour for the text which ranges from 0 to 255
    \*param [g]: green colour for the text which ranges from 0 to 255
    \*param [b]: blue colour for the text which ranges from 0 to 255
    \*param [a]: alpha colour for the text which ranges from 0 to 255
    \*param [threshold]: the value from 0 to 1 which defines in what alpha will the text be filled in
    **/
    extern const void set_text_shader(Batch* batch, 
								    float r = 0, float g = 0, float b = 0, float a = 255);
}};

#endif