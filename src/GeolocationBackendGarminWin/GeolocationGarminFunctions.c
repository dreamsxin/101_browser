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

#include "GeolocationBackendGarminWin/GeolocationGarminFunctions.h"

typedef struct
{
	uint8_t type;
	uint16_t pid;
	volatile bool hasReceived;
} WaitForPacketData;

void waitForPacketHandler(Packet_t *in_pPacket, void *in_pData)
{
	WaitForPacketData *waitForData = (WaitForPacketData *) in_pData;
	
	if (in_pPacket->mPacketType == waitForData->type &&
		in_pPacket->mPacketId == waitForData->pid)
	{
		waitForData->hasReceived = true;
	}
}

bool waitForPacket(
	GarminUsbData *in_out_pUsbData,
	uint8_t in_type,
	uint16_t in_pid)
{
	WaitForPacketData waitForPacketData = { in_type, in_pid, false };

	in_out_pUsbData->pPacketHandlerFunc = &waitForPacketHandler;
	in_out_pUsbData->pPacketHandlerData = &waitForPacketData;

	while (!waitForPacketData.hasReceived)
	{
		startGeolocationCoroutine(in_out_pUsbData);

		if (in_out_pUsbData->coroutineState != GarminCoroutineStateOK)
			return false;
	}

	return true;
}
