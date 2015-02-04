#ifndef _PXL_SYSTEM_H
#define _PXL_SYSTEM_H

#include "system/PXL_Window.h"
#include "system/PXL_Math.h"
#include "system/PXL_Timer.h"
#include "system/PXL_Exception.h"

/** Initialises PXL system
**/
extern void PXL_system_init();

//template<typename... Args>
extern char* PXL_append_char(char* c1, char* c2);
extern std::string PXL_get_os_error();

#endif