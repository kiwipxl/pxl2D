#ifndef _LINUXWINDOW_H
#define _LINUXWINDOW_H

#include "graphics/GraphicsAPI.h"

namespace pxl { namespace sys {

    #if defined(PLATFORM_LINUX)

    #include <string>
    #include <vector>
    #include "system/Event.h"
    #include "system/WindowBase.h"

    class LinuxWindow : public WindowBase {

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
            WindowBase& create(int window_width, int window_height, std::string title) override;

		    bool poll_event(Event& e) override;

		    /**
		    \*brief: frees all data from the window
		    **/
		    void free() override;

		    void display() override;

	    private:
		    //window info
    };

    #endif
}};
#endif