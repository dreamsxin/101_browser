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
#include "MiniStdlib/xchg.h"

#define IOCTL_ASYNC_IN CTL_CODE (FILE_DEVICE_UNKNOWN, 0x850, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define MAX_BUFFER_SIZE 4096
#define ASYNC_DATA_SIZE 64

void __stdcall geolocationCoroutine(void *in_pGarminUsbData)
{
	GarminUsbData *pGarminUsbData = (GarminUsbData*) in_pGarminUsbData;

	DWORD theBytesReturned;
	BYTE* theBuffer = NULL;

begin_of_geolocationCoroutine_loop:
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
				pGarminUsbData->pPacket = NULL;
				switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

				return;
			}

			if (!DeviceIoControl(
				pGarminUsbData->garminHandle,
				IOCTL_ASYNC_IN,
				0, 0,
				theBuffer+theBufferSize, ASYNC_DATA_SIZE,
				&theBytesReturned,
				NULL))
			{
				pGarminUsbData->coroutineState = GarminCoroutineStateErrorDeviceIoControl;
				pGarminUsbData->pPacket = NULL;
				switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

				return;
			}

			theBufferSize += ASYNC_DATA_SIZE;
			readBytesCount += theBytesReturned;

			assert(readBytesCount <= theBufferSize);
			assert(_msize(theBuffer) == theBufferSize);
		}

		// So we have read the async packet

		if (!isPacketBufferOfCorrectSize((Packet_t*) theBuffer, readBytesCount))
		{
			safe_free(&theBuffer);
			pGarminUsbData->coroutineState = GarminCoroutineStateErrorInvalidData;
			pGarminUsbData->pPacket = NULL;
			switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

			return;
		}

		if (PacketType_USB_Protocol_Layer != ((Packet_t*) theBuffer)->mPacketType || 
			Pid_Data_Available != ((Packet_t*) theBuffer)->mPacketId)
		{
			pGarminUsbData->coroutineState = GarminCoroutineStateOkSendingPossible;
			pGarminUsbData->pPacket = (Packet_t*) theBuffer;
			switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

			safe_free(&theBuffer);

			if (GarminCoroutineStateRequestToTerminate == pGarminUsbData->coroutineState)
			{
				pGarminUsbData->coroutineState = GarminCoroutineStateTerminated;
				return;
			}

			continue;
		}

		/*
		* We got a signal package before that we haven't deleted
		*/
		assert(theBuffer != NULL);
		safe_free(&theBuffer);

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
			safe_free(&theBuffer);

			goto begin_of_geolocationCoroutine_loop;
		}

		if (!isPacketBufferOfCorrectSize((Packet_t*) theBuffer, theBytesReturned))
		{
			safe_free(&theBuffer);

			pGarminUsbData->coroutineState = GarminCoroutineStateErrorInvalidData;
			pGarminUsbData->pPacket = NULL;
			switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

			return;
		}

		{
			BYTE *theNextBuffer = NULL;

			theNextBuffer = (BYTE*) malloc(MAX_BUFFER_SIZE);

			if (theNextBuffer == NULL)
			{
				safe_free(theBuffer);
				pGarminUsbData->coroutineState = GarminCoroutineStateErrorAllocation;
				pGarminUsbData->pPacket = NULL;
				switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

				return;
			}

			/*
			* Keep reading packets 
			* until the driver returns a 0 size buffer.
			*/
			while (1)
			{
				if (!ReadFile(pGarminUsbData->garminHandle,
					theNextBuffer,
					MAX_BUFFER_SIZE,
					&theBytesReturned,
					NULL))
				{
					safe_free(&theNextBuffer);
					safe_free(&theBuffer);

					pGarminUsbData->coroutineState = GarminCoroutineStateErrorReadFile;
					pGarminUsbData->pPacket = NULL;
					switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

					return;
				}

				if (theBytesReturned == 0)
				{
					safe_free(&theNextBuffer);

					pGarminUsbData->coroutineState = GarminCoroutineStateOkSendingPossible;
					pGarminUsbData->pPacket = (Packet_t*) theBuffer;
					switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

					safe_free(&theBuffer);

					if (GarminCoroutineStateRequestToTerminate == pGarminUsbData->coroutineState)
					{
						pGarminUsbData->coroutineState = GarminCoroutineStateTerminated;
						return;
					}

					break;
				}

				if (!isPacketBufferOfCorrectSize((Packet_t*) theNextBuffer, theBytesReturned))
				{
					safe_free(&theNextBuffer);
					safe_free(&theBuffer);

					pGarminUsbData->coroutineState = GarminCoroutineStateErrorInvalidData;
					pGarminUsbData->pPacket = NULL;
					switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

					return;
				}
				
				pGarminUsbData->coroutineState = GarminCoroutineStateOkSendingNotPossible;
				pGarminUsbData->pPacket = (Packet_t*) theBuffer;
				switchToCoroutine(pGarminUsbData->pGeolocationCoroutine, pGarminUsbData->pMainCoroutine);

				if (GarminCoroutineStateRequestToTerminate == pGarminUsbData->coroutineState)
				{
					safe_free(&theNextBuffer);
					safe_free(&theBuffer);

					pGarminUsbData->coroutineState = GarminCoroutineStateTerminated;
					return;
				}

				xchg(&theBuffer, &theNextBuffer, sizeof(BYTE*));
			}
		}
	}
}
