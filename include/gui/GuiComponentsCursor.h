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
