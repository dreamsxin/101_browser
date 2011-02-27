/*
* Copyright 2008-2011 Wolfgang Keller
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

#ifndef _2_5_4_1_h
#define _2_5_4_1_h

/*
* 2.5.4.1 Non-negative integers
* Living Standard — 7 January 2011
*/

#include "BigNumber/BigInteger.h"
#include "Util/Iterator.h"
#include "Util/ReadResult.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
	__declspec(dllexport)
#endif
/*!
* Return value: ResultOK on success (in this case in_pUnsignedInteger will
*               contain the parsed number)
*               ResultError or ResultAllocationFailure on failure
*/
ReadResult parseNonNegativeInteger(SingleIterator in_iterator, void* in_iteratorState, 
UnsignedBigInteger* in_pUnsignedInteger);

#ifdef __cplusplus
}
#endif

#endif
