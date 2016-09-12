#include "system/Window.h"

namespace pxl { namespace sys {

    Window* createWindow(uint32 width, uint32 height, const char* title, GLFWmonitor* monitor) {
        GLFWwindow* handle = glfwCreateWindow(width, height, title, monitor, NULL);
        
        // make the context current on this window
        glfwMakeContextCurrent(handle);

        return handle ? new Window(handle) : NULL;
    }

    Window::~Window() {
        glfwDestroyWindow(handle);
    }

    uint32 Window::getWidth() {
        int width;
        glfwGetWindowSize(handle, &width, NULL);
        return width;
    }

    uint32 Window::getHeight() {
        int height;
        glfwGetWindowSize(handle, NULL, &height);
        return height;
    }
}};
