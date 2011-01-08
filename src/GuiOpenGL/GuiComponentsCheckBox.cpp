/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
