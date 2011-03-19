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

#include "GeolocationBackendGarminWin/GeolocationGarminInitAndTerminate.h"
#include "GeolocationBackendGarminWin/GeolocationGarminPacketFunctions.h" // for isGarminCoroutineStateActive

#include <assert.h>

#include <windows.h>
#include <initguid.h>
#include <setupapi.h>
#include <winioctl.h>


#define IOCTL_USB_PACKET_SIZE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x851, METHOD_BUFFERED, FILE_ANY_ACCESS)

DEFINE_GUID(GUID_DEVINTERFACE_GRMNUSB, 0x2c9c45c2L, 0x8e7d, 0x4c08, 0xa1, 0x2d, 0x81, 0x6b, 0xba, 0xe7, 0x22, 0xc0);

HANDLE initializeGeolocationBackendGarmin(WORD *in_out_pUsbSize)
{
	DWORD theBytesReturned = 0;
	HANDLE out_handle = 0;

	HDEVINFO theDevInfo = SetupDiGetClassDevs(
		(GUID*) &GUID_DEVINTERFACE_GRMNUSB,
		NULL,
		NULL,
		DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);

	SP_DEVICE_INTERFACE_DATA theInterfaceData;
	theInterfaceData.cbSize = sizeof(theInterfaceData);

	if(!SetupDiEnumDeviceInterfaces(theDevInfo,
		NULL, (GUID*) &GUID_DEVINTERFACE_GRMNUSB, 0, &theInterfaceData ) &&
		GetLastError() == ERROR_NO_MORE_ITEMS)
	{
		return 0;
	}

	SetupDiGetDeviceInterfaceDetail(theDevInfo, &theInterfaceData, NULL, 0, 
		&theBytesReturned, NULL);

	{
		PSP_INTERFACE_DEVICE_DETAIL_DATA theDevDetailData = 0;
		SP_DEVINFO_DATA theDevInfoData = { sizeof(SP_DEVINFO_DATA) };

		theDevDetailData = (PSP_INTERFACE_DEVICE_DETAIL_DATA) malloc(theBytesReturned);

		if (theDevDetailData == NULL)
			return 0;

		theDevDetailData->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);

		SetupDiGetDeviceInterfaceDetail(theDevInfo, &theInterfaceData,
			theDevDetailData, theBytesReturned, NULL, &theDevInfoData);

		out_handle = CreateFile(theDevDetailData->DevicePath,
			GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);

		free(theDevDetailData);
	}

	// Get the USB packet size, which we need for sending packets
	DeviceIoControl(out_handle, IOCTL_USB_PACKET_SIZE, 0, 0, in_out_pUsbSize,
		sizeof(*in_out_pUsbSize), &theBytesReturned, NULL);

	return out_handle;
}

bool createGeolocationCoroutine(GarminUsbData *out_pGarminUsbData, 
	CoroutineDescriptor *in_pCurrentCoroutine, 
	CoroutineDescriptor *in_pGeolocationCoroutine,
	HANDLE in_garminHandle)
{
	out_pGarminUsbData->pMainCoroutine = in_pCurrentCoroutine;
	out_pGarminUsbData->pGeolocationCoroutine = in_pGeolocationCoroutine;
	out_pGarminUsbData->garminHandle = in_garminHandle;
	out_pGarminUsbData->coroutineState = GarminCoroutineStateOkSendingPossible;
	out_pGarminUsbData->pPacket = NULL;

	return createCoroutine(0, &geolocationCoroutine, out_pGarminUsbData, out_pGarminUsbData->pGeolocationCoroutine);
}

void startGeolocationCoroutine(GarminUsbData *in_out_pGarminUsbData)
{
	switchToCoroutine(in_out_pGarminUsbData->pMainCoroutine, in_out_pGarminUsbData->pGeolocationCoroutine);
}

void stopGeolocationCoroutine(GarminUsbData *in_out_pGarminUsbData)
{
	if (isGarminCoroutineStateActive(in_out_pGarminUsbData->coroutineState))
	{
		in_out_pGarminUsbData->coroutineState = GarminCoroutineStateRequestToTerminate;
		switchToCoroutine(in_out_pGarminUsbData->pMainCoroutine, in_out_pGarminUsbData->pGeolocationCoroutine);
		assert(GarminCoroutineStateTerminated == in_out_pGarminUsbData->coroutineState);
	}
}
