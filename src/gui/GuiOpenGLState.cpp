#include "gui/GuiOpenGLState.h"
#include <windows.h>
#include <gl/gl.h>

float angle;

void initializeOpenGLGuiState()
{
	angle = 0.0f;

	glClearColor (0.0f, 0.0f, 0.0f, 0.5f);						// Black Background
	glClearDepth (1.0f);										// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);									// Enable Depth Testing
	glShadeModel (GL_SMOOTH);									// Select Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate
}

void UpdateGuiState(unsigned int deltaMilliSeconds)
{
	angle += (float)(deltaMilliSeconds) / 5.0f;
}

void drawGui()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity ();											// Reset The Modelview Matrix
	glTranslatef (0.0f, 0.0f, -6.0f);							// Translate 6 Units Into The Screen
	glRotatef (angle, 0.0f, 1.0f, 0.0f);						// Rotate On The Y-Axis By angle

	for (int rot1=0; rot1<2; rot1++)							// 2 Passes
	{
		glRotatef(90.0f,0.0f,1.0f,0.0f);						// Rotate 90 Degrees On The Y-Axis
		glRotatef(180.0f,1.0f,0.0f,0.0f);						// Rotate 180 Degress On The X-Axis
		for (int rot2=0; rot2<2; rot2++)						// 2 Passes
		{
			glRotatef(180.0f,0.0f,1.0f,0.0f);					// Rotate 180 Degrees On The Y-Axis
			glBegin (GL_TRIANGLES);								// Begin Drawing Triangles
				glColor3f (1.f, 0.f, 0.f);	glVertex3f( 0.0f, 1.0f, 0.0f);
				glColor3f (0.f, 1.f, 0.f);	glVertex3f(-1.0f,-1.0f, 1.0f);
				glColor3f (0.f, 0.f, 1.f);	glVertex3f( 1.0f,-1.0f, 1.0f);
			glEnd ();											// Done Drawing Triangles
		}
	}

	glFlush ();													// Flush The GL Rendering Pipeline
}
