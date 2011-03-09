/*
 * Copyright 2011 Wolfgang Keller
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

#ifndef _MTAx_GeolocationGarminWin_GeolocationGarminCoroutineDataTypes_h
#define _MTAx_GeolocationGarminWin_GeolocationGarminCoroutineDataTypes_h

#include "GeolocationBackendGarminWin/GeolocationGarminPackets.h" // for Packet_t

#include "Coroutine/Coroutine.h"

#include <windows.h>

typedef enum
{
	GarminCoroutineStateOkSendingPossible,
	GarminCoroutineStateOkSendingNotPossible,

	GarminCoroutineStateErrorAllocation,
	GarminCoroutineStateErrorDeviceIoControl,
	GarminCoroutineStateErrorReadFile,
	GarminCoroutineStateErrorInvalidData
} GarminCoroutineState;

typedef struct
{
	CoroutineDescriptor *pMainCoroutine;
	CoroutineDescriptor *pGeolocationCoroutine;
	HANDLE garminHandle;
	volatile GarminCoroutineState coroutineState;
	Packet_t * volatile pPacket;
} GarminUsbData;

#endif
