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

#ifndef _MTAx_IO_BidirectionalStream_h
#define _MTAx_IO_BidirectionalStream_h

#include "Coroutine/Coroutine.h"
#include "IO/CoroutineStream.h"
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/declspec.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	// This state is before the halfstream started reading or writing
	BidirectionalHalfStreamActionNoAction,
	BidirectionalHalfStreamActionReading,
	BidirectionalHalfStreamActionWriting,
	BidirectionalHalfStreamActionTerminated
} BidirectionalHalfStreamAction;

typedef struct
{
	CoroutineDescriptor *mpCoroutineDescriptor;
	volatile BidirectionalHalfStreamAction mAction;
} BidirectionalHalfStreamState;

typedef struct
{
	// this has to be the first member of the struct because it will be casted
	CoroutineStreamFunctions mFunctions;

	const uint8_t * volatile mpCurrentBuffer;
	volatile size_t mCurrentBufferSize;
	
	BidirectionalHalfStreamState mHalfStreamStates[2];
	uint8_t mCurrentSide;
} BidirectionalStreamState;

DLLEXPORT bool bidirectionalStreamIsReadingPossible(void *in_pBidirectionalStreamState);

DLLEXPORT bool bidirectionalStreamIsWritingPossible(void *in_pBidirectionalStreamState);

DLLEXPORT bool bidirectionalStreamInit(BidirectionalStreamState *out_pBidirectionalStreamState,
	ByteStreamInterface *out_pByteStreamInterface,
	bool in_startThisCoroutineAsWriter,
	CoroutineDescriptor *out_pThisCoroutine,
	CoroutineDescriptor *out_pOtherCoroutine,
	void (*in_pOtherCoroutineStartup)(ByteStreamReference in_byteStreamReference, void *in_pUserData),
	void *in_pUserData);

DLLEXPORT void bidirectionalStreamStateDestroy(BidirectionalStreamState *out_pBidirectionalStreamState);

DLLEXPORT size_t bidirectionalStreamRead(void *in_out_pBidirectionalStreamState, void *out_pBuffer, size_t in_count);

DLLEXPORT size_t bidirectionalStreamWrite(void *in_out_pBidirectionalStreamState, const void *in_pBuffer, size_t in_count);

#ifdef __cplusplus
}
#endif

#endif
