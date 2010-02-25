#include "gui/Cursor.h"
#include "gui/Texture.h"
#include <cassert>

namespace Gui
{
	bool createCursorImage(HDC hDC)
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
		bool isColorIcon = iconInfo.hbmColor;
		BITMAP colorBitmap;

		if (!GetObject(iconInfo.hbmMask, sizeof(BITMAP), &maskBitmap))
		{
			DeleteObject(iconInfo.hbmMask);
			if (isColorIcon)
				DeleteObject(iconInfo.hbmColor);
			return false;
		}

		if (isColorIcon)
		{
			if (!GetObject(iconInfo.hbmColor, sizeof(BITMAP), &colorBitmap))
			{
				DeleteObject(iconInfo.hbmMask);
#if 0
				// not necessary here since we tested this above
				if (isColorIcon)
#endif
					DeleteObject(iconInfo.hbmColor);
				return false;
			}
		}

		/*
		 * According to 
		 * http://msdn.microsoft.com/en-us/library/ms648052(VS.85).aspx
		 * If this structure defines a black and white icon, 
		 * this bitmask is formatted so that the upper half is 
		 * the icon AND bitmask and the lower half is the icon 
		 * XOR bitmask. Under this condition, the height should 
		 * be an even multiple of two. If this structure defines 
		 * a color icon, this mask only defines the AND bitmask 
		 * of the icon.
		 */
		Texture andMap, xorMap;

		assert(maskBitmap.bmPlanes == 1);
		assert(maskBitmap.bmBitsPixel == 1);

		andMap.colorMode = ColorModeRGBA;
		andMap.width = maskBitmap.bmWidth;

		if (!isColorIcon)
		{
			assert(maskBitmap.bmHeight%2 == 0);
			andMap.height = maskBitmap.bmHeight/2;
			xorMap.colorMode = ColorModeRGBA;
			xorMap.width = maskBitmap.bmWidth;
			xorMap.height = maskBitmap.bmHeight/2;
			allocateTextureMemory(&xorMap);
		}
		else
		{
			andMap.height=maskBitmap.bmHeight;
		}

		allocateTextureMemory(&andMap);

		if (andMap.data == NULL)
		{
			DeleteObject(iconInfo.hbmMask);
			if (isColorIcon)
				DeleteObject(iconInfo.hbmColor);
		}

		BITMAPINFOHEADER bih;
		bih.biSize = sizeof (BITMAPINFOHEADER);
		bih.biWidth = andMap.width;
		// setting this value to a negative one the texture gets 
		// mirrored vertically - this is how OpenGL wishes
		bih.biHeight = -maskBitmap.bmHeight;
		bih.biPlanes = 1;
		bih.biBitCount = 8*colorModePixelBytesCount(andMap.colorMode);
		bih.biCompression = BI_RGB;
		bih.biSizeImage = 0;
		bih.biXPelsPerMeter = 0;
		bih.biYPelsPerMeter = 0;
		bih.biClrUsed = 0;
		bih.biClrImportant = 0;
		
		if (!GetDIBits(hDC, iconInfo.hbmMask, 
			0, andMap.height, 
			andMap.data, (BITMAPINFO*) &bih, 
			DIB_RGB_COLORS))
		{
			DeleteObject(iconInfo.hbmMask);
			if (isColorIcon)
				DeleteObject(iconInfo.hbmColor);
		}

		if (!isColorIcon)
		{
			if (!GetDIBits(hDC, iconInfo.hbmMask, 
				andMap.height, xorMap.height, 
				xorMap.data, (BITMAPINFO*) &bih, 
				DIB_RGB_COLORS))
			{
				DeleteObject(iconInfo.hbmMask);
				if (isColorIcon)
					DeleteObject(iconInfo.hbmColor);
			}
		}

		DeleteObject(iconInfo.hbmMask);
		if (isColorIcon)
			DeleteObject(iconInfo.hbmColor);

		return true;
	}
}
