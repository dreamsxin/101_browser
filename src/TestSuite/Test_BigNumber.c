#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "BigNumber/BigInteger.h"

void testBigNumber()
{
	/* Test whether a number of zero makes number empty */
	{
		UnsignedBigInteger n;

		test(initUnsignedBigIntegerUC(&n, 0));
		test(isZero(&n));

		/* We test whether a number stays zero when freed */
		freeUnsignedBigInteger(&n);
		test(isZero(&n));
	}

	/* Test whether a number of non-zero makes number non-empty */
	{
		UnsignedBigInteger n;

		test(initUnsignedBigIntegerUC(&n, 42));
		test(!isZero(&n));

		/* We test whether a number stays zero when freed */
		freeUnsignedBigInteger(&n);
		test(isZero(&n));
	}

	/* Test whether an empty array becomes number 0 */
	{
		UnsignedBigInteger n;

		test(initUnsignedBigIntegerUCA(&n, NULL, 0));
		test(isZero(&n));

		/* We test whether a number stays zero when freed */
		freeUnsignedBigInteger(&n);
		test(isZero(&n));
	}

	/* Test whether an empty array becomes number 0 */
	{
		UnsignedBigInteger n;

		uint32_t l[5] = {0, 0, 0, 0, 0};

		test(initUnsignedBigIntegerUCA(&n, l, 0));
		test(isZero(&n));

		/* We test whether a number stays zero when freed */
		freeUnsignedBigInteger(&n);
		test(isZero(&n));
	}

	/* Test whether an array of zeros becomes 0 */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0, 0, 0, 0, 0};

		test(initUnsignedBigIntegerUCA(&n, l, 5));
		test(isZero(&n));

		/* We test whether a number stays zero when freed */
		freeUnsignedBigInteger(&n);
		test(isZero(&n));
	}

	/* Test whether an zeroes at the end do not mind */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {2, 3, 0, 0, 0};

		test(initUnsignedBigIntegerUCA(&n, l, 5));
		test(n.numberSize == 2);

		/* We test whether a number gets to zero when freed */
		freeUnsignedBigInteger(&n);
		test(isZero(&n));
	}

	/* Incrementing zero */
	{
		UnsignedBigInteger n;

		test(initUnsignedBigIntegerUCA(&n, NULL, 0));
		test(n.numberSize == 0);
		test(incUB(&n) == 0);
		test(n.numberSize == 1);
		test(!isZero(&n));

		/* We test whether a number gets to zero when freed */
		freeUnsignedBigInteger(&n);
		test(isZero(&n));
	}

	/* Incrementing without overflow */
	{
		UnsignedBigInteger n;
		uint32_t l[1] = {0x22};

		test(initUnsignedBigIntegerUCA(&n, l, 1));
		test(n.numberSize == 1);
		test(incUB(&n) == 0);
		test(n.numberSize == 1);

		/* We test whether a number gets to zero when freed */
		freeUnsignedBigInteger(&n);
		test(isZero(&n));
	}

	/* Incrementing with overflow */
	{
		UnsignedBigInteger n;
		uint32_t l[1] = {0xFFFFFFFF};

		test(initUnsignedBigIntegerUCA(&n, l, 1));
		test(n.numberSize == 1);
		test(incUB(&n) == 0);
		test(n.numberSize == 2);

		/* We test whether a number gets to zero when freed */
		freeUnsignedBigInteger(&n);
		test(isZero(&n));
	}

	/* The most simple overflow case in addition */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0xFFFFFFFF, 0, 0, 0, 0};

		test(initUnsignedBigIntegerUCA(&n, l, 5));
		test(addUBUC(&n, 1) == 0);
		test(n.numberSize == 2);

		if (n.numberSize == 2)
		{
			test(n.limbs[0] == 0);
			test(n.limbs[1] == 1);
		}

		freeUnsignedBigInteger(&n);
	}

	/* A more complicated (ripple carrier) overflow case */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0};

		test(initUnsignedBigIntegerUCA(&n, l, 5));
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
	
	/* Test whether additional memory gets allocated correctly */
	{
		UnsignedBigInteger n;
		uint32_t l[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

		test(initUnsignedBigIntegerUCA(&n, l, 4));
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

	/* A test for multiplication with zero */
	{
		UnsignedBigInteger n;
		uint32_t l[2] = {0x22, 0x22};

		test(initUnsignedBigIntegerUCA(&n, l, 2));
		test(mulUBUC(&n, 0) == 0);
		test(isZero(&n));

		freeUnsignedBigInteger(&n);
	}

	/* A test for multiplication without overflow */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0, 0};

		test(initUnsignedBigIntegerUCA(&n, l, 5));
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

	/* A test for multiplication with simple overflow */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0xFFFFFFFF, 0xFFFFFFFE};

		test(initUnsignedBigIntegerUCA(&n, l, 5));
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

		test(initUnsignedBigIntegerUCA(&n, l, 5));
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

	/* Testing modulo of 0 */
	{
		UnsignedBigInteger n;
		uint32_t l[1] = { 0x0 };

		test(initUnsignedBigIntegerUCA(&n, l, 1));
		test(modUBUC(&n, 12) == 0);

		freeUnsignedBigInteger(&n);
	}

	/* Testing modulo of a single limb */
	{
		UnsignedBigInteger n;
		uint32_t l[1] = { 99 };

		test(initUnsignedBigIntegerUCA(&n, l, 1));
		test(modUBUC(&n, 10) == 9);

		freeUnsignedBigInteger(&n);
	}

	/* Testing modulo of a longer number against a "small" value */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

		test(initUnsignedBigIntegerUCA(&n, l, 5));
		test(modUBUC(&n, 37) == 8);

		freeUnsignedBigInteger(&n);
	}

	/* Testing modulo of a longer number against a "big" value */
	{
		UnsignedBigInteger n;
		uint32_t l[5] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

		test(initUnsignedBigIntegerUCA(&n, l, 5));
		test(modUBUC(&n, 0xFFFFFFFE) == 0x1F);

		freeUnsignedBigInteger(&n);
	}
}
