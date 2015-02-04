#include "PXL_Window.h"
#include <iostream>
#include <glew.h>
#include <wglew.h>
#include <algorithm>
#include "PXL_Graphics.h"
#include "PXL_System.h"

int PXL_window_width;
int PXL_window_height;
int PXL_center_window_x;
int PXL_center_window_y;
std::vector<PXL_Window*> PXL_windows;
bool init_dummy_window = true;

int context_attribs[] = {
	WGL_CONTEXT_MAJOR_VERSION_ARB, 3, 
	WGL_CONTEXT_MINOR_VERSION_ARB, 1, 
	WGL_CONTEXT_FLAGS_ARB, 0, 
	0 //end attribs list
};

LRESULT CALLBACK win_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_CLOSE:
			DestroyWindow(hwnd);
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
}

PXL_Window* PXL_create_window(int window_width, int window_height, std::string title) {
	return new PXL_Window(window_width, window_height, title);
}

void PXL_swap_buffers(PXL_Window* window) {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	SwapBuffers(window->device_context_handle);
}

void PXL_swap_buffers(int window_index) {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	SwapBuffers(PXL_windows[window_index]->device_context_handle);
}

void PXL_Window::create_window(int window_width, int window_height, std::string title) {
	free();

	instance_handle = GetModuleHandle(NULL);
	std::string hash = PXL_sha256(std::to_string(PXL_windows.size())).substr(0, 16);
	class_name = hash.c_str();
	window_name = title.c_str();

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

	//calculate adjusted window rect with border so the client rect is the same size as the specified width/height
	tagRECT rect;
	rect.left = 0; rect.top = 0; rect.right = window_width; rect.bottom = window_height;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	PXL_window_width = window_width;
	PXL_window_height = window_height;
	PXL_center_window_x = PXL_window_width / 2;
	PXL_center_window_y = PXL_window_height / 2;

	if (!RegisterClass(&win_class)) {
		PXL_force_show_exception("Window registration failed. Error: " + PXL_get_os_error());
	}

	win_handle = CreateWindowEx(WS_EX_CLIENTEDGE, class_name, window_name, WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, CW_USEDEFAULT, (rect.right - rect.left) + 4, (rect.bottom - rect.top) + 4,
								NULL, NULL, instance_handle, NULL);

	if (win_handle == NULL) {
		PXL_force_show_exception("Window creation failed! Error: " + PXL_get_os_error());
	}
	if (!(device_context_handle = GetDC(win_handle))) {
		PXL_force_show_exception("Couldn't create an openGL device context. Error: " + PXL_get_os_error());
	}

	PIXELFORMATDESCRIPTOR pix_format_desc;
	memset(&pix_format_desc, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pix_format_desc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pix_format_desc.nVersion = 1;
	pix_format_desc.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pix_format_desc.iPixelType = PFD_TYPE_RGBA;
	pix_format_desc.cColorBits = 32;
	pix_format_desc.cDepthBits = 32;
	pix_format_desc.iLayerType = PFD_MAIN_PLANE;

	if (!(pixel_format = ChoosePixelFormat(device_context_handle, &pix_format_desc))) {
		PXL_force_show_exception("Pixel format is invalid. Error: " + PXL_get_os_error());
	}
	if (!SetPixelFormat(device_context_handle, pixel_format, &pix_format_desc)) {
		PXL_force_show_exception("Failed to set pixel format. Error: " + PXL_get_os_error());
	}
	if (WGLEW_ARB_create_context && WGLEW_ARB_pixel_format) {
		if (!(gl_render_context_handle = wglCreateContextAttribsARB(device_context_handle, 0, context_attribs))) {
			PXL_force_show_exception("Failed to create openGL2+ rendering context. Error: " + PXL_get_os_error());
		}
	}else {
		if (!(gl_render_context_handle = wglCreateContext(device_context_handle))) {
			PXL_force_show_exception("Failed to create an openGL rendering context. Error: " + PXL_get_os_error());
		}
	}
	if (!wglMakeCurrent(device_context_handle, gl_render_context_handle)) {
		PXL_force_show_exception("Failed to activate an openGL rendering context. Error: " + PXL_get_os_error());
	}

	window_loaded = true;

	if (init_dummy_window) {
		init_dummy_window = false;
		PXL_glew_init();
		free();
		create_window(window_width, window_height, title);

		int min;
		glGetIntegerv(GL_MINOR_VERSION, &min);
		int target;
		glGetIntegerv(GL_MAJOR_VERSION, &target);
		std::cout << "updated gl version: " << glGetString(GL_VERSION) << ", min: " << min << ", target: " << target << "\n";
		std::cout << "updated glsl version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
		return;
	}else {
		ShowWindow(win_handle, SW_SHOW);
		UpdateWindow(win_handle);
	}

	PXL_windows.push_back(this);
}

void PXL_Window::free() {
	if (window_loaded) {
		window_loaded = false;
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(gl_render_context_handle);
		DestroyWindow(win_handle);
		UnregisterClass(win_class.lpszClassName, win_class.hInstance);

		PXL_windows.erase(std::remove(PXL_windows.begin(), PXL_windows.end(), this), PXL_windows.end());
	}
}

PXL_Window::~PXL_Window() {
	free();
}