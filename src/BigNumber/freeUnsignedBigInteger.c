#include "BigNumber/BigInteger.h"
#include "MiniStdlib/safe_free.h"

void freeUnsignedBigInteger(UnsignedBigInteger* in_pNumber)
{
	in_pNumber->numberSize = 0;
	in_pNumber->allocedLimbsCount = 0;
	safe_free(&in_pNumber->limbs);
}
