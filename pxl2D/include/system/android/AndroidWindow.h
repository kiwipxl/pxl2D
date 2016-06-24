#ifndef _ANDROIDWINDOW_H
#define _ANDROIDWINDOW_H

#include "graphics/GraphicsAPI.h"

namespace pxl { namespace system {

    #if defined(PLATFORM_ANDROID)

    #include <string>
    #include <vector>
    #include "system/Event.h"
    #include "system/WindowImpl.h"
    #include "API.h"

    struct android_app;
    struct ASensorManager;
    struct ASensor;
    struct ASensorEventQueue;
    typedef void* EGLDisplay;
    typedef void* EGLSurface;
    typedef void* EGLContext;

    /**
    * Our saved state data.
    */
    struct AppSavedState {
	    float angle;
	    int32_t x;
	    int32_t y;
    };

    /**
    * Shared state for our app.
    */
    struct AppWinData {
	    android_app* app;

	    ASensorManager* sensorManager;
	    const ASensor* accelerometerSensor;
	    ASensorEventQueue* sensorEventQueue;

	    int in_focus;
	    EGLDisplay display;
	    EGLSurface surface;
	    EGLContext context;
	    int32_t width;
	    int32_t height;
	    AppSavedState state;
	    TouchEvent touch_event;
    };

    extern struct android_app* android_state;

    class AndroidWindow : public WindowImpl {

	    public:
		    AndroidWindow();
		    /**
		    \*brief: window deconstructor
		    **/
		    ~AndroidWindow();

		    /**
		    \*brief: loads a window from the specified path
		    \*param [path]: the path and file name for the window to load
		    **/
		    virtual void create_window(int window_width, int window_height, std::string title);

		    virtual bool poll_event(Event& e);

		    /**
		    \*brief: frees all data from the window
		    **/
		    virtual void free();

		    virtual void display();

	    private:
		    //window info
    };

    #endif
}};
#endif