#ifndef _PXL_WINDOW_IMPL_H
#define _PXL_WINDOW_IMPL_H

#include <string>
#include "system/PXL_Event.h"
#include "system/PXL_API.h"

class PXL_WindowImpl {

	public:
		/**
		\*brief: loads a window from the specified path
		\*param [path]: the path and file name for the window to load
		**/
		virtual void create_window(int window_width, int window_height, std::string title) = 0;

		virtual bool poll_event(PXL_Event& e) = 0;

		virtual void display() = 0;

		/**
		\*brief: frees all data from the window
		**/
		virtual void free() = 0;

		int get_width() { return width; }
		int get_height() { return height; }
		std::string get_title() { return title; }

	protected:
		//window info
		bool window_created = false;
		int width;
		int height;
		std::string title;
};

#endif