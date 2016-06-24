#include "graphics/ShaderProgram.h"
#include "system/Debug.h"
#include "system/Exception.h"

namespace pxl { namespace graphics {

    ShaderProgram::ShaderProgram(std::string vertex_shader, std::string fragment_shader, 
									     std::string v_shader_name, std::string f_shader_name) {
	    vertex_id = glCreateShader(GL_VERTEX_SHADER);
        fragment_id = glCreateShader(GL_FRAGMENT_SHADER);

	    const GLchar* v_shader = vertex_shader.c_str();
	    const GLint v_len = vertex_shader.length();
	    const GLchar* f_shader = fragment_shader.c_str();
	    const GLint f_len = fragment_shader.length();

	    glShaderSource(vertex_id, 1, &v_shader, &v_len);
	    glShaderSource(fragment_id, 1, &f_shader, &f_len);

	    glCompileShader(vertex_id);
	    glCompileShader(fragment_id);

	    bool v_success = compile(vertex_id, GL_VERTEX_SHADER, v_shader_name);
	    bool f_success = compile(fragment_id, GL_FRAGMENT_SHADER, f_shader_name);

	    if (v_success && f_success) {
		    //create program, attach shaders and link the program
            program_id = glCreateProgram();

            glAttachShader(program_id, vertex_id);
            glAttachShader(program_id, fragment_id);

            glBindAttribLocation(program_id, 0, "a_position");
            glBindAttribLocation(program_id, 1, "a_tex_coord");
            glBindAttribLocation(program_id, 2, "a_colour");

            glLinkProgram(program_id);

		    //check whether program link was successful
		    GLint linked;
		    glGetProgramiv(program_id, GL_LINK_STATUS, &linked);
		    if (linked) {
                system::print << "shader (" << v_shader_name << ", " << f_shader_name << ") linked successfully\n";
			    matrix_loc = glGetUniformLocation(program_id, "matrix");
		    }else {
                system::show_exception("shader (" + v_shader_name + ", " + f_shader_name + ") link failed", ERROR_SHADER_LINK_FAILED, system::EXCEPTION_CONSOLE, false);
			    print_program_log(program_id);
		    }

		    //detach shaders whether or not linking was successful
		    glDetachShader(program_id, vertex_id);
		    glDetachShader(program_id, fragment_id);
	    }
	    //delete shaders
	    glDeleteShader(vertex_id);
	    glDeleteShader(fragment_id);
    }

    bool ShaderProgram::compile(GLuint shader_id, int shader_type, std::string shader_name) {
	    GLint compiled;
	    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
	    if (compiled) {
            system::print << "shader (" << shader_name << ") compiled successfully\n";
	    }else {
		    switch (shader_type) {
			    case GL_VERTEX_SHADER:
                    system::show_exception("Vertex shader(" + shader_name + ") failed to compile", ERROR_SHADER_COMPILE_FAILED, system::EXCEPTION_CONSOLE, false);
				    break;
			    case GL_FRAGMENT_SHADER:
                    system::show_exception("Fragment shader (" + shader_name + ") failed to compile", ERROR_SHADER_COMPILE_FAILED, system::EXCEPTION_CONSOLE, false);
				    break;
			    default:
                    system::show_exception("Unknown shader (" + shader_name + ") failed to compile", ERROR_SHADER_COMPILE_FAILED, system::EXCEPTION_CONSOLE, false);
				    break;
		    }
		    print_shader_log(shader_id);
	    }
	    return compiled;
    }

    void ShaderProgram::print_program_log(GLuint program_id) {
	    GLint log_len;
	    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_len);
        system::print << "[program log]: ";
	    if (log_len > 1) {
		    GLchar* new_log = new GLchar[log_len];

		    glGetProgramInfoLog(program_id, log_len, 0, new_log);

            system::print << new_log << "\n";
		    delete new_log;
	    }else {
            system::print << "empty\n";
	    }
    }

    void ShaderProgram::print_shader_log(GLuint shader_id) {
	    GLint log_len;
	    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_len);
        system::print << "[compiler log]: ";
	    if (log_len > 1) {
		    GLchar* new_log = new GLchar[log_len];

		    glGetShaderInfoLog(shader_id, log_len, 0, new_log);

            system::print << new_log << "\n";
		    delete new_log;
	    }else {
            system::print << "empty\n";
	    }
    }

    GLuint ShaderProgram::add_uniform_location(std::string uniform_name) {
	    GLuint loc = glGetUniformLocation(program_id, uniform_name.c_str());
	    locations.push_back(loc);
	    return loc;
    }

}};