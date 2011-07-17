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
#include "MiniStdlib/cstdint.h" // for uint8_t and uint16_t

void* memset2(void * in_out_pBuffer, int in_value, size_t in_num)
{
	uint16_t *ptr = (uint16_t *) in_out_pBuffer;

	while (in_num--)
	{
		/*
		* Note that the postincrement increments the adress the pointer 
		* points to.
		*/
		*ptr++ = in_value;
	}

	return in_out_pBuffer;
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

void* endianness_convert(void *in_out_pBuffer, size_t in_num)
{
	uint8_t *ptr = (uint8_t *) in_out_pBuffer;
	size_t idx;
	
	for (idx = 0; idx < in_num/2; idx++)
	{
		memxchg(ptr+idx, ptr+in_num-1-idx, 1);
	}

	return in_out_pBuffer;
}
