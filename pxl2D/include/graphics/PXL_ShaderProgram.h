#ifndef _PXL_SHADER_PROGRAM_H
#define _PXL_SHADER_PROGRAM_H

#include <string>
#include <iostream>
#include <vector>
#include "graphics/PXL_GraphicsAPI.h"
#include "system/PXL_API.h"

class PXL_ShaderProgram {

	public:
		/**
		\*brief: takes in a vertex and fragment shader path and creates the shader program from it
		\*param [vertex_shader]: the path to a vertex shader file
		\*param [fragment_shader]: the path to a fragment shader file
		\*param [v_shader_name]: the name of the vertex shader file
		\*param [f_shader_name]: the name of the fragment shader file
		**/
		PXL_ShaderProgram(std::string vertex_shader,		 std::string fragment_shader,
						  std::string v_shader_name = "n/a", std::string f_shader_name = "n/a");

		/**
		\*brief: gets the program id
		**/
		PXL_uint get_program_id() { return program_id; }
		PXL_uint get_matrix_loc() { return matrix_loc; }
		PXL_uint get_uniform_location(int index) { return locations[index]; }
		PXL_uint add_uniform_location(std::string uniform_name);

		void print_program_log(GLuint program_id);
		void print_shader_log(GLuint shader_id);

	private:
		//shaderprogram ids
		PXL_uint vertex_id;
		PXL_uint fragment_id;
		PXL_uint program_id;

		//cached locations
		PXL_uint matrix_loc;
		std::vector<PXL_uint> locations;

		/**
		\*brief: takes in a shader id and attempts to compile it
		\*param [shader_id]: the shader id
		\*param [shader_type]: the type of shader, being either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
		\*param [shader_name]: the name of the shader to compile
		**/
		bool compile(PXL_uint shader_id, int shader_type, std::string shader_name);

		/**
		\*brief: gathers any logs from the shader and prints it if there is
		\*param [shader_id]: the shader id
		**/
		void log(PXL_uint shader_id);
};

#endif