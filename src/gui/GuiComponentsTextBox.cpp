#include <vector>

#include "gui/GuiComponentsTextBox.h"
#include "gui/GuiComponentsBasic.h"
#include "gui/GuiComponentsDefaults.h"

void drawTextBox(float left, float top, float width, float height, float borderSize, 
				 float currentHeight)
{
	std::vector<Vertex> boxVertices;
	std::vector<Vertex> borderVertices;
	createBoxVertices(&boxVertices, left, top, width, height, currentHeight);
	createBorderVertices(&boxVertices, &borderVertices, borderSize);
	drawVertexArray(&boxVertices, cTextBoxBackgroundColor);
	drawVertexArray(&borderVertices, cTextBoxBorderColor);
}