#ifndef _WINDOW_H_
#define _WINDOW_H_

/*
* abstracts away the GLFWwindow type from the engine user. This is because, if we wanted
* to change our implementation later to not use glfw, we only need to implement it inside this
* window class.
*
* note: we can grab the handle object for glfw functions that are not interfaced here
*/

#include "graphics/GraphicsAPI.h"
#include "API.h"

namespace pxl { namespace sys {

    class Window {

    public:
        Window(GLFWwindow* _handle) {
            handle = _handle;
        }

        uint32 getWidth() {
            int width;
            glfwGetWindowSize(handle, &width, NULL);
            return width;
        }

        uint32 getHeight() {
            int height;
            glfwGetWindowSize(handle, NULL, &height);
            return height;
        }

        GLFWwindow* handle;
    };

    extern Window* createWindow(uint32 width, uint32 height, const char* title, GLFWmonitor* monitor);
}};

#endif
