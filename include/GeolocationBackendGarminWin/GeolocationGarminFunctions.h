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

#ifndef _MTAx_GeolocationGarminWin_GeolocationGarminFunctions_h
#define _MTAx_GeolocationGarminWin_GeolocationGarminFunctions_h

#include "GeolocationBackendGarminWin/GeolocationGarmin.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Return value:
 *
 * true - success
 * false - error
 */
__declspec(dllexport) bool waitForPacket(
	GarminUsbData *in_out_pUsbData,
	uint8_t in_type,
	uint16_t in_pid,
	bool in_setNextHandler,
	void (*in_pNextHandlerFunc)(Packet_t *, void *),
	void * volatile in_pNextHandlerData);

#ifdef __cplusplus
}
#endif

#endif
