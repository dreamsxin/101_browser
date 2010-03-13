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
			createBorderTriangleStrip(&boxVertices, &borderTriangleStrip, 
				&createStraightBorder, borderSize, 12345);
			std::vector<Vertex2<float> > coloredBorder;
			createBorderTriangleStrip(&borderTriangleStrip, &coloredBorder, 
				&createRoundBorder, 4, 4);
			drawVertexArray(&boxVertices, colors);
			drawVertexArray(&borderTriangleStrip, Defaults::cCheckBoxBorderColor);
			// Before we reactivate this line we have to use an iterator using only
			// the outer contour
#if 0
			drawVertexArray(&coloredBorder, Defaults::cColorRed);
#endif
		}
	}
}
