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

size_t bidirectionalStreamRead(uint8_t in_side, void *in_out_pBidirectionalStreamState, void *out_pBuffer, size_t in_count)
{
	BidirectionalStreamState *pStreamState = (BidirectionalStreamState*) in_out_pBidirectionalStreamState;

	assert(0 == in_side || 1 == in_side);



	return 0;
}

size_t bidirectionalStreamWrite(uint8_t in_side, void *in_out_pBidirectionalStreamState, const void *in_pBuffer, size_t in_count)
{
	BidirectionalStreamState *pStreamState = (BidirectionalStreamState*) in_out_pBidirectionalStreamState;

	assert(0 == in_side || 1 == in_side);


	
	return 0;
}

size_t bidirectionalStreamSide0Read(void *in_out_pBidirectionalStreamState, void *out_pBuffer, size_t in_count)
{
	return bidirectionalStreamRead(0, in_out_pBidirectionalStreamState, out_pBuffer, in_count);
}

size_t bidirectionalStreamSide1Read(void *in_out_pBidirectionalStreamState, void *out_pBuffer, size_t in_count)
{
	return bidirectionalStreamRead(1, in_out_pBidirectionalStreamState, out_pBuffer, in_count);
}

size_t bidirectionalStreamSide0Write(void *in_out_pBidirectionalStreamState, const void *in_pBuffer, size_t in_count)
{
	return bidirectionalStreamWrite(0, in_out_pBidirectionalStreamState, in_pBuffer, in_count);
}

size_t bidirectionalStreamSide1Write(void *in_out_pBidirectionalStreamState, const void *in_pBuffer, size_t in_count)
{
	return bidirectionalStreamWrite(1, in_out_pBidirectionalStreamState, in_pBuffer, in_count);
}
