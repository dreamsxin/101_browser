#ifndef _GeolocationBackendGarminWinBackend_h
#define _GeolocationBackendGarminWinBackend_h

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack( push, 1)
typedef struct
{
	unsigned char  mPacketType;
	unsigned char  mReserved1;
	unsigned short mReserved2;
	unsigned short mPacketId;
	unsigned short mReserved3;
	unsigned long  mDataSize;
	BYTE           mData[1];
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

/*!
 * Frees the packet and sets the pointer to NULL
 */
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
Packet_t* receivePacket(HANDLE in_garminHandle);

/*!
 * Either returns NULL (in case of error) or a received packet with
 * mPacketType == in_type and
 * mPacketId == in_pid
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
Packet_t* waitForPacket(HANDLE in_garminHandle, unsigned char in_type, unsigned short in_pid);

#ifdef __cplusplus
}
#endif

#endif
