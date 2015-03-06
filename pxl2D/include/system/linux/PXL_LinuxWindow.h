#ifndef _PXL_LINUXWINDOW_H
#define _PXL_LINUXWINDOW_H

#include <string>
#include <vector>
#include "system/PXL_Event.h"
#include "system/PXL_WindowImpl.h"

class PXL_LinuxWindow : public PXL_WindowImpl {

	public:
		PXL_LinuxWindow() { }
		/**
		\*brief: window deconstructor
		**/
		~PXL_LinuxWindow();

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