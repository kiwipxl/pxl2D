#include "PXL_Exception.h"
#include <iostream>
#include <Windows.h>
#include "PXL_Config.h"

extern bool PXL_show_exception(std::string exception_message, PXL_ErrorCode error_string, PXL_ExceptionType type, bool exit) {
	#if (defined(_DEBUG) && PXL_SHOW_EXCEPTIONS_IN_DEBUG) || (!defined(_DEBUG) && PXL_SHOW_EXCEPTIONS_IN_RELEASE)
	PXL_force_show_exception(exception_message, error_string, type, exit);
	return true;
	#endif
	return false;
}

extern void PXL_force_show_exception(std::string exception_message, PXL_ErrorCode error_string, PXL_ExceptionType type, bool exit) {
	std::string msg = PXL_EXCEPTION_TITLE;
	msg += " : " + exception_message;
	if (error_string != "") {
		msg += " - Error code: " + error_string;
	}

	if ((type & PXL_EXCEPTION_CONSOLE) != 0) {
		std::cout << msg << "\n";
	}
	if ((type & PXL_EXCEPTION_MSG_BOX) != 0) {
		MessageBox(NULL, msg.c_str(), PXL_EXCEPTION_TITLE, MB_ICONERROR | MB_OK);
	}
	if (exit) std::exit(0);
}