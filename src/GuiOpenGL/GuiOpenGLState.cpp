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

#include "GuiOpenGL/GuiOpenGLState.h"
#include "GuiOpenGL/GuiComponentsDefaults.h"
#include "GuiOpenGL/GuiComponentsCheckBox.h"
#include "GuiOpenGL/GuiComponentsTextBox.h"
#ifdef _WIN32
#include "gui/GuiComponentsCursor.h"
#endif

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <cassert>

float currentWidth, currentHeight;

void ReshapeGL(int width, int height)                               // Reshape The Window When It's Moved Or Resized
{
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);            // Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);                                    // Select The Projection Matrix
	glLoadIdentity();                                               // Reset The Projection Matrix

	glOrtho(0.0, width, 0.0, height, -1, 1);

	currentWidth  = static_cast<float>(width );
	currentHeight = static_cast<float>(height);
	
	glMatrixMode(GL_MODELVIEW);                                     // Select The Modelview Matrix
	glLoadIdentity ();                                              // Reset The Modelview Matrix
}

void initializeOpenGLGuiState()
{
	glClearColor(
		Gui::Components::Defaults::cWindowBackgroundColor.r, 
		Gui::Components::Defaults::cWindowBackgroundColor.g, 
		Gui::Components::Defaults::cWindowBackgroundColor.b,
		Gui::Components::Defaults::cWindowBackgroundColor.a);

	glClearDepth(1.0f);                                         // Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);                                     // The Type Of Depth Testing (Less Or Equal)
	glEnable(GL_DEPTH_TEST);                                    // Enable Depth Testing
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);                                    // Select Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Set Perspective Calculations To Most Accurate
}

void drawGui(
#ifdef _WIN32
			 const ArrayBlock<Gui::Mouse::RawMouse>* in_pRawMice
#endif
			 )
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        // Clear Screen And Depth Buffer
	glLoadIdentity();                                           // Reset The Modelview Matrix

	Gui::Components::drawTextBox(20.0f, 20.0f, 
		currentWidth-2*20.0f, 
		(float) Gui::Components::Defaults::cTextBoxHeight, 
		(float) Gui::Components::Defaults::cTextBoxBorderSize, 
		currentHeight);

	Gui::Components::drawCheckBox(20.0f, 80.0f, 
		(float) Gui::Components::Defaults::cCheckBoxWidth, 
		(float) Gui::Components::Defaults::cCheckBoxHeight, 
		(float) Gui::Components::Defaults::cCheckBoxBorderSize,
		currentHeight);

#ifdef _WIN32
	extern Gui::Cursor cursor;

	if (in_pRawMice != NULL)
	{
		for (size_t currentCursorIndex = 0; currentCursorIndex < in_pRawMice->count(); 
			currentCursorIndex++)
		{
			Gui::Components::drawCursor(
				static_cast<float>(in_pRawMice->data()[currentCursorIndex].x), 
				static_cast<float>(in_pRawMice->data()[currentCursorIndex].y), 
				currentHeight, &cursor);
		}
	}
#endif

	// Flush The GL Rendering Pipeline
	glFlush();
}

