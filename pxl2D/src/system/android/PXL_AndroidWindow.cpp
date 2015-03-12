#include "system/android/PXL_AndroidWindow.h"

#if defined(PXL_PLATFORM_ANDROID)

#include <jni.h>
#include <errno.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include <string>
#include <iostream>

#include "graphics/PXL_GraphicsAPI.h"
#include "input/PXL_Keyboard.h"
#include "system/PXL_Exception.h"
#include "system/PXL_Debug.h"

//defines to make classes a little more readable
#define AndroidApp android_app
#define AndroidPollSource android_poll_source

AndroidApp* android_state;
AppSavedState saved_state;
AppWinData win_data;
PXL_AndroidWindow* window = NULL;

void swap_buffers() {
	if (win_data.display != EGL_NO_DISPLAY) {
		eglSwapBuffers(win_data.display, win_data.surface);
	}
}

/**
* Initialize an EGL context for the current display.
*/
int win_data_init_display(AppWinData* win_data) {
	// initialize OpenGL ES and EGL

	/*
	* Here specify the attributes of the desired configuration.
	* Below, we select an EGLConfig with at least 8 bits per color
	* component compatible with on-screen windows
	*/

	PXL_print << "win_data initialising...\n";

	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};
	EGLint w, h, dummy, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/* Here, the application chooses the configuration it desires. In this
	* sample, we have a very simplified selection process, where we pick
	* the first EGLConfig that matches our criteria */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	* As soon as we picked a EGLConfig, we can safely reconfigure the
	* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(win_data->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, win_data->app->window, NULL);
	context = eglCreateContext(display, config, NULL, NULL);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		PXL_force_show_exception("Unable to eglMakeCurrent");
		return -1;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	win_data->display = display;
	win_data->context = context;
	win_data->surface = surface;
	win_data->width = w;
	win_data->height = h;
	win_data->state.angle = 0;

	//todo: these were prob removed in gles2
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	//glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	PXL_print << "initialised successfully\n";

	return 0;
}

/**
* Process the next input event.
*/
int32_t win_data_handle_input(AndroidApp* app, AInputEvent* event) {
	AppWinData* win_data = (AppWinData*)app->userData;
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		win_data->in_focus = true;
		win_data->state.x = AMotionEvent_getX(event, 0);
		win_data->state.y = AMotionEvent_getY(event, 0);
		return 1;
	}
	return 0;
}

/**
* Process the next main command.
*/
void win_data_handle_cmd(AndroidApp* app, int32_t cmd) {
	AppWinData* win_data = (AppWinData*)app->userData;
	PXL_print << "cmd: " << cmd << "\n";
	switch (cmd) {
		case APP_CMD_SAVE_STATE:
			// The system has asked us to save our current state.  Do so.
			win_data->app->savedState = malloc(sizeof(AppSavedState));
			*((AppSavedState*)win_data->app->savedState) = win_data->state;
			win_data->app->savedStateSize = sizeof(AppSavedState);
			break;
		case APP_CMD_INIT_WINDOW:
			// The window is being shown, get it ready.
			PXL_print << "initwindow!\n";
			if (win_data->app->window != NULL) {
				PXL_print << "inited\n";
				win_data_init_display(win_data);
				swap_buffers();
			}
			break;
		case APP_CMD_TERM_WINDOW:
			// The window is being hidden or closed, clean it up.
			window->free();
			break;
		case APP_CMD_GAINED_FOCUS:
			// When our app gains focus, we start monitoring the accelerometer.
			if (win_data->accelerometerSensor != NULL) {
				ASensorEventQueue_enableSensor(win_data->sensorEventQueue,
					win_data->accelerometerSensor);
				// We'd like to get 60 events per second (in us).
				ASensorEventQueue_setEventRate(win_data->sensorEventQueue,
					win_data->accelerometerSensor, (1000L / 60) * 1000);
			}
			break;
		case APP_CMD_LOST_FOCUS:
			// When our app loses focus, we stop monitoring the accelerometer.
			// This is to avoid consuming battery while not being used.
			if (win_data->accelerometerSensor != NULL) {
				ASensorEventQueue_disableSensor(win_data->sensorEventQueue,
					win_data->accelerometerSensor);
			}
			// Also stop in_focus.
			win_data->in_focus = false;
			swap_buffers();
			break;
	}
}

PXL_AndroidWindow::PXL_AndroidWindow() {

}

void PXL_AndroidWindow::create_window(int window_width, int window_height, std::string title) {
	free();

	width = window_width;
	height = window_height;

	// Make sure glue isn't stripped.
	app_dummy();

	android_state->userData = &win_data;
	android_state->onAppCmd = win_data_handle_cmd;
	android_state->onInputEvent = win_data_handle_input;
	win_data.app = android_state;

	// Prepare to monitor accelerometer
	win_data.sensorManager = ASensorManager_getInstance();
	win_data.accelerometerSensor = ASensorManager_getDefaultSensor(win_data.sensorManager,
		ASENSOR_TYPE_ACCELEROMETER);
	win_data.sensorEventQueue = ASensorManager_createEventQueue(win_data.sensorManager,
		android_state->looper, LOOPER_ID_USER, NULL, NULL);

	if (android_state->savedState != NULL) {
		// We are starting with a previous saved state; restore from it.
		win_data.state = *(AppSavedState*)android_state->savedState;
	}

	window_created = true;
	win_data.in_focus = true;
}

void PXL_AndroidWindow::display() {
	if (window_created) {
		swap_buffers();
	}else {
		PXL_show_exception("Window display failed - window has not been created yet", PXL_ERROR_SWAP_BUFFERS_FAILED);
	}
}

bool PXL_AndroidWindow::poll_event(PXL_Event& e) {
	if (!win_data.in_focus) { return false; }

	//read all pending events
	int ident;
	int events;
	AndroidPollSource* source;

	if ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) {

		PXL_print << "event14: " << win_data.in_focus << "\n";

		// Process this event.
		if (source != NULL) {
			source->process(android_state, source);
		}

		// If a sensor has data, process it now.
		if (ident == LOOPER_ID_USER) {
			if (win_data.accelerometerSensor != NULL) {
				ASensorEvent event;
				while (ASensorEventQueue_getEvents(win_data.sensorEventQueue, &event, 1) > 0) {
					/*PXL_print << "accelerometer: " << "  x = " << event.acceleration.x 
												   << ", y = " << event.acceleration.y 
												   << ", z = " << event.acceleration.z;*/
				}
			}
		}

		// Check if we are exiting.
		if (android_state->destroyRequested != 0) {
			free();
			return false;
		}
	}
	return false;
}

void PXL_AndroidWindow::free() {
	if (window_created) {
		window_created = false;

		if (win_data.display != EGL_NO_DISPLAY) {
			eglMakeCurrent(win_data.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			if (win_data.context != EGL_NO_CONTEXT) {
				eglDestroyContext(win_data.display, win_data.context);
			}
			if (win_data.surface != EGL_NO_SURFACE) {
				eglDestroySurface(win_data.display, win_data.surface);
			}
			eglTerminate(win_data.display);
		}
		win_data.in_focus = false;
		win_data.display = EGL_NO_DISPLAY;
		win_data.context = EGL_NO_CONTEXT;
		win_data.surface = EGL_NO_SURFACE;
	}
}

PXL_AndroidWindow::~PXL_AndroidWindow() {
	free();
}

#endif