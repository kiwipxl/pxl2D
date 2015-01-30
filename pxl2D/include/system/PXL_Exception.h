#ifndef _PXL_EXCEPTION_H
#define _PXL_EXCEPTION_H

#include <string>

typedef int PXL_ExceptionType;
#define PXL_EXCEPTION_CONSOLE 0
#define PXL_EXCEPTION_MSG_BOX 1
#define PXL_EXCEPTION_MSG_BOX_CONSOLE_WRITE 2

extern bool PXL_show_exception(std::string exception_message, PXL_ExceptionType type = PXL_EXCEPTION_MSG_BOX, bool exit = true);
extern void PXL_force_show_exception(std::string exception_message, PXL_ExceptionType type = PXL_EXCEPTION_MSG_BOX, bool exit = true);

#endif