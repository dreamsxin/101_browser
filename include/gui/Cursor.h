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
	 * true: then *in_pAndMap was changed and *xorColorMap
	 *       will become changed but this need not be the case
	 * false: something wrong happened
	 */
	bool createCursor(Cursor* in_pCursor);
}

#endif
