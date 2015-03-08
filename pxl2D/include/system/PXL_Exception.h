#ifndef _PXL_EXCEPTION_H
#define _PXL_EXCEPTION_H

#include <string>
#include "system/PXL_Config.h"

enum PXL_ExceptionType {
	PXL_EXCEPTION_CONSOLE = 1, 
	PXL_EXCEPTION_MSG_BOX = 1 << 1
};

extern bool PXL_show_exception(std::string exception_message, PXL_ErrorCode error_string = PXL_ERROR_UNKNOWN, 
							   PXL_ExceptionType type = PXL_EXCEPTION_MSG_BOX, bool exit = true);
extern void PXL_force_show_exception(std::string exception_message, PXL_ErrorCode error_string = PXL_ERROR_UNKNOWN, 
							   PXL_ExceptionType type = PXL_EXCEPTION_MSG_BOX, bool exit = true);

#endif