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

#include "Algorithm/Bitmask.h"
#include "MiniStdlib/cstddef.h" // for SIZE_MAX on Unix
#include <assert.h>

bool checkBitmask(const void *in_pBuffer, size_t in_bufferSize, const void *in_pMaskIdentifierBits, const void *in_pMaskValueBits)
{
	const uint8_t *pBuffer = (const uint8_t*) in_pBuffer;
	const uint8_t *pMaskIdentifierBits = (const uint8_t*) in_pMaskIdentifierBits;
	const uint8_t *pMaskValueBits = (const uint8_t*) in_pMaskValueBits;
	size_t index;
	uint8_t currentBit;
	
	for (index = 0; index < in_bufferSize; index++)
	{
		for (currentBit = 0; currentBit < 8; currentBit++)
		{
			if (pMaskIdentifierBits[index] & (1 << currentBit))
			{
				if ((pBuffer[index] & (1 << currentBit)) != (pMaskValueBits[index] & (1 << currentBit)))
				{
					return false;
				}
			}
		}
	}

	return true;
}
