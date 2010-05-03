#include <X11/Xlib.h>
#include <xcb/xcb.h>

/*
 * the extern "C" is a workaround for http://bugs.freedesktop.org/show_bug.cgi?id=22252
 *
 * I will remove it soon
 */
extern "C"
{
#include <X11/Xlib-xcb.h> /* for XGetXCBConnection */
}

#include <GL/gl.h>
#include <GL/glx.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <vector>

#include "GuiOpenGL/GuiOpenGLState.h"

using namespace std;

struct OZO_Display
{
	Display* display;
	xcb_connection_t *connection;

	int defaultScreen;
	xcb_screen_t *screen;

	//Window rootWindow;
};

struct OZO_Window
{
	GLXFBConfig* FBConfig;
	//Window handle;
	xcb_window_t window;
	GLXContext context;
	GLXDrawable drawable;
};

struct OZO_Atoms
{
	Atom deleteWindow;
};

void cleanup(OZO_Display* pDisplay, OZO_Window* pWindow);

void initDisplay(OZO_Display* pDisplay, const char* displayName)
{
	if ((pDisplay->display = XOpenDisplay(displayName)) == NULL)
	{
		fprintf(stderr, "Can't open display\n");
		cleanup(pDisplay, NULL);
		exit(1);
	}

	pDisplay->defaultScreen = DefaultScreen(pDisplay->display);
	//pDisplay->rootWindow = RootWindow(pDisplay->display, pDisplay->defaultScreen);

	pDisplay->connection = XGetXCBConnection(pDisplay->display);

	if (!pDisplay->connection)
	{
		fprintf(stderr, "Can't get xcb connection from display\n");
		cleanup(pDisplay, NULL);
		exit(1);
	}

	//XSetEventQueueOwner(pDisplay->display, XCBOwnsEventQueue);

	xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(pDisplay->connection));
        for(int screen_num = pDisplay->defaultScreen; screen_iter.rem && screen_num > 0; --screen_num, xcb_screen_next(&screen_iter));
        pDisplay->screen = screen_iter.data;
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

	attributes.push_back(GLX_DEPTH_SIZE);
	attributes.push_back(1);

	attributes.push_back(GLX_DOUBLEBUFFER);
	attributes.push_back(True);

	attributes.push_back(None);

	GLXFBConfig * fbconfigArray;  /*  Array of FBConfigs  */
        int fbconfigArraySize;        /*  Number of FBConfigs in the array  */
	fbconfigArray = glXChooseFBConfig(pDisplay->display,
		pDisplay->defaultScreen,
		&attributes.at(0),
		&fbconfigArraySize );
		
	return fbconfigArray;
}

