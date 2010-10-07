#include "stdio.h"
#include "stdlib.h"
#include "GeolocationBackendGarminWin/GeolocationBackendGarminWinFrontend.h"
#include "GeolocationBackendGarminWin/GeolocationBackendGarminWinBackend.h"

int main()
{
	WORD lUsbSize;

	HANDLE lGarminHandle = initializeGeolocationBackendGarmin(&lUsbSize);
	Packet_t theProductDataPacket = { PacketType_Application_Layer, 0, 0, Pid_Product_Rqst, 0 , 0 };
	Packet_t* thePacket;

	if (!lGarminHandle)
	{
		printf("Could not find Garmin device. Exiting.\n");
		return 0;
	}

	startSession(lGarminHandle, lUsbSize);

	sendPacket(lGarminHandle, theProductDataPacket, lUsbSize);

	thePacket = waitForPacket(lGarminHandle, PacketType_Application_Layer, Pid_Product_Data);
	if (thePacket == NULL)
	{
		fprintf(stderr, "Memory allocation failure.\n");
		exit(EXIT_FAILURE);
	}
	printf("%s\n", (char*) &thePacket->mData[4]);
	freePacket(&thePacket);
	
	return 0;
}
