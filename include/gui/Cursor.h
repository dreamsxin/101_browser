#ifndef _Cursor_h
#define _Cursor_h

#include "gui/Vertex.h"
#include "windows.h"

namespace Gui
{
	struct Cursor
	{
		Vertex2<DWORD> hotspot;

	};

	bool createCursorImage(HDC hDC);
}

#endif
