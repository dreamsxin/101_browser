#include <windows.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <string>

struct Window
{
	HINSTANCE	hInstance;		// Holds The Instance Of The Application
	std::wstring windowClassName;
	HWND		hWnd;		    // Holds Our Window Handle
	HDC			hDC;		    // Private GDI Device Context
	HGLRC		hGLRC;		    // Permanent Rendering Context
	bool isVisible;				// Is the window visible?

	Window(HINSTANCE in_hInstance) : hInstance(in_hInstance), hWnd(0), hDC(0), hGLRC(0), isVisible(false)
	{ }
};

LRESULT CALLBACK WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool registerWindowClass(const Window& in_window)
{
	WNDCLASSEX windowClass;												// Window Class
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));						// Make Sure Memory Is Cleared
	windowClass.cbSize			= sizeof(WNDCLASSEX);					// Size Of The windowClass Structure
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraws The Window For Any Movement / Resizing
	windowClass.lpfnWndProc		= (WNDPROC)(WndProc);					// WindowProc Handles Messages
	windowClass.hInstance		= in_window.hInstance;					// Set The Instance
	windowClass.hbrBackground	= NULL;									// Class Background Brush Color
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	windowClass.lpszClassName	= in_window.windowClassName.c_str();	// Sets The Applications Classname
	if (!RegisterClassEx (&windowClass))								// Did Registering The Class Fail?
		return false;
	return true;														// Return True (Success)
}

bool unregisterWindowClass(const Window& in_window)
{
	if (!UnregisterClass(in_window.windowClassName.c_str(), in_window.hInstance))
		return false;
	return true;
}

void ReshapeGL (int width, int height)									// Reshape The Window When It's Moved Or Resized
{
	glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				// Reset The Current Viewport
	glMatrixMode (GL_PROJECTION);										// Select The Projection Matrix
	glLoadIdentity ();													// Reset The Projection Matrix
	if (height==0)
		height=1;

	float aspect = (GLfloat)(width)/(GLfloat)(height);
	float fov = 45.0f;
	float zNear = 1.0f;
	float zFar = 100.0f;

	float top = tanf(fov*(((float) M_PI)/360.0f)) * zNear;
	float bottom = -top;
	float left = aspect * bottom;
	float right = aspect * top;

	// Avoid using gluPerspective
	glFrustum(left, right, bottom, top, zNear, zFar);
	
	glMatrixMode (GL_MODELVIEW);										// Select The Modelview Matrix
	glLoadIdentity ();													// Reset The Modelview Matrix
}

void destroyWindow(Window* in_window);

