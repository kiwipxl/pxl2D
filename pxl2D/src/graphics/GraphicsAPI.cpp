#include "graphics/GraphicsAPI.h"
#include "API.h"
#include "system/Exception.h"
#include "system/Debug.h"

namespace pxl { namespace graphics {

    extern void glew_init() {
	    #if defined(PLATFORM_WIN32)
		    glewExperimental = true;
            GLenum error;
            if ((error = glewInit()) != GL_NO_ERROR) {
                sys::force_show_exception("Could not initialise GLEW. Error: " + (std::string)(char*)glewGetErrorString(error));
            }
            sys::print << "glew initiated\n";
		    int min;
		    glGetIntegerv(GL_MINOR_VERSION, &min);
		    int target;
		    glGetIntegerv(GL_MAJOR_VERSION, &target);
            sys::print << "gl version: " << glGetString(GL_VERSION) << ", min: " << min << ", target: " << target << "\n";
            sys::print << "glsl version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
            sys::print << "vendor: " << glGetString(GL_VENDOR) << "\n";
            sys::print << "renderer: " << glGetString(GL_RENDERER) << "\n";
	    #endif
    }

    extern void set_clear_colour(float r, float g, float b, float a) {
	    glClearColor(r, g, b, a);
    }

    extern void set_clear_depth(float d) {
	    glClearDepthf(d);
    }

    extern void clear() {
	    glClear(GL_COLOR_BUFFER_BIT);
    }
}};