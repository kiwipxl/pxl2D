#include "PXL_ShaderUtils.h"
#include <fstream>

PXL_ShaderProgram* PXL_load_shader(string vertex_file, string fragment_file) {
	return new PXL_ShaderProgram(PXL_load_file(vertex_file), PXL_load_file(fragment_file), vertex_file, fragment_file);
}

PXL_ShaderProgram* PXL_load_glsl_shader(string glsl_file) {
	string s = PXL_load_file(glsl_file);

	int start_v = s.find(start_v_header);
	int end_v = s.find(end_v_header);
	int start_f = s.find(start_f_header);
	int end_f = s.find(end_f_header);

	if (start_v != -1 && end_v != -1 && start_f != -1 && end_f != -1) {
		string vertex_file = s.substr(start_v + strlen(start_v_header) + 1, end_v - (start_v + strlen(start_v_header) + 1));
		string fragment_file = s.substr(start_f + strlen(start_f_header) + 1, end_f - (start_f + strlen(start_f_header) + 1));
		return new PXL_ShaderProgram(vertex_file, fragment_file, glsl_file + " - vertex", glsl_file + " - fragment");
	}else {
		log_error("headers not found. start the vertex shader with #START_VERTEX and end with #END_VERTEX and for fragment too");
	}
	return new PXL_ShaderProgram("", "");
}

string PXL_load_file(string file_name) {
	ifstream file(file_name, ifstream::in);
	if (file) {
		file.ignore(numeric_limits<streamsize>::max());
		streamsize size = file.gcount();
		file.seekg(0, ifstream::beg);

		if (size >= 0) {
			char* buffer = new char[size];
			file.read(buffer, size);

			file.close();
			if (buffer) {
				buffer[size] = '\0';
				return buffer;
			}else {
				log_error("(" + file_name + ") could not be read successfully");
				delete[] buffer;
			}
		}else {
			log_error("size of (" + file_name + ") is less than zero");
		}
	}else {
		log_error("couldn't load shader file (" + file_name + "), may not exist");
	}
	return "";
}

void log_error(string e) {
	cout << "[shader log error]: " << e << "\n";
}