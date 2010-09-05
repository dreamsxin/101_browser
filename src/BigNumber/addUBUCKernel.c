#include "BigNumber/BigInteger.h"

bool addUBUCKernel(uint32_t* in_pCarry, uint32_t* in_pCurrentDigit)
{
	/*!
	 * Notice:
	 * this function can be represented in a very elegant (but slower - why?) way
	 * by the following assembler code:
	 * 
	 *     mov edx, dword ptr [in_pCurrentDigit]
	 *     mov ecx, dword ptr [in_pCarry]
	 * 
	 *     mov eax, dword ptr [ecx]
	 *     add dword ptr [edx], eax
	 * 
	 *     setc al
	 *     mov byte ptr [ecx], al
	 */
	uint64_t sum = ((uint64_t) *in_pCarry) + ((uint64_t) *in_pCurrentDigit);
	*in_pCurrentDigit = (uint32_t) sum;
	*in_pCarry = (uint32_t) (sum >> 32);

	return (*in_pCarry != 0);
}
