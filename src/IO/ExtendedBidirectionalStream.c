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
		if (!bidirectionalStreamIsReadingPossible(in_out_pExtendedBidirectionalStreamState))
		{
			bidirectionalStreamWrite(in_out_pExtendedBidirectionalStreamState, NULL, 0);

			if (!bidirectionalStreamIsReadingPossible(in_out_pExtendedBidirectionalStreamState))
				return SIZE_MAX;
		}

		assert(bidirectionalStreamIsReadingPossible(in_out_pExtendedBidirectionalStreamState));
		pExtendedBidirectionalStreamState->mIsExtendedSideReading = true;
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
		if (!bidirectionalStreamIsWritingPossible(in_out_pExtendedBidirectionalStreamState))
		{
			bidirectionalStreamRead(in_out_pExtendedBidirectionalStreamState, NULL, 0);

			if (!bidirectionalStreamIsWritingPossible(in_out_pExtendedBidirectionalStreamState))
				return SIZE_MAX;
		}

		assert(bidirectionalStreamIsWritingPossible(in_out_pExtendedBidirectionalStreamState));
		pExtendedBidirectionalStreamState->mIsExtendedSideReading = true;
		return bidirectionalStreamWrite(in_out_pExtendedBidirectionalStreamState, in_pBuffer, in_count);
	}
}
