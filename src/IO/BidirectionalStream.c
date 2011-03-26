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
	uint8_t *pBuffer = (uint8_t*) out_pBuffer;
	size_t bytesRead = 0;
	size_t bytesToReadInCurrentIterationCount = MIN(in_count-bytesRead, pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mBufferSize);

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

	if (0 == bytesToReadInCurrentIterationCount)
		goto switch_to_writer;

	while (1)
	{
		// copy bytes
		memcpy(pBuffer, pStreamState->mHalfStreamStates[1-pStreamState->mCurrentSide].mpBuffer, bytesToReadInCurrentIterationCount);

		bytesRead += bytesToReadInCurrentIterationCount;
		pBuffer += bytesToReadInCurrentIterationCount;
		pStreamState->mHalfStreamStates[1-pStreamState->mCurrentSide].mpBuffer += bytesToReadInCurrentIterationCount;
		pStreamState->mHalfStreamStates[1-pStreamState->mCurrentSide].mBufferSize -= bytesToReadInCurrentIterationCount;

		if (pStreamState->mHalfStreamStates[1-pStreamState->mCurrentSide].mBufferSize > 0 || bytesRead == in_count)
			break;


		// switch to writer
switch_to_writer:

		invertAndSwitchCoroutine(pStreamState);

		bytesToReadInCurrentIterationCount = MIN(in_count-bytesRead, 
			pStreamState->mHalfStreamStates[1-pStreamState->mCurrentSide].mBufferSize);

		if (bytesToReadInCurrentIterationCount == 0)
			break;
	}

	if (0 != in_count)
	{
		pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mAction = BidirectionalHalfStreamActionNoAction;
	}

	return bytesRead;
}

size_t bidirectionalStreamWrite(void *in_out_pBidirectionalStreamState, const void *in_pBuffer, size_t in_count)
{
	BidirectionalStreamState *pStreamState = (BidirectionalStreamState*) in_out_pBidirectionalStreamState;

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

	pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mpBuffer = (const uint8_t*) in_pBuffer;
	pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mBufferSize = in_count;
	invertAndSwitchCoroutine(pStreamState);

	if (0 != in_count)
	{
		pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mAction = BidirectionalHalfStreamActionNoAction;
	}

	return in_count - pStreamState->mHalfStreamStates[pStreamState->mCurrentSide].mBufferSize;
}
