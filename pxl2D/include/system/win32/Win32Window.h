#ifndef _WIN32WINDOW_H
#define _WIN32WINDOW_H

#include "API.h"

#if defined(PLATFORM_WIN32)

#include <string>
#include <vector>

#include "system/Event.h"
#include "system/WindowBase.h"

#include <windows.h>

namespace pxl { namespace sys {

    class Win32Window : public WindowBase {

	    public:
            Win32Window() { }
		    /**
		    \*brief: window deconstructor
		    **/
            ~Win32Window();

		    //window info
		    int pixel_format;
		    HDC device_context_handle;
		    HGLRC gl_render_context_handle;
		    HINSTANCE instance_handle;
		    WNDCLASS win_class;
		    HWND win_handle;
		    std::string class_name;
		    std::string win_name;

		    /**
		    \*brief: loads a window from the specified path
		    \*param [path]: the path and file name for the window to load
		    **/
            WindowBase& create(int win_width, int win_height, std::string win_title) override;

		    bool poll_event(Event& e) override;

		    /**
		    \*brief: frees all data from the window
		    **/
		    void free() override;

		    void display() override;

	    private:
		    //window info
		    MSG msg;

		    void register_class();
		    void unregister_class();
		    void create_context();
    };

}};

#endif
#endif