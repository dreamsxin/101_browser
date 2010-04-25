#include "BasicDataStructures/Memory/SafeMemoryManagement.h"
#include "gui/MultiMouse.h"
#include "GuiWin/GuiWin.h"
#include <windows.h>

namespace Gui
{
	namespace Mouse
	{
		void initMultiMouse()
		{
			UINT nDevices;
			if (GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0)
			{
				showErrorMessageBox(L"Unable to count raw input devices");
				exit(1);
			}

			PRAWINPUTDEVICELIST pRawInputDeviceList;
			if ((pRawInputDeviceList = (PRAWINPUTDEVICELIST) 
				malloc(sizeof(RAWINPUTDEVICELIST) * nDevices)) == NULL)
			{
				exit(1);
			}

			if (GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST)) == (UINT)-1)
			{
				showErrorMessageBox(L"Unable to get raw input device list");
				exit(1);
			}

			safe_free(&pRawInputDeviceList);
		}
	}
}
