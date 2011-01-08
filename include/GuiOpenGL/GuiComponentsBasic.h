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

#ifndef _GuiComponentsBasic_h
#define _GuiComponentsBasic_h

#include <vector>

#include "gui/Vertex.h"
#include "GraphicsDataStructures/Color.h"
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
									  DoubleIteratorInstance<
									  TriangleStripBorderIterator<Vertex2<float> >::IteratorState> itInstance,
									  std::vector<Vertex2<float> >* pBorderTriangleStrip,
									  void (*borderCreatingFunction)(Vertex2<float>, 
									  Vertex2<float>, Vertex2<float>, 
									  std::vector<Vertex2<float> >*, 
									  float, size_t),
									  float borderWidth,
									  size_t curveSegmentsCount)
{
	assert(itInstance.state.mpVector->size()>=2);

	IterateResult itRes;

	itRes = (*itInstance.iterator.mpfIteratePrev)(&itInstance.state);
	assert(itRes == IterateResultOverBoundary);

	Vertex2<float> prevVertex = *(Vertex2<float>*) (*itInstance.iterator.mpfGet)(&itInstance.state);

	itRes = itInstance.iterator.mpfIterateNext(&itInstance.state);
	assert(itRes == IterateResultOverBoundary);

	Vertex2<float> currVertex = *(Vertex2<float>*) (*itInstance.iterator.mpfGet)(&itInstance.state);

	itRes = itInstance.iterator.mpfIterateNext(&itInstance.state);
	assert(itRes == IterateResultOK);

	Vertex2<float> nextVertex = *(Vertex2<float>*) (*itInstance.iterator.mpfGet)(&itInstance.state);

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
		itRes = (*itInstance.iterator.mpfIterateNext)(&itInstance.state);

		if (breakAfterSecondNextIteration)
		{
			breakAfterNextIteration = true;
		}

		// if this is true we do exactly one additional iteration
		if (itRes == IterateResultOverBoundary)
			breakAfterSecondNextIteration = true;

		prevVertex = currVertex;
		currVertex = nextVertex;
		nextVertex = *(Vertex2<float>*) (*itInstance.iterator.mpfGet)(&itInstance.state);
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
