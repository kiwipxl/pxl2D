#ifndef _PXL_WINDOW_H
#define _PXL_WINDOW_H

#include <string>
#include "system/PXL_Event.h"

#if defined(PXL_SYSTEM_WINDOWS)
	#include "win32/PXL_Win32Window.h"
	typedef PXL_Win32Window WinImplClass;
#elif defined(PXL_SYSTEM_ANDROID)
	#include "android/PXL_AndroidWindow.h"
	typedef PXL_AndroidWindow WinImplClass;
#elif defined (PXL_SYSTEM_LINUX)
	#include "linux/PXL_LinuxWindow.h"
	typedef PXL_LinuxWindow WinImplClass;
#elif defined(PXL_SYSTEM_MAC_OS)
	#include "mac/PXL_MacWindow.h"
	typedef PXL_MacWindow WinImplClass;
#endif

class PXL_Window : public WinImplClass {

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
};

#endif