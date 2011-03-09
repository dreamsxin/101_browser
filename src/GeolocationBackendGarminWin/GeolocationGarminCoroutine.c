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

#include "GeolocationBackendGarminWin/GeolocationGarminCoroutineDataTypes.h"
#include "GeolocationBackendGarminWin/GeolocationGarminInternalMemory.h"

#include <assert.h>
#include <malloc.h> // for _msize
#include <stdio.h>  // for printf

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

			if (!DeviceIoControl(pGarminUsbData->garminHandle,
				IOCTL_ASYNC_IN,
				0,
				0,
				theBuffer+theBufferSize,
				ASYNC_DATA_SIZE,
				&theBytesReturned,
				NULL))
			{
				pGarminUsbData->coroutineState = GarminCoroutineStateErrorDeviceIoControl;
				switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

				return;
			}

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
				pGarminUsbData->coroutineState = GarminCoroutineStateOkSendingPossible;
				pGarminUsbData->pPacket = (Packet_t*) theBuffer;
				switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

				safe_free(&theBuffer);

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
			pGarminUsbData->pPacket = NULL;
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
				pGarminUsbData->pPacket = NULL;
				switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

				return;
			}

			if (theBytesReturned == 0)
			{
				// LBL: GeolocationGarminCoroutine:125
				safe_free(&theBuffer);

				pGarminUsbData->coroutineState = GarminCoroutineStateOkSendingPossible;
				pGarminUsbData->pPacket = NULL;
				switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);
				
				break;
			}

			if (!isPacketBufferOfCorrectSize((Packet_t*) theBuffer, theBytesReturned))
			{
				safe_free(&theBuffer);

				pGarminUsbData->coroutineState = GarminCoroutineStateErrorInvalidData;
				pGarminUsbData->pPacket = NULL;
				switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

				return;
			}
			else
			{
				pGarminUsbData->coroutineState = GarminCoroutineStateOkSendingNotPossible;
				pGarminUsbData->pPacket = (Packet_t*) theBuffer;
				switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

				/*
				 * We don't free the buffer here, since it would not be reallocated.
				 * Instead we free it at LBL: GeolocationGarminCoroutine:125
				 */

				continue;
			}
		}
	}
}
