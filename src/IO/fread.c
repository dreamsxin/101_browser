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

#include "IO/fread.h"
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/climits.h"
#include <assert.h>

size_t fread_withState(void *in_dstBuffer, size_t in_elementSize, size_t in_count, FILE* in_file,
	void *in_pState, void (*in_pHandler)(void* in_pState, const void* in_pBuffer, size_t in_count))
{
	size_t readCount;
	size_t countIndex;
	
	// Reason for this assertion: the loops won't terminate otherwise
	assert(in_count < SIZE_MAX);

	readCount = fread(in_dstBuffer, in_elementSize, in_count, in_file);

	for (countIndex = 0; countIndex < readCount; countIndex++)
	{
		(*in_pHandler)(in_pState, ((const uint8_t*) in_dstBuffer)+countIndex*in_elementSize, in_elementSize);
	}

	return readCount;
}
