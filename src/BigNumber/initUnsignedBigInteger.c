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
#include <assert.h>
#include <stdlib.h>

bool initUnsignedBigIntegerUC(UnsignedBigInteger* in_pNumber, uint32_t in_value)
{
	if (in_value != 0)
	{
		uint32_t* allocLimbs = (uint32_t*) malloc(sizeof(uint32_t));

		if (allocLimbs == NULL)
			return false;
		else
		{
			in_pNumber->numberSize = 1;
			in_pNumber->allocedLimbsCount = 1;
			in_pNumber->limbs = allocLimbs;
			in_pNumber->limbs[0] = in_value;
			return true;
		}
	}
	else
	{
		in_pNumber->numberSize = 0;
		in_pNumber->allocedLimbsCount = 0;
		in_pNumber->limbs = NULL;
		return true;
	}
}

bool initUnsignedBigIntegerUCA(UnsignedBigInteger* in_pNumber, 
								  uint32_t* limbs, size_t limbsCount)
{
	size_t idx;

	assert(limbsCount < SIZE_MAX/sizeof(uint32_t));

	{
		uint32_t* allocLimbs = (uint32_t*) malloc(sizeof(uint32_t) * limbsCount);

		if (allocLimbs == NULL)
			return false;
		else
			in_pNumber->limbs = allocLimbs;
	}

	in_pNumber->allocedLimbsCount = limbsCount;
	in_pNumber->numberSize = 0;

	for (idx = 0; idx < limbsCount; idx++)
	{
		if (limbs[idx] != 0)
			in_pNumber->numberSize = idx+1;

		in_pNumber->limbs[idx] = limbs[idx];
	}

	return true;
}
