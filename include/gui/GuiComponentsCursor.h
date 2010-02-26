#ifndef _GuiComponentsCursor_h
#define _GuiComponentsCursor_h

#include "gui/Cursor.h"

namespace Gui
{
	namespace Components
	{
		/*!
		 * Draws a cursor that has its hotspot in position (left, top)
		 */
		void drawCursor(float left, float top, const Cursor* cursor);
	}
}


#endif
