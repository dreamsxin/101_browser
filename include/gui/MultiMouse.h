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
			TCHAR* psName;
			ArrayBlock<bool> buttonsPressed;
		};

		ArrayBlock<RawMouse> getRawMouseArray();
		void registerRawMice(HWND hWnd);
		void unregisterRawMice();
	}
}

#endif
