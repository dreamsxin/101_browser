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

#include "IO/ExtendedBidirectionalStream.h"
#include <assert.h>

size_t extendedBidirectionalStreamRead(
	void *in_out_pExtendedBidirectionalStreamState, 
	void *out_pBuffer, size_t in_count)
{
	ExtendedBidirectionalStreamState *pExtendedBidirectionalStreamState = 
		(ExtendedBidirectionalStreamState *) in_out_pExtendedBidirectionalStreamState;

	if (pExtendedBidirectionalStreamState->mStreamState.mCurrentSide != pExtendedBidirectionalStreamState->mWhichSideToExtend)
	{
		if (pExtendedBidirectionalStreamState->mIsExtendedSideReading)
			return SIZE_MAX;
		else
		{
			/*
			* TODO: From where does this invariant follow?
			*/
			assert(bidirectionalStreamIsReadingPossible(in_out_pExtendedBidirectionalStreamState));

			return bidirectionalStreamRead(in_out_pExtendedBidirectionalStreamState, out_pBuffer, in_count);
		}
	}
	else
	{
		/*
		* Q: Why do we already set this flag here and not further down
		*    when calling bidirectionalStreamRead?
		* A: If we have to call bidirectionalStreamWrite and after this the after coroutine
		*    calls extendedBidirectionalStreamWrite, this call could become blocked because
		*    mIsExtendedSideReading could be false.
		*/
		// LBL:ExtendedBidirectionalStream_c_50
		pExtendedBidirectionalStreamState->mIsExtendedSideReading = true;
		
		if (!bidirectionalStreamIsReadingPossible(in_out_pExtendedBidirectionalStreamState))
		{
			// LBL:ExtendedBidirectionalStream_c_55
			bidirectionalStreamWrite(in_out_pExtendedBidirectionalStreamState, NULL, 0);
		}

		/*
		* Q: Why does this assertion hold?
		* A: If LBL:ExtendedBidirectionalStream_c_55 was never reached, it obviously holds.
		*    Now we have to consider the case that it was reached.
		*    The !bidirectionalStreamIsReadingPossible(...) can only not hold if
		*    pExtendedBidirectionalStreamState->mStreamState.mHalfStreamStates[pExtendedBidirectionalStreamState->mStreamState.mCurrentSide].mAction 
		*    == BidirectionalHalfStreamActionReading
		*    But this is only set if extendedBidirectionalStreamWrite was called.
		*    Since such a call gets blocked because true == pExtendedBidirectionalStreamState->mIsExtendedSideReading
		*    (because of LBL:ExtendedBidirectionalStream_c_50 this is fulfilled) we get the assertion.
		*/
		assert(bidirectionalStreamIsReadingPossible(in_out_pExtendedBidirectionalStreamState));
		
		return bidirectionalStreamRead(in_out_pExtendedBidirectionalStreamState, out_pBuffer, in_count);
	}
}

size_t extendedBidirectionalStreamWrite(
	void *in_out_pExtendedBidirectionalStreamState, 
	const void *in_pBuffer, size_t in_count)
{
	ExtendedBidirectionalStreamState *pExtendedBidirectionalStreamState = 
		(ExtendedBidirectionalStreamState *) in_out_pExtendedBidirectionalStreamState;

	if (pExtendedBidirectionalStreamState->mStreamState.mCurrentSide != pExtendedBidirectionalStreamState->mWhichSideToExtend)
	{
		if (!pExtendedBidirectionalStreamState->mIsExtendedSideReading)
			return SIZE_MAX;
		else
		{
			/*
			* TODO: From where does this invariant follow?
			*/
			assert(bidirectionalStreamIsWritingPossible(in_out_pExtendedBidirectionalStreamState));

			return bidirectionalStreamWrite(in_out_pExtendedBidirectionalStreamState, in_pBuffer, in_count);
		}
	}
	else
	{
		/*
		* Q: Why do we already unset this flag here and not further down
		*    when calling bidirectionalStreamWrite?
		* A: If we have to call bidirectionalStreamRead and after this the after coroutine
		*    calls extendedBidirectionalStreamRead, this call could become blocked because
		*    mIsExtendedSideReading could be false.
		*/
		// LBL:ExtendedBidirectionalStream_c_106
		pExtendedBidirectionalStreamState->mIsExtendedSideReading = false;

		if (!bidirectionalStreamIsWritingPossible(in_out_pExtendedBidirectionalStreamState))
		{
			// LBL:ExtendedBidirectionalStream_c_111
			bidirectionalStreamRead(in_out_pExtendedBidirectionalStreamState, NULL, 0);
		}

		/*
		* Q: Why does this assertion hold?
		* A: If LBL:ExtendedBidirectionalStream_c_111 was never reached, it obviously holds.
		*    Now we have to consider the case that it was reached.
		*    The !bidirectionalStreamIsWritingPossible(...) can only not hold if
		*    pExtendedBidirectionalStreamState->mStreamState.mHalfStreamStates[pExtendedBidirectionalStreamState->mStreamState.mCurrentSide].mAction 
		*    == BidirectionalHalfStreamActionWriting
		*    But this is only set if extendedBidirectionalStreamRead was called.
		*    Since such a call gets blocked because false == pExtendedBidirectionalStreamState->mIsExtendedSideReading
		*    (because of LBL:ExtendedBidirectionalStream_c_106 this is fulfilled) we get the assertion.
		*/
		assert(bidirectionalStreamIsWritingPossible(in_out_pExtendedBidirectionalStreamState));
		
		return bidirectionalStreamWrite(in_out_pExtendedBidirectionalStreamState, in_pBuffer, in_count);
	}
}
