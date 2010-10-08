#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "GeolocationBackendGarminWin/GeolocationBackendGarminWinFrontend.h"
#include "GeolocationBackendGarminWin/GeolocationBackendGarminWinBackend.h"

int main()
{
	WORD lUsbSize;
	HANDLE lGarminHandle = initializeGeolocationBackendGarmin(&lUsbSize);
	Packet_t theProductDataPacket = { PacketType_Application_Layer, 0, 0, Pid_Product_Rqst, 0 , 0 };
	Packet_t* thePacket;
	ReceivePacketState receivePacketState;
	ReceivePacketResult receivePacketResult;

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
		printf("%u %u\n", (unsigned) thePacket->mPacketType, 
		(unsigned) thePacket->mPacketId);

		freePacket(&thePacket);

		receivePacketResult = receivePacket(&receivePacketState,
			lGarminHandle,
			&thePacket);

		if (ReceivePacketResultError == receivePacketResult)
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
