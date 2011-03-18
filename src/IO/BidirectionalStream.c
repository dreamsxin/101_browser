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

#include "IO/BidirectionalStream.h"
#include <assert.h>

bool bidirectionalStreamIsReadingPossible(uint8_t in_side, void *in_pBidirectionalStreamState)
{
	BidirectionalStreamState *pStreamState = (BidirectionalStreamState*) in_pBidirectionalStreamState;

	if (in_side != 0 && in_side != 1)
		return false;

	return (BidirectionalHalfStreamActionWriting == pStreamState->mHalfStreamStates[1-in_side].mAction || 
		BidirectionalHalfStreamActionNoAction == pStreamState->mHalfStreamStates[1-in_side].mAction);
}

bool bidirectionalStreamIsWritingPossible(uint8_t in_side, void *in_pBidirectionalStreamState)
{
	BidirectionalStreamState *pStreamState = (BidirectionalStreamState*) in_pBidirectionalStreamState;

	if (in_side != 0 && in_side != 1)
		return false;

	return (BidirectionalHalfStreamActionReading == pStreamState->mHalfStreamStates[1-in_side].mAction || 
		BidirectionalHalfStreamActionNoAction == pStreamState->mHalfStreamStates[1-in_side].mAction);
}

size_t bidirectionalStreamRead(uint8_t in_side, void *in_out_pBidirectionalStreamState, void *out_pBuffer, size_t in_count)
{
	BidirectionalStreamState *pStreamState = (BidirectionalStreamState*) in_out_pBidirectionalStreamState;

	if (in_side != 0 && in_side != 1)
		return 0;

	if (bidirectionalStreamIsReadingPossible(in_side, in_out_pBidirectionalStreamState))
	{
		// TODO...
		return 42;
	}
	else
		return 0;
}

size_t bidirectionalStreamWrite(uint8_t in_side, void *in_out_pBidirectionalStreamState, const void *in_pBuffer, size_t in_count)
{
	BidirectionalStreamState *pStreamState = (BidirectionalStreamState*) in_out_pBidirectionalStreamState;

	if (in_side != 0 && in_side != 1)
		return 0;

	if (bidirectionalStreamIsWritingPossible(in_side, in_out_pBidirectionalStreamState))
	{
		size_t writtenBytes;
		
		pStreamState->mHalfStreamStates[in_side].mpBuffer = (const uint8_t*) in_pBuffer;
		pStreamState->mHalfStreamStates[in_side].mCurrentBufferSize = in_count;
		pStreamState->mHalfStreamStates[in_side].mAction = BidirectionalHalfStreamActionWriting;

		switchToCoroutine(pStreamState->mHalfStreamStates[in_side].mpCoroutineDescriptor, 
			pStreamState->mHalfStreamStates[1-in_side].mpCoroutineDescriptor);
		writtenBytes = in_count - pStreamState->mHalfStreamStates[in_side].mCurrentBufferSize;

		// the first two lines are for additional security...
		pStreamState->mHalfStreamStates[in_side].mpBuffer = NULL;
		pStreamState->mHalfStreamStates[in_side].mCurrentBufferSize = 0;
		pStreamState->mHalfStreamStates[in_side].mAction = BidirectionalHalfStreamActionNoAction;

		return writtenBytes;
	}
	else
		return 0;
}
