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

void bidirectionalStreamTerminalLoopWrite(void *in_out_pBidirectionalStreamState)
{
	BidirectionalStreamState *pBidirectionalStreamState = (BidirectionalStreamState *) in_out_pBidirectionalStreamState;
	
	if (0 != pBidirectionalStreamState->mCurrentSide && 1 != pBidirectionalStreamState->mCurrentSide)
		return;

	assert(BidirectionalHalfStreamActionNoAction == pBidirectionalStreamState->mHalfStreamStates->mAction);

	pBidirectionalStreamState->mHalfStreamStates[pBidirectionalStreamState->mCurrentSide].mpCurrentBuffer = NULL;
	pBidirectionalStreamState->mHalfStreamStates[pBidirectionalStreamState->mCurrentSide].mCurrentBufferSize = 0;

	(pBidirectionalStreamState->mFunctions.mpfSwitchCoroutine)(in_out_pBidirectionalStreamState);
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

	if (0 != in_count)
	{
		pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mAction = BidirectionalHalfStreamActionReading;
	}

	out_bytesRead = coroutineStreamRead(in_out_pBidirectionalStreamState, 
		out_pBuffer, in_count, 
		&pStreamState->mHalfStreamStates[1-pStreamState->mCurrentSide].mpCurrentBuffer,
		&pStreamState->mHalfStreamStates[1-pStreamState->mCurrentSide].mCurrentBufferSize);

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

	if (0 != in_count)
	{
		pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mAction = BidirectionalHalfStreamActionWriting;
	}

	out_bytesWritten = coroutineStreamWrite(pStreamState, in_pBuffer, in_count, 
		&pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mpCurrentBuffer,
		&pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mCurrentBufferSize);

	if (0 != in_count)
	{
		pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mAction = BidirectionalHalfStreamActionNoAction;
	}

	return out_bytesWritten;
}
