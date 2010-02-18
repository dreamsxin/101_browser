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

	currentWidth = (float) width;
	currentHeight = (float) height;
	
	glMatrixMode (GL_MODELVIEW);										// Select The Modelview Matrix
	glLoadIdentity ();													// Reset The Modelview Matrix
}

void initializeOpenGLGuiState()
{
	glClearColor(
		Gui::Components::Defaults::cWindowBackgroundColor.r, 
		Gui::Components::Defaults::cWindowBackgroundColor.g, 
		Gui::Components::Defaults::cWindowBackgroundColor.b,
		Gui::Components::Defaults::cWindowBackgroundColor.a);

	glClearDepth (1.0f);										// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);									// Enable Depth Testing
	glEnable(GL_CULL_FACE);
	glShadeModel (GL_SMOOTH);									// Select Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate
}

void UpdateGuiState()
{
}

void drawGui()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity();											// Reset The Modelview Matrix

	Gui::Components::drawTextBox(20.0f, 20.0f, 
		currentWidth-2*20.0f, 
		Gui::Components::Defaults::cTextBoxHeight, 
		Gui::Components::Defaults::cTextBoxBorderSize, 
		currentHeight);

	Gui::Components::drawCheckBox(20.0f, 80.0f, 
		Gui::Components::Defaults::cCheckBoxWidth, 
		Gui::Components::Defaults::cCheckBoxHeight, 
		Gui::Components::Defaults::cCheckBoxBorderSize,
		currentHeight);

	glFlush();													// Flush The GL Rendering Pipeline
}