bool createWindow(Window* in_window, std::wstring in_titleText, 
				  int in_width, int in_height, BYTE in_colorBits, BYTE in_depthBits, 
				  int nCmdShow)
{
	int			PixelFormat;											// Holds The Results After Searching For A Match
	DWORD		dwExStyle = WS_EX_APPWINDOW;							// Window Extended Style
	DWORD		dwStyle = WS_OVERLAPPEDWINDOW;							// Window Style
	RECT		windowRect;												// Grabs Rectangle Upper Left / Lower Right Values
	windowRect.left=(long) 0;											// Set Left Value To 0
	windowRect.right=(long) in_width;									// Set Right Value To Requested Width
	windowRect.top=(long) 0;											// Set Top Value To 0
	windowRect.bottom=(long) in_height;									// Set Bottom Value To Requested Height

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);			// Adjust Window To True Requested Size

	in_window->hWnd = CreateWindowEx (dwExStyle,						// Extended Style
								   in_window->windowClassName.c_str(),	// Class Name
								   in_titleText.c_str(),				// Window Title
								   dwStyle,								// Window Style
								   windowRect.left,						// Window X Position
								   windowRect.top,						// Window Y Position
								   windowRect.right - windowRect.left,	// Window Width
								   windowRect.bottom - windowRect.top,	// Window Height
								   HWND_DESKTOP,						// Desktop Is Window's Parent
								   0,									// No Menu
								   in_window->hInstance,				// Pass The Window Instance
								   in_window);

	if (!in_window->hWnd)
		// Note that destroyWindow(in_window); is not necessary here
		return false;

	in_window->hDC = GetDC (in_window->hWnd);							// Grab A Device Context For This Window
	if (!in_window->hDC)												// Did We Get A Device Context?
	{
		destroyWindow(in_window);										// Destroy the window
		return false;													// Return False
	}

	PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
		1,																// Version Number
		PFD_DRAW_TO_WINDOW |											// Format Must Support Window
		PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,												// Must Support Double Buffering
		PFD_TYPE_RGBA,													// Request An RGBA Format
		in_colorBits,													// Select Our Color Depth
		0, 0, 0, 0, 0, 0,												// Color Bits Ignored
		0,																// No Alpha Buffer
		0,																// Shift Bit Ignored
		0,																// No Accumulation Buffer
		0, 0, 0, 0,														// Accumulation Bits Ignored
		in_depthBits,													// 16Bit Z-Buffer (Depth Buffer)  
		0,																// No Stencil Buffer
		0,																// No Auxiliary Buffer
		PFD_MAIN_PLANE,													// Main Drawing Layer
		0,																// Reserved
		0, 0, 0															// Layer Masks Ignored
	};

	PixelFormat = ChoosePixelFormat (in_window->hDC, &pfd);				// Find A Compatible Pixel Format
	if (!PixelFormat)													// Did We Find A Compatible Format?
	{
		destroyWindow(in_window);										// Destroy the window
		return false;													// Return False
	}

	if (!SetPixelFormat (in_window->hDC, PixelFormat, &pfd))
	{
		destroyWindow(in_window);										// Destroy the window
		return false;													// Return False
	}

	in_window->hGLRC = wglCreateContext (in_window->hDC);				// Try To Get A Rendering Context

	if (!in_window->hGLRC)												// Did We Get A Rendering Context?
	{
		destroyWindow(in_window);										// Destroy the window
		return false;													// Return False
	}

	// Make The Rendering Context Our Current Rendering Context
	if (!wglMakeCurrent (in_window->hDC, in_window->hGLRC))
	{
		destroyWindow(in_window);										// Destroy the window
		return false;													// Return False
	}

	ShowWindow (in_window->hWnd, nCmdShow);								// Make The Window Visible
	in_window->isVisible = true;

	ReshapeGL(in_width, in_height);

	return true;
}

void destroyWindow(Window* in_window)
{
	if (in_window->hWnd)												// Does The Window Have A Handle?
	{	
		if (in_window->hDC)												// Does The Window Have A Device Context?
		{
			wglMakeCurrent(in_window->hDC, 0);							// Set The Current Active Rendering Context To Zero
			if (in_window->hGLRC)										// Does The Window Have A Rendering Context?
			{
				wglDeleteContext(in_window->hGLRC);						// Release The Rendering Context
				in_window->hGLRC = 0;									// Zero The Rendering Context

			}
			ReleaseDC (in_window->hWnd, in_window->hDC);				// Release The Device Context
			in_window->hDC = 0;											// Zero The Device Context
		}
		DestroyWindow (in_window->hWnd);								// Destroy The Window
		in_window->hWnd = 0;											// Zero The Window Handle
	}
}

void showErrorMessageBox(const wchar_t* const in_message)
{
	MessageBox(HWND_DESKTOP, in_message, L"Error", MB_OK | MB_ICONEXCLAMATION);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE,	// hPrevInstance	// Previous Instance
					LPSTR,		//lpCmdLine,		// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done=false;								// Bool Variable To Exit Loop
	
	Window window(hInstance);
	window.windowClassName = L"101_window_class";

	if (!registerWindowClass(window))
	{
		showErrorMessageBox(L"RegisterClassEx failed!");
		return 0;
	}

	// for the 2nd time (and so on) we replace nCmdShow by SW_NORMAL (for example)
	if (!createWindow(&window, L"101 browser", 640, 480, 32, 32, nCmdShow))
	{
		showErrorMessageBox(L"Could not create window!");// Quit If Window Was Not Created
		return 0;
	}

	destroyWindow(&window);

	if (!unregisterWindowClass(window))
	{
		showErrorMessageBox(L"UnregisterClass failed!");
		return 0;
	}

	return 0;
}
