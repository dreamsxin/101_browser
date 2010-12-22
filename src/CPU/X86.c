#ifdef _WIN32
#include <intrin.h>
#endif
#include "CPU/X86.h"

// Source: http://wiki.osdev.org/CPUID#Using_CPUID_from_GCC
enum cpuid_requests {
  CPUID_GETVENDORSTRING,
  CPUID_GETFEATURES,
  CPUID_GETTLB,
  CPUID_GETSERIAL,
 
  CPUID_INTELEXTENDED=0x80000000,
  CPUID_INTELFEATURES,
  CPUID_INTELBRANDSTRING,
  CPUID_INTELBRANDSTRINGMORE,
  CPUID_INTELBRANDSTRINGEND,
};

#ifdef __GNUC__
/**
 * issue a complete request, storing general registers output as a string
 */
static inline void cpuid(int code, int where[4])
{
	// Comment is there because of a bug in the gcc code generator
	asm volatile("cpuid":"=a"(*where), /*"=b"(*(where+1)),*/ "=c"(*(where+2)),"=d"(*(where+3)):"0"(code));

}
#endif

int Check_CPU_support_AES()
{
	int CPUInfo[4] = {-1};
#if defined(_WIN32)
	__cpuid(CPUInfo, CPUID_GETFEATURES);
#elif defined(__GNUC__)
	 cpuid(CPUID_GETFEATURES, CPUInfo);
#else
	#error No supported platform
#endif
	
	return (CPUInfo[2] & 0x2000000);
}

