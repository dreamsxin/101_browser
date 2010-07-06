#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "rfc1950/rfc1950.h"
#include <cstring>

void testRfc1950()
{
	/*
	 * Example taken from
	 * http://en.wikipedia.org/w/index.php?title=Adler-32&oldid=366924289#Example
	 */
	char wikipediaString[] = "Wikipedia";
	uint32_t wikipediaStringAdler = adler32((uint8_t*) wikipediaString, strlen(wikipediaString));
	test(wikipediaStringAdler == 0x11E60398ul);
}
