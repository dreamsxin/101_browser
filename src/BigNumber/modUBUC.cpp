#include "BigNumber/BigInteger.h"
#include "Template/HigherOrderFunctions/Fold.h"

uint32_t modUBUCKernel(uint32_t in_carry, uint32_t* in_pCurrentDigit, uint32_t in_n)
{
	uint64_t currentNumber = (((uint64_t) in_carry) << 32)+(uint64_t) *in_pCurrentDigit;
	uint32_t currentMod = (uint32_t) (currentNumber % in_n);
	return currentMod;
}

uint32_t modUBUC(UnsignedBigInteger const * const  in_pInt, uint32_t in_c)
{
	return MTAx::HigherOrderFunctions::foldlWithModificationAndParam<uint32_t, uint32_t, uint32_t>(
		&modUBUCKernel, 0, in_pInt->limbs, in_pInt->numberSize, in_c);
}
