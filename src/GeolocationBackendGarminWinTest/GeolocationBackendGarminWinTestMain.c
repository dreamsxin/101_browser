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
#define _USE_MATH_DEFINES
#include <math.h>

#include "GeolocationBackendGarminWin/GeolocationGarmin.h"
#include "GeolocationBackendGarminWin/GeolocationGarminFunctions.h"
#include "GeolocationBackendGarminWin/GeolocationGarminPacketsUtil.h"

typedef struct
{
	volatile bool A010_supported;
	volatile bool A800_supported;
	volatile bool D800_supported;
} SupportedProtocols;

void interprete_Protocol_Array(Packet_t *in_pPacket, void *in_pSupportedProtocols)
{
	SupportedProtocols *pSupportedProtocols = (SupportedProtocols *) in_pSupportedProtocols;

#if 0
	printf("%u %u\n", in_pPacket->mPacketType, in_pPacket->mPacketId);
#endif

	if (in_pPacket->mPacketType == PacketType_Application_Layer &&
		in_pPacket->mPacketId == Pid_Protocol_Array)
	{
		size_t idx;

		for (idx = 0; idx < in_pPacket->mDataSize/3; idx++)
		{
			Protocol_Data_Type *current_Protocol_Data_Type = 
				((Protocol_Data_Type*) in_pPacket->mData)+idx;

			if ('A' == current_Protocol_Data_Type->tag)
			{
				if (10 == current_Protocol_Data_Type->data)
					pSupportedProtocols->A010_supported = true;
				else if (800 == current_Protocol_Data_Type->data)
					pSupportedProtocols->A800_supported = true;
			}
			else if ('D' == current_Protocol_Data_Type->tag)
			{
				if (800 == current_Protocol_Data_Type->data)
					pSupportedProtocols->D800_supported = true;
			}

			printf("%c%03u\n", current_Protocol_Data_Type->tag, 
				(unsigned) current_Protocol_Data_Type->data);
		}
	}
}

void pvtHandler(Packet_t *in_pPacket, void *in_pData)
{
	if (PacketType_Application_Layer == in_pPacket->mPacketType && 
		Pid_Pvt_Data == in_pPacket->mPacketId)
	{
		if (sizeof(D800_Pvt_Data_Type) == in_pPacket->mDataSize)
		{
			D800_Pvt_Data_Type *pPvtData = (D800_Pvt_Data_Type*) in_pPacket->mData;
			printf("%f %f\n", pPvtData->posn.lon * 180.0/M_PI, pPvtData->posn.lat * 180.0/M_PI);
		}
	}
}

int main()
{
	WORD lUsbSize;
	HANDLE lGarminHandle;

	CoroutineDescriptor currentCoroutine, geolocationCoroutine;

	Packet_t theStartSessionPacket;
	Packet_t theProductDataPacket;
	Device_Command_Packet_t theStartPvtDataPacket;
	Device_Command_Packet_t theStopPvtDataPacket;

	GarminUsbData garminUsbData;
	SupportedProtocols supportedProcotocols = { false, false, false };

	fillEmptyPacket(&theStartSessionPacket, PacketType_USB_Protocol_Layer, Pid_Start_Session);
	fillEmptyPacket(&theProductDataPacket, PacketType_Application_Layer, Pid_Product_Rqst);
	fillDeviceCommandPacket(&theStartPvtDataPacket, Cmnd_Start_Pvt_Data);
	fillDeviceCommandPacket(&theStopPvtDataPacket, Cmnd_Stop_Pvt_Data);

	lGarminHandle = initializeGeolocationBackendGarmin(&lUsbSize);

	if (!lGarminHandle)
	{
		printf("Could not find Garmin device.\n");
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

	if (!sendPacket(lGarminHandle, &theStartSessionPacket, lUsbSize))
	{
		printf("Could not send start session packet.\n");
		exit(EXIT_FAILURE);
	}

	if (!waitForPacket(&garminUsbData, PacketType_USB_Protocol_Layer, Pid_Session_Started, false, NULL, NULL))
	{
		printf("Could not start session.\n");
		exit(EXIT_FAILURE);
	}

	if (!sendPacket(lGarminHandle, &theProductDataPacket, lUsbSize))
	{
		printf("Could not send product data packet.\n");
		exit(EXIT_FAILURE);
	}

	if (!waitForPacket(&garminUsbData, PacketType_Application_Layer, Pid_Product_Data, 
		true, &interprete_Protocol_Array, &supportedProcotocols))
	{
		printf("Could not get product data.\n");
		exit(EXIT_FAILURE);
	}

	if (!supportedProcotocols.A010_supported)
	{
		printf("Protocol A010 is not supported.\n");
		exit(EXIT_FAILURE);
	}

	if (!supportedProcotocols.A800_supported)
	{
		printf("Protocol A800 is not supported.\n");
		exit(EXIT_FAILURE);
	}

	if (!supportedProcotocols.D800_supported)
	{
		printf("Protocol D800 is not supported.\n");
		exit(EXIT_FAILURE);
	}

	garminUsbData.pPacketHandlerFunc = &pvtHandler;
	garminUsbData.pPacketHandlerData = NULL;

	if (!sendPacket(lGarminHandle, &theStartPvtDataPacket, lUsbSize))
	{
		printf("Could not send start PVT packet.\n");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		startGeolocationCoroutine(&garminUsbData);

		if (garminUsbData.coroutineState != GarminCoroutineStateOK)
		{
			printf("Invalid state.\n");
			exit(1);
		}
	}

	return 0;
}
