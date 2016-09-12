#ifndef _DEBUG_H
#define _DEBUG_H

#include <iostream>
#include <string>
#include <sstream>

#include "PXLAPI.h"

namespace pxl { namespace sys {

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

    #define print PrintDebugger()

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

    #define log LogDebugger()

    extern const std::string stack_trace(int num_traces = 8);
    extern std::string get_last_error();
}};

#endif