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

			inline RawMouse(size_t in_buttonsCount) : buttonsPressed(in_buttonsCount) { }
		};

		ArrayBlock<RawMouse>* getRawMouseArray();
		inline void destroyRawMouseArray(ArrayBlock<RawMouse>** in_rawMice)
		{
			safe_delete(in_rawMice);
		}

		void registerRawMice(HWND hWnd);
		void unregisterRawMice();
	}
}

#endif
