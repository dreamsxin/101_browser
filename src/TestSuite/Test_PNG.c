#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "PNG/PNG.h"
#include "PNG/CRC.h"

void test_PNG_CRC()
{
	// First a manual computing
	uint32_t currentCRC = initCRC();
	currentCRC = updateCRC(currentCRC, 'I');
	currentCRC = updateCRC(currentCRC, 'E');
	currentCRC = updateCRC(currentCRC, 'N');
	currentCRC = updateCRC(currentCRC, 'D');
	currentCRC = terminateCRC(currentCRC);

	test(currentCRC == 0xAE426082);
}

void test_PNG()
{
	wprintf(L"Testing PNG - CRC\n");
	test_PNG_CRC();
}
