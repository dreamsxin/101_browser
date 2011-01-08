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

#ifndef _GeolocationBackendGarminWinFrontend_h
#define _GeolocationBackendGarminWinFrontend_h

#include "GeolocationBackendGarminWin/GeolocationBackendGarminWinCommon.h"
#include <windows.h>
#include "MiniStdlib/cstdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllexport)
#endif
HANDLE initializeGeolocationBackendGarmin(WORD *in_out_pUsbSize);

#ifdef _WIN32
__declspec(dllexport)
#endif
ReceivePacketState getInitialReceivePacketState();

#ifdef _WIN32
__declspec(dllexport)
#endif
bool startSession(ReceivePacketState *in_pState,
				  HANDLE in_garminHandle,
				  WORD in_usbPacketSize);

#ifdef __cplusplus
}
#endif

#endif
