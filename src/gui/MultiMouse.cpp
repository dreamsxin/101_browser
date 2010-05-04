#include "BasicDataStructures/Memory/SafeMemoryManagement.h"
#include "gui/MultiMouse.h"
#include "GuiWin/GuiWin.h"
#include <cassert>
#include <list>
using namespace std;

namespace Gui
{
	namespace Mouse
	{
		ArrayBlock<RawMouse> initMultiMouse()
		{
			UINT nDevices;
			PRAWINPUTDEVICELIST pRawInputDeviceList;
			list<RawMouse> rawMiceList;

			if (GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0)
			{
				showErrorMessageBox(L"GetRawInputDeviceList() count");
				exit(1);
			}
			
			if ((pRawInputDeviceList = (PRAWINPUTDEVICELIST) 
				malloc(sizeof(RAWINPUTDEVICELIST) * nDevices)) == NULL)
			{
				showErrorMessageBox(L"malloc()");
				exit(1);
			}

			if (((INT) GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST))) < 0)
			{
				showErrorMessageBox(L"GetRawInputDeviceList() get");
				exit(1);
			}

			for (UINT currentDeviceIndex = 0; currentDeviceIndex < nDevices; currentDeviceIndex++)
			{
				DWORD currentDeviceType = pRawInputDeviceList[currentDeviceIndex].dwType;

				if (currentDeviceType == RIM_TYPEMOUSE)
				{
					UINT cbSize;
#if 0
					TCHAR* psName;

					if (GetRawInputDeviceInfo(currentDeviceType, RIDI_DEVICENAME, NULL, &cbSize) != 0)
					{
						showErrorMessageBox(L"GetRawInputDeviceInfo() count");
						exit(1);
					}

					if ((psName = (char*) malloc(sizeof(TCHAR) * cbSize)) == NULL)
					{
						showErrorMessageBox(L"malloc()");
						exit(1);
					}

					if (((INT) GetRawInputDeviceInfo(currentDeviceType, RIDI_DEVICENAME, psName, &cbSize)) < 0)
					{
						safe_free(psName);
						showErrorMessageBox(L"GetRawInputDeviceInfo() get");
						exit(1);
					}
#endif

					HANDLE hDevice = pRawInputDeviceList[currentDeviceIndex].hDevice;

					if (GetRawInputDeviceInfo(hDevice, RIDI_DEVICEINFO, NULL, &cbSize) != 0)
					{
						showErrorMessageBox(L"GetRawInputDeviceInfo() count");
						exit(1);
					}

					PRID_DEVICE_INFO pDeviceInfo = (PRID_DEVICE_INFO) malloc(cbSize);

					if (!pDeviceInfo)
					{
						showErrorMessageBox(L"malloc()");
						exit(1);
					}

					pDeviceInfo->cbSize = cbSize;

					if (((INT) GetRawInputDeviceInfo(hDevice, RIDI_DEVICEINFO, pDeviceInfo, &cbSize)) < 0)
					{
						showErrorMessageBox(L"GetRawInputDeviceInfo() get");
						exit(1);
					}

					assert(pDeviceInfo->dwType == RIM_TYPEMOUSE);

					RawMouse currentMouse;
					currentMouse.deviceHandle = pRawInputDeviceList[currentDeviceIndex].hDevice;
					currentMouse.x = 0;
					currentMouse.y = 0;
					currentMouse.z = 0;
					currentMouse.buttonsPressed.size = pDeviceInfo->mouse.dwNumberOfButtons;
					currentMouse.buttonsPressed.data = 
						(bool*) malloc(sizeof(bool)*currentMouse.buttonsPressed.size);

					if (!currentMouse.buttonsPressed.data)
					{
						showErrorMessageBox(L"malloc()");
						exit(1);
					}

					memset(currentMouse.buttonsPressed.data, 0, 
						sizeof(bool)*currentMouse.buttonsPressed.size);

					rawMiceList.push_back(currentMouse);
#if 0
					safe_free(psName);
#endif
				}
			}

			safe_free(&pRawInputDeviceList);

			ArrayBlock<RawMouse> out_rawMiceArray;
			out_rawMiceArray.size = rawMiceList.size();
			out_rawMiceArray.data = (RawMouse*) malloc(sizeof(RawMouse) * out_rawMiceArray.size);

			if (out_rawMiceArray.data == 0)
			{
				showErrorMessageBox(L"malloc()");
				exit(1);
			}

			size_t currentPos = 0;

			for (list<RawMouse>::iterator rawMouseIt = rawMiceList.begin(); 
				rawMouseIt != rawMiceList.end(); rawMouseIt++)
			{
				out_rawMiceArray.data[currentPos] = *rawMouseIt;
				currentPos++;
			}

			return out_rawMiceArray;
		}
	}
}
