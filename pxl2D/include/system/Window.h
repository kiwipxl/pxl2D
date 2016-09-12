#ifndef _WINDOW_H_
#define _WINDOW_H_

/*
* abstracts away the GLFWwindow type from the engine user. This is because, if we wanted
* to change our implementation later to not use glfw, we only need to implement it inside this
* window class.
*
* note: we can grab the handle object for glfw functions that are not interfaced here
*/

#include "PXLAPI.h"

namespace pxl { namespace sys {

    class Window {

    public:
        Window(SDL_Window* _window, SDL_GLContext _contextHandle) {
            window = _window;
            contextHandle = _contextHandle;
        }
        ~Window();

        uint32 getWidth();
        uint32 getHeight();

        SDL_Window* window;
        SDL_GLContext contextHandle;
    };

    extern Window* createWindow(Rect rect, const char* title);
}};

#endif
