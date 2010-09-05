#include "BigNumber/BigInteger.h"
#include "Template/HigherOrderFunctions/Fold.h"

uint32_t addUBUC(UnsignedBigInteger* in_pInt, uint32_t in_c)
{
	return applyCarry(in_pInt, 
		MTAx::HigherOrderFunctions::foldlWithModificationAndBreak<uint32_t, uint32_t>(
		&addUBUCKernel, in_c, in_pInt->limbs, in_pInt->numberSize));
}
