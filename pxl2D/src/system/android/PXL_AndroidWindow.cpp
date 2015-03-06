#include "android/PXL_AndroidWindow.h"
#include "graphics/PXL_GraphicsAPI.h"
#include "input/PXL_Keyboard.h"
#include "system/PXL_Exception.h"
#include "system/PXL_Debug.h"

void PXL_AndroidWindow::create_window(int window_width, int window_height, std::string title) {
	free();
}

void PXL_AndroidWindow::display() {
	if (window_created) {
	}else {
		PXL_show_exception("Window display failed - window has not been created yet", PXL_ERROR_SWAP_BUFFERS_FAILED);
	}
}

void PXL_AndroidWindow::free() {
	if (window_created) {
		window_created = false;
	}
}

PXL_AndroidWindow::~PXL_AndroidWindow() {
	free();
}