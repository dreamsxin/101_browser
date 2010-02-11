#include <vector>

#include "gui/GuiComponentsCheckBox.h"
#include "gui/GuiComponentsBasic.h"
#include "gui/GuiComponentsDefaults.h"

void drawCheckBox(float left, float top, float width, float height, float borderSize, 
				  float currentHeight)
{
	Color4f colors[4]={
		cCheckBoxBottomLeftBackgroundColor,
		cCheckBoxBottomRightBackgroundColor,
		cCheckBoxTopLeftBackgroundColor, 
		cCheckBoxTopRightBackgroundColor
		};
	std::vector<Vertex> boxVertices;
	std::vector<Vertex> borderVertices;
	createBoxVertices(&boxVertices, left, top, width, height, currentHeight);
	createBorderVertices(&boxVertices, &borderVertices, borderSize);
	drawVertexArray(&boxVertices, colors);
	drawVertexArray(&borderVertices, cCheckBoxBorderColor);
}