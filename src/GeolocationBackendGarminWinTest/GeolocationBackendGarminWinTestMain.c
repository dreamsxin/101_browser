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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "GeolocationBackendGarminWin/GeolocationGarmin.h"
#include "GeolocationBackendGarminWin/GeolocationGarminFunctions.h"

int main()
{
	WORD lUsbSize;
	HANDLE lGarminHandle;

	CoroutineDescriptor currentCoroutine, geolocationCoroutine;

	Packet_t theStartSessionPacket = {
		PacketType_USB_Protocol_Layer,
		0, 0, // reserved fields
		Pid_Start_Session,
		0,    // data size
		0     // data
	};

	Packet_t theProductDataPacket = {
		PacketType_Application_Layer, 
		0, 0, // reserved fields
		Pid_Product_Rqst, 
		0,    // data size
		0     // data
	};
	GarminUsbData garminUsbData;

	lGarminHandle = initializeGeolocationBackendGarmin(&lUsbSize);

	if (!lGarminHandle)
	{
		printf("Could not find Garmin device. Exiting.\n");
		exit(EXIT_FAILURE);
	}

	if (!convertThreadToCoroutine(&currentCoroutine))
	{
		printf("Could not convert current thread to coroutine.\n");
		exit(EXIT_FAILURE);
	}

	if (!createGeolocationCoroutine(&garminUsbData, &currentCoroutine, &geolocationCoroutine, 
		lGarminHandle, NULL, NULL))
	{
		printf("Could not create Geolocation coroutine.\n");
		exit(EXIT_FAILURE);
	}

	sendPacket(lGarminHandle, &theStartSessionPacket, lUsbSize);

	if (!waitForPacket(&garminUsbData, PacketType_USB_Protocol_Layer, Pid_Session_Started))
	{
		printf("Could not start session.\n");
		exit(EXIT_FAILURE);
	}

	sendPacket(lGarminHandle, &theProductDataPacket, lUsbSize);
	if (!waitForPacket(&garminUsbData, PacketType_Application_Layer, Pid_Product_Data))
	{
		printf("Could not get product data.\n");
		exit(EXIT_FAILURE);
	}

	/*while (ReceivePacketResultPacketContinueRead == receivePacketResult)
	{
		printf("Type: %u\tId: %u\n", (unsigned) thePacket->mPacketType, 
		(unsigned) thePacket->mPacketId);

		if (thePacket->mPacketType == PacketType_Application_Layer &&
			thePacket->mPacketId == Pid_Protocol_Array)
		{
			size_t idx;

			for (idx = 0; idx < thePacket->mDataSize/3; idx++)
			{
				Protocol_Data_Type *current_Protocol_Data_Type = 
					((Protocol_Data_Type*) thePacket->mData)+idx;

				printf("%c%u\n", current_Protocol_Data_Type->tag, 
					(unsigned) current_Protocol_Data_Type->data);
			}
		}

		receivePacketResult = receivePacket(&receivePacketState,
			lGarminHandle,
			&thePacket);

		if (isErrorResult(receivePacketResult))
		{
			fprintf(stderr, "An error occured.\n");
			exit(EXIT_FAILURE);
		}
	}*/
	
	return 0;
}
