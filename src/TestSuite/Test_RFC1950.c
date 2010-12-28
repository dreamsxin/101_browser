#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "RFC1950/RFC1950_adler32.h"
#include "RFC1950/RFC1950_zlib.h"
#include <string.h> // for strlen

void test_RFC1950_adler32()
{
	/*
	 * Test case taken from
	 * http://en.wikipedia.org/w/index.php?title=Adler-32&oldid=366924289#Example
	 */
	char wikipediaString[] = "Wikipedia";
	uint32_t wikipediaStringAdler = RFC1950_adler32_compute((uint8_t*) wikipediaString, strlen(wikipediaString));
	test(wikipediaStringAdler == 0x11E60398ul);
}

void test_RFC1950_zlib()
{
	test(sizeof(ZlibStreamHeader) == 2);
}

void test_RFC1950()
{
	wprintf(L"Testing RFC 1950 - adler32\n");
	test_RFC1950_adler32();
	wprintf(L"Testing RFC 1950 - zlib\n");
	test_RFC1950_zlib();
}
