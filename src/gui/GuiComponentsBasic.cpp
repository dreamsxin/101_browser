#include "gui/GuiComponentsBasic.h"
#include "GuiDataStructures/TriangleStripBorderIterator.h"
#include "GuiDataStructures/OuterBorderIterator.h"

#include <windows.h>
#include <gl/gl.h>
#include <cassert>

void createStraightBorder(Vertex2<float> prevVertex, 
						  Vertex2<float> currVertex,
						  Vertex2<float> nextVertex,
						  std::vector<Vertex2<float> >* pBorderTriangleStrip,
						  float borderWidth)
{
	Vector2<float> prevToCurrVect = currVertex - prevVertex;
	Vector2<float> currToNextVect = nextVertex - currVertex;

	normalize(&prevToCurrVect);
	normalize(&currToNextVect);

	Vector2<float> direction = prevToCurrVect+currToNextVect;
	normalize(&direction);

	Vector2<float> rightFromDirection = Vector2<float>(
		direction.y, 
		-direction.x);

	float scaleFactor = 
		-borderWidth/(currToNextVect.x*rightFromDirection.x+
		currToNextVect.y*rightFromDirection.y);

	pBorderTriangleStrip->push_back(currVertex);
	pBorderTriangleStrip->push_back(currVertex+rightFromDirection*scaleFactor);
}

void createBorderTriangleStrip(const std::vector<Vertex2<float> >* triangleStrip,
							   std::vector<Vertex2<float> >* pBorderTriangleStrip,
							   float borderWidth)
{
	TriangleStripBorderIterator<Vertex2<float> >::ConstIteratorState state = 
		triangleStripBorderConstIteratorState_create(triangleStrip);
	DoubleIterator<const Vertex2<float>, TriangleStripBorderIterator<Vertex2<float> >::ConstIteratorState> it =
		triangleStripBorderConstIterator_create<Vertex2<float> >();

	if (triangleStrip->size()>=2)
	{
		IterateResult itRes;

		itRes = (*it.mpfIteratePrev)(&state);
		assert(itRes == IterateResultOverBoundary);

		Vertex2<float> prevVertex = *(*it.mpfGet)(&state);

		itRes = it.mpfIterateNext(&state);
		assert(itRes == IterateResultOverBoundary);

		Vertex2<float> currVertex = *(*it.mpfGet)(&state);

		itRes = it.mpfIterateNext(&state);
		assert(itRes == IterateResultOK);

		Vertex2<float> nextVertex = *(*it.mpfGet)(&state);

		bool breakAfterSecondNextIteration = false;
		bool breakAfterNextIteration = false;

		while (true)
		{
			createStraightBorder(prevVertex, currVertex, nextVertex, 
				pBorderTriangleStrip, borderWidth);

			// If this was set in the previous iteration we shall break
			if (breakAfterNextIteration)
				break;

			// else we iterate
			itRes = (*it.mpfIterateNext)(&state);

			if (breakAfterSecondNextIteration)
			{
				breakAfterNextIteration = true;
			}

			// if this is true we do exactly one additional iteration
			if (itRes == IterateResultOverBoundary)
				breakAfterSecondNextIteration = true;

			prevVertex = currVertex;
			currVertex = nextVertex;
			nextVertex = *(*it.mpfGet)(&state);
		}
	}
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

void drawVertexArray(const std::vector<Vertex2<float> >* vertices, Color4<float> colors[4])
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);	

	glVertexPointer(2, GL_FLOAT, 0, &vertices->at(0));
	glColorPointer(4, GL_FLOAT, 0, colors);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawVertexArray(const std::vector<Vertex2<float> >* vertices, Color4<float> color)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4fv(&color.r);

	glVertexPointer(2, GL_FLOAT, 0, &vertices->at(0));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices->size());

	glDisableClientState(GL_VERTEX_ARRAY);
}
