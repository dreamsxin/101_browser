#include "gui/GuiComponentsBasic.h"

#include <windows.h>
#include <gl/gl.h>
#include <vector>

void createBorderVertices(const std::vector<Vertex2f>* boxVertices,
						  std::vector<Vertex2f>* borderVertices,
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
		Vertex2f(boxVertices->at(0).x, boxVertices->at(0).y));
	// 1
	borderVertices->push_back(
		Vertex2f(boxVertices->at(0).x-borderWidth, boxVertices->at(0).y-borderWidth));
	// 2
	borderVertices->push_back(
		Vertex2f(boxVertices->at(1).x, boxVertices->at(1).y));
	// 3
	borderVertices->push_back(
		Vertex2f(boxVertices->at(1).x+borderWidth, boxVertices->at(1).y-borderWidth));
	// 4
	borderVertices->push_back(
		Vertex2f(boxVertices->at(3).x, boxVertices->at(3).y));
	// 5
	borderVertices->push_back(
		Vertex2f(boxVertices->at(3).x+borderWidth, boxVertices->at(3).y+borderWidth));
	// 6
	borderVertices->push_back(
		Vertex2f(boxVertices->at(2).x, boxVertices->at(2).y));
	// 7
	borderVertices->push_back(
		Vertex2f(boxVertices->at(2).x-borderWidth, boxVertices->at(2).y+borderWidth));
	// 8
	borderVertices->push_back(borderVertices->at(0));
	// 9
	borderVertices->push_back(borderVertices->at(1));
}
/*!
 * vertices output:
 * [0]: bottom left
 * [1]: bottom right
 * [2]: top left
 * [3]: top right
 */
void createBoxVertices(std::vector<Vertex2f>* boxVertices,
					float left, float top, float width, float height,
					float currentHeight)
{
	/*
	 * 2-3
	 * |\|
	 * 0-1
	 */
	boxVertices->push_back(Vertex2f(left, currentHeight-top-height)); // bottom left
	boxVertices->push_back(Vertex2f(left+width, currentHeight-top-height)); // bottom right
	boxVertices->push_back(Vertex2f(left, currentHeight-top)); // top left
	boxVertices->push_back(Vertex2f(left+width, currentHeight-top)); // top right
}

void drawVertexArray(const std::vector<Vertex2f>* vertices, Color4f colors[4])
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);	
	
	glVertexPointer(2, GL_FLOAT, 0, &vertices->at(0));
	glColorPointer(4, GL_FLOAT, 0, colors);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawVertexArray(const std::vector<Vertex2f>* vertices, Color4f color)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4fv(&color.r);
	
	glVertexPointer(2, GL_FLOAT, 0, &vertices->at(0));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices->size());
	
	glDisableClientState(GL_VERTEX_ARRAY);
}
