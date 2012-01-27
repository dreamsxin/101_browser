/*
* Copyright 2012 Wolfgang Keller
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

#include "Unicode/Parser.h"

ReadResult terminateStream(
	ByteStreamInterface in_writeInterface,
	void *in_pWriteState)
{
	(*in_writeInterface.mpfWrite)(in_pWriteState, NULL, 0);

	return ReadResultOK;
}

ReadResult writeTerminalReplacementCharacter(
	ByteStreamInterface in_writeInterface,
	void *in_pWriteState)
{
	UnicodeCodePoint currentCodePoint = REPLACEMENT_CHARACTER;
	size_t rwCount = (*in_writeInterface.mpfWrite)(in_pWriteState, &currentCodePoint, sizeof(UnicodeCodePoint));

	if (rwCount != sizeof(UnicodeCodePoint))
		return ReadResultWriteError;

	terminateStream(in_writeInterface, in_pWriteState);
	return ReadResultOK;
}
