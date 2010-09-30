#ifndef _Garmin_h
#define _Garmin_h

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
#pragma pack( pop)

#ifdef __cplusplus
}
#endif

#endif
