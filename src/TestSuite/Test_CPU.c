#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "CPU/X86.h"
#include <stdio.h>

void testCPU()
{
	if (Check_CPU_support_AES())
		wprintf(L"AES is supported\n");
	else
		wprintf(L"AES is not supported\n");
}
