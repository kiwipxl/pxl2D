#include "PXL_Exception.h"
#include <iostream>
#include <Windows.h>
#include "PXL_Config.h"
#include "PXL_Debug.h"

extern bool PXL_show_exception(std::string exception_message, PXL_ErrorCode error_string, PXL_ExceptionType type, bool exit) {
	#if (defined(PXL_DEBUG) && PXL_CONFIG_SHOW_EXCEPTIONS_IN_DEBUG) || (defined(PXL_RELEASE) && PXL_CONFIG_SHOW_EXCEPTIONS_IN_RELEASE)
	PXL_force_show_exception(exception_message, error_string, type, exit);
	return true;
	#endif
	return false;
}

extern void PXL_force_show_exception(std::string exception_message, PXL_ErrorCode error_string, PXL_ExceptionType type, bool exit) {
	std::string msg = PXL_CONFIG_EXCEPTION_TITLE;
	msg += " : " + exception_message;
	if (error_string != "") {
		msg += " - Error code: " + error_string;
	}
	#if PXL_CONFIG_SHOW_STACK_TRACE
		msg += "\n\n-- Stack trace --\n" + PXL_stack_trace();
	#else
		msg += "\n";
	#endif

	std::cout << msg << "\n";

	if ((type & PXL_EXCEPTION_CONSOLE) != 0) {
		std::cout << msg << "\n";
	}
	if ((type & PXL_EXCEPTION_MSG_BOX) != 0) {
		MessageBox(NULL, msg.c_str(), PXL_CONFIG_EXCEPTION_TITLE, MB_ICONERROR | MB_OK);
	}
	if (exit) std::exit(0);
}