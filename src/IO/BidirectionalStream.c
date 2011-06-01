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

#include "MiniStdlib/minmax.h"
#include "IO/BidirectionalStream.h"
#include "IO/CoroutineStream.h"
#include <assert.h>
#include <string.h> // for memcpy

void invertAndSwitchCoroutine(void *in_out_pBidirectionalStreamState)
{
	BidirectionalStreamState *pBidirectionalStreamState = (BidirectionalStreamState *) in_out_pBidirectionalStreamState;

	if (0 != pBidirectionalStreamState->mCurrentSide && 1 != pBidirectionalStreamState->mCurrentSide)
		return;
	
	pBidirectionalStreamState->mCurrentSide = 1-pBidirectionalStreamState->mCurrentSide;
	/*
	* Note that we just inverted in_out_pStreamState->mCurrentSide; so
	* 1-in_out_pStreamState->mCurrentSide is the index of the current coroutine.
	*/
	switchToCoroutine(pBidirectionalStreamState->mHalfStreamStates[1-pBidirectionalStreamState->mCurrentSide].mpCoroutineDescriptor,
		pBidirectionalStreamState->mHalfStreamStates[pBidirectionalStreamState->mCurrentSide].mpCoroutineDescriptor);
}

void bidirectionalStreamTerminalLoop(void *in_out_pBidirectionalStreamState)
{
	BidirectionalStreamState *pBidirectionalStreamState = (BidirectionalStreamState *) in_out_pBidirectionalStreamState;
	
	if (0 != pBidirectionalStreamState->mCurrentSide && 1 != pBidirectionalStreamState->mCurrentSide)
		return;

	assert(BidirectionalHalfStreamActionNoAction == 
		pBidirectionalStreamState->mHalfStreamStates[pBidirectionalStreamState->mCurrentSide].mAction);

	/*
	* Let's assure that in all cases this function works correctly:
	*
	* Other coroutine calling
	* read(0): 
	* read(in_count) (in_count > 0): 
	* write(0):
	* write(in_count) (in_count > 0): 
	*/
	if (bidirectionalStreamIsWritingPossible(in_out_pBidirectionalStreamState))
	{
		bidirectionalStreamWrite(in_out_pBidirectionalStreamState, NULL, 0);
	}
	else
	{
		bidirectionalStreamRead(in_out_pBidirectionalStreamState, NULL, 0);
	}
}

bool bidirectionalStreamIsReadingPossible(void *in_pBidirectionalStreamState)
{
	BidirectionalStreamState *pStreamState = (BidirectionalStreamState*) in_pBidirectionalStreamState;

	if (0 != pStreamState->mCurrentSide && 1 != pStreamState->mCurrentSide)
		return false;

	return (BidirectionalHalfStreamActionReading != pStreamState->mHalfStreamStates[1- pStreamState->mCurrentSide].mAction);
}

bool bidirectionalStreamIsWritingPossible(void *in_pBidirectionalStreamState)
{
	BidirectionalStreamState *pStreamState = (BidirectionalStreamState*) in_pBidirectionalStreamState;

	if (0 != pStreamState->mCurrentSide && 1 != pStreamState->mCurrentSide)
		return false;

	return (BidirectionalHalfStreamActionWriting != pStreamState->mHalfStreamStates[1- pStreamState->mCurrentSide].mAction);
}

