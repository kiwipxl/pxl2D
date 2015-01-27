#include "PXL_Window.h"
#include <iostream>
#include <Windows.h>
#include "PXL_Config.h"

extern bool PXL_show_exception(std::string exception_message, bool console_write, bool exit) {
	#if (defined(_DEBUG) && PXL_SHOW_EXCEPTIONS_IN_DEBUG) || (!defined(_DEBUG) && PXL_SHOW_EXCEPTIONS_IN_RELEASE)
	if (console_write) {
		std::cout << PXL_EXCEPTION_TITLE << ": " << exception_message.c_str() << "\n";
	}else {
		MessageBox(NULL, exception_message.c_str(), PXL_EXCEPTION_TITLE, MB_ICONERROR | MB_OK);
	}
	if (exit) std::exit(0);
	return true;
	#endif
	return false;
}

extern void PXL_force_show_exception(std::string exception_message, bool console_write, bool exit) {
	if (console_write) {
		std::cout << PXL_EXCEPTION_TITLE << ": " << exception_message.c_str() << "\n";
	}else {
		MessageBox(NULL, exception_message.c_str(), PXL_EXCEPTION_TITLE, MB_ICONERROR | MB_OK);
	}
	if (exit) std::exit(0);
}