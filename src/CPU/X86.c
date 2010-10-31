#include <intrin.h>
#include "CPU/X86.h"

int Check_CPU_support_AES()
{
	int CPUInfo[4] = {-1};
	__cpuid(CPUInfo, 0);
	return (CPUInfo[2] & 0x2000000);
}
