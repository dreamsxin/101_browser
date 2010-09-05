#include "BigNumber/BigInteger.h"

uint32_t modUBUCKernel(uint32_t in_carry, uint32_t* in_pCurrentDigit, uint32_t in_n)
{
	uint64_t currentNumber = (((uint64_t) in_carry) << 32)+(uint64_t) *in_pCurrentDigit;
	uint32_t currentMod = (uint32_t) (currentNumber % in_n);
	return currentMod;
}
