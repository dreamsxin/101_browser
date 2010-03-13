#ifndef _GuiComponentsBasic_h
#define _GuiComponentsBasic_h

#include <vector>

#include "gui/Vertex.h"
#include "gui/Color.h"
#include "GuiDataStructures/TriangleStripBorderIterator.h"
#include "GuiDataStructures/OuterBorderIterator.h"

void createStraightBorder(Vertex2<float> prevVertex, 
						  Vertex2<float> currVertex,
						  Vertex2<float> nextVertex,
						  std::vector<Vertex2<float> >* pBorderTriangleStrip,
						  float borderWidth, 
						  size_t);

void createRoundBorder(Vertex2<float> prevVertex, 
					   Vertex2<float> currVertex,
					   Vertex2<float> nextVertex,
					   std::vector<Vertex2<float> >* pBorderTriangleStrip,
					   float borderWidth, 
					   size_t curveSegmentsCount);

/*!
* triangleStrip: a triangle strip with non-repeating vertices
* borderTriangleStrip: where the border gets stored
* borderWidth: the desired width of the border
*/
inline void createBorderTriangleStrip(
	const std::vector<Vertex2<float> >* triangleStrip,
	std::vector<Vertex2<float> >* pBorderTriangleStrip,
	void (*borderCreatingFunction)(Vertex2<float>, 
	Vertex2<float>, Vertex2<float>, 
	std::vector<Vertex2<float> >*, 
	float, size_t),
	float borderWidth,
	size_t curveSegmentsCount)
{
	TriangleStripBorderIterator<Vertex2<float> >::ConstIteratorState state = 
		triangleStripBorderConstIteratorState_create(triangleStrip);
	DoubleIterator<const Vertex2<float>, TriangleStripBorderIterator<Vertex2<float> >::ConstIteratorState> it =
		triangleStripBorderConstIterator_create<Vertex2<float> >();

	if (state.mpVector->size()>=2)
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
			(*borderCreatingFunction)(prevVertex, currVertex, nextVertex, 
				pBorderTriangleStrip, borderWidth, curveSegmentsCount);

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
*
* left, top, width, height: properties of the component
* currentHeight: the height of the desired viewport
*/
void createBoxVertices(std::vector<Vertex2<float> >* boxVertices,
					   float left, float top, float width, float height,
					   float currentHeight);

void drawVertexArray(const std::vector<Vertex2<float> >* vertices, Color4<float> colors[4]);

void drawVertexArray(const std::vector<Vertex2<float> >* vertices, Color4<float> color);

#endif
