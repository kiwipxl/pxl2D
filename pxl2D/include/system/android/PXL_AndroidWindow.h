#ifndef _PXL_ANDROIDWINDOW_H
#define _PXL_ANDROIDWINDOW_H

#include "graphics/PXL_GraphicsAPI.h"

#if defined(PXL_PLATFORM_ANDROID)

#include <string>
#include <vector>
#include "system/PXL_Event.h"
#include "system/PXL_WindowImpl.h"
#include "system/PXL_API.h"

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
	PXL_TouchEvent touch_event;
};

extern struct android_app* android_state;

class PXL_AndroidWindow : public PXL_WindowImpl {

	public:
		PXL_AndroidWindow();
		/**
		\*brief: window deconstructor
		**/
		~PXL_AndroidWindow();

		/**
		\*brief: loads a window from the specified path
		\*param [path]: the path and file name for the window to load
		**/
		virtual void create_window(int window_width, int window_height, std::string title);

		virtual bool poll_event(PXL_Event& e);

		/**
		\*brief: frees all data from the window
		**/
		virtual void free();

		virtual void display();

	private:
		//window info
};

#endif
#endif