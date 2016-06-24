#ifndef _EXCEPTION_H
#define _EXCEPTION_H

namespace pxl { namespace system {

    #include "system/Config.h"
    #include <string>

    enum ExceptionType {
	    EXCEPTION_CONSOLE = 1, 
	    EXCEPTION_MSG_BOX = 1 << 1
    };

    extern bool show_exception(std::string exception_message, ErrorCode error_string = ERROR_UNKNOWN, 
							       ExceptionType type = EXCEPTION_MSG_BOX, bool exit = true);
    extern void force_show_exception(std::string exception_message, ErrorCode error_string = ERROR_UNKNOWN, 
							       ExceptionType type = EXCEPTION_MSG_BOX, bool exit = true);
}};

#endif