#ifndef _PXL_DEBUG_H
#define _PXL_DEBUG_H

#include <iostream>
#include <string>
#include <sstream>
#include "system/PXL_API.h"

class PrintDebugger {

	public:
		std::ostringstream msg;

		PrintDebugger() { }

		template <class T> PrintDebugger &operator<<(const T &v) {
			msg << v;
			return *this;
		}

		~PrintDebugger();
};

#define PXL_print PrintDebugger()

class LogDebugger {

	public:
		std::ostringstream msg;
		std::ostringstream tag;
		bool has_set_tag = false;

		LogDebugger() { }

		template <class T> LogDebugger &operator<<(const T &v) {
			if (has_set_tag) {
				tag << v;
				has_set_tag = true;
			}else {
				msg << v;
			}
			return *this;
		}

		~LogDebugger();
};

#define PXL_log LogDebugger()

extern const std::string PXL_stack_trace(int num_traces = 8);
extern std::string PXL_get_last_error();

#endif