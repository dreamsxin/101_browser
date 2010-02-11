#include "gui/GuiOpenGLState.h"
#include "gui/GuiComponentsDefaults.h"
#include "gui/GuiComponentsCheckBox.h"
#include "gui/GuiComponentsTextBox.h"

#include <windows.h>
#include <gl/gl.h>

float currentWidth, currentHeight;

void ReshapeGL (int width, int height)									// Reshape The Window When It's Moved Or Resized
{
	glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				// Reset The Current Viewport
	glMatrixMode (GL_PROJECTION);										// Select The Projection Matrix
	glLoadIdentity ();													// Reset The Projection Matrix
	
	glOrtho(0.0, width, 0.0, height, -1, 1);

	currentWidth = width;
	currentHeight = height;
	
	glMatrixMode (GL_MODELVIEW);										// Select The Modelview Matrix
	glLoadIdentity ();													// Reset The Modelview Matrix
}

void initializeOpenGLGuiState()
{
	glClearColor(
		cWindowBackgroundColor.r, 
		cWindowBackgroundColor.g, 
		cWindowBackgroundColor.b,
		cWindowBackgroundColor.a);

	glClearDepth (1.0f);										// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);									// Enable Depth Testing
	glEnable(GL_CULL_FACE);
	glShadeModel (GL_SMOOTH);									// Select Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate
}

void UpdateGuiState(unsigned int deltaMilliSeconds)
{
	//angle += (float)(deltaMilliSeconds) / 5.0f;
}

void drawGui()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity();											// Reset The Modelview Matrix

	drawTextBox(20, 20, 
		currentWidth-2*20, cTextBoxHeight, 
		cTextBoxBorderSize, 
		currentHeight);

	drawCheckBox(20, 80, 
		cCheckBoxWidth, cCheckBoxHeight, 
		cCheckBoxBorderSize,
		currentHeight);

	glFlush();													// Flush The GL Rendering Pipeline
}
