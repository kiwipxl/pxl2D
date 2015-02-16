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
int class_id; //used to register a unique class name

int context_attribs[] = {
	WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
	WGL_CONTEXT_MINOR_VERSION_ARB, 1,
	WGL_CONTEXT_FLAGS_ARB, 0,
	0 //end attribs list
};

LRESULT CALLBACK win_proc(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param);

/**----------------------------------------------------------------------------
						Window class handling
----------------------------------------------------------------------------**/

PXL_Window::PXL_Window(int window_width, int window_height, std::string title) {
	window_loaded = false;
	create_window(window_width, window_height, title);
}

PXL_Window* PXL_create_window(int window_width, int window_height, std::string title) {
	return new PXL_Window(window_width, window_height, title);
}

void PXL_Window::register_class() {
	win_class.style = CS_DROPSHADOW | CS_OWNDC;
	win_class.lpfnWndProc = win_proc;
	win_class.cbClsExtra = 0;
	win_class.cbWndExtra = 0;
	win_class.hInstance = instance_handle;
	win_class.hIcon = LoadIcon(NULL, IDI_INFORMATION);
	win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	win_class.hbrBackground = 0;
	win_class.lpszMenuName = 0;
	win_class.lpszClassName = (LPCSTR)class_name.c_str();

	if (!RegisterClass(&win_class)) {
		PXL_force_show_exception("Window registration failed. Error: " + PXL_get_os_error());
	}
}

void PXL_Window::unregister_class() {
	UnregisterClass(win_class.lpszClassName, win_class.hInstance);
}

void PXL_Window::create_context() {
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
}

void PXL_Window::create_window(int window_width, int window_height, std::string title) {
	free();

	instance_handle = GetModuleHandle(NULL);
	class_name = "PXL_Class" + std::to_string(class_id);
	++class_id;
	win_name = title;

	register_class();

	//calculate adjusted window rect with border so the client rect is the same size as the specified width/height
	tagRECT rect;
	rect.left = 0; rect.top = 0; rect.right = window_width; rect.bottom = window_height;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	PXL_window_width = window_width;
	PXL_window_height = window_height;
	PXL_center_window_x = PXL_window_width / 2;
	PXL_center_window_y = PXL_window_height / 2;

	win_handle = CreateWindowEx(WS_EX_CLIENTEDGE, (LPCSTR)class_name.c_str(), (LPCSTR)win_name.c_str(), WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, CW_USEDEFAULT, (rect.right - rect.left) + 4, (rect.bottom - rect.top) + 4,
								NULL, NULL, instance_handle, NULL);

	if (win_handle == NULL) {
		PXL_force_show_exception("Window creation failed! Error: " + PXL_get_os_error());
	}

	create_context();

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
		unregister_class();

		PXL_windows.erase(std::remove(PXL_windows.begin(), PXL_windows.end(), this), PXL_windows.end());
	}
}

PXL_Window::~PXL_Window() {
	free();
}

/**----------------------------------------------------------------------------
						Window event handling
----------------------------------------------------------------------------**/

LRESULT CALLBACK win_proc(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param) {
	if (msg == 4) {
		int a = 5;
	}

	int a;
	switch (msg) {
		case WM_CLOSE:
			PostQuitMessage(1);
			break;
		case WM_KEYDOWN:
			a = 5;
			break;
		default:
			return DefWindowProc(handle, msg, w_param, l_param);
	}
	return 0;
}

bool PXL_Window::poll_event(PXL_Event& e) {
	if (PeekMessage(&msg, win_handle, 0, 0, PM_REMOVE) > 0) {
		PXL_get_joystick(0);
		JOYINFOEX joy_info;
		joy_info.dwFlags = JOY_RETURNALL;
		joyGetPosEx(1, &joy_info);
		e.jbuttons = joy_info.dwButtons;
		e.jnum_buttons = joy_info.dwButtonNumber;

		e.mouse_x = LOWORD(msg.lParam);
		e.mouse_y = HIWORD(msg.lParam);

		e.key_code = msg.wParam;
		e.type = msg.message;
		e.time = msg.time;

		if (e.jbuttons == 1) {
			SendMessage(win_handle, 4, 1, 0);
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		return true;
	}
	return false;
}

/**----------------------------------------------------------------------------
						Global function handling
----------------------------------------------------------------------------**/

void PXL_swap_buffers(PXL_Window* window) {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	SwapBuffers(window->device_context_handle);
}

void PXL_swap_buffers(int window_index) {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	SwapBuffers(PXL_windows[window_index]->device_context_handle);
}