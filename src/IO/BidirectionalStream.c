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

void invertAndSwitchCoroutine(void *in_out_pBidirectionalStream)
{
	BidirectionalStream *pBidirectionalStream = (BidirectionalStream *) in_out_pBidirectionalStream;

	if (0 != pBidirectionalStream->mCurrentSide && 1 != pBidirectionalStream->mCurrentSide)
		return;
	
	pBidirectionalStream->mCurrentSide = 1-pBidirectionalStream->mCurrentSide;
	/*
	* Note that we just inverted in_out_pStreamState->mCurrentSide; so
	* 1-in_out_pStreamState->mCurrentSide is the index of the current coroutine.
	*/
	switchToCoroutine(pBidirectionalStream->mHalfStreamStates[1-pBidirectionalStream->mCurrentSide].mpCoroutineDescriptor,
		pBidirectionalStream->mHalfStreamStates[pBidirectionalStream->mCurrentSide].mpCoroutineDescriptor);
}

void bidirectionalStreamTerminalLoop(void *in_out_pBidirectionalStream)
{
	BidirectionalStream *pBidirectionalStream = (BidirectionalStream *) in_out_pBidirectionalStream;
	
	if (0 != pBidirectionalStream->mCurrentSide && 1 != pBidirectionalStream->mCurrentSide)
		return;

	assert(BidirectionalHalfStreamActionNoAction == 
		pBidirectionalStream->mHalfStreamStates[pBidirectionalStream->mCurrentSide].mAction);

	/*
	* Let's assure that in all cases this function works correctly:
	*
	* Other coroutine calling
	* read(0): 
	* read(in_count) (in_count > 0): 
	* write(0):
	* write(in_count) (in_count > 0): 
	*/
	if (bidirectionalStreamIsWritingPossible(in_out_pBidirectionalStream))
	{
		bidirectionalStreamWrite(in_out_pBidirectionalStream, NULL, 0);
	}
	else
	{
		bidirectionalStreamRead(in_out_pBidirectionalStream, NULL, 0);
	}
}

bool bidirectionalStreamIsReadingPossible(void *in_pBidirectionalStream)
{
	BidirectionalStream *pBidirectionalStream = (BidirectionalStream*) in_pBidirectionalStream;

	if (0 != pBidirectionalStream->mCurrentSide && 1 != pBidirectionalStream->mCurrentSide)
		return false;

	return (BidirectionalHalfStreamActionReading != 
		pBidirectionalStream->mHalfStreamStates[1- pBidirectionalStream->mCurrentSide].mAction);
}

bool bidirectionalStreamIsWritingPossible(void *in_pBidirectionalStream)
{
	BidirectionalStream *pStreamState = (BidirectionalStream*) in_pBidirectionalStream;

	if (0 != pStreamState->mCurrentSide && 1 != pStreamState->mCurrentSide)
		return false;

	return (BidirectionalHalfStreamActionWriting != pStreamState->mHalfStreamStates[1- pStreamState->mCurrentSide].mAction);
}

bool bidirectionalStreamInit(BidirectionalStream *out_pBidirectionalStream,
	CoroutineDescriptor *out_pThisCoroutine,
	CoroutineDescriptor *out_pOtherCoroutine,
	void (*in_pOtherCoroutineStartup)(void *in_pStreamState, void *in_pUserData),
	void *in_pUserData)
{
	uint8_t idx;
	
	memset(&out_pBidirectionalStream->mCoroutineStreamInterface.mByteStreamInterface, 0, 
		sizeof(out_pBidirectionalStream->mCoroutineStreamInterface.mByteStreamInterface));
	out_pBidirectionalStream->mCoroutineStreamInterface.mByteStreamInterface.mpfRead = bidirectionalStreamRead;
	out_pBidirectionalStream->mCoroutineStreamInterface.mByteStreamInterface.mpfWrite = bidirectionalStreamWrite;
	out_pBidirectionalStream->mCoroutineStreamInterface.mpfSwitchCoroutine = invertAndSwitchCoroutine;

	out_pBidirectionalStream->mpCurrentBuffer = NULL;
	out_pBidirectionalStream->mCurrentBufferSize = 0;

	out_pBidirectionalStream->mHalfStreamStates[0].mpCoroutineDescriptor = out_pThisCoroutine;
	out_pBidirectionalStream->mHalfStreamStates[1].mpCoroutineDescriptor = out_pOtherCoroutine;

	for (idx = 0; idx < 2; idx++)
	{
		out_pBidirectionalStream->mHalfStreamStates[idx].mAction = BidirectionalHalfStreamActionNoAction;
	}

	out_pBidirectionalStream->mCurrentSide = 0;

	return coroutineStreamStart(out_pBidirectionalStream,
		out_pThisCoroutine,
		out_pOtherCoroutine,
		in_pOtherCoroutineStartup,
		NULL,
		bidirectionalStreamTerminalLoop,
		in_pUserData);
}

