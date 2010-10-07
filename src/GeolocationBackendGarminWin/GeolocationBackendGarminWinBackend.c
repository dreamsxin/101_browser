#include "GeolocationBackendGarminWin/GeolocationBackendGarminWinBackend.h"
#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/safe_free.h"
#include <assert.h>
#include <string.h>
#include <malloc.h> // for _msize
#include <stdio.h>

#define IOCTL_ASYNC_IN        CTL_CODE (FILE_DEVICE_UNKNOWN, 0x850, METHOD_BUFFERED, FILE_ANY_ACCESS)

void freePacket(Packet_t **in_ppPacket)
{
	safe_free(in_ppPacket);
}

void sendPacket(HANDLE in_garminHandle, Packet_t in_packet, WORD in_usbPacketSize)
{
	DWORD theBytesToWrite = sizeof(Packet_t) - 1 + in_packet.mDataSize;
	DWORD theBytesReturned = 0;

	WriteFile(in_garminHandle, &in_packet, theBytesToWrite, &theBytesReturned, NULL);

	if (theBytesToWrite % in_usbPacketSize == 0)
	{
		WriteFile(in_garminHandle, 0, 0, &theBytesReturned, NULL);
	}
}

#define MAX_BUFFER_SIZE 4096
#define ASYNC_DATA_SIZE 64

/*!
 * Return value: true on success
 *               false on failure
 */
bool reallocBufferAndItsSizeAndDeleteIfFailure(BYTE **in_ppTheBuffer, DWORD *in_pSize, 
									 size_t in_additionalDataSize)
{
	BYTE* theNewBuffer = (BYTE*) realloc(*in_ppTheBuffer, *in_pSize+in_additionalDataSize);

	if (theNewBuffer == NULL)
	{
		safe_free(in_ppTheBuffer);
		*in_pSize = 0;
		return false;
	}
	else
	{
		*in_ppTheBuffer = theNewBuffer;
		*in_pSize += in_additionalDataSize;
		return true;
	}
}

Packet_t* receivePacket(HANDLE in_garminHandle)
{
	Packet_t* thePacket = 0;
	DWORD theBytesReturned = ASYNC_DATA_SIZE;

	DWORD theBufferSize = 0;
	BYTE* theBuffer = NULL;

	while (ASYNC_DATA_SIZE == theBytesReturned)
	{
		if (theBuffer != NULL)
			assert(_msize(theBuffer) == theBufferSize);

		if (!reallocBufferAndItsSizeAndDeleteIfFailure(&theBuffer, &theBufferSize, ASYNC_DATA_SIZE))
			return NULL;

		assert(_msize(theBuffer) == theBufferSize);

		DeviceIoControl(in_garminHandle,
			IOCTL_ASYNC_IN,
			0,
			0,
			theBuffer+theBufferSize-ASYNC_DATA_SIZE,
			ASYNC_DATA_SIZE,
			&theBytesReturned,
			NULL);
	}

	thePacket = (Packet_t*) theBuffer;

	if (PacketType_USB_Protocol_Layer == thePacket->mPacketType &&
		Pid_Data_Available == thePacket->mPacketId)
	{
		safe_free(&thePacket);
		theBuffer = NULL;
		theBufferSize = 0;

		theBytesReturned = 1;
		while (theBytesReturned != 0)
		{
			if (theBuffer != NULL)
				assert(_msize(theBuffer) == theBufferSize);

			if (!reallocBufferAndItsSizeAndDeleteIfFailure(&theBuffer, &theBufferSize, MAX_BUFFER_SIZE))
				return NULL;

			assert(_msize(theBuffer) == theBufferSize);

			ReadFile(in_garminHandle,
				theBuffer+theBufferSize-MAX_BUFFER_SIZE,
				MAX_BUFFER_SIZE,
				&theBytesReturned,
				NULL);
		}

		return (Packet_t*) theBuffer;
	}
	else
	{
		return thePacket;
	}
}

Packet_t* waitForPacket(HANDLE in_garminHandle, unsigned char in_type, unsigned short in_pid)
{
	Packet_t* thePacket = NULL;

	do
	{
		thePacket = receivePacket(in_garminHandle);

		if (thePacket == NULL)
			return NULL;

		printf("%u %u\n", (unsigned) thePacket->mPacketType, (unsigned) thePacket->mPacketId);
		 
		if (thePacket->mPacketType == in_type && thePacket->mPacketId == in_pid)
		{
			return thePacket;
		}

		freePacket(&thePacket);
	} while (1);
}
