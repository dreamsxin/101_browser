#include "gui/GuiComponentsCursor.h"
#include "gui/GuiComponentsBasic.h"
#include "gui/GuiComponentsDefaults.h"
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
				left-in_pCursor->hotspot.x, top-in_pCursor->hotspot.y,
				in_pCursor->andMap.width, in_pCursor->andMap.height, currentHeight);
			Vertex2<float> textureUV[4]=
			{
				Vertex2<float>(0.0f, 0.0f),
				Vertex2<float>(1.0f, 0.0f),
				Vertex2<float>(0.0f, 1.0f),
				Vertex2<float>(1.0f, 1.0f)
			};

			glColor3fv(&Defaults::cColorWhite.r);

			if (!in_pCursor->colored)
			{
				glEnable(GL_TEXTURE_2D);

				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				glVertexPointer(2, GL_FLOAT, 0, &boxVertices.at(0));
				glTexCoordPointer(2, GL_FLOAT, 0, textureUV);

				glEnable(GL_BLEND);

				glBindTexture(GL_TEXTURE_2D, in_pCursor->andMap.textureID);
				glBlendFunc(GL_DST_COLOR, GL_ZERO);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				glBindTexture(GL_TEXTURE_2D, in_pCursor->xorMap.textureID);
				glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				glDisable(GL_BLEND);

				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisable(GL_TEXTURE_2D);
			}
		}
	}
}
