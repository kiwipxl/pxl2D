#include "PXL_Window.h"
#include <iostream>

int PXL_window_width;
int PXL_window_height;
int PXL_center_window_x;
int PXL_center_window_y;
std::vector<PXL_Window*> PXL_windows;

PIXELFORMATDESCRIPTOR pix_format_desc =                  // pix_format_desc Tells Windows How We Want Things To Be
{
	sizeof(PIXELFORMATDESCRIPTOR),                  // Size Of This Pixel Format Descriptor
	1,                              // Version Number
	PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
	PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
	PFD_DOUBLEBUFFER,                       // Must Support Double Buffering
	PFD_TYPE_RGBA,                          // Request An RGBA Format
	32,                               // Select Our Color Depth
	0, 0, 0, 0, 0, 0,                       // Color Bits Ignored
	0,                              // No Alpha Buffer
	0,                              // Shift Bit Ignored
	0,                              // No Accumulation Buffer
	0, 0, 0, 0,                         // Accumulation Bits Ignored
	16,                             // 16Bit Z-Buffer (Depth Buffer)
	0,                              // No Stencil Buffer
	0,                              // No Auxiliary Buffer
	PFD_MAIN_PLANE,                         // Main Drawing Layer
	0,                              // Reserved
	0, 0, 0                             // Layer Masks Ignored
};

LRESULT CALLBACK win_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

PXL_Window::PXL_Window(int window_width, int window_height, std::string title) {
	window_loaded = false;
	create_window(window_width, window_height, title);
	PXL_windows.push_back(this);
}

PXL_Window* PXL_create_window(int window_width, int window_height, std::string title) {
	return new PXL_Window(window_width, window_height, title);
}

void PXL_swap_buffers(PXL_Window* window) {
	SwapBuffers(window->device_context_handle);
}

void PXL_swap_buffers(int window_index) {
	SwapBuffers(PXL_windows[window_index]->device_context_handle);
}

void PXL_Window::create_window(int window_width, int window_height, std::string title) {
	free();

	instance_handle = GetModuleHandle(NULL);
	class_name = title.c_str();
	window_name = title.c_str();

	win_class.cbSize = sizeof(WNDCLASSEX);
	win_class.style = CS_DROPSHADOW | CS_OWNDC;
	win_class.lpfnWndProc = win_proc;
	win_class.cbClsExtra = 0;
	win_class.cbWndExtra = 0;
	win_class.hInstance = instance_handle;
	win_class.hIcon = LoadIcon(NULL, IDI_INFORMATION);
	win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	win_class.hbrBackground = 0;
	win_class.lpszMenuName = 0;
	win_class.lpszClassName = class_name;
	win_class.hIconSm = LoadIcon(NULL, IDI_INFORMATION);

	//calculate adjusted window rect with border so the client rect is the same size as the specified width/height
	tagRECT rect;
	rect.left = 0; rect.top = 0; rect.right = window_width; rect.bottom = window_height;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	PXL_window_width = window_width;
	PXL_window_height = window_height;
	PXL_center_window_x = PXL_window_width / 2;
	PXL_center_window_y = PXL_window_height / 2;

	if (!RegisterClassEx(&win_class)) {
		MessageBox(NULL, "Window registration failed", "An error occurred!", MB_ICONEXCLAMATION | MB_OK);
	}

	win_handle = CreateWindowEx(WS_EX_CLIENTEDGE, class_name, window_name, WS_OVERLAPPEDWINDOW, 
							    CW_USEDEFAULT, CW_USEDEFAULT, (rect.right - rect.left) + 8, (rect.bottom - rect.top) + 8,  
								NULL, NULL, instance_handle, NULL);

	if (win_handle == NULL) {
		MessageBox(NULL, "Window creation failed!", "An error occurred", MB_ICONEXCLAMATION | MB_OK);
	}

	if (!(device_context_handle = GetDC(win_handle))) {
		MessageBox(NULL, "Couldn't create an openGL device context", "An error occurred", MB_ICONEXCLAMATION | MB_OK);
	}

	if (!(pixel_format = ChoosePixelFormat(device_context_handle, &pix_format_desc))) {
		MessageBox(NULL, "Pixel format is invalid", "An error occurred", MB_ICONEXCLAMATION | MB_OK);
	}

	if (!SetPixelFormat(device_context_handle, pixel_format, &pix_format_desc)) {
		MessageBox(NULL, "Failed to set pixel format", "An error occurred", MB_ICONEXCLAMATION | MB_OK);
	}

	if (!(gl_render_context_handle = wglCreateContext(device_context_handle))) {
		MessageBox(NULL, "Failed to create an openGL rendering context", "An error occurred", MB_ICONEXCLAMATION | MB_OK);
	}

	if (!wglMakeCurrent(device_context_handle, gl_render_context_handle)) {
		MessageBox(NULL, "Failed to activate an openGL rendering context", "An error occurred", MB_ICONEXCLAMATION | MB_OK);
	}

	ShowWindow(win_handle, SW_SHOW);
	UpdateWindow(win_handle);

	window_loaded = true;
}

void PXL_Window::free() {
	if (window_loaded) {
		window_loaded = false;
	}
}

PXL_Window::~PXL_Window() {
	free();
}