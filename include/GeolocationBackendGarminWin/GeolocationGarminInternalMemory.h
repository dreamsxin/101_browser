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

#ifndef _MTAx_GeolocationGarminWin_GeolocationGarminInternalMemory_h
#define _MTAx_GeolocationGarminWin_GeolocationGarminInternalMemory_h

#include "GeolocationBackendGarminWin/GeolocationGarminPackets.h"

#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/cstddef.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Return value: true on success
 *               false on failure
 */
bool reallocBufferAndItsSizeAndDeleteIfFailure(void *in_ppTheBuffer,
	size_t in_size, size_t in_additionalDataSize);

bool isPacketBufferOfCorrectSize(Packet_t* in_pPacket, size_t in_bufferSize);

#ifdef __cplusplus
}
#endif

#endif
