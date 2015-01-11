#include "ShaderProgram.h"
#include <fstream>

ShaderProgram::ShaderProgram(string vertex_shader, string fragment_shader, string v_shader_name, string f_shader_name) {
	vertex_id = glCreateShader(GL_VERTEX_SHADER);
	fragment_id = glCreateShader(GL_FRAGMENT_SHADER);

	const GLcharARB* v_shader = vertex_shader.c_str();
	const GLint v_len = vertex_shader.length();
	const GLcharARB* f_shader = fragment_shader.c_str();
	const GLint f_len = fragment_shader.length();

	glShaderSourceARB(vertex_id, 1, &v_shader, &v_len);
	glShaderSourceARB(fragment_id, 1, &f_shader, &f_len);

	glCompileShaderARB(vertex_id);
	glCompileShaderARB(fragment_id);

	bool v_success = compile(vertex_id, GL_VERTEX_SHADER, v_shader_name);
	bool f_success = compile(fragment_id, GL_FRAGMENT_SHADER, f_shader_name);

	if (v_success && f_success) {
		program_id = glCreateProgram();
		glAttachShader(program_id, vertex_id);
		glAttachShader(program_id, fragment_id);
		glLinkProgram(program_id);

		GLint linked;
		glGetProgramiv(program_id, GL_LINK_STATUS, &linked);
		if (linked) {
			cout << "shader linked successfully\n";
		}else {
			cout << "shader link failed\n";
		}
	}
}

bool ShaderProgram::compile(GLuint shader_id, int shader_type, string shader_name) {
	GLint compiled;
	glGetObjectParameterivARB(shader_id, GL_COMPILE_STATUS, &compiled);
	if (compiled) {
		cout << "shader compiled successfully\n";
	}else {
		switch (shader_type) {
			case GL_VERTEX_SHADER:
				cout << "vertex shader (" << shader_name << ") failed to compile\n";
				break;
			case GL_FRAGMENT_SHADER:
				cout << "fragment shader (" << shader_name << ") failed to compile\n";
				break;
			default:
				cout << "unknown shader (" << shader_name << ") failed to compile\n";
				break;
		}
		log(shader_id);
	}
	return compiled;
}

void ShaderProgram::log(GLuint shader_id) {
	GLint log_len;
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_len);
	cout << "[compiler log]: ";
	if (log_len > 1) {
		GLchar* log = (GLchar*)malloc(log_len);

		glGetInfoLogARB(shader_id, log_len, 0, log);

		cout << log << "\n";
		delete log;
	}else {
		cout << "empty\n";
	}
}