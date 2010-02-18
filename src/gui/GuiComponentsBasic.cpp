#include "gui/GuiComponentsBasic.h"
#include "GuiDataStructures/TriangleStripBorderIterator.h"

#include <windows.h>
#include <gl/gl.h>
#include <vector>

void createBorderTriangleStrip(const std::vector<Vertex2<float> >* triangleStrip,
							   std::vector<Vertex2<float> >* borderTriangleStrip,
							   float borderWidth)
{
	TriangleStripBorderConstIteratorState<Vertex2<float> > state = 
		triangleStripBorderConstIteratorState_create(triangleStrip);
	Iterator<const Vertex2<float>, TriangleStripBorderConstIteratorState<Vertex2<float> > > it =
		triangleStripBorderConstIterator_create<Vertex2<float> >();

	// This is a hack. Todo: write a sensitive iterator interface.
	if (triangleStrip->size()>=3)
	{
		Vertex2<float> first = *(*it.mpfGet)(&state);
		Vertex2<float> last = state.mVector->at(state.mVector->size()-1);

		Vertex2<float> vertexBundle[3];
		vertexBundle[1] = last;

		assert(!(*it.mpfEnd)(&state));
		vertexBundle[2] = *(*it.mpfGet)(&state);

		while (!(*it.mpfEnd)(&state))
		{
			vertexBundle[0] = vertexBundle[1];
			vertexBundle[1] = vertexBundle[2];

			(*it.mpfNext)(&state);

			if (!(*it.mpfEnd)(&state))
			{
				vertexBundle[2] = *(*it.mpfGet)(&state);
			}
			else
			{
				vertexBundle[2] = first;
			}

			// create vertices for strip
			// TODO
		}
	}
}

void createBorderVertices(const std::vector<Vertex2<float> >* boxVertices,
						  std::vector<Vertex2<float> >* borderVertices,
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
		Vertex2<float>(boxVertices->at(0).x, boxVertices->at(0).y));
	// 1
	borderVertices->push_back(
		Vertex2<float>(boxVertices->at(0).x-borderWidth, boxVertices->at(0).y-borderWidth));
	// 2
	borderVertices->push_back(
		Vertex2<float>(boxVertices->at(1).x, boxVertices->at(1).y));
	// 3
	borderVertices->push_back(
		Vertex2<float>(boxVertices->at(1).x+borderWidth, boxVertices->at(1).y-borderWidth));
	// 4
	borderVertices->push_back(
		Vertex2<float>(boxVertices->at(3).x, boxVertices->at(3).y));
	// 5
	borderVertices->push_back(
		Vertex2<float>(boxVertices->at(3).x+borderWidth, boxVertices->at(3).y+borderWidth));
	// 6
	borderVertices->push_back(
		Vertex2<float>(boxVertices->at(2).x, boxVertices->at(2).y));
	// 7
	borderVertices->push_back(
		Vertex2<float>(boxVertices->at(2).x-borderWidth, boxVertices->at(2).y+borderWidth));
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
void createBoxVertices(std::vector<Vertex2<float> >* boxVertices,
					float left, float top, float width, float height,
					float currentHeight)
{
	/*
	 * 2-3
	 * |\|
	 * 0-1
	 */
	boxVertices->push_back(Vertex2<float>(left, currentHeight-top-height)); // bottom left
	boxVertices->push_back(Vertex2<float>(left+width, currentHeight-top-height)); // bottom right
	boxVertices->push_back(Vertex2<float>(left, currentHeight-top)); // top left
	boxVertices->push_back(Vertex2<float>(left+width, currentHeight-top)); // top right
}

void drawVertexArray(const std::vector<Vertex2<float> >* vertices, Color4f colors[4])
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);	
	
	glVertexPointer(2, GL_FLOAT, 0, &vertices->at(0));
	glColorPointer(4, GL_FLOAT, 0, colors);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawVertexArray(const std::vector<Vertex2<float> >* vertices, Color4f color)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4fv(&color.r);
	
	glVertexPointer(2, GL_FLOAT, 0, &vertices->at(0));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices->size());
	
	glDisableClientState(GL_VERTEX_ARRAY);
}
