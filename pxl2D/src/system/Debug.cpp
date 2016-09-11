#include "system/Debug.h"
#include <string>

namespace pxl { namespace sys {

	const std::string stack_trace(int num_traces) {
		return "";
	}

	std::string get_last_error() {
        return "";
	}

	PrintDebugger::~PrintDebugger() {
		std::cout << msg.str();
	}

	LogDebugger::~LogDebugger() {

	}
}};