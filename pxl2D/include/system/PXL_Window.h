#ifndef _PXL_WINDOW_H
#define _PXL_WINDOW_H

#include <string>
#include <vector>
#include <Windows.h>

class PXL_Window;

extern int PXL_window_width;
extern int PXL_window_height;
extern int PXL_center_window_x;
extern int PXL_center_window_y;
extern std::vector<PXL_Window*> PXL_windows;

/**
\*brief: loads and creates a window from the specified path
\*param [path]: the path and file name for the window to load
**/
extern PXL_Window* PXL_create_window(int window_width, int window_height, std::string title);
extern void PXL_swap_buffers(PXL_Window* window);
extern void PXL_swap_buffers(int window_index = 0);

class PXL_Window {

	public:
		PXL_Window();
		/**
		\*brief: creates the window with specified values
		\*param [width]: the width of the image
		\*param [height]: the height of the image
		\*param [window]: an array of pixels for the image
		**/
		PXL_Window(int window_width, int window_height, std::string title);
		/**
		\*brief: window deconstructor
		**/
		~PXL_Window();

		//window info
		int width;
		int height;
		int pixel_format;
		HDC device_context_handle;
		HGLRC gl_render_context_handle;
		HINSTANCE instance_handle;
		WNDCLASSEX win_class;
		HWND win_handle;
		MSG msg;
		LPCSTR class_name;
		LPCSTR window_name;

		/**
		\*brief: loads a window from the specified path
		\*param [path]: the path and file name for the window to load
		**/
		void create_window(int window_width, int window_height, std::string title);

		/**
		\*brief: frees all data from the window
		**/
		void free();

	private:
		//window info
		bool window_loaded;
};

#endif