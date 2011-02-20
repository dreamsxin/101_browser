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

#ifndef _MTAx_GeolocationGarminWin_GeolocationGarminPacketsUtil_h
#define _MTAx_GeolocationGarminWin_GeolocationGarminPacketsUtil_h

#include "GeolocationBackendGarminWin/GeolocationGarminPackets.h"


#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) void fillEmptyPacket(Packet_t *in_pPacket, 
	uint8_t in_packetType, 
	uint16_t in_packetId);

__declspec(dllexport) void fillDeviceCommandPacket(Device_Command_Packet_t *in_pPacket, 
	Command_Id_Type in_command_Id_Type);

#ifdef __cplusplus
}
#endif

#endif
