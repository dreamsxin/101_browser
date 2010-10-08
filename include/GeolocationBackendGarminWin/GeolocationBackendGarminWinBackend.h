#ifndef _GeolocationBackendGarminWinBackend_h
#define _GeolocationBackendGarminWinBackend_h

#include "GeolocationBackendGarminWin/GeolocationBackendGarminWinCommon.h"
#include <windows.h>
#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)
typedef struct
{
	uint8_t  mPacketType;
	uint8_t  mReserved1;
	uint16_t mReserved2;
	uint16_t mPacketId;
	uint16_t mReserved3;
	uint32_t mDataSize;
	uint8_t  mData[1];
} Packet_t;
#pragma pack(pop)

enum
{
	PacketType_USB_Protocol_Layer = 0,
	PacketType_Application_Layer = 20
};

enum
{
	Pid_Data_Available = 2,
	Pid_Start_Session = 5,
	Pid_Session_Started = 6 
};

enum 
{ 
	Pid_Protocol_Array = 253, /* may not be implemented in all devices */
	Pid_Product_Rqst = 254,
	Pid_Product_Data = 255,
	Pid_Ext_Product_Data = 248 /* may not be implemented in all devices */
};

#pragma pack(push, 1)
typedef struct
{
	uint8_t  tag;
	uint16_t data;
} Protocol_Data_Type;
#pragma pack(pop)

typedef Protocol_Data_Type Protocol_Array_Type[];

#ifdef _WIN32
__declspec(dllexport)
#endif
void freePacket(Packet_t **in_ppPacket);

#ifdef _WIN32
__declspec(dllexport)
#endif
void sendPacket(HANDLE in_garminHandle, Packet_t in_packet, WORD in_usbPacketSize);


#ifdef _WIN32
__declspec(dllexport)
#endif
ReceivePacketResult receivePacket(ReceivePacketState *in_pState, 
								  HANDLE in_garminHandle,
								  Packet_t** out_ppPacket);

/*!
 * Either returns NULL (in case of error) or a received packet with
 * mPacketType == in_type and
 * mPacketId == in_pid
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
ReceivePacketResult waitForPacket(ReceivePacketState *in_pState, 
								  HANDLE in_garminHandle, 
								  Packet_t** out_ppPacket,
								  unsigned char in_type,
								  unsigned short in_pid);

#ifdef __cplusplus
}
#endif

#endif
