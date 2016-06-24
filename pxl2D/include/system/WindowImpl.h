#ifndef _WINDOW_IMPL_H
#define _WINDOW_IMPL_H

#include <string>
#include "system/Event.h"
#include "API.h"

namespace pxl { namespace system {

    class WindowImpl {

	    public:
		    bool window_created = false;
		    std::string title;
		    int width;
		    int height;

		    //temporary code
		    //todo: make cross-platform window handle
		    #if defined(PLATFORM_WIN32)
			    HWND hwnd;
		    #endif

		    /**
		    \*brief: loads a window from the specified path
		    \*param [path]: the path and file name for the window to load
		    **/
		    virtual void create_window(int window_width, int window_height, std::string title) = 0;

		    virtual bool poll_event(Event& e) = 0;

		    virtual void display() = 0;

		    /**
		    \*brief: frees all data from the window
		    **/
		    virtual void free() = 0;

		    int get_width() { return width; }
		    int get_height() { return height; }
		    std::string get_title() { return title; }

	    protected:
		    //window info
    };
}};

#endif