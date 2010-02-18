#include "gui/Cursor.h"
#include <cassert>

namespace Gui
{
	bool createCursorImage()
	{
		HCURSOR hCursor;
		ICONINFO iconInfo;

		if (!(hCursor = LoadCursor(NULL, IDC_ARROW)))
		{
			return false;
		}
		
		if (!GetIconInfo(hCursor, &iconInfo))
		{
			return false;
		}

		// This means we really got a cursor and not an icon
		assert(iconInfo.fIcon == FALSE);

		Cursor cursor = { Vertex2<DWORD>(iconInfo.xHotspot, iconInfo.yHotspot) };

		BITMAP maskBitmap;
		BITMAP colorBitmap;

		if (!GetObject(iconInfo.hbmMask, sizeof(BITMAP), &maskBitmap))
		{
			DeleteObject(iconInfo.hbmMask);
			DeleteObject(iconInfo.hbmColor);
			return false;
		}
		if (!GetObject(iconInfo.hbmColor, sizeof(BITMAP), &colorBitmap))
		{
			DeleteObject(iconInfo.hbmMask);
			DeleteObject(iconInfo.hbmColor);
			return false;
		}

		// ...

		DeleteObject(iconInfo.hbmMask);
		DeleteObject(iconInfo.hbmColor);

		return true;
	}
}
