#include "PXL.h"
#include <Windows.h>

int PXL_screen_width;
int PXL_screen_height;
int PXL_center_screen_x;
int PXL_center_screen_y;

HDC PXL_handle_dc;

static PIXELFORMATDESCRIPTOR pix_format_desc =                  // pix_format_desc Tells Windows How We Want Things To Be
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

void PXL_init(int screen_width, int screen_height) {
	PXL_screen_width = screen_width;
	PXL_screen_height = screen_height;
	PXL_center_screen_x = PXL_screen_width / 2;
	PXL_center_screen_y = PXL_screen_height / 2;

	HINSTANCE instance_handle = GetModuleHandle(NULL);
	WNDCLASSEX win_class;
	HWND win_handle;
	MSG msg;
	LPCSTR class_name = "classname";
	LPCSTR window_name = "window title";

	win_class.cbSize = sizeof(WNDCLASSEX);
	win_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
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

	if (!RegisterClassEx(&win_class)) {
		MessageBox(NULL, "Window registration failed", "An error occurred!", MB_ICONEXCLAMATION | MB_OK);
	}

	win_handle = CreateWindowEx(WS_EX_CLIENTEDGE, class_name, window_name, WS_OVERLAPPEDWINDOW, 
							    CW_USEDEFAULT, CW_USEDEFAULT, PXL_screen_width, PXL_screen_height,
								NULL, NULL, instance_handle, NULL);

	if (win_handle == NULL) {
		MessageBox(NULL, "Window creation failed!", "An error occurred", MB_ICONEXCLAMATION | MB_OK);
	}

	if (!(PXL_handle_dc = GetDC(win_handle))) {
		MessageBox(NULL, "Couldn't create an openGL device context", "An error occurred", MB_ICONEXCLAMATION | MB_OK);
	}

	int pixel_format;
	if (!(pixel_format = ChoosePixelFormat(PXL_handle_dc, &pix_format_desc))) {
		MessageBox(NULL, "Pixel format is invalid", "An error occurred", MB_ICONEXCLAMATION | MB_OK);
	}

	if (!SetPixelFormat(PXL_handle_dc, pixel_format, &pix_format_desc)) {
		MessageBox(NULL, "Failed to set pixel format", "An error occurred", MB_ICONEXCLAMATION | MB_OK);
	}

	HGLRC handle_rc;
	if (!(handle_rc = wglCreateContext(PXL_handle_dc))) {
		MessageBox(NULL, "Failed to create an openGL rendering context", "An error occurred", MB_ICONEXCLAMATION | MB_OK);
	}

	if (!wglMakeCurrent(PXL_handle_dc, handle_rc)) {
		MessageBox(NULL, "Failed to activate an openGL rendering context", "An error occurred", MB_ICONEXCLAMATION | MB_OK);
	}

	GLenum error;
	if ((error = glewInit()) != GLEW_OK) {
		MessageBox(NULL, (char*)glewGetErrorString(error), "An error occurred!", MB_ICONEXCLAMATION | MB_OK);
	}
	cout << "glew initiated\n";
	cout << "gl version: " << glGetString(GL_VERSION) << "\n";
	cout << "glsl version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

	ShowWindow(win_handle, SW_SHOW);
	UpdateWindow(win_handle);

	PXL_shader_init();
	PXL_font_init();
	PXL_lights_init();
}