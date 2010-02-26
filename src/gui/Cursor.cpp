#include "gui/Cursor.h"
#include "gui/Texture.h"
#include <cassert>
#if 0
// for creating a BMP file for testing purposes
#include <cstdio>
#endif

namespace Gui
{
	bool createCursor(Cursor* in_pCursor)
	{
		HCURSOR hCursor;
		ICONINFO iconInfo;
		HDC hDC = CreateCompatibleDC(NULL);

		if (!(hCursor = LoadCursor(NULL, 
			IDC_ARROW
			//IDC_CROSS
			)))
		{
			return false;
		}
		
		if (!GetIconInfo(hCursor, &iconInfo))
		{
			return false;
		}

		// This means we really got a cursor and not an icon
		assert(iconInfo.fIcon == FALSE);

		BITMAP maskBitmap;
		bool isColorIcon = (iconInfo.hbmColor != NULL);
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
		// Note: remove the minus sign if you want to create a BMP file
		// of the cursor for testing purposes
		bih.biHeight = -maskBitmap.bmHeight;
		bih.biPlanes = 1;
		// a byte has 8 bits, so we do <<3, which multiplies by 8
		bih.biBitCount = colorModePixelBytesCount(andMap.colorMode)<<3;
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

#if 0
		// Write the result to a BMP file for testing
		BITMAPFILEHEADER bmfHeader;
		bmfHeader.bfType = 0x4D42;
		bmfHeader.bfSize = textureBytesCount(&andMap) + textureBytesCount(&xorMap)
			+ sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		bmfHeader.bfReserved1 = 0;
		bmfHeader.bfReserved2 = 0;
		bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

		FILE* cursorBmpFile = fopen("cursor.bmp", "w+b");
		if (cursorBmpFile)
		{
			fwrite(&bmfHeader, sizeof(BITMAPFILEHEADER), 1, cursorBmpFile);
			bih.biSizeImage *= 2;
			fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, cursorBmpFile);
			fwrite(andMap.data, textureBytesCount(&andMap), 1, cursorBmpFile);
			fwrite(xorMap.data, textureBytesCount(&xorMap), 1, cursorBmpFile);
			fclose(cursorBmpFile);
		}
#endif

		in_pCursor->hotspot  = Vertex2<DWORD>(iconInfo.xHotspot, iconInfo.yHotspot);
		in_pCursor->colored  = isColorIcon;
		in_pCursor->andMap   = andMap;
		in_pCursor->xorMap   = !isColorIcon ? xorMap : Texture();
		in_pCursor->colorMap = Texture();

		DeleteObject(iconInfo.hbmMask);
		if (isColorIcon)
			DeleteObject(iconInfo.hbmColor);

		return true;
	}
}
