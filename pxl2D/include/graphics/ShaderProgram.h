#ifndef _SHADER_PROGRAM_H
#define _SHADER_PROGRAM_H

#include <string>
#include <iostream>
#include <vector>
#include "graphics/GraphicsAPI.h"
#include "API.h"

namespace pxl { namespace graphics {

    class ShaderProgram {

	    public:
		    /**
		    \*brief: takes in a vertex and fragment shader path and creates the shader program from it
		    \*param [vertex_shader]: the path to a vertex shader file
		    \*param [fragment_shader]: the path to a fragment shader file
		    \*param [v_shader_name]: the name of the vertex shader file
		    \*param [f_shader_name]: the name of the fragment shader file
		    **/
		    ShaderProgram(std::string vertex_shader,		 std::string fragment_shader,
						      std::string v_shader_name = "n/a", std::string f_shader_name = "n/a");

		    /**
		    \*brief: gets the program id
		    **/
		    uint32 get_program_id() { return program_id; }
		    uint32 get_matrix_loc() { return matrix_loc; }
		    uint32 get_uniform_location(int index) { return locations[index]; }
		    uint32 add_uniform_location(std::string uniform_name);

		    void print_program_log(GLuint program_id);
		    void print_shader_log(GLuint shader_id);

	    private:
		    //shaderprogram ids
		    uint32 vertex_id;
		    uint32 fragment_id;
		    uint32 program_id;

		    //cached locations
		    uint32 matrix_loc;
		    std::vector<uint32> locations;

		    /**
		    \*brief: takes in a shader id and attempts to compile it
		    \*param [shader_id]: the shader id
		    \*param [shader_type]: the type of shader, being either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
		    \*param [shader_name]: the name of the shader to compile
		    **/
		    bool compile(uint32 shader_id, int shader_type, std::string shader_name);

		    /**
		    \*brief: gathers any logs from the shader and prints it if there is
		    \*param [shader_id]: the shader id
		    **/
		    void log(uint32 shader_id);
    };
}};

#endif