void createWindow(OZO_Display* pDisplay, OZO_Window* pWindow, const char* title, OZO_Atoms* pAtoms, 
unsigned int width, unsigned int height)
{
	pWindow->FBConfig = chooseFBConfig(pDisplay);
	if (pWindow->FBConfig == NULL)
	{
		fprintf(stderr, "glXChooseFBConfig failed\n");
		cleanup(pDisplay, pWindow);
		exit(1);
	}
	XVisualInfo * visualInfo = glXGetVisualFromFBConfig(pDisplay->display, *(pWindow->FBConfig));

	// this should probably not be used with OpenGL >= 3.x
	pWindow->context = glXCreateNewContext(pDisplay->display, *(pWindow->FBConfig), GLX_RGBA_TYPE, NULL, True);

	if (!pWindow->context)
	{
		fprintf(stderr, "Could not create context\n");
		cleanup(pDisplay, pWindow);
		exit(1);
	}

	//XSetWindowAttributes winAttr;
	//winAttr.background_pixmap = None;
	uint32_t back_pixmap = XCB_BACK_PIXMAP_NONE;
	//winAttr.background_pixel  = 0;
	uint32_t background_pixel = 0;
	//winAttr.border_pixel      = 0;
	uint32_t border_pixel = 0;

	//winAttr.colormap = XCreateColormap(
	//	pDisplay->display, pDisplay->rootWindow,
	//	visualInfo->visual, AllocNone
	//);
	xcb_colormap_t colormap = xcb_generate_id(pDisplay->connection);
	xcb_create_colormap(
		pDisplay->connection,
		XCB_COLORMAP_ALLOC_NONE,
		colormap,
		pDisplay->screen->root,
		visualInfo->visualid);

	//winAttr.event_mask = StructureNotifyMask | SubstructureNotifyMask | ExposureMask | 
	//	ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask |
	//	VisibilityChangeMask | EnterWindowMask | LeaveWindowMask |
	//	PointerMotionMask | ButtonMotionMask;
	uint32_t event_mask = XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_EXPOSURE | 
		XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
		XCB_EVENT_MASK_VISIBILITY_CHANGE | XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW |
		XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_BUTTON_MOTION;

	/*
	 * this tells us which fields of winAttr we have set
	 */
	//long mask = CWBackPixmap | CWBorderPixel | CWEventMask | CWColormap;
	uint32_t valuemask = XCB_CW_BACK_PIXMAP | XCB_CW_BORDER_PIXEL | XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;
	uint32_t valuelist[] = { 0, 0, event_mask, colormap };

	//int x = -1;
	//int y = -1;
	int16_t x = -1;
	int16_t y = -1;
		

	//pWindow->handle = XCreateWindow(
	//	pDisplay->display,
		/*
		 * In freeglut we have:
		 * pWindow->parent == NULL ? pDisplay->rootWindow : pWindow->parent->handle
		 * 
		 * In future we could perhaps use that?
		 */
	//	pDisplay->rootWindow, // parent window
	//	x, y, (unsigned int) width, (unsigned int) height,
	//	0,                    // border width
	//	visualInfo->depth,    // depth
	//	InputOutput,          // class
	//	visualInfo->visual,
	//	mask,                 // which values are defined in winAttr
	//	&winAttr);
	pWindow->window = xcb_generate_id(pDisplay->connection);
	xcb_create_window(
		pDisplay->connection, 
		visualInfo->depth,             // depth
		pWindow->window,
		pDisplay->screen->root,
		x, y, (uint16_t) width, (uint16_t) height,
		0,                             // border width
		XCB_WINDOW_CLASS_INPUT_OUTPUT, // class
		visualInfo->visualid,
		valuemask,
		valuelist
	);

	XFree(visualInfo);

	bool positionUse = false;
	bool sizeUse = true;
	
	/*XSizeHints sizeHints;
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
		NULL,              // char **argv
		0,                 // int argc
		&sizeHints,
       		&wmHints,
		NULL               // XClassHint *class_hints
	);
	XFree(textProperty.value);

	pAtoms->deleteWindow = XInternAtom(pDisplay->display, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(pDisplay->display, pWindow->handle, &pAtoms->deleteWindow, 1);*/

	//XMapWindow(pDisplay->display, pWindow->handle);
	xcb_map_window(pDisplay->connection, pWindow->window);

	GLXWindow glxwindow = glXCreateWindow(
                pDisplay->display,
                *(pWindow->FBConfig),
                pWindow->window,
                NULL
                );

	if (!glxwindow)
	{
		fprintf(stderr, "glXCreateWindow failed\n");
		cleanup(pDisplay, pWindow);
		exit(1);
	}

	pWindow->drawable = glxwindow;

	//glXMakeContextCurrent(
	//	pDisplay->display,
	//	pWindow->handle,    // GLX drawable to render into
	//	pWindow->handle,    // GLX drawable to read from
	//	pWindow->context
	//);
	glXMakeContextCurrent(
		pDisplay->display,
		glxwindow,    // GLX drawable to render into
		glxwindow,    // GLX drawable to read from
		pWindow->context
	);
}

