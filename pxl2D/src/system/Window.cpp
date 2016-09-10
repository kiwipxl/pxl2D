#include "Window.h"

namespace pxl { namespace sys {
    
    Window* createWindow(uint32 width, uint32 height, const char* title, GLFWmonitor* monitor) {
        GLFWwindow* handle = glfwCreateWindow(width, height, title, monitor, NULL);

        return new Window(handle);
    }
}};
