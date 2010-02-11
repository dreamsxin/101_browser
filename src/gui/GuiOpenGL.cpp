#include "gui/GuiOpenGL.h"
#include <windows.h>
#include <gl/gl.h>
#define _USE_MATH_DEFINES
#include <cmath>			// for M_PI and tanf

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
