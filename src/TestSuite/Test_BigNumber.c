#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "BigNumber/BigInteger.h"

void testBigNumber()
{
	/*
	 * Test whether an array of zeros really gets to number 0
	 */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0, 0, 0, 0, 0};

		test(initUnsignedBigInteger(&n, l, 5));
		test(isZero(&n));
	}

	/*
	 * Test whether an zeroes at the end do not mind
	 */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {2, 3, 0, 0, 0};

		test(initUnsignedBigInteger(&n, l, 5));
		test(n.numberSize == 2);
	}

	/*
	 * The most simple overflow case in addition
	 */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0xFFFFFFFF, 0, 0, 0, 0};

		test(initUnsignedBigInteger(&n, l, 5));
		addUBUC(&n, 1);
		test(n.numberSize == 2);

		if (n.numberSize == 2)
		{
			test(n.limbs[0] == 0);
			test(n.limbs[1] == 1);
		}
	}

	/*
	 * A more complicated (ripple carrier) overflow case
	 */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0};

		test(initUnsignedBigInteger(&n, l, 5));
		addUBUC(&n, 1);
		test(n.numberSize == 5);

		if (n.numberSize == 5)
		{
			test(n.limbs[0] == 0);
			test(n.limbs[1] == 0);
			test(n.limbs[2] == 0);
			test(n.limbs[3] == 0);
			test(n.limbs[4] == 1);
		}
	}

	
	/*
	 * Test whether additional memory gets allocated correctly
	 */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

		test(initUnsignedBigInteger(&n, l, 4));
		addUBUC(&n, 1);
		test(n.numberSize == 5);

		if (n.numberSize == 5)
		{
			test(n.limbs[0] == 0);
			test(n.limbs[1] == 0);
			test(n.limbs[2] == 0);
			test(n.limbs[3] == 0);
			test(n.limbs[4] == 1);
		}
	}

	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0xFFFFFFFF, 0xFFFFFFFF, 0, 0, 0};

		test(initUnsignedBigInteger(&n, l, 5));
		mulUBUC(&n, 2);
		test(n.numberSize == 3);

		if (n.numberSize == 3)
		{
			test(n.limbs[0] == 0xFFFFFFFE);
			test(n.limbs[1] == 0xFFFFFFFF);
			test(n.limbs[2] == 1);
		}
	}
}
