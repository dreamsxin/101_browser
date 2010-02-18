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
			std::vector<Vertex2f> boxVertices;
			std::vector<Vertex2f> borderVertices;
			createBoxVertices(&boxVertices, left, top, width, height, currentHeight);
			createBorderVertices(&boxVertices, &borderVertices, borderSize);
			drawVertexArray(&boxVertices, Defaults::cTextBoxBackgroundColor);
			drawVertexArray(&borderVertices, Defaults::cTextBoxBorderColor);
		}
	}
}
