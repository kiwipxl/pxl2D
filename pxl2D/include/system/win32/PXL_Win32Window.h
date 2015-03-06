#ifndef _PXL_WIN32WINDOW_H
#define _PXL_WIN32WINDOW_H

#include <string>
#include <vector>
#define NOMINMAX //macro to not have the windows header define min/max so it doesn't interfere
#include <Windows.h>
#include "system/PXL_Event.h"
#include "system/PXL_WindowImpl.h"
#include "system/PXL_API.h"

class PXL_Win32Window : public PXL_WindowImpl {

	public:
		PXL_Win32Window() { }
		/**
		\*brief: window deconstructor
		**/
		~PXL_Win32Window();

		//window info
		int pixel_format;
		HDC device_context_handle;
		HGLRC gl_render_context_handle;
		HINSTANCE instance_handle;
		WNDCLASS win_class;
		HWND win_handle;
		std::string class_name;
		std::string win_name;

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
		MSG msg;

		void register_class();
		void unregister_class();
		void create_context();
};

#endif