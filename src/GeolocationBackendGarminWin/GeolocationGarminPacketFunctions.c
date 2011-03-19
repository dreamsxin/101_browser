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

#include "GeolocationBackendGarminWin/GeolocationGarminPackets.h"
#include "GeolocationBackendGarminWin/GeolocationGarminCoroutineStateFunctions.h"
#include "GeolocationBackendGarminWin/GeolocationGarminPacketFunctions.h"

bool sendPacket(GarminUsbData *in_pGarminUsbData, void *in_pPacket, 
	WORD in_usbPacketSize)
{
	if (!isSendingPossible(in_pGarminUsbData->coroutineState))
		return false;

	{
		DWORD theBytesToWrite = sizeof(Packet_t) - 1 + ((Packet_t *) in_pPacket)->mDataSize;
		DWORD theBytesReturned = 0;

		if (!WriteFile(in_pGarminUsbData->garminHandle, in_pPacket, theBytesToWrite, &theBytesReturned, NULL))
			return false;

		/*
		* If the packet size was an exact multiple of the USB packet
		* size, we must make a final write call with no data.
		*/
		if (theBytesToWrite % in_usbPacketSize == 0)
		{
			if (!WriteFile(in_pGarminUsbData->garminHandle, 0, 0, &theBytesReturned, NULL))
				return false;
		}

		return true;
	}
}

bool receivePacket(GarminUsbData *in_pGarminUsbData, Packet_t **in_ppPacket)
{
	if (!isGarminCoroutineStateActive(in_pGarminUsbData->coroutineState))
		return false;

	switchToCoroutine(in_pGarminUsbData->pMainCoroutine, in_pGarminUsbData->pGeolocationCoroutine);

	if (!isGarminCoroutineStateActive(in_pGarminUsbData->coroutineState))
		return false;
	else
	{
		if (in_ppPacket != NULL)
			*in_ppPacket = in_pGarminUsbData->pPacket;

		return true;
	}
}
