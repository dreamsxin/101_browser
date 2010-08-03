#include "Template/Memory/SafeMemoryManagement.h"
#include "MiniStdlib/safe_free.h"
#include "BasicDataStructures/Error/ErrorHandling.h"
#include "gui/MultiMouse.h"
#include "contrib/hidusage.h"
#include <tchar.h>
#include <cassert>
#include <list>
using namespace std;

namespace Gui
{
	namespace Mouse
	{
		ArrayBlock<RawMouse> getRawMouseArray()
		{
			UINT nDevices;
			PRAWINPUTDEVICELIST pRawInputDeviceList;
			list<RawMouse> rawMiceList;

			if (GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0)
			{
				showErrorMessageAndExit(L"GetRawInputDeviceList() count");
			}

			pRawInputDeviceList = safeMallocExitOnFailure<RAWINPUTDEVICELIST>(nDevices);

			if (((INT) GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST))) < 0)
			{
				showErrorMessageAndExit(L"GetRawInputDeviceList() get");
			}

			for (UINT currentDeviceIndex = 0; currentDeviceIndex < nDevices; currentDeviceIndex++)
			{
				DWORD currentDeviceType = pRawInputDeviceList[currentDeviceIndex].dwType;
				HANDLE hDevice = pRawInputDeviceList[currentDeviceIndex].hDevice;

				if (currentDeviceType == RIM_TYPEMOUSE)
				{
					UINT cbSize;
					TCHAR* psName;

					if (GetRawInputDeviceInfo(hDevice, RIDI_DEVICENAME, NULL, &cbSize) != 0)
					{
						showErrorMessageAndExit(L"GetRawInputDeviceInfo() count");
					}

					psName = safeMallocExitOnFailure<TCHAR>(cbSize);

					if (((INT) GetRawInputDeviceInfo(hDevice, RIDI_DEVICENAME, psName, &cbSize)) < 0)
					{
						safe_free(&psName);
						showErrorMessageAndExit(L"GetRawInputDeviceInfo() get");
					}

					assert(_tcsnlen(psName, cbSize) == cbSize - 1);

					// We want to ignore RDP mice
					{
						TCHAR rdpMouseName[] = _T("\\??\\Root#RDP_MOU#0000#");
						size_t rdpMouseNameStrlen = _tcslen(rdpMouseName);

						if ((_tcslen(psName) >= rdpMouseNameStrlen) && 
							_tcsncmp(rdpMouseName, psName, rdpMouseNameStrlen) == 0)
						{
							safe_free(&psName);
							continue;
						}
					}

					safe_free(&psName);

					if (GetRawInputDeviceInfo(hDevice, RIDI_DEVICEINFO, NULL, &cbSize) != 0)
					{
						showErrorMessageAndExit(L"GetRawInputDeviceInfo() count");
					}

					PRID_DEVICE_INFO pDeviceInfo = safeMallocBytesExitOnFailure<RID_DEVICE_INFO>(cbSize);

					pDeviceInfo->cbSize = cbSize;

					if (((INT) GetRawInputDeviceInfo(hDevice, RIDI_DEVICEINFO, pDeviceInfo, &cbSize)) < 0)
					{
						showErrorMessageAndExit(L"GetRawInputDeviceInfo() get");
					}

					assert(pDeviceInfo->dwType == RIM_TYPEMOUSE);

					RawMouse currentMouse;
					currentMouse.deviceHandle = pRawInputDeviceList[currentDeviceIndex].hDevice;
					currentMouse.x = 0;
					currentMouse.y = 0;
					currentMouse.z = 0;
					currentMouse.buttonsPressed = ArrayBlock<bool>();
					currentMouse.buttonsPressed.allocate(pDeviceInfo->mouse.dwNumberOfButtons);

					memset(currentMouse.buttonsPressed.data(), 0, 
						sizeof(bool)*currentMouse.buttonsPressed.count());

					rawMiceList.push_back(currentMouse);
				}
			}

			safe_free(&pRawInputDeviceList);

			ArrayBlock<RawMouse> out_rawMiceArray;
			out_rawMiceArray.allocate(rawMiceList.size());

			size_t currentPos = 0;

			for (list<RawMouse>::iterator rawMouseIt = rawMiceList.begin(); 
				rawMouseIt != rawMiceList.end(); rawMouseIt++)
			{
				out_rawMiceArray.data()[currentPos] = *rawMouseIt;
				currentPos++;
			}

			return out_rawMiceArray;
		}

		void syncRawMousePositions(ArrayBlock<RawMouse>* in_oldRawMice, ArrayBlock<RawMouse>* in_newRawMice)
		{
			assert(in_oldRawMice->count() < SIZE_MAX);
			assert(in_newRawMice->count() < SIZE_MAX);

			for (size_t lNewIndex = 0; lNewIndex < in_newRawMice->count(); lNewIndex++)
			{
				RawMouse* pCurrentNewRawMouse = in_newRawMice->data()+lNewIndex;

				for (size_t lOldIndex = 0; lOldIndex < in_oldRawMice->count(); lOldIndex++)
				{
					RawMouse* pCurrentOldRawMouse = in_oldRawMice->data()+lOldIndex;

					if (pCurrentNewRawMouse->deviceHandle == pCurrentOldRawMouse->deviceHandle)
					{
						pCurrentNewRawMouse->x = pCurrentOldRawMouse->x;
						pCurrentNewRawMouse->y = pCurrentOldRawMouse->y;
						pCurrentNewRawMouse->z = pCurrentOldRawMouse->z;
						break;
					}
				}
			}
		}

		void destroyRawMouseArray(ArrayBlock<RawMouse>* in_rawMice)
		{
			assert(in_rawMice->count() < SIZE_MAX);

			for (size_t index = 0; index < in_rawMice->count(); index++)
			{
				in_rawMice->data()[index].buttonsPressed.free();
			}

			in_rawMice->free();
		}

		void registerRawMice(HWND hWnd)
		{
			OSVERSIONINFO osvi;
			ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			GetVersionEx(&osvi);

			bool isVistaOrLater = osvi.dwMajorVersion >= 6;

			RAWINPUTDEVICE rawInputDevice;
			rawInputDevice.usUsagePage = HID_USAGE_PAGE_GENERIC;
			rawInputDevice.usUsage = HID_USAGE_GENERIC_MOUSE;
			rawInputDevice.dwFlags = RIDEV_CAPTUREMOUSE | RIDEV_NOLEGACY | 
				// this option works only under Windows Vista or later
				(isVistaOrLater ? RIDEV_DEVNOTIFY : 0); 
			rawInputDevice.hwndTarget = hWnd;

			if (RegisterRawInputDevices(&rawInputDevice, 1, sizeof(rawInputDevice)) != TRUE)
			{
				showErrorMessageAndExit(L"RegisterRawInputDevices()");
			}
		}

		void unregisterRawMice()
		{
			RAWINPUTDEVICE rawInputDevice;
			rawInputDevice.usUsagePage = HID_USAGE_PAGE_GENERIC;
			rawInputDevice.usUsage = HID_USAGE_GENERIC_MOUSE;
			rawInputDevice.dwFlags = RIDEV_REMOVE;
			rawInputDevice.hwndTarget = 0;

			if (RegisterRawInputDevices(&rawInputDevice, 1, sizeof(rawInputDevice)) != TRUE)
			{
				showErrorMessageAndExit(L"RegisterRawInputDevices()");
			}
		}
	}
}