size_t bidirectionalStreamRead(void *in_out_pBidirectionalStream, void *out_pBuffer, size_t in_count)
{
	BidirectionalStream *pStreamStateBidirectionalStream = (BidirectionalStream*) in_out_pBidirectionalStream;
	size_t out_bytesRead;
	
	if (0 != pStreamStateBidirectionalStream->mCurrentSide && 
		1 != pStreamStateBidirectionalStream->mCurrentSide)
		// Error
		return SIZE_MAX;
	
	if (!bidirectionalStreamIsReadingPossible(in_out_pBidirectionalStream))
		// Error
		return SIZE_MAX;

	assert(BidirectionalHalfStreamActionNoAction == 
		pStreamStateBidirectionalStream->mHalfStreamStates[pStreamStateBidirectionalStream->mCurrentSide].mAction);

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
		pStreamStateBidirectionalStream->mHalfStreamStates[pStreamStateBidirectionalStream->mCurrentSide].mAction = 
			BidirectionalHalfStreamActionReading;
	}

	out_bytesRead = coroutineStreamRead(in_out_pBidirectionalStream, 
		out_pBuffer, in_count, 
		&pStreamStateBidirectionalStream->mpCurrentBuffer,
		&pStreamStateBidirectionalStream->mCurrentBufferSize);

	if (0 != in_count)
	{
		pStreamStateBidirectionalStream->mHalfStreamStates[pStreamStateBidirectionalStream->mCurrentSide].mAction = 
			BidirectionalHalfStreamActionNoAction;
	}

	return out_bytesRead;
}

size_t bidirectionalStreamWrite(void *in_out_pBidirectionalStream, const void *in_pBuffer, size_t in_count)
{
	BidirectionalStream *pBidirectionalStream = (BidirectionalStream*) in_out_pBidirectionalStream;
	size_t out_bytesWritten;

	assert(0 == pBidirectionalStream->mCurrentSide || 1 == pBidirectionalStream->mCurrentSide);
	
	if (0 != pBidirectionalStream->mCurrentSide && 1 != pBidirectionalStream->mCurrentSide)
		// Error
		return SIZE_MAX;
	
	if (!bidirectionalStreamIsWritingPossible(in_out_pBidirectionalStream))
		// Error
		return SIZE_MAX;

	/*
	* See comment in bidirectionalStreamRead concerning this code block.
	*/
	if (0 != in_count)
	{
		pBidirectionalStream->mHalfStreamStates[pBidirectionalStream->mCurrentSide].mAction = 
			BidirectionalHalfStreamActionWriting;
	}

	/*
	* Q: Why do we set the last parameter to true to reset the buffer to NULL?
	* A: If we would call read(in_count) with in_count > 0 we would try to access a buffer
	*    that already could have become invalid.
	*/
	out_bytesWritten = coroutineStreamWrite(pBidirectionalStream, in_pBuffer, in_count, 
		&pBidirectionalStream->mpCurrentBuffer,
		&pBidirectionalStream->mCurrentBufferSize,
		true);

	if (0 != in_count)
	{
		pBidirectionalStream->mHalfStreamStates[pBidirectionalStream->mCurrentSide].mAction = 
			BidirectionalHalfStreamActionNoAction;
	}

	return out_bytesWritten;
}
