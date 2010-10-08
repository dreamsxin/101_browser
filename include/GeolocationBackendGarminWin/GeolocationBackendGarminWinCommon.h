#ifndef _GeolocationBackendGarminWinCommon_h
#define _GeolocationBackendGarminWinCommon_h

typedef enum _ReceivePacketState
{
	ReceiveByDeviceIoControl,
	ReceiveByReadFile
} ReceivePacketState;


typedef enum _ReceivePacketResult
{
	ReceivePacketResultNoPacket,
	ReceivePacketResultPacket,
	/*
	 * ReceivePacketResultPacketContinueRead
	 * is returned if we have to continue reading
	 * before sending another packet
	 */
	ReceivePacketResultPacketContinueRead,
	ReceivePacketResultError
} ReceivePacketResult;

#endif
