#ifndef _PXL_EXCEPTION_H
#define _PXL_EXCEPTION_H

extern bool PXL_show_exception(std::string exception_message, bool console_write = false, bool exit = true);
extern void PXL_force_show_exception(std::string exception_message, bool console_write = false, bool exit = true);

#endif