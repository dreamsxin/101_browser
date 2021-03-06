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

#include "gui/GuiComponentsCursor.h"
#include "GuiOpenGL/GuiComponentsBasic.h"
#include "GuiOpenGL/GuiComponentsDefaults.h"
#include "GL/glext.h"

namespace Gui
{
	namespace Components
	{
		void drawCursor(float left, float top, float currentHeight, 
			const Cursor* in_pCursor)
		{
			std::vector<Vertex2<float> > boxVertices;
			createBoxVertices(&boxVertices, 
				(float) (left-in_pCursor->hotspot.x), 
				(float) (top-in_pCursor->hotspot.y),
				(float) (in_pCursor->andMap.width), 
				(float) (in_pCursor->andMap.height), 
				currentHeight);
			Vertex2<float> textureUV[4]=
			{
				Vertex2<float>(0.0f, 0.0f),
				Vertex2<float>(1.0f, 0.0f),
				Vertex2<float>(0.0f, 1.0f),
				Vertex2<float>(1.0f, 1.0f)
			};

			glColor3fv(&Defaults::cColorWhite.r);

			glEnable(GL_TEXTURE_2D);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glVertexPointer(2, GL_FLOAT, 0, &boxVertices.at(0));
			glTexCoordPointer(2, GL_FLOAT, 0, textureUV);

			glEnable(GL_BLEND);

			glBindTexture(GL_TEXTURE_2D, in_pCursor->andMap.textureID);
			glBlendFunc(GL_DST_COLOR, GL_ZERO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glBindTexture(GL_TEXTURE_2D, in_pCursor->xorColorMap.textureID);
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glDisable(GL_BLEND);

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisable(GL_TEXTURE_2D);
		}
	}
}
