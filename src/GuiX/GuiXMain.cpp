/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <X11/Xlib.h>
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
	int screen;
	Window rootWindow;
};

struct OZO_Window
{
	GLXFBConfig* FBConfig;
	Window handle;
	GLXContext context;
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
		fprintf(stderr, "Could not open display\n");
		cleanup(pDisplay, NULL);
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

	attributes.push_back(GLX_DEPTH_SIZE);
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

	pWindow->handle = XCreateWindow(
		pDisplay->display,
		/*
		 * In freeglut we have:
		 * pWindow->parent == NULL ? pDisplay->rootWindow : pWindow->parent->handle
		 * 
		 * In future we could perhaps use that?
		 */
		pDisplay->rootWindow, // parent window
		x, y, (unsigned int) width, (unsigned int) height,
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
		cleanup(pDisplay, pWindow);
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
		NULL,              // char **argv
		0,                 // int argc
		&sizeHints,
       		&wmHints,
		NULL               // XClassHint *class_hints
	);
	XFree(textProperty.value);

	pAtoms->deleteWindow = XInternAtom(pDisplay->display, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(pDisplay->display, pWindow->handle, &pAtoms->deleteWindow, 1);

	glXMakeContextCurrent(
		pDisplay->display,
		pWindow->handle,    // GLX drawable to render into
		pWindow->handle,    // GLX drawable to read from
		pWindow->context
	);

	XMapWindow(pDisplay->display, pWindow->handle);

	XFree(visualInfo);
}

void cleanup(OZO_Display* pDisplay, OZO_Window* pWindow)
{
	if (pWindow)
	{
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

		if (pWindow->handle)
		{
			XDestroyWindow(pDisplay->display, pWindow->handle);
			memset(&pWindow->handle, sizeof(pWindow->handle), 0);
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

void printEvent(int type)
{
	switch(type)
	{
	case KeyPress:
		printf("KeyPress\n");
		break;
	case ClientMessage:
		printf("ClientMessage\n");
		break;
	default:
		printf("Unknown event with value %u\n", type);
		break;
	}
}

void draw(const OZO_Display* pDisplay, const OZO_Window* pWindow)
{
	UpdateGuiState();
	drawGui();
	glXSwapBuffers(pDisplay->display, pWindow->handle);
}

void mainLoop(OZO_Display* pDisplay, const OZO_Window* pWindow, OZO_Atoms* pAtoms, int *currentWidth, int *currentHeight)
{
	bool done = false;

	while (!done)
	{	
		while (XPending(pDisplay->display))
		{
			XEvent event;
			XNextEvent(pDisplay->display, &event);

			printEvent(event.type);

			switch(event.type)
			{
			case KeyPress:
				if (XLookupKeysym(&event.xkey, 0) == XK_Escape)
				{
					done = true;
                    		}
				break;
			case ClientMessage:
				if((Atom) event.xclient.data.l[0] == pAtoms->deleteWindow)
				{
					done = true;
				}
				break;
			case CreateNotify:
        		case ConfigureNotify:
			{
				int newWidth, newHeight;
				if (event.type == CreateNotify) {
					newWidth = event.xcreatewindow.width;
					newHeight = event.xcreatewindow.height;
                		} else {
					assert(event.type == ConfigureNotify);
					newWidth = event.xconfigure.width;
					newHeight = event.xconfigure.height;
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
	memset(&ozowindow, 0, sizeof(ozowindow));

	OZO_Atoms ozoatoms;
	memset(&ozoatoms, 0, sizeof(ozoatoms));

	int width = 640;
	int height = 480;

	createWindow(&ozodisplay, &ozowindow, "101 browser", &ozoatoms, width, height);

	ReshapeGL(width, height);
	initializeOpenGLGuiState();

	mainLoop(&ozodisplay, &ozowindow, &ozoatoms, &width, &height);

	cleanup(&ozodisplay, &ozowindow);

	return 0;
}

