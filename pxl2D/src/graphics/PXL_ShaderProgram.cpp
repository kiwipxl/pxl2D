#include "PXL_ShaderProgram.h"
#include <fstream>
#include "system/PXL_Exception.h"

PXL_ShaderProgram::PXL_ShaderProgram(std::string vertex_shader, std::string fragment_shader, 
									 std::string v_shader_name, std::string f_shader_name) {
	vertex_id = glCreateShader(GL_VERTEX_SHADER);
	fragment_id = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* v_shader = vertex_shader.c_str();
	const GLint v_len = vertex_shader.length();
	const GLchar* f_shader = fragment_shader.c_str();
	const GLint f_len = fragment_shader.length();

	glShaderSourceARB(vertex_id, 1, &v_shader, &v_len);
	glShaderSourceARB(fragment_id, 1, &f_shader, &f_len);

	glCompileShaderARB(vertex_id);
	glCompileShaderARB(fragment_id);

	bool v_success = compile(vertex_id, GL_VERTEX_SHADER, v_shader_name);
	bool f_success = compile(fragment_id, GL_FRAGMENT_SHADER, f_shader_name);

	if (v_success && f_success) {
		//create program, attach shaders and link the program
		program_id = glCreateProgram();

		glAttachShader(program_id, vertex_id);
		glAttachShader(program_id, fragment_id);
		glLinkProgram(program_id);

		//check whether program link was successful
		GLint linked;
		glGetProgramiv(program_id, GL_LINK_STATUS, &linked);
		if (linked) {
			std::cout << "shader (" + v_shader_name + ") linked successfully\n";
		}else {
			PXL_show_exception("shader (" + v_shader_name + ") link failed", true, false);
		}

		//detach shaders whether or not linking was successful
		glDetachShader(program_id, vertex_id);
		glDetachShader(program_id, fragment_id);
	}
	//delete shaders
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);
}

bool PXL_ShaderProgram::compile(GLuint shader_id, int shader_type, std::string shader_name) {
	GLint compiled;
	glGetObjectParameterivARB(shader_id, GL_COMPILE_STATUS, &compiled);
	if (compiled) {
		std::cout << "shader compiled successfully\n";
	}else {
		switch (shader_type) {
			case GL_VERTEX_SHADER:
				PXL_show_exception("Vertex shader(" + shader_name + ") failed to compile", true, false);
				break;
			case GL_FRAGMENT_SHADER:
				PXL_show_exception("Fragment shader (" + shader_name + ") failed to compile", true, false);
				break;
			default:
				PXL_show_exception("Unknown shader (" + shader_name + ") failed to compile", true, false);
				break;
		}
		log(shader_id);
	}
	return compiled;
}

void PXL_ShaderProgram::log(GLuint shader_id) {
	GLint log_len;
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_len);
	std::cout << "[compiler log]: ";
	if (log_len > 1) {
		GLchar* log = (GLchar*)malloc(log_len);

		glGetInfoLogARB(shader_id, log_len, 0, log);

		std::cout << log << "\n";
		delete log;
	}else {
		std::cout << "empty\n";
	}
}