bool bidirectionalStreamInit(BidirectionalStreamState *out_pBidirectionalStreamState,
	ByteStreamInterface *out_pByteStreamInterface,
	CoroutineDescriptor *out_pThisCoroutine,
	CoroutineDescriptor *out_pOtherCoroutine,
	void (*in_pOtherCoroutineStartup)(ByteStreamReference in_byteStreamReference, void *in_pUserData),
	void *in_pUserData)
{
	uint8_t idx;
	ByteStreamReference byteStreamReference;
	byteStreamReference.mpByteStreamState = out_pBidirectionalStreamState;
	memset(&byteStreamReference.mByteStreamInterface, 0, sizeof(byteStreamReference.mByteStreamInterface));
	byteStreamReference.mByteStreamInterface.mpfRead = bidirectionalStreamRead;
	byteStreamReference.mByteStreamInterface.mpfWrite = bidirectionalStreamWrite;
	
	out_pBidirectionalStreamState->mFunctions.mpfSwitchCoroutine = invertAndSwitchCoroutine;

	out_pBidirectionalStreamState->mpCurrentBuffer = NULL;
	out_pBidirectionalStreamState->mCurrentBufferSize = 0;

	out_pBidirectionalStreamState->mHalfStreamStates[0].mpCoroutineDescriptor = out_pThisCoroutine;
	out_pBidirectionalStreamState->mHalfStreamStates[1].mpCoroutineDescriptor = out_pOtherCoroutine;

	for (idx = 0; idx < 2; idx++)
	{
		out_pBidirectionalStreamState->mHalfStreamStates[idx].mAction = BidirectionalHalfStreamActionNoAction;
	}

	out_pBidirectionalStreamState->mCurrentSide = 0;

	return coroutineStreamStart(byteStreamReference,
		out_pThisCoroutine,
		out_pOtherCoroutine,
		in_pOtherCoroutineStartup,
		NULL,
		bidirectionalStreamTerminalLoop,
		in_pUserData);
}

size_t bidirectionalStreamRead(void *in_out_pBidirectionalStreamState, void *out_pBuffer, size_t in_count)
{
	BidirectionalStreamState *pStreamState = (BidirectionalStreamState*) in_out_pBidirectionalStreamState;
	size_t out_bytesRead;
	
	if (0 != pStreamState->mCurrentSide && 1 != pStreamState->mCurrentSide)
		// Error
		return SIZE_MAX;
	
	if (!bidirectionalStreamIsReadingPossible(in_out_pBidirectionalStreamState))
		// Error
		return SIZE_MAX;

	assert(BidirectionalHalfStreamActionNoAction == pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mAction);

	/*
	* Q: Why do we only set this variable in case of 0 != in_count?
	* A: If we have 0 == in_count, coroutineStreamRead will simply switch the coroutine.
	*    With
	*    pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mAction == BidirectionalHalfStreamActionReading
	*    any reading attempt of the other coroutine would be blocked. 
	*    But enabling the other coroutine is exactly the purpose of calling 
	*    bidirectionalStreamRead with 0 == in_count.
	*
	*    Note that in case of pStreamState->mCurrentBufferSize (where we require
	*    *not* to switch the coroutine) this causes no harm.
	*/
	if (0 != in_count)
	{
		pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mAction = BidirectionalHalfStreamActionReading;
	}

	out_bytesRead = coroutineStreamRead(in_out_pBidirectionalStreamState, 
		out_pBuffer, in_count, 
		&pStreamState->mpCurrentBuffer,
		&pStreamState->mCurrentBufferSize);

	if (0 != in_count)
	{
		pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mAction = BidirectionalHalfStreamActionNoAction;
	}

	return out_bytesRead;
}

size_t bidirectionalStreamWrite(void *in_out_pBidirectionalStreamState, const void *in_pBuffer, size_t in_count)
{
	BidirectionalStreamState *pStreamState = (BidirectionalStreamState*) in_out_pBidirectionalStreamState;
	size_t out_bytesWritten;

	assert(0 == pStreamState->mCurrentSide || 1 == pStreamState->mCurrentSide);
	
	if (0 != pStreamState->mCurrentSide && 1 != pStreamState->mCurrentSide)
		// Error
		return SIZE_MAX;
	
	if (!bidirectionalStreamIsWritingPossible(in_out_pBidirectionalStreamState))
		// Error
		return SIZE_MAX;

	/*
	* See comment in bidirectionalStreamRead concerning this code block.
	*/
	if (0 != in_count)
	{
		pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mAction = BidirectionalHalfStreamActionWriting;
	}

	/*
	* Q: Why do we set the last parameter to true to reset the buffer to NULL?
	* A: If we would call read(in_count) with in_count > 0 we would try to access a buffer
	*    that already could have become invalid.
	*/
	out_bytesWritten = coroutineStreamWrite(pStreamState, in_pBuffer, in_count, 
		&pStreamState->mpCurrentBuffer,
		&pStreamState->mCurrentBufferSize,
		true);

	if (0 != in_count)
	{
		pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mAction = BidirectionalHalfStreamActionNoAction;
	}

	return out_bytesWritten;
}
