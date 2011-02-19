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

#ifndef _MTAx_GeolocationGarminWin_GeolocationGarmin_h
#define _MTAx_GeolocationGarminWin_GeolocationGarmin_h

#include "GeolocationBackendGarminWin/GeolocationGarminPackets.h"

#include "Coroutine/Coroutine.h"

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	GarminCoroutineStateOK,
	GarminCoroutineStateErrorAllocation,
	GarminCoroutineStateErrorReadFile,
	GarminCoroutineStateErrorInvalidData
} GarminCoroutineState;

typedef struct
{
	CoroutineDescriptor *pMainCoroutine;
	CoroutineDescriptor *pGeolocationCoroutine;
	HANDLE garminHandle;
	volatile GarminCoroutineState coroutineState;

	void (* volatile pPacketHandlerFunc)(Packet_t *, void *);
	void * volatile pPacketHandlerData;
} GarminUsbData;

__declspec(dllexport) HANDLE initializeGeolocationBackendGarmin(WORD *in_out_pUsbSize);

__declspec(dllexport) bool createGeolocationCoroutine(GarminUsbData *out_pGarminUsbData, 
	CoroutineDescriptor *in_pCurrentCoroutine, 
	CoroutineDescriptor *in_pGeolocationCoroutine,
	HANDLE in_garminHandle, 
	void (*in_pPacketHandlerFunc)(Packet_t*, void*),
	void *in_pPacketHandlerData);

__declspec(dllexport) void startGeolocationCoroutine(GarminUsbData *in_out_pGarminUsbData);

void __stdcall geolocationCoroutine(void *in_pGarminUsbData);

__declspec(dllexport) void sendPacket(HANDLE in_garminHandle, Packet_t *in_pPacket, WORD in_usbPacketSize);

#ifdef __cplusplus
}
#endif

#endif
