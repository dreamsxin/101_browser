#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "PNG/PNG.h"
#include "PNG/CRC.h"

void test_PNG_CRC()
{
	uint8_t buffer[] = {'I', 'E', 'N', 'D'};
	uint32_t currentCRC;

	// First a manual computing
	currentCRC = CRC_init();
	currentCRC = CRC_update(currentCRC, buffer[0]);
	currentCRC = CRC_update(currentCRC, buffer[1]);
	currentCRC = CRC_update(currentCRC, buffer[2]);
	currentCRC = CRC_update(currentCRC, buffer[3]);
	currentCRC = CRC_terminate(currentCRC);
	test(currentCRC == 0xAE426082);

	// Now an automatic computation
	currentCRC = CRC_compute(buffer, 4);
	test(currentCRC == 0xAE426082);
}

void test_PNG()
{
	wprintf(L"Testing PNG - CRC\n");
	test_PNG_CRC();
}
