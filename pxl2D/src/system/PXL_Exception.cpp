#include "PXL_Exception.h"
#include <iostream>
#include <Windows.h>
#include "PXL_Config.h"

extern bool PXL_show_exception(std::string exception_message, PXL_ExceptionType type, bool exit) {
	#if (defined(_DEBUG) && PXL_SHOW_EXCEPTIONS_IN_DEBUG) || (!defined(_DEBUG) && PXL_SHOW_EXCEPTIONS_IN_RELEASE)
	PXL_force_show_exception(exception_message, type, exit);
	return true;
	#endif
	return false;
}

extern void PXL_force_show_exception(std::string exception_message, PXL_ExceptionType type, bool exit) {
	if (type == PXL_EXCEPTION_CONSOLE || type == PXL_EXCEPTION_MSG_BOX_CONSOLE_WRITE) {
		std::cout << PXL_EXCEPTION_TITLE << ": " << exception_message.c_str() << "\n";
	}
	if (type == PXL_EXCEPTION_MSG_BOX || type == PXL_EXCEPTION_MSG_BOX_CONSOLE_WRITE) {
		MessageBox(NULL, exception_message.c_str(), PXL_EXCEPTION_TITLE, MB_ICONERROR | MB_OK);
	}
	if (exit) std::exit(0);
}