#include "linux/PXL_LinuxWindow.h"
#include "PXL_Graphics.h"
#include "PXL_System.h"
#include "input/PXL_Keyboard.h"

/**----------------------------------------------------------------------------
						Window class handling
----------------------------------------------------------------------------**/

void PXL_LinuxWindow::create_window(int window_width, int window_height, std::string title) {

}

void PXL_LinuxWindow::display() {
	if (window_created) {
	}else {
		PXL_show_exception("Window display failed - window has not been created yet", PXL_ERROR_SWAP_BUFFERS_FAILED);
	}
}

void PXL_LinuxWindow::free() {
	if (window_created) {
		window_created = false;
	}
}

PXL_LinuxWindow::~PXL_LinuxWindow() {
	free();
}