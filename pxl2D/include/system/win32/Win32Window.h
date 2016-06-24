#ifndef _WIN32WINDOW_H
#define _WIN32WINDOW_H

#include "API.h"

#if defined(PLATFORM_WIN32)

class HDC;
class HGLRC;
class HINSTANCE;
class WNDCLASS;

namespace pxl { namespace system {

    #include <string>
    #include <vector>

    #include "system/Event.h"
    #include "system/WindowImpl.h"

    class Win32Window : public WindowImpl {

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
		    virtual void create_window(int win_width, int win_height, std::string win_title);

		    virtual bool poll_event(Event& e);

		    /**
		    \*brief: frees all data from the window
		    **/
		    virtual void free();

		    virtual void display();

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