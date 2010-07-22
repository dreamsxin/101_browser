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

			std::vector<Vertex2<float> > boxVertices;
			createBoxVertices(&boxVertices, left, top, width, height, currentHeight);

			std::vector<Vertex2<float> > borderTriangleStrip;
			createBorderTriangleStrip(
				triangleStripBorderIteratorInstance_create(&boxVertices), 
				&borderTriangleStrip, 
				&createStraightBorder, borderSize, 12345);

			std::vector<Vertex2<float> > coloredBorder;
			createBorderTriangleStrip(
				outerBorderIteratorInstance_create(&borderTriangleStrip), 
				&coloredBorder, 
				&createRoundBorder, 16, 8);
			drawVertexArray(&boxVertices, colors);
			drawVertexArray(&borderTriangleStrip, Defaults::cCheckBoxBorderColor);
			
#if 1
			drawVertexArray(&coloredBorder, Defaults::cColorRed);
#endif
		}
	}
}
