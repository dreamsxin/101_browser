#ifndef _MultiMouse_h
#define _MultiMouse_h

#include <windows.h>
#include "BasicDataStructures/Memory/ArrayBlock.h"

namespace Gui
{
	namespace Mouse
	{
		struct RawMouse
		{
			HANDLE deviceHandle;
			ULONG x, y, z;
			ArrayBlock<bool> buttonsPressed;
		};

		ArrayBlock<RawMouse> getRawMouseArray();

		void syncRawMousePositions(ArrayBlock<RawMouse>* in_oldRawMice, ArrayBlock<RawMouse>* in_newRawMice);

		void destroyRawMouseArray(ArrayBlock<RawMouse>* in_rawMice);

		void registerRawMice(HWND hWnd);
		void unregisterRawMice();
	}
}

#endif
