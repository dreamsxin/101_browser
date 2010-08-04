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

		/*
		 * We test whether a number stays zero when freed
		 */
		freeUnsignedBigInteger(&n);
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

		/*
		 * We test whether a number gets to zero when freed
		 */
		freeUnsignedBigInteger(&n);
		test(isZero(&n));
	}

	/*
	 * The most simple overflow case in addition
	 */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0xFFFFFFFF, 0, 0, 0, 0};

		test(initUnsignedBigInteger(&n, l, 5));
		test(addUBUC(&n, 1) == 0);
		test(n.numberSize == 2);

		if (n.numberSize == 2)
		{
			test(n.limbs[0] == 0);
			test(n.limbs[1] == 1);
		}

		freeUnsignedBigInteger(&n);
	}

	/*
	 * A more complicated (ripple carrier) overflow case
	 */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0};

		test(initUnsignedBigInteger(&n, l, 5));
		test(addUBUC(&n, 1) == 0);
		test(n.numberSize == 5);

		if (n.numberSize == 5)
		{
			test(n.limbs[0] == 0);
			test(n.limbs[1] == 0);
			test(n.limbs[2] == 0);
			test(n.limbs[3] == 0);
			test(n.limbs[4] == 1);
		}

		freeUnsignedBigInteger(&n);
	}
	
	/*
	 * Test whether additional memory gets allocated correctly
	 */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

		test(initUnsignedBigInteger(&n, l, 4));
		test(addUBUC(&n, 1) == 0);
		test(n.numberSize == 5);

		if (n.numberSize == 5)
		{
			test(n.limbs[0] == 0);
			test(n.limbs[1] == 0);
			test(n.limbs[2] == 0);
			test(n.limbs[3] == 0);
			test(n.limbs[4] == 1);
		}

		freeUnsignedBigInteger(&n);
	}

	/* 
	 * A test for multiplication without overflow 
	 */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0, 0};

		test(initUnsignedBigInteger(&n, l, 5));
		test(mulUBUC(&n, 2) == 0);
		test(n.numberSize == 3);

		if (n.numberSize == 3)
		{
			test(n.limbs[0] == 0xFFFFFFFE);
			test(n.limbs[1] == 0xFFFFFFFE);
			test(n.limbs[2] == 0xFFFFFFFE);
		}

		freeUnsignedBigInteger(&n);
	}

	/* 
	 * A test for multiplication with simple overflow
	 */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0xFFFFFFFF, 0xFFFFFFFE};

		test(initUnsignedBigInteger(&n, l, 5));
		test(mulUBUC(&n, 5) == 0);
		test(n.numberSize == 3);

		if (n.numberSize == 3)
		{
			test(n.limbs[0] == 0xFFFFFFFB);
			test(n.limbs[1] == 0xFFFFFFFA);
			test(n.limbs[2] == 4);
		}

		freeUnsignedBigInteger(&n);
	}

	/* 
	 * A test for multiplication with more complicated overflow 
	 * and need to allocate additional memory
	 */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

		test(initUnsignedBigInteger(&n, l, 5));
		test(mulUBUC(&n, 0xFFFFFFFF) == 0);
		test(n.numberSize == 6);

		if (n.numberSize == 6)
		{
			test(n.limbs[0] == 0x1);
			test(n.limbs[1] == 0xFFFFFFFF);
			test(n.limbs[2] == 0xFFFFFFFF);
			test(n.limbs[3] == 0xFFFFFFFF);
			test(n.limbs[4] == 0xFFFFFFFF);
			test(n.limbs[5] == 0xFFFFFFFE);
		}

		freeUnsignedBigInteger(&n);
	}
}
