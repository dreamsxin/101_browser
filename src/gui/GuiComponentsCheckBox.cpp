#include <vector>

#include "gui/GuiComponentsCheckBox.h"
#include "gui/GuiComponentsBasic.h"
#include "gui/GuiComponentsDefaults.h"

namespace Gui
{
	namespace Components
	{
		void drawCheckBox(float left, float top, float width, float height, 
			float borderSize, float currentHeight)
		{
			Color4f colors[4]={
				Defaults::cCheckBoxBottomLeftBackgroundColor,
				Defaults::cCheckBoxBottomRightBackgroundColor,
				Defaults::cCheckBoxTopLeftBackgroundColor, 
				Defaults::cCheckBoxTopRightBackgroundColor
			};
			std::vector<Vertex2<float> > boxVertices;
			std::vector<Vertex2<float> > borderVertices;
			createBoxVertices(&boxVertices, left, top, width, height, currentHeight);
			createBorderVertices(&boxVertices, &borderVertices, borderSize);
			drawVertexArray(&boxVertices, colors);
			drawVertexArray(&borderVertices, Defaults::cCheckBoxBorderColor);
		}
	}
}
