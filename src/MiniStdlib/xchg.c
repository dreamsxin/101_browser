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

#include "MiniStdlib/xchg.h"
#include "MiniStdlib/cstdint.h"
#include <assert.h>
#include <string.h>
#include <limits.h>

void xchg(void *in_p1, void *in_p2, size_t in_dataSize)
{
	uint8_t temp;
	size_t idx;

	for (idx = 0; idx<in_dataSize; idx++)
	{
		temp = ((uint8_t*) in_p1)[idx];
		((uint8_t*) in_p1)[idx] = ((uint8_t*) in_p2)[idx];
		((uint8_t*) in_p2)[idx] = temp;
	}
}

