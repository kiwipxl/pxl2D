#include "graphics/ShaderUtils.h"
#include <fstream>
#include "graphics/Batch.h"
#include "graphics/PrebuiltShaders.h"
#include "system/Debug.h"

namespace pxl { namespace graphics {

    //defines
    const char* start_v_header = "//[START_VERTEX]";
    const char* end_v_header = "//[END_VERTEX]";
    const char* start_f_header = "//[START_FRAGMENT]";
    const char* end_f_header = "//[END_FRAGMENT]";

    ShaderProgram* default_shader;
    ShaderProgram* bloom_shader;
    ShaderProgram* repeat_shader;
    ShaderProgram* grayscale_shader;
    ShaderProgram* blur_shader;
    ShaderProgram* outline_shader;
    ShaderProgram* glow_shader;
    ShaderProgram* text_shader;
    ShaderProgram* point_light_shader;

    const void init_shader() {
	    //setup premade pxl glsl shaders
	    default_shader = create_shader(basic_vertex_shader_str, default_shader_str, "default_vert", "default_frag");
	    bloom_shader = create_shader(basic_vertex_shader_str, bloom_shader_str, "default_vert", "bloom_frag");
	    repeat_shader = create_shader(basic_vertex_shader_str, repeat_shader_str, "default_vert", "repeat_frag");
	    grayscale_shader = create_shader(basic_vertex_shader_str, grayscale_shader_str, "default_vert", "grayscale_frag");
	    blur_shader = create_shader(basic_vertex_shader_str, blur_shader_str, "default_vert", "blur_frag");
	    outline_shader = create_shader(basic_vertex_shader_str, outline_shader_str, "default_vert", "outline_frag");
	    glow_shader = create_shader(basic_vertex_shader_str, glow_shader_str, "default_vert", "glow_frag");
        text_shader = create_shader(basic_vertex_shader_str, text_shader_str, "default_vert", "text_frag");
        //point_light_shader = create_shader(basic_vertex_shader_str, point_light_shader_str, "default_vert", "point_light_frag");
    }

    const void set_default_shader(Batch* batch) {
	    glUseProgram(default_shader->get_program_id());
    }

    const void set_bloom_shader(Batch* batch, float spread, float intensity) {
	    glUseProgram(bloom_shader->get_program_id());
	    glUniform1f(glGetUniformLocation(bloom_shader->get_program_id(), "outline_spread"), spread);
	    glUniform1f(glGetUniformLocation(bloom_shader->get_program_id(), "outline_intensity"), intensity);
    }

    const void set_repeat_shader(Batch* batch, float repeat_x, float repeat_y) {
	    glUseProgram(repeat_shader->get_program_id());
	    glUniform2f(glGetUniformLocation(repeat_shader->get_program_id(), "repeat"), repeat_x, repeat_y);
    }

    const void set_grayscale_shader(Batch* batch) {
	    glUseProgram(grayscale_shader->get_program_id());
    }

    const void set_blur_shader(Batch* batch, float spread_x, float spread_y) {
	    glUseProgram(blur_shader->get_program_id());
	    glUniform2f(glGetUniformLocation(blur_shader->get_program_id(), "blur_size"), spread_x, spread_y);
    }

    const void set_outline_shader(Batch* batch, float thickness, float r, float g, float b, float a, float threshold) {
	    glUseProgram(outline_shader->get_program_id());
	    glUniform1f(glGetUniformLocation(outline_shader->get_program_id(), "outline_thickness"), thickness);
	    glUniform4f(glGetUniformLocation(outline_shader->get_program_id(), "outline_colour"), r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	    glUniform1f(glGetUniformLocation(outline_shader->get_program_id(), "outline_threshold"), threshold);
    }

    const void set_glow_shader(Batch* batch, float size, float r, float g, float b, float intensity, float threshold) {
	    glUseProgram(glow_shader->get_program_id());
	    glUniform1f(glGetUniformLocation(outline_shader->get_program_id(), "outline_size"), size);
	    glUniform3f(glGetUniformLocation(outline_shader->get_program_id(), "outline_colour"), r / 255.0f, g / 255.0f, b / 255.0f);
	    glUniform1f(glGetUniformLocation(outline_shader->get_program_id(), "outline_threshold"), threshold);
	    glUniform1f(glGetUniformLocation(outline_shader->get_program_id(), "outline_intensity"), intensity);
    }

    const void set_text_shader(Batch* batch, float r, float g, float b, float a) {
	    glUseProgram(text_shader->get_program_id());
	    glUniform3f(glGetUniformLocation(text_shader->get_program_id(), "text_colour"), r / 255.0f, g / 255.0f, b / 255.0f);
    }

    ShaderProgram* create_shader(std::string vertex_file, std::string fragment_file) {
	    return new ShaderProgram(load_file(vertex_file), load_file(fragment_file), vertex_file, fragment_file);
    }

    ShaderProgram* create_shader(std::string vertex_string, std::string fragment_string, 
								       std::string vertex_name, std::string fragment_name) {
	    return new ShaderProgram(vertex_string, fragment_string, vertex_name, fragment_name);
    }

    ShaderProgram* verify_and_create_program_shader(std::string program_str, std::string program_name) {
	    int start_v = program_str.find(start_v_header);
	    int end_v = program_str.find(end_v_header);
	    int start_f = program_str.find(start_f_header);
	    int end_f = program_str.find(end_f_header);

	    if (start_v != -1 && end_v != -1 && start_f != -1 && end_f != -1) {
		    std::string vertex_file = program_str.substr(start_v + strlen(start_v_header) + 1, end_v - (start_v + strlen(start_v_header) + 1));
		    std::string fragment_file = program_str.substr(start_f + strlen(start_f_header) + 1, end_f - (start_f + strlen(start_f_header) + 1));
		    return new ShaderProgram(vertex_file, fragment_file, program_name + " - vertex", program_name + " - fragment");
	    }else {
            system::show_exception("Headers not found when loading (" + program_name + "). Custom program shaders use " +
			    start_v_header + " at the beginning of a vertex shader and " + end_v_header + " at the end. Fragment " +
			    "shaders use " + start_f_header + " and " + end_f_header, ERROR_MISSING_SHADER_HEADERS, system::EXCEPTION_CONSOLE, false);
	    }
	    return NULL;
    }

    ShaderProgram* create_program_shader(std::string program_file) {
	    return verify_and_create_program_shader(load_file(program_file), program_file);
    }

    ShaderProgram* create_program_shader(std::string program_str, std::string program_name) {
	    return verify_and_create_program_shader(program_str, program_name);
    }

    std::string load_file(std::string file_name) {
	    std::ifstream file(file_name.c_str(), std::ifstream::in);
	    if (file) {
		    file.ignore(std::numeric_limits<std::streamsize>::max());
		    std::streamsize size = file.gcount();
		    file.seekg(0, std::ifstream::beg);

		    if (size >= 0) {
			    char* buffer = new char[size];
			    file.read(buffer, size);

			    file.close();
			    if (buffer) {
				    buffer[size] = '\0';
				    return buffer;
			    }else {
                    system::show_exception("(" + file_name + ") could not be read successfully", ERROR_INVALID_FILE, system::EXCEPTION_CONSOLE, false);
				    delete[] buffer;
			    }
		    }else {
                system::show_exception("(" + file_name + ") does not contain any content when read", ERROR_EMPTY_FILE, system::EXCEPTION_CONSOLE, false);
		    }
	    }else {
            system::show_exception("Couldn't load file (" + file_name + "). It may not exist", ERROR_INVALID_FILE, system::EXCEPTION_CONSOLE, false);
	    }
	    return "";
    }

}};