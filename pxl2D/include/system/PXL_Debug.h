#ifndef _PXL_DEBUG_H
#define _PXL_DEBUG_H

#include <iostream>
#include "system/PXL_API.h"

extern const std::string PXL_stack_trace(int num_traces = 8);
extern std::string PXL_get_last_error();
extern void PXL_log(std::string msg, std::string tag = "");

#endif