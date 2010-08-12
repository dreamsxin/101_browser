#include "BigNumber/BigInteger.h"
#include "Template/HigherOrderFunctions/Fold.h"
#include "MiniStdlib/safe_free.h"

uint32_t mulUBUCKernel(uint32_t in_carry, uint32_t* in_pCurrentDigit, uint32_t in_n)
{
	uint64_t prod = ((uint64_t) in_n) * ((uint64_t) *in_pCurrentDigit)+(uint64_t) in_carry;
	*in_pCurrentDigit = (uint32_t) prod;

	return (uint32_t) (prod >> 32);
}

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

