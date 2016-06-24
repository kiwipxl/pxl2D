#ifndef _WINDOW_H
#define _WINDOW_H

#include <string>
#include "system/Event.h"

namespace pxl { namespace system {

    #if defined(PLATFORM_WIN32)
	    #include "system/win32/Win32Window.h"
	    typedef Win32Window WinImplClass;
    #elif defined(PLATFORM_ANDROID)
	    #include "system/android/AndroidWindow.h"
	    typedef AndroidWindow WinImplClass;
    #elif defined (PLATFORM_LINUX)
	    #include "system/linux/LinuxWindow.h"
	    typedef LinuxWindow WinImplClass;
    #elif defined(PLATFORM_MAC_OS)
	    #include "system/osx/OSXWindow.h"
	    typedef OSXWindow WinImplClass;
    #endif

    class Window : public WinImplClass {

	    public:
		    Window() { }
		    /**
		    \*brief: creates the window with specified values
		    \*param [width]: the width of the image
		    \*param [height]: the height of the image
		    \*param [window]: an array of pixels for the image
		    **/
		    Window(int window_width, int window_height, std::string title) {
			    create_window(window_width, window_height, title);
		    }

		    /**
		    \*brief: window deconstructor
		    **/
		    ~Window() { free(); }
    };
}};

#endif