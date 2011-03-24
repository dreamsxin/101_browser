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
#include "Template/HigherOrderFunctions/Fold.h"
#include "MiniStdlib/safe_free.h"

uint32_t mulUBUC(UnsignedBigInteger* in_pInt, uint32_t in_c)
{
	if (in_c != 0)
	{
		return applyCarry(in_pInt, 
			MTAx::HigherOrderFunctions::foldlWithModificationAndParam<uint32_t, uint32_t, uint32_t>(
			&mulUBUCKernel, 0, in_pInt->limbs, in_pInt->numberSize, in_c));
	}
	else
	{
		in_pInt->numberSize = 0;
		in_pInt->allocedLimbsCount = 0;
		safe_free(&in_pInt->limbs);

		return 0;
	}
}
