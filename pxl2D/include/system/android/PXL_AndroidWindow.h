#ifndef _PXL_ANDROIDWINDOW_H
#define _PXL_ANDROIDWINDOW_H

#include <string>
#include <vector>
#include "system/PXL_Event.h"
#include "system/PXL_WindowImpl.h"
#include "system/PXL_API.h"

struct android_app;
struct engine engine;

extern struct android_app* android_state;

class PXL_AndroidWindow : public PXL_WindowImpl {

	public:
		PXL_AndroidWindow() { }
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