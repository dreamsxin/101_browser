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

#ifndef _MTAx_Util_ReadResult_h
#define _MTAx_Util_ReadResult_h

typedef enum
{
	ReadResultOK,
	
	/*
	* A special case of ReadResultPrematureEndOfStream when exactly 0 bytes
	* could be read and this could be correct in some cases.
	*/
	ReadResultNullData,
	ReadResultPrematureEndOfStream,
	/*
	* Exactly the opposite of ReadResultPrematureEndOfStream:
	* an end was expected (and is normally necessary to be checked!),
	* but did not occur.
	*/
	ReadResultOverdueEndOfStream,

	ReadResultInvalidData,
	ReadResultAllocationFailure,
	ReadResultNotImplemented,

	/*
	* This normally means that a passed parameter does not
	* comply to the postulated invarint. So it is a sign
	* of a serious bug.
	*/
	ReadResultInternalInconsistency,

	/*
	* Could not write ParseResult
	*/
	ReadResultWriteError,

	/*
	* When we don't know what kind of error it is, we use this. Should
	* be avoided if possible.
	*/
	ReadResultSomeError
} ReadResult;

#endif
