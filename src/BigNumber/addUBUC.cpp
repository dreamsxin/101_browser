#include "BigNumber/BigInteger.h"
#include "Template/HigherOrderFunctions/Fold.h"

bool addUBUCKernel(uint32_t* in_pCarry, uint32_t* in_pCurrentDigit)
{
	uint64_t sum = ((uint64_t) *in_pCarry) + ((uint64_t) *in_pCurrentDigit);
	*in_pCurrentDigit = (uint32_t) sum;
	*in_pCarry = (uint32_t) (sum >> 32);

	return (*in_pCarry != 0);
}

uint32_t addUBUC(UnsignedBigInteger* in_pInt, uint32_t in_c)
{
	return applyCarry(in_pInt, 
		MTAx::HigherOrderFunctions::foldlWithModificationAndBreak<uint32_t, uint32_t>(
		&addUBUCKernel, in_c, in_pInt->limbs, in_pInt->numberSize));
}
