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

#ifndef _MTAx_GeolocationGarminWin_GeolocationGarminPacketFunctions_h
#define _MTAx_GeolocationGarminWin_GeolocationGarminPacketFunctions_h

#include "GeolocationBackendGarminWin/GeolocationGarminCoroutineStateFunctions.h"

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) bool sendPacket(GarminUsbData *in_pGarminUsbData, void *in_pPacket, 
	WORD in_usbPacketSize);

__declspec(dllexport) bool receivePacket(GarminUsbData *in_pGarminUsbData, Packet_t **in_ppPacket);

#ifdef __cplusplus
}
#endif

#endif

