#include "gui/GuiOpenGLState.h"
#include "gui/GuiComponentsDefaults.h"
#include "gui/Vertex.h"
#include <windows.h>
#include <gl/gl.h>
#include <vector>

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

/*!
 * vertices output:
 * [0]: bottom left
 * [1]: bottom right
 * [2]: top left
 * [3]: top right
 */
void createBoxVertices(std::vector<Vertex>* boxVertices,
					float left, float top, float width, float height)
{
	/*
	 * 2-3
	 * |\|
	 * 0-1
	 */
	boxVertices->push_back(Vertex(left, currentHeight-top-height)); // bottom left
	boxVertices->push_back(Vertex(left+width, currentHeight-top-height)); // bottom right
	boxVertices->push_back(Vertex(left, currentHeight-top)); // top left
	boxVertices->push_back(Vertex(left+width, currentHeight-top)); // top right
}

void createBorderVertices(const std::vector<Vertex>* boxVertices,
						  std::vector<Vertex>* borderVertices,
						  float borderWidth)
{
	/*
	 * Layout of vertices:
	 * 2-3
	 * |\|
	 * 0-1
	 * 
	 * Layout of borderVertices:
	 * 
	 * 7--------5
	 * |\      /|
	 * | 6----4 |
	 * | |    | |
	 * |0,8---2 |
	 * |/      \|
	 * 1,9------3
	 */
	// 0
	borderVertices->push_back(
		Vertex(boxVertices->at(0).x, boxVertices->at(0).y));
	// 1
	borderVertices->push_back(
		Vertex(boxVertices->at(0).x-borderWidth, boxVertices->at(0).y-borderWidth));
	// 2
	borderVertices->push_back(
		Vertex(boxVertices->at(1).x, boxVertices->at(1).y));
	// 3
	borderVertices->push_back(
		Vertex(boxVertices->at(1).x+borderWidth, boxVertices->at(1).y-borderWidth));
	// 4
	borderVertices->push_back(
		Vertex(boxVertices->at(3).x, boxVertices->at(3).y));
	// 5
	borderVertices->push_back(
		Vertex(boxVertices->at(3).x+borderWidth, boxVertices->at(3).y+borderWidth));
	// 6
	borderVertices->push_back(
		Vertex(boxVertices->at(2).x, boxVertices->at(2).y));
	// 7
	borderVertices->push_back(
		Vertex(boxVertices->at(2).x-borderWidth, boxVertices->at(2).y+borderWidth));
	// 8
	borderVertices->push_back(borderVertices->at(0));
	// 9
	borderVertices->push_back(borderVertices->at(1));
}

void drawVertexArray(const std::vector<Vertex>* vertices, Color4f colors[4])
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);	
	
	glVertexPointer(3, GL_FLOAT, 0, &vertices->at(0));
	glColorPointer(4, GL_FLOAT, 0, colors);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawVertexArray(const std::vector<Vertex>* vertices, Color4f color)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4fv(&color.r);
	
	glVertexPointer(3, GL_FLOAT, 0, &vertices->at(0));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices->size());
	
	glDisableClientState(GL_VERTEX_ARRAY);
}


void drawCheckbox(float left, float top, float width, float height, float borderSize)
{
	Color4f colors[4]={
		cCheckBoxBottomLeftBackgroundColor,
		cCheckBoxBottomRightBackgroundColor,
		cCheckBoxTopLeftBackgroundColor, 
		cCheckBoxTopRightBackgroundColor
		};
	std::vector<Vertex> boxVertices;
	std::vector<Vertex> borderVertices;
	createBoxVertices(&boxVertices, left, top, width, height);
	createBorderVertices(&boxVertices, &borderVertices, borderSize);
	drawVertexArray(&boxVertices, colors);
	drawVertexArray(&borderVertices, cCheckBoxBorderColor);
}

void drawGui()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity();											// Reset The Modelview Matrix
	
	glColor3f(1.f, 0.f, 0.f);

	drawCheckbox(20, 20, cCheckBoxWidth, cCheckBoxHeight, cCheckBoxBorderSize);

	glFlush ();													// Flush The GL Rendering Pipeline
}
