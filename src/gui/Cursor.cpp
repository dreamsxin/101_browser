#include "gui/Cursor.h"
#include "gui/Texture.h"
#include <cassert>
// for creating a BMP file for testing purposes
#include <cstdio>

namespace Gui
{
	void createBMP(char *filename, HDC hDC, BITMAPINFOHEADER bih, HBITMAP hBitmap)
	{
		Texture out=Texture();
		out.width = bih.biWidth;
		out.height = bih.biHeight;
		out.colorMode = ColorModeRGBA;
		allocateTextureMemory(&out);
		GetDIBits(hDC, hBitmap, 
			0, out.height, 
			out.data, (BITMAPINFO*) &bih, 
			DIB_RGB_COLORS);
		
		// Write the result to a BMP file for testing
		BITMAPFILEHEADER bmfHeader;
		bmfHeader.bfType = 0x4D42;
		bmfHeader.bfSize = textureBytesCount(&out)
			+ sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		bmfHeader.bfReserved1 = 0;
		bmfHeader.bfReserved2 = 0;
		bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

		FILE* bmpFile;
		bmpFile = fopen(filename, "w+b");
		fwrite(&bmfHeader, sizeof(BITMAPFILEHEADER), 1, bmpFile);
		fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, bmpFile);
		fwrite(out.data, textureBytesCount(&out), 1, bmpFile);
			
		fclose(bmpFile);
	}

	bool createCursor(Cursor* in_pCursor)
	{
		HCURSOR hCursor;
		ICONINFO iconInfo;
		HDC hDC = CreateCompatibleDC(NULL);

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
				/*
				 * we tested above that it is a color icon - so we 
				 * may (and have to) do this
				 */
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
		Texture andMap, xorMap, colorMap;

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
			colorMap.colorMode = ColorModeRGBA;
			colorMap.width = colorBitmap.bmWidth;
			colorMap.height = colorBitmap.bmHeight;
			allocateTextureMemory(&colorMap);
		}

		allocateTextureMemory(&andMap);

		if (andMap.data == NULL)
		{
			DeleteObject(iconInfo.hbmMask);
			if (isColorIcon)
				DeleteObject(iconInfo.hbmColor);
			return false;
		}

		BITMAPINFOHEADER bih;
		bih.biSize = sizeof (BITMAPINFOHEADER);
		bih.biWidth = andMap.width;
		bih.biHeight = maskBitmap.bmHeight;
		bih.biPlanes = 1;
		// a byte has 8 bits, so we do <<3, which multiplies by 8
		bih.biBitCount = (WORD) colorModePixelBytesCount(andMap.colorMode)<<3;
		bih.biCompression = BI_RGB;
		bih.biSizeImage = 0;
		bih.biXPelsPerMeter = 0;
		bih.biYPelsPerMeter = 0;
		bih.biClrUsed = 0;
		bih.biClrImportant = 0;

		int retVal;
		
		retVal = GetDIBits(hDC, iconInfo.hbmMask, 
			// if we have no color icon: xorMap.height=0
			// otherwise it has a sensible value
			// so we can shorten
			// !isColorIcon ? xorMap.height : 0
			// to xorMap.height
			xorMap.height, andMap.height,
			andMap.data, (BITMAPINFO*) &bih, 
			DIB_RGB_COLORS);

		if (retVal < 0 || ((unsigned long) retVal) != andMap.height)
		{
			DeleteObject(iconInfo.hbmMask);
			if (isColorIcon)
				DeleteObject(iconInfo.hbmColor);
			return false;
		}
		
		if (!isColorIcon)
		{
			retVal  = GetDIBits(hDC, iconInfo.hbmMask, 
				0, xorMap.height, 
				xorMap.data, (BITMAPINFO*) &bih, 
				DIB_RGB_COLORS);

			if (retVal < 0 || ((unsigned long) retVal) != xorMap.height)
			{
				DeleteObject(iconInfo.hbmMask);

				assert(isColorIcon);
				if (isColorIcon)
					DeleteObject(iconInfo.hbmColor);
				return false;
			}
		}
		else
		{
			retVal = GetDIBits(hDC, iconInfo.hbmColor,
				0, colorMap.height,
				colorMap.data, (BITMAPINFO*) &bih, 
				DIB_RGB_COLORS);

			if (retVal < 0 || ((unsigned long) retVal) != colorMap.height)
			{
				DeleteObject(iconInfo.hbmColor);

				assert(isColorIcon);
				if (isColorIcon)
					DeleteObject(iconInfo.hbmColor);
				return false;
			}
		}

		in_pCursor->hotspot  = Vertex2<DWORD>(iconInfo.xHotspot, iconInfo.yHotspot);
		in_pCursor->colored  = isColorIcon;
		in_pCursor->andMap   = andMap;
		in_pCursor->xorColorMap   = !isColorIcon ? xorMap : colorMap;

		DeleteObject(iconInfo.hbmMask);
		if (isColorIcon)
			DeleteObject(iconInfo.hbmColor);

		return true;
	}
}
