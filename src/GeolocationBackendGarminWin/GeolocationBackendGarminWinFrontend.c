#include "GeolocationBackendGarminWin/GeolocationBackendGarminWinFrontend.h"
#include "GeolocationBackendGarminWin/GeolocationBackendGarminWinBackend.h"

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

bool startSession(HANDLE in_garminHandle, WORD in_usbPacketSize)
{
	Packet_t theStartSessionPacket = {
		PacketType_USB_Protocol_Layer,
		0, 0, // reserved fields
		Pid_Start_Session,
		0,    // data size
		0     // data
	};

	Packet_t* thePacket = NULL;

	bool sessionStartedPacketReceived = false;

	sendPacket(in_garminHandle, theStartSessionPacket, in_usbPacketSize);

	while (!sessionStartedPacketReceived)
	{
		thePacket = receivePacket(in_garminHandle);

		if (thePacket == NULL)
			return false;

		if (thePacket->mPacketType == PacketType_USB_Protocol_Layer &&
		thePacket->mPacketId == Pid_Session_Started)
		{
			sessionStartedPacketReceived = true;
		}

		freePacket(&thePacket);
	}

	return true;
}
