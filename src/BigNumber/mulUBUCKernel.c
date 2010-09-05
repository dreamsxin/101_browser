#include "BigNumber/BigInteger.h"

uint32_t mulUBUCKernel(uint32_t in_carry, uint32_t* in_pCurrentDigit, uint32_t in_n)
{
	uint64_t prod = ((uint64_t) in_n) * ((uint64_t) *in_pCurrentDigit)+(uint64_t) in_carry;
	*in_pCurrentDigit = (uint32_t) prod;

	return (uint32_t) (prod >> 32);
}