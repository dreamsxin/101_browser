#include <cwchar>
#include <cstdio>
#include <cstdlib>

#include "TestSuite/TestSuite.h"

unsigned long testNr=0;
bool verbose = false;
bool exitOnFailure = true;

void testPassed(const wchar_t *_Message, const wchar_t *_File, unsigned int _Line)
{
	extern unsigned long testNr;
	extern bool verbose;
	if (verbose)
		wprintf(L"Test %u testing %s in %s:%u OK\n", testNr, _Message, _File, _Line);
	else
		wprintf(L"Test %u OK\n", testNr);
	testNr++;
}

void testFailed(const wchar_t *_Message, const wchar_t *_File, unsigned int _Line)
{
	extern unsigned long testNr;
	extern bool exitOnFailure;

	wprintf(L"Test %u testing %s in %s:%u FAILED\n", testNr, _Message, _File, _Line);

	if (exitOnFailure)
		exit(-1);
}

