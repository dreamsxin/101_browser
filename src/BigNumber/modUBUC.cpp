#include "BigNumber/BigInteger.h"
#include "Template/HigherOrderFunctions/Fold.h"

uint32_t modUBUC(UnsignedBigInteger const * const  in_pInt, uint32_t in_c)
{
	return MTAx::HigherOrderFunctions::foldlWithModificationAndParam<uint32_t, uint32_t, uint32_t>(
		&modUBUCKernel, 0, in_pInt->limbs, in_pInt->numberSize, in_c);
}
