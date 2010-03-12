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
			Color4<float> colors[4]={
				Defaults::cCheckBoxBottomLeftBackgroundColor,
				Defaults::cCheckBoxBottomRightBackgroundColor,
				Defaults::cCheckBoxTopLeftBackgroundColor, 
				Defaults::cCheckBoxTopRightBackgroundColor
			};

			std::vector<Vertex2<float> > boxVertices;
			std::vector<Vertex2<float> > borderTriangleStrip;
			createBoxVertices(&boxVertices, left, top, width, height, currentHeight);
			createBorderTriangleStrip(&boxVertices, &borderTriangleStrip, borderSize);
			drawVertexArray(&boxVertices, colors);
			drawVertexArray(&borderTriangleStrip, Defaults::cCheckBoxBorderColor);
		}
	}
}
