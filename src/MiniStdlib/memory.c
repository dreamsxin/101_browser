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

#include "MiniStdlib/memory.h"
#include "MiniStdlib/cstdint.h"

void* memset2(void * in_out_ptr, int in_value, size_t in_num)
{
	uint16_t *ptr = (uint16_t *) in_out_ptr;

	while (in_num--)
	{
		/*
		* Note that the postincrement increments the adress the pointer 
		* points to.
		*/
		*ptr++ = in_value;
	}

	return in_out_ptr;
}

void memxchg(void *in_p0, void *in_p1, size_t in_num)
{
	uint8_t *ptr0 = (uint8_t*) in_p0;
	uint8_t *ptr1 = (uint8_t*) in_p1;

	while (in_num--)
	{
		/*
		* Note that the postincrement increments the adress the pointer 
		* points to.
		*/
		uint8_t temp = *ptr0;
		*ptr0++ = *ptr1;
		*ptr1++ = temp;
	}
}
