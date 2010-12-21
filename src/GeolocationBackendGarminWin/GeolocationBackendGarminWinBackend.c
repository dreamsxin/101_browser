#include "GeolocationBackendGarminWin/GeolocationBackendGarminWinBackend.h"
#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/safe_free.h"
#include <assert.h>
#include <string.h>
#include <malloc.h> // for _msize
#include <stdio.h>
#include <stddef.h> // for offsetof

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
bool reallocBufferAndItsSizeAndDeleteIfFailure(void *in_ppTheBuffer, size_t in_size, 
									 size_t in_additionalDataSize)
{
	void* theNewBuffer = realloc(*(void**) in_ppTheBuffer, in_size+in_additionalDataSize);

	if (theNewBuffer == NULL)
	{
		safe_free(in_ppTheBuffer);
		return false;
	}
	else
	{
		*(void**) in_ppTheBuffer = theNewBuffer;
		return true;
	}
}

bool isPacketBufferLargeEnough(Packet_t* in_pPacket, size_t in_bufferSize)
{
	assert(in_pPacket != NULL);

	/*
	 * It is more secure to assume the worst possible thing in this
	 * case.
	 */
	if (in_pPacket == NULL)
		return false;

	return in_pPacket->mDataSize + offsetof(Packet_t, mData) <= in_bufferSize;
}

ReceivePacketResult receivePacket(ReceivePacketState *in_pState, 
								  HANDLE in_garminHandle,
								  Packet_t** out_ppPacket)
{
	
	DWORD theBytesReturned;
	BYTE* theBuffer = NULL;

	switch (*in_pState)
	{
	case ReceiveByDeviceIoControl:
		{
			Packet_t* thePacket = NULL;
			DWORD theBufferSize = 0;
			theBytesReturned = ASYNC_DATA_SIZE;

			while (ASYNC_DATA_SIZE == theBytesReturned)
			{
				if (theBuffer != NULL)
					assert(_msize(theBuffer) == theBufferSize);

				if (!reallocBufferAndItsSizeAndDeleteIfFailure(&theBuffer, theBufferSize, ASYNC_DATA_SIZE))
				{
					return ReceivePacketResultErrorAllocation;
				}

				DeviceIoControl(in_garminHandle,
					IOCTL_ASYNC_IN,
					0,
					0,
					theBuffer+theBufferSize,
					ASYNC_DATA_SIZE,
					&theBytesReturned,
					NULL);

				theBufferSize += ASYNC_DATA_SIZE;

				assert(_msize(theBuffer) == theBufferSize);
			}

			thePacket = (Packet_t*) theBuffer;

			printf("Async: concrete data size=%u\n", thePacket->mDataSize);

			if (PacketType_USB_Protocol_Layer != thePacket->mPacketType || 
				Pid_Data_Available != thePacket->mPacketId)
			{
				if (!isPacketBufferLargeEnough((Packet_t*) theBuffer, theBufferSize))
				{
					safe_free(&theBuffer);
					return ReceivePacketResultErrorInvalidData;
				}
				else
				{
					*out_ppPacket = (Packet_t*) theBuffer;
					return ReceivePacketResultPacket;
				}
			}
			else
			{
				*in_pState = ReceiveByReadFile;
				safe_free(&theBuffer);
				// we continue in the next block ReceiveByReadFile
			}
		}
		// it is correct that there is no break here
	case ReceiveByReadFile:
		{
			theBuffer = (BYTE*) malloc(MAX_BUFFER_SIZE);

			if (theBuffer == NULL)
			{
				return ReceivePacketResultErrorAllocation;
			}

			if (!ReadFile(in_garminHandle,
				theBuffer,
				MAX_BUFFER_SIZE,
				&theBytesReturned,
				NULL))
			{
				return ReceivePacketResultErrorReadFile;
			}

			if (theBytesReturned == 0)
			{
				safe_free(&theBuffer);
				*in_pState = ReceiveByDeviceIoControl;
				return ReceivePacketResultNoPacket;
			}
			else
			{
				/*
				 * It would still be safe to use 4096 instead of theBytesReturned.
				 * But if theBytesReturned is not sufficient, we assume a 
				 * malicious attempt.
				 */
				if (!isPacketBufferLargeEnough((Packet_t*) theBuffer, theBytesReturned))
				{
					safe_free(&theBuffer);
					return ReceivePacketResultErrorInvalidData;
				}
				else
				{
					*out_ppPacket = (Packet_t*) theBuffer;
					return ReceivePacketResultPacketContinueRead;
				}
			}
		}
		break;
	default:
		return ReceivePacketResultErrorInvalidState;
	}
}

ReceivePacketResult waitForPacket(ReceivePacketState *in_pState, 
								  HANDLE in_garminHandle, 
								  Packet_t** out_ppPacket,
								  unsigned char in_type,
								  unsigned short in_pid)
{
	Packet_t* thePacket = NULL;

	do
	{
		ReceivePacketResult result = receivePacket(in_pState, in_garminHandle, &thePacket);

		if (isErrorResult(result))
		{
			return result;
		}
		else if (ReceivePacketResultNoPacket == result)
		{
			continue;
		}
		else
		{
			assert(ReceivePacketResultPacket == result || 
				ReceivePacketResultPacketContinueRead == result);

			if (thePacket->mPacketType == in_type && thePacket->mPacketId == in_pid)
			{
				*out_ppPacket = thePacket;
				return result;
			}

			freePacket(&thePacket);
		}		
	} while (1);
}

bool isErrorResult(ReceivePacketResult in_result)
{
	return in_result > ReceivePacketResultPacketContinueRead;
}
