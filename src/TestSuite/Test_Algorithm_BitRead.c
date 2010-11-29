#include "TestSuite/TestAlgorithm.h"
#include <stdlib.h>
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/MTAx_cstdio.h"

void testAlgorithm_BitRead()
{
	// Source:
	// http://www.random.org/integers/?num=2&min=0&max=15&col=2&base=16&format=plain&rnd=new
	uint8_t bytes2[] = { 0xe4 };

	// Source:
	// http://www.random.org/integers/?num=20&min=0&max=15&col=2&base=16&format=plain&rnd=new
	uint8_t bytes10[] = {
		0x4c,
		0xd7,
		0x86,
		0x4a,
		0x74,
		0xa1,
		0x0c,
		0xa1,
		0x28,
		0xf8
	};
}
