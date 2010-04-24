#include <vector>

#include "GuiOpenGL/GuiComponentsCheckBox.h"
#include "GuiOpenGL/GuiComponentsBasic.h"
#include "GuiOpenGL/GuiComponentsDefaults.h"

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

			std::vector<const Vertex2<float> > boxVertices;
			createBoxVertices(&boxVertices, left, top, width, height, currentHeight);

			std::vector<const Vertex2<float> > borderTriangleStrip;
			createBorderTriangleStrip(
				triangleStripBorderConstIteratorInstance_create(&boxVertices), 
				&borderTriangleStrip, 
				&createStraightBorder, borderSize, 12345);

			std::vector<const Vertex2<float> > coloredBorder;
			
			createBorderTriangleStrip(
				outerBorderConstIteratorInstance_create(&borderTriangleStrip), 
				&coloredBorder, 
				&createRoundBorder, 16, 8);
			drawVertexArray(&boxVertices, colors);
			drawVertexArray(&borderTriangleStrip, Defaults::cCheckBoxBorderColor);
			// Before we reactivate this line we have to use an iterator using only
			// the outer contour
#if 1
			drawVertexArray(&coloredBorder, Defaults::cColorRed);
#endif
		}
	}
}
