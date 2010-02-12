#ifndef _GuiComponentsBasic_h
#define _GuiComponentsBasic_h

#include <vector>

#include "gui/Vertex.h"
#include "gui/Color.h"

void createBorderVertices(const std::vector<Vertex2f>* boxVertices,
						  std::vector<Vertex2f>* borderVertices,
						  float borderWidth);

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
void createBoxVertices(std::vector<Vertex2f>* boxVertices,
					float left, float top, float width, float height,
					float currentHeight);

void drawVertexArray(const std::vector<Vertex2f>* vertices, Color4f colors[4]);

void drawVertexArray(const std::vector<Vertex2f>* vertices, Color4f color);

#endif
