#ifndef _Cursor_h
#define _Cursor_h

#include "gui/Texture.h"
#include "gui/Vertex.h"
#include "windows.h" // for DWORD

namespace Gui
{
	struct Cursor
	{
		Vertex2<DWORD> hotspot;
		bool colored; // if colored is false, andMap and xorMap are valid
		              // if colored is true, andMap and colorMap are valid
		Texture andMap;
		// if the cursor is not colored it is used as xor map; else it is used as
		// color map
		Texture xorColorMap;
	};

	/*
	 * Tries to create a cursor texture. 
	 * Return value:
	 * true: then *in_pAndMap was changed and *either*  *in_pXorMap or *in_pColorMap 
	 *       will become changed but this need not be the case
	 * false: something wrong happened
	 */
	bool createCursor(Cursor* in_pCursor);
}

#endif