void cleanup(OZO_Display* pDisplay, OZO_Window* pWindow)
{
	if (pWindow)
	{
		if (pWindow->drawable)
		{
			glXDestroyWindow(pDisplay->display, pWindow->drawable);
		}

		if (pWindow->window)
		{
			xcb_destroy_window(pDisplay->connection, pWindow->window);
			memset(&pWindow->window, sizeof(pWindow->window), 0);
		}

		if (pWindow->context)
		{
			glXDestroyContext(pDisplay->display, pWindow->context);
			memset(&pWindow->context, sizeof(pWindow->context), 0);
		}

		if (pWindow->FBConfig)
		{
			XFree(pWindow->FBConfig);
			memset(&pWindow->FBConfig, sizeof(pWindow->FBConfig), 0);
		}
	}

	assert(pDisplay != NULL);

	if (pDisplay->display != NULL)
	{
		XSetCloseDownMode(pDisplay->display, DestroyAll);
		XCloseDisplay(pDisplay->display);
		memset(&pDisplay->display, sizeof(pDisplay->display), 0);
	}
}

//void printEvent(int type)
void printEvent(uint8_t response_type)
{
	switch(response_type)
	{
	//case KeyPress:
	case XCB_KEY_PRESS:
		printf("XCB_KEY_PRESS\n");
		break;
	//case ClientMessage:
	case XCB_CLIENT_MESSAGE:
		printf("XCB_CLIENT_MESSAGE\n");
		break;
	default:
		printf("Unknown event with value %u\n", response_type);
		break;
	}
}

void draw(const OZO_Display* pDisplay, const OZO_Window* pWindow)
{
	UpdateGuiState();
	drawGui();
	glXSwapBuffers(pDisplay->display, pWindow->drawable);
}

void mainLoop(OZO_Display* pDisplay, const OZO_Window* pWindow, OZO_Atoms* pAtoms, int *currentWidth, int *currentHeight)
{
	bool done = false;

	while (!done)
	{
		xcb_generic_event_t *ev;

		//while (XPending(pDisplay->display))
		while (ev = xcb_poll_for_event(pDisplay->connection))
		{
			//XEvent event;
			//XNextEvent(pDisplay->display, &event);

			//printEvent(event.type);
			printEvent(ev->response_type);

			//switch (event.type)
			switch (ev->response_type)
			{
			//case KeyPress:
			case XCB_KEY_PRESS:
				//if (XLookupKeysym(&event.xkey, 0) == XK_Escape)
				break;
			//case ClientMessage:
			case XCB_CLIENT_MESSAGE:
			/*	if((Atom) event.xclient.data.l[0] == pAtoms->deleteWindow)
				{
					done = true;
				}*/
				break;
			//case CreateNotify:
			case XCB_CREATE_NOTIFY:
        		//case ConfigureNotify:
			case XCB_CONFIGURE_NOTIFY:
			{
				uint16_t newWidth, newHeight;
				if (ev->response_type == XCB_CREATE_NOTIFY) {
					//newWidth = event.xcreatewindow.width;
					//newHeight = event.xcreatewindow.height;
					newWidth = ((xcb_create_notify_event_t *) ev)->width;
					newHeight = ((xcb_create_notify_event_t *) ev)->height;
                		} else {
					assert(ev->response_type == XCB_CONFIGURE_NOTIFY);
					//newWidth = event.xconfigure.width;
					//newHeight = event.xconfigure.height;
					newWidth = ((xcb_configure_notify_event_t *) ev)->width;
					newHeight = ((xcb_configure_notify_event_t *) ev)->height;
				}

				if ((*currentWidth != newWidth) || (*currentHeight != newHeight))
				{
					*currentWidth = newWidth;
					*currentHeight = newHeight;
					ReshapeGL(newWidth, newHeight);
				}
			}
			default:
				break;
			}
		}

		draw(pDisplay, pWindow);
	}
}

int main(int argc, char** argv)
{
	OZO_Display ozodisplay;
	memset(&ozodisplay, sizeof(ozodisplay), NULL);

	initDisplay(&ozodisplay, NULL);

	OZO_Window ozowindow;
	memset(&ozowindow, sizeof(ozowindow), NULL);

	OZO_Atoms ozoatoms;
	memset(&ozoatoms, sizeof(ozoatoms), NULL);

	int width = 640;
	int height = 480;

	createWindow(&ozodisplay, &ozowindow, "101 browser", &ozoatoms, width, height);

	ReshapeGL(width, height);
	initializeOpenGLGuiState();

	mainLoop(&ozodisplay, &ozowindow, &ozoatoms, &width, &height);

	cleanup(&ozodisplay, &ozowindow);

	return 0;
}

