#ifndef _PXL_DEBUG_H
#define _PXL_DEBUG_H

#include <Windows.h>
#include <DbgHelp.h>
#include <iostream>
#include "PXL_API.h"

extern std::string PXL_stack_trace(int num_traces = 8);

#endif