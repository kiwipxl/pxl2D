#ifndef _LINUXWINDOW_H
#define _LINUXWINDOW_H

#include "graphics/GraphicsAPI.h"

namespace pxl { namespace system {

    #if defined(PLATFORM_LINUX)

    #include <string>
    #include <vector>
    #include "system/Event.h"
    #include "system/WindowImpl.h"

    class LinuxWindow : public WindowImpl {

	    public:
		    LinuxWindow() { }
		    /**
		    \*brief: window deconstructor
		    **/
		    ~LinuxWindow();

		    /**
		    \*brief: loads a window from the specified path
		    \*param [path]: the path and file name for the window to load
		    **/
		    virtual void create_window(int window_width, int window_height, std::string title);

		    virtual bool poll_event(Event& e);

		    /**
		    \*brief: frees all data from the window
		    **/
		    virtual void free();

		    virtual void display();

	    private:
		    //window info
    };

    #endif
}};
#endif