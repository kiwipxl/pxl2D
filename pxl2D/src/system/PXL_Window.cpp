#include "PXL_Window.h"
#include <iostream>

PXL_Window::PXL_Window(int window_width, int window_height, std::string title) {
	window_created = false;
	create(window_width, window_height, title);
}

//bool PXL_Window::poll_event(PXL_Event& e) {
//
//}
//
//void PXL_Window::free() {
//
//}
//
//PXL_Window::~PXL_Window() {
//	free();
//}