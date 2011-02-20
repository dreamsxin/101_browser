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
	GarminUsbData *pUsbData;
	volatile bool hasReceived;
	bool setNextHandler;
	void (* pNextHandlerFunc)(Packet_t *, void *);
	void * volatile pNextHandlerData;
} WaitForPacketData;

void waitForPacketHandler(Packet_t *in_pPacket, void *in_pData)
{
	WaitForPacketData *waitForData = (WaitForPacketData *) in_pData;
	
	if (in_pPacket->mPacketType == waitForData->type &&
		in_pPacket->mPacketId == waitForData->pid)
	{
		waitForData->hasReceived = true;

		if (waitForData->setNextHandler)
		{
			waitForData->pUsbData->pPacketHandlerFunc = waitForData->pNextHandlerFunc;
			waitForData->pUsbData->pPacketHandlerData = waitForData->pNextHandlerData;
		}
	}
}

bool waitForPacket(
	GarminUsbData *in_out_pUsbData,
	uint8_t in_type,
	uint16_t in_pid,
	bool in_setNextHandler,
	void (*in_pNextHandlerFunc)(Packet_t *, void *),
	void * volatile in_pNextHandlerData)
{
	WaitForPacketData waitForPacketData = { in_type, in_pid, in_out_pUsbData, 
		false, in_setNextHandler, in_pNextHandlerFunc, in_pNextHandlerData };

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
