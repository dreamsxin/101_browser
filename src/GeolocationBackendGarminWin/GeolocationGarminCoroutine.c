#include "GeolocationBackendGarminWin/GeolocationGarmin.h"

#include <assert.h>
#include <malloc.h> // for _msize
#include <stdio.h>  // for printf

#include "GeolocationBackendGarminWin/GeolocationGarminInternalMemory.h"
#include "MiniStdlib/safe_free.h" // for safe_free

#define IOCTL_ASYNC_IN CTL_CODE (FILE_DEVICE_UNKNOWN, 0x850, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define MAX_BUFFER_SIZE 4096
#define ASYNC_DATA_SIZE 64

void __stdcall geolocationCoroutine(void *in_pGarminUsbData)
{
	GarminUsbData *pGarminUsbData = (GarminUsbData*) in_pGarminUsbData;

	DWORD theBytesReturned;
	BYTE* theBuffer = NULL;

	while (1)
	{
		DWORD theBufferSize = 0;
		DWORD readBytesCount = 0;
		theBytesReturned = ASYNC_DATA_SIZE;

		while (ASYNC_DATA_SIZE == theBytesReturned)
		{
			if (theBuffer != NULL)
				assert(_msize(theBuffer) == theBufferSize);

			if (!reallocBufferAndItsSizeAndDeleteIfFailure(&theBuffer, theBufferSize, ASYNC_DATA_SIZE))
			{
				pGarminUsbData->coroutineState = GarminCoroutineStateErrorAllocation;
				switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

				return;
			}

			DeviceIoControl(pGarminUsbData->garminHandle,
				IOCTL_ASYNC_IN,
				0,
				0,
				theBuffer+theBufferSize,
				ASYNC_DATA_SIZE,
				&theBytesReturned,
				NULL);

			theBufferSize += ASYNC_DATA_SIZE;
			readBytesCount += theBytesReturned;

			assert(readBytesCount <= theBufferSize);
			assert(_msize(theBuffer) == theBufferSize);
		}

		if (PacketType_USB_Protocol_Layer != ((Packet_t*) theBuffer)->mPacketType || 
			Pid_Data_Available != ((Packet_t*) theBuffer)->mPacketId)
		{
			if (!isPacketBufferOfCorrectSize((Packet_t*) theBuffer, readBytesCount))
			{
				safe_free(&theBuffer);
				pGarminUsbData->coroutineState = GarminCoroutineStateErrorInvalidData;
				switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

				return;
			}
			else
			{
				if (pGarminUsbData->pPacketHandlerFunc)
					(*pGarminUsbData->pPacketHandlerFunc)((Packet_t*) theBuffer, pGarminUsbData->pPacketHandlerData);

				safe_free(&theBuffer);

				pGarminUsbData->coroutineState = GarminCoroutineStateOK;
				switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

				continue;
			}
		}

		/*
		 * If this was a small "signal" packet, read a real
		 * packet using ReadFile
		 */

		theBuffer = (BYTE*) malloc(MAX_BUFFER_SIZE);

		if (theBuffer == NULL)
		{
			pGarminUsbData->coroutineState = GarminCoroutineStateErrorAllocation;
			switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

			return;
		}

		/*
		 * Keep reading (and queueing) packets 
		 * until the driver returns a 0 size buffer.
		 */
		while (1)
		{
			if (!ReadFile(pGarminUsbData->garminHandle,
				theBuffer,
				MAX_BUFFER_SIZE,
				&theBytesReturned,
				NULL))
			{
				safe_free(&theBuffer);

				pGarminUsbData->coroutineState = GarminCoroutineStateErrorReadFile;
				switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

				return;
			}

			if (theBytesReturned == 0)
			{
				safe_free(&theBuffer);

				pGarminUsbData->coroutineState = GarminCoroutineStateOK;
				switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);
				
				break;
			}

			if (!isPacketBufferOfCorrectSize((Packet_t*) theBuffer, theBytesReturned))
			{
				safe_free(&theBuffer);

				pGarminUsbData->coroutineState = GarminCoroutineStateErrorInvalidData;
				switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

				return;
			}
			else
			{
				if (pGarminUsbData->pPacketHandlerFunc)
					(*pGarminUsbData->pPacketHandlerFunc)((Packet_t*) theBuffer, pGarminUsbData->pPacketHandlerData);

				/*
				 * We don't free the buffer here, since it would not be reallocated.
				 */

				continue;
			}
		}
	}
}
