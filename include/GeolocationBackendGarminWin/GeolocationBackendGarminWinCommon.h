#ifndef _GeolocationBackendGarminWinCommon_h
#define _GeolocationBackendGarminWinCommon_h

typedef enum
{
	ReceiveByDeviceIoControl,
	ReceiveByReadFile
} ReceivePacketState;


typedef enum
{
	/*
	 * ReceivePacketResultNoPacket is returned when no additional packet is received. 
	 * This means as a corollary that we can start sending packages.
	 */
	ReceivePacketResultNoPacket,
	/*
	 * ReceivePacketResultPacket is returned when a packet is received after which we 
	 * can send packages.
	 */
	ReceivePacketResultPacket,
	/*
	 * ReceivePacketResultPacketContinueRead
	 * is returned if we have to continue reading
	 * before sending another packet
	 */
	ReceivePacketResultPacketContinueRead,

	ReceivePacketResultErrorAllocation,
	ReceivePacketResultErrorReadFile,
	ReceivePacketResultErrorInvalidData,
	ReceivePacketResultErrorInvalidState
} ReceivePacketResult;

#endif
