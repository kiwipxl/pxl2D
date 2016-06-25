#include "system/linux/LinuxWindow.h"

#if defined(PLATFORM_LINUX)

#include "graphics/GraphicsAPI.h"
#include "input/Keyboard.h"
#include "system/Exception.h"
#include "system/Debug.h"

namespace pxl { namespace sys {

    /**----------------------------------------------------------------------------
						    Window class handling
    ----------------------------------------------------------------------------**/

    WindowBase& LinuxWindow::create(int window_width, int window_height, std::string title) {
	    width = window_width;
	    height = window_height;

        return *this;
    }

    void LinuxWindow::display() {
	    if (window_created) {
	    }else {
		    show_exception("Window display failed - window has not been created yet", ERROR_SWAP_BUFFERS_FAILED);
	    }
    }

    bool LinuxWindow::poll_event(Event& e) {
	    return false;
    }

    void LinuxWindow::free() {
	    if (window_created) {
		    window_created = false;
	    }
    }

    LinuxWindow::~LinuxWindow() {
	    free();
    }
}};

#endif