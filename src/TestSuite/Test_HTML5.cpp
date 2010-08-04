#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "HTML5/2_4_5.h"
#include <cassert>

void testHTML5()
{
	// 2.4.5
	UnsignedBigInteger year;

	test(initUnsignedBigIntegerUC(&year, 1900));
	test(numberOfDaysInMonthOfYear(2, year) == 28);
	freeUnsignedBigInteger(&year);

	test(initUnsignedBigIntegerUC(&year, 1999));
	test(numberOfDaysInMonthOfYear(1, year) == 31);
	test(numberOfDaysInMonthOfYear(2, year) == 28);
	test(numberOfDaysInMonthOfYear(3, year) == 31);
	test(numberOfDaysInMonthOfYear(4, year) == 30);
	test(numberOfDaysInMonthOfYear(5, year) == 31);
	test(numberOfDaysInMonthOfYear(6, year) == 30);
	test(numberOfDaysInMonthOfYear(7, year) == 31);
	test(numberOfDaysInMonthOfYear(8, year) == 31);
	test(numberOfDaysInMonthOfYear(9, year) == 30);
	test(numberOfDaysInMonthOfYear(10, year) == 31);
	test(numberOfDaysInMonthOfYear(11, year) == 30);
	test(numberOfDaysInMonthOfYear(12, year) == 31);
	freeUnsignedBigInteger(&year);

	test(initUnsignedBigIntegerUC(&year, 2000));
	test(numberOfDaysInMonthOfYear(2, year) == 29);
	freeUnsignedBigInteger(&year);

	test(initUnsignedBigIntegerUC(&year, 2004));
	test(numberOfDaysInMonthOfYear(2, year) == 29);
	freeUnsignedBigInteger(&year);

	assert('/' < '0');
	test(!isDigit('/'));
	test(isDigit('0'));
	test(isDigit('5'));
	test(isDigit('9'));
	assert(':' > '9');
	test(!isDigit(':'));
}
