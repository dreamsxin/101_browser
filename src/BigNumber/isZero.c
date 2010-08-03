#include "BigNumber/BigInteger.h"

uint8_t isZero(UnsignedBigInteger const * const in_pcInt)
{
	return in_pcInt->numberSize == 0;
}
