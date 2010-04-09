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

void createWindow(const OZO_Display* pDisplay, OZO_Window* pWindow)
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
}

int main(int argc, char** argv)
{
	OZO_Display ozodisplay;
	void* resultPointer = memset(&ozodisplay, sizeof(ozodisplay), NULL);
	assert(resultPointer == &ozodisplay); // This should always be the case but we test it nevertheless

	initDisplay(&ozodisplay, NULL);

	OZO_Window window;
	createWindow(&ozodisplay, &window);

	return 0;
}
