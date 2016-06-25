#include "system/Window.h"

#if defined(PLATFORM_WIN32)
    #include "system/win32/Win32Window.h"
    typedef pxl::sys::Win32Window WindowClass;
#elif defined(PLATFORM_ANDROID)
    #include "system/android/AndroidWindow.h"
    typedef pxl::sys::AndroidWindow WindowClass;
#elif defined (PLATFORM_LINUX)
    #include "system/linux/LinuxWindow.h"
    typedef pxl::sys::LinuxWindow WindowClass;
#elif defined(PLATFORM_MAC_OS)
    #include "system/osx/OSXWindow.h"
    typedef pxl::sys::OSXWindow WindowClass;
#endif

namespace pxl { namespace sys {

    Window::Window(int window_width, int window_height, std::string title) {
        base = new WindowClass();
        base->create(window_width, window_height, title);
    }

    Window::~Window() {
        delete base;
    }
}};