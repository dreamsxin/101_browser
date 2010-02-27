#ifndef _GuiComponentsCursor_h
#define _GuiComponentsCursor_h

#include "gui/Cursor.h"

namespace Gui
{
	namespace Components
	{
		/*!
		 * Draws a cursor that has its hotspot in pixel position (left, top)
		 */
		void drawCursor(float left, float top, float currentHeight, 
			const Cursor* in_pCursor);
	}
}


#endif
