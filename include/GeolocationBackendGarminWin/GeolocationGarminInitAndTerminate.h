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

#ifndef _MTAx_GeolocationGarminWin_GeolocationGarminInitAndTerminate_h
#define _MTAx_GeolocationGarminWin_GeolocationGarminInitAndTerminate_h

#include "GeolocationBackendGarminWin/GeolocationGarminPackets.h"
#include "GeolocationBackendGarminWin/GeolocationGarminCoroutineDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) HANDLE initializeGeolocationBackendGarmin(WORD *in_out_pUsbSize);

__declspec(dllexport) bool createGeolocationCoroutine(GarminUsbData *out_pGarminUsbData, 
	CoroutineDescriptor *in_pCurrentCoroutine, 
	CoroutineDescriptor *in_pGeolocationCoroutine,
	HANDLE in_garminHandle);

void COROUTINE_CALL geolocationCoroutine(void *in_pGarminUsbData);

void startGeolocationCoroutine(GarminUsbData *in_out_pGarminUsbData);

__declspec(dllexport) void stopGeolocationCoroutine(GarminUsbData *in_out_pGarminUsbData);

#ifdef __cplusplus
}
#endif

#endif
