#include "system/PXL_IO.h"

#include <fstream>

#include "system/PXL_Exception.h"
#include "system/PXL_Debug.h"

std::string PXL_read_file_contents(std::string file_name) {
	std::ifstream file(file_name.c_str(), std::ifstream::in);
	if (file) {
		file.ignore(std::numeric_limits<std::streamsize>::max());
		std::streamsize size = file.gcount();
		file.seekg(0, std::ifstream::beg);

		PXL_print << "file size: " << size << "\n";

		if (size >= 0) {
			char* buffer = new char[size];
			file.read(buffer, size);

			file.close();
			if (buffer) {
				buffer[size] = '\0';
				return buffer;
			}else {
				PXL_show_exception("(" + file_name + ") could not be read successfully", PXL_ERROR_INVALID_FILE, PXL_EXCEPTION_CONSOLE, false);
				delete[] buffer;
			}
		}else {
			file.close();
			PXL_show_exception("(" + file_name + ") does not contain any content when read", PXL_ERROR_EMPTY_FILE, PXL_EXCEPTION_CONSOLE, false);
		}
	}else {
		file.close();
		PXL_show_exception("Couldn't load file (" + file_name + "). It may not exist", PXL_ERROR_INVALID_FILE, PXL_EXCEPTION_CONSOLE, false);
	}
	return "";
}

char* PXL_append_char(const char* c1, const char* c2) {
	char* buffer = new char[strlen(c1) + strlen(c2)];
	strcpy(buffer, c1);
	strcat(buffer, c2);
	return buffer;
}