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

#include "GeolocationBackendGarminWin/GeolocationGarminInit.h"
#include "GeolocationBackendGarminWin/GeolocationGarminCoroutineStateFunctions.h"
#include "GeolocationBackendGarminWin/GeolocationGarminPacketFunctions.h"
#include "GeolocationBackendGarminWin/GeolocationGarminPacketsUtil.h"

typedef struct
{
	volatile bool A010_supported;
	volatile bool A800_supported;
	volatile bool D800_supported;
} SupportedProtocols;

void interprete_Protocol_Array(Packet_t *in_pPacket, SupportedProtocols *in_pSupportedProtocols)
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
			printf("Position: %f %f (+/- %f m)\n", pPvtData->posn.lat * 180.0/M_PI, pPvtData->posn.lon * 180.0/M_PI, pPvtData->eph);
			printf("Height: %f (+/- %f m)\n", pPvtData->alt+pPvtData->msl_hght, pPvtData->epv);

			printf("\n");
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
	Packet_t *pReceivedPacket;

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
		lGarminHandle))
	{
		printf("Could not create Geolocation coroutine.\n");
		exit(EXIT_FAILURE);
	}

	if (!sendPacket(&garminUsbData, &theStartSessionPacket, lUsbSize))
	{
		printf("Could not send start session packet.\n");
		exit(EXIT_FAILURE);
	}

	do
	{
		if (!receivePacket(&garminUsbData, &pReceivedPacket))
		{
			printf("Could not receive packet.\n");
			exit(EXIT_FAILURE);
		}
	} while (
		NULL == pReceivedPacket ||
		pReceivedPacket->mPacketType != PacketType_USB_Protocol_Layer || 
		pReceivedPacket->mPacketId != Pid_Session_Started);

	do
	{
		if (isSendingPossible(garminUsbData.coroutineState))
			break;

		if (!receivePacket(&garminUsbData, &pReceivedPacket))
		{
			printf("Could not receive packet.\n");
			exit(EXIT_FAILURE);
		}
	} while (1);

	if (!sendPacket(&garminUsbData, &theProductDataPacket, lUsbSize))
	{
		printf("Could not send product data packet.\n");
		exit(EXIT_FAILURE);
	}

	do
	{
		if (!receivePacket(&garminUsbData, &pReceivedPacket))
		{
			printf("Could not receive packet.\n");
			exit(EXIT_FAILURE);
		}
	} while (
		NULL == pReceivedPacket ||
		pReceivedPacket->mPacketType != PacketType_Application_Layer || 
		pReceivedPacket->mPacketId != Pid_Product_Data);

	do
	{
		if (isSendingPossible(garminUsbData.coroutineState))
			break;

		if (!receivePacket(&garminUsbData, &pReceivedPacket))
		{
			printf("Could not receive packet.\n");
			exit(EXIT_FAILURE);
		}
	} while (1);

	if (!sendPacket(&garminUsbData, &theProductDataPacket, lUsbSize))
	{
		printf("Could not send product data packet.\n");
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

#if 0
#if 0
	garminUsbData.pPacketHandlerFunc = &pvtHandler;
	garminUsbData.pPacketHandlerData = NULL;
#endif

	if (!sendPacket(&garminUsbData, &theStartPvtDataPacket, lUsbSize))
	{
		printf("Could not send start PVT packet.\n");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		startGeolocationCoroutine(&garminUsbData);

		if (!isGarminCoroutineStateOK(garminUsbData.coroutineState))
		{
			printf("Invalid state.\n");
			exit(1);
		}
	}
#endif

	return 0;
}
