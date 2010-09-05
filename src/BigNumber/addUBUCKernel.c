#include "BigNumber/BigInteger.h"

bool addUBUCKernel(uint32_t* in_pCarry, uint32_t* in_pCurrentDigit)
{
#ifdef _M_IX86
	__asm
	{
		mov edx, dword ptr [in_pCurrentDigit]
		mov ecx, dword ptr [in_pCarry]

		mov eax, dword ptr [ecx]
		add dword ptr [edx], eax

		setc al
		mov byte ptr [ecx], al
	}
#else
	uint64_t sum = ((uint64_t) *in_pCarry) + ((uint64_t) *in_pCurrentDigit);
	*in_pCurrentDigit = (uint32_t) sum;
	*in_pCarry = (uint32_t) (sum >> 32);

	return (*in_pCarry != 0);
#endif
}
