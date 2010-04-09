/* Xlib.h is the default header that is included and has the core functionallity */
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <vector>
using namespace std;

struct OZO_Display
{
	Display* display;
	int screen;
	Window rootWindow;
};

struct OZO_Window
{
	GLXFBConfig* FBConfig;
	Window handle;
	GLXContext context;
};

void initDisplay(OZO_Display* pDisplay, const char* displayName)
{
	if ((pDisplay->display = XOpenDisplay(displayName)) == NULL)
	{
		fprintf(stderr, "Could not open display\n");
		exit(1);
	}

	pDisplay->screen = DefaultScreen(pDisplay->display);
	pDisplay->rootWindow = RootWindow(pDisplay->display, pDisplay->screen);
}

GLXFBConfig* chooseFBConfig(const OZO_Display* pDisplay)
{
	vector<int> attributes;

	attributes.push_back(GLX_RED_SIZE);
	attributes.push_back(1);

	attributes.push_back(GLX_GREEN_SIZE);
	attributes.push_back(1);

	attributes.push_back(GLX_BLUE_SIZE);
	attributes.push_back(1);

	attributes.push_back(GLX_DOUBLEBUFFER);
	attributes.push_back(True);

	attributes.push_back(None);

	GLXFBConfig * fbconfigArray;  /*  Array of FBConfigs  */
        int fbconfigArraySize;        /*  Number of FBConfigs in the array  */
	fbconfigArray = glXChooseFBConfig(pDisplay->display,
		pDisplay->screen,
		&attributes.at(0),
		&fbconfigArraySize );
		
	return fbconfigArray;
}

GLXContext createContext(const OZO_Display* pDisplay, const OZO_Window* pWindow)
{
	// this should probably not be used with OpenGL >= 3.x
	return glXCreateNewContext(pDisplay->display, *(pWindow->FBConfig), GLX_RGBA_TYPE, NULL, True);
}

void createWindow(const OZO_Display* pDisplay, OZO_Window* pWindow, const char* title)
{
	pWindow->FBConfig = chooseFBConfig(pDisplay);
	if (pWindow->FBConfig == NULL)
	{
		fprintf(stderr, "glXChooseFBConfig failed\n");
		exit(1);
	}
	XVisualInfo * visualInfo = glXGetVisualFromFBConfig(pDisplay->display, *(pWindow->FBConfig));

	XSetWindowAttributes winAttr;

	winAttr.background_pixmap = None;
	winAttr.background_pixel  = 0;
	winAttr.border_pixel      = 0;
	winAttr.colormap = XCreateColormap(
		pDisplay->display, pDisplay->rootWindow,
		visualInfo->visual, AllocNone
	);

	winAttr.event_mask = StructureNotifyMask | SubstructureNotifyMask | ExposureMask | 
		ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask |
		VisibilityChangeMask | EnterWindowMask | LeaveWindowMask |
		PointerMotionMask | ButtonMotionMask;

	/*
	 * this tells us which fields of winAttr we have set
	 */
	long mask = CWBackPixmap | CWBorderPixel | CWColormap | CWEventMask;

	int x = -1;
	int y = -1;
	int width = 640;
	int height = 480;

	pWindow->handle = XCreateWindow(
		pDisplay->display,
		/*
		 * In freeglut we have:
		 * pWindow->parent == NULL ? pDisplay->rootWindow : pWindow->parent->handle
		 * 
		 * In future we could perhaps use that?
		 */
		pDisplay->rootWindow, // parent window
		x, y, width, height,
		0,                    // border width
		visualInfo->depth,    // depth
		InputOutput,          // class
		visualInfo->visual,
		mask,                 // which values are defined in winAttr
		&winAttr);

	pWindow->context = createContext(pDisplay, pWindow);

	if (!pWindow->context)
	{
		fprintf(stderr, "Could not create context\n");
		exit(1);
	}

	bool positionUse = false;
	bool sizeUse = true;
	
	XSizeHints sizeHints;
	sizeHints.flags = 0;
    	if (positionUse)
        	sizeHints.flags |= USPosition;
    	if (sizeUse)
        	sizeHints.flags |= USSize;
	sizeHints.x = x;
	sizeHints.y = y;
	sizeHints.width = width;
	sizeHints.height = height;

	XWMHints wmHints;
	wmHints.flags = StateHint;
	wmHints.initial_state = NormalState;

	XTextProperty textProperty;

	XStringListToTextProperty((char **) &title, 1, &textProperty);
	XSetWMProperties(
		pDisplay->display,
		pWindow->handle,
		&textProperty,     // window name
		&textProperty,     // icon name
		0,                 // char **argv
		0,                 // int argc
		&sizeHints,
       		&wmHints,
		NULL               // XClassHint *class_hints
	);
	XFree(textProperty.value);

	Atom deleteWindow = XInternAtom(pDisplay->display, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(pDisplay->display, pWindow->handle, &deleteWindow, 1);

	glXMakeContextCurrent(
		pDisplay->display,
		pWindow->handle,    // GLX drawable to render into
		pWindow->handle,    // GLX drawable to read from
		pWindow->context
	);

	XMapWindow(pDisplay->display, pWindow->handle);

	XFree(visualInfo);
}

int main(int argc, char** argv)
{
	OZO_Display ozodisplay;
	void* resultPointer = memset(&ozodisplay, sizeof(ozodisplay), NULL);
	assert(resultPointer == &ozodisplay); // This should always be the case but we test it nevertheless

	initDisplay(&ozodisplay, NULL);

	OZO_Window window;
	createWindow(&ozodisplay, &window, "101 browser");

	return 0;
}

