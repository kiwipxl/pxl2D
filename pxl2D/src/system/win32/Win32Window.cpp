#include "system/win32/Win32Window.h"

#if defined(PLATFORM_WIN32)

#include <wglew.h>
#define NOMINMAX //macro to not have the windows header define min/max so it doesn't interfere
#include <Windows.h>
#undef ABSOLUTE
#undef RELATIVE

#include "input/Keyboard.h"
#include "system/Exception.h"
#include "system/Debug.h"

namespace pxl { namespace system {

    int context_attribs[] = {
	    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
	    WGL_CONTEXT_MINOR_VERSION_ARB, 1,
	    WGL_CONTEXT_FLAGS_ARB, 0,
	    0 //end attribs list
    };

    LRESULT CALLBACK win_proc(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param);

    bool init_dummy_window = true;
    uint32 class_id = 0;

    /**----------------------------------------------------------------------------
						    Window class handling
    ----------------------------------------------------------------------------**/

    void Win32Window::register_class() {
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
            force_show_exception("Window registration failed. Error: " + get_last_error());
	    }
    }

    void Win32Window::unregister_class() {
	    UnregisterClass(win_class.lpszClassName, win_class.hInstance);
    }

    void Win32Window::create_context() {
	    if (!(device_context_handle = GetDC(win_handle))) {
		    force_show_exception("Couldn't create an openGL device context. Error: " + get_last_error());
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
		    force_show_exception("Pixel format is invalid. Error: " + get_last_error());
	    }
	    if (!SetPixelFormat(device_context_handle, pixel_format, &pix_format_desc)) {
		    force_show_exception("Failed to set pixel format. Error: " + get_last_error());
	    }
	    if (WGLEW_ARB_create_context && WGLEW_ARB_pixel_format) {
		    if (!(gl_render_context_handle = wglCreateContextAttribsARB(device_context_handle, 0, context_attribs))) {
			    force_show_exception("Failed to create openGL2+ rendering context. Error: " + get_last_error());
		    }
	    }else {
		    if (!(gl_render_context_handle = wglCreateContext(device_context_handle))) {
			    force_show_exception("Failed to create an openGL rendering context. Error: " + get_last_error());
		    }
	    }
	    if (!wglMakeCurrent(device_context_handle, gl_render_context_handle)) {
		    force_show_exception("Failed to activate an openGL rendering context. Error: " + get_last_error());
	    }
    }

    void Win32Window::create_window(int win_width, int win_height, std::string win_title) {
	    free();

	    width = win_width;
        height = win_height;
        title = win_title;

	    instance_handle = GetModuleHandle(NULL);
	    class_name = "Class" + std::to_string(class_id);
	    ++class_id;
	    win_name = title;

	    register_class();

	    //calculate adjusted window rect with border so the client rect is the same size as the specified width/height
	    tagRECT rect;
	    rect.left = 0; rect.top = 0; rect.right = win_width; rect.bottom = win_height;
	    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	    win_handle = CreateWindowEx(WS_EX_CLIENTEDGE, (LPCSTR)class_name.c_str(), (LPCSTR)win_name.c_str(), WS_OVERLAPPEDWINDOW,
								    CW_USEDEFAULT, CW_USEDEFAULT, (rect.right - rect.left) + 4, (rect.bottom - rect.top) + 4,
								    NULL, NULL, instance_handle, NULL);

	    if (win_handle == NULL) {
		    force_show_exception("Window creation failed! Error: " + get_last_error());
	    }

	    hwnd = win_handle;

	    create_context();

	    window_created = true;

	    if (init_dummy_window) {
		    init_dummy_window = false;
		    glew_init();
		    free();
		    create_window(win_width, win_height, win_title);

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
    }

    void Win32Window::display() {
	    if (window_created) {
		    if (device_context_handle != NULL) {
			    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			    SwapBuffers(device_context_handle);
		    }else {
			    show_exception("Window display failed - device context handle is NULL", ERROR_SWAP_BUFFERS_FAILED);
		    }
	    }else {
		    show_exception("Window display failed - window has not been created yet", ERROR_SWAP_BUFFERS_FAILED);
	    }
    }

    void Win32Window::free() {
	    if (window_created) {
		    window_created = false;
		    wglMakeCurrent(NULL, NULL);
		    wglDeleteContext(gl_render_context_handle);
		    DestroyWindow(win_handle);
		    unregister_class();
	    }
    }

    Win32Window::~Win32Window() {
	    free();
    }

    /**----------------------------------------------------------------------------
						    Window event handling
    ----------------------------------------------------------------------------**/

    LRESULT CALLBACK win_proc(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param) {
	    switch (msg) {
		    case EVENT_CLOSE:
			    PostQuitMessage(1);
			    break;
		    case EVENT_KEYDOWN:
			    if (w_param < 255) {
				    keys[w_param].key_down = true;
			    }
			    break;
		    case EVENT_KEYUP:
			    if (w_param < 255) {
				    keys[w_param].key_down = false;
			    }
			    break;
		    case 1200:
			    print << "received msg!\n";
			    break;
		    default:
			    return DefWindowProc(handle, msg, w_param, l_param);
	    }
	    return 0;
    }

    bool Win32Window::poll_event(Event& e) {
	    if (PeekMessage(&msg, win_handle, 0, 0, PM_REMOVE) > 0) {
		    if (num_joysticks() > 0) {
			    get_joystick(0);
			    JOYINFOEX joy_info;
			    joy_info.dwFlags = JOY_RETURNALL;
			    joyGetPosEx(1, &joy_info);
			    e.jbuttons = joy_info.dwButtons;
			    e.jnum_buttons = joy_info.dwButtonNumber;
		    }

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

#endif
}};