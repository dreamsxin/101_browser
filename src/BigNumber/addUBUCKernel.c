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

#include "BigNumber/BigInteger.h"

bool addUBUCKernel(uint32_t* in_pCarry, uint32_t* in_pCurrentDigit)
{
	/*!
	 * Notice:
	 * this function can be represented in a very elegant (but slower - why?) way
	 * by the following assembler code:
	 * 
	 *     mov edx, dword ptr [in_pCurrentDigit]
	 *     mov ecx, dword ptr [in_pCarry]
	 * 
	 *     mov eax, dword ptr [ecx]
	 *     add dword ptr [edx], eax
	 * 
	 *     setc al
	 *     mov byte ptr [ecx], al
	 */
	uint64_t sum = ((uint64_t) *in_pCarry) + ((uint64_t) *in_pCurrentDigit);
	*in_pCurrentDigit = (uint32_t) sum;
	*in_pCarry = (uint32_t) (sum >> 32);

	return (*in_pCarry != 0);
}
