#include "PXL_System.h"

void PXL_system_init() {
	PXL_math_init();
}

char* PXL_append_char(char* c1, char* c2) {
	char* buffer = new char[strlen(c1) + strlen(c2)];
	strcpy(buffer, c1);
	strcat(buffer, c2);
	return buffer;
}

/**
Returns the last win32 error in a string. Returns an empty string if there is no error
**/
std::string PXL_get_os_error() {
	#if defined(WIN32)
		//gets the last error message from windows
		DWORD error_msg_id = GetLastError();
		if (error_msg_id == 0) { return "No error message has been recorded"; }

		LPSTR msg_buffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, error_msg_id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msg_buffer, 0, NULL);

		std::string message = msg_buffer;
		LocalFree(msg_buffer);

		return message;
	#endif
	return "";
}