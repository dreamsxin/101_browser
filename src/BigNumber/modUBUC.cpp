#include "BigNumber/BigInteger.h"
#include "Template/HigherOrderFunctions/Fold.h"

uint32_t modUBUCKernel(uint32_t in_carry, uint32_t* in_pCurrentDigit, uint32_t in_n)
{
	uint64_t currentNumber = (uint64_t) in_carry + (((uint64_t) *in_pCurrentDigit) << 32);
	uint32_t currentMod = (uint32_t) (currentNumber % in_n);
	return currentMod;
}

uint32_t modUBUC(UnsignedBigInteger const * const  in_pInt, uint32_t in_c)
{
	if (in_pInt->numberSize == 0)
	{
		return 0;
	}
	else if (in_pInt->numberSize == 1)
	{
		return in_pInt->limbs[0] % in_c;
	}
	return MTAx::HigherOrderFunctions::foldlWithModificationAndParam<uint32_t, uint32_t, uint32_t>(
		&modUBUCKernel, in_pInt->limbs[0], in_pInt->limbs+1, in_pInt->numberSize-1, in_c);
}
