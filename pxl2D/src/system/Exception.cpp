#include "system/Exception.h"

#define NOMINMAX //macro to not have the windows header define min/max so it doesn't interfere
#include <Windows.h>
#undef ABSOLUTE
#undef RELATIVE
#include <string>

#include "system/Config.h"
#include "system/Debug.h"

namespace pxl { namespace sys {

    bool show_exception(std::string exception_message, ErrorCode error_string, ExceptionType type, bool exit) {
	    //#if (defined(DEBUG) && CONFIG_SHOW_EXCEPTIONS_IN_DEBUG) || (defined(RELEASE) && CONFIG_SHOW_EXCEPTIONS_IN_RELEASE)
		    force_show_exception(exception_message, error_string, type, exit);
		    return true;
	    //#endif
	    return false;
    }

    void force_show_exception(std::string exception_message, ErrorCode error_string, ExceptionType type, bool exit) {
	    std::string msg = CONFIG_EXCEPTION_TITLE;
	    msg += " : " + exception_message;
	    if (error_string != "") {
		    msg += " - Error code: " + error_string;
	    }
	    #if CONFIG_SHOW_STACK_TRACE
		    msg += "\n\n-- Stack trace --\n" + stack_trace();
	    #else
		    msg += "\n";
	    #endif

	    if ((type & EXCEPTION_CONSOLE) != 0) {
		    print << msg << "\n";
	    }
	    if ((type & EXCEPTION_MSG_BOX) != 0) {
		    #if defined(PLATFORM_WIN32)
			    MessageBox(NULL, msg.c_str(), CONFIG_EXCEPTION_TITLE, MB_ICONERROR | MB_OK);
		    #elif defined(PLATFORM_ANDROID)
			    //todo: add msg box here as well as printing
			    print << msg << "\n";
		    #endif
	    }
	    //todo: broken on android
	    //if (exit) std::exit(0);
    }

}};
