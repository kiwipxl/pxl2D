#ifndef _PXL_DEBUG_H
#define _PXL_DEBUG_H

#include <Windows.h>
#if defined(_DEBUG)
	#include <DbgHelp.h>
#endif
#include <iostream>
#include "PXL_API.h"

extern const std::string PXL_stack_trace(int num_traces = 8);
extern std::string PXL_get_last_error();

#endif