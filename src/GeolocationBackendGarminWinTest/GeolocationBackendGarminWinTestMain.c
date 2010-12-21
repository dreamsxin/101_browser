#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "GeolocationBackendGarminWin/GeolocationBackendGarminWinFrontend.h"
#include "GeolocationBackendGarminWin/GeolocationBackendGarminWinBackend.h"

int main()
{
	WORD lUsbSize;
	HANDLE lGarminHandle;
	Packet_t theProductDataPacket = { PacketType_Application_Layer, 0, 0, Pid_Product_Rqst, 0 , 0 };
	Packet_t* thePacket;
	ReceivePacketState receivePacketState;
	ReceivePacketResult receivePacketResult;

	lGarminHandle = initializeGeolocationBackendGarmin(&lUsbSize);

	if (!lGarminHandle)
	{
		printf("Could not find Garmin device. Exiting.\n");
		return 0;
	}

	receivePacketState = getInitialReceivePacketState();

	startSession(&receivePacketState, lGarminHandle, lUsbSize);

	sendPacket(lGarminHandle, theProductDataPacket, lUsbSize);

	receivePacketResult = waitForPacket(&receivePacketState, 
		lGarminHandle,
		&thePacket,
		PacketType_Application_Layer,
		Pid_Product_Data);

	while (ReceivePacketResultPacketContinueRead == receivePacketResult)
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

		freePacket(&thePacket);

		receivePacketResult = receivePacket(&receivePacketState,
			lGarminHandle,
			&thePacket);

		if (isErrorResult(receivePacketResult))
		{
			fprintf(stderr, "An error occured.\n");
			exit(EXIT_FAILURE);
		}
	}
	
	assert(thePacket == NULL);
	// Only to be sure...
	freePacket(&thePacket);
	
	return 0;
}
