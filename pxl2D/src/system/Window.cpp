#include "system/Window.h"

namespace pxl { namespace sys {

    Window* createWindow(Rect rect, const char* title) {
        SDL_Window* window = SDL_CreateWindow(title, rect.x, rect.y, rect.w, rect.h, SDL_WINDOW_OPENGL);
        
        if (window) {
            SDL_GLContext context = SDL_GL_CreateContext(window);
            
            return new Window(window, context);
        }else {
            return NULL;
        }
    }

    Window::~Window() {
        SDL_GL_DeleteContext(contextHandle);
        SDL_DestroyWindow(window);
    }

    uint32 Window::getWidth() {
        int width;
        SDL_GetWindowSize(window, &width, NULL);
        return width;
    }

    uint32 Window::getHeight() {
        int height;
        SDL_GetWindowSize(window, NULL, &height);
        return height;
    }
}};
