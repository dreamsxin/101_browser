#include "BasicDataStructures/Memory/SafeMemoryManagement.h"
#include "gui/MultiMouse.h"
#include "GuiWin/GuiWin.h"
#include <tchar.h>
#include <hidusage.h>
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
				showErrorMessageBox(L"GetRawInputDeviceList() count");
				exit(1);
			}

			if ((pRawInputDeviceList = static_cast<PRAWINPUTDEVICELIST>( 
				malloc(sizeof(RAWINPUTDEVICELIST) * nDevices))) == NULL)
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
				HANDLE hDevice = pRawInputDeviceList[currentDeviceIndex].hDevice;

				if (currentDeviceType == RIM_TYPEMOUSE)
				{
					UINT cbSize;
					TCHAR* psName;

					if (GetRawInputDeviceInfo(hDevice, RIDI_DEVICENAME, NULL, &cbSize) != 0)
					{
						showErrorMessageBox(L"GetRawInputDeviceInfo() count");
						exit(1);
					}

					if ((psName = static_cast<TCHAR*>(malloc(sizeof(TCHAR) * cbSize))) == NULL)
					{
						showErrorMessageBox(L"malloc()");
						exit(1);
					}

					if (((INT) GetRawInputDeviceInfo(hDevice, RIDI_DEVICENAME, psName, &cbSize)) < 0)
					{
						safe_free(&psName);
						showErrorMessageBox(L"GetRawInputDeviceInfo() get");
						exit(1);
					}

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

					if (GetRawInputDeviceInfo(hDevice, RIDI_DEVICEINFO, NULL, &cbSize) != 0)
					{
						showErrorMessageBox(L"GetRawInputDeviceInfo() count");
						exit(1);
					}

					PRID_DEVICE_INFO pDeviceInfo = static_cast<PRID_DEVICE_INFO>(malloc(cbSize));

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
					currentMouse.psName = psName;
					currentMouse.buttonsPressed.size = pDeviceInfo->mouse.dwNumberOfButtons;
					currentMouse.buttonsPressed.data = 
						static_cast<bool*>(malloc(sizeof(bool)*currentMouse.buttonsPressed.size));

					if (!currentMouse.buttonsPressed.data)
					{
						showErrorMessageBox(L"malloc()");
						exit(1);
					}

					memset(currentMouse.buttonsPressed.data, 0, 
						sizeof(bool)*currentMouse.buttonsPressed.size);

					rawMiceList.push_back(currentMouse);
				}
			}

			safe_free(&pRawInputDeviceList);

			ArrayBlock<RawMouse> out_rawMiceArray;
			out_rawMiceArray.size = rawMiceList.size();
			out_rawMiceArray.data = static_cast<RawMouse*>(
				malloc(sizeof(RawMouse) * out_rawMiceArray.size));

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
				showErrorMessageBox(L"RegisterRawInputDevices()");
				exit(1);
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
				showErrorMessageBox(L"RegisterRawInputDevices()");
				exit(1);
			}
		}
	}
}
