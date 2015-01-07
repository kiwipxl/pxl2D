#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glew.h>
#include <string>
#include <iostream>

using namespace std;

class ShaderProgram {

	public:
		ShaderProgram(string vertex_shader, string fragment_shader,
			string v_shader_name = "n/a", string f_shader_name = "n/a");

		GLuint get_vertex_id() { return vertex_id; }
		GLuint get_fragment_id() { return fragment_id; }

	private:
		GLuint vertex_id;
		GLuint fragment_id;
		GLuint program_id;

		bool compile(GLuint shader_id, int shader_type, string shader_name);
		void log(GLuint shader_id);
};

#endif