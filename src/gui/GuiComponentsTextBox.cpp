#include <vector>

#include "gui/GuiComponentsTextBox.h"
#include "gui/GuiComponentsBasic.h"
#include "gui/GuiComponentsDefaults.h"

namespace Gui
{
	namespace Components
	{
		void drawTextBox(float left, float top, float width, float height, 
			float borderSize, float currentHeight)
		{
			std::vector<Vertex2<float> > boxVertices;
			std::vector<Vertex2<float> > borderTriangleStrip;
			createBoxVertices(&boxVertices, left, top, width, height, currentHeight);
			createBorderTriangleStrip(&boxVertices, &borderTriangleStrip, 
				&createStraightBorder, borderSize, 12345);
			drawVertexArray(&boxVertices, Defaults::cTextBoxBackgroundColor);
			drawVertexArray(&borderTriangleStrip, Defaults::cTextBoxBorderColor);
		}
	}
}
