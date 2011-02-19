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

#include "GeolocationBackendGarminWin/GeolocationGarmin.h"
#include "GeolocationBackendGarminWin/GeolocationGarminInternalMemory.h"

#include <assert.h>

#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/cstddef.h"
#include "MiniStdlib/MTAx_cstdlib.h"
#include "MiniStdlib/safe_free.h"

bool reallocBufferAndItsSizeAndDeleteIfFailure(void *in_ppTheBuffer, size_t in_size, 
									 size_t in_additionalDataSize)
{
	void* theNewBuffer = realloc(*(void**) in_ppTheBuffer, in_size+in_additionalDataSize);

	if (theNewBuffer == NULL)
	{
		safe_free(in_ppTheBuffer);
		return false;
	}
	else
	{
		*(void**) in_ppTheBuffer = theNewBuffer;
		return true;
	}
}

bool isPacketBufferLargeEnough(Packet_t* in_pPacket, size_t in_bufferSize)
{
	assert(in_pPacket != NULL);

	/*
	 * It is more secure to assume the worst possible thing in this
	 * case.
	 */
	if (in_pPacket == NULL)
		return false;

	return in_pPacket->mDataSize + offsetof(Packet_t, mData) <= in_bufferSize;
}
