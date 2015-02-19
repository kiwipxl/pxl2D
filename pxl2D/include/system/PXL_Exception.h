#ifndef _PXL_EXCEPTION_H
#define _PXL_EXCEPTION_H

#include <string>
#include "PXL_Config.h"

typedef int PXL_ExceptionType;
#define PXL_EXCEPTION_CONSOLE 1
#define PXL_EXCEPTION_MSG_BOX 1 << 1

extern bool PXL_show_exception(std::string exception_message, PXL_ErrorCode error_string = PXL_ERROR_NONE, 
							   PXL_ExceptionType type = PXL_EXCEPTION_MSG_BOX, bool exit = true);
extern void PXL_force_show_exception(std::string exception_message, PXL_ErrorCode error_string = PXL_ERROR_NONE, 
							   PXL_ExceptionType type = PXL_EXCEPTION_MSG_BOX, bool exit = true);

#endif