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

#ifndef _MTAx_IO_ExtendedBidirectionalStream_h
#define _MTAx_IO_ExtendedBidirectionalStream_h

#include "IO/BidirectionalStream.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
* Invariants:
* INV:IO_ExtendedBidirectionalStream_h_28: 
*      mIsExtendedSideReading => mStreamState.mHalfStreamStates[mWhichSideToExtend].mAction != 
*/
typedef struct
{
	BidirectionalStream mBidirectionalStream;
	//! which side shall get the extended behaviour
	uint8_t mWhichSideToExtend;
	/*!
	* mIsExtendedSideReading is true if the side having the extended behaviour
	* is in the "reading loop" and false if it is in the "writing loop".
	*/
	bool mIsExtendedSideReading;
} ExtendedBidirectionalStreamState;


DLLEXPORT size_t extendedBidirectionalStreamRead(
	void *in_out_pExtendedBidirectionalStreamState, 
	void *out_pBuffer, size_t in_count);

DLLEXPORT size_t extendedBidirectionalStreamWrite(
	void *in_out_pExtendedBidirectionalStreamState, 
	const void *in_pBuffer, size_t in_count);

#ifdef __cplusplus
}
#endif

#endif
