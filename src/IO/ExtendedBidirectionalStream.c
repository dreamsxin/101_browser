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

size_t extendedBidirectionalStreamRead(
	void *in_out_pExtendedBidirectionalStreamState, 
	void *out_pBuffer, size_t in_count)
{
	ExtendedBidirectionalStreamState *pExtendedBidirectionalStreamState = 
		(ExtendedBidirectionalStreamState *) in_out_pExtendedBidirectionalStreamState;

	if (pExtendedBidirectionalStreamState->mWhichSideToExtend != pExtendedBidirectionalStreamState->mStreamState.mCurrentSide)
		return bidirectionalStreamRead(&pExtendedBidirectionalStreamState->mStreamState, out_pBuffer, in_count);
	
	return 0;
}

size_t extendedBidirectionalStreamWrite(
	void *in_out_pExtendedBidirectionalStreamState, 
	const void *in_pBuffer, size_t in_count)
{
	ExtendedBidirectionalStreamState *pExtendedBidirectionalStreamState = 
		(ExtendedBidirectionalStreamState *) in_out_pExtendedBidirectionalStreamState;

	if (pExtendedBidirectionalStreamState->mWhichSideToExtend != pExtendedBidirectionalStreamState->mStreamState.mCurrentSide)
		return bidirectionalStreamWrite(&pExtendedBidirectionalStreamState->mStreamState, in_pBuffer, in_count);

	return 0;
